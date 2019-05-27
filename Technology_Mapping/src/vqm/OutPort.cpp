#include "OutPort.h"
#include <iostream>

using std::cout;
using std::endl;

OutPort::OutPort(Node * node, string autoName, string realName)
{
	this->pnode = node;
	this->autoName = autoName;
	this->realName = realName;

	//cout << "Outport:" << autoName << " " << realName << endl;

}


OutPort::~OutPort(void)
{
	//cout << "outport deconstruct..." << endl;

}

void OutPort::setWires(string str, Wire &wire)
{
	this->wires[str] = &wire;
}
