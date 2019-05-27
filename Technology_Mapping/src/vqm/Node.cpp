#include "Node.h"

Node::Node(string nodeType)
{
	this->blackbox = false;
	this->packed = false;
	this->nodeType = nodeType;
}

Node::~Node(void)
{
}

const string Node::REG_TYPE= "Reg";
const string Node::LATCH_TYPE = "Latch";
const string Node::LATCH_LUT_TYPE = "Latch_LUT";
const string Node::MODEL_TYPE= "Model";
const string Node::LUT_TYPE = "LUT";
const string Node::PLL_TYPE = "PLL";
const string Node::RAM_TYPE = "RAM";

string Node::getName(void) const
{
	return this->name;
}

void Node::setBlackbox(bool blackbox)
{
	this->blackbox = blackbox;
}

bool Node::isPacked(void) const
{
	return this->packed;
}

void Node::setPacked(bool packed)
{
	this->packed = packed;
}

map<string, OutPort *> & Node::getOutports(void)
{
	return outports;
}

void Node::addTempWireFromInport(InPort &)
{

}

map<string, InPort *> & Node::getInports(void)
{
	return inports;
}

string Node::getNodeType(void)
{
	return this->nodeType;
}

string Node::getLutMask(void) const
{
	return "";
}

void Node::setMaskMode(string maskMode)
{

}

void Node::mask(void)
{

}
