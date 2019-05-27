#include "LUT.h"
#include <iostream>
#include <math.h>
#include <queue>

using std::cout;
using std::endl;
using std::hex;
using std::queue;
using std::vector;

LUT::LUT(Node * node, string name, string nodeType): Node(nodeType)
{
	this->name = name;
	this->pnode = node;
	this->corrLatch = NULL;
	this->maskMode = "dcba";
	this->inoutPortRealNameCount = 0;
	this->buffer = false;
	//this->adataLUT = false;
}


LUT::~LUT(void)
{
	//cout << this->name << " deconstruct..." << endl;

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

}

const int LUT::INPORT_TYPE_COUNT = 4;

const int LUT::OUTPORT_TYPE_COUNT = 1;

//const string LUT::inportName [INPORT_TYPE_COUNT] = {"dataa", "datab", "datac", "datad"};
const string LUT::inportName [INPORT_TYPE_COUNT] = {"datad", "datac", "datab", "dataa"};

const string LUT::outportName [OUTPORT_TYPE_COUNT] = {"combout"};

void LUT::addInoutPortRealName(string realName)
{
	inoutPortRealName.push_back(realName);
	this->inoutPortRealNameCount++;
}


void LUT::addInTrueTable(string str)
{
	this->inTrueTable.push_back(str);
}

string LUT::decToHex(long maskResult)
{
	string str = "0000";
	int rem = 0;

	int index = 3;

	while(maskResult != 0){
		rem = maskResult % 16;
		switch (rem)
		{
		case 0:
			str.at(index) = '0';
			break;
		case 1:
			str.at(index) = '1';
			break;
		case 2:
			str.at(index) = '2';
			break;
		case 3:
			str.at(index) = '3';
			break;
		case 4:
			str.at(index) = '4';
			break;
		case 5:
			str.at(index) = '5';
			break;
		case 6:
			str.at(index) = '6';
			break;
		case 7:
			str.at(index) = '7';
			break;
		case 8:
			str.at(index) = '8';
			break;
		case 9:
			str.at(index) = '9';
			break;
		case 10:
			str.at(index) = 'A';
			break;
		case 11:
			str.at(index) = 'B';
			break;
		case 12:
			str.at(index) = 'C';
			break;
		case 13:
			str.at(index) = 'D';
			break;
		case 14:
			str.at(index) = 'E';
			break;
		case 15:
			str.at(index) = 'F';
			break;
		}

		index--;
		maskResult = maskResult / 16;
	}

	return str;
}

void LUT::genWireInport(void)
{
	for (map<string, InPort *>::const_iterator it = inports.begin(); 
		it != inports.end(); it++)
	{
		if(it->second != NULL){

			//cout << it->second->getRealName() << endl;

			this->pnode->addTempWireFromInport(*(it->second));

		}
	}
}


int LUT::getInoutPortRealNameCount(void) const
{
	return this->inoutPortRealNameCount;
}

string LUT::getLutMask(void) const
{
	return this->lutMask;
}

string LUT::getMaskMode(void) const
{
	return this->maskMode;
}

bool LUT::isBuffer(void)
{
	return buffer;
}

//int LUT::getOutportTypeCount(void) const
//{
//	return this->OUTPORT_TYPE_COUNT;
//}

string LUT::locationTrans(string str)
{
	string temp = "0000";

	for (int i = 0; i < maskMode.size(); i++)
	{
		char ch = maskMode.at(i);

		switch(ch){
		case 'd':
			temp.at(0) = str.at(i);
			break;
		case 'c':
			temp.at(1) = str.at(i);
			break;
		case 'b':
			temp.at(2) = str.at(i);
			break;
		case 'a':
			temp.at(3) = str.at(i);
			break;
		}

	}

	return temp;
}

void LUT::mask(void)
{
	if (this->inoutPortRealNameCount > 1)
	{
		bitset<16> bs;
		//cout << bs << endl;

		if(outFormat == '1'){
			this->maskFormatOne(bs);
		}else{
			this->maskFormatZero(bs);
		}

		this->lutMask = this->decToHex(bs.to_ulong());

		//cout << this->lutMask << endl;


		//cout << hex;
		//cout << maskResult << endl;

		/*for (vector<string>::const_iterator it = inTrueTable.begin(); it != inTrueTable.end(); it++)
		{
		cout << *it << endl;
		}
		cout << outFormat << endl;*/
	}else{
		switch(outFormat){
		case '0':
			this->lutMask = "0000";
			break;
		case '1':
			this->lutMask = "FFFF";
			break;
		}
	}
}

void LUT::maskFormatOne(bitset<16> &bs)
{
	for (vector<string>::const_iterator it = inTrueTable.begin(); it != inTrueTable.end(); it++)
	{
		string str = *it;
		int len = str.size();

		if(len < INPORT_TYPE_COUNT){
			for(int i = 0; i < INPORT_TYPE_COUNT - len; i++){
				str = str + "-";
			}
		}

		str = this->locationTrans(str);

		queue<string> qstr;

		qstr.push(str);

		while(!qstr.empty()){
			string tempStr = qstr.front();

			int index = 0;
			if((index = tempStr.find_first_of('-')) != string::npos){
				string str1 = tempStr;
				str1.at(index) = '0';
				qstr.push(str1);

				string str2 = tempStr;
				str2.at(index) = '1';
				qstr.push(str2);
			}else{
				bitset<4> bsStr(tempStr);
				bs.at((int)bsStr.to_ulong()) = 1;
			}

			qstr.pop();
		}

	}
}

void LUT::maskFormatZero(bitset<16> &bs)
{
	this->maskFormatOne(bs);
	bs.flip();
}

//vector<string> LUT::getInoutPortRealName(void)
//{
//	return this->inoutPortRealName;
//}

void LUT::matchPort(void)
{
	//cout << inoutPortRealName.size() << endl;

	// dcba
	for(int i = 0; i < inoutPortRealName.size() - OUTPORT_TYPE_COUNT; i++){
		inports[inportName[i]] = new InPort(this, inportName[i], inoutPortRealName.at(i));
	}

	for (int i = 0; i < OUTPORT_TYPE_COUNT; i++)
	{
		outports[outportName[i]] = new OutPort(this, outportName[i], 
			inoutPortRealName.at(inoutPortRealName.size() + i - OUTPORT_TYPE_COUNT));
	}
}

void LUT::setBuffer(bool buffer)
{
	this->buffer = buffer;
}

void LUT::setCorrLatch(Latch &latch)
{
	this->corrLatch = &latch;
}

void LUT::setLutMask(string lutMask)
{
	this->lutMask = lutMask;
}

void LUT::setMaskMode(string maskMode)
{
	this->maskMode = maskMode;
}

//void LUT::setAdataLUT(bool adataLUT)
//{
//	this->adataLUT = adataLUT;
//}
//
//bool LUT::isAdataLUT(void) const
//{
//	return this->adataLUT;
//}






void LUT::setOutFormat(char ch)
{
	this->outFormat = ch;
}

vector<string> & LUT::getInTrueTable(void)
{
	return this->inTrueTable;
}

char LUT::getOutFormat(void) const
{
	return this->outFormat;
}
