#include <string>
#include <vector>
#include <map>
#include "SubcktModelInfo.h"
#include "Model.h"
#include "InPort.h"
#include "OutPort.h"
#include "LUT.h"
#include "Reg.h"

using std::string;
using std::vector;
using std::map;

class BlifToVQM
{	
public:
	BlifToVQM(string fromFile, string toFile, string vqmFile, string infoFile);
	~BlifToVQM(void);

	static const string REG_NAME;
	static const string REG_TYPE;
	static const string LATCH_NAME;
	static const string LATCH_TYPE;
	static const string LUT_TYPE;
	static const string PLL_NAME;
	static const string PLL_TYPE;
	static const string RAM_NAME;
	static const string RAM_TYPE;
	static const string MODEL_TYPE;

	static class Err{};

private:
	string fromFile;
	string toFile;
	string vqmFile;
	string infoFilePath;
	string vqmInfo;
	string pllInfo;
	string ramInfo;

	long lutOnlyLECount;
	long regOnlyLECount;
	long lutRegLECount;
	long regsLECount;
	long LECount;
	int dspCount;
	int pllCount;
	int memCount;

	long leCnt;
	int pllCnt;
	int dspCnt;
	int memCnt;

	enum ScopeState{
		NONE, MODEL, INPUTS, OUTPUTS, SUBCKT, NAMES, BLACKBOX, END
	};
	ScopeState scopeState;
	int stateCount;

	Model * topModel;
	string topModelName;
	string curModelName;
	LUT * curLUT;
	string curSubcktType;
	string curSubcktName;
	string curSubcktModelName;

	vector<Model *> models;
	map<string, map<string, SubcktModelInfo *> *> subcktModelsInfo;  // Eg: reg->fd1->subckt model info

	map<string, vector<string> *> infoFiles;

	string vsion;
	string family;

	void showInfo(void);

	void parseInfoFile(string pllRamInfoFile);
	void parseFile(void);

	void parseModel(string);
	void parseInputs(string);
	void parseOutputs(string);
	void parseSubckt(string);
	void parseNone(void);
	void parseNames(string, int &);
	void parseBlackbox(void);
	void parseEnd(void);

	vector<string> split(string str, string ptn);
	string trim(string str);  // no blank front or end of one line

	void setTopModel(map<string, map<string, SubcktModelInfo *> *> &);
	
	void addSubcktModelInfo(string subcktName, string subcktModelName);

	void printVQM(void);

	string getVqmInfo(void);

	string straightLineAway(string str);  // no "|" in wire name

	void printReport(void) const;
};



