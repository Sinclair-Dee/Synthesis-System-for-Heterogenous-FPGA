`timescale 1 ps / 1 ps
module altpll
(
	inclk,
	fbin,
	pllena,
	clkswitch,
	areset,
	pfdena,
	clkena,
	extclkena,
	scanclk,
	scanclkena,
	scanaclr,
	scanread,
	scanwrite,
	scandata,
	phasecounterselect,
	phaseupdown,
	phasestep,
	configupdate,
	fbmimicbidir,

	clk,
	extclk,
	clkbad,
	enable1,
	enable0,
	activeclock,
	clkloss,
	locked,
	scandataout,
	scandone,
	sclkout0,
	sclkout1,
	phasedone,
	vcooverrange,
	vcounderrange,
	fbout
);

	input [1:0] inclk;
	input fbin;
	input pllena;
	input clkswitch;
	input areset;
	input pfdena;
	input [5:0] clkena;
	input [3:0] extclkena;
	input scanclk;
	input scanclkena;
	input scanaclr;
	input scanread;
	input scanwrite;
	input scandata;
	input [3:0] phasecounterselect;
	input phaseupdown;
	input phasestep;
	input configupdate;

	inout fbmimicbidir;

	output [5:0] clk;
	output [3:0] extclk;
	output [1:0] clkbad;
	output enable1;
	output enable0;
	output activeclock;
	output clkloss;
	output locked;
	output scandataout;
	output scandone;
	output sclkout0;
	output sclkout1;
	output phasedone;
	output vcooverrange;
	output vcounderrange;
	output fbout;
	
comet_pll(inclk[0],pllena,areset,pfdena,clk[0],clk[1],extclk[0],locked);

endmodule
