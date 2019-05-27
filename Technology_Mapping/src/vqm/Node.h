#pragma once
#include <string>
#include <map>
#include <vector>
#include "InPort.h"
#include "OutPort.h"

using std::string;
using std::map;
using std::vector;

/*
    NODE:
          LUTS, WIRES, REGS
*/

class Node
{
public:
	Node(string nodeType);
	~Node(void);

	virtual void addTempWireFromInport(InPort &);
	virtual string getLutMask(void) const;
	virtual void setMaskMode(string maskMode);
	virtual void mask(void);

	string getName(void) const;
	void setBlackbox(bool blackbox);

	bool isPacked(void) const;
	void setPacked(bool packed);

	map<string, InPort *> & getInports(void);
	map<string, OutPort *> & getOutports(void);

	string getNodeType(void);

protected:
	string name;
	bool blackbox;
	string nodeType;
	bool packed;

	static const string MODEL_TYPE;
	static const string LUT_TYPE;
	static const string REG_TYPE;
	static const string LATCH_TYPE;
	static const string LATCH_LUT_TYPE;
	static const string PLL_TYPE;
	static const string RAM_TYPE;

	map<string, InPort *> inports;
	map<string, OutPort *> outports;
};

