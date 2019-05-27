#pragma once

#include "Port.h"

class InPort:
	public Port
{
public:
	InPort(Node * pnode, string autoName, string realName);
	~InPort(void);
};

