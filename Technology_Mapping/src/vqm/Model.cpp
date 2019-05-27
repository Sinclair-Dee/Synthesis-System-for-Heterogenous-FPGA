#include "Model.h"
#include <iostream>
#include <sstream>
#include "LUT.h"
#include <fstream>
#include <stdlib.h>

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ostringstream;
using std::min;

Model::Model(string name, string nodeType, string infoFile): Node(nodeType)
{
	this->setBlackbox(false);
	this->name = name;
	this->lutCount = 0;
	this->iniLutName = "lut";
	this->regCount = 0;
	this->iniRegName = "reg";
	this->iniWireName = "wire~";
	this->latchCount = 0;
	this->iniLatchName = "latch";
	this->pllCount = 0;
	this->iniPllName = "pll";
	this->ramCount = 0;
	this->iniRamName = "ram";
	this->iniLEName = "LE~";
	this->regCascade = false;
	//this->bufferAway = false;
	this->bufferAway = true;

	this->bufferPortInfo = new map<string, string>();
}


Model::~Model(void)
{
	//cout << this->name << " deconstruct..." << endl;

	if (bufferPortInfo != nullptr)
	{
		delete bufferPortInfo;
	}

	for (map<string, InPort *>::const_iterator it = outportsPseudoInports.begin(); 
		it != outportsPseudoInports.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

	for (map<string, OutPort *>::const_iterator it = inportsPseudoOutports.begin(); 
		it != inportsPseudoOutports.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

	for (map<string, LUT *>::const_iterator it = luts.begin(); 
		it != luts.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

	for (map<string, Wire *>::const_iterator it = wires.begin(); 
		it != wires.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

	for (map<string, vector<Wire *> *>::const_iterator it = tempWires.begin(); 
		it != tempWires.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

	for (map<string, map<string, map<string, map<string, string> *> *> *>::const_iterator it = subckts.begin(); 
		it != subckts.end(); it++){
			if (it->second != NULL)
			{
				for (map<string, map<string, map<string, string> *> *>::const_iterator ito = (*(it->second)).begin(); 
					ito != (*(it->second)).end(); ito++){
						if (ito->second != NULL)
						{
							for (map<string, map<string, string> *>::const_iterator itoo = (*(ito->second)).begin(); 
								itoo != (*(ito->second)).end(); itoo++){
									if (itoo->second != NULL)
									{
										delete itoo->second;
									}
							}

							delete ito->second;
						}
				}

				delete it->second;
			}
	}

	for (map<string, Reg *>::const_iterator it = regs.begin(); 
		it != regs.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

	for (map<string, Latch *>::const_iterator it = latches.begin(); 
		it != latches.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

	for (map<string, LE *>::const_iterator it = les.begin(); 
		it != les.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

	for (map<string, InPort *>::const_iterator it = inports.begin(); 
		it != inports.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

	for (map<string, OutPort *>::const_iterator it = outports.begin(); 
		it != outports.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

	for (map<string, CometPLL *>::const_iterator it = cometPlls.begin(); 
		it != cometPlls.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

	for (map<string, CometMem *>::const_iterator it = cometMems.begin(); 
		it != cometMems.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

	for (map<string, PLL *>::const_iterator it = plls.begin(); 
		it != plls.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}

	for (map<string, RamBlock *>::const_iterator it = ramBlocks.begin(); 
		it != ramBlocks.end(); it++){
			if (it->second != NULL)
			{
				delete it->second;
			}
	}
}

void Model::addCometMem(CometMem &mem)
{
	string name = mem.getName();
	cometMems[name] = &mem;
}

void Model::addCometPll(CometPLL &pll)
{
	string name = pll.getName();
	cometPlls[name] = &pll;
}

void Model::addInfoToSubckt(string subcktType, string curSubcktModelName, string subcktName, string portName, string realName)
{
	(*((*((*(this->subckts[subcktType]))[curSubcktModelName]))[subcktName]))[portName] = realName;
}

void Model::addInPort(OutPort &inport)
{
	string name = inport.getRealName();
	inportsPseudoOutports[name] = &inport;
}

void Model::addLatch(Latch &latch)
{
	string name = latch.getName();
	latches[name] = &latch;
}

void Model::addLUT(LUT &lut)
{
	string name = lut.getName();
	luts[name] = &lut;
}

//map<string, InPort *> Model::getInputs(void) const
//{
//	return this->inputs;
//}

void Model::addOutPort(InPort &outport)
{
	string name = outport.getRealName();
	outportsPseudoInports[name] = &outport;
}

void Model::addReg(Reg &reg)
{
	string name = reg.getName();
	regs[name] = &reg;
}

void Model::addSubckt(string subcktType, string curSubcktModelName, string subcktName)
{
	if (this->subckts.count(subcktType) == 0)
	{
		(*((*(this->subckts[subcktType] = 
			new map<string, map<string, map<string, string> *> *>))[curSubcktModelName] = 
			new map<string, map<string, string> *>))[subcktName] = 
			new map<string, string>;

		return;

	}else if((*(this->subckts[subcktType])).count(curSubcktModelName) == 0){
		(*((*(this->subckts[subcktType]))[curSubcktModelName] = 
			new map<string, map<string, string> *>))[subcktName] = 
			new map<string, string>;

		return;
	}

	(*((*(this->subckts[subcktType]))[curSubcktModelName]))[subcktName] = 
		new map<string, string>;

}

void Model::addTempWireFromInport(InPort &inport)
{
	string tempWireRealName = this->getWireRealNameFromInport(inport);

	if (this->tempWires.count(tempWireRealName) == 0){
		tempWires[tempWireRealName] = new vector<Wire *>;
	}

	tempWires[tempWireRealName]->	push_back(new Wire(*this, tempWireRealName, inport));
}

void Model::bufferGone(void)
{
	// ----------------------------  Find Buffer and Record Ports-------------------------------

	for (map<string, LUT *>::const_iterator it = luts.begin(); 
		it != luts.end(); it++){
			if (it->second != NULL)
			{
				LUT &curLUT = *it->second;
				if ((!curLUT.getNodeType().compare(LUT_TYPE)))
				{
					vector<string> &tempInTrueTable = curLUT.getInTrueTable();
					string tempTrueTable = *tempInTrueTable.begin();
					string tempOutFormat = "0";
					tempOutFormat.at(0) = curLUT.getOutFormat();
					string temp = tempTrueTable + tempOutFormat;

					if (curLUT.getInoutPortRealNameCount() == 2 
						&& tempInTrueTable.size() == 1 
						&& !(temp.compare("11") && temp.compare("00")))
					{
						curLUT.setBuffer(true);

						(*bufferPortInfo)[curLUT.getOutports().begin()->second->getRealName()] 
						= curLUT.getInports().begin()->second->getRealName();
					}
				}
			}
	}

	// ----------------------------  Rename  Ports-------------------------------

	for (map<string, string>::const_iterator itBufferPortInfo = bufferPortInfo->begin(); 
		itBufferPortInfo != bufferPortInfo->end(); itBufferPortInfo++)
	{
		while (true)
		{
			if (bufferPortInfo->count(itBufferPortInfo->second))
			{
				(*bufferPortInfo)[itBufferPortInfo->first] 
				= (*bufferPortInfo)[itBufferPortInfo->second];
			}else{
				break;
			}
		}
	}

	// ------------------------------  Change LUT InPort realName  ----------------------------------

	for (map<string, LUT *>::const_iterator it = luts.begin(); 
		it != luts.end(); it++){
			if (it->second != NULL && (!it->second->isBuffer())){

				map<string, InPort *> &tempInports = it->second->getInports();

				for (map<string, InPort *>::const_iterator itInport = tempInports.begin(); 
					itInport != tempInports.end(); itInport++) 
				{
					if (itInport->second != NULL 
						&& bufferPortInfo->count(itInport->second->getRealName()) != 0)
					{
						itInport->second->setRealName((*bufferPortInfo)[itInport->second->getRealName()]);
					}
				}
			}
	}

	// ------------------------------  Change Reg InPort realName  ----------------------------------

	for (map<string, Reg *>::const_iterator it = regs.begin(); 
		it != regs.end(); it++){
			if (it->second != NULL){

				map<string, InPort *> &tempInports = it->second->getInports();

				for (map<string, InPort *>::const_iterator itInport = tempInports.begin(); 
					itInport != tempInports.end(); itInport++) 
				{
					if (itInport->second != NULL 
						&& bufferPortInfo->count(itInport->second->getRealName()) != 0)
					{
						itInport->second->setRealName((*bufferPortInfo)[itInport->second->getRealName()]);
					}
				}
			}
	}

	// ------------------------------  Change Comet_PLL InPort realName  ----------------------------------

	for (map<string, CometPLL *>::const_iterator it = cometPlls.begin(); 
		it != cometPlls.end(); it++){
			if (it->second != NULL){

				map<string, InPort *> &tempInports = it->second->getInports();

				for (map<string, InPort *>::const_iterator itInport = tempInports.begin(); 
					itInport != tempInports.end(); itInport++) 
				{
					if (itInport->second != NULL 
						&& bufferPortInfo->count(itInport->second->getRealName()) != 0)
					{
						itInport->second->setRealName((*bufferPortInfo)[itInport->second->getRealName()]);
					}
				}
			}
	}

	// ------------------------------  Change Comet_Mem InPort realName  ----------------------------------

	for (map<string, CometMem *>::const_iterator it = cometMems.begin(); 
		it != cometMems.end(); it++){
			if (it->second != NULL){

				map<string, InPort *> &tempInports = it->second->getInports();

				for (map<string, InPort *>::const_iterator itInport = tempInports.begin(); 
					itInport != tempInports.end(); itInport++) 
				{
					if (itInport->second != NULL 
						&& bufferPortInfo->count(itInport->second->getRealName()) != 0)
					{
						itInport->second->setRealName((*bufferPortInfo)[itInport->second->getRealName()]);
					}
				}
			}
	}

	// ------------------------------  Change Model OutPort realName  ----------------------------------

	for (map<string, InPort *>::const_iterator itInport = outportsPseudoInports.begin(); 
		itInport != outportsPseudoInports.end(); itInport++) 
	{
		if (itInport->second != NULL 
			&& bufferPortInfo->count(itInport->second->getRealName()) != 0)
		{
			itInport->second->setRealName((*bufferPortInfo)[itInport->second->getRealName()]);
		}
	}
}

void Model::connectTempWireToOutport(OutPort &outport)
{
	vector<Wire *> * tempV = tempWires[outport.getRealName()];

	if (tempV != NULL)
	{
		for (vector<Wire *>::const_iterator it = tempV->begin(); 
			it != tempV->end(); it++){
				if (*it != NULL)
				{
					(*it)->setWireName(this->genWireNameFromTempWire(**it, outport));
					outport.setWires((**it).getWireName(), **it);
					(*it)->setOutport(outport);
				}
		}
	}
}

void Model::connectWireToInport(void)
{
	for (map<string, Wire *>::const_iterator it = wires.begin(); it != wires.end(); it++)
	{
		it->second->getInport().addWire(it->first, *it->second);
	}
}

//map<string, OutPort *> Model::getOutputs(void) const
//{
//	return this->outputs;
//}

void Model::connWires(void)
{
	// Generate tempWires from inports

	// --------------------------    MODEL   --------------------------------

	for (map<string, InPort *>::const_iterator it = outportsPseudoInports.begin(); 
		it != outportsPseudoInports.end(); it++){
			if (it->second != NULL)
			{
				this->addTempWireFromInport(*(it->second));
			}
	}

	// --------------------------    LUT   --------------------------------

	for (map<string, LUT *>::const_iterator it = luts.begin(); 
		it != luts.end(); it++){
			if (it->second != NULL && (!it->second->isBuffer()))
			{
				it->second->genWireInport();
			}
	}

	// --------------------------    REG   --------------------------------

	for (map<string, Reg *>::const_iterator it = regs.begin(); 
		it != regs.end(); it++){
			if (it->second != NULL)
			{
				it->second->genWireInport();
			}
	}

	// --------------------------    COMET_PLL   --------------------------------

	for (map<string, CometPLL *>::const_iterator it = cometPlls.begin(); 
		it != cometPlls.end(); it++){
			if (it->second != NULL)
			{
				it->second->genWireInport();
			}
	}

	// --------------------------    COMET_MEM   --------------------------------

	for (map<string, CometMem *>::const_iterator it = cometMems.begin(); 
		it != cometMems.end(); it++){
			if (it->second != NULL)
			{
				it->second->genWireInport();
			}
	}

	// Connect outports to tempWires constructed

	// --------------------------    LUT   --------------------------------

	for (map<string, LUT *>::const_iterator it = luts.begin(); 
		it != luts.end(); it++){
			if (it->second != NULL && (!it->second->isBuffer()))
			{
				map<string, OutPort *> &outports = it->second->getOutports();

				for (map<string, OutPort *>::const_iterator ito = outports.begin(); 
					ito != outports.end(); ito++){
						if (ito->second != NULL)
						{
							this->connectTempWireToOutport(*ito->second);
						}
				}
			}
	}

	// --------------------------    MODEL   --------------------------------

	for (map<string, OutPort *>::const_iterator it = inportsPseudoOutports.begin(); 
		it != inportsPseudoOutports.end(); it++){			
			if (it->second != NULL)
			{
				this->connectTempWireToOutport(*(it->second));
			}
	}

	// --------------------------    REG   --------------------------------

	for (map<string, Reg *>::const_iterator it = regs.begin(); 
		it != regs.end(); it++){
			if (it->second != NULL)
			{
				map<string, OutPort *> &outports = it->second->getOutports();

				for (map<string, OutPort *>::const_iterator ito = outports.begin(); 
					ito != outports.end(); ito++){

						if (ito->second != NULL)
						{
							this->connectTempWireToOutport(*ito->second);
						}
				}
			}
	}

	// --------------------------    COMET_PLL   --------------------------------

	for (map<string, CometPLL *>::const_iterator it = cometPlls.begin(); 
		it != cometPlls.end(); it++){
			if (it->second != NULL)
			{
				map<string, OutPort *> &outports = it->second->getOutports();

				for (map<string, OutPort *>::const_iterator ito = outports.begin(); 
					ito != outports.end(); ito++){

						if (ito->second != NULL)
						{
							this->connectTempWireToOutport(*ito->second);
						}
				}
			}
	}

	// --------------------------    COMET_MEM   --------------------------------

	for (map<string, CometMem *>::const_iterator it = cometMems.begin(); 
		it != cometMems.end(); it++){
			if (it->second != NULL)
			{
				map<string, OutPort *> &outports = it->second->getOutports();

				for (map<string, OutPort *>::const_iterator ito = outports.begin(); 
					ito != outports.end(); ito++){

						if (ito->second != NULL)
						{
							this->connectTempWireToOutport(*ito->second);
						}
				}
			}
	}

	// Generate  wires according to tempWires

	for (map<string, vector<Wire *> *>::const_iterator it = tempWires.begin(); 
		it != tempWires.end(); it++){
			if (it->second != NULL)
			{
				for (vector<Wire *>::const_iterator ito = it->second->begin(); 
					ito != it->second->end(); ito++){
						if (*ito != NULL)
						{
							wires[(*ito)->getWireName()] = *ito;
						}
				}
			}
	}

	// Connect inports to Wires constructed

	this->connectWireToInport();

}

string Model::genLEName(string lutName, string regName)
{
	if (!(lutName.compare("") && regName.compare("")) )
	{
		return iniLEName + lutName + regName;
	}

	return iniLEName + lutName + "-" + regName;
}

//long Model::getLutCount(void) const
//{
//	return this->lutCount;
//}

string Model::genLutRegLatchName(string nodeType)
{
	char buf [11];

	if (!nodeType.compare(LUT_TYPE))
	{
		ltoa(this->lutCount++, buf, 10);
		string str(buf);
		str = iniLutName + "[" + str + "]";
		return string(str);

	}else if(!nodeType.compare(REG_TYPE)){
		ltoa(this->regCount++, buf, 10);
		string str(buf);
		str = iniRegName + "[" + str + "]";
		return string(str);

	}else if(!nodeType.compare(LATCH_TYPE)){
		ltoa(this->latchCount++, buf, 10);
		string str(buf);
		str = iniLatchName + "[" + str + "]";
		return string(str);
	}

	return "";

}

string Model::genPllRamName(string nodeType)
{
	char buf [11];

	if (!nodeType.compare(PLL_TYPE))
	{
		ltoa(this->pllCount++, buf, 10);
		string str(buf);
		str = iniPllName + "[" + str + "]";
		return string(str);

	} else if (!nodeType.compare(RAM_TYPE)) {
		ltoa(this->ramCount++, buf, 10);
		string str(buf);
		str = iniRamName + "[" + str + "]";
		return string(str);

	}

	return "";

}

void Model::genPortStatus(void)
{
	for(map<string, InPort *>::const_iterator it = outportsPseudoInports.begin(); 
		it != outportsPseudoInports.end(); it++){

			string tempName = it->first;

			vector<string> tempVstr = this->split(tempName, "[");

			if(tempVstr.size() == 1){
				outportStatus[tempName] = -1;
			}else{
				tempName = tempVstr.at(0);

				tempVstr = this->split(tempVstr.at(1), "]");
				string num = tempVstr.at(0);

				const char * str = num.c_str();
				int no = atoi(str);

				if (outportStatus.count(tempName) == 0)
				{
					outportStatus[tempName] = no;
				}else if(no > outportStatus[tempName]){
					outportStatus[tempName] = no;
				}
			}
	}

	for(map<string, OutPort *>::const_iterator it = inportsPseudoOutports.begin(); 
		it != inportsPseudoOutports.end(); it++){

			string tempName = it->first;

			vector<string> tempVstr = this->split(tempName, "[");

			if(tempVstr.size() == 1){
				inportStatus[tempName] = -1;
			}else{
				tempName = tempVstr.at(0);

				tempVstr = this->split(tempVstr.at(1), "]");
				string num = tempVstr.at(0);

				const char * str = num.c_str();
				int no = atoi(str);

				if (inportStatus.count(tempName) == 0)
				{
					inportStatus[tempName] = no;
				}else if(no > inportStatus[tempName]){
					inportStatus[tempName] = no;
				}
			}
	}

	//for (map<string, int>::const_iterator it = inportStatus.begin(); 
	//	it != inportStatus.end(); it++)
	//{
	//	cout << it->first << ": " << it->second << endl;
	//}

	//for (map<string, int>::const_iterator it = outportStatus.begin(); 
	//	it != outportStatus.end(); it++)
	//{
	//	cout << it->first << ": " << it->second << endl;
	//}
}

//map<string, LUT *> Model::getLuts(void) const
//{
//	return this -> luts;
//}

void Model::genSubckt(map<string, map<string, SubcktModelInfo *> *> &subcktModelsInfo)
{
	for (map<string, map<string, SubcktModelInfo *> *>::const_iterator 
		it = subcktModelsInfo.begin(); 
		it != subcktModelsInfo.end(); it++){

			string subcktType = it->first;

			if (!subcktType.compare(REG_TYPE))
			{
				for (map<string, SubcktModelInfo *>::const_iterator ito = it->second->begin(); 
					ito != it->second->end(); ito++)
				{

					string curSubcktModelName = ito->first;

					vector<string> &subcktInports = ito->second->getSubcktInports();
					vector<string> &subcktOutports = ito->second->getSubcktOutports();

					for (map<string, map<string, string> *>::const_iterator 
						itoo = ((*(subckts[subcktType]))[curSubcktModelName])->begin(); 
						itoo != ((*(subckts[subcktType]))[curSubcktModelName])->end(); itoo++){

							Reg *newReg = new Reg(this, itoo->first, curSubcktModelName, REG_TYPE);
							this->addReg(*newReg);

							if (subcktInports.size() == itoo->second->size() - subcktOutports.size()){

								for (vector<string>::const_iterator itooo = subcktInports.begin(); 
									itooo != subcktInports.end(); itooo++)
								{
									string portName =  *itooo;

									if(!(portName.compare("Data0") &&
										portName.compare("Enable") &&
										portName.compare("Aclr") &&
										portName.compare("Clock") &&
										portName.compare("Sclr") &&
										portName.compare("Aset") &&
										portName.compare("Sset"))){
											newReg->addInoutPortRealName(portName, (*(itoo->second))[portName], 1);
									}
								}
							}

							if (subcktOutports.size() == itoo->second->size() - subcktInports.size()){
								for (vector<string>::const_iterator itooo = subcktOutports.begin(); 
									itooo != subcktOutports.end(); itooo++)
								{
									string portName =  *itooo;

									if(!portName.compare("Q0")){
										newReg->addInoutPortRealName(portName, (*(itoo->second))[portName], 0);

									}
								}
							}

					}
				}

			} else if (!subcktType.compare(LATCH_TYPE)) {

				for (map<string, SubcktModelInfo *>::const_iterator ito = it->second->begin(); 
					ito != it->second->end(); ito++) {

						string curSubcktModelName = ito->first;

						vector<string> &subcktInports = ito->second->getSubcktInports();
						vector<string> &subcktOutports = ito->second->getSubcktOutports();

						for (map<string, map<string, string> *>::const_iterator 
							itoo = ((*(subckts[subcktType]))[curSubcktModelName])->begin(); 
							itoo != ((*(subckts[subcktType]))[curSubcktModelName])->end(); itoo++){

								Latch *newLatch = new Latch(this, itoo->first, curSubcktModelName, LATCH_TYPE);
								this->addLatch(*newLatch);

								if (subcktInports.size() == itoo->second->size() - subcktOutports.size()){

									for (vector<string>::const_iterator itooo = subcktInports.begin(); 
										itooo != subcktInports.end(); itooo++)
									{
										string portName =  *itooo;

										if(!(portName.compare("Data0") &&
											portName.compare("Gate"))){
												newLatch->addInoutPortRealName(portName, (*(itoo->second))[portName], 1);
										}
									}
								}

								if (subcktOutports.size() == itoo->second->size() - subcktInports.size()) {
									for (vector<string>::const_iterator itooo = subcktOutports.begin(); 
										itooo != subcktOutports.end(); itooo++)
									{
										string portName =  *itooo;

										if(!portName.compare("Q0")){
											newLatch->addInoutPortRealName(portName, (*(itoo->second))[portName], 0);

										}
									}
								}
						}
				}

			} else if (!subcktType.compare(PLL_TYPE)) {

				for (map<string, SubcktModelInfo *>::const_iterator ito = it->second->begin(); 
					ito != it->second->end(); ito++)
				{

					string curSubcktModelName = ito->first;

					vector<string> &subcktInports = ito->second->getSubcktInports();
					vector<string> &subcktOutports = ito->second->getSubcktOutports();

					for (map<string, map<string, string> *>::const_iterator 
						itoo = ((*(subckts[subcktType]))[curSubcktModelName])->begin(); 
						itoo != ((*(subckts[subcktType]))[curSubcktModelName])->end(); itoo++) {

							string tmp = itoo->first;

							vector<string> cometPllVct = this->split(tmp, "++++");
							string cometPllName = cometPllVct.at(0);
							string cometPllVFile = cometPllVct.at(1);

							CometPLL *newCometPll = new CometPLL(this, cometPllName, curSubcktModelName, PLL_TYPE, cometPllVFile);
							this->addCometPll(*newCometPll);

							for (map<string, string>::const_iterator ioInfo = itoo->second->begin(); 
								ioInfo != itoo->second->end(); ioInfo++){

									bool isFound = false;

									for (int i = 0; i < CometPLL::INPORT_TYPE_COUNT; i++)
									{
										if (!ioInfo->first.compare(CometPLL::inportName[i]))
										{
											isFound = true;
											newCometPll->addInoutPortRealName(ioInfo->first, ioInfo->second, 1);
											break;
										}
									}

									if (!isFound)
									{
										for (int i = 0; i < CometPLL::OUTPORT_TYPE_COUNT; i++) {
											if (!ioInfo->first.compare(CometPLL::outportName[i])) {
												newCometPll->addInoutPortRealName(ioInfo->first, ioInfo->second, 0);
												break;
											}
										}
									}
							}
					}
				}

			} else if (!subcktType.compare(RAM_TYPE)) {

				for (map<string, SubcktModelInfo *>::const_iterator ito = it->second->begin(); 
					ito != it->second->end(); ito++) {

						string curSubcktModelName = ito->first;

						vector<string> &subcktInports = ito->second->getSubcktInports();
						vector<string> &subcktOutports = ito->second->getSubcktOutports();

						for (map<string, map<string, string> *>::const_iterator 
							itoo = ((*(subckts[subcktType]))[curSubcktModelName])->begin(); 
							itoo != ((*(subckts[subcktType]))[curSubcktModelName])->end(); itoo++){

								string tmp = itoo->first;

								vector<string> cometMemVct = this->split(tmp, "++++");
								string cometMemName = cometMemVct.at(0);
								string cometMemInfo = cometMemVct.at(1);

								vector<string> cometMemInfoV = this->split(cometMemInfo, " | ");

								string cometMemLogicName = cometMemInfoV.at(0);
								string cometMemVFile = cometMemInfoV.at(1);
								string memIniFilePath = cometMemInfoV.at(2);

								vector<string> * portA = new vector<string>();
								vector<string> * portB = new vector<string>();

								portA->push_back(cometMemInfoV.at(3));
								portA->push_back(cometMemInfoV.at(4));
								portA->push_back(cometMemInfoV.at(5));
								
								if (cometMemInfoV.size() > 6)
								{
									portB->push_back(cometMemInfoV.at(6));
									portB->push_back(cometMemInfoV.at(7));
									portB->push_back(cometMemInfoV.at(8));
								}

								CometMem *newCometMem = new CometMem(this, cometMemName, curSubcktModelName, RAM_TYPE, cometMemVFile);
								this->addCometMem(*newCometMem);

								newCometMem->setRamIniFilePath(memIniFilePath);
								newCometMem->setPortA(*portA);
								newCometMem->setPortB(*portB);
								newCometMem->setLogicName(cometMemLogicName);

								if (subcktInports.size() == itoo->second->size() - subcktOutports.size()){

									for (vector<string>::const_iterator itooo = subcktInports.begin(); 
										itooo != subcktInports.end(); itooo++)
									{
										string portName =  *itooo;

										if (!(portName.compare("clocken0") &&
											portName.compare("clocken1") &&
											portName.compare("wren_a") &&
											portName.compare("aclr0") &&
											portName.compare("clock0") &&
											portName.compare("wren_b") &&
											portName.compare("aclr1") &&
											portName.compare("clock1") && 
											portName.substr(0, 9).compare("address_a") &&
											portName.substr(0, 9).compare("address_b") && 
											portName.substr(0, 6).compare("data_a") &&
											portName.substr(0, 6).compare("data_b"))) {

												newCometMem->addInoutPortRealName(portName, (*(itoo->second))[portName], 1);										
									
										}
									}
								}

								if (subcktOutports.size() == itoo->second->size() - subcktInports.size()) {
									for (vector<string>::const_iterator itooo = subcktOutports.begin(); 
										itooo != subcktOutports.end(); itooo++)
									{
										string portName =  *itooo;

										if (!(portName.substr(0, 3).compare("q_a") &&
											portName.substr(0, 3).compare("q_b")))
										{
											newCometMem->addInoutPortRealName(portName, (*(itoo->second))[portName], 0);
										}

									}
								}
						}
				}
			}
	} 
}

string Model::getWireRealNameFromInport(InPort &inport)
{
	return inport.getRealName();
}

string Model::genWireNameFromTempWire(Wire &wire, OutPort &outport)
{
	return this->iniWireName + outport.getPnode().getName() 
		+ "[" + outport.getAutoName() +"]"+ "~" + wire.getHierInportName() ;
}

map<string, OutPort *> & Model::getInportsPseudoOutports(void)
{
	return inportsPseudoOutports;
}

map<string, int> & Model::getInportStatus(void)
{
	return this->inportStatus;
}

map<string, LE *> & Model::getLEs(void)
{
	return this->les;
}

map<string, InPort *> & Model::getOutportsPseudoInports(void)
{
	return outportsPseudoInports;
}

map<string, int> & Model::getOutportStatus(void)
{
	return this->outportStatus;
}

string Model::getPhaseShiftDutyCycleVal(string val)
{
	bool quot = false;

	if (val.at(0) == '"')
	{
		quot = true;
		val = this->trim(val.substr(1, val.length() - 2));
	}

	if (val.at(0) == '.') 
	{
		return (quot ? "\"0\"" : "0");
	}

	int index = val.find_first_of('.');

	if (index != string::npos)
	{
		return (quot ? "\"" + val.substr(0, index) + "\"" : val.substr(0, index));
	} else {
		return (quot ? "\"" + val + "\"" : val);
	}
}

long Model::getPllCount(void) const
{
	return this->pllCount;
}


map<string, PLL *> & Model::getPLLs(void)
{
	return this->plls;
}

map<string, RamBlock *> & Model::getRamBlocks(void)
{
	return this->ramBlocks;
}

long Model::getRamCount(void) const
{
	return this->ramCount;
}

void Model::maskLut(void)
{
	for (map<string, LUT *>::const_iterator it = luts.begin(); 
		it != luts.end(); it++){
			(*it).second->mask();
	}
}

void Model::maskRecovery(void)
{
	for (map<string, LUT *>::const_iterator itLUT = luts.begin(); itLUT != luts.end(); itLUT++)
	{
		if (itLUT->second != NULL)
		{
			if (!itLUT->second->isBuffer())
			{
				itLUT->second->setMaskMode("dcba");
				itLUT->second->mask();
			}			
		}
	}
}

void Model::matchCometMemPort( void )
{
	for (map<string, CometMem *>::const_iterator it = cometMems.begin(); 
		it != cometMems.end(); it++){
			(*it).second->matchPort();
	}
}

void Model::matchCometPllPort(void)
{
	for (map<string, CometPLL *>::const_iterator it = cometPlls.begin(); 
		it != cometPlls.end(); it++){
			(*it).second->matchPort();
	}
}

void Model::matchLutPort(void)
{
	for (map<string, LUT *>::const_iterator it = luts.begin(); 
		it != luts.end(); it++){
			(*it).second->matchPort();
	}
}

void Model::matchRegPort(void)
{
	for (map<string, Reg *>::const_iterator it = regs.begin(); 
		it != regs.end(); it++){
			(*it).second->matchPort();
	}
}

void Model::pack(void)
{
	// ---------------------------  Version 1.0  -------------------------------

	//this->pack_v1();

	// ---------------------------  Version 2.0  -------------------------------

	this->pack_pack();

}

void Model::pack_pack(void)
{
	// ---------------------------------  Get all REGs  ---------------------------------------

	for (map<string, Reg *>::const_iterator itReg = regs.begin(); 
		itReg != regs.end(); itReg++){

			Reg &curReg = *itReg->second;

			//if(!curReg.getName().compare("reg[82]")){
			//	cout << "get it!" << endl;
			//}

			map<string, InPort *> &regInports = curReg.getInports();
			map<string, OutPort *> &regOutports = curReg.getOutports();

			if (regInports.count("data0") != 0)
			{
				map<string, Wire *> &regInportData0Wires = regInports["data0"]->getWires();

				if(regInportData0Wires.size() == 1){

					map<string, Wire *>::const_iterator itRegInportWire = regInportData0Wires.begin();

					Node &regFrontNode = (*itRegInportWire->second).getOutport().getPnode();
					string regFrontNodeType = regFrontNode.getNodeType();
					map<string, InPort *> &regFrontNodeInports = regFrontNode.getInports();
					map<string, OutPort *> &regFrontNodeOutports = regFrontNode.getOutports();

					map<string, Wire *> &regOutportWires = regOutports.begin()->second->getWires();

					// Does there exist a LUT(unpacked) in front of the current Reg?
					// --------------------------------  LUT -> Reg -> others ?  -----------------------------------

					if (!(regFrontNodeType.compare(LUT_TYPE) || regFrontNode.isPacked()))
					{
						if (regFrontNodeInports.size() == 4)
						{
							if (regInports.count("adata") == 1)
							{

								// Is qfbk enable?
								// LUT(4-in, qfbk) + Reg(with adata: aload or sload) can be packed together!
								for (map<string, InPort *>::const_iterator itRegFrontNodeInport = regFrontNodeInports.begin(); 
									itRegFrontNodeInport != regFrontNodeInports.end(); itRegFrontNodeInport++)
								{
									string regFrontNodeInportWireName = itRegFrontNodeInport->second->getWires().begin()->first;

									if (regOutportWires.count(regFrontNodeInportWireName) != 0)
									{
										// LUT(4-in, qfbk) + Reg(with adata: aload or sload) can be packed together!

										curReg.setPacked(true);
										regFrontNode.setPacked(true);

										// ------------------------------------------  Packing...  -----------------------------------------------

										LE * tempLE = new LE(this, this->genLEName(regFrontNode.getName(), itReg->first));
										les[tempLE->getName()] = tempLE;

										tempLE->setLUT((LUT &)regFrontNode);   // Warning!!!!!!!!!!
										tempLE->setReg(curReg);

										// set defparams 

										tempLE->setDefparams(tempLE->defParamName[0], "normal");         // "operation_mode"
										tempLE->setDefparams(tempLE->defParamName[1], "on");         // "synch_mode"
										tempLE->setDefparams(tempLE->defParamName[2], "off");         // "register_cascade_mode"
										tempLE->setDefparams(tempLE->defParamName[3], "qfbk");         // "sum_lutc_input"
										tempLE->setDefparams(tempLE->defParamName[4], regFrontNode.getLutMask());         // "lut_mask" 
										tempLE->setDefparams(tempLE->defParamName[5], "reg_only");         // "output_mode"

										// add inports, outports		

										if(regInports.count("sload") == 0 && regInports.count("sclr") == 0){
											tempLE->setDefparams(tempLE->defParamName[1], "off");         // "synch_mode"
										}

										if(regInports.count("aclr") == 0){
											tempLE->addInport(tempLE->inportName[5], "gnd");   // aclr -> gnd
										}

										tempLE->addInport(tempLE->inportName[3], "vcc");   // datac(adata) -> 1

										// Reg inports

										for (map<string, InPort *>::const_iterator itRegInport = regInports.begin(); 
											itRegInport != regInports.end(); itRegInport++)
										{
											if (itRegInport->second != NULL)
											{
												string regInportName = itRegInport->first;

												if (!(regInportName.compare(tempLE->inportName[0]) &&           // clk 
													regInportName.compare(tempLE->inportName[5]) &&              // aclr
													regInportName.compare(tempLE->inportName[6]) &&              // aload
													regInportName.compare(tempLE->inportName[7]) &&              // sclr 
													regInportName.compare(tempLE->inportName[8]) &&              // sload
													regInportName.compare(tempLE->inportName[9])))                  // ena
												{
													tempLE->addInport(regInportName, itRegInport->second->getRealName());
												}
											}
										}

										// Reg outports

										if (regOutportWires.size() > 1)
										{
											for (map<string, OutPort *>::const_iterator itRegOutport = regOutports.begin(); 
												itRegOutport != regOutports.end(); itRegOutport++)
											{
												if (itRegOutport->second != NULL)
												{

													string regOutportName = itRegOutport->first;

													if (!regOutportName.compare(tempLE->outportName[1]))                  // regout
													{
														tempLE->addOutport(regOutportName, itRegOutport->second->getRealName());
													}
												}
											}
										}

										// LUT inports

										// reg -> which std port of LUT ??? 
										// Change only the mask, not the port-to-wire relation of the LUT

										string regOutToLUTInStdName =	
											regOutportWires[regFrontNodeInportWireName]->getInport().getAutoName();

										char ch = regOutToLUTInStdName.at(4);

										switch(ch){
										case 'a':
											regFrontNode.setMaskMode("dabc");
											break;
										case 'b':
											regFrontNode.setMaskMode("dbca");
											break;
										case 'c':
											regFrontNode.setMaskMode("dcba");
											break;
										case'd':
											regFrontNode.setMaskMode("cdba");
											break;
										}

										regFrontNode.mask();
										tempLE->setDefparams(tempLE->defParamName[4], regFrontNode.getLutMask());         // "lut_mask" 

										// (virtually) Change the order of LUT ports, 
										// so that it can match the LE ports correctly.

										for (map<string, InPort *>::const_iterator itLutInport = regFrontNodeInports.begin(); 
											itLutInport != regFrontNodeInports.end(); itLutInport++)
										{
											if (itLutInport->second != NULL)
											{
												string regFrontNodeInportName = itLutInport->first;

												if (!(regFrontNodeInportName.compare(tempLE->inportName[1]) &&           // dataa 
													regFrontNodeInportName.compare(tempLE->inportName[2]) &&              // datab
													regFrontNodeInportName.compare(tempLE->inportName[4])))                  // datad
												{
													if (!(regFrontNodeInportName.compare(regOutToLUTInStdName)))
													{
														if (regFrontNodeInports.size() > 1)
														{
															tempLE->addInport(regFrontNodeInportName, 
																regFrontNodeInports[tempLE->inportName[3]]->getRealName());      // datac
														}
													}else{
														tempLE->addInport(regFrontNodeInportName, itLutInport->second->getRealName());
													}
												}
											}
										}

										// LUT outports

										if (regFrontNodeOutports.begin()->second->getWires().size() > 1)
										{
											for (map<string, OutPort *>::const_iterator itRegFrontNodeOutport = regFrontNodeOutports.begin(); 
												itRegFrontNodeOutport != regFrontNodeOutports.end(); itRegFrontNodeOutport++)
											{
												if (itRegFrontNodeOutport->second != NULL)
												{
													string regFrontNodeOutportName = itRegFrontNodeOutport->first;

													if (!regFrontNodeOutportName.compare(tempLE->outportName[0]))                  // combout
													{
														tempLE->addOutport(regFrontNodeOutportName, itRegFrontNodeOutport->second->getRealName());
													}
												}
											}
										}

										// Set defparam "output_mode"

										if (tempLE->getOutports().count(tempLE->outportName[0]) != 0){   // combout

											if (tempLE->getOutports().count(tempLE->outportName[1]) != 0)   // regout
											{
												// "output_mode" = "reg_and_comb"
												tempLE->setDefparams(tempLE->defParamName[5], "reg_and_comb"); 
											}else{

												// "output_mode" = "comb_only"
												tempLE->setDefparams(tempLE->defParamName[5], "comb_only"); 

											}

										}else{

											// "output_mode" = "reg_only"
											tempLE->setDefparams(tempLE->defParamName[5], "reg_only"); 

										}

										// -------------------  Print LE Info  --------------------------

										//this->showLEInfo(*tempLE);

										break;
									}
								}

								if (curReg.isPacked())
								{
									continue;
								}

							}else{

								// LUT(4-in) + Reg(without adata: aload or sload) can be packed together!

								curReg.setPacked(true);
								regFrontNode.setPacked(true);

								// Packing...

								LE * tempLE = new LE(this, this->genLEName(regFrontNode.getName(), itReg->first));
								les[tempLE->getName()] = tempLE;

								tempLE->setLUT((LUT &)regFrontNode);   // Warning!!!!!!!!!!
								tempLE->setReg(curReg);

								// set defparams 

								tempLE->setDefparams(tempLE->defParamName[0], "normal");         // "operation_mode"
								tempLE->setDefparams(tempLE->defParamName[1], "on");         // "synch_mode"  
								tempLE->setDefparams(tempLE->defParamName[2], "off");         // "register_cascade_mode"
								tempLE->setDefparams(tempLE->defParamName[3], "datac");         // "sum_lutc_input"
								tempLE->setDefparams(tempLE->defParamName[4], regFrontNode.getLutMask());         // "lut_mask" 
								tempLE->setDefparams(tempLE->defParamName[5], "reg_only");         // "output_mode"

								// add inports, outports		

								if(regInports.count("sload") == 0 && regInports.count("sclr") == 0){
									tempLE->setDefparams(tempLE->defParamName[1], "off");         // "synch_mode"
								}

								if(regInports.count("aclr") == 0){
									tempLE->addInport(tempLE->inportName[5], "gnd");   // aclr -> gnd
								}	

								// Reg inports

								for (map<string, InPort *>::const_iterator itRegInport = regInports.begin(); 
									itRegInport != regInports.end(); itRegInport++)
								{
									if (itRegInport->second != NULL)
									{
										string regInportName = itRegInport->first;

										if (!(regInportName.compare(tempLE->inportName[0]) &&           // clk 
											regInportName.compare(tempLE->inportName[5]) &&              // aclr
											regInportName.compare(tempLE->inportName[6]) &&              // aload
											regInportName.compare(tempLE->inportName[7]) &&              // sclr 
											regInportName.compare(tempLE->inportName[8]) &&              // sload
											regInportName.compare(tempLE->inportName[9])))                  // ena
										{
											tempLE->addInport(regInportName, itRegInport->second->getRealName());
										}
									}
								}



								// LUT inports

								// Is qfbk enable?

								bool qfbk = false;

								for (map<string, InPort *>::const_iterator itRegFrontNodeInport = regFrontNodeInports.begin(); 
									itRegFrontNodeInport != regFrontNodeInports.end(); itRegFrontNodeInport++)
								{
									string regFrontNodeInportWireName = itRegFrontNodeInport->second->getWires().begin()->first;

									if (regOutportWires.count(regFrontNodeInportWireName) != 0)
									{
										// Exists qfbk !!!	

										qfbk = true;

										tempLE->setDefparams(tempLE->defParamName[3], "qfbk");         // "sum_lutc_input"

										// reg -> which std port of LUT ??? 
										// Change only the mask, not the port-to-wire relation of the LUT

										string regOutToLUTInStdName =	
											regOutportWires[regFrontNodeInportWireName]->getInport().getAutoName();

										char ch = regOutToLUTInStdName.at(4);

										switch(ch){
										case 'a':
											regFrontNode.setMaskMode("dabc");
											break;
										case 'b':
											regFrontNode.setMaskMode("dbca");
											break;
										case 'c':
											regFrontNode.setMaskMode("dcba");
											break;
										case'd':
											regFrontNode.setMaskMode("cdba");
											break;
										}

										regFrontNode.mask();
										tempLE->setDefparams(tempLE->defParamName[4], regFrontNode.getLutMask());         // "lut_mask" 

										// (virtually) Change the order of LUT ports, 
										// so that it can match the LE ports correctly.

										for (map<string, InPort *>::const_iterator itLutInport = regFrontNodeInports.begin(); 
											itLutInport != regFrontNodeInports.end(); itLutInport++)
										{
											if (itLutInport->second != NULL)
											{
												string regFrontNodeInportName = itLutInport->first;

												if (!(regFrontNodeInportName.compare(tempLE->inportName[1]) &&           // dataa 
													regFrontNodeInportName.compare(tempLE->inportName[2]) &&              // datab
													regFrontNodeInportName.compare(tempLE->inportName[4])))                  // datad
												{
													if (!(regFrontNodeInportName.compare(regOutToLUTInStdName)))
													{
														if (regFrontNodeInports.size() > 1)
														{
															tempLE->addInport(regFrontNodeInportName, 
																regFrontNodeInports[tempLE->inportName[3]]->getRealName());      // datac
														}
													}else{
														tempLE->addInport(regFrontNodeInportName, itLutInport->second->getRealName());
													}
												}
											}
										}

										break;
									}
								}

								// No qfbk ???

								if (!qfbk)
								{
									for (map<string, InPort *>::const_iterator itLutInport = regFrontNodeInports.begin(); 
										itLutInport != regFrontNodeInports.end(); itLutInport++)
									{
										if (itLutInport->second != NULL)
										{
											string regFrontNodeInportName = itLutInport->first;

											if (!(regFrontNodeInportName.compare(tempLE->inportName[1]) &&           // dataa 
												regFrontNodeInportName.compare(tempLE->inportName[2]) &&              // datab
												regFrontNodeInportName.compare(tempLE->inportName[3]) &&              // datac
												regFrontNodeInportName.compare(tempLE->inportName[4])))                  // datad
											{
												tempLE->addInport(regFrontNodeInportName, itLutInport->second->getRealName());
											}
										}
									}
								}

								// Reg outports

								if (!(regOutportWires.size() <= 1 && qfbk))
								{
									for (map<string, OutPort *>::const_iterator itRegOutport = regOutports.begin(); 
										itRegOutport != regOutports.end(); itRegOutport++)
									{
										if (itRegOutport->second != NULL)
										{
											string regOutportName = itRegOutport->first;

											if (!regOutportName.compare(tempLE->outportName[1]))                  // regout
											{
												tempLE->addOutport(regOutportName, itRegOutport->second->getRealName());
											}
										}
									}
								}

								// LUT outports

								if (regFrontNodeOutports.begin()->second->getWires().size() > 1)
								{
									for (map<string, OutPort *>::const_iterator itRegFrontNodeOutport = regFrontNodeOutports.begin(); 
										itRegFrontNodeOutport != regFrontNodeOutports.end(); itRegFrontNodeOutport++)
									{
										if (itRegFrontNodeOutport->second != NULL)
										{
											string regFrontNodeOutportName = itRegFrontNodeOutport->first;

											if (!regFrontNodeOutportName.compare(tempLE->outportName[0]))                  // combout
											{
												tempLE->addOutport(regFrontNodeOutportName, itRegFrontNodeOutport->second->getRealName());
											}
										}
									}
								}

								// Set defparam "output_mode"

								if (tempLE->getOutports().count(tempLE->outportName[0]) != 0){   // combout

									if (tempLE->getOutports().count(tempLE->outportName[1]) != 0)   // regout
									{
										// "output_mode" = "reg_and_comb"
										tempLE->setDefparams(tempLE->defParamName[5], "reg_and_comb"); 
									}else{

										// "output_mode" = "comb_only"
										tempLE->setDefparams(tempLE->defParamName[5], "comb_only"); 

									}

								}else{

									// "output_mode" = "reg_only"
									tempLE->setDefparams(tempLE->defParamName[5], "reg_only"); 

								}

								// -------------------  Print LE Info  --------------------------

								//this->showLEInfo(*tempLE);

								continue;

							}

						}else{

							// LUT( < 4-in ) + Reg can be packed together!

							// Packing...

							curReg.setPacked(true);
							regFrontNode.setPacked(true);

							LE * tempLE = new LE(this, this->genLEName(regFrontNode.getName(), itReg->first));
							les[tempLE->getName()] = tempLE;

							tempLE->setLUT((LUT &)regFrontNode);   // Warning!!!!!!!!!!
							tempLE->setReg(curReg);

							// set defparams 

							tempLE->setDefparams(tempLE->defParamName[0], "normal");         // "operation_mode"
							tempLE->setDefparams(tempLE->defParamName[1], "on");         // "synch_mode"
							tempLE->setDefparams(tempLE->defParamName[2], "off");         // "register_cascade_mode"   
							tempLE->setDefparams(tempLE->defParamName[3], "datac");         // "sum_lutc_input"
							tempLE->setDefparams(tempLE->defParamName[4], regFrontNode.getLutMask());         // "lut_mask"
							tempLE->setDefparams(tempLE->defParamName[5], "reg_only");         // "output_mode"

							// add inports, outports		

							if(regInports.count("sload") == 0 && regInports.count("sclr") == 0){
								tempLE->setDefparams(tempLE->defParamName[1], "off");         // "synch_mode"
							}

							if(regInports.count("aclr") == 0){
								tempLE->addInport(tempLE->inportName[5], "gnd");   // aclr -> gnd
							}					

							// Exists adata?

							if (regInports.count("adata") != 0)
							{
								tempLE->addInport(tempLE->inportName[3], "vcc");   // datac(adata) -> 1	
							}

							// Reg inports

							for (map<string, InPort *>::const_iterator itRegInport = regInports.begin(); 
								itRegInport != regInports.end(); itRegInport++)
							{
								if (itRegInport->second != NULL)
								{
									string regInportName = itRegInport->first;

									if (!(regInportName.compare(tempLE->inportName[0]) &&           // clk 
										regInportName.compare(tempLE->inportName[5]) &&              // aclr
										regInportName.compare(tempLE->inportName[6]) &&              // aload
										regInportName.compare(tempLE->inportName[7]) &&              // sclr 
										regInportName.compare(tempLE->inportName[8]) &&              // sload
										regInportName.compare(tempLE->inportName[9])))                  // ena
									{
										tempLE->addInport(regInportName, itRegInport->second->getRealName());
									}
								}
							}

							// LUT inports

							// Is qfbk enable?

							bool qfbk = false;

							for (map<string, InPort *>::const_iterator itRegFrontNodeInport = regFrontNodeInports.begin(); 
								itRegFrontNodeInport != regFrontNodeInports.end(); itRegFrontNodeInport++)
							{
								string regFrontNodeInportWireName = itRegFrontNodeInport->second->getWires().begin()->first;

								if (regOutportWires.count(regFrontNodeInportWireName) != 0)
								{
									// Exists qfbk !!!

									qfbk = true;

									tempLE->setDefparams(tempLE->defParamName[3], "qfbk");         // "sum_lutc_input" 

									// reg -> which std port of LUT ??? 
									// Change only the mask, not the port-to-wire relation of the LUT

									string regOutToLUTInStdName =	
										regOutportWires[regFrontNodeInportWireName]->getInport().getAutoName();

									char ch = regOutToLUTInStdName.at(4);

									switch(ch){
									case 'a':
										regFrontNode.setMaskMode("dabc");
										break;
									case 'b':
										regFrontNode.setMaskMode("dbca");
										break;
									case 'c':
										regFrontNode.setMaskMode("dcba");
										break;
									case'd':
										regFrontNode.setMaskMode("cdba");
										break;
									}

									regFrontNode.mask();
									tempLE->setDefparams(tempLE->defParamName[4], regFrontNode.getLutMask());         // "lut_mask" 

									// (virtually) Change the order of LUT ports, 
									// so that it can match the LE ports correctly.

									for (map<string, InPort *>::const_iterator itLutInport = regFrontNodeInports.begin(); 
										itLutInport != regFrontNodeInports.end(); itLutInport++)
									{
										if (itLutInport->second != NULL)
										{
											string regFrontNodeInportName = itLutInport->first;

											if (!(regFrontNodeInportName.compare(tempLE->inportName[1]) &&           // dataa 
												regFrontNodeInportName.compare(tempLE->inportName[2]) &&              // datab
												regFrontNodeInportName.compare(tempLE->inportName[4])))                  // datad
											{
												if (!(regFrontNodeInportName.compare(regOutToLUTInStdName)))
												{
													// regOutToLUTInStdPort conn to qfbk
													// original LUT datac (virtually) change to regOutToLUTInStdPort

													if (regFrontNodeInports.size() > 1)
													{
														tempLE->addInport(regFrontNodeInportName, 
															regFrontNodeInports[tempLE->inportName[3]]->getRealName());  
													}

												}else{
													tempLE->addInport(regFrontNodeInportName, itLutInport->second->getRealName());
												}
											}
										}
									}

									break;
								}
							}							

							// No qfbk ???

							if (!qfbk)
							{
								// Exists adata ???

								if (regInports.count("adata") != 0)
								{
									// Original LUT port order unchanged, only change mask and port order in LE
									regFrontNode.setMaskMode("dbac");
									regFrontNode.mask();
									tempLE->setDefparams(tempLE->defParamName[4], regFrontNode.getLutMask());   // "lut_mask"


									for (map<string, InPort *>::const_iterator itLutInport = regFrontNodeInports.begin(); 
										itLutInport != regFrontNodeInports.end(); itLutInport++)
									{
										if (itLutInport->second != NULL)
										{
											string regFrontNodeInportName = itLutInport->first;

											// datad

											if(!(regFrontNodeInportName.compare(tempLE->inportName[4])))
											{
												tempLE->addInport(regFrontNodeInportName, itLutInport->second->getRealName());
											}

											// datac -> datab

											else if(!(regFrontNodeInportName.compare(tempLE->inportName[3])))
											{
												tempLE->addInport(tempLE->inportName[2], itLutInport->second->getRealName());
											}

											// datab -> dataa

											else if(!(regFrontNodeInportName.compare(tempLE->inportName[2])))
											{
												tempLE->addInport(tempLE->inportName[1], itLutInport->second->getRealName());
											}

											// dataa

											else if(!(regFrontNodeInportName.compare(tempLE->inportName[1])))
											{

											}
										}
									}
								}else{

									// No adata !!!

									for (map<string, InPort *>::const_iterator itLutInport = regFrontNodeInports.begin(); 
										itLutInport != regFrontNodeInports.end(); itLutInport++)
									{
										if (itLutInport->second != NULL)
										{
											string regFrontNodeInportName = itLutInport->first;

											if (!(regFrontNodeInportName.compare(tempLE->inportName[1]) &&           // dataa 
												regFrontNodeInportName.compare(tempLE->inportName[2]) &&              // datab
												regFrontNodeInportName.compare(tempLE->inportName[3]) &&              // datac
												regFrontNodeInportName.compare(tempLE->inportName[4])))                  // datad
											{
												tempLE->addInport(regFrontNodeInportName, itLutInport->second->getRealName());
											}
										}
									}
								}
							}

							// Reg outports

							if (!(regOutportWires.size() <= 1 && qfbk))
							{
								for (map<string, OutPort *>::const_iterator itRegOutport = regOutports.begin(); 
									itRegOutport != regOutports.end(); itRegOutport++)
								{
									if (itRegOutport->second != NULL)
									{
										string regOutportName = itRegOutport->first;

										if (!regOutportName.compare(tempLE->outportName[1]))                  // regout
										{
											tempLE->addOutport(regOutportName, itRegOutport->second->getRealName());
										}
									}
								}
							}

							// LUT outports

							if (regFrontNodeOutports.begin()->second->getWires().size() > 1)
							{
								for (map<string, OutPort *>::const_iterator itRegFrontNodeOutport = regFrontNodeOutports.begin(); 
									itRegFrontNodeOutport != regFrontNodeOutports.end(); itRegFrontNodeOutport++)
								{
									if (itRegFrontNodeOutport->second != NULL)
									{
										string regFrontNodeOutportName = itRegFrontNodeOutport->first;

										if (!regFrontNodeOutportName.compare(tempLE->outportName[0]))                  // combout
										{
											tempLE->addOutport(regFrontNodeOutportName, itRegFrontNodeOutport->second->getRealName());
										}
									}
								}
							}

							// Set defparam "output_mode"

							if (tempLE->getOutports().count(tempLE->outportName[0]) != 0){   // combout

								if (tempLE->getOutports().count(tempLE->outportName[1]) != 0)   // regout
								{
									// "output_mode" = "reg_and_comb"
									tempLE->setDefparams(tempLE->defParamName[5], "reg_and_comb"); 
								}else{

									// "output_mode" = "comb_only"
									tempLE->setDefparams(tempLE->defParamName[5], "comb_only"); 

								}

							}else{

								// "output_mode" = "reg_only"
								tempLE->setDefparams(tempLE->defParamName[5], "reg_only"); 

							}

							// -------------------  Print LE Info  --------------------------

							//this->showLEInfo(*tempLE);

							continue;
						}

					}

					// --------------------------------  not LUT -> Reg -> LUT(exist) ?  -----------------------------------

					for (map<string, Wire *>::const_iterator itRegOutportWire = regOutportWires.begin(); 
						itRegOutportWire != regOutportWires.end(); itRegOutportWire++)
					{
						Node &regBackNode = itRegOutportWire->second->getInport().getPnode();
						string regBackNodeType = regBackNode.getNodeType();
						map<string, OutPort *> &regBackNodeOutports = regBackNode.getOutports();

						map<string, InPort *> &regBackNodeInports = regBackNode.getInports();

						if (!(regBackNodeType.compare(LUT_TYPE) || regBackNode.isPacked())){

							if (curReg.getInports().count("adata") != 0)
							{

								// Reg -> Reg(with adata: aload or sload) + LUT can be packed together!

								if ((!regFrontNodeType.compare(REG_TYPE)) && regCascade)
								{
									// Reg -> Reg(with adata: aload or sload) + LUT(4-in) can be packed together!

									// Packing...

									curReg.setPacked(true);
									regBackNode.setPacked(true);

									LE * tempLE = new LE(this, this->genLEName(regBackNode.getName(), itReg->first));
									les[tempLE->getName()] = tempLE;

									tempLE->setLUT((LUT &)regBackNode);   // Warning!!!!!!!!!!
									tempLE->setReg(curReg);

									// set defparams 

									tempLE->setDefparams(tempLE->defParamName[0], "normal");         // "operation_mode"
									tempLE->setDefparams(tempLE->defParamName[1], "on");         // "synch_mode"
									tempLE->setDefparams(tempLE->defParamName[2], "on");         // "register_cascade_mode"
									tempLE->setDefparams(tempLE->defParamName[3], "qfbk");         // "sum_lutc_input"
									tempLE->setDefparams(tempLE->defParamName[4], regBackNode.getLutMask());         // "lut_mask"
									tempLE->setDefparams(tempLE->defParamName[5], "reg_only");         // "output_mode"

									// add inports, outports		

									if(regInports.count("sload") == 0 && regInports.count("sclr") == 0){
										tempLE->setDefparams(tempLE->defParamName[1], "off");         // "synch_mode"
									}

									if(regInports.count("aclr") == 0){
										tempLE->addInport(tempLE->inportName[5], "gnd");   // aclr -> gnd
									}

									tempLE->addInport(tempLE->inportName[3], "vcc");   // datac(adata) -> 1

									// Reg inports

									for (map<string, InPort *>::const_iterator itRegInport = regInports.begin(); 
										itRegInport != regInports.end(); itRegInport++)
									{
										if (itRegInport->second != NULL)
										{
											string regInportName = itRegInport->first;

											if (!(regInportName.compare(tempLE->inportName[0]) &&           // clk 
												regInportName.compare(tempLE->inportName[5]) &&              // aclr
												regInportName.compare(tempLE->inportName[6]) &&              // aload
												regInportName.compare(tempLE->inportName[7]) &&              // sclr 
												regInportName.compare(tempLE->inportName[8]) &&              // sload
												regInportName.compare(tempLE->inportName[9])))                  // ena
											{
												tempLE->addInport(regInportName, itRegInport->second->getRealName());
											}

											if (!(regInportName.compare("data0")))           // regcascin
											{
												tempLE->addInport(tempLE->inportName[14], itRegInport->second->getRealName());
											}

										}
									}

									// Reg outports

									if (regOutportWires.size() > 1)
									{
										for (map<string, OutPort *>::const_iterator itRegOutport = regOutports.begin(); 
											itRegOutport != regOutports.end(); itRegOutport++)
										{
											if (itRegOutport->second != NULL)
											{
												string regOutportName = itRegOutport->first;

												if (!regOutportName.compare(tempLE->outportName[1]))                  // regout
												{
													tempLE->addOutport(regOutportName, itRegOutport->second->getRealName());
												}
											}
										}
									}

									// LUT inports

									// reg -> which std port of LUT ??? 
									// Change only the mask, not the port-to-wire relation of the LUT

									string regOutToLUTInStdName =	
										itRegOutportWire->second->getInport().getAutoName();

									char ch = regOutToLUTInStdName.at(4);

									switch(ch){
									case 'a':
										regBackNode.setMaskMode("dabc");
										break;
									case 'b':
										regBackNode.setMaskMode("dbca");
										break;
									case 'c':
										regBackNode.setMaskMode("dcba");
										break;
									case'd':
										regBackNode.setMaskMode("cdba");
										break;
									}

									regBackNode.mask();
									tempLE->setDefparams(tempLE->defParamName[4], regBackNode.getLutMask());         // "lut_mask" 

									// (virtually) Change the order of LUT ports, 
									// so that it can match the LE ports correctly.

									for (map<string, InPort *>::const_iterator itLutInport = regBackNodeInports.begin(); 
										itLutInport != regBackNodeInports.end(); itLutInport++)
									{
										if (itLutInport->second != NULL)
										{
											string regBackNodeInportName = itLutInport->first;

											if (!(regBackNodeInportName.compare(tempLE->inportName[1]) &&           // dataa 
												regBackNodeInportName.compare(tempLE->inportName[2]) &&              // datab
												regBackNodeInportName.compare(tempLE->inportName[4])))                  // datad
											{
												if (!(regBackNodeInportName.compare(regOutToLUTInStdName)))
												{
													if (regBackNodeInports.size() > 1)
													{
														tempLE->addInport(regBackNodeInportName, 
															regBackNodeInports[tempLE->inportName[3]]->getRealName());      // datac
													}
												}else{
													tempLE->addInport(regBackNodeInportName, itLutInport->second->getRealName());
												}
											}
										}
									}

									// LUT outports

									for (map<string, OutPort *>::const_iterator itRegBackNodeOutport = regBackNodeOutports.begin(); 
										itRegBackNodeOutport != regBackNodeOutports.end(); itRegBackNodeOutport++)
									{
										if (itRegBackNodeOutport->second != NULL)
										{
											string regBackNodeOutportName = itRegBackNodeOutport->first;

											if (!regBackNodeOutportName.compare(tempLE->outportName[0]))                  // combout
											{
												tempLE->addOutport(regBackNodeOutportName, itRegBackNodeOutport->second->getRealName());
											}
										}
									}

									// Set defparam "output_mode"

									if (tempLE->getOutports().count(tempLE->outportName[0]) != 0){   // combout

										if (tempLE->getOutports().count(tempLE->outportName[1]) != 0)   // regout
										{
											// "output_mode" = "reg_and_comb"
											tempLE->setDefparams(tempLE->defParamName[5], "reg_and_comb"); 
										}else{

											// "output_mode" = "comb_only"
											tempLE->setDefparams(tempLE->defParamName[5], "comb_only"); 

										}

									}else{

										// "output_mode" = "reg_only"
										tempLE->setDefparams(tempLE->defParamName[5], "reg_only"); 

									}

									// -------------------  Print LE Info  --------------------------

									//this->showLEInfo(*tempLE);

									break;

								}else{
									continue;
								}

							}

							// Reg(without adata: aload or sload) + LUT can be packed together!

							else{

								// Reg(without adata: aload or sload) + LUT can be packed together!

								// Packing...

								curReg.setPacked(true);
								regBackNode.setPacked(true);

								LE * tempLE = new LE(this, this->genLEName(regBackNode.getName(), itReg->first));
								les[tempLE->getName()] = tempLE;

								tempLE->setLUT((LUT &)regBackNode);   // Warning!!!!!!!!!!
								tempLE->setReg(curReg);

								// set defparams 

								tempLE->setDefparams(tempLE->defParamName[0], "normal");         // "operation_mode"
								tempLE->setDefparams(tempLE->defParamName[1], "on");         // "synch_mode"
								tempLE->setDefparams(tempLE->defParamName[2], "off");         // "register_cascade_mode"
								tempLE->setDefparams(tempLE->defParamName[3], "qfbk");         // "sum_lutc_input"
								tempLE->setDefparams(tempLE->defParamName[4], regBackNode.getLutMask());         // "lut_mask" ???
								tempLE->setDefparams(tempLE->defParamName[5], "reg_only");         // "output_mode"

								// add inports, outports		

								if(regInports.count("aclr") == 0){
									tempLE->addInport(tempLE->inportName[5], "gnd");   // aclr -> gnd
								}

								tempLE->addInport(tempLE->inportName[8], "vcc");   // sload -> 1

								// Reg inports

								for (map<string, InPort *>::const_iterator itRegInport = regInports.begin(); 
									itRegInport != regInports.end(); itRegInport++)
								{
									if (itRegInport->second != NULL)
									{
										string regInportName = itRegInport->first;

										if (!(regInportName.compare(tempLE->inportName[0]) &&           // clk 
											regInportName.compare(tempLE->inportName[5]) &&              // aclr
											regInportName.compare(tempLE->inportName[6]) &&              // aload
											regInportName.compare(tempLE->inportName[7]) &&              // sclr 
											regInportName.compare(tempLE->inportName[8]) &&              // sload
											regInportName.compare(tempLE->inportName[9])))                  // ena
										{
											tempLE->addInport(regInportName, itRegInport->second->getRealName());
										}

										if (!(regInportName.compare("data0")))           // datac -> data0
										{
											tempLE->addInport(tempLE->inportName[3], itRegInport->second->getRealName());
										}

									}
								}

								// Reg outports

								if (regOutportWires.size() > 1)
								{
									for (map<string, OutPort *>::const_iterator itRegOutport = regOutports.begin(); 
										itRegOutport != regOutports.end(); itRegOutport++)
									{
										if (itRegOutport->second != NULL)
										{
											string regOutportName = itRegOutport->first;

											if (!regOutportName.compare(tempLE->outportName[1]))                  // regout
											{
												tempLE->addOutport(regOutportName, itRegOutport->second->getRealName());
											}
										}
									}
								}

								// LUT inports

								// reg -> which std port of LUT ??? 
								// Change only the mask, not the port-to-wire relation of the LUT

								string regOutToLUTInStdName =	
									itRegOutportWire->second->getInport().getAutoName();

								char ch = regOutToLUTInStdName.at(4);

								switch(ch){
								case 'a':
									regBackNode.setMaskMode("dabc");
									break;
								case 'b':
									regBackNode.setMaskMode("dbca");
									break;
								case 'c':
									regBackNode.setMaskMode("dcba");
									break;
								case'd':
									regBackNode.setMaskMode("cdba");
									break;
								}

								regBackNode.mask();
								tempLE->setDefparams(tempLE->defParamName[4], regBackNode.getLutMask());         // "lut_mask" 

								// (virtually) Change the order of LUT ports, 
								// so that it can match the LE ports correctly.

								for (map<string, InPort *>::const_iterator itLutInport = regBackNodeInports.begin(); 
									itLutInport != regBackNodeInports.end(); itLutInport++)
								{
									if (itLutInport->second != NULL)
									{
										string regBackNodeInportName = itLutInport->first;

										if (!(regBackNodeInportName.compare(tempLE->inportName[1]) &&           // dataa 
											regBackNodeInportName.compare(tempLE->inportName[2]) &&              // datab
											regBackNodeInportName.compare(tempLE->inportName[4])))                  // datad
										{
											if (!(regBackNodeInportName.compare(regOutToLUTInStdName)))
											{
												if (regBackNodeInports.size() > 1)
												{
													tempLE->addInport(regBackNodeInportName, 
														regBackNodeInports[tempLE->inportName[3]]->getRealName());      // datac
												}
											}else{
												tempLE->addInport(regBackNodeInportName, itLutInport->second->getRealName());
											}
										}
									}
								}

								// LUT outports

								for (map<string, OutPort *>::const_iterator itRegBackNodeOutport = regBackNodeOutports.begin(); 
									itRegBackNodeOutport != regBackNodeOutports.end(); itRegBackNodeOutport++)
								{
									if (itRegBackNodeOutport->second != NULL)
									{
										string regBackNodeOutportName = itRegBackNodeOutport->first;

										if (!regBackNodeOutportName.compare(tempLE->outportName[0]))                  // combout
										{
											tempLE->addOutport(regBackNodeOutportName, itRegBackNodeOutport->second->getRealName());
										}
									}
								}

								// Set defparam "output_mode"

								if (tempLE->getOutports().count(tempLE->outportName[0]) != 0){   // combout

									if (tempLE->getOutports().count(tempLE->outportName[1]) != 0)   // regout
									{
										// "output_mode" = "reg_and_comb"
										tempLE->setDefparams(tempLE->defParamName[5], "reg_and_comb"); 
									}else{

										// "output_mode" = "comb_only"
										tempLE->setDefparams(tempLE->defParamName[5], "comb_only"); 

									}

								}else{

									// "output_mode" = "reg_only"
									tempLE->setDefparams(tempLE->defParamName[5], "reg_only"); 

								}

								// -------------------  Print LE Info  --------------------------

								//this->showLEInfo(*tempLE);

								break;

							}

						}

						if (curReg.isPacked())
						{
							break;
						}

					}

					// Reg still unpacked ??? 

					if (curReg.isPacked())
					{
						continue;
					}

					// -------------------------------  not LUT -> Reg -> not exist LUT ?  -----------------------------------

					LE * tempLE = new LE(this, this->genLEName(itReg->first, ""));
					les[tempLE->getName()] = tempLE;

					tempLE->setReg(curReg);

					// set defparams 

					tempLE->setDefparams(tempLE->defParamName[0], "normal");         // "operation_mode"
					tempLE->setDefparams(tempLE->defParamName[1], "on");         // "synch_mode"
					tempLE->setDefparams(tempLE->defParamName[2], "off");         // "register_cascade_mode"
					tempLE->setDefparams(tempLE->defParamName[3], "datac");         // "sum_lutc_input"
					tempLE->setDefparams(tempLE->defParamName[4], "F0F0");         // "lut_mask"
					tempLE->setDefparams(tempLE->defParamName[5], "reg_only");         // "output_mode"

					// add inports, outports

					if (regInports.count("adata") == 0)
					{
						tempLE->addInport(tempLE->inportName[8], "vcc");   // sload -> 1

						if (regInports.count("data0") != 0)
						{
							tempLE->addInport(tempLE->inportName[3], (regInports["data0"])->getRealName());  // datac -> d
						}

					}else{

						tempLE->addInport(tempLE->inportName[4], (regInports["data0"])->getRealName());  // datad -> d
						tempLE->addInport(tempLE->inportName[3], "vcc");   // datac(adata) -> 1

						tempLE->setDefparams(tempLE->defParamName[4], "FF00");         // "lut_mask"

						if(regInports.count("sload") == 0){		

							if(regInports.count("sclr") == 0){
								tempLE->setDefparams(tempLE->defParamName[1], "off");         // "synch_mode"
							}

						}else{

							tempLE->addInport(tempLE->inportName[8], (regInports["sload"])->getRealName());   // sload

						}	

					}

					for (map<string, InPort *>::const_iterator ito = regInports.begin(); 
						ito != regInports.end(); ito++)
					{
						if (ito->second != NULL)
						{
							string portName = ito->first;

							if (!(portName.compare(tempLE->inportName[0]) &&           // clk 
								portName.compare(tempLE->inportName[5]) &&              // aclr
								portName.compare(tempLE->inportName[6]) &&              // aload
								portName.compare(tempLE->inportName[7]) &&              // sclr
								portName.compare(tempLE->inportName[9])))                  // ena
							{
								tempLE->addInport(portName, ito->second->getRealName());
							}
						}
					}

					for (map<string, OutPort *>::const_iterator ito = regOutports.begin(); 
						ito != regOutports.end(); ito++)
					{
						if (ito->second != NULL)
						{
							string portName = ito->first;

							if (!portName.compare(tempLE->outportName[1]))                  // regout
							{
								tempLE->addOutport(portName, ito->second->getRealName());
							}
						}
					}

					curReg.setPacked(true);

					// -------------------  Print LE Info  --------------------------

					//this->showLEInfo(*tempLE);

				}

			}

	}

	// ---------------------------------  Get all LUTs  ---------------------------------------

	for (map<string, LUT *>::const_iterator itLut = luts.begin(); 
		itLut != luts.end(); itLut++){
			//stratix_lcell \comb~0_I (
			//	.dataa(n0),
			//	.datab(b),
			//	.datac(e),
			//	.datad(a),
			//	.combout(\comb~0 ));
			//defparam \comb~0_I .operation_mode = "normal";
			//defparam \comb~0_I .synch_mode = "off";
			//defparam \comb~0_I .register_cascade_mode = "off";
			//defparam \comb~0_I .sum_lutc_input = "datac";
			//defparam \comb~0_I .lut_mask = "565A";
			//defparam \comb~0_I .output_mode = "comb_only";

			if ((!itLut->second->isBuffer()) && (!itLut->second->isPacked()))
			{
				map<string, InPort *> &lutInport = itLut->second->getInports();
				map<string, OutPort *> &lutOutport = itLut->second->getOutports();

				LE * tempLE = new LE(this, this->genLEName(itLut->first, ""));
				les[tempLE->getName()] = tempLE;

				tempLE->setLUT(*(itLut->second)); 

				// add inports, outports

				for (map<string, InPort *>::const_iterator itLutInport = lutInport.begin(); 
					itLutInport != lutInport.end(); itLutInport++)
				{
					if (itLutInport->second != NULL)
					{
						string portName = itLutInport->first;

						if (!(portName.compare(tempLE->inportName[1]) &&              // dataa 
							portName.compare(tempLE->inportName[2]) &&              // datab
							portName.compare(tempLE->inportName[3]) &&              // datac
							portName.compare(tempLE->inportName[4])))                  // datad
						{
							tempLE->addInport(portName, itLutInport->second->getRealName());
						}
					}
				}

				for (map<string, OutPort *>::const_iterator itLutOutport = lutOutport.begin(); 
					itLutOutport != lutOutport.end(); itLutOutport++)
				{
					if (itLutOutport->second != NULL)
					{
						string portName = itLutOutport->first;

						if (!portName.compare(tempLE->outportName[0]))                  // combout
						{
							tempLE->addOutport(portName, itLutOutport->second->getRealName());
						}
					}
				}

				// set defparams 

				tempLE->setDefparams(tempLE->defParamName[0], "normal");         // "operation_mode"
				tempLE->setDefparams(tempLE->defParamName[1], "off");         // "synch_mode"
				tempLE->setDefparams(tempLE->defParamName[2], "off");         // "register_cascade_mode"
				tempLE->setDefparams(tempLE->defParamName[3], "datac");         // "sum_lutc_input"
				tempLE->setDefparams(tempLE->defParamName[4], itLut->second->getLutMask());         // "lut_mask"
				tempLE->setDefparams(tempLE->defParamName[5], "comb_only");         // "output_mode"

				// -------------------  Print LE Info  --------------------------

				//this->showLEInfo(*tempLE);
			}
	}
}

void Model::pack_pll(void)
{
	for (map<string, CometPLL *>::const_iterator cometpll = cometPlls.begin(); cometpll != cometPlls.end(); cometpll++)
	{

		string pllVFile = cometpll->second->getPllVFile();

		map<string, InPort *> &tempInport = cometpll->second->getInports();
		map<string, OutPort *> &tempOutport = cometpll->second->getOutports();

		PLL * tempPll = new PLL(this, cometpll->first);
		string tempPllName = tempPll->getName();
		plls[tempPllName] = tempPll;

		tempPll->setCometPLL(*(cometpll->second)); 

		// add inports, outports

		for (map<string, InPort *>::const_iterator itPllInport = tempInport.begin(); 
			itPllInport != tempInport.end(); itPllInport++)
		{
			if (itPllInport->second != NULL) {				
				string portName = itPllInport->first;

				if (!(portName.compare("inclk0"))) {
					tempPll->addInport("inclk", "{gnd," + itPllInport->second->getRealName() + "}");				
				} else if (!(portName.compare("pllena"))) {
					tempPll->addInport("ena", itPllInport->second->getRealName());
				} else if (!(portName.compare("areset"))) {
					tempPll->addInport("areset", itPllInport->second->getRealName());
				} else if (!(portName.compare("pfdena"))) {
					tempPll->addInport("pfdena", itPllInport->second->getRealName());
				}
			}
		}

		bool e0State = false;

		string clkEna [] = {"gnd", "gnd", "gnd", "gnd", "gnd", "gnd"};
		string extClkEna [] = {"gnd", "gnd", "gnd", "gnd"};
		
		string clkVal [] = {tempPllName + "_CLK5", tempPllName + "_CLK4", tempPllName + "_CLK3", 
			tempPllName + "_CLK2" , tempPllName + "_CLK1", tempPllName + "_CLK0"};
		string extClkVal [] = {"extclk_unconnected_wire_0", "extclk_unconnected_wire_1", 
			"extclk_unconnected_wire_2", "extclk_unconnected_wire_3"};

		for (map<string, OutPort *>::const_iterator itPllOutport = tempOutport.begin(); 
			itPllOutport != tempOutport.end(); itPllOutport++)
		{
			if (itPllOutport->second != NULL)
			{
				string portName = itPllOutport->first;

				if (!portName.compare("c0")) {
					clkVal[5] = itPllOutport->second->getRealName();
					clkEna[5] = "vcc";
				} else if (!portName.compare("c1")) {
					clkVal[4] = itPllOutport->second->getRealName();
					clkEna[4] = "vcc";
				} else if (!portName.compare("e0")) {
					e0State = true;
					extClkVal[3] = itPllOutport->second->getRealName();
					extClkEna[3] = "vcc";
				} else if (!portName.compare("locked")) {
					tempPll->addOutport("locked", itPllOutport->second->getRealName());
				}
			}
		}

		tempPll->addInport("clkena", "{" + clkEna[0] + "," + clkEna[1] + "," + clkEna[2] + "," 
			+ clkEna[3] + "," + clkEna[4] + "," + clkEna[5] + "}");
		tempPll->addInport("extclkena", "{" + extClkEna[0] + "," + extClkEna[1] + "," 
			+ extClkEna[2] + "," 	+ extClkEna[3] + "}");

		tempPll->addOutport("clk", "{" + clkVal[0] + "," + clkVal[1] + "," + clkVal[2] + "," 
			+ clkVal[3] + "," + clkVal[4] + "," + clkVal[5] + "}");

		if (e0State) {
			tempPll->addOutport("extclk", "{" + extClkVal[0] + "," + extClkVal[1] + "," 
				+ extClkVal[2] + "," + extClkVal[3] + "}");
		}

		// set defparams 

		ifstream icin(pllVFile);

		if (icin.is_open()) {

			string str = "";
			bool isDefFound = false;
			string defLineStr = "";
			string defPara = "";
			string defParaVal = "";

			map<string, string> tmpDef;

			while (getline(icin, str)) {
				
				str = this->trim(str);

				if (str.empty())
				{
					continue;
				}

				if (!str.compare("defparam")) {
					isDefFound = true;
					continue;
				} 
				
				if (!str.compare("endmodule")) {
					isDefFound = false;
				} 

				if (isDefFound) {
				
					int index = 0;
					index = str.find_first_of('.');

					if (index != string::npos && index != (str.length() - 1) 
						&& (str.at(str.length() - 1) == ',' || str.at(str.length() - 1) == ';'))
					{
						defLineStr = str.substr(index + 1);
						defLineStr = this->trim(defLineStr.substr(0, defLineStr.length() - 1));

						defPara = this->trim(this->split(defLineStr, "=").at(0));
						defParaVal = this->trim(this->split(defLineStr, "=").at(1));

						tmpDef[defPara] = defParaVal;
					}
				}
			}

			icin.close();

			if (tmpDef.count("operation_mode") != 0)
			{
					tempPll->setDefparams("operation_mode", this->toLowerCase(tmpDef["operation_mode"]));
			} 

			if (tmpDef.count("pll_type") != 0)
			{
				tempPll->setDefparams("pll_type", this->toLowerCase(tmpDef["pll_type"]));
			} else {
				tempPll->setDefparams("pll_type", "\"enhanced\"");
			}
			
			if (tmpDef.count("qualify_conf_done") != 0)
			{
				tempPll->setDefparams("qualify_conf_done", this->toLowerCase(tmpDef["qualify_conf_done"]));
			} else {
				tempPll->setDefparams("qualify_conf_done", "\"off\"");
			}

			if (tmpDef.count("valid_lock_multiplier") != 0)
			{
				tempPll->setDefparams("valid_lock_multiplier", this->toLowerCase(tmpDef["valid_lock_multiplier"]));
			} else {
				tempPll->setDefparams("valid_lock_multiplier", "1");
			}

			if (tmpDef.count("invalid_lock_multiplier") != 0)
			{
				tempPll->setDefparams("invalid_lock_multiplier", this->toLowerCase(tmpDef["invalid_lock_multiplier"]));
			} else {
				tempPll->setDefparams("invalid_lock_multiplier", "5");
			}

			if (tmpDef.count("compensate_clock") != 0)
			{
				tempPll->setDefparams("compensate_clock", this->toLowerCase(tmpDef["compensate_clock"]));
			}

			if (tmpDef.count("inclk0_input_frequency") != 0)
			{
				tempPll->setDefparams("inclk0_input_frequency", this->toLowerCase(tmpDef["inclk0_input_frequency"]));
			}

			if (tmpDef.count("inclk1_input_frequency") != 0)
			{
				tempPll->setDefparams("inclk1_input_frequency", this->toLowerCase(tmpDef["inclk1_input_frequency"]));
			} else {
				tempPll->setDefparams("inclk1_input_frequency", this->toLowerCase(tmpDef["inclk0_input_frequency"]));
			}

			if (tmpDef.count("skip_vco") != 0)
			{
				tempPll->setDefparams("skip_vco", this->toLowerCase(tmpDef["skip_vco"]));
			} else {
				tempPll->setDefparams("skip_vco", "\"off\"");
			}

			if (tmpDef.count("primary_clock") != 0)
			{
				tempPll->setDefparams("primary_clock", this->toLowerCase(tmpDef["primary_clock"]));
			} else {
				tempPll->setDefparams("primary_clock", "\"inclk0\"");
			}

			if (tmpDef.count("switch_over_on_lossclk") != 0)
			{
				tempPll->setDefparams("switch_over_on_lossclk", this->toLowerCase(tmpDef["switch_over_on_lossclk"]));
			} else {
				tempPll->setDefparams("switch_over_on_lossclk", "\"off\"");
			}

			if (tmpDef.count("switch_over_on_gated_lock") != 0)
			{
				tempPll->setDefparams("switch_over_on_gated_lock", this->toLowerCase(tmpDef["switch_over_on_gated_lock"]));
			} else {
				tempPll->setDefparams("switch_over_on_gated_lock", "\"off\"");
			}

			if (tmpDef.count("enable_switch_over_counter") != 0)
			{
				tempPll->setDefparams("enable_switch_over_counter", this->toLowerCase(tmpDef["enable_switch_over_counter"]));
			} else {
				tempPll->setDefparams("enable_switch_over_counter", "\"off\"");
			}

			if (tmpDef.count("gate_lock_signal") != 0)
			{
				tempPll->setDefparams("gate_lock_signal", this->toLowerCase(tmpDef["gate_lock_signal"]));
			} else {
				tempPll->setDefparams("gate_lock_signal", "\"no\"");
			}

			if (tmpDef.count("gate_lock_counter") != 0)
			{
				tempPll->setDefparams("gate_lock_counter", this->toLowerCase(tmpDef["gate_lock_counter"]));
			} else {
				tempPll->setDefparams("gate_lock_counter", "0");
			}

			if (tmpDef.count("switch_over_counter") != 0)
			{
				tempPll->setDefparams("switch_over_counter", this->toLowerCase(tmpDef["switch_over_counter"]));
			} else {
				tempPll->setDefparams("switch_over_counter", "1");
			}

			if (tmpDef.count("bandwidth_type") != 0)
			{
				tempPll->setDefparams("bandwidth_type", this->toLowerCase(tmpDef["bandwidth_type"]));
			} else {
				tempPll->setDefparams("bandwidth_type", "\"auto\"");
			}

			if (tmpDef.count("bandwidth") != 0)
			{
				tempPll->setDefparams("bandwidth", this->toLowerCase(tmpDef["bandwidth"]));
			} else {
				tempPll->setDefparams("bandwidth", "0");
			}

			if (tmpDef.count("spread_frequency") != 0)
			{
				tempPll->setDefparams("spread_frequency", this->toLowerCase(tmpDef["spread_frequency"]));
			} else {
				tempPll->setDefparams("spread_frequency", "0");
			}

			if (tmpDef.count("down_spread") != 0)
			{
				tempPll->setDefparams("down_spread", this->toLowerCase(tmpDef["down_spread"]));
			} else {
				tempPll->setDefparams("down_spread", "\"0\"");
			}

			if (tmpDef.count("clk0_multiply_by") != 0)
			{
				tempPll->setDefparams("clk0_multiply_by", this->toLowerCase(tmpDef["clk0_multiply_by"]));
			} else {
				tempPll->setDefparams("clk0_multiply_by", "1");
			}

			if (tmpDef.count("clk1_multiply_by") != 0)
			{
				tempPll->setDefparams("clk1_multiply_by", this->toLowerCase(tmpDef["clk1_multiply_by"]));
			} else {
				tempPll->setDefparams("clk1_multiply_by", "1");
			}

			if (tmpDef.count("clk2_multiply_by") != 0)
			{
				tempPll->setDefparams("clk2_multiply_by", this->toLowerCase(tmpDef["clk2_multiply_by"]));
			} else {
				tempPll->setDefparams("clk2_multiply_by", "1");
			}

			if (tmpDef.count("clk3_multiply_by") != 0)
			{
				tempPll->setDefparams("clk3_multiply_by", this->toLowerCase(tmpDef["clk3_multiply_by"]));
			} else {
				tempPll->setDefparams("clk3_multiply_by", "1");
			}

			if (tmpDef.count("clk4_multiply_by") != 0)
			{
				tempPll->setDefparams("clk4_multiply_by", this->toLowerCase(tmpDef["clk4_multiply_by"]));
			} else {
				tempPll->setDefparams("clk4_multiply_by", "1");
			}

			if (tmpDef.count("clk5_multiply_by") != 0)
			{
				tempPll->setDefparams("clk5_multiply_by", this->toLowerCase(tmpDef["clk5_multiply_by"]));
			} else {
				tempPll->setDefparams("clk5_multiply_by", "1");
			}

			if (tmpDef.count("extclk0_multiply_by") != 0)
			{
				tempPll->setDefparams("extclk0_multiply_by", this->toLowerCase(tmpDef["extclk0_multiply_by"]));
			} else {
				tempPll->setDefparams("extclk0_multiply_by", "1");
			}

			if (tmpDef.count("extclk1_multiply_by") != 0)
			{
				tempPll->setDefparams("extclk1_multiply_by", this->toLowerCase(tmpDef["extclk1_multiply_by"]));
			} else {
				tempPll->setDefparams("extclk1_multiply_by", "1");
			}

			if (tmpDef.count("extclk2_multiply_by") != 0)
			{
				tempPll->setDefparams("extclk2_multiply_by", this->toLowerCase(tmpDef["extclk2_multiply_by"]));
			} else {
				tempPll->setDefparams("extclk2_multiply_by", "1");
			}

			if (tmpDef.count("extclk3_multiply_by") != 0)
			{
				tempPll->setDefparams("extclk3_multiply_by", this->toLowerCase(tmpDef["extclk3_multiply_by"]));
			} else {
				tempPll->setDefparams("extclk3_multiply_by", "1");
			}

			if (tmpDef.count("clk0_divide_by") != 0)
			{
				tempPll->setDefparams("clk0_divide_by", this->toLowerCase(tmpDef["clk0_divide_by"]));
			} else {
				tempPll->setDefparams("clk0_divide_by", "1");
			}

			if (tmpDef.count("clk1_divide_by") != 0)
			{
				tempPll->setDefparams("clk1_divide_by", this->toLowerCase(tmpDef["clk1_divide_by"]));
			} else {
				tempPll->setDefparams("clk1_divide_by", "1");
			}

			if (tmpDef.count("clk2_divide_by") != 0)
			{
				tempPll->setDefparams("clk2_divide_by", this->toLowerCase(tmpDef["clk2_divide_by"]));
			} else {
				tempPll->setDefparams("clk2_divide_by", "1");
			}

			if (tmpDef.count("clk3_divide_by") != 0)
			{
				tempPll->setDefparams("clk3_divide_by", this->toLowerCase(tmpDef["clk3_divide_by"]));
			} else {
				tempPll->setDefparams("clk3_divide_by", "1");
			}

			if (tmpDef.count("clk4_divide_by") != 0)
			{
				tempPll->setDefparams("clk4_divide_by", this->toLowerCase(tmpDef["clk4_divide_by"]));
			} else {
				tempPll->setDefparams("clk4_divide_by", "1");
			}

			if (tmpDef.count("clk5_divide_by") != 0)
			{
				tempPll->setDefparams("clk5_divide_by", this->toLowerCase(tmpDef["clk5_divide_by"]));
			} else {
				tempPll->setDefparams("clk5_divide_by", "1");
			}

			if (tmpDef.count("extclk0_divide_by") != 0)
			{
				tempPll->setDefparams("extclk0_divide_by", this->toLowerCase(tmpDef["extclk0_divide_by"]));
			} else {
				tempPll->setDefparams("extclk0_divide_by", "1");
			}

			if (tmpDef.count("extclk1_divide_by") != 0)
			{
				tempPll->setDefparams("extclk1_divide_by", this->toLowerCase(tmpDef["extclk1_divide_by"]));
			} else {
				tempPll->setDefparams("extclk1_divide_by", "1");
			}

			if (tmpDef.count("extclk2_divide_by") != 0)
			{
				tempPll->setDefparams("extclk2_divide_by", this->toLowerCase(tmpDef["extclk2_divide_by"]));
			} else {
				tempPll->setDefparams("extclk2_divide_by", "1");
			}

			if (tmpDef.count("extclk3_divide_by") != 0)
			{
				tempPll->setDefparams("extclk3_divide_by", this->toLowerCase(tmpDef["extclk3_divide_by"]));
			} else {
				tempPll->setDefparams("extclk3_divide_by", "1");
			}

			if (tmpDef.count("clk0_phase_shift") != 0)
			{
				tempPll->setDefparams("clk0_phase_shift", this->getPhaseShiftDutyCycleVal(tmpDef["clk0_phase_shift"]));
			} else {
				tempPll->setDefparams("clk0_phase_shift", "\"0\"");
			}

			if (tmpDef.count("clk1_phase_shift") != 0)
			{
				tempPll->setDefparams("clk1_phase_shift", this->getPhaseShiftDutyCycleVal(tmpDef["clk1_phase_shift"]));
			} else {
				tempPll->setDefparams("clk1_phase_shift", "\"0\"");
			}

			if (tmpDef.count("clk2_phase_shift") != 0)
			{
				tempPll->setDefparams("clk2_phase_shift", this->getPhaseShiftDutyCycleVal(tmpDef["clk2_phase_shift"]));
			} else {
				tempPll->setDefparams("clk2_phase_shift", "\"0\"");
			}

			if (tmpDef.count("clk3_phase_shift") != 0)
			{
				tempPll->setDefparams("clk3_phase_shift", this->getPhaseShiftDutyCycleVal(tmpDef["clk3_phase_shift"]));
			} else {
				tempPll->setDefparams("clk3_phase_shift", "\"0\"");
			}

			if (tmpDef.count("clk4_phase_shift") != 0)
			{
				tempPll->setDefparams("clk4_phase_shift", this->getPhaseShiftDutyCycleVal(tmpDef["clk4_phase_shift"]));
			} else {
				tempPll->setDefparams("clk4_phase_shift", "\"0\"");
			}

			if (tmpDef.count("clk5_phase_shift") != 0)
			{
				tempPll->setDefparams("clk5_phase_shift", this->getPhaseShiftDutyCycleVal(tmpDef["clk5_phase_shift"]));
			} else {
				tempPll->setDefparams("clk5_phase_shift", "\"0\"");
			}

			if (tmpDef.count("extclk0_phase_shift") != 0)
			{
				tempPll->setDefparams("extclk0_phase_shift", this->getPhaseShiftDutyCycleVal(tmpDef["extclk0_phase_shift"]));
			} else {
				tempPll->setDefparams("extclk0_phase_shift", "\"0\"");
			}

			if (tmpDef.count("extclk1_phase_shift") != 0)
			{
				tempPll->setDefparams("extclk1_phase_shift", this->getPhaseShiftDutyCycleVal(tmpDef["extclk1_phase_shift"]));
			} else {
				tempPll->setDefparams("extclk1_phase_shift", "\"0\"");
			}

			if (tmpDef.count("extclk2_phase_shift") != 0)
			{
				tempPll->setDefparams("extclk2_phase_shift", this->getPhaseShiftDutyCycleVal(tmpDef["extclk2_phase_shift"]));
			} else {
				tempPll->setDefparams("extclk2_phase_shift", "\"0\"");
			}

			if (tmpDef.count("extclk3_phase_shift") != 0)
			{
				tempPll->setDefparams("extclk3_phase_shift", this->getPhaseShiftDutyCycleVal(tmpDef["extclk3_phase_shift"]));
			} else {
				tempPll->setDefparams("extclk3_phase_shift", "\"0\"");
			}

			if (tmpDef.count("clk0_time_delay") != 0)
			{
				tempPll->setDefparams("clk0_time_delay", this->toLowerCase(tmpDef["clk0_time_delay"]));
			} else {
				tempPll->setDefparams("clk0_time_delay", "\"0\"");
			}

			if (tmpDef.count("clk1_time_delay") != 0)
			{
				tempPll->setDefparams("clk1_time_delay", this->toLowerCase(tmpDef["clk1_time_delay"]));
			} else {
				tempPll->setDefparams("clk1_time_delay", "\"0\"");
			}

			if (tmpDef.count("clk2_time_delay") != 0)
			{
				tempPll->setDefparams("clk2_time_delay", this->toLowerCase(tmpDef["clk2_time_delay"]));
			} else {
				tempPll->setDefparams("clk2_time_delay", "\"0\"");
			}

			if (tmpDef.count("clk3_time_delay") != 0)
			{
				tempPll->setDefparams("clk3_time_delay", this->toLowerCase(tmpDef["clk3_time_delay"]));
			} else {
				tempPll->setDefparams("clk3_time_delay", "\"0\"");
			}

			if (tmpDef.count("clk4_time_delay") != 0)
			{
				tempPll->setDefparams("clk4_time_delay", this->toLowerCase(tmpDef["clk4_time_delay"]));
			} else {
				tempPll->setDefparams("clk4_time_delay", "\"0\"");
			}

			if (tmpDef.count("clk5_time_delay") != 0)
			{
				tempPll->setDefparams("clk5_time_delay", this->toLowerCase(tmpDef["clk5_time_delay"]));
			} else {
				tempPll->setDefparams("clk5_time_delay", "\"0\"");
			}

			if (tmpDef.count("extclk0_time_delay") != 0)
			{
				tempPll->setDefparams("extclk0_time_delay", this->toLowerCase(tmpDef["extclk0_time_delay"]));
			} else {
				tempPll->setDefparams("extclk0_time_delay", "\"0\"");
			}

			if (tmpDef.count("extclk1_time_delay") != 0)
			{
				tempPll->setDefparams("extclk1_time_delay", this->toLowerCase(tmpDef["extclk1_time_delay"]));
			} else {
				tempPll->setDefparams("extclk1_time_delay", "\"0\"");
			}

			if (tmpDef.count("extclk2_time_delay") != 0)
			{
				tempPll->setDefparams("extclk2_time_delay", this->toLowerCase(tmpDef["extclk2_time_delay"]));
			} else {
				tempPll->setDefparams("extclk2_time_delay", "\"0\"");
			}

			if (tmpDef.count("extclk3_time_delay") != 0)
			{
				tempPll->setDefparams("extclk3_time_delay", this->toLowerCase(tmpDef["extclk3_time_delay"]));
			} else {
				tempPll->setDefparams("extclk3_time_delay", "\"0\"");
			}

			if (tmpDef.count("clk0_duty_cycle") != 0)
			{
				tempPll->setDefparams("clk0_duty_cycle", this->getPhaseShiftDutyCycleVal(tmpDef["clk0_duty_cycle"]));
			} else {
				tempPll->setDefparams("clk0_duty_cycle", "50");
			}

			if (tmpDef.count("clk1_duty_cycle") != 0)
			{
				tempPll->setDefparams("clk1_duty_cycle", this->getPhaseShiftDutyCycleVal(tmpDef["clk1_duty_cycle"]));
			} else {
				tempPll->setDefparams("clk1_duty_cycle", "50");
			}

			if (tmpDef.count("clk2_duty_cycle") != 0)
			{
				tempPll->setDefparams("clk2_duty_cycle", this->getPhaseShiftDutyCycleVal(tmpDef["clk2_duty_cycle"]));
			} else {
				tempPll->setDefparams("clk2_duty_cycle", "50");
			}

			if (tmpDef.count("clk3_duty_cycle") != 0)
			{
				tempPll->setDefparams("clk3_duty_cycle", this->getPhaseShiftDutyCycleVal(tmpDef["clk3_duty_cycle"]));
			} else {
				tempPll->setDefparams("clk3_duty_cycle", "50");
			}

			if (tmpDef.count("clk4_duty_cycle") != 0)
			{
				tempPll->setDefparams("clk4_duty_cycle", this->getPhaseShiftDutyCycleVal(tmpDef["clk4_duty_cycle"]));
			} else {
				tempPll->setDefparams("clk4_duty_cycle", "50");
			}

			if (tmpDef.count("clk5_duty_cycle") != 0)
			{
				tempPll->setDefparams("clk5_duty_cycle", this->getPhaseShiftDutyCycleVal(tmpDef["clk5_duty_cycle"]));
			} else {
				tempPll->setDefparams("clk5_duty_cycle", "50");
			}

			if (tmpDef.count("extclk0_duty_cycle") != 0)
			{
				tempPll->setDefparams("extclk0_duty_cycle", this->getPhaseShiftDutyCycleVal(tmpDef["extclk0_duty_cycle"]));
			} else {
				tempPll->setDefparams("extclk0_duty_cycle", "50");
			}

			if (tmpDef.count("extclk1_duty_cycle") != 0)
			{
				tempPll->setDefparams("extclk1_duty_cycle", this->getPhaseShiftDutyCycleVal(tmpDef["extclk1_duty_cycle"]));
			} else {
				tempPll->setDefparams("extclk1_duty_cycle", "50");
			}

			if (tmpDef.count("extclk2_duty_cycle") != 0)
			{
				tempPll->setDefparams("extclk2_duty_cycle", this->getPhaseShiftDutyCycleVal(tmpDef["extclk2_duty_cycle"]));
			} else {
				tempPll->setDefparams("extclk2_duty_cycle", "50");
			}

			if (tmpDef.count("extclk3_duty_cycle") != 0)
			{
				tempPll->setDefparams("extclk3_duty_cycle", this->getPhaseShiftDutyCycleVal(tmpDef["extclk3_duty_cycle"]));
			} else {
				tempPll->setDefparams("extclk3_duty_cycle", "50");
			}

		} else {
			cerr << "Error: " << "File " + pllVFile + " cannot be found!\n";
		}
	}
}

void Model::pack_ram_block(void)
{
	for (map<string, CometMem *>::const_iterator cometMem = cometMems.begin(); 
		cometMem != cometMems.end(); cometMem++)
	{

		string memVFile = cometMem->second->getRamVFile();
		string memLogicName = cometMem->second->getLogicName();
		string memIniFilePath = cometMem->second->getRamIniFilePath();

		vector<string> &portA = cometMem->second->getPortA();
		vector<string> &portB = cometMem->second->getPortB();

		map<string, InPort *> &tempInport = cometMem->second->getInports();
		map<string, OutPort *> &tempOutport = cometMem->second->getOutports();

		RamBlock * tempRam = new RamBlock(this, cometMem->first);
		string tempRamName = tempRam->getName();
		ramBlocks[tempRamName] = tempRam;

		tempRam->setCometMem(*(cometMem->second)); 

		// add inports, outports
		bool isPortBExist = false;
		bool sgPort = false;
		bool sgClk = false;
		bool sgPortSgClk = false;
		bool naclr = false;
		int address_a_cnt = -1;
		int address_b_cnt = -1;
		int data_a_cnt = -1;
		int data_b_cnt = -1;
		int q_a_cnt = -1;
		int q_b_cnt = -1;

		map<int, string> address_a;
		map<int, string> address_b;
		map<int, string> data_a;
		map<int, string> data_b;
		map<int, string> q_a;
		map<int, string> q_b;

		ifstream iTmpCin(memVFile);

		if (iTmpCin.is_open()) {

			string str = "";
			bool aclk0Found = false;
			bool aclr1Found = false;
			bool isDefFound = false;

			string defLineStr = "";
			string defPara = "";
			string defParaVal = "";			

			while (getline(iTmpCin, str)) {

				str = this->trim(str);

				if (str.empty())
				{
					continue;
				}

				if (str.length() > 12)
				{
					if (!str.substr(0, 7).compare(".clock1"))
					{
						if (!str.substr(9, 4).compare("1'b1"))
						{
							sgClk = true;
						}
					}
				
					if (!str.substr(0, 6).compare(".aclr0"))
					{
						if (!str.substr(8, 4).compare("1'b0"))
						{
							aclk0Found = true;
						}
					}
				
					if (!str.substr(0, 6).compare(".aclr1"))
					{
						if (!str.substr(8, 4).compare("1'b0"))
						{
							aclr1Found = true;
						}						
					}				
				}

				if (!str.compare("defparam")) {
					isDefFound = true;
					continue;
				} 

				if (!str.compare("endmodule")) {
					isDefFound = false;
				} 

				if (isDefFound) {

					int index = 0;
					index = str.find_first_of('.');

					if (index != string::npos && index != (str.length() - 1) 
						&& (str.at(str.length() - 1) == ',' || str.at(str.length() - 1) == ';'))
					{
						defLineStr = str.substr(index + 1);
						defLineStr = this->trim(defLineStr.substr(0, defLineStr.length() - 1));

						defPara = this->trim(this->split(defLineStr, "=").at(0));
						defParaVal = this->trim(this->split(defLineStr, "=").at(1));

						if (!defPara.compare("operation_mode"))
						{
							isDefFound = false;

							string opMode = this->toLowerCase(defParaVal.substr(1, defParaVal.length() - 2));
							
							if (!opMode.compare("single_port"))
							{
								sgPort = true;
							}
						}
					}
				}
			}
			
			if (aclk0Found && aclr1Found)
			{
				naclr = true;
			}

			iTmpCin.close();
		}

		if (sgPort && sgClk)
		{
			sgPortSgClk = true;
		}

		for (map<string, InPort *>::const_iterator itRamInport = tempInport.begin(); 
			itRamInport != tempInport.end(); itRamInport++)
		{
			if (itRamInport->second != NULL) {				
				string portName = itRamInport->first;

				if (!(portName.compare("wren_a"))) {
					tempRam->addInport("portawe", itRamInport->second->getRealName());		
					//tempRam->addInport("portawe", "{gnd," + itRamInport->second->getRealName() + "}");				
				} else if (!(portName.compare("clock0"))) {
					tempRam->addInport("clk0", itRamInport->second->getRealName());
				} else if (!(portName.compare("clock1"))) {
					if (!sgPortSgClk)
					{
						tempRam->addInport("clk1", itRamInport->second->getRealName());
					}
				} else if (!(portName.compare("clocken0"))) {
					tempRam->addInport("ena0", itRamInport->second->getRealName());
				} else if (!(portName.compare("clocken1"))) {
					if (!sgPortSgClk)
					{
						tempRam->addInport("ena1", itRamInport->second->getRealName());
					}					
				} else if (!(portName.compare("aclr0"))) {
					if (!naclr)
					{
						tempRam->addInport("clr0", itRamInport->second->getRealName());
					}
				} else if (!(portName.compare("aclr1"))) {
					if (!(sgPortSgClk || naclr))
					{
						tempRam->addInport("clr1", itRamInport->second->getRealName());
					}
				} else if (!(portName.compare("wren_b"))) {
					tempRam->addInport("portbrewe", itRamInport->second->getRealName());
				} else if (!(portName.substr(0, 9).compare("address_a"))) {
					int index = portName.find_last_of('_');
					if (index != string::npos && index != portName.length() - 1)
					{
						int num = atoi(portName.substr(index + 1).c_str());
						if (address_a.count(num) == 0)
						{
							address_a[num] = itRamInport->second->getRealName();
						}
						if (num > address_a_cnt)
						{
							address_a_cnt = num;
						}
					} else {
						address_a_cnt = 0;
						address_a[0] = itRamInport->second->getRealName();
					}
				} else if (!(portName.substr(0, 9).compare("address_b"))) {
					int index = portName.find_last_of('_');
					if (index != string::npos && index != portName.length() - 1)
					{
						int num = atoi(portName.substr(index + 1).c_str());
						if (address_b.count(num) == 0)
						{
							address_b[num] = itRamInport->second->getRealName();
						}
						if (num > address_b_cnt)
						{
							address_b_cnt = num;
						}
					} else {
						address_b_cnt = 0;
						address_b[0] = itRamInport->second->getRealName();
					}
				} else if (!(portName.substr(0, 6).compare("data_a"))) {
					int index = portName.find_last_of('_');
					if (index != string::npos && index != portName.length() - 1)
					{
						int num = atoi(portName.substr(index + 1).c_str());
						if (data_a.count(num) == 0)
						{
							data_a[num] = itRamInport->second->getRealName();
						}
						if (num > data_a_cnt)
						{
							data_a_cnt = num;
						}
					} else {
						data_a_cnt = 0;
						data_a[0] = itRamInport->second->getRealName();
					}
				} else if (!(portName.substr(0, 6).compare("data_b"))) {
					int index = portName.find_last_of('_');
					if (index != string::npos && index != portName.length() - 1)
					{
						int num = atoi(portName.substr(index + 1).c_str());
						if (data_b.count(num) == 0)
						{
							data_b[num] = itRamInport->second->getRealName();
						}
						if (num > data_b_cnt)
						{
							data_b_cnt = num;
						}
					} else {
						data_b_cnt = 0;
						data_b[0] = itRamInport->second->getRealName();
					}
				}
			}
		}

		if (address_a_cnt != -1)
		{
			string val = "{";

			if (address_a_cnt == 0)
			{
				val += address_a[0];
			} else {
				val += address_a[address_a_cnt];

				for (int i = address_a_cnt - 1; i >= 0; i--)
				{
					val += ("," + address_a[i]);
				}
			}

			val += "}";			
			tempRam->addInport("portaaddr", val);
		}

		if (address_b_cnt != -1)
		{
			string val = "{";

			if (address_b_cnt == 0)
			{
				val += address_b[0];
			} else {
				val += address_b[address_b_cnt];

				for (int i = address_b_cnt - 1; i >= 0; i--)
				{
					val += ("," + address_b[i]);
				}
			}

			val += "}";			
			tempRam->addInport("portbaddr", val);
		}

		if (data_a_cnt != -1)
		{
			string val = "{";

			if (data_a_cnt == 0)
			{
				val += data_a[0];
			} else {
				val += data_a[data_a_cnt];

				for (int i = data_a_cnt - 1; i >= 0; i--)
				{
					val += ("," + data_a[i]);
				}
			}

			val += "}";			
			tempRam->addInport("portadatain", val);
		}

		if (data_b_cnt != -1)
		{
			string val = "{";

			if (data_b_cnt == 0)
			{
				val += data_b[0];
			} else {
				val += data_b[data_b_cnt];

				for (int i = data_b_cnt - 1; i >= 0; i--)
				{
					val += ("," + data_b[i]);
				}
			}

			val += "}";			
			tempRam->addInport("portbdatain", val);
		}

		for (map<string, OutPort *>::const_iterator itRamOutport = tempOutport.begin(); 
			itRamOutport != tempOutport.end(); itRamOutport++)
		{
			if (itRamOutport->second != NULL)
			{
				string portName = itRamOutport->first;

				if (!(portName.substr(0, 3).compare("q_a"))) {
					int index = portName.find_last_of('_');
					if (index != string::npos && index != portName.length() - 1)
					{
						int num = atoi(portName.substr(index + 1).c_str());
						if (q_a.count(num) == 0)
						{
							q_a[num] = itRamOutport->second->getRealName();
						}
						if (num > q_a_cnt)
						{
							q_a_cnt = num;
						}
					} else {
						q_a_cnt = 0;
						q_a[0] = itRamOutport->second->getRealName();
					}
				} else if (!(portName.substr(0, 3).compare("q_b"))) {
					int index = portName.find_last_of('_');
					if (index != string::npos && index != portName.length() - 1)
					{
						int num = atoi(portName.substr(index + 1).c_str());
						if (q_b.count(num) == 0)
						{
							q_b[num] = itRamOutport->second->getRealName();
						}
						if (num > q_b_cnt)
						{
							q_b_cnt = num;
						}
					} else {
						q_b_cnt = 0;
						q_b[0] = itRamOutport->second->getRealName();
					}
				} 
			}
		}

		if (q_a_cnt != -1)
		{
			string val = "{";

			if (q_a_cnt == 0)
			{
				val += q_a[0];
			} else {
				val += q_a[q_a_cnt];

				for (int i = q_a_cnt - 1; i >= 0; i--)
				{
					val += ("," + q_a[i]);
				}
			}

			val += "}";			
			tempRam->addOutport("portadataout", val);
		}

		if (q_b_cnt != -1)
		{
			isPortBExist = true;
			tempRam->setFlagPortB(isPortBExist);

			string val = "{";

			if (q_b_cnt == 0)
			{
				val += q_b[0];
			} else {
				val += q_b[q_b_cnt];

				for (int i = q_b_cnt - 1; i >= 0; i--)
				{
					val += ("," + q_b[i]);
				}
			}

			val += "}";			
			tempRam->addOutport("portbdataout", val);
		}

		// set defparams 

		ifstream icin(memVFile);

		if (icin.is_open()) {

			string str = "";
			bool isDefFound = false;
			string defLineStr = "";
			string defPara = "";
			string defParaVal = "";

			string iClrA = "\"none\"";
			string iClrB = "\"none\"";
			bool oClkA = false;
			bool oClkB = false;
			bool isClk1Exist = false;

			map<string, string> tmpDef;

			while (getline(icin, str)) {

				str = this->trim(str);

				if (str.empty())
				{
					continue;
				}

				if (!str.compare("defparam")) {
					isDefFound = true;
					continue;
				} 

				if (!str.compare("endmodule")) {
					isDefFound = false;
				} 

				if (isDefFound) {

					int index = 0;
					index = str.find_first_of('.');

					if (index != string::npos && index != (str.length() - 1) 
						&& (str.at(str.length() - 1) == ',' || str.at(str.length() - 1) == ';'))
					{
						defLineStr = str.substr(index + 1);
						defLineStr = this->trim(defLineStr.substr(0, defLineStr.length() - 1));

						defPara = this->trim(this->split(defLineStr, "=").at(0));
						defParaVal = this->trim(this->split(defLineStr, "=").at(1));

						tmpDef[defPara] = defParaVal;
					}
				}
			}

			icin.close();

			ostringstream ostr;

			bool needExchange = false;

			if (tmpDef.count("operation_mode") != 0)
			{
				string opMode = this->toLowerCase(tmpDef["operation_mode"]);
				tempRam->setDefparams("operation_mode", opMode);
			}

			if (tmpDef.count("read_during_write_mode_mixed_ports") != 0)
			{
				if (!tmpDef["read_during_write_mode_mixed_ports"].compare("\"OLD_DATA\""))
				{
					tempRam->setDefparams("mixed_port_feed_through_mode", "\"old\"");
				} else {
					tempRam->setDefparams("mixed_port_feed_through_mode", "\"dont_care\"");
				}
			}

			if (tmpDef.count("ram_block_type") != 0)
			{
				if (!this->toLowerCase(tmpDef["ram_block_type"]).compare("auto"))
				{
					tempRam->setDefparams("ram_block_type", this->toLowerCase(tmpDef["ram_block_type"]));
				} else {
					tempRam->setDefparams("ram_block_type", this->toUpperCase(tmpDef["ram_block_type"]));
				}				
			}

			tempRam->setDefparams("logical_ram_name", "\"" + memLogicName + "\"");

			//if (tmpDef.count("init_file") != 0)
			//{
			//	tempRam->setDefparams("initial_file", tmpDef["init_file"]);			
			//}

			if (!memIniFilePath.substr(1, memIniFilePath.length() - 2).empty())
			{
				tempRam->setDefparams("init_file", memIniFilePath);	
			}

			if (tmpDef.count("init_file_layout") != 0)
			{
				if (!tmpDef["init_file_layout"].compare("PORT_A"))
				{
					tempRam->setDefparams("init_file_layout", "\"Port_A\"");
				} else {
					tempRam->setDefparams("init_file_layout", "\"Port_B\"");
				}				
			}

			if (isPortBExist)	{

				int minWidth = min(data_a_cnt, q_b_cnt);
				ostr.str("");
				ostr.clear();
				ostr << (minWidth + 1);

				tempRam->setDefparams("data_interleave_width_in_bits", ostr.str());

				string strWidthA = tmpDef["width_a"];
				string strWidthB = tmpDef["width_b"];

				int widthA = atoi(strWidthA.c_str());
				int widthB = atoi(strWidthB.c_str());

				int minOffset = min(widthA, widthB);
				ostr.str("");
				ostr.clear();
				ostr << minOffset;

				tempRam->setDefparams("data_interleave_offset_in_bits", ostr.str());

				if ((widthA < widthB) && !tempRam->getPara("operation_mode").compare("\"bidir_dual_port\""))
				{
					needExchange = true;
				}

			} else {
				tempRam->setDefparams("data_interleave_width_in_bits", "1");
				tempRam->setDefparams("data_interleave_offset_in_bits", "1");
			}

			if (needExchange)
			{
				tempRam->exchangeParam("port_a_address_width", "port_b_address_width");
			}

			if (tmpDef.count("numwords_a") != 0) 
			{
				tempRam->setDefparams("port_a_logical_ram_depth", tmpDef["numwords_a"]);
			}

			if (tmpDef.count("numwords_b") != 0) 
			{
				tempRam->setDefparams("port_b_logical_ram_depth", tmpDef["numwords_b"]);
			}

			if (needExchange)
			{
				tempRam->exchangeParam("port_a_logical_ram_depth", "port_b_logical_ram_depth");
			}

			if (tmpDef.count("width_a") != 0) 
			{
				tempRam->setDefparams("port_a_logical_ram_width", tmpDef["width_a"]);
			}

			if (tmpDef.count("width_b") != 0) 
			{
				tempRam->setDefparams("port_b_logical_ram_width", tmpDef["width_b"]);
			}

			if (needExchange)
			{
				tempRam->exchangeParam("port_a_logical_ram_width", "port_b_logical_ram_width");
			}

			if (tmpDef.count("indata_aclr_a") != 0) 
			{
				string state = this->toLowerCase(tmpDef["indata_aclr_a"]);
				tempRam->setDefparams("port_a_data_in_clear", state);

				if (state.compare("\"none\""))
				{
					iClrA = state;
				}
			}

			if (isPortBExist)
			{
				if (tmpDef.count("indata_aclr_b") != 0) 
				{
					string state = this->toLowerCase(tmpDef["indata_aclr_b"]);
					tempRam->setDefparams("port_b_data_in_clear", state);

					if (state.compare("\"none\""))
					{
						iClrB = state;
					}
				}
	
				if (tmpDef.count("indata_reg_b") != 0) 
				{
					string state = this->toLowerCase(tmpDef["indata_reg_b"]);

					if (!(state.compare("\"clock0\"") && state.compare("\"clock1\"")))
					{
						tempRam->setDefparams("port_b_data_in_clock", state);

						if (!state.compare("\"clock1\"")) {
							isClk1Exist = true;
						}

					} else {
						tempRam->setDefparams("port_b_data_in_clock", "\"none\"");
					}
				}
			}

			if (tmpDef.count("address_aclr_a") != 0) 
			{
				string state = this->toLowerCase(tmpDef["address_aclr_a"]);
				tempRam->setDefparams("port_a_address_clear", state);

				if (state.compare("\"none\""))
				{
					iClrA = state;
				}
			}

			if (isPortBExist)
			{
				if (tmpDef.count("address_aclr_b") != 0) 
				{
					string state = this->toLowerCase(tmpDef["address_aclr_b"]);
					tempRam->setDefparams("port_b_address_clear", state);

					if (state.compare("\"none\""))
					{
						iClrB = state;
					}
				}
	
				if (tmpDef.count("address_reg_b") != 0) 
				{
					string state = this->toLowerCase(tmpDef["address_reg_b"]);

					if (!(state.compare("\"clock0\"") && state.compare("\"clock1\"")))
					{
						tempRam->setDefparams("port_b_address_clock", state);

						if (!state.compare("\"clock1\"")) {
							isClk1Exist = true;
						}

					} else {
						tempRam->setDefparams("port_b_address_clock", "\"none\"");
					}
				}
			}

			if (tmpDef.count("wrcontrol_aclr_a") != 0) 
			{
				string state = this->toLowerCase(tmpDef["wrcontrol_aclr_a"]);
				tempRam->setDefparams("port_a_write_enable_clear", state);

				if (state.compare("\"none\""))
				{
					iClrA = state;
				}
			}

			//if (tmpDef.count("byteena_aclr_a") != 0) 
			//{
			//	tempRam->setDefparams("port_a_byte_enable_clear", this->toLowerCase(tmpDef["byteena_aclr_a"]));
			//}

			if (isPortBExist)
			{
				if (tmpDef.count("wrcontrol_aclr_b") != 0) 
				{
					string state = this->toLowerCase(tmpDef["wrcontrol_aclr_b"]);
					tempRam->setDefparams("port_b_read_enable_write_enable_clear", state);

					if (state.compare("\"none\""))
					{
						iClrB = state;
					}

				} else if (tmpDef.count("rdcontrol_aclr_b") != 0) {
					string state = this->toLowerCase(tmpDef["rdcontrol_aclr_b"]);
					tempRam->setDefparams("port_b_read_enable_write_enable_clear", state);

					if (state.compare("\"none\""))
					{
						iClrB = state;
					}

				} else {
					tempRam->setDefparams("port_b_read_enable_write_enable_clear", "\"none\"");
				}
	
				if (tmpDef.count("wrcontrol_wraddress_reg_b") != 0) 
				{
					tempRam->setDefparams("port_b_read_enable_write_enable_clock", this->toLowerCase(tmpDef["wrcontrol_wraddress_reg_b"]));
				} else if (tmpDef.count("rdcontrol_reg_b") != 0) {
					tempRam->setDefparams("port_b_read_enable_write_enable_clock", this->toLowerCase(tmpDef["rdcontrol_reg_b"]));
				} else if (tmpDef.count("address_reg_b") != 0) {
					tempRam->setDefparams("port_b_read_enable_write_enable_clock", this->toLowerCase(tmpDef["address_reg_b"]));
				}

				string st = tempRam->getPara("port_b_read_enable_write_enable_clock");

				if (!st.compare("\"clock1\"")) {
					isClk1Exist = true;
				}
			}

			/*		if (isPortBExist)
			{	*/
				//if (tmpDef.count("byteena_reg_b") != 0)
				//{
				//	tempRam->setDefparams("port_b_byte_enable_clock", this->toLowerCase(tmpDef["byteena_reg_b"]));
				//}

				//if (tmpDef.count("byteena_aclr_b") != 0)
				//{
				//	tempRam->setDefparams("port_b_byte_enable_clear", this->toLowerCase(tmpDef["byteena_aclr_b"]));
				//}
				
			//}

			if (tmpDef.count("outdata_reg_a") != 0) 
			{
				string state = this->toLowerCase(tmpDef["outdata_reg_a"]);

				if (!(state.compare("\"clock0\"") && state.compare("\"clock1\"")))
				{
					oClkA = true;
					tempRam->setDefparams("port_a_data_out_clock", state);

					if (!state.compare("\"clock1\"")) {
						isClk1Exist = true;
					}

				} else {
					tempRam->setDefparams("port_a_data_out_clock", "\"none\"");
				}
			}

			if (isPortBExist)
			{
				if (tmpDef.count("outdata_reg_b") != 0) 
				{
					string state = this->toLowerCase(tmpDef["outdata_reg_b"]);
	
					if (!(state.compare("\"clock0\"") && state.compare("\"clock1\"")))
					{
						oClkB = true;
						tempRam->setDefparams("port_b_data_out_clock", state);

						if (!state.compare("\"clock1\"")) {
							isClk1Exist = true;
						}

					} else {
						tempRam->setDefparams("port_b_data_out_clock", "\"none\"");
					}
				}
			}

			if (tmpDef.count("outdata_aclr_a") != 0) 
			{
				tempRam->setDefparams("port_a_data_out_clear", this->toLowerCase(tmpDef["outdata_aclr_a"]));
			}

			if (isPortBExist)
			{
				if (tmpDef.count("outdata_aclr_b") != 0) 
				{
					tempRam->setDefparams("port_b_data_out_clear", this->toLowerCase(tmpDef["outdata_aclr_b"]));
				}
			}

			tempRam->setDefparams("port_a_first_address", portA.at(0));
			tempRam->setDefparams("port_a_last_address", portA.at(1));
			tempRam->setDefparams("port_a_first_bit_number", portA.at(2));
			ostr.str("");
			ostr.clear();
			ostr << (data_a_cnt + 1);
			tempRam->setDefparams("port_a_data_width", ostr.str());

			long firstAddrA = atol(portA.at(0).c_str());
			long lastAddrA = atol(portA.at(1).c_str());
			long wdA = lastAddrA - firstAddrA + 1;

			int cntA = 0;

			while (wdA != 1)
			{
				wdA = wdA / 2;
				cntA++;
			}

			ostr.str("");
			ostr.clear();
			ostr << cntA;
			tempRam->setDefparams("port_a_address_width", ostr.str());
				
			if (isPortBExist)
			{
				if (!portB.empty())
				{
					tempRam->setDefparams("port_b_first_address", portB.at(0));
					tempRam->setDefparams("port_b_last_address", portB.at(1));
					tempRam->setDefparams("port_b_first_bit_number", portB.at(2));
					
					long firstAddrB = atol(portB.at(0).c_str());
					long lastAddrB = atol(portB.at(1).c_str());
					long wdB = lastAddrB - firstAddrB + 1;

					int cntB = 0;

					while (wdB != 1)
					{
						wdB = wdB / 2;
						cntB++;
					}

					ostr.str("");
					ostr.clear();
					ostr << cntB;
					tempRam->setDefparams("port_b_address_width", ostr.str());

				}

				ostr.str("");
				ostr.clear();
				ostr << (q_b_cnt + 1);
				tempRam->setDefparams("port_b_data_width", ostr.str());

				
				//if (tmpDef.count("widthad_b") != 0) 
				//{
				//	int width =  atoi(tmpDef["widthad_b"].c_str());
				//	if (width < 13)
				//	{
				//tempRam->setDefparams("port_b_address_width", tmpDef["widthad_b"]);
				//	} 
				//	else
				//	{
				//		tempRam->setDefparams("port_b_address_width", "12");
				//	}
				//}
			}

			if (iClrA.compare("\"none\""))
			{
				tempRam->modifyParam("port_a_data_in_clear", iClrA);
				tempRam->modifyParam("port_a_address_clear", iClrA);
				tempRam->modifyParam("port_a_write_enable_clear", iClrA);
			}

			if (iClrB.compare("\"none\""))
			{
				tempRam->modifyParam("port_b_data_in_clear", iClrB);
				tempRam->modifyParam("port_b_address_clear", iClrB);
				tempRam->modifyParam("port_b_read_enable_write_enable_clear", iClrB);
			}

			if (!oClkA)
			{
				tempRam->modifyParam("port_a_data_out_clock", "\"none\""); 
				tempRam->modifyParam("port_a_data_out_clear", "\"none\""); 
			}

			if (!oClkB)
			{
				tempRam->modifyParam("port_b_data_out_clock", "\"none\""); 
				tempRam->modifyParam("port_b_data_out_clear", "\"none\""); 
			}

			if (!isClk1Exist)
			{
				tempRam->delInPort("clk1");
				tempRam->delInPort("clr1");
				tempRam->delInPort("ena1");
			}

		} else {
			cerr << "Error: " << "File " + memVFile + " cannot be found!\n";
		}
	}
}

void Model::pack_unpack(void)
{
	// Get all regs
	for (map<string, Reg *>::const_iterator it = regs.begin(); 
		it != regs.end(); it++){
			//stratix_lcell \edge_dff:dff1|q~I (
			//	.clk(clock[0]),
			//	.datac(\edge_dff:dff0|q ),
			//	.aclr(gnd),
			//	.sload(vcc),
			//	.regout(\edge_dff:dff1|q ));
			//defparam \edge_dff:dff1|q~I .operation_mode = "normal";
			//defparam \edge_dff:dff1|q~I .synch_mode = "on";
			//defparam \edge_dff:dff1|q~I .register_cascade_mode = "off";
			//defparam \edge_dff:dff1|q~I .sum_lutc_input = "datac";
			//defparam \edge_dff:dff1|q~I .lut_mask = "0000";
			//defparam \edge_dff:dff1|q~I .output_mode = "reg_only";

			map<string, InPort *> &tempInport = it->second->getInports();
			map<string, OutPort *> &tempOutport = it->second->getOutports();

			LE * tempLE = new LE(this, this->genLEName(it->first, ""));
			les[tempLE->getName()] = tempLE;

			// set defparams 

			tempLE->setDefparams(tempLE->defParamName[0], "normal");         // "operation_mode"
			tempLE->setDefparams(tempLE->defParamName[1], "on");         // "synch_mode"
			tempLE->setDefparams(tempLE->defParamName[2], "off");         // "register_cascade_mode"
			tempLE->setDefparams(tempLE->defParamName[3], "datac");         // "sum_lutc_input"
			tempLE->setDefparams(tempLE->defParamName[4], "0000");         // "lut_mask"
			tempLE->setDefparams(tempLE->defParamName[5], "reg_only");         // "output_mode"

			// add inports, outports

			if (tempInport.count("adata") == 0)
			{
				tempLE->addInport(tempLE->inportName[8], "vcc");   // sload -> 1

				if (tempInport.count("data0") != 0)
				{
					tempLE->addInport(tempLE->inportName[3], (tempInport["data0"])->getRealName());  // datac -> d
				}

			} else {

				tempLE->addInport(tempLE->inportName[4], (tempInport["data0"])->getRealName());  // datad -> d
				tempLE->addInport(tempLE->inportName[3], "vcc");   // datac(adata) -> 1

				tempLE->setDefparams(tempLE->defParamName[4], "FF00");         // "lut_mask"

				if(tempInport.count("sload") == 0){		

					if(tempInport.count("sclr") == 0){
						tempLE->setDefparams(tempLE->defParamName[1], "off");         // "synch_mode"
					}

				}else{

					tempLE->addInport(tempLE->inportName[8], (tempInport["sload"])->getRealName());   // sload

				}	

			}

			for (map<string, InPort *>::const_iterator ito = tempInport.begin(); 
				ito != tempInport.end(); ito++)
			{
				if (ito->second != NULL)
				{
					string portName = ito->first;

					if (!(portName.compare(tempLE->inportName[0]) &&           // clk 
						portName.compare(tempLE->inportName[5]) &&              // aclr
						portName.compare(tempLE->inportName[6]) &&              // aload
						portName.compare(tempLE->inportName[7]) &&              // sclr
						portName.compare(tempLE->inportName[9])))                  // ena
					{
						tempLE->addInport(portName, ito->second->getRealName());
					}
				}
			}

			for (map<string, OutPort *>::const_iterator ito = tempOutport.begin(); 
				ito != tempOutport.end(); ito++)
			{
				if (ito->second != NULL)
				{
					string portName = ito->first;

					if (!portName.compare(tempLE->outportName[1]))                  // regout
					{
						tempLE->addOutport(portName, ito->second->getRealName());
					}
				}
			}

			// -------------------  Print LE Info  --------------------------

			//this->showLEInfo(*tempLE);
	}

	// Get all LUTs
	for (map<string, LUT *>::const_iterator it = luts.begin(); 
		it != luts.end(); it++){
			//stratix_lcell \comb~0_I (
			//	.dataa(n0),
			//	.datab(b),
			//	.datac(e),
			//	.datad(a),
			//	.combout(\comb~0 ));
			//defparam \comb~0_I .operation_mode = "normal";
			//defparam \comb~0_I .synch_mode = "off";
			//defparam \comb~0_I .register_cascade_mode = "off";
			//defparam \comb~0_I .sum_lutc_input = "datac";
			//defparam \comb~0_I .lut_mask = "565A";
			//defparam \comb~0_I .output_mode = "comb_only";

			map<string, InPort *> &tempInport = it->second->getInports();
			map<string, OutPort *> &tempOutport = it->second->getOutports();

			LE * tempLE = new LE(this, this->genLEName(it->first, ""));
			les[tempLE->getName()] = tempLE;

			// add inports, outports

			for (map<string, InPort *>::const_iterator ito = tempInport.begin(); 
				ito != tempInport.end(); ito++)
			{
				if (ito->second != NULL)
				{
					string portName = ito->first;

					if (!(portName.compare(tempLE->inportName[1]) &&              // dataa 
						portName.compare(tempLE->inportName[2]) &&              // datab
						portName.compare(tempLE->inportName[3]) &&              // datac
						portName.compare(tempLE->inportName[4])))                  // datad
					{
						tempLE->addInport(portName, ito->second->getRealName());
					}
				}
			}

			for (map<string, OutPort *>::const_iterator ito = tempOutport.begin(); 
				ito != tempOutport.end(); ito++)
			{
				if (ito->second != NULL)
				{
					string portName = ito->first;

					if (!portName.compare(tempLE->outportName[0]))                  // combout
					{
						tempLE->addOutport(portName, ito->second->getRealName());
					}
				}
			}

			// set defparams 

			tempLE->setDefparams(tempLE->defParamName[0], "normal");         // "operation_mode"
			tempLE->setDefparams(tempLE->defParamName[1], "off");         // "synch_mode"
			tempLE->setDefparams(tempLE->defParamName[2], "off");         // "register_cascade_mode"
			tempLE->setDefparams(tempLE->defParamName[3], "datac");         // "sum_lutc_input"
			tempLE->setDefparams(tempLE->defParamName[4], it->second->getLutMask());         // "lut_mask"
			tempLE->setDefparams(tempLE->defParamName[5], "comb_only");         // "output_mode"

			// -------------------  Print LE Info  --------------------------

			//this->showLEInfo(*tempLE);

	}
}

void Model::setModel(map<string, map<string, SubcktModelInfo *> *> &subcktModelsInfo)
{
	this->genPortStatus();
	this->maskLut();
	this->genSubckt(subcktModelsInfo);
	this->transLatchToLUT();
	this->matchLutPort();
	this->matchRegPort();
	this->matchCometPllPort();
	this->matchCometMemPort();
	if (bufferAway)
	{
		this->bufferGone();  
	}
	this->connWires();
	this->pack();
	this->pack_pll();
	this->pack_ram_block();
	this->maskRecovery();
	//this->showSubckts();
}

void Model::showLEInfo(LE &tempLE)
{

	cout << tempLE.getName() << endl;

	map<string, string> &tempInports = tempLE.getInports();
	map<string, string> &tempOutports = tempLE.getOutports();
	map<string, string> &tempDefparams = tempLE.getDefparams();

	for (map<string, string>::const_iterator it = tempInports.begin(); 
		it != tempInports.end(); it++)
	{
		cout << it->first << ": " << it->second << endl;
	}

	for (map<string, string>::const_iterator it = tempOutports.begin(); 
		it != tempOutports.end(); it++)
	{
		cout << it->first << ": " << it->second << endl;
	}

	for (map<string, string>::const_iterator it = tempDefparams.begin(); 
		it != tempDefparams.end(); it++)
	{
		cout << it->first << ": " << it->second << endl;
	}

	cout << "" << endl;
}

void Model::showSubckts(void)
{
	for (map<string, map<string, map<string, map<string, string> *> *> *>::const_iterator it = subckts.begin(); 
		it != subckts.end(); it++){

			for (map<string, map<string, map<string, string> *> *>::const_iterator ito = (*(it->second)).begin(); 
				ito != (*(it->second)).end(); ito++){

					for (map<string, map<string, string> *>::const_iterator itoo = (*(ito->second)).begin(); 
						itoo != (*(ito->second)).end(); itoo++)
					{
						cout << itoo->first << endl;

						for (map<string, string>::const_iterator itooo = itoo->second->begin(); 
							itooo != itoo->second->end(); itooo++){
								cout << itooo->first << ": " << itooo->second << endl;
						}
					}
			}

	}

}

vector<string> Model::split(string str, string ptn)
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

string Model::toLowerCase(string str) {

	for (int i = 0; i < str.length(); i++) {

		char ch = str.at(i);

		if (isalpha(ch))
		{
			if (isupper(ch))
			{
				ch = tolower(ch);
				str.at(i) = ch;
			}
		}

	}

	return str;
}

string Model::toUpperCase(string str) {

	for (int i = 0; i < str.length(); i++) {

		char ch = str.at(i);

		if (isalpha(ch))
		{
			if (islower(ch))
			{
				ch = toupper(ch);
				str.at(i) = ch;
			}
		}

	}

	return str;
}

void Model::transLatchToLUT(void)
{
	for (map<string, Latch *>::const_iterator it = latches.begin(); 
		it != latches.end(); it++){
			if (it->second != NULL)
			{
				(*it).second->transToLUT();
			}
	}
}

// Discard blanks in front of or at the end of certain line
string Model::trim(string str)
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

map<string, string> & Model::getBufferPortInfo(void)
{
	return *bufferPortInfo;
}
