#include "PLL.h"
#include <iostream>

using std::cout;
using std::endl;

PLL::PLL(Node * node, string name)
{
	this->name = name;
	this->pnode = node;
	this->cometPLL = NULL;
}


PLL::~PLL(void)
{
	//cout << this->name << " deconstruct..." << endl;
}


const int PLL::INPORT_TYPE_COUNT = 11;

const int PLL::OUTPORT_TYPE_COUNT = 12;

const string PLL::inportName [INPORT_TYPE_COUNT] = {"inclk", "clkswitch", "ena", 
	"areset", "pfdena", "fbin", "scanclk", "scanread", "scanwrite", "scandata", "testin"};

const string PLL::outportName [OUTPORT_TYPE_COUNT] = {"scandataout", "scandone", 
	"clk", "clkbad", "activeclock", "clkloss", "locked", "sclkout", "enable0", "enable1", 
	"testupout", "testdownout"};

const int PLL::DEFPARAM_TYPE_COUNT = 70;

const string PLL::defParamName [DEFPARAM_TYPE_COUNT] = {"operation_mode", 
	"pll_type", "qualify_conf_done", "valid_lock_multiplier", "invalid_lock_multiplier", 
	"compensate_clock", "inclk0_input_frequency", "inclk1_input_frequency", 
	"skip_vco", "primary_clock", "switch_over_on_lossclk", "switch_over_on_gated_lock", 
	"enable_switch_over_counter", "gate_lock_signal", "gate_lock_counter", 
	"switch_over_counter", "bandwidth_type", "bandwidth", "spread_frequency", 
	"down_spread", "clk0_multiply_by", "clk1_multiply_by", "clk2_multiply_by", 
	"clk3_multiply_by", "clk4_multiply_by", "clk5_multiply_by", "extclk0_multiply_by", 
	"extclk1_multiply_by", "extclk2_multiply_by", "extclk3_multiply_by", "clk0_divide_by", 
	"clk1_divide_by", "clk2_divide_by", "clk3_divide_by", "clk4_divide_by", 
	"clk5_divide_by", "extclk0_divide_by", "extclk1_divide_by", "extclk2_divide_by", 
	"extclk3_divide_by", "clk0_phase_shift", "clk1_phase_shift", "clk2_phase_shift", 
	"clk3_phase_shift", "clk4_phase_shift", "clk5_phase_shift", "extclk0_phase_shift", 
	"extclk1_phase_shift", "extclk2_phase_shift", "extclk3_phase_shift", 
	"clk0_time_delay", "clk1_time_delay", "clk2_time_delay", "clk3_time_delay", 
	"clk4_time_delay", "clk5_time_delay", "extclk0_time_delay", "extclk1_time_delay", 
	"extclk2_time_delay", "extclk3_time_delay", "clk0_duty_cycle", "clk1_duty_cycle", 
	"clk2_duty_cycle", "clk3_duty_cycle", "clk4_duty_cycle", "clk5_duty_cycle", 
	"extclk0_duty_cycle", "extclk1_duty_cycle", "extclk2_duty_cycle", "extclk3_duty_cycle"};

CometPLL * PLL::getCometPLL(void) const
{
	return this->cometPLL;
}

void PLL::setCometPLL(CometPLL &cometPLL)
{
	this->cometPLL = &cometPLL;
}

string PLL::getName(void) const
{
	return this->name;
}

void PLL::addInport(string stdPortName, string portRealName)
{
	inports[stdPortName] = portRealName;
}

void PLL::addOutport(string stdPortName, string portRealName)
{
	outports[stdPortName] = portRealName;
}

map<string, string> & PLL::getInports(void)
{
	return inports;
}

map<string, string> & PLL::getOutports(void)
{
	return outports;
}

map<string, string> & PLL::getDefparams(void)
{
	return defparams;
}

void PLL::setDefparams(string stdDefparamName, string val)
{
	defparams[stdDefparamName] = val;
}
