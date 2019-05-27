#pragma once
#include "Node.h"

class Reg:
	public Node
{
public:
	Reg(Node * node, string name, string regModelName, string nodeType);
	~Reg(void);

	void matchPort(void);

	void addInoutPortRealName(string portName, string realName, int type);

	void genWireInport(void);  // Generate wire from inport 

private:
	Node * pnode;
	map<string, string> inportRealName;       // BLIF portName, realName
	map<string, string> outportRealName;    // BLIF portName, realName
	string regModelName;     // such as fdce1...

	static const int INPORT_TYPE_COUNT;         // Standard inport num
	static const int OUTPORT_TYPE_COUNT;     // Standard outport num

	static const string inportName [];       // Standard inport names
	static const string outportName [];     // Standard outport names

	static const string getPortName(string str);

};

