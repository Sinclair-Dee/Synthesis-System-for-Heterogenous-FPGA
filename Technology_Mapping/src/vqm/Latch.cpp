#include "Latch.h"
#include "Model.h"
#include <iostream>

using std::cout;
using std::endl;

Latch::Latch(Node * node, string name, string latchModelName, string nodeType): Node(nodeType)
{
	this->name = name;
	this->pnode = node;
	this->latchModelName = latchModelName;
	this->corrLUT = NULL;
}


Latch::~Latch(void)
{
	//cout << this->name << " deconstruct..." << endl;
}


const int Latch::INPORT_TYPE_COUNT = 2;

const int Latch::OUTPORT_TYPE_COUNT = 1;

const string Latch::inportName [INPORT_TYPE_COUNT] = {"data0", "ena"};

const string Latch::outportName [OUTPORT_TYPE_COUNT] = {"latchout"};

void Latch::addInoutPortRealName( string portName, string realName, int type )
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

void Latch::transToLUT(void)
{
	corrLUT = new LUT(pnode, "lut_" + this->name, LATCH_LUT_TYPE);
	((Model *)pnode)->addLUT(*corrLUT);    // Warning!!!!!!!!!!!!!!!!!!!!!!!!!!!
	corrLUT->setCorrLatch(*this);

	if(outportRealName.count("Q0") != 0)      // Q0    datad
	{
		corrLUT->addInoutPortRealName(outportRealName["Q0"]);
	}

	if(inportRealName.count("Gate") != 0)      // ena    datac
	{
		corrLUT->addInoutPortRealName(inportRealName["Gate"]);	
	}

	if(inportRealName.count("Data0") != 0)      // data0    datab
	{
		corrLUT->addInoutPortRealName(inportRealName["Data0"]);	
	}

	if(outportRealName.count("Q0") != 0)      // Q0    combout
	{
		corrLUT->addInoutPortRealName(outportRealName["Q0"]);
	}

	corrLUT->setLutMask(this->maskLUT());
}

string Latch::maskLUT(void)
{
	return "CFC0";
}
