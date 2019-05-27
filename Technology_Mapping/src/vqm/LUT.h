#pragma once

#include "Node.h"
#include <bitset>
#include "Latch.h"

using std::bitset;

class LUT : 
	public Node
{
public:

	LUT(Node * node, string name, string nodeType);
	~LUT(void);

	bool isBuffer(void);
	void setBuffer(bool buffer);
	//	vector<string> getInoutPortRealName(void);
	//int getOutportTypeCount(void) const;

	void mask(void);
	void setLutMask(string lutMask);
	void setOutFormat(char);
	void addInTrueTable(string);
	
	void genWireInport(void);

	void matchPort(void);

	void setCorrLatch(Latch &latch);  // if generated from latch

	void addInoutPortRealName(string name);  // port realname list from BLIF .names

	string getLutMask(void) const;

	//bool isAdataLUT(void) const;
	//void setAdataLUT(bool adataLUT);

	string getMaskMode(void) const;
	void setMaskMode(string maskMode);

	int getInoutPortRealNameCount(void) const;

	vector<string> & getInTrueTable(void);

	char getOutFormat(void) const;

private:	

	Latch * corrLatch;
	Node * pnode;
	string lutMask;
	string maskMode;
	//bool adataLUT;
	
	char outFormat;             // items in BLIF determine TrueTable 1 or 0
	vector<string> inTrueTable;
	vector<string> inoutPortRealName;
	int inoutPortRealNameCount;

	bool buffer;

	void maskFormatOne(bitset<16> &);
	void maskFormatZero(bitset<16> &);
	string locationTrans(string);           // Port Priority?
	string decToHex(long); 

	static const int INPORT_TYPE_COUNT;        // Standard inport num
	static const int OUTPORT_TYPE_COUNT;     // Standard outport num

	static const string inportName [];       // Standard inport names
	static const string outportName [];     // Standard outport names
	
};

