#include "LE.h"
#include <iostream>

using std::cout;
using std::endl;

LE::LE(Node * node, string name)
{
	this->name = name;
	this->pnode = node;
	this->lut = NULL;
	this->reg = NULL;
}


LE::~LE(void)
{
	//cout << this->name << " deconstruct..." << endl;
}


const int LE::INPORT_TYPE_COUNT = 15;

const int LE::OUTPORT_TYPE_COUNT = 5;

const string LE::inportName [INPORT_TYPE_COUNT] = {"clk", "dataa", "datab", "datac", 
	"datad", "aclr", "aload", "sclr", "sload", "ena", "cin", "cin0", "cin1", "inverta", "regcascin"};

const string LE::outportName [OUTPORT_TYPE_COUNT] = {"combout", "regout", "cout", 
	"cout0", "cout1"};

const int LE::DEFPARAM_TYPE_COUNT = 6;

const string LE::defParamName [DEFPARAM_TYPE_COUNT] = {"operation_mode", 
	"synch_mode", "register_cascade_mode", "sum_lutc_input", 	"lut_mask", 
	"output_mode"};

LUT * LE::getLUT(void) const
{
	return this->lut;
}

void LE::setLUT(LUT &lut)
{
	this->lut = &lut;
}

Reg * LE::getReg(void) const
{
	return this->reg;
}

void LE::setReg(Reg &reg)
{
	this->reg = &reg;
}

string LE::getName(void) const
{
	return this->name;
}

void LE::addInport(string stdPortName, string portRealName)
{
	inports[stdPortName] = portRealName;
}

void LE::addOutport(string stdPortName, string portRealName)
{
	outports[stdPortName] = portRealName;
}

map<string, string> & LE::getInports(void)
{
	return inports;
}

map<string, string> & LE::getOutports(void)
{
	return outports;
}

map<string, string> & LE::getDefparams(void)
{
	return defparams;
}

void LE::setDefparams(string stdDefparamName, string val)
{
	defparams[stdDefparamName] = val;
}
