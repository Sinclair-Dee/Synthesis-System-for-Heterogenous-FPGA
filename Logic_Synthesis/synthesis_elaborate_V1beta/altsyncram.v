`timescale 1 ps / 1 ps
module altsyncram
(
	address_a,
	address_b,
	wren_a,
	wren_b,
	rden_a,
	rden_b,
	data_a,
	data_b,

	addressstall_a,
	addressstall_b,
	clock0,
	clock1,
	clocken0,
	clocken1,
	clocken2,
	clocken3,
	aclr0,
	aclr1,
	byteena_a,
	byteena_b,

	q_a,
	q_b,
	eccstatus
);

parameter
	////////////////////////////
	// port A parameters //
	////////////////////////////
	width_a = 1,
	widthad_a = 1,

	////////////////////////////
	// port B parameter //
	////////////////////////////
	width_b = 1,
	widthad_b = 1,
	
	////////////////////////////
	// peration mode //
	////////////////////////////
	operation_mode = "BIDIR_DUAL_PORT";

	input wren_a;
	input rden_a;
	input wren_b;
	input rden_b;
	input [width_a-1:0] data_a;
	input [width_b-1:0] data_b;
	input [widthad_a-1:0] address_a;
	input [widthad_b-1:0] address_b;
	input addressstall_a;
	input addressstall_b;
	input clock0;
	input clock1;
	input clocken0;
	input clocken1;
	input clocken2;
	input clocken3;
	input aclr0;
	input aclr1;
	input [3:0] byteena_a;
	input [3:0] byteena_b;

	output [width_a-1:0] q_a;
	output [width_b-1:0] q_b;
	output [2:0] eccstatus;

assign psk_mem(clocken0,clocken1,wren_a,aclr0,clock0,wren_b,aclr1,clock1,address_a,address_b,data_a,data_b,q_a,q_b,rden_a,rden_b);

endmodule