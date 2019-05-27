#pragma once
#include "node.h"

class LUT;

class Latch :
	public Node
{
public:
	Latch(Node * node, string name, string latchModelName,  string nodeType);
	~Latch(void);


	void transToLUT(void);

	//static const string getPortName(string str);

	void addInoutPortRealName(string portName, string realName, int type);

	//void genWireInport(void);

	//map<string, OutPort *> * getOutports(void);



private:
	LUT * corrLUT;
	Node * pnode;
	map<string, string> inportRealName;
	map<string, string> outportRealName;
	string latchModelName;  // such as latch1, latch2...

	static const int INPORT_TYPE_COUNT;        // Standard inport num
	static const int OUTPORT_TYPE_COUNT;     // Standard outport num

	static const string inportName [];       // Standard inport names
	static const string outportName [];     // Standard outport names


	string maskLUT(void);  // mask LUT generated
};

