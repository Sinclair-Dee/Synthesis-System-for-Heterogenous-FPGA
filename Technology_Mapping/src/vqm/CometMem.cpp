#include "CometMem.h"
#include <iostream>

using std::cout;
using std::endl;

CometMem::CometMem(Node * node, string name, string ramModelName, string nodeType, string ramVFile): Node(nodeType)
{
	this->name = name;
	this->pnode = node;
	this->ramModelName = ramModelName;
	this->ramVFile = ramVFile;
}

CometMem::~CometMem(void)
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

	if (portA != NULL)
	{
		delete portA;
	}

	if (portB != NULL)
	{
		delete portB;
	}
}

const int CometMem::INPORT_TYPE_COUNT = 4;
const int CometMem::OUTPORT_TYPE_COUNT = 4;

const string CometMem::inportName [INPORT_TYPE_COUNT] = {"inclk0", "pllena", "areset", "pfdena"};
const string CometMem::outportName [OUTPORT_TYPE_COUNT] = {"c0", "c1", "e0", "locked"};

void CometMem::addInoutPortRealName(string portName, string realName, int type)
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

void CometMem::genWireInport(void)
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

string CometMem::getRamVFile(void)
{
	return this->ramVFile;
}

void CometMem::matchPort(void)
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

void CometMem::setPortA(vector<string> &tmpPortA)
{
	this->portA = &tmpPortA;
}

void CometMem::setPortB(vector<string> &tmpPortB)
{
	this->portB = &tmpPortB;
}

vector<string> & CometMem::getPortA(void) const
{
	return *portA;
}

vector<string> & CometMem::getPortB(void) const
{
	return *portB;
}

string CometMem::getLogicName(void) const
{
	return this->logicName;
}

void CometMem::setLogicName(string name)
{
	this->logicName = name;
}

string CometMem::getRamIniFilePath(void) const
{
	return this->ramIniFilePath;
}

void CometMem::setRamIniFilePath(string path)
{
	this->ramIniFilePath = path;
}
