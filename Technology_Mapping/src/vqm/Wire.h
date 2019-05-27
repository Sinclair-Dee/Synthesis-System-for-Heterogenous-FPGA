#pragma once

#include <string>
#include "InPort.h"
#include "OutPort.h"
#include "Node.h"

using std::string;

class Wire
{
public:
	Wire(Node &pnode, string wireName, InPort &inport);
	~Wire(void);

	void setWireName(string wireName);
	void setOutport(OutPort &outport);

	string getHierInportName(void) const;
	string getWireName(void) const;

	InPort & getInport(void) const;
	OutPort & getOutport(void) const;

private:
	string wireName;
    // Whom it belongs to
	Node * pnode;	
	// To which InPort
	InPort * inport;
	// To which OutPort
	OutPort * outport;
};

