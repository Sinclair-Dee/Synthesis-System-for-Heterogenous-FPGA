#include <iostream>
#include "InPort.h"

using std::cout;
using std::endl;

InPort::InPort(Node * node, string autoName, string realName)
{
	this->pnode = node;
	this->autoName = autoName;
	this->realName = realName;

	//cout << "Inport:" << autoName << " " << realName << endl;
}

InPort::~InPort(void)
{
	//cout << "inport deconstruct..." << endl;

}



