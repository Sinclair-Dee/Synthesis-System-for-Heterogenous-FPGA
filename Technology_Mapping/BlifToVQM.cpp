#include "BlifToVQM.h"
#include <iostream>
#include <fstream>
#include"time.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::cerr;

BlifToVQM::BlifToVQM(string fromFile, string toFile, string vqmFile, string infoFilePath)
{
	this->fromFile = fromFile;
	this->toFile = toFile;
	this->vqmFile = vqmFile;
	this->infoFilePath = infoFilePath;
	this->pllInfo = "comet_pll.info";
	this->ramInfo = "comet_m4k.info";
	this->stateCount = 0;
	this->scopeState = NONE;
	this->curLUT = NULL;
	this->topModel = NULL;
	this->topModelName = "";
	this->curModelName = "";
	this->curSubcktType = "";
	this->curSubcktName = "";
	this->curSubcktModelName = "";
	this->vqmInfo = "";
	this->vsion = "2.6";
	this->family = "Comet 02G";

	this->lutOnlyLECount = 0;
	this->lutRegLECount = 0;
	this->regsLECount = 0;
	this->regOnlyLECount = 0;
	this->LECount = 0;
	this->dspCount = 0;
	this->pllCount = 0;
	this->memCount = 0;

	this->leCnt = 18240;
	this->pllCnt = 8;
	this->dspCnt = 96;
	this->memCnt = 192;
	//this->showInfo();
	try
	{
		this->parseInfoFile(pllInfo);
		this->parseInfoFile(ramInfo);
		this->parseFile();
	}
	catch (BlifToVQM::Err)
	{
		throw Err();
	}

	if (topModel != NULL)
	{
		this->setTopModel(subcktModelsInfo);
		this->printVQM();
		this->printReport();
	} 

}

BlifToVQM::~BlifToVQM(void)
{	
	this->topModel = NULL;
	this->curLUT = NULL;

	// Deconstruct the models
	for(vector<Model *>::const_iterator it = models.begin(); 
		it != models.end(); it++){
			if (*it != NULL)
			{
				delete *it;
			}
	}

	for (map<string, map<string, SubcktModelInfo *> *>::const_iterator it = subcktModelsInfo.begin(); 
		it != subcktModelsInfo.end(); it++) {
			if (it->second != NULL)
			{
				for (map<string, SubcktModelInfo *>::const_iterator ito = it->second->begin(); 
					ito != it->second->end(); ito++) {
						if (ito->second != NULL)
						{
							delete ito->second;
						}
				}

				delete it->second;
			}
	}

	for (map<string, vector<string> *>::const_iterator it = infoFiles.begin(); 
		it != infoFiles.end(); it++) {
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

}

const string BlifToVQM::REG_NAME = "fd";
const string BlifToVQM::REG_TYPE = "Reg";
const string BlifToVQM::LATCH_NAME = "latch";
const string BlifToVQM::LATCH_TYPE = "Latch";
const string BlifToVQM::LUT_TYPE = "LUT";
const string BlifToVQM::PLL_NAME = "comet_pll";
const string BlifToVQM::PLL_TYPE = "PLL";
const string BlifToVQM::RAM_NAME = "mem_";
const string BlifToVQM::RAM_TYPE = "RAM";
const string BlifToVQM::MODEL_TYPE= "Model";

void BlifToVQM::addSubcktModelInfo(string subcktName, string subcktModelName)
{
	if (subcktModelsInfo.count(subcktName) == 0){
		subcktModelsInfo[subcktName] = new map<string, SubcktModelInfo *>;
	}

	(*subcktModelsInfo[subcktName])[subcktModelName] = 
		new SubcktModelInfo(subcktName, subcktModelName);
}

string BlifToVQM::getVqmInfo(void)
{
	struct tm t1;
	time_t now;
	//char buff[30] = {0};
	char tempDateTime[512] = {0};
	time(&now);
	_localtime64_s(&t1, &now);
	//asctime_s(buff, sizeof(buff), &t1);

	sprintf(tempDateTime, "%d-%d-%d %d:%d:%d", 
		t1.tm_year - 100 + 2000, t1.tm_mon + 1, t1.tm_mday, 
		t1.tm_hour, t1.tm_min, t1.tm_sec);

	string copyright = "// Copyright (C) IECAS\n\n";
	string vendor = 
		"// VENDOR \"中国科学院电子学研究所可编程芯片与系统实验室\"\n";
	string program = "// PROGRAM \"Passkey\"\n";
	string version = "// VERSION \"Version " + vsion + "\"\n\n";

	string dateTime(tempDateTime);
	string date = "// DATE \""+ dateTime +"\"\n";

	return copyright + vendor + program + version + date;
}

void BlifToVQM::parseFile(void)
{
	const string ptn = " "; 
	string str;
	vector<string> vStr;

	int nameCyc = 0;

	ifstream icin(fromFile);
	//ofstream ocout(toFile);

	if (icin.is_open()) {

		//		cout << "Open!" << endl;

		while(getline(icin, str)){
			str = this->trim(str);

			if(str.empty() || str.at(0) == '#'){
				continue;
			}

			vStr = this->split(str, ptn);

			for(vector<string>::const_iterator it = vStr.begin(); 
				it != vStr.end(); it++ ){

					string st = *it;
					if(!st.compare("\\")) {
						vStr.erase(it--);
						continue;
					}

					if (!st.empty()){
						// Parsing...
						//if (ocout.is_open()){
						//	ocout << st << endl;
						//}

						if(!st.compare(".model")){
							stateCount = 0;
							scopeState = MODEL;
						}else if(!st.compare(".inputs")){
							stateCount = 0;
							scopeState = INPUTS;
						}else if(!st.compare(".outputs")){
							stateCount = 0;
							scopeState = OUTPUTS;
						}else if(!st.compare(".subckt")){
							stateCount = 0;
							scopeState = SUBCKT;
						}else if(!st.compare(".names")){
							stateCount = 0;
							nameCyc = 0;
							curLUT = NULL;
							scopeState = NAMES;
						}else if(!st.compare(".blackbox")){
							stateCount = 0;
							scopeState = BLACKBOX;
						}else if(!st.compare(".end")){
							stateCount = 0;
							scopeState = END;
							parseEnd();
						}else{
							stateCount++;
							switch (scopeState)
							{
							case NONE:
								parseNone();
								break;
							case MODEL:
								parseModel(st);
								break;
							case INPUTS:
								parseInputs(st);
								break;
							case OUTPUTS:
								parseOutputs(st);
								break;
							case SUBCKT:
								parseSubckt(st);
								break;
							case NAMES:
								parseNames(st, nameCyc);
								break;
							case BLACKBOX:
								parseBlackbox();
								break;
							case END:							
								break;
							}
						}
					}
			}
		}

		icin.close();
		//ocout.close();
	}else{

		cerr << "Error: " << "File " + fromFile + " cannot be found!\n";
		//exit(0);
		throw Err();
	}
}

void BlifToVQM::parseInfoFile(string pllRamInfoFile) {

	ifstream icin(infoFilePath + "\\" + pllRamInfoFile);

	if (icin.is_open()) {

		string str = "";
		string flagIP = "";

		vector<string> * pllVct = new vector<string>();
		vector<string> * ramVct = new vector<string>();

		while (getline(icin, str)) {

			str = this->trim(str);

			if (str.empty())
			{
				continue;
			}

			if (str.at(0) == '#') {

				if (!str.substr(1).compare("comet_pll")) {
					flagIP = "PLL";

					if (infoFiles.count("PLL") == 0) {
						infoFiles["PLL"] = pllVct;
					}

				} else if (!str.substr(1).compare("comet_m4k")) {

					flagIP = "RAM";

					if (infoFiles.count("RAM") == 0) {
						infoFiles["RAM"] = ramVct;
					}
				}

			} else {

				if (!flagIP.compare("PLL"))	{
					pllVct->push_back(str);
				} else if (!flagIP.compare("RAM")) {
					ramVct->push_back(str);
				}
			}
		}

		icin.close();

	} else {

		//cerr << "Error: " << "File " + infoFilePath + " cannot be found!\n";

	}

}

void BlifToVQM::parseNone(void)
{

}

void BlifToVQM::parseModel(string str)
{
	if (stateCount == 1) {

		if (!str.substr(0, REG_NAME.length()).compare(REG_NAME)) {

			curModelName = str;
			//(*subcktModelsInfo["Reg"])[curModelName] = 
			//	new SubcktModelInfo("Reg", curModelName);
			this->addSubcktModelInfo(REG_TYPE, curModelName);

		} else if (!str.substr(0, LATCH_NAME.length()).compare(LATCH_NAME)) {

			curModelName = str;
			this->addSubcktModelInfo(LATCH_TYPE, curModelName);

		} else if (!str.substr(0, PLL_NAME.length()).compare(PLL_NAME)) {

			curModelName = str;
			this->addSubcktModelInfo(PLL_TYPE, curModelName);

		} else if (!str.substr(0, RAM_NAME.length()).compare(RAM_NAME)) {

			curModelName = str;
			this->addSubcktModelInfo(RAM_TYPE, curModelName);

		} else {

			models.push_back(topModel = new Model(str, MODEL_TYPE, infoFilePath));
			topModel->addInPort(*(new OutPort(topModel, "vcc", "vcc")));
			topModel->addInPort(*(new OutPort(topModel, "gnd", "gnd")));
			topModelName = str;
			curModelName = str;

		}
	}

	/*for(vector<Model *>::const_iterator it = models.begin(); it != models.end(); it++)
	cout << (*it)->getName() << endl;*/
}

void BlifToVQM::parseInputs(string str)
{

	if (stateCount > 0) { 

		if (!curModelName.compare(topModelName)) {

			topModel->addInPort(*(new OutPort(topModel, str, str)));

			/*map<string, InPort *> tempInputs = curModel -> getInputs();
			for (map<string, InPort *>::const_iterator it = tempInputs.begin(); 
			it != tempInputs.end(); it++)
			cout << (*it).second -> getRealName() << endl;*/
		} else if (!curModelName.substr(0, REG_NAME.length()).compare(REG_NAME)) {

			(*subcktModelsInfo[REG_TYPE])[curModelName]->addSubcktInports(str);

		} else if (!curModelName.substr(0, LATCH_NAME.length()).compare(LATCH_NAME)) {

			(*subcktModelsInfo[LATCH_TYPE])[curModelName]->addSubcktInports(str);

		} else if (!curModelName.substr(0, PLL_NAME.length()).compare(PLL_NAME)) {

			(*subcktModelsInfo[PLL_TYPE])[curModelName]->addSubcktInports(str);

		} else if (!curModelName.substr(0, RAM_NAME.length()).compare(RAM_NAME)) {

			(*subcktModelsInfo[RAM_TYPE])[curModelName]->addSubcktInports(str);

		}
	}
}

void BlifToVQM::parseOutputs(string str)
{
	if (stateCount > 0) { 

		if (!curModelName.compare(topModelName)) {

			topModel->addOutPort(*(new InPort(topModel, str, str)));

			/*map<string, OutPort *> tempOutputs = curModel -> getOutputs();
			for (map<string, OutPort *>::const_iterator it = tempOutputs.begin(); 
			it != tempOutputs.end(); it++)
			cout << (*it).second -> getRealName() << endl;*/

		} else if (!curModelName.substr(0, REG_NAME.length()).compare(REG_NAME)) {

			(*subcktModelsInfo[REG_TYPE])[curModelName]->addSubcktOutports(str);

		} else if (!curModelName.substr(0, LATCH_NAME.length()).compare(LATCH_NAME)) {

			(*subcktModelsInfo[LATCH_TYPE])[curModelName]->addSubcktOutports(str);

		} else if (!curModelName.substr(0, PLL_NAME.length()).compare(PLL_NAME)) {

			(*subcktModelsInfo[PLL_TYPE])[curModelName]->addSubcktOutports(str);

		} else if (!curModelName.substr(0, RAM_NAME.length()).compare(RAM_NAME)) {

			(*subcktModelsInfo[RAM_TYPE])[curModelName]->addSubcktOutports(str);

		}
	}
}

void BlifToVQM::parseSubckt(string str)
{
	if(stateCount > 0){

		if (stateCount == 1) { 

			if (!str.substr(0, REG_NAME.length()).compare(REG_NAME) && isdigit(str.at(str.size() - 1))) {

				curSubcktModelName = str;
				curSubcktType = REG_TYPE;
				curSubcktName = topModel->genLutRegLatchName(curSubcktType);
				topModel->addSubckt(curSubcktType, curSubcktModelName, curSubcktName);

			} else if (!str.substr(0, LATCH_NAME.length()).compare(LATCH_NAME) && isdigit(str.at(str.size() - 1))) {

				curSubcktModelName = str;
				curSubcktType = LATCH_TYPE;
				curSubcktName = topModel->genLutRegLatchName(curSubcktType);
				topModel->addSubckt(curSubcktType, curSubcktModelName, curSubcktName);

			} else if (!str.substr(0, PLL_NAME.length()).compare(PLL_NAME)) {

				curSubcktModelName = str;
				curSubcktType = PLL_TYPE;
				curSubcktName = topModel->genPllRamName(curSubcktType);
				if (infoFiles.count("PLL") != 0)
				{
					curSubcktName = curSubcktName + "++++" 
						+ infoFiles["PLL"]->at(topModel->getPllCount() - 1);
				}
				topModel->addSubckt(curSubcktType, curSubcktModelName, curSubcktName);

			} else if (!str.substr(0, RAM_NAME.length()).compare(RAM_NAME)) {

				curSubcktModelName = str;
				curSubcktType = RAM_TYPE;
				curSubcktName = topModel->genPllRamName(curSubcktType);
				curSubcktName = curSubcktName + "++++" 
					+ infoFiles["RAM"]->at(topModel->getRamCount() - 1);
				topModel->addSubckt(curSubcktType, curSubcktModelName, curSubcktName);

			}

		}

		if (stateCount > 1){
			string portName = this->split(str, "=").at(0);
			//portName = Reg.getPortName(portName);
			string realName = this->split(str, "=").at(1);
			realName = this->straightLineAway(realName);
			topModel->addInfoToSubckt(curSubcktType, curSubcktModelName, curSubcktName, portName, realName);
		}

	}
}

void BlifToVQM::parseNames(string str, int &nameCyc)
{

	if(stateCount > 0){

		if (!curModelName.compare(topModelName))
		{
			if(stateCount == 1){
				topModel->addLUT(*(curLUT = 
					new LUT(topModel, topModel->genLutRegLatchName(LUT_TYPE), LUT_TYPE)));
			}

			if (isdigit(str.at(0)) || str.at(0) == '-')
			{

				if (curLUT->getInoutPortRealNameCount() > 1)
				{
					if(nameCyc%2 == 0){
						curLUT->addInTrueTable(str);
					}

					if(nameCyc == 1 && str.size() == 1){
						curLUT->setOutFormat(str.at(0));
					}

					nameCyc++;
				}else if(str.size() == 1){
					curLUT->addInTrueTable("----");
					curLUT->setOutFormat(str.at(0));
				}

				/*cout << str[0] << endl;*/
				//vector<string> inoutPortRealName = curLUT->getInoutPortRealName();

				//for (vector<string>::const_iterator it = inoutPortRealName.begin(); 
				//	it != inoutPortRealName.end(); it++)
				//	cout << (*it) << endl;

			} else{
				curLUT->addInoutPortRealName(this->straightLineAway(str));
			}

			/*map<string, LUT *> tempLuts = curModel -> getLuts();
			for (map<string, LUT *>::const_iterator it = tempLuts.begin(); 
			it != tempLuts.end(); it++)
			cout << (*it).second -> getName() << endl;*/
		}

	}
}

void BlifToVQM::parseBlackbox(void)
{
	if (stateCount > 0) { 
		if (!curModelName.compare(topModelName)) {

		} else if (!curModelName.substr(0, REG_NAME.length()).compare(REG_NAME)) {
			(*(subcktModelsInfo[REG_TYPE]))[curModelName]->setBlackbox(true);

		} else if (!curModelName.substr(0, LATCH_NAME.length()).compare(LATCH_NAME)) {
			(*(subcktModelsInfo[LATCH_TYPE]))[curModelName]->setBlackbox(true);

		} else if (!curModelName.substr(0, PLL_NAME.length()).compare(PLL_NAME)) {
			(*(subcktModelsInfo[PLL_TYPE]))[curModelName]->setBlackbox(true);

		} else if (!curModelName.substr(0, RAM_NAME.length()).compare(RAM_NAME)) {
			(*(subcktModelsInfo[RAM_TYPE]))[curModelName]->setBlackbox(true);
		}
	}
}

void BlifToVQM::parseEnd(void)
{
	curLUT = NULL;
	stateCount = 0;
}

void BlifToVQM::printReport(void) const
{
	string reportPath = infoFilePath + "\\resource.rpt";
	ofstream oRptCout(reportPath);

	if (oRptCout.is_open()) {

		oRptCout << "Fitter Status\t\t\t" << "Successful" << endl;
		oRptCout << "PASSKEY Version\t\t\t" << vsion << endl;
		oRptCout << "Top-level Entity Name\t\t" << topModel->getName() << endl;
		oRptCout << "Family\t\t\t\t" << this->family << endl;
		oRptCout << "Total logic elements\t\t" << this->LECount << " / " << this->leCnt << " (" << (this->LECount * 100 / this->leCnt) << " %)" << endl;
		oRptCout << "Total memories\t\t\t" << this->memCount << " / " << this->memCnt << " (" << (this->memCount * 100 / this->memCnt) << " %)" << endl;
		oRptCout << "Total DSPs\t\t\t" << this->dspCount << " / " << this->dspCnt << " (" << (this->dspCount * 100 / this->dspCnt) << " %)" << endl;
		oRptCout << "Total PLLs\t\t\t" << this->pllCount << " / " << this->pllCnt << " (" << (this->pllCount * 100 / this->pllCnt) << " %)" << endl;
		oRptCout << "Total pins\t\t\t" << "0 / 222 (0 %)" << endl;
		oRptCout << "Total LUT-only LCs\t\t" << this->lutOnlyLECount << endl;
		oRptCout << "Total Register LCs\t\t" << this->regsLECount << endl;
		oRptCout << "Total Register-only LCs\t\t" << this->regOnlyLECount << endl;
		oRptCout << "Total LUT/Register LCs\t\t" << this->lutRegLECount << endl;

		oRptCout.close();

	} else {
		cerr << "Error: Source.rpt print failed..." << endl;
	}
}

void BlifToVQM::printVQM(void)
{
	ofstream ocout(vqmFile);

	if (ocout.is_open()){

		// ----------------------------------------------------   Info   -------------------------------------------------------

		ocout << this->getVqmInfo() << endl;

		// --------------------------------------------------  Module ------------------------------------------------------

		ocout << "module " << topModel->getName() << " (" << endl;

		map<string, InPort *> &tempOutportsPseudoInports = topModel->getOutportsPseudoInports();
		map<string, OutPort *> &tempInportsPseudoOutports = topModel->getInportsPseudoOutports();

		map<string, int> &tempInportStatus =  topModel->getInportStatus();
		map<string, int> &tempOutportStatus = topModel->getOutportStatus();

		for(map<string, int>::const_iterator it = tempOutportStatus.begin(); 
			it != tempOutportStatus.end(); it++){
				ocout << "    " << it->first << "," << endl;
		}

		bool firstInportPrint = false;

		for(map<string, int>::const_iterator it = tempInportStatus.begin(); 
			it != tempInportStatus.end(); it++){

				if (it->first.compare("vcc") && it->first.compare("gnd")){

					if (!firstInportPrint)
					{
						ocout << "    " << it->first;
						firstInportPrint = true;

					}else{
						ocout << "," << endl;
						ocout << "    " << it->first;
					}
				}
		}

		ocout << ");" << endl;

		// ---------------------------------------------------- Ports ------------------------------------------------------

		for(map<string, int>::const_iterator it = tempOutportStatus.begin(); 
			it != tempOutportStatus.end(); it++){

				if (it->second <0)
				{
					ocout << "output  " << it->first << ";" << endl;
				}else{
					ocout << "output  [" << it->second << ":0] "<< it->first << ";" << endl;
				}

		}

		for(map<string, int>::const_iterator it = tempInportStatus.begin(); 
			it != tempInportStatus.end(); it++){

				if (it->first.compare("vcc") && it->first.compare("gnd")){
					if (it->second <0)
					{
						ocout << "input   " << it->first << ";" << endl;
					}else{
						ocout << "input   [" << it->second << ":0] "<< it->first << ";" << endl;
					}
				}
		}

		// ---------------------------------------------------- Wires -----------------------------------------------------

		map<string, LE *> &tempLes = topModel->getLEs();
		map<string, PLL *> &tempPLLs = topModel->getPLLs();
		map<string, RamBlock *> &tempRamBlocks = topModel->getRamBlocks();

		map<string, bool> wirePrint;

		for(map<string, LE *>::const_iterator it = tempLes.begin(); 
			it != tempLes.end(); it++){

				map<string, string> &tempInports = it->second->getInports();
				map<string, string> &tempOutports = it->second->getOutports();

				for(map<string, string>::const_iterator ito = tempInports.begin(); 
					ito != tempInports.end(); ito++){

						if(ito->second.compare("") && 
							ito->second.compare("vcc") && 
							ito->second.compare("gnd") &&
							tempInportsPseudoOutports.count(ito->second) == 0
							//&&
							//tempOutportsPseudoInports.count(ito->second) == 0
							){
								if (!wirePrint[ito->second])
								{
									ocout << "wire \\" 
										<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "") 
										<< ito->second << " ;" << endl;
									wirePrint[ito->second] = true;
								}
						}

				}

				for(map<string, string>::const_iterator ito = tempOutports.begin(); 
					ito != tempOutports.end(); ito++){
						if (ito->second.compare("") &&
							//tempOutportsPseudoInports.count(ito->second) == 0 &&
							tempInportsPseudoOutports.count(ito->second) == 0)
						{
							if (!wirePrint[ito->second])
							{
								ocout << "wire \\" 
									<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "")
									<< ito->second << " ;" << endl;
								wirePrint[ito->second] = true;
							}
						}
				}
		}

		for(map<string, PLL *>::const_iterator it = tempPLLs.begin(); 
			it != tempPLLs.end(); it++){

				map<string, string> &tempInports = it->second->getInports();
				map<string, string> &tempOutports = it->second->getOutports();

				for(map<string, string>::const_iterator ito = tempInports.begin(); 
					ito != tempInports.end(); ito++){

						string portName = ito->first;

						if (!(portName.compare("inclk") && portName.compare("clkena") 
							&& portName.compare("extclkena")))
						{
							string realName = ito->second;
							realName = realName.substr(1, realName.length() - 2);

							vector<string> realNameV = this->split(realName, ",");

							for (vector<string>::const_iterator itRealName = realNameV.begin(); 
								itRealName != realNameV.end(); itRealName++)
							{
								string rName = *itRealName;

								if ((!rName.empty()) && rName.compare("gnd") && rName.compare("vcc") 
									&& tempInportsPseudoOutports.count(rName) == 0)
								{
									if (!wirePrint[rName])
									{
										ocout << "wire \\" 
											<< ((tempOutportsPseudoInports.count(rName) != 0) ? "~" : "") 
											<< rName << " ;" << endl;
										wirePrint[rName] = true;
									}
								}
							}

						} else {
							if (ito->second.compare("") && 
								ito->second.compare("vcc") && 
								ito->second.compare("gnd") &&
								tempInportsPseudoOutports.count(ito->second) == 0
								//&&
								//tempOutportsPseudoInports.count(ito->second) == 0
								) {
									if (!wirePrint[ito->second])
									{
										ocout << "wire \\" 
											<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "") 
											<< ito->second << " ;" << endl;
										wirePrint[ito->second] = true;
									}
							}
						}
				}

				for(map<string, string>::const_iterator ito = tempOutports.begin(); 
					ito != tempOutports.end(); ito++){

						string portName = ito->first;

						if (!portName.compare("clk"))
						{
							string realName = ito->second;
							realName = realName.substr(1, realName.length() - 2);

							vector<string> realNameV = this->split(realName, ",");

							for (vector<string>::const_iterator itRealName = realNameV.begin(); 
								itRealName != realNameV.end(); itRealName++)
							{
								string rName = *itRealName;

								if ((!rName.empty()) && rName.compare("gnd") && rName.compare("vcc"))
								{
									if (!wirePrint[rName])
									{
										ocout << "wire \\" 
											<< ((tempOutportsPseudoInports.count(rName) != 0) ? "~" : "") 
											<< rName << " ;" << endl;
										wirePrint[rName] = true;
									}
								}
							}

						} else if (!portName.compare("extclk")) {

							string realName = ito->second;
							realName = realName.substr(1, realName.length() - 2);

							vector<string> realNameV = this->split(realName, ",");

							for (vector<string>::const_iterator itRealName = realNameV.begin(); 
								itRealName != realNameV.end(); itRealName++)
							{
								string rName = *itRealName;

								if ((!rName.empty()) && rName.compare("gnd") && rName.compare("vcc"))
								{
									if (rName.length() != 25 
										|| (rName.length() == 25 
										&& !(!(rName.substr(0, 24).compare("extclk_unconnected_wire_"))
										&& isdigit(rName.at(24)))))
									{
										if (!wirePrint[rName])
										{
											ocout << "wire \\" 
												<< ((tempOutportsPseudoInports.count(rName) != 0) ? "~" : "") 
												<< rName << " ;" << endl;
											wirePrint[rName] = true;
										}
									}
								}
							}

						} else {

							if (ito->second.compare("") &&
								//tempOutportsPseudoInports.count(ito->second) == 0 &&
								tempInportsPseudoOutports.count(ito->second) == 0)
							{
								if (!wirePrint[ito->second])
								{
									ocout << "wire \\" 
										<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "")
										<< ito->second << " ;" << endl;
									wirePrint[ito->second] = true;
								}
							}

						}
				}
		}

		for(map<string, RamBlock *>::const_iterator it = tempRamBlocks.begin(); 
			it != tempRamBlocks.end(); it++){

				map<string, string> &tempInports = it->second->getInports();
				map<string, string> &tempOutports = it->second->getOutports();

				for(map<string, string>::const_iterator ito = tempInports.begin(); 
					ito != tempInports.end(); ito++){

						string portName = ito->first;

						if (!(portName.compare("portaaddr") && portName.compare("portbaddr") 
							&& portName.compare("portadatain") && portName.compare("portbdatain")))
						{
							string realName = ito->second;
							realName = realName.substr(1, realName.length() - 2);

							vector<string> realNameV = this->split(realName, ",");

							for (vector<string>::const_iterator itRealName = realNameV.begin(); 
								itRealName != realNameV.end(); itRealName++)
							{
								string rName = *itRealName;

								if ((!rName.empty()) && rName.compare("gnd") && rName.compare("vcc") 
									&& tempInportsPseudoOutports.count(rName) == 0)
								{
									if (!wirePrint[rName])
									{
										ocout << "wire \\" 
											<< ((tempOutportsPseudoInports.count(rName) != 0) ? "~" : "") 
											<< rName << " ;" << endl;
										wirePrint[rName] = true;
									}
								}
							}

						} else {
							if (ito->second.compare("") && 
								ito->second.compare("vcc") && 
								ito->second.compare("gnd") &&
								tempInportsPseudoOutports.count(ito->second) == 0
								//&&
								//tempOutportsPseudoInports.count(ito->second) == 0
								) {
									if (!wirePrint[ito->second])
									{
										ocout << "wire \\" 
											<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "") 
											<< ito->second << " ;" << endl;
										wirePrint[ito->second] = true;
									}
							}
						}
				}

				for(map<string, string>::const_iterator ito = tempOutports.begin(); 
					ito != tempOutports.end(); ito++){

						string portName = ito->first;

						if (!(portName.compare("portadataout") && portName.compare("portbdataout")))
						{
							string realName = ito->second;
							realName = realName.substr(1, realName.length() - 2);

							vector<string> realNameV = this->split(realName, ",");

							for (vector<string>::const_iterator itRealName = realNameV.begin(); 
								itRealName != realNameV.end(); itRealName++)
							{
								string rName = *itRealName;

								if ((!rName.empty()) && rName.compare("gnd") && rName.compare("vcc"))
								{
									if (!wirePrint[rName])
									{
										ocout << "wire \\" 
											<< ((tempOutportsPseudoInports.count(rName) != 0) ? "~" : "") 
											<< rName << " ;" << endl;
										wirePrint[rName] = true;
									}
								}
							}

						} else {

							if (ito->second.compare("") &&
								//tempOutportsPseudoInports.count(ito->second) == 0 &&
								tempInportsPseudoOutports.count(ito->second) == 0)
							{
								if (!wirePrint[ito->second])
								{
									ocout << "wire \\" 
										<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "")
										<< ito->second << " ;" << endl;
									wirePrint[ito->second] = true;
								}
							}

						}
				}
		}

		// ------------------------------------------------- GND VCC ---------------------------------------------------

		ocout << "\n" << "wire gnd;" << endl;
		ocout << "wire vcc;\n" << endl;

		ocout << "assign gnd = 1'b0;" << endl;
		ocout << "assign vcc = 1'b1;\n" << endl;

		// ------------------------------------------------- Stratix_lcell ---------------------------------------------------

		for(map<string, LE *>::const_iterator it = tempLes.begin(); 
			it != tempLes.end(); it++){

				if (it->second->getOutports().size() == 0)
				{
					cout << "Warning: " << it->second->getName() << " with NO OUTPUT!" << endl;

					if (it->second->getLUT() != NULL)
					{
						LUT &tempLUTNoOutLE = *it->second->getLUT();
						map<string, InPort *> &tempInportsLUTNoOutLE = tempLUTNoOutLE.getInports();
						map<string, OutPort *> &tempOutportsLUTNoOutLE = tempLUTNoOutLE.getOutports();

						cout << tempLUTNoOutLE.getName() << endl;

						for (map<string, InPort *>::const_iterator itInport = tempInportsLUTNoOutLE.begin(); 
							itInport != tempInportsLUTNoOutLE.end(); itInport++)
						{
							cout << itInport->second->showPortInfo() << endl;
						}

						for (map<string, OutPort *>::const_iterator itOutport = tempOutportsLUTNoOutLE.begin(); 
							itOutport != tempOutportsLUTNoOutLE.end(); itOutport++)
						{
							cout << itOutport->second->showPortInfo() << endl;
						}
					}

					if (it->second->getReg() != NULL)
					{
						Reg &tempRegNoOutLE = *it->second->getReg();
						map<string, InPort *> &tempInportsRegNoOutLE = tempRegNoOutLE.getInports();
						map<string, OutPort *> &tempOutportsRegNoOutLE = tempRegNoOutLE.getOutports();

						cout << tempRegNoOutLE.getName() << endl;

						for (map<string, InPort *>::const_iterator itInport = tempInportsRegNoOutLE.begin(); 
							itInport != tempInportsRegNoOutLE.end(); itInport++)
						{
							cout << itInport->second->showPortInfo() << endl;
						}

						for (map<string, OutPort *>::const_iterator itOutport = tempOutportsRegNoOutLE.begin(); 
							itOutport != tempOutportsRegNoOutLE.end(); itOutport++)
						{
							cout << itOutport->second->showPortInfo() << endl;
						}
					}

					cout << "----------------------------------------------------------------" << endl;

					continue;
				}

				this->LECount++;
				LUT * tmpLut = it->second->getLUT();
				Reg * tmpReg = it->second->getReg();

				if (tmpLut != NULL && tmpReg != NULL)
				{
					lutRegLECount++;
					regsLECount++;
				} else if (tmpLut == NULL && tmpReg != NULL) {
					regOnlyLECount++;
					regsLECount++;
				} else if (tmpLut != NULL && tmpReg == NULL) {
					lutOnlyLECount++;
				}

				ocout << "stratix_lcell \\" << it->second->getName() << " (" << endl;

				map<string, string> &tempInports = it->second->getInports();
				map<string, string> &tempOutports = it->second->getOutports();

				for(map<string, string>::const_iterator ito = tempInports.begin(); 
					ito != tempInports.end(); ito++){
						if (ito->second.compare(""))
						{
							ocout << "    ." << ito->first << "(" 
								<< (wirePrint.count(ito->second) ? "\\" : "") 
								<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "")
								<< ito->second << (wirePrint.count(ito->second) ? " " : "") << ")," << endl;
						}
				}

				firstInportPrint = false;

				for(map<string, string>::const_iterator ito = tempOutports.begin(); 
					ito != tempOutports.end(); ito++){

						if (ito->second.compare(""))
						{
							if (!firstInportPrint)
							{
								ocout << "    ." << ito->first << "(" 
									<< (wirePrint.count(ito->second) ? "\\" : "") 
									<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "")
									<< ito->second << (wirePrint.count(ito->second) ? " " : "") << ")";
								firstInportPrint = true;

							} else {
								ocout << "," << endl;
								ocout << "    ." << ito->first << "(" 
									<< (wirePrint.count(ito->second) ? "\\" : "") 
									<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "")
									<< ito->second << (wirePrint.count(ito->second) ? " " : "") << ")";
							}
						}
				}

				ocout << ");" << endl;

				// defparams

				map<string, string> &tempDefparams = it->second->getDefparams();

				for(int i = 0; i < it->second->DEFPARAM_TYPE_COUNT; i++){

					ocout << "defparam \\" 
						<< it->first << " ." 
						<< it->second->defParamName[i] << " = \"" 
						<< tempDefparams[it->second->defParamName[i]] << "\";" << endl;

				}

				ocout << endl;

		}

		// ------------------------------------------------- Stratix_pll ---------------------------------------------------

		for(map<string, PLL *>::const_iterator it = tempPLLs.begin(); 
			it != tempPLLs.end(); it++){

				if (it->second->getOutports().size() == 0)
				{
					cout << "Warning: " << it->second->getName() << " with NO OUTPUT!" << endl;

					if (it->second->getCometPLL() != NULL)
					{
						CometPLL &tempCometPllNoOutPLL = *it->second->getCometPLL();
						map<string, InPort *> &tempInportsCometPllNoOutPLL = tempCometPllNoOutPLL.getInports();
						map<string, OutPort *> &tempOutportsCometPllNoOutPLL = tempCometPllNoOutPLL.getOutports();

						cout << tempCometPllNoOutPLL.getName() << endl;

						for (map<string, InPort *>::const_iterator itInport = tempInportsCometPllNoOutPLL.begin(); 
							itInport != tempInportsCometPllNoOutPLL.end(); itInport++)
						{
							cout << itInport->second->showPortInfo() << endl;
						}

						for (map<string, OutPort *>::const_iterator itOutport = tempOutportsCometPllNoOutPLL.begin(); 
							itOutport != tempOutportsCometPllNoOutPLL.end(); itOutport++)
						{
							cout << itOutport->second->showPortInfo() << endl;
						}
					}

					cout << "----------------------------------------------------------------" << endl;

					continue;
				}

				this->pllCount++;
				ocout << "stratix_pll \\" << it->second->getName() << " (" << endl;

				map<string, string> &tempInports = it->second->getInports();
				map<string, string> &tempOutports = it->second->getOutports();

				for(map<string, string>::const_iterator ito = tempInports.begin(); 
					ito != tempInports.end(); ito++){

						string portName = ito->first;

						if (!portName.compare("inclk"))
						{
							string realName = ito->second;
							realName = realName.substr(1, realName.length() - 2);

							vector<string> realNameV = this->split(realName, ",");
							int vSize = realNameV.size();

							string wrRealName = "{";

							int cnt = 0;
							for (vector<string>::const_iterator itRealName = realNameV.begin(); 
								itRealName != realNameV.end(); itRealName++)
							{
								cnt++;
								string rName = *itRealName;

								if (rName.empty())
								{
									continue;
								}

								if (!(rName.compare("gnd") && rName.compare("vcc"))) {

									wrRealName += rName;

								} else {

									if (wirePrint.count(rName)) {
										wrRealName += "\\";
									}

									if (tempOutportsPseudoInports.count(rName)) {
										wrRealName += "~";
									}

									wrRealName += rName;

									if (wirePrint.count(rName)) {
										wrRealName += " ";
									}

								}

								if(cnt != vSize){
									wrRealName += ",";
								}
							}

							wrRealName += "}";

							ocout << "    ." << ito->first << "(" 	<< wrRealName << ")," << endl;

						} else {

							if (ito->second.compare(""))
							{
								ocout << "    ." << ito->first << "(" 
									<< (wirePrint.count(ito->second) ? "\\" : "") 
									<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "")
									<< ito->second << (wirePrint.count(ito->second) ? " " : "") << ")," << endl;
							}
						}						
				}

				firstInportPrint = false;

				for(map<string, string>::const_iterator ito = tempOutports.begin(); 
					ito != tempOutports.end(); ito++){

						string portName = ito->first;

						if (!portName.compare("clk"))
						{
							string realName = ito->second;
							realName = realName.substr(1, realName.length() - 2);

							vector<string> realNameV = this->split(realName, ",");
							int vSize = realNameV.size();

							string wrRealName = "{";

							int cnt = 0;

							for (vector<string>::const_iterator itRealName = realNameV.begin(); 
								itRealName != realNameV.end(); itRealName++)
							{
								cnt++;
								string rName = *itRealName;

								if (rName.empty())
								{
									continue;
								}

								if (!(rName.compare("gnd") && rName.compare("vcc"))) {

									wrRealName += rName;

								} else {

									if (wirePrint.count(rName)) {
										wrRealName += "\\";
									}

									if (tempOutportsPseudoInports.count(rName)) {
										wrRealName += "~";
									}

									wrRealName += rName;

									if (wirePrint.count(rName)) {
										wrRealName += " ";
									}

								}

								if(cnt != vSize){
									wrRealName += ",";
								}
							}

							wrRealName += "}";

							if (!firstInportPrint) {
								ocout << "    ." << ito->first << "(" 	<< wrRealName << ")";
								firstInportPrint = true;
							} else {
								ocout << "," << endl;
								ocout << "    ." << ito->first << "(" 	<< wrRealName << ")";
							}

						} else if (!portName.compare("extclk")){

							string realName = ito->second;
							realName = realName.substr(1, realName.length() - 2);

							vector<string> realNameV = this->split(realName, ",");
							int vSize = realNameV.size();

							string wrRealName = "{";

							int cnt = 0;
							for (vector<string>::const_iterator itRealName = realNameV.begin(); 
								itRealName != realNameV.end(); itRealName++)
							{
								cnt++;
								string rName = *itRealName;

								if (rName.empty())
								{
									continue;
								}

								if (!(rName.compare("gnd") && rName.compare("vcc"))) {

									wrRealName += rName;

								} else if (rName.length() == 25 && (!rName.substr(0, 24).compare(""))) {



								} else {

									if (wirePrint.count(rName)) {
										wrRealName += "\\";
									}

									if (tempOutportsPseudoInports.count(rName)) {
										wrRealName += "~";
									}

									wrRealName += rName;

									if (wirePrint.count(rName)) {
										wrRealName += " ";
									}

								}

								if(cnt != vSize){
									wrRealName += ",";
								}
							}

							wrRealName += "}";

							if (!firstInportPrint) {
								ocout << "    ." << ito->first << "(" 	<< wrRealName << ")";
								firstInportPrint = true;
							} else {
								ocout << "," << endl;
								ocout << "    ." << ito->first << "(" 	<< wrRealName << ")";
							}

						} else {
							if (ito->second.compare(""))
							{
								if (!firstInportPrint)
								{
									ocout << "    ." << ito->first << "(" 
										<< (wirePrint.count(ito->second) ? "\\" : "") 
										<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "")
										<< ito->second << (wirePrint.count(ito->second) ? " " : "") << ")";
									firstInportPrint = true;

								} else {
									ocout << "," << endl;
									ocout << "    ." << ito->first << "(" 
										<< (wirePrint.count(ito->second) ? "\\" : "") 
										<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "")
										<< ito->second << (wirePrint.count(ito->second) ? " " : "") << ")";
								}
							}
						}					
				}

				ocout << ");" << endl;

				// defparams

				map<string, string> &tempDefparams = it->second->getDefparams();

				//for(map<string, string>::const_iterator defpara = tempDefparams.begin(); 
				//	defpara != tempDefparams.end(); defpara++){

				//	ocout << "defparam \\" 
				//		<< it->first << " ." 
				//		<< defpara->first << " = " 
				//		<< defpara->second << ";" << endl;

				//}

				for(int i = 0; i < PLL::DEFPARAM_TYPE_COUNT; i++){

					if (tempDefparams.count(PLL::defParamName[i]) != 0)
					{
						ocout << "defparam \\" 
							<< it->first << " ." 
							<< PLL::defParamName[i] << " = " 
							<< tempDefparams[PLL::defParamName[i]] << ";" << endl;
					}

				}

				ocout << endl;

		}

		// ------------------------------------------------- Stratix_ram_block ---------------------------------------------------

		for(map<string, RamBlock *>::const_iterator it = tempRamBlocks.begin(); 
			it != tempRamBlocks.end(); it++){

				if (it->second->getOutports().size() == 0)
				{
					cout << "Warning: " << it->second->getName() << " with NO OUTPUT!" << endl;

					if (it->second->getCometMem() != NULL)
					{
						CometMem &tempCometMemNoOutPLL = *it->second->getCometMem();
						map<string, InPort *> &tempInportsCometMemNoOutPLL = tempCometMemNoOutPLL.getInports();
						map<string, OutPort *> &tempOutportsCometMemNoOutPLL = tempCometMemNoOutPLL.getOutports();

						cout << tempCometMemNoOutPLL.getName() << endl;

						for (map<string, InPort *>::const_iterator itInport = tempInportsCometMemNoOutPLL.begin(); 
							itInport != tempInportsCometMemNoOutPLL.end(); itInport++)
						{
							cout << itInport->second->showPortInfo() << endl;
						}

						for (map<string, OutPort *>::const_iterator itOutport = tempOutportsCometMemNoOutPLL.begin(); 
							itOutport != tempOutportsCometMemNoOutPLL.end(); itOutport++)
						{
							cout << itOutport->second->showPortInfo() << endl;
						}
					}

					cout << "----------------------------------------------------------------" << endl;

					continue;
				}

				this->memCount++;
				ocout << "stratix_ram_block \\" << it->second->getName() << " (" << endl;

				map<string, string> &tempInports = it->second->getInports();
				map<string, string> &tempOutports = it->second->getOutports();

				for(map<string, string>::const_iterator ito = tempInports.begin(); 
					ito != tempInports.end(); ito++){

						string portName = ito->first;

						if (!portName.compare("portbrewe"))
						{
							if (!it->second->getFlagPortB())
							{
								continue;
							}
						}

						if (!(portName.compare("portaaddr") && portName.compare("portbaddr") 
							&& portName.compare("portadatain") && portName.compare("portbdatain")))
						{
							string realName = ito->second;
							realName = realName.substr(1, realName.length() - 2);

							vector<string> realNameV = this->split(realName, ",");
							int vSize = realNameV.size();

							string wrRealName = "{";

							int cnt = 0;
							for (vector<string>::const_iterator itRealName = realNameV.begin(); 
								itRealName != realNameV.end(); itRealName++)
							{
								cnt++;
								string rName = *itRealName;

								if (rName.empty())
								{
									continue;
								}

								if (!(rName.compare("gnd") && rName.compare("vcc"))) {

									wrRealName += rName;

								} else {

									if (wirePrint.count(rName)) {
										wrRealName += "\\";
									}

									if (tempOutportsPseudoInports.count(rName)) {
										wrRealName += "~";
									}

									wrRealName += rName;

									if (wirePrint.count(rName)) {
										wrRealName += " ";
									}

								}

								if(cnt != vSize){
									wrRealName += ",";
								}
							}

							wrRealName += "}";

							ocout << "    ." << ito->first << "(" 	<< wrRealName << ")," << endl;

						} else {

							if (ito->second.compare(""))
							{
								ocout << "    ." << ito->first << "(" 
									<< (wirePrint.count(ito->second) ? "\\" : "") 
									<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "")
									<< ito->second << (wirePrint.count(ito->second) ? " " : "") << ")," << endl;
							}
						}						
				}

				firstInportPrint = false;

				for(map<string, string>::const_iterator ito = tempOutports.begin(); 
					ito != tempOutports.end(); ito++){

						string portName = ito->first;

						if (!(portName.compare("portadataout") && portName.compare("portbdataout")))
						{
							string realName = ito->second;
							realName = realName.substr(1, realName.length() - 2);

							vector<string> realNameV = this->split(realName, ",");
							int vSize = realNameV.size();

							string wrRealName = "{";

							int cnt = 0;

							for (vector<string>::const_iterator itRealName = realNameV.begin(); 
								itRealName != realNameV.end(); itRealName++)
							{
								cnt++;
								string rName = *itRealName;

								if (rName.empty())
								{
									continue;
								}

								if (!(rName.compare("gnd") && rName.compare("vcc"))) {

									wrRealName += rName;

								} else {

									if (wirePrint.count(rName)) {
										wrRealName += "\\";
									}

									if (tempOutportsPseudoInports.count(rName)) {
										wrRealName += "~";
									}

									wrRealName += rName;

									if (wirePrint.count(rName)) {
										wrRealName += " ";
									}

								}

								if(cnt != vSize){
									wrRealName += ",";
								}
							}

							wrRealName += "}";

							if (!firstInportPrint) {
								ocout << "    ." << ito->first << "(" 	<< wrRealName << ")";
								firstInportPrint = true;
							} else {
								ocout << "," << endl;
								ocout << "    ." << ito->first << "(" 	<< wrRealName << ")";
							}

						} else {
							if (ito->second.compare(""))
							{
								if (!firstInportPrint)
								{
									ocout << "    ." << ito->first << "(" 
										<< (wirePrint.count(ito->second) ? "\\" : "") 
										<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "")
										<< ito->second << (wirePrint.count(ito->second) ? " " : "") << ")";
									firstInportPrint = true;

								}else{
									ocout << "," << endl;
									ocout << "    ." << ito->first << "(" 
										<< (wirePrint.count(ito->second) ? "\\" : "") 
										<< ((tempOutportsPseudoInports.count(ito->second) != 0) ? "~" : "")
										<< ito->second << (wirePrint.count(ito->second) ? " " : "") << ")";
								}
							}
						}					
				}

				ocout << ");" << endl;

				// defparams

				map<string, string> &tempDefparams = it->second->getDefparams();

				//for(map<string, string>::const_iterator defpara = tempDefparams.begin(); 
				//	defpara != tempDefparams.end(); defpara++){

				//	ocout << "defparam \\" 
				//		<< it->first << " ." 
				//		<< defpara->first << " = " 
				//		<< defpara->second << ";" << endl;

				//}

				for(int i = 0; i < RamBlock::DEFPARAM_TYPE_COUNT; i++){

					if (tempDefparams.count(RamBlock::defParamName[i]) != 0)
					{
						ocout << "defparam \\" 
							<< it->first << " ." 
							<< RamBlock::defParamName[i] << " = " 
							<< tempDefparams[RamBlock::defParamName[i]] << ";" << endl;
					}

				}

				ocout << endl;

		}

		// ---------------------------------------------- Assign Outport ------------------------------------------------

		for(map<string, InPort *>::const_iterator it = tempOutportsPseudoInports.begin(); 
			it != tempOutportsPseudoInports.end(); it++){

				if (it->second != NULL)
				{
					string tempRealName = it->second->getRealName();
					ocout << "assign " << it->first << " = " 
						<< (topModel->getInportStatus().count(tempRealName) == 0? 
						(topModel->getOutportStatus().count(this->split(tempRealName, "[").at(0)) == 0? "\\" : "\\~") : "")
						<< tempRealName 
						<< (topModel->getInportStatus().count(tempRealName) == 0? " " : "")
						<< ";" << endl;
				}
		}

		// ---------------------------------------------- Endmodule ------------------------------------------------

		ocout << "\nendmodule" << endl;

	}

	ocout.close();
}

void BlifToVQM::setTopModel(map<string, map<string, SubcktModelInfo *> *> &subcktModelsInfo)
{
	topModel->setModel(subcktModelsInfo);
}

void BlifToVQM::showInfo(void)
{
	cout << "Copyright (C) 1991-2007 Altera Corporation \n" << endl;
}

vector<string> BlifToVQM::split(string str, string ptn)
{
	string::size_type pos;
	vector<string> result;
	str += ptn;
	int size = str.size();

	for(int i = 0; i < size; i++)
	{
		pos = str.find(ptn, i);
		if(pos < size)
		{
			string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + ptn.size() - 1;
		}
	}
	return result;
}

string BlifToVQM::straightLineAway(string str)
{
	vector<string> tempWireName;
	tempWireName = this->split(str, "|");

	if (tempWireName.size() == 2)
	{
		str = tempWireName.at(1);
	}

	return str;
}


// Discard blanks in front of or at the end of certain line
string BlifToVQM::trim(string str)
{
	//const char * val = new char[str.size()];
	//val = str.c_str();

	const char * val = str.c_str();

	int len = str.size();
	int st = 0;

	while ((st < len) && (val[st] <= ' ')) {
		st++;
	}

	while ((st < len) && (val[len - 1] <= ' ')) {
		len--;
	}

	return ((st > 0) || (len < str.size())) ? str.substr(st, len - st) : str;

}
