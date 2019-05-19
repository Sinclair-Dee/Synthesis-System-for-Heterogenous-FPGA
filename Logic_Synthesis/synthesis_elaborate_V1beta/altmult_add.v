`timescale 1 ps / 1 ps
module altmult_add
(
	clock0,
	clock1,
	clock2,
	clock3,
	dataa,
	datab,

	aclr0,
	aclr1,
	aclr2,
	aclr3,
	ena0,
	ena1,
	ena2,
	ena3,
	accum_sload,
	addnsub1,
	addnsub1_round,
	addnsub3,
	addnsub3_round,
	chainin,
	chainout_round,
	chainout_saturate,
	mult01_round,
	mult01_saturation,
	mult23_round,
	mult23_saturation,
	output_round,
	output_saturate,
	rotate,
	scanina,
	scaninb,
	shift_right,
	signa,
	signb,
	sourcea,
	sourceb,
	zero_chainout,
	zero_loopback,

	scanouta,
	scanoutb,
	result,
	chainout_sat_overflow,
	mult0_is_saturated,
	mult1_is_saturated,
	mult2_is_saturated,
	mult3_is_saturated,
	overflow
);

parameter
	number_of_multipliers = 1,
	width_a = 1,
	width_b = 1,
	width_result = 2;

	input [width_a*number_of_multipliers-1:0] dataa;
	input [width_b*number_of_multipliers-1:0] datab;
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
	input accum_sload;
	input addnsub1;
	input addnsub1_round;
	input addnsub3;
	input addnsub3_round;
	input chainin;
	input chainout_round;
	input chainout_saturate;
	input mult01_round;
	input mult01_saturation;
	input mult23_round;
	input mult23_saturation;
	input output_round;
	input output_saturate;
	input rotate;
	input [width_a-1:0] scanina;
	input [width_b-1:0] scaninb;
	input shift_right;
	input signa;
	input signb;
	input [number_of_multipliers-1:0] sourcea;
	input [number_of_multipliers-1:0] sourceb;
	input zero_chainout;
	input zero_loopback;

	output [width_a-1:0] scanouta;
	output [width_b-1:0] scanoutb;
	output [width_result-1:0] result;
	output chainout_sat_overflow;
	output mult0_is_saturated;
	output mult1_is_saturated;
	output mult2_is_saturated;
	output mult3_is_saturated;
	output overflow;

assign mult_add(dataa,datab,clock0,clock1,clock2,clock3,aclr0,aclr1,aclr2,aclr3,ena0,ena1,ena2,ena3,signa,signb,scanouta,scanoutb,result);

endmodule