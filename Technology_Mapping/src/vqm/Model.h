#pragma once

#include "Node.h"
#include <string>
#include <map>
#include <vector>
#include <list>
#include "InPort.h"
#include "OutPort.h"
#include "LUT.h"
#include "Latch.h"
#include "Wire.h"
#include "Reg.h"
#include "SubcktModelInfo.h"
#include "LE.h"
#include "CometPLL.h"
#include "PLL.h"
#include "CometMem.h"
#include "RamBlock.h"

using std::vector;
using std::string;
using std::map;
using std::list;

class Model:
	public Node
{
public:
	Model(string name, string nodeType, string infoFile);
	~Model(void);

	void addInPort(OutPort &inport);
//	map<string, InPort *> getInputs(void) const;
	void addOutPort(InPort &outport);
	void addLUT(LUT &lut);
//	map<string, OutPort *> getOutputs(void) const;

//	long getLutCount(void) const;
	string genLutRegLatchName(string nodeType);  // Element name
	string genPllRamName(string nodeType);  // Element name

	//map<string, LUT *> getLuts(void) const;

	// new a structual to save subckt info for certain subckt instance
	void addSubckt(string subcktType, string curSubcktModelName, string subcktName);  
	void addInfoToSubckt(string subcktType, string curSubcktModelName, string subcktName, string portName, string realName);
	void showSubckts(void);
	// map<string, map<string, vector<string> *> *> & getSubckts(void);      ////  !!!!!!!!!!!!!!!!!

	void setModel(map<string, map<string, SubcktModelInfo *> *> &subcktModelsInfo);

	map<string, InPort *> & getOutportsPseudoInports(void);
	map<string, OutPort *> & getInportsPseudoOutports(void);

	map<string, int> & getInportStatus(void);
	map<string, int> & getOutportStatus(void);

	map<string, LE *> & getLEs(void);
	map<string, PLL *> & getPLLs(void);
	map<string, RamBlock *> & getRamBlocks(void);

	long getPllCount(void) const;
	long getRamCount(void) const;

	map<string, string> & getBufferPortInfo(void);

private:
	bool regCascade;
	bool bufferAway;

	long lutCount;
	string iniLutName;
	
	long regCount;
	string iniRegName;

	long latchCount;
	string iniLatchName;

	long pllCount;
	string iniPllName;

	long ramCount;
	string iniRamName;

	string iniWireName;

	string iniLEName;

	// inports and outports only for model
	// NOTICE!!!!!!!!!!!!!!!!
	map<string, InPort *> outportsPseudoInports;  // inner 
	map<string, OutPort *> inportsPseudoOutports;

	// Port Array ??? 
   // Single: -1   Array: max
	map<string, int> inportStatus, outportStatus;
	map<string, string> * bufferPortInfo;
	// all subckts, luts and wires

	// Eg: reg->fd1->reg[0]->(<ena, n1>, <data0, n2>)
	map<string, map<string, map<string, map<string, string> *> *> *> subckts;
	
	map<string, LUT *> luts;
	map<string, vector<Wire *> *> tempWires;  //RealName, wires
	map<string, Wire *> wires;
	map<string, Reg *> regs;
	map<string, Latch *> latches;
	map<string, CometPLL *> cometPlls;
	map<string, CometMem *> cometMems;

	map<string, LE *> les;
	map<string, PLL *> plls;
	map<string, RamBlock *> ramBlocks;

	string getWireRealNameFromInport(InPort &inport);
	string genWireNameFromTempWire(Wire &wire, OutPort &outport);  // Get wire final name
	
	void connWires(void);
	void addTempWireFromInport(InPort &);  // Generate wire according to inport
	void connectTempWireToOutport(OutPort &);
	void connectWireToInport(void);

	void matchLutPort(void);
	void maskLut(void);

	void addReg(Reg &reg);
	void matchRegPort(void);

	void addCometPll(CometPLL &pll);
	void matchCometPllPort(void);

	void addCometMem(CometMem &mem);
	void matchCometMemPort(void);

	void addLatch(Latch &latch);
	void transLatchToLUT(void);

	// Generate Subckt Objs
	void genSubckt(map<string, map<string, SubcktModelInfo *> *> &subcktModelsInfo);

	// Generate LE
	string genLEName(string lutName, string regName);
	void pack(void);
	void pack_unpack(void);
	void pack_pack(void);	
	void pack_pll(void);
	void pack_ram_block(void);

	void showLEInfo(LE &tempLE);

	void genPortStatus(void);

	vector<string> split(string str, string ptn);

	void bufferGone(void);    // Move buffer away
	void maskRecovery(void);

	string trim(string str);  // no blank front or end of one line
	string toLowerCase(string str);
	string toUpperCase(string str);

	string getPhaseShiftDutyCycleVal(string val);
};
