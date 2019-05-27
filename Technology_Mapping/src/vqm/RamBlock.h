#pragma once
#include <string>
#include <map>

using std::string;
using std::map;

class Node;
class CometMem;

class RamBlock
{
public:
	RamBlock(Node * node, string name);
	~RamBlock(void);

	CometMem * getCometMem(void) const;   
	void setCometMem(CometMem &cometMem);

	string getName(void) const;

	static const int INPORT_TYPE_COUNT;        // Standard inport num
	static const int OUTPORT_TYPE_COUNT;     // Standard outport num

	static const string inportName [];       // Standard inport names
	static const string outportName [];     // Standard outport names

	static const int DEFPARAM_TYPE_COUNT;     // Standard defparam num
	static const string defParamName [];    // Standard defparam names

	void addInport(string stdPortName, string portRealName);
	void addOutport(string stdPortName, string portRealName);
	
	void delInPort(string portName);

	void setDefparams(string stdDefparamName, string val);
	void modifyParam(string key, string val);
	string getPara(string key);

	map<string, string> & getInports(void); 
	map<string, string> & getOutports(void); 
	map<string, string> & getDefparams(void); 

	void setFlagPortB(bool val);
	bool getFlagPortB(void) const;

	void exchangeParam(string firstParam, string secondPara);

private:
	string name;
	Node * pnode;
	CometMem * cometMem;

	map<string, string> inports, outports, defparams;    // [std port, wire] or [std defname, value]

	bool flagPortB;
};

