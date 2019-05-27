#pragma once
#include <string>
#include <map>

class Wire;
class Node;

using std::string;
using std::map;

class Port
{
public:
	Port(void);
	~Port(void);

	string getRealName(void) const;   // real name of wire in net conned to certain port 
	string getAutoName(void) const;  // std port name in element, such as LUT, Reg...
	void setRealName(string realName);

	Node & getPnode(void) const;
	void addWire(string str, Wire &wire);

	map<string, Wire *> & getWires(void);

	virtual string showPortInfo(void) const;

protected:
	// Standard name
	string autoName;
	// Name in BLIF
	string realName;
	// Whom it belongs to
	Node * pnode;
	// Wires connected to
	map<string, Wire *> wires;
};

