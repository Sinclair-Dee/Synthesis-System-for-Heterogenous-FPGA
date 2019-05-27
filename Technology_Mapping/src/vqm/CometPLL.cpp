#include "CometPLL.h"
#include <iostream>

using std::cout;
using std::endl;

CometPLL::CometPLL(Node * node, string name, string pllModelName, string nodeType, string pllVFile): Node(nodeType)
{
	this->name = name;
	this->pnode = node;
	this->pllModelName = pllModelName;
	this->pllVFile = pllVFile;
}

CometPLL::~CometPLL(void)
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

const int CometPLL::INPORT_TYPE_COUNT = 4;
const int CometPLL::OUTPORT_TYPE_COUNT = 4;

const string CometPLL::inportName [INPORT_TYPE_COUNT] = {"inclk0", "pllena", "areset", "pfdena"};
const string CometPLL::outportName [OUTPORT_TYPE_COUNT] = {"c0", "c1", "e0", "locked"};

void CometPLL::addInoutPortRealName(string portName, string realName, int type)
{
	switch(type){
	case 0:
		outportRealName[portName] = realName;
		break;
	case 1:
		inportRealName[portName] = realName;
		break;
	}
}

void CometPLL::genWireInport(void)
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

string CometPLL::getPllVFile(void)
{
	return this->pllVFile;
}

void CometPLL::matchPort(void)
{
	//cout << inoutPortRealName.size() << endl;
	//for(int i = 0; i < inportRealName.size(); i++){
	//	inports[inportName[i]] = new InPort(this, inportName[i], 
	//		inoutPortRealName[this->getPortName(inportName[i])]);
	//}

	for(map<string, string>::const_iterator it = inportRealName.begin(); 
		it != inportRealName.end(); it++){
			string portName = it->first;
			string portRealName = it->second;

			inports[portName] = new InPort(this, portName, portRealName);
	}

	//for (int i = 0; i < OUTPORT_TYPE_COUNT; i++)
	//{
	//	outports[outportName[i]] = new OutPort(this, outportName[i], 
	//		inoutPortRealName[this->getPortName(outportName[i])]);
	//}

	for(map<string, string>::const_iterator it = outportRealName.begin(); 
		it != outportRealName.end(); it++){
			string portName = it->first;
			string portRealName = it->second;

			outports[portName] = new OutPort(this, portName, portRealName);
	}
}
