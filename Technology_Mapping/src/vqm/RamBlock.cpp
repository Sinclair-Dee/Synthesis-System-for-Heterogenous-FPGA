#include "RamBlock.h"
#include <iostream>

using std::cout;
using std::endl;

RamBlock::RamBlock(Node * node, string name)
{
	this->name = name;
	this->pnode = node;
	this->cometMem = NULL;
	this->flagPortB = false;
}


RamBlock::~RamBlock(void)
{
	//cout << this->name << " deconstruct..." << endl;
}


const int RamBlock::INPORT_TYPE_COUNT = 14;

const int RamBlock::OUTPORT_TYPE_COUNT = 2;

const string RamBlock::inportName [INPORT_TYPE_COUNT] = {"portadatain", "portaaddr", "portawe", 
	"portbdatain", "portbaddr", "portbrewe", "clk0", "clk1", "ena0", "ena1", "clr0", "clr1", 
	"portabyteenamasks", "portbbyteenamasks"};

const string RamBlock::outportName [OUTPORT_TYPE_COUNT] = {"portadataout", "portbdataout"};

const int RamBlock::DEFPARAM_TYPE_COUNT = 39;

const string RamBlock::defParamName [DEFPARAM_TYPE_COUNT] = {"operation_mode", 
	"mixed_port_feed_through_mode", "ram_block_type", "logical_ram_name", "init_file", 
	"init_file_restructured", "init_file_layout", "data_interleave_width_in_bits", 
	"data_interleave_offset_in_bits", "port_a_address_width", "port_a_logical_ram_depth", 
	"port_a_logical_ram_width", "port_a_data_in_clear", "port_a_address_clear", 
	"port_a_write_enable_clear", "port_a_byte_enable_clear", "port_a_data_out_clock", 
	"port_a_data_out_clear", "port_a_first_address", "port_a_last_address", 
	"port_a_first_bit_number", "port_a_data_width", 
	"port_b_address_width", "port_b_logical_ram_depth", "port_b_logical_ram_width", 
	"port_b_data_in_clock", "port_b_data_in_clear", "port_b_address_clock", 
	"port_b_address_clear", "port_b_read_enable_write_enable_clock", 
	"port_b_read_enable_write_enable_clear", "port_b_byte_enable_clock", 
	"port_b_byte_enable_clear", "port_b_data_out_clock", "port_b_data_out_clear", 
	"port_b_first_address", "port_b_last_address", "port_b_first_bit_number", 
	"port_b_data_width"};

CometMem * RamBlock::getCometMem(void) const
{
	return this->cometMem;
}

void RamBlock::setCometMem(CometMem &cometMem)
{
	this->cometMem = &cometMem;
}

string RamBlock::getName(void) const
{
	return this->name;
}

void RamBlock::addInport(string stdPortName, string portRealName)
{
	inports[stdPortName] = portRealName;
}

void RamBlock::addOutport(string stdPortName, string portRealName)
{
	outports[stdPortName] = portRealName;
}

map<string, string> & RamBlock::getInports(void)
{
	return inports;
}

map<string, string> & RamBlock::getOutports(void)
{
	return outports;
}

map<string, string> & RamBlock::getDefparams(void)
{
	return defparams;
}

void RamBlock::setDefparams(string stdDefparamName, string val)
{
	defparams[stdDefparamName] = val;
}

bool RamBlock::getFlagPortB(void) const
{
	return this->flagPortB;
}

void RamBlock::setFlagPortB(bool val)
{
	this->flagPortB = val;
}

void RamBlock::exchangeParam(string firstParam, string secondParam)
{
	if (defparams.count(firstParam) != 0 && defparams.count(secondParam) != 0)
	{
		string tmp = defparams[firstParam];

		defparams[firstParam] = defparams[secondParam];
		defparams[secondParam] = tmp;
	}
}

void RamBlock::modifyParam(string key, string val)
{
	if (defparams.count(key) != 0)
	{
		defparams[key] = val;
	}
}

void RamBlock::delInPort(string portName)
{
	if (inports.count(portName) != 0)
	{
		inports.erase(portName);
	}
}

string RamBlock::getPara(string key)
{
	string res = "";
	if (defparams.count(key) != 0)
	{
		res = defparams[key];
	}

	return res;
}
