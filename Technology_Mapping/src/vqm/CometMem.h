#pragma once
#include "node.h"

class CometMem :
	public Node
{
public:
	CometMem(Node * node, string name, string ramModelName, string nodeType, string ramVFile);
	~CometMem(void);

	static const int INPORT_TYPE_COUNT;         // Standard inport num
	static const int OUTPORT_TYPE_COUNT;     // Standard outport num

	static const string inportName [];       // Standard inport names
	static const string outportName [];     // Standard outport names

	void matchPort(void);

	void addInoutPortRealName(string portName, string realName, int type);

	void genWireInport(void);  // Generate wire from inport 

	string getRamVFile(void);

	void setPortA(vector<string> &tmpPortA);
	void setPortB(vector<string> &tmpPortB);

	vector<string> & getPortA(void) const;
	vector<string> & getPortB(void) const;

	string getLogicName(void) const;
	void setLogicName(string name);

	string getRamIniFilePath(void) const;
	void setRamIniFilePath(string path);

private:
	Node * pnode;
	map<string, string> inportRealName;       // BLIF portName, realName
	map<string, string> outportRealName;    // BLIF portName, realName
	string ramModelName;     // such as fdce1...

	string ramVFile;
	string logicName;
	string ramIniFilePath;

	vector<string> * portA;
	vector<string> * portB;
};

