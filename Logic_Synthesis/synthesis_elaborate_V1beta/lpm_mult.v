`timescale 1 ps / 1 ps
module lpm_mult
(
	dataa,
	datab,
	clken,
	aclr,
	clock,
	sum,
	result
);

parameter
	lpm_widtha = 1,
	lpm_widthb = 1,
	lpm_widths = 1,
	lpm_widthp = 2;

	input [lpm_widtha-1:0] dataa;
	input [lpm_widthb-1:0] datab;
	input clken;
	input aclr;
	input clock;
	input [lpm_widths-1:0] sum;

	output [lpm_widthp-1:0] result;

assign mult(dataa,datab,clken,aclr,clock,sum,result);

endmodule