#pragma once
#include <string>
#include <map>

using std::string;
using std::map;

class Node;
class LUT;
class Reg;

class LE
{
public:
	LE(Node * node, string name);
	~LE(void);

	LUT * getLUT(void) const;   
	void setLUT(LUT &lut);

	Reg * getReg(void) const;     
	void setReg(Reg &reg);

	string getName(void) const;

	static const int INPORT_TYPE_COUNT;        // Standard inport num
	static const int OUTPORT_TYPE_COUNT;     // Standard outport num

	static const string inportName [];       // Standard inport names
	static const string outportName [];     // Standard outport names

	static const int DEFPARAM_TYPE_COUNT;     // Standard defparam num
	static const string defParamName [];    // Standard defparam names

	void addInport(string stdPortName, string portRealName);
	void addOutport(string stdPortName, string portRealName);
	void setDefparams(string stdDefparamName, string val);

	map<string, string> & getInports(void); 
	map<string, string> & getOutports(void); 
	map<string, string> & getDefparams(void); 

private:
	string name;
	Node * pnode;
	LUT * lut;
	Reg * reg;

	map<string, string> inports, outports, defparams;    // [std port, wire] or [std defname, value]

};

