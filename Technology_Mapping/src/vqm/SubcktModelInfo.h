#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

class SubcktModelInfo
{
public:
	SubcktModelInfo(string subcktType, string subcktName);
	~SubcktModelInfo(void);

	bool isBlackbox(void) const;
	vector<string> & getSubcktInports(void);
	vector<string> & getSubcktOutports(void);

	void setSubcktName(string subcktName);
	void setBlackbox(bool isBlackbox);

	void addSubcktInports(string subcktInport);
	void addSubcktOutports(string subcktOutport);

private:
	string subcktType;
	string subcktName;
	vector<string> subcktInports; 
	vector<string> subcktOutports; 
	bool blackbox;

};

