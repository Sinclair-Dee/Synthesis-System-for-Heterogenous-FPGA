#include "Reg.h"
#include <iostream>

using std::cout;
using std::endl;

Reg::Reg(Node * node, string name, string regModelName, string nodeType): Node(nodeType)
{
	this->name = name;
	this->pnode = node;
	this->regModelName = regModelName;
}

Reg::~Reg(void)
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

const int Reg::INPORT_TYPE_COUNT = 8;

const int Reg::OUTPORT_TYPE_COUNT = 1;

const string Reg::inportName [INPORT_TYPE_COUNT] = {"clk", "data0", "aclr", 
	"ena", "sclr", "aload", "sload", "adata"};

const string Reg::outportName [OUTPORT_TYPE_COUNT] = {"regout"};

void Reg::addInoutPortRealName(string portName, string realName, int type)
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

 const string Reg::getPortName(string str)
{
	if(!str.compare("Clock")){
		return inportName[0];                //"clk";
	}else if(!str.compare("Data0")){
		return inportName[1];                //"data0";
	}else if(!str.compare("Aclr")){
		return inportName[2];                //"aclr";
	}else if(!str.compare("Enable")){
		return inportName[3];                //"ena";
	}else if(!str.compare("Sclr")){
		return inportName[4];               //"sclr";
	}else if(!str.compare("Aset")){
		return inportName[5];               //"aload";
	}else if(!str.compare("Sset")){
		return inportName[6];              //"sload";
	}else if(!str.compare("Q0")){
		return outportName[0];            //"regout";
	}

	return "";
}

void Reg::genWireInport( void )
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

void Reg::matchPort(void)
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

			string stdPortName = this->getPortName(portName);
			inports[stdPortName] = new InPort(this, stdPortName, portRealName);

			if(!(portName.compare("Aset") && portName.compare("Sset"))){

				inports[inportName[7]] = new InPort(this, inportName[7], "vcc");   // "adata"

			}

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
			string stdPortName = this->getPortName(portName);

			outports[stdPortName] = new OutPort(this, stdPortName, portRealName);
	}
}
