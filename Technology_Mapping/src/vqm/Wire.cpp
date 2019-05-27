#include "Wire.h"
#include <iostream>

using std::cout;
using std::endl;

Wire::Wire(Node &pnode, string wireName, InPort &inport)
{
	this->wireName = wireName;
	this->inport = &inport;
	this->pnode = &pnode;
	this->outport = NULL;

}

Wire::~Wire(void)
{
	//cout << this->wireName << " deconstruct..." << endl;
}

void Wire::setOutport(OutPort &outport)
{
	this->outport = &outport;
}

string Wire::getHierInportName(void) const
{
	
	return (this->inport)->getPnode().getName() + "[" + 
		this->inport->getAutoName() + "]" + "~" + 
		(this->inport)->getRealName();
}

void Wire::setWireName(string wireName)
{
	this->wireName = wireName;
}

string Wire::getWireName(void) const
{
	return this->wireName;
}

InPort & Wire::getInport(void) const
{
	return *inport;
}

OutPort & Wire::getOutport(void) const
{
	return *outport;
}
