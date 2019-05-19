`timescale 1 ps / 1 ps
module altmult_accum
(
	dataa,
	datab,
	accum_sload,
	clock0,
	clock1,
	clock2,
	clock3,
	aclr0,
	aclr1,
	aclr2,
	aclr3,
	ena0,
	ena1,
	ena2,
	ena3,
	accum_round,
	accum_saturation,
	accum_sload_upper_data,
	addnsub,
	mult_round,
	mult_saturation,
	scanina,
	scaninb,
	signa,
	signb,
	sourcea,
	sourceb,

	scanouta,
	scanoutb,
	result,
	overflow,
	accum_is_saturated,
	mult_is_saturated
);

parameter
	width_a = 1,
	width_b = 1,
	width_result = 2;

	input [width_a-1:0] dataa;
	input [width_b-1:0] datab;
	input accum_sload;
	input clock0;
	input clock1;
	input clock2;
	input clock3;
	input aclr0;
	input aclr1;
	input aclr2;
	input aclr3;
	input ena0;
	input ena1;
	input ena2;
	input ena3;
	input accum_round;
	input accum_saturation;
	input accum_sload_upper_data;
	input addnsub;
	input mult_round;
	input mult_saturation;
	input [width_a-1:0] scanina;
	input [width_b-1:0] scaninb;
	input signa;
	input signb;
	input sourcea;
	input sourceb;

	output [width_a-1:0] scanouta;
	output [width_b-1:0] scanoutb;
	output [width_result-1:0] result;
	output overflow;
	output accum_is_saturated;
	output mult_is_saturated;

assign mult_accum(dataa,datab,accum_sload,clock0,clock1,clock2,clock3,aclr0,aclr1,aclr2,aclr3,ena0,ena1,ena2,ena3,scanouta,scanoutb,result,overflow);

endmodule