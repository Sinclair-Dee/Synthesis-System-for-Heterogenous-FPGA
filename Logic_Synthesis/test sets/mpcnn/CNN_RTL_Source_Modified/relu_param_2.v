`include "param_2.vh"
module relu_param_2 (

	clock,
	enable,
	reset,		 
	result,
	result_relu,
	count_sload
	);	

	//data width parameter		
parameter	ACCUM_DATA_WIDTH=`ACCUM_DATA_WIDTH;		
parameter	DATA_WIDTH=`DATA_WIDTH;	
parameter COUNT_SLOAD_BITWIDTH = `COUNT_SLOAD_BITWIDTH;	

	input clock;
	input enable;
	input reset;
	input [COUNT_SLOAD_BITWIDTH-1:0]count_sload;
	 
	input wire signed [ACCUM_DATA_WIDTH-1: 0] result;

	output wire signed [DATA_WIDTH-1: 0] result_relu;
	
	
	 
	reg signed [DATA_WIDTH-1: 0] result_jiewei ;

always @ (posedge clock)

	if(count_sload==2) 					 
		result_jiewei <=  {result[ACCUM_DATA_WIDTH-1],result[26:12]};					 

	
assign	result_relu=(result_jiewei[DATA_WIDTH-1])?16'b0:result_jiewei;
	
endmodule	
