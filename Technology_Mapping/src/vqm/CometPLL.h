#pragma once
#include "node.h"

class CometPLL :
	public Node
{
public:
	CometPLL(Node * node, string name, string pllModelName, string nodeType, string pllVFile);
	~CometPLL(void);

	static const int INPORT_TYPE_COUNT;         // Standard inport num
	static const int OUTPORT_TYPE_COUNT;     // Standard outport num

	static const string inportName [];       // Standard inport names
	static const string outportName [];     // Standard outport names

	void matchPort(void);

	void addInoutPortRealName(string portName, string realName, int type);

	void genWireInport(void);  // Generate wire from inport 

	string getPllVFile(void);

private:
	Node * pnode;
	map<string, string> inportRealName;       // BLIF portName, realName
	map<string, string> outportRealName;    // BLIF portName, realName
	string pllModelName;     // such as fdce1...

	string pllVFile;
};

