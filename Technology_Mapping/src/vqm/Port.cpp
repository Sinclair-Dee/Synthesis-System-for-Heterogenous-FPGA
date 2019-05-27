#include "Port.h"


Port::Port(void)
{
}


Port::~Port(void)
{
}

string Port::getRealName(void) const
{
	return realName;
}

string Port::getAutoName(void) const
{
	return autoName;
}

Node & Port::getPnode( void ) const
{
	return *pnode;
}

void Port::addWire(string str, Wire &wire)
{
	wires[str] = &wire;
}

map<string, Wire *> & Port::getWires(void)
{
	return this->wires;
}

string Port::showPortInfo(void) const
{
	return this->autoName + ": " + this->realName;
}

void Port::setRealName(string realName)
{
	this->realName = realName;
}

