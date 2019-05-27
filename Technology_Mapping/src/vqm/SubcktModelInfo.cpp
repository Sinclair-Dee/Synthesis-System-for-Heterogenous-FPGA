#include "SubcktModelInfo.h"
#include <iostream>

using std::cout;
using std::endl;

SubcktModelInfo::SubcktModelInfo(string subcktType, string subcktName)
{
	this->subcktName = subcktName;
	this->subcktType = subcktType;
}

SubcktModelInfo::~SubcktModelInfo(void)
{
	//cout << "subcktModelInfo deconstruct..." << endl;
}

void SubcktModelInfo::setSubcktName(string subcktName)
{
	this->subcktName = subcktName;
}

void SubcktModelInfo::setBlackbox(bool blackbox)
{
	this->blackbox = blackbox;
}

void SubcktModelInfo::addSubcktInports(string subcktInport)
{
	this->subcktInports.push_back(subcktInport);
}

void SubcktModelInfo::addSubcktOutports(string subcktOutport)
{
	this->subcktOutports.push_back(subcktOutport);
}

vector<string> & SubcktModelInfo::getSubcktInports(void)
{
	return this->subcktInports;
}

vector<string> & SubcktModelInfo::getSubcktOutports(void)
{
	return this->subcktOutports;
}
