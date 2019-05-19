`timescale 1 ps / 1 ps
module altsquare
(
	data,
	clken,
	aclr,
	clock,
	result,
	sum
);

parameter
	lpm_widtha = 1,
	lpm_widthp = 2;

	input [lpm_widtha-1:0] data;
	input clken;
	input aclr;
	input clock;
	input sum;

	output [lpm_widthp-1:0] result;

assign square(data,clken,aclr,clock,result);

endmodule