#pragma once
#include "Port.h"

class OutPort:
	public Port
{
public:
	OutPort(Node * pnode, string autoName, string realName);
	~OutPort(void);

	void setWires(string str, Wire &wire);
};

