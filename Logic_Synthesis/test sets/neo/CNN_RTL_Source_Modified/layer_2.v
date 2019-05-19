`include "param_2.vh"
module layer_2 (
// Global signals
	clock,
	enable,
	reset,
	pool_done,
//	communication with former layer
	in_feature_q_a_all,
	in_feature_q_b_all,
	
	in_feature_addra,
	in_feature_addrb,
	
	 in_feature_rden_a ,
	 in_feature_rden_b ,
	 in_feature_wren_a ,
	 in_feature_wren_b ,


	 
//communication with next layer
	 
	address_a_t_out,
	address_b_t_out,

	rden_a_out	,
	rden_b_out	,
	wren_a_out	,
	wren_b_out	,

//use
	address_a_t_use_out,
	address_b_t_use_out,

	rden_a_use_out	,
	rden_b_use_out	,
	wren_a_use_out	,
	wren_b_use_out	,



	q_a_all_out,
	q_b_all_out

		


	);	

//ADD 20170906
parameter IFMAP_PAR= 2;
parameter NUM_DSP =32;
//******* ADD for resource parameter

parameter INPUT_NUM_MEM=4;	


parameter NUM_MULT=16;

	//data width parameter		
parameter	ACCUM_DATA_WIDTH=44;		
parameter	DATA_WIDTH=16;		
// number of input output feature map

	

//addr width
   parameter	IN_FEATURE_ADDR_WIDTH=9;		
parameter	WEIGHT_ADDR_WIDTH=10;		
parameter	POOL_ADDR_WIDTH=9;		

	
parameter	COUNT_SLOAD_BITWIDTH=5;		
			
 
 //global signals
	input wire clock;
	input wire enable;
	input wire reset;

input	wire [DATA_WIDTH*INPUT_NUM_MEM-1: 0] in_feature_q_a_all;
input	wire [DATA_WIDTH*INPUT_NUM_MEM-1: 0] in_feature_q_b_all;
	
output	wire [IN_FEATURE_ADDR_WIDTH - 1 : 0] in_feature_addra;
output	wire [IN_FEATURE_ADDR_WIDTH - 1 : 0] in_feature_addrb;
	
output	wire   in_feature_rden_a ;
output	wire   in_feature_rden_b ;
output	wire   in_feature_wren_a ;
output	wire   in_feature_wren_b ;

output	wire [DATA_WIDTH*NUM_MULT-1: 0] q_a_all_out;
output	wire [DATA_WIDTH*NUM_MULT-1: 0] q_b_all_out;
	//**************
//**********************
	wire [COUNT_SLOAD_BITWIDTH-1:0] count_sload;
	
	
//wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use[NUM_MULT-1:0] ;展开-->16
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_0;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_1;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_2;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_3;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_4;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_5;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_6;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_7;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_8;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_9;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_10;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_11;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_12;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_13;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_14;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_15;
//wire signed [ACCUM_DATA_WIDTH-1:0]  result[ NUM_DSP -1:0] ;展开-->32
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_0;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_1;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_2;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_3;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_4;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_5;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_6;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_7;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_8;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_9;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_10;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_11;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_12;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_13;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_14;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_15;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_16;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_17;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_18;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_19;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_20;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_21;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_22;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_23;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_24;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_25;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_26;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_27;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_28;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_29;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_30;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_31;
	

//	
	output wire pool_done;
	wire pool_start;
//wire [DATA_WIDTH-1: 0] result_after_relu[NUM_MULT-1:0];	展开-->16
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_0;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_1;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_2;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_3;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_4;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_5;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_6;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_7;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_8;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_9;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_10;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_11;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_12;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_13;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_14;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_15;


	//internal signals  weight_feature
	wire weight_rden_a ;
	wire weight_rden_b ;
   wire weight_wren_a ;
   wire weight_wren_b ;

	//weight ram 
//wire [DATA_WIDTH-1:0] weight_q_a[NUM_DSP-1:0];展开-->32
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_0;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_1;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_2;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_3;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_4;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_5;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_6;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_7;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_8;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_9;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_10;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_11;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_12;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_13;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_14;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_15;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_16;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_17;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_18;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_19;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_20;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_21;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_22;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_23;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_24;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_25;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_26;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_27;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_28;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_29;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_30;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_31;
//wire [DATA_WIDTH-1:0] weight_q_b[NUM_DSP-1:0];展开-->32
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_0;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_1;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_2;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_3;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_4;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_5;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_6;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_7;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_8;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_9;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_10;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_11;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_12;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_13;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_14;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_15;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_16;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_17;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_18;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_19;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_20;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_21;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_22;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_23;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_24;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_25;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_26;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_27;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_28;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_29;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_30;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_31;
 
//reg  [DATA_WIDTH-1: 0] in_feature_data_a [INPUT_NUM_MEM-1:0];展开-->4
 	reg  [DATA_WIDTH-1: 0] in_feature_data_a_unroll_0;
 	reg  [DATA_WIDTH-1: 0] in_feature_data_a_unroll_1;
 	reg  [DATA_WIDTH-1: 0] in_feature_data_a_unroll_2;
 	reg  [DATA_WIDTH-1: 0] in_feature_data_a_unroll_3;
//reg  [DATA_WIDTH-1: 0] in_feature_data_b [INPUT_NUM_MEM-1:0];展开-->4
	reg  [DATA_WIDTH-1: 0] in_feature_data_b_unroll_0;
	reg  [DATA_WIDTH-1: 0] in_feature_data_b_unroll_1;
	reg  [DATA_WIDTH-1: 0] in_feature_data_b_unroll_2;
	reg  [DATA_WIDTH-1: 0] in_feature_data_b_unroll_3;
 
//reg  [DATA_WIDTH-1: 0] weight_data_a[ NUM_DSP-1:0] ;展开-->32
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_0;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_1;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_2;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_3;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_4;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_5;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_6;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_7;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_8;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_9;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_10;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_11;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_12;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_13;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_14;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_15;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_16;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_17;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_18;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_19;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_20;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_21;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_22;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_23;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_24;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_25;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_26;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_27;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_28;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_29;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_30;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_31;
//reg  [DATA_WIDTH-1: 0] weight_data_b[ NUM_DSP-1:0] ;展开-->32
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_0;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_1;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_2;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_3;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_4;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_5;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_6;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_7;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_8;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_9;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_10;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_11;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_12;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_13;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_14;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_15;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_16;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_17;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_18;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_19;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_20;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_21;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_22;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_23;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_24;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_25;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_26;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_27;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_28;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_29;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_30;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_31;
 

	
	wire [WEIGHT_ADDR_WIDTH-1:0] weight_addra;
	wire [WEIGHT_ADDR_WIDTH-1:0] weight_addrb;
 
	//in feature ram
//wire [DATA_WIDTH-1: 0] in_feature_q_a[INPUT_NUM_MEM-1:0];展开-->4
   wire [DATA_WIDTH-1: 0] in_feature_q_a_unroll_0;
   wire [DATA_WIDTH-1: 0] in_feature_q_a_unroll_1;
   wire [DATA_WIDTH-1: 0] in_feature_q_a_unroll_2;
   wire [DATA_WIDTH-1: 0] in_feature_q_a_unroll_3;
//wire [DATA_WIDTH-1: 0] in_feature_q_b[INPUT_NUM_MEM-1:0];展开-->4
	wire [DATA_WIDTH-1: 0] in_feature_q_b_unroll_0;
	wire [DATA_WIDTH-1: 0] in_feature_q_b_unroll_1;
	wire [DATA_WIDTH-1: 0] in_feature_q_b_unroll_2;
	wire [DATA_WIDTH-1: 0] in_feature_q_b_unroll_3;
	
//wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use [NUM_DSP -1 :0];展开-->32
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_0;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_1;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_2;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_3;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_4;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_5;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_6;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_7;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_8;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_9;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_10;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_11;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_12;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_13;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_14;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_15;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_16;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_17;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_18;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_19;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_20;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_21;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_22;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_23;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_24;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_25;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_26;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_27;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_28;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_29;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_30;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_31;
//wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use [NUM_DSP -1 :0];	展开-->32
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_0;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_1;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_2;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_3;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_4;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_5;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_6;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_7;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_8;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_9;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_10;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_11;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_12;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_13;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_14;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_15;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_16;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_17;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_18;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_19;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_20;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_21;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_22;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_23;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_24;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_25;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_26;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_27;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_28;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_29;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_30;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_31;
	


	wire enable_addrger ;
	wire accum_sload;
	wire enable_weightaddrger;
	wire enable_mult;
	wire clear_mult;
//*****out reda write address
output	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_out;
output	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_out;

output	wire	 rden_a_out;
output	wire	 rden_b_out;
output	wire	 wren_a_out;
output	wire	 wren_b_out;

//use
input	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_out;
input	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_out;

input	wire	 rden_a_use_out;
input	wire	 rden_b_use_out;
input	wire	 wren_a_use_out;
input	wire	 wren_b_use_out;

//*********	
 	wire  [DATA_WIDTH*IFMAP_PAR-1: 0] in_feature_q_a_mux_all;
 	wire  [DATA_WIDTH*IFMAP_PAR-1: 0] in_feature_q_b_mux_all;	


//***********
//wire	[POOL_ADDR_WIDTH-1:0] address_a_t[NUM_MULT-1:0];展开-->16
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_0;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_1;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_2;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_3;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_4;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_5;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_6;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_7;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_8;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_9;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_10;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_11;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_12;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_13;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_14;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_15;
//wire	[POOL_ADDR_WIDTH-1:0] address_b_t[NUM_MULT-1:0];展开-->16
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_0;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_1;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_2;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_3;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_4;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_5;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_6;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_7;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_8;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_9;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_10;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_11;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_12;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_13;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_14;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_15;
//wire	[DATA_WIDTH-1: 0] data_a[NUM_MULT-1:0];展开-->16
	wire	[DATA_WIDTH-1: 0] data_a_unroll_0;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_1;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_2;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_3;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_4;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_5;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_6;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_7;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_8;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_9;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_10;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_11;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_12;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_13;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_14;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_15;
//wire	[DATA_WIDTH-1: 0] data_b[NUM_MULT-1:0];展开-->16
	wire	[DATA_WIDTH-1: 0] data_b_unroll_0;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_1;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_2;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_3;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_4;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_5;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_6;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_7;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_8;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_9;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_10;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_11;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_12;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_13;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_14;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_15;
//wire	 rden_a	[NUM_MULT-1:0];展开-->16
	wire	 rden_a_unroll_0;
	wire	 rden_a_unroll_1;
	wire	 rden_a_unroll_2;
	wire	 rden_a_unroll_3;
	wire	 rden_a_unroll_4;
	wire	 rden_a_unroll_5;
	wire	 rden_a_unroll_6;
	wire	 rden_a_unroll_7;
	wire	 rden_a_unroll_8;
	wire	 rden_a_unroll_9;
	wire	 rden_a_unroll_10;
	wire	 rden_a_unroll_11;
	wire	 rden_a_unroll_12;
	wire	 rden_a_unroll_13;
	wire	 rden_a_unroll_14;
	wire	 rden_a_unroll_15;
//wire	 rden_b	[NUM_MULT-1:0];展开-->16
	wire	 rden_b_unroll_0;
	wire	 rden_b_unroll_1;
	wire	 rden_b_unroll_2;
	wire	 rden_b_unroll_3;
	wire	 rden_b_unroll_4;
	wire	 rden_b_unroll_5;
	wire	 rden_b_unroll_6;
	wire	 rden_b_unroll_7;
	wire	 rden_b_unroll_8;
	wire	 rden_b_unroll_9;
	wire	 rden_b_unroll_10;
	wire	 rden_b_unroll_11;
	wire	 rden_b_unroll_12;
	wire	 rden_b_unroll_13;
	wire	 rden_b_unroll_14;
	wire	 rden_b_unroll_15;
//wire	 wren_a	[NUM_MULT-1:0];展开-->16
	wire	 wren_a_unroll_0;
	wire	 wren_a_unroll_1;
	wire	 wren_a_unroll_2;
	wire	 wren_a_unroll_3;
	wire	 wren_a_unroll_4;
	wire	 wren_a_unroll_5;
	wire	 wren_a_unroll_6;
	wire	 wren_a_unroll_7;
	wire	 wren_a_unroll_8;
	wire	 wren_a_unroll_9;
	wire	 wren_a_unroll_10;
	wire	 wren_a_unroll_11;
	wire	 wren_a_unroll_12;
	wire	 wren_a_unroll_13;
	wire	 wren_a_unroll_14;
	wire	 wren_a_unroll_15;
//wire	 wren_b	[NUM_MULT-1:0];展开-->16
	wire	 wren_b_unroll_0;
	wire	 wren_b_unroll_1;
	wire	 wren_b_unroll_2;
	wire	 wren_b_unroll_3;
	wire	 wren_b_unroll_4;
	wire	 wren_b_unroll_5;
	wire	 wren_b_unroll_6;
	wire	 wren_b_unroll_7;
	wire	 wren_b_unroll_8;
	wire	 wren_b_unroll_9;
	wire	 wren_b_unroll_10;
	wire	 wren_b_unroll_11;
	wire	 wren_b_unroll_12;
	wire	 wren_b_unroll_13;
	wire	 wren_b_unroll_14;
	wire	 wren_b_unroll_15;
//wire	[DATA_WIDTH-1: 0] q_a[NUM_MULT-1:0];展开-->16
	wire	[DATA_WIDTH-1: 0] q_a_unroll_0;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_1;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_2;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_3;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_4;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_5;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_6;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_7;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_8;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_9;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_10;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_11;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_12;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_13;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_14;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_15;
//wire	[DATA_WIDTH-1: 0] q_b[NUM_MULT-1:0];展开-->16
	wire	[DATA_WIDTH-1: 0] q_b_unroll_0;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_1;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_2;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_3;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_4;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_5;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_6;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_7;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_8;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_9;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_10;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_11;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_12;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_13;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_14;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_15;
//use
//wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use[NUM_MULT-1:0];展开-->16
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_0;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_1;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_2;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_3;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_4;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_5;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_6;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_7;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_8;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_9;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_10;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_11;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_12;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_13;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_14;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_15;
//wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use[NUM_MULT-1:0];展开-->16
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_0;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_1;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_2;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_3;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_4;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_5;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_6;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_7;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_8;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_9;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_10;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_11;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_12;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_13;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_14;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_15;

//wire	 rden_a_use	[NUM_MULT-1:0];展开-->16
	wire	 rden_a_use_unroll_0;
	wire	 rden_a_use_unroll_1;
	wire	 rden_a_use_unroll_2;
	wire	 rden_a_use_unroll_3;
	wire	 rden_a_use_unroll_4;
	wire	 rden_a_use_unroll_5;
	wire	 rden_a_use_unroll_6;
	wire	 rden_a_use_unroll_7;
	wire	 rden_a_use_unroll_8;
	wire	 rden_a_use_unroll_9;
	wire	 rden_a_use_unroll_10;
	wire	 rden_a_use_unroll_11;
	wire	 rden_a_use_unroll_12;
	wire	 rden_a_use_unroll_13;
	wire	 rden_a_use_unroll_14;
	wire	 rden_a_use_unroll_15;
//wire	 rden_b_use	[NUM_MULT-1:0];展开-->16
	wire	 rden_b_use_unroll_0;
	wire	 rden_b_use_unroll_1;
	wire	 rden_b_use_unroll_2;
	wire	 rden_b_use_unroll_3;
	wire	 rden_b_use_unroll_4;
	wire	 rden_b_use_unroll_5;
	wire	 rden_b_use_unroll_6;
	wire	 rden_b_use_unroll_7;
	wire	 rden_b_use_unroll_8;
	wire	 rden_b_use_unroll_9;
	wire	 rden_b_use_unroll_10;
	wire	 rden_b_use_unroll_11;
	wire	 rden_b_use_unroll_12;
	wire	 rden_b_use_unroll_13;
	wire	 rden_b_use_unroll_14;
	wire	 rden_b_use_unroll_15;
//wire	 wren_a_use	[NUM_MULT-1:0];展开-->16
	wire	 wren_a_use_unroll_0;
	wire	 wren_a_use_unroll_1;
	wire	 wren_a_use_unroll_2;
	wire	 wren_a_use_unroll_3;
	wire	 wren_a_use_unroll_4;
	wire	 wren_a_use_unroll_5;
	wire	 wren_a_use_unroll_6;
	wire	 wren_a_use_unroll_7;
	wire	 wren_a_use_unroll_8;
	wire	 wren_a_use_unroll_9;
	wire	 wren_a_use_unroll_10;
	wire	 wren_a_use_unroll_11;
	wire	 wren_a_use_unroll_12;
	wire	 wren_a_use_unroll_13;
	wire	 wren_a_use_unroll_14;
	wire	 wren_a_use_unroll_15;
//wire	 wren_b_use	[NUM_MULT-1:0];展开-->16
	wire	 wren_b_use_unroll_0;
	wire	 wren_b_use_unroll_1;
	wire	 wren_b_use_unroll_2;
	wire	 wren_b_use_unroll_3;
	wire	 wren_b_use_unroll_4;
	wire	 wren_b_use_unroll_5;
	wire	 wren_b_use_unroll_6;
	wire	 wren_b_use_unroll_7;
	wire	 wren_b_use_unroll_8;
	wire	 wren_b_use_unroll_9;
	wire	 wren_b_use_unroll_10;
	wire	 wren_b_use_unroll_11;
	wire	 wren_b_use_unroll_12;
	wire	 wren_b_use_unroll_13;
	wire	 wren_b_use_unroll_14;
	wire	 wren_b_use_unroll_15;
//**************************************** *************in feature map*******************
	wire [DATA_WIDTH*NUM_MULT-1:0]q_b_all;



assign address_a_t_out=address_a_t_unroll_0;
assign address_b_t_out=address_b_t_unroll_0;
assign rden_a_out=rden_a_unroll_0;
assign rden_b_out=rden_b_unroll_0;
assign wren_a_out=wren_a_unroll_0;
assign wren_b_out=wren_b_unroll_0;



	
//generate for开始------------------------------->
//wire_ur:16
//第0次展开：0/16
	
assign address_a_t_use_unroll_0=address_a_t_use_out;
assign address_b_t_use_unroll_0=address_b_t_use_out;
assign rden_a_use_unroll_0=rden_a_use_out;
assign rden_b_use_unroll_0=rden_b_use_out;
assign wren_a_use_unroll_0=wren_a_use_out;
assign wren_b_use_unroll_0=wren_b_use_out;	
		
//第1次展开：1/16
	
assign address_a_t_use_unroll_1=address_a_t_use_out;
assign address_b_t_use_unroll_1=address_b_t_use_out;
assign rden_a_use_unroll_1=rden_a_use_out;
assign rden_b_use_unroll_1=rden_b_use_out;
assign wren_a_use_unroll_1=wren_a_use_out;
assign wren_b_use_unroll_1=wren_b_use_out;	
		
//第2次展开：2/16
	
assign address_a_t_use_unroll_2=address_a_t_use_out;
assign address_b_t_use_unroll_2=address_b_t_use_out;
assign rden_a_use_unroll_2=rden_a_use_out;
assign rden_b_use_unroll_2=rden_b_use_out;
assign wren_a_use_unroll_2=wren_a_use_out;
assign wren_b_use_unroll_2=wren_b_use_out;	
		
//第3次展开：3/16
	
assign address_a_t_use_unroll_3=address_a_t_use_out;
assign address_b_t_use_unroll_3=address_b_t_use_out;
assign rden_a_use_unroll_3=rden_a_use_out;
assign rden_b_use_unroll_3=rden_b_use_out;
assign wren_a_use_unroll_3=wren_a_use_out;
assign wren_b_use_unroll_3=wren_b_use_out;	
		
//第4次展开：4/16
	
assign address_a_t_use_unroll_4=address_a_t_use_out;
assign address_b_t_use_unroll_4=address_b_t_use_out;
assign rden_a_use_unroll_4=rden_a_use_out;
assign rden_b_use_unroll_4=rden_b_use_out;
assign wren_a_use_unroll_4=wren_a_use_out;
assign wren_b_use_unroll_4=wren_b_use_out;	
		
//第5次展开：5/16
	
assign address_a_t_use_unroll_5=address_a_t_use_out;
assign address_b_t_use_unroll_5=address_b_t_use_out;
assign rden_a_use_unroll_5=rden_a_use_out;
assign rden_b_use_unroll_5=rden_b_use_out;
assign wren_a_use_unroll_5=wren_a_use_out;
assign wren_b_use_unroll_5=wren_b_use_out;	
		
//第6次展开：6/16
	
assign address_a_t_use_unroll_6=address_a_t_use_out;
assign address_b_t_use_unroll_6=address_b_t_use_out;
assign rden_a_use_unroll_6=rden_a_use_out;
assign rden_b_use_unroll_6=rden_b_use_out;
assign wren_a_use_unroll_6=wren_a_use_out;
assign wren_b_use_unroll_6=wren_b_use_out;	
		
//第7次展开：7/16
	
assign address_a_t_use_unroll_7=address_a_t_use_out;
assign address_b_t_use_unroll_7=address_b_t_use_out;
assign rden_a_use_unroll_7=rden_a_use_out;
assign rden_b_use_unroll_7=rden_b_use_out;
assign wren_a_use_unroll_7=wren_a_use_out;
assign wren_b_use_unroll_7=wren_b_use_out;	
		
//第8次展开：8/16
	
assign address_a_t_use_unroll_8=address_a_t_use_out;
assign address_b_t_use_unroll_8=address_b_t_use_out;
assign rden_a_use_unroll_8=rden_a_use_out;
assign rden_b_use_unroll_8=rden_b_use_out;
assign wren_a_use_unroll_8=wren_a_use_out;
assign wren_b_use_unroll_8=wren_b_use_out;	
		
//第9次展开：9/16
	
assign address_a_t_use_unroll_9=address_a_t_use_out;
assign address_b_t_use_unroll_9=address_b_t_use_out;
assign rden_a_use_unroll_9=rden_a_use_out;
assign rden_b_use_unroll_9=rden_b_use_out;
assign wren_a_use_unroll_9=wren_a_use_out;
assign wren_b_use_unroll_9=wren_b_use_out;	
		
//第10次展开：10/16
	
assign address_a_t_use_unroll_10=address_a_t_use_out;
assign address_b_t_use_unroll_10=address_b_t_use_out;
assign rden_a_use_unroll_10=rden_a_use_out;
assign rden_b_use_unroll_10=rden_b_use_out;
assign wren_a_use_unroll_10=wren_a_use_out;
assign wren_b_use_unroll_10=wren_b_use_out;	
		
//第11次展开：11/16
	
assign address_a_t_use_unroll_11=address_a_t_use_out;
assign address_b_t_use_unroll_11=address_b_t_use_out;
assign rden_a_use_unroll_11=rden_a_use_out;
assign rden_b_use_unroll_11=rden_b_use_out;
assign wren_a_use_unroll_11=wren_a_use_out;
assign wren_b_use_unroll_11=wren_b_use_out;	
		
//第12次展开：12/16
	
assign address_a_t_use_unroll_12=address_a_t_use_out;
assign address_b_t_use_unroll_12=address_b_t_use_out;
assign rden_a_use_unroll_12=rden_a_use_out;
assign rden_b_use_unroll_12=rden_b_use_out;
assign wren_a_use_unroll_12=wren_a_use_out;
assign wren_b_use_unroll_12=wren_b_use_out;	
		
//第13次展开：13/16
	
assign address_a_t_use_unroll_13=address_a_t_use_out;
assign address_b_t_use_unroll_13=address_b_t_use_out;
assign rden_a_use_unroll_13=rden_a_use_out;
assign rden_b_use_unroll_13=rden_b_use_out;
assign wren_a_use_unroll_13=wren_a_use_out;
assign wren_b_use_unroll_13=wren_b_use_out;	
		
//第14次展开：14/16
	
assign address_a_t_use_unroll_14=address_a_t_use_out;
assign address_b_t_use_unroll_14=address_b_t_use_out;
assign rden_a_use_unroll_14=rden_a_use_out;
assign rden_b_use_unroll_14=rden_b_use_out;
assign wren_a_use_unroll_14=wren_a_use_out;
assign wren_b_use_unroll_14=wren_b_use_out;	
		
//第15次展开：15/16
	
assign address_a_t_use_unroll_15=address_a_t_use_out;
assign address_b_t_use_unroll_15=address_b_t_use_out;
assign rden_a_use_unroll_15=rden_a_use_out;
assign rden_b_use_unroll_15=rden_b_use_out;
assign wren_a_use_unroll_15=wren_a_use_out;
assign wren_b_use_unroll_15=wren_b_use_out;	
		
	
//**********************************************	
	
	


control_param_2  control_param_inst(

	.clock(clock),
	.enable(enable),
	.reset(reset),	
   
	.enable_addrger(enable_addrger),
	.enable_weightaddrger(enable_weightaddrger),
	//in feature
	.in_feature_rden_a(in_feature_rden_a),
	.in_feature_rden_b(in_feature_rden_b),
	.in_feature_wren_a(in_feature_wren_a),
	.in_feature_wren_b(in_feature_wren_b),
	//mult
	.accum_sload(accum_sload),
	.enable_mult(enable_mult),
	.clear_mult(clear_mult),
	// weight feature	
	.weight_rden_a(weight_rden_a) ,
   .weight_rden_b(weight_rden_b) ,
	.weight_wren_a(weight_wren_a) ,
	.weight_wren_b(weight_wren_b) ,
	.in_feature_q_a_mux_all(in_feature_q_a_mux_all) ,
   .in_feature_q_b_mux_all(in_feature_q_b_mux_all) ,
	
   .in_feature_q_a_all(in_feature_q_a_all),
	.in_feature_q_b_all(in_feature_q_b_all),
	
	.start(pool_start),
	.conv_done(pool_done),
	.count_sload(count_sload)

	  
		);

addrgener_param_2  addrgener_param(
 	 .clk(clock),
    .enable(enable_addrger),
	 .reset(reset),	
	 .addra_temp(in_feature_addra),
	 .addrb_temp(in_feature_addrb)

 );
weight_addrgener_param_2 weight_addrgener_param_inst(
	.clk(clock),
	.enable(enable_weightaddrger),
	.reset(reset),	
	.addra(weight_addra),
	.addrb(weight_addrb)
 );
 
//generate for开始------------------------------->
//num:32
//第0次展开：0/32
  
  assign   in_feature_q_a_dsp_use_unroll_0 = in_feature_q_a_mux_all[DATA_WIDTH* (0%IFMAP_PAR+1)-1  : DATA_WIDTH* (0%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_0 = in_feature_q_b_mux_all[DATA_WIDTH* (0%IFMAP_PAR+1)-1  : DATA_WIDTH* (0%IFMAP_PAR)];
//第1次展开：1/32
  
  assign   in_feature_q_a_dsp_use_unroll_1 = in_feature_q_a_mux_all[DATA_WIDTH* (1%IFMAP_PAR+1)-1  : DATA_WIDTH* (1%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_1 = in_feature_q_b_mux_all[DATA_WIDTH* (1%IFMAP_PAR+1)-1  : DATA_WIDTH* (1%IFMAP_PAR)];
//第2次展开：2/32
  
  assign   in_feature_q_a_dsp_use_unroll_2 = in_feature_q_a_mux_all[DATA_WIDTH* (2%IFMAP_PAR+1)-1  : DATA_WIDTH* (2%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_2 = in_feature_q_b_mux_all[DATA_WIDTH* (2%IFMAP_PAR+1)-1  : DATA_WIDTH* (2%IFMAP_PAR)];
//第3次展开：3/32
  
  assign   in_feature_q_a_dsp_use_unroll_3 = in_feature_q_a_mux_all[DATA_WIDTH* (3%IFMAP_PAR+1)-1  : DATA_WIDTH* (3%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_3 = in_feature_q_b_mux_all[DATA_WIDTH* (3%IFMAP_PAR+1)-1  : DATA_WIDTH* (3%IFMAP_PAR)];
//第4次展开：4/32
  
  assign   in_feature_q_a_dsp_use_unroll_4 = in_feature_q_a_mux_all[DATA_WIDTH* (4%IFMAP_PAR+1)-1  : DATA_WIDTH* (4%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_4 = in_feature_q_b_mux_all[DATA_WIDTH* (4%IFMAP_PAR+1)-1  : DATA_WIDTH* (4%IFMAP_PAR)];
//第5次展开：5/32
  
  assign   in_feature_q_a_dsp_use_unroll_5 = in_feature_q_a_mux_all[DATA_WIDTH* (5%IFMAP_PAR+1)-1  : DATA_WIDTH* (5%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_5 = in_feature_q_b_mux_all[DATA_WIDTH* (5%IFMAP_PAR+1)-1  : DATA_WIDTH* (5%IFMAP_PAR)];
//第6次展开：6/32
  
  assign   in_feature_q_a_dsp_use_unroll_6 = in_feature_q_a_mux_all[DATA_WIDTH* (6%IFMAP_PAR+1)-1  : DATA_WIDTH* (6%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_6 = in_feature_q_b_mux_all[DATA_WIDTH* (6%IFMAP_PAR+1)-1  : DATA_WIDTH* (6%IFMAP_PAR)];
//第7次展开：7/32
  
  assign   in_feature_q_a_dsp_use_unroll_7 = in_feature_q_a_mux_all[DATA_WIDTH* (7%IFMAP_PAR+1)-1  : DATA_WIDTH* (7%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_7 = in_feature_q_b_mux_all[DATA_WIDTH* (7%IFMAP_PAR+1)-1  : DATA_WIDTH* (7%IFMAP_PAR)];
//第8次展开：8/32
  
  assign   in_feature_q_a_dsp_use_unroll_8 = in_feature_q_a_mux_all[DATA_WIDTH* (8%IFMAP_PAR+1)-1  : DATA_WIDTH* (8%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_8 = in_feature_q_b_mux_all[DATA_WIDTH* (8%IFMAP_PAR+1)-1  : DATA_WIDTH* (8%IFMAP_PAR)];
//第9次展开：9/32
  
  assign   in_feature_q_a_dsp_use_unroll_9 = in_feature_q_a_mux_all[DATA_WIDTH* (9%IFMAP_PAR+1)-1  : DATA_WIDTH* (9%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_9 = in_feature_q_b_mux_all[DATA_WIDTH* (9%IFMAP_PAR+1)-1  : DATA_WIDTH* (9%IFMAP_PAR)];
//第10次展开：10/32
  
  assign   in_feature_q_a_dsp_use_unroll_10 = in_feature_q_a_mux_all[DATA_WIDTH* (10%IFMAP_PAR+1)-1  : DATA_WIDTH* (10%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_10 = in_feature_q_b_mux_all[DATA_WIDTH* (10%IFMAP_PAR+1)-1  : DATA_WIDTH* (10%IFMAP_PAR)];
//第11次展开：11/32
  
  assign   in_feature_q_a_dsp_use_unroll_11 = in_feature_q_a_mux_all[DATA_WIDTH* (11%IFMAP_PAR+1)-1  : DATA_WIDTH* (11%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_11 = in_feature_q_b_mux_all[DATA_WIDTH* (11%IFMAP_PAR+1)-1  : DATA_WIDTH* (11%IFMAP_PAR)];
//第12次展开：12/32
  
  assign   in_feature_q_a_dsp_use_unroll_12 = in_feature_q_a_mux_all[DATA_WIDTH* (12%IFMAP_PAR+1)-1  : DATA_WIDTH* (12%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_12 = in_feature_q_b_mux_all[DATA_WIDTH* (12%IFMAP_PAR+1)-1  : DATA_WIDTH* (12%IFMAP_PAR)];
//第13次展开：13/32
  
  assign   in_feature_q_a_dsp_use_unroll_13 = in_feature_q_a_mux_all[DATA_WIDTH* (13%IFMAP_PAR+1)-1  : DATA_WIDTH* (13%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_13 = in_feature_q_b_mux_all[DATA_WIDTH* (13%IFMAP_PAR+1)-1  : DATA_WIDTH* (13%IFMAP_PAR)];
//第14次展开：14/32
  
  assign   in_feature_q_a_dsp_use_unroll_14 = in_feature_q_a_mux_all[DATA_WIDTH* (14%IFMAP_PAR+1)-1  : DATA_WIDTH* (14%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_14 = in_feature_q_b_mux_all[DATA_WIDTH* (14%IFMAP_PAR+1)-1  : DATA_WIDTH* (14%IFMAP_PAR)];
//第15次展开：15/32
  
  assign   in_feature_q_a_dsp_use_unroll_15 = in_feature_q_a_mux_all[DATA_WIDTH* (15%IFMAP_PAR+1)-1  : DATA_WIDTH* (15%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_15 = in_feature_q_b_mux_all[DATA_WIDTH* (15%IFMAP_PAR+1)-1  : DATA_WIDTH* (15%IFMAP_PAR)];
//第16次展开：16/32
  
  assign   in_feature_q_a_dsp_use_unroll_16 = in_feature_q_a_mux_all[DATA_WIDTH* (16%IFMAP_PAR+1)-1  : DATA_WIDTH* (16%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_16 = in_feature_q_b_mux_all[DATA_WIDTH* (16%IFMAP_PAR+1)-1  : DATA_WIDTH* (16%IFMAP_PAR)];
//第17次展开：17/32
  
  assign   in_feature_q_a_dsp_use_unroll_17 = in_feature_q_a_mux_all[DATA_WIDTH* (17%IFMAP_PAR+1)-1  : DATA_WIDTH* (17%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_17 = in_feature_q_b_mux_all[DATA_WIDTH* (17%IFMAP_PAR+1)-1  : DATA_WIDTH* (17%IFMAP_PAR)];
//第18次展开：18/32
  
  assign   in_feature_q_a_dsp_use_unroll_18 = in_feature_q_a_mux_all[DATA_WIDTH* (18%IFMAP_PAR+1)-1  : DATA_WIDTH* (18%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_18 = in_feature_q_b_mux_all[DATA_WIDTH* (18%IFMAP_PAR+1)-1  : DATA_WIDTH* (18%IFMAP_PAR)];
//第19次展开：19/32
  
  assign   in_feature_q_a_dsp_use_unroll_19 = in_feature_q_a_mux_all[DATA_WIDTH* (19%IFMAP_PAR+1)-1  : DATA_WIDTH* (19%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_19 = in_feature_q_b_mux_all[DATA_WIDTH* (19%IFMAP_PAR+1)-1  : DATA_WIDTH* (19%IFMAP_PAR)];
//第20次展开：20/32
  
  assign   in_feature_q_a_dsp_use_unroll_20 = in_feature_q_a_mux_all[DATA_WIDTH* (20%IFMAP_PAR+1)-1  : DATA_WIDTH* (20%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_20 = in_feature_q_b_mux_all[DATA_WIDTH* (20%IFMAP_PAR+1)-1  : DATA_WIDTH* (20%IFMAP_PAR)];
//第21次展开：21/32
  
  assign   in_feature_q_a_dsp_use_unroll_21 = in_feature_q_a_mux_all[DATA_WIDTH* (21%IFMAP_PAR+1)-1  : DATA_WIDTH* (21%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_21 = in_feature_q_b_mux_all[DATA_WIDTH* (21%IFMAP_PAR+1)-1  : DATA_WIDTH* (21%IFMAP_PAR)];
//第22次展开：22/32
  
  assign   in_feature_q_a_dsp_use_unroll_22 = in_feature_q_a_mux_all[DATA_WIDTH* (22%IFMAP_PAR+1)-1  : DATA_WIDTH* (22%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_22 = in_feature_q_b_mux_all[DATA_WIDTH* (22%IFMAP_PAR+1)-1  : DATA_WIDTH* (22%IFMAP_PAR)];
//第23次展开：23/32
  
  assign   in_feature_q_a_dsp_use_unroll_23 = in_feature_q_a_mux_all[DATA_WIDTH* (23%IFMAP_PAR+1)-1  : DATA_WIDTH* (23%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_23 = in_feature_q_b_mux_all[DATA_WIDTH* (23%IFMAP_PAR+1)-1  : DATA_WIDTH* (23%IFMAP_PAR)];
//第24次展开：24/32
  
  assign   in_feature_q_a_dsp_use_unroll_24 = in_feature_q_a_mux_all[DATA_WIDTH* (24%IFMAP_PAR+1)-1  : DATA_WIDTH* (24%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_24 = in_feature_q_b_mux_all[DATA_WIDTH* (24%IFMAP_PAR+1)-1  : DATA_WIDTH* (24%IFMAP_PAR)];
//第25次展开：25/32
  
  assign   in_feature_q_a_dsp_use_unroll_25 = in_feature_q_a_mux_all[DATA_WIDTH* (25%IFMAP_PAR+1)-1  : DATA_WIDTH* (25%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_25 = in_feature_q_b_mux_all[DATA_WIDTH* (25%IFMAP_PAR+1)-1  : DATA_WIDTH* (25%IFMAP_PAR)];
//第26次展开：26/32
  
  assign   in_feature_q_a_dsp_use_unroll_26 = in_feature_q_a_mux_all[DATA_WIDTH* (26%IFMAP_PAR+1)-1  : DATA_WIDTH* (26%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_26 = in_feature_q_b_mux_all[DATA_WIDTH* (26%IFMAP_PAR+1)-1  : DATA_WIDTH* (26%IFMAP_PAR)];
//第27次展开：27/32
  
  assign   in_feature_q_a_dsp_use_unroll_27 = in_feature_q_a_mux_all[DATA_WIDTH* (27%IFMAP_PAR+1)-1  : DATA_WIDTH* (27%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_27 = in_feature_q_b_mux_all[DATA_WIDTH* (27%IFMAP_PAR+1)-1  : DATA_WIDTH* (27%IFMAP_PAR)];
//第28次展开：28/32
  
  assign   in_feature_q_a_dsp_use_unroll_28 = in_feature_q_a_mux_all[DATA_WIDTH* (28%IFMAP_PAR+1)-1  : DATA_WIDTH* (28%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_28 = in_feature_q_b_mux_all[DATA_WIDTH* (28%IFMAP_PAR+1)-1  : DATA_WIDTH* (28%IFMAP_PAR)];
//第29次展开：29/32
  
  assign   in_feature_q_a_dsp_use_unroll_29 = in_feature_q_a_mux_all[DATA_WIDTH* (29%IFMAP_PAR+1)-1  : DATA_WIDTH* (29%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_29 = in_feature_q_b_mux_all[DATA_WIDTH* (29%IFMAP_PAR+1)-1  : DATA_WIDTH* (29%IFMAP_PAR)];
//第30次展开：30/32
  
  assign   in_feature_q_a_dsp_use_unroll_30 = in_feature_q_a_mux_all[DATA_WIDTH* (30%IFMAP_PAR+1)-1  : DATA_WIDTH* (30%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_30 = in_feature_q_b_mux_all[DATA_WIDTH* (30%IFMAP_PAR+1)-1  : DATA_WIDTH* (30%IFMAP_PAR)];
//第31次展开：31/32
  
  assign   in_feature_q_a_dsp_use_unroll_31 = in_feature_q_a_mux_all[DATA_WIDTH* (31%IFMAP_PAR+1)-1  : DATA_WIDTH* (31%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_31 = in_feature_q_b_mux_all[DATA_WIDTH* (31%IFMAP_PAR+1)-1  : DATA_WIDTH* (31%IFMAP_PAR)];
 

 
//generate for开始------------------------------->
//dsp_count:32
//第0次展开：0/32

  
// *************half DSP
	mult	mult_inst_0(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_0 ),
		.datab_0 ( weight_q_a_unroll_0),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_0 ),
		.datab_1 ( weight_q_b_unroll_0),
		.ena0 ( enable_mult ),
		.result ( result_unroll_0 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_0(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_0 ),
		.data_b ( weight_data_b_unroll_0 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_0 ),
		.q_b ( weight_q_b_unroll_0 )
		);
			
	
	
//第1次展开：1/32

  
// *************half DSP
	mult	mult_inst_1(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_1 ),
		.datab_0 ( weight_q_a_unroll_1),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_1 ),
		.datab_1 ( weight_q_b_unroll_1),
		.ena0 ( enable_mult ),
		.result ( result_unroll_1 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_1(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_1 ),
		.data_b ( weight_data_b_unroll_1 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_1 ),
		.q_b ( weight_q_b_unroll_1 )
		);
			
	
	
//第2次展开：2/32

  
// *************half DSP
	mult	mult_inst_2(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_2 ),
		.datab_0 ( weight_q_a_unroll_2),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_2 ),
		.datab_1 ( weight_q_b_unroll_2),
		.ena0 ( enable_mult ),
		.result ( result_unroll_2 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_2(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_2 ),
		.data_b ( weight_data_b_unroll_2 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_2 ),
		.q_b ( weight_q_b_unroll_2 )
		);
			
	
	
//第3次展开：3/32

  
// *************half DSP
	mult	mult_inst_3(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_3 ),
		.datab_0 ( weight_q_a_unroll_3),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_3 ),
		.datab_1 ( weight_q_b_unroll_3),
		.ena0 ( enable_mult ),
		.result ( result_unroll_3 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_3(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_3 ),
		.data_b ( weight_data_b_unroll_3 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_3 ),
		.q_b ( weight_q_b_unroll_3 )
		);
			
	
	
//第4次展开：4/32

  
// *************half DSP
	mult	mult_inst_4(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_4 ),
		.datab_0 ( weight_q_a_unroll_4),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_4 ),
		.datab_1 ( weight_q_b_unroll_4),
		.ena0 ( enable_mult ),
		.result ( result_unroll_4 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_4(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_4 ),
		.data_b ( weight_data_b_unroll_4 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_4 ),
		.q_b ( weight_q_b_unroll_4 )
		);
			
	
	
//第5次展开：5/32

  
// *************half DSP
	mult	mult_inst_5(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_5 ),
		.datab_0 ( weight_q_a_unroll_5),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_5 ),
		.datab_1 ( weight_q_b_unroll_5),
		.ena0 ( enable_mult ),
		.result ( result_unroll_5 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_5(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_5 ),
		.data_b ( weight_data_b_unroll_5 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_5 ),
		.q_b ( weight_q_b_unroll_5 )
		);
			
	
	
//第6次展开：6/32

  
// *************half DSP
	mult	mult_inst_6(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_6 ),
		.datab_0 ( weight_q_a_unroll_6),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_6 ),
		.datab_1 ( weight_q_b_unroll_6),
		.ena0 ( enable_mult ),
		.result ( result_unroll_6 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_6(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_6 ),
		.data_b ( weight_data_b_unroll_6 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_6 ),
		.q_b ( weight_q_b_unroll_6 )
		);
			
	
	
//第7次展开：7/32

  
// *************half DSP
	mult	mult_inst_7(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_7 ),
		.datab_0 ( weight_q_a_unroll_7),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_7 ),
		.datab_1 ( weight_q_b_unroll_7),
		.ena0 ( enable_mult ),
		.result ( result_unroll_7 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_7(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_7 ),
		.data_b ( weight_data_b_unroll_7 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_7 ),
		.q_b ( weight_q_b_unroll_7 )
		);
			
	
	
//第8次展开：8/32

  
// *************half DSP
	mult	mult_inst_8(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_8 ),
		.datab_0 ( weight_q_a_unroll_8),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_8 ),
		.datab_1 ( weight_q_b_unroll_8),
		.ena0 ( enable_mult ),
		.result ( result_unroll_8 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_8(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_8 ),
		.data_b ( weight_data_b_unroll_8 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_8 ),
		.q_b ( weight_q_b_unroll_8 )
		);
			
	
	
//第9次展开：9/32

  
// *************half DSP
	mult	mult_inst_9(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_9 ),
		.datab_0 ( weight_q_a_unroll_9),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_9 ),
		.datab_1 ( weight_q_b_unroll_9),
		.ena0 ( enable_mult ),
		.result ( result_unroll_9 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_9(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_9 ),
		.data_b ( weight_data_b_unroll_9 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_9 ),
		.q_b ( weight_q_b_unroll_9 )
		);
			
	
	
//第10次展开：10/32

  
// *************half DSP
	mult	mult_inst_10(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_10 ),
		.datab_0 ( weight_q_a_unroll_10),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_10 ),
		.datab_1 ( weight_q_b_unroll_10),
		.ena0 ( enable_mult ),
		.result ( result_unroll_10 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_10(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_10 ),
		.data_b ( weight_data_b_unroll_10 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_10 ),
		.q_b ( weight_q_b_unroll_10 )
		);
			
	
	
//第11次展开：11/32

  
// *************half DSP
	mult	mult_inst_11(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_11 ),
		.datab_0 ( weight_q_a_unroll_11),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_11 ),
		.datab_1 ( weight_q_b_unroll_11),
		.ena0 ( enable_mult ),
		.result ( result_unroll_11 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_11(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_11 ),
		.data_b ( weight_data_b_unroll_11 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_11 ),
		.q_b ( weight_q_b_unroll_11 )
		);
			
	
	
//第12次展开：12/32

  
// *************half DSP
	mult	mult_inst_12(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_12 ),
		.datab_0 ( weight_q_a_unroll_12),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_12 ),
		.datab_1 ( weight_q_b_unroll_12),
		.ena0 ( enable_mult ),
		.result ( result_unroll_12 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_12(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_12 ),
		.data_b ( weight_data_b_unroll_12 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_12 ),
		.q_b ( weight_q_b_unroll_12 )
		);
			
	
	
//第13次展开：13/32

  
// *************half DSP
	mult	mult_inst_13(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_13 ),
		.datab_0 ( weight_q_a_unroll_13),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_13 ),
		.datab_1 ( weight_q_b_unroll_13),
		.ena0 ( enable_mult ),
		.result ( result_unroll_13 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_13(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_13 ),
		.data_b ( weight_data_b_unroll_13 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_13 ),
		.q_b ( weight_q_b_unroll_13 )
		);
			
	
	
//第14次展开：14/32

  
// *************half DSP
	mult	mult_inst_14(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_14 ),
		.datab_0 ( weight_q_a_unroll_14),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_14 ),
		.datab_1 ( weight_q_b_unroll_14),
		.ena0 ( enable_mult ),
		.result ( result_unroll_14 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_14(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_14 ),
		.data_b ( weight_data_b_unroll_14 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_14 ),
		.q_b ( weight_q_b_unroll_14 )
		);
			
	
	
//第15次展开：15/32

  
// *************half DSP
	mult	mult_inst_15(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_15 ),
		.datab_0 ( weight_q_a_unroll_15),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_15 ),
		.datab_1 ( weight_q_b_unroll_15),
		.ena0 ( enable_mult ),
		.result ( result_unroll_15 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_15(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_15 ),
		.data_b ( weight_data_b_unroll_15 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_15 ),
		.q_b ( weight_q_b_unroll_15 )
		);
			
	
	
//第16次展开：16/32

  
// *************half DSP
	mult	mult_inst_16(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_16 ),
		.datab_0 ( weight_q_a_unroll_16),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_16 ),
		.datab_1 ( weight_q_b_unroll_16),
		.ena0 ( enable_mult ),
		.result ( result_unroll_16 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_16(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_16 ),
		.data_b ( weight_data_b_unroll_16 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_16 ),
		.q_b ( weight_q_b_unroll_16 )
		);
			
	
	
//第17次展开：17/32

  
// *************half DSP
	mult	mult_inst_17(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_17 ),
		.datab_0 ( weight_q_a_unroll_17),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_17 ),
		.datab_1 ( weight_q_b_unroll_17),
		.ena0 ( enable_mult ),
		.result ( result_unroll_17 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_17(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_17 ),
		.data_b ( weight_data_b_unroll_17 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_17 ),
		.q_b ( weight_q_b_unroll_17 )
		);
			
	
	
//第18次展开：18/32

  
// *************half DSP
	mult	mult_inst_18(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_18 ),
		.datab_0 ( weight_q_a_unroll_18),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_18 ),
		.datab_1 ( weight_q_b_unroll_18),
		.ena0 ( enable_mult ),
		.result ( result_unroll_18 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_18(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_18 ),
		.data_b ( weight_data_b_unroll_18 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_18 ),
		.q_b ( weight_q_b_unroll_18 )
		);
			
	
	
//第19次展开：19/32

  
// *************half DSP
	mult	mult_inst_19(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_19 ),
		.datab_0 ( weight_q_a_unroll_19),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_19 ),
		.datab_1 ( weight_q_b_unroll_19),
		.ena0 ( enable_mult ),
		.result ( result_unroll_19 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_19(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_19 ),
		.data_b ( weight_data_b_unroll_19 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_19 ),
		.q_b ( weight_q_b_unroll_19 )
		);
			
	
	
//第20次展开：20/32

  
// *************half DSP
	mult	mult_inst_20(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_20 ),
		.datab_0 ( weight_q_a_unroll_20),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_20 ),
		.datab_1 ( weight_q_b_unroll_20),
		.ena0 ( enable_mult ),
		.result ( result_unroll_20 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_20(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_20 ),
		.data_b ( weight_data_b_unroll_20 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_20 ),
		.q_b ( weight_q_b_unroll_20 )
		);
			
	
	
//第21次展开：21/32

  
// *************half DSP
	mult	mult_inst_21(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_21 ),
		.datab_0 ( weight_q_a_unroll_21),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_21 ),
		.datab_1 ( weight_q_b_unroll_21),
		.ena0 ( enable_mult ),
		.result ( result_unroll_21 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_21(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_21 ),
		.data_b ( weight_data_b_unroll_21 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_21 ),
		.q_b ( weight_q_b_unroll_21 )
		);
			
	
	
//第22次展开：22/32

  
// *************half DSP
	mult	mult_inst_22(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_22 ),
		.datab_0 ( weight_q_a_unroll_22),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_22 ),
		.datab_1 ( weight_q_b_unroll_22),
		.ena0 ( enable_mult ),
		.result ( result_unroll_22 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_22(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_22 ),
		.data_b ( weight_data_b_unroll_22 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_22 ),
		.q_b ( weight_q_b_unroll_22 )
		);
			
	
	
//第23次展开：23/32

  
// *************half DSP
	mult	mult_inst_23(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_23 ),
		.datab_0 ( weight_q_a_unroll_23),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_23 ),
		.datab_1 ( weight_q_b_unroll_23),
		.ena0 ( enable_mult ),
		.result ( result_unroll_23 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_23(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_23 ),
		.data_b ( weight_data_b_unroll_23 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_23 ),
		.q_b ( weight_q_b_unroll_23 )
		);
			
	
	
//第24次展开：24/32

  
// *************half DSP
	mult	mult_inst_24(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_24 ),
		.datab_0 ( weight_q_a_unroll_24),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_24 ),
		.datab_1 ( weight_q_b_unroll_24),
		.ena0 ( enable_mult ),
		.result ( result_unroll_24 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_24(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_24 ),
		.data_b ( weight_data_b_unroll_24 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_24 ),
		.q_b ( weight_q_b_unroll_24 )
		);
			
	
	
//第25次展开：25/32

  
// *************half DSP
	mult	mult_inst_25(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_25 ),
		.datab_0 ( weight_q_a_unroll_25),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_25 ),
		.datab_1 ( weight_q_b_unroll_25),
		.ena0 ( enable_mult ),
		.result ( result_unroll_25 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_25(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_25 ),
		.data_b ( weight_data_b_unroll_25 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_25 ),
		.q_b ( weight_q_b_unroll_25 )
		);
			
	
	
//第26次展开：26/32

  
// *************half DSP
	mult	mult_inst_26(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_26 ),
		.datab_0 ( weight_q_a_unroll_26),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_26 ),
		.datab_1 ( weight_q_b_unroll_26),
		.ena0 ( enable_mult ),
		.result ( result_unroll_26 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_26(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_26 ),
		.data_b ( weight_data_b_unroll_26 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_26 ),
		.q_b ( weight_q_b_unroll_26 )
		);
			
	
	
//第27次展开：27/32

  
// *************half DSP
	mult	mult_inst_27(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_27 ),
		.datab_0 ( weight_q_a_unroll_27),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_27 ),
		.datab_1 ( weight_q_b_unroll_27),
		.ena0 ( enable_mult ),
		.result ( result_unroll_27 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_27(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_27 ),
		.data_b ( weight_data_b_unroll_27 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_27 ),
		.q_b ( weight_q_b_unroll_27 )
		);
			
	
	
//第28次展开：28/32

  
// *************half DSP
	mult	mult_inst_28(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_28 ),
		.datab_0 ( weight_q_a_unroll_28),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_28 ),
		.datab_1 ( weight_q_b_unroll_28),
		.ena0 ( enable_mult ),
		.result ( result_unroll_28 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_28(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_28 ),
		.data_b ( weight_data_b_unroll_28 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_28 ),
		.q_b ( weight_q_b_unroll_28 )
		);
			
	
	
//第29次展开：29/32

  
// *************half DSP
	mult	mult_inst_29(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_29 ),
		.datab_0 ( weight_q_a_unroll_29),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_29 ),
		.datab_1 ( weight_q_b_unroll_29),
		.ena0 ( enable_mult ),
		.result ( result_unroll_29 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_29(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_29 ),
		.data_b ( weight_data_b_unroll_29 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_29 ),
		.q_b ( weight_q_b_unroll_29 )
		);
			
	
	
//第30次展开：30/32

  
// *************half DSP
	mult	mult_inst_30(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_30 ),
		.datab_0 ( weight_q_a_unroll_30),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_30 ),
		.datab_1 ( weight_q_b_unroll_30),
		.ena0 ( enable_mult ),
		.result ( result_unroll_30 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_30(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_30 ),
		.data_b ( weight_data_b_unroll_30 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_30 ),
		.q_b ( weight_q_b_unroll_30 )
		);
			
	
	
//第31次展开：31/32

  
// *************half DSP
	mult	mult_inst_31(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use_unroll_31 ),
		.datab_0 ( weight_q_a_unroll_31),
		.dataa_1 ( in_feature_q_b_dsp_use_unroll_31 ),
		.datab_1 ( weight_q_b_unroll_31),
		.ena0 ( enable_mult ),
		.result ( result_unroll_31 ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst_31(
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a_unroll_31 ),
		.data_b ( weight_data_b_unroll_31 ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a_unroll_31 ),
		.q_b ( weight_q_b_unroll_31 )
		);
			
	
	


//generate for开始------------------------------->
//num1:16
//第0次展开：0/16
  
  assign   result_relu_use_unroll_0 =result[0*2]+result[0*2+1];

//第1次展开：1/16
  
  assign   result_relu_use_unroll_1 =result[1*2]+result[1*2+1];

//第2次展开：2/16
  
  assign   result_relu_use_unroll_2 =result[2*2]+result[2*2+1];

//第3次展开：3/16
  
  assign   result_relu_use_unroll_3 =result[3*2]+result[3*2+1];

//第4次展开：4/16
  
  assign   result_relu_use_unroll_4 =result[4*2]+result[4*2+1];

//第5次展开：5/16
  
  assign   result_relu_use_unroll_5 =result[5*2]+result[5*2+1];

//第6次展开：6/16
  
  assign   result_relu_use_unroll_6 =result[6*2]+result[6*2+1];

//第7次展开：7/16
  
  assign   result_relu_use_unroll_7 =result[7*2]+result[7*2+1];

//第8次展开：8/16
  
  assign   result_relu_use_unroll_8 =result[8*2]+result[8*2+1];

//第9次展开：9/16
  
  assign   result_relu_use_unroll_9 =result[9*2]+result[9*2+1];

//第10次展开：10/16
  
  assign   result_relu_use_unroll_10 =result[10*2]+result[10*2+1];

//第11次展开：11/16
  
  assign   result_relu_use_unroll_11 =result[11*2]+result[11*2+1];

//第12次展开：12/16
  
  assign   result_relu_use_unroll_12 =result[12*2]+result[12*2+1];

//第13次展开：13/16
  
  assign   result_relu_use_unroll_13 =result[13*2]+result[13*2+1];

//第14次展开：14/16
  
  assign   result_relu_use_unroll_14 =result[14*2]+result[14*2+1];

//第15次展开：15/16
  
  assign   result_relu_use_unroll_15 =result[15*2]+result[15*2+1];







//generate for开始------------------------------->
//pool_relu_count:16
//第0次展开：0/16

  

	
	pool_2 pool_inst_0( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_0),
	 
		.data_a(data_a_unroll_0),
		.data_b(data_b_unroll_0),
		.rden_a(rden_a_unroll_0),
		.rden_b(rden_b_unroll_0),
		.wren_a(wren_a_unroll_0),
		.wren_b(wren_b_unroll_0),
		
		.address_a_t(address_a_t_unroll_0),
		.address_b_t(address_b_t_unroll_0),
		
		.q_a(q_a_unroll_0),
		.q_b(q_b_unroll_0)
		);
		
	pool_memo_2 pool_memo_inst_0(     
		.address_a(address_a_t_use_unroll_0),
		.address_b (address_b_t_use_unroll_0),
		.clock(clock),
		.data_a(data_a_unroll_0),
		.data_b(data_b_unroll_0),
		.rden_a(rden_a_use_unroll_0),
		.rden_b(rden_b_use_unroll_0),
		.wren_a(wren_a_use_unroll_0),
		.wren_b(wren_b_use_unroll_0),
		.q_a(q_a_unroll_0),
		.q_b(q_b_unroll_0) 
		);	


	relu_param_2 relu_param_0(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_0),
		.result_relu(result_after_relu_unroll_0)

);		
	
//第1次展开：1/16

  

	
	pool_2 pool_inst_1( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_1),
	 
		.data_a(data_a_unroll_1),
		.data_b(data_b_unroll_1),
		.rden_a(rden_a_unroll_1),
		.rden_b(rden_b_unroll_1),
		.wren_a(wren_a_unroll_1),
		.wren_b(wren_b_unroll_1),
		
		.address_a_t(address_a_t_unroll_1),
		.address_b_t(address_b_t_unroll_1),
		
		.q_a(q_a_unroll_1),
		.q_b(q_b_unroll_1)
		);
		
	pool_memo_2 pool_memo_inst_1(     
		.address_a(address_a_t_use_unroll_1),
		.address_b (address_b_t_use_unroll_1),
		.clock(clock),
		.data_a(data_a_unroll_1),
		.data_b(data_b_unroll_1),
		.rden_a(rden_a_use_unroll_1),
		.rden_b(rden_b_use_unroll_1),
		.wren_a(wren_a_use_unroll_1),
		.wren_b(wren_b_use_unroll_1),
		.q_a(q_a_unroll_1),
		.q_b(q_b_unroll_1) 
		);	


	relu_param_2 relu_param_1(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_1),
		.result_relu(result_after_relu_unroll_1)

);		
	
//第2次展开：2/16

  

	
	pool_2 pool_inst_2( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_2),
	 
		.data_a(data_a_unroll_2),
		.data_b(data_b_unroll_2),
		.rden_a(rden_a_unroll_2),
		.rden_b(rden_b_unroll_2),
		.wren_a(wren_a_unroll_2),
		.wren_b(wren_b_unroll_2),
		
		.address_a_t(address_a_t_unroll_2),
		.address_b_t(address_b_t_unroll_2),
		
		.q_a(q_a_unroll_2),
		.q_b(q_b_unroll_2)
		);
		
	pool_memo_2 pool_memo_inst_2(     
		.address_a(address_a_t_use_unroll_2),
		.address_b (address_b_t_use_unroll_2),
		.clock(clock),
		.data_a(data_a_unroll_2),
		.data_b(data_b_unroll_2),
		.rden_a(rden_a_use_unroll_2),
		.rden_b(rden_b_use_unroll_2),
		.wren_a(wren_a_use_unroll_2),
		.wren_b(wren_b_use_unroll_2),
		.q_a(q_a_unroll_2),
		.q_b(q_b_unroll_2) 
		);	


	relu_param_2 relu_param_2(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_2),
		.result_relu(result_after_relu_unroll_2)

);		
	
//第3次展开：3/16

  

	
	pool_2 pool_inst_3( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_3),
	 
		.data_a(data_a_unroll_3),
		.data_b(data_b_unroll_3),
		.rden_a(rden_a_unroll_3),
		.rden_b(rden_b_unroll_3),
		.wren_a(wren_a_unroll_3),
		.wren_b(wren_b_unroll_3),
		
		.address_a_t(address_a_t_unroll_3),
		.address_b_t(address_b_t_unroll_3),
		
		.q_a(q_a_unroll_3),
		.q_b(q_b_unroll_3)
		);
		
	pool_memo_2 pool_memo_inst_3(     
		.address_a(address_a_t_use_unroll_3),
		.address_b (address_b_t_use_unroll_3),
		.clock(clock),
		.data_a(data_a_unroll_3),
		.data_b(data_b_unroll_3),
		.rden_a(rden_a_use_unroll_3),
		.rden_b(rden_b_use_unroll_3),
		.wren_a(wren_a_use_unroll_3),
		.wren_b(wren_b_use_unroll_3),
		.q_a(q_a_unroll_3),
		.q_b(q_b_unroll_3) 
		);	


	relu_param_2 relu_param_3(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_3),
		.result_relu(result_after_relu_unroll_3)

);		
	
//第4次展开：4/16

  

	
	pool_2 pool_inst_4( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_4),
	 
		.data_a(data_a_unroll_4),
		.data_b(data_b_unroll_4),
		.rden_a(rden_a_unroll_4),
		.rden_b(rden_b_unroll_4),
		.wren_a(wren_a_unroll_4),
		.wren_b(wren_b_unroll_4),
		
		.address_a_t(address_a_t_unroll_4),
		.address_b_t(address_b_t_unroll_4),
		
		.q_a(q_a_unroll_4),
		.q_b(q_b_unroll_4)
		);
		
	pool_memo_2 pool_memo_inst_4(     
		.address_a(address_a_t_use_unroll_4),
		.address_b (address_b_t_use_unroll_4),
		.clock(clock),
		.data_a(data_a_unroll_4),
		.data_b(data_b_unroll_4),
		.rden_a(rden_a_use_unroll_4),
		.rden_b(rden_b_use_unroll_4),
		.wren_a(wren_a_use_unroll_4),
		.wren_b(wren_b_use_unroll_4),
		.q_a(q_a_unroll_4),
		.q_b(q_b_unroll_4) 
		);	


	relu_param_2 relu_param_4(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_4),
		.result_relu(result_after_relu_unroll_4)

);		
	
//第5次展开：5/16

  

	
	pool_2 pool_inst_5( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_5),
	 
		.data_a(data_a_unroll_5),
		.data_b(data_b_unroll_5),
		.rden_a(rden_a_unroll_5),
		.rden_b(rden_b_unroll_5),
		.wren_a(wren_a_unroll_5),
		.wren_b(wren_b_unroll_5),
		
		.address_a_t(address_a_t_unroll_5),
		.address_b_t(address_b_t_unroll_5),
		
		.q_a(q_a_unroll_5),
		.q_b(q_b_unroll_5)
		);
		
	pool_memo_2 pool_memo_inst_5(     
		.address_a(address_a_t_use_unroll_5),
		.address_b (address_b_t_use_unroll_5),
		.clock(clock),
		.data_a(data_a_unroll_5),
		.data_b(data_b_unroll_5),
		.rden_a(rden_a_use_unroll_5),
		.rden_b(rden_b_use_unroll_5),
		.wren_a(wren_a_use_unroll_5),
		.wren_b(wren_b_use_unroll_5),
		.q_a(q_a_unroll_5),
		.q_b(q_b_unroll_5) 
		);	


	relu_param_2 relu_param_5(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_5),
		.result_relu(result_after_relu_unroll_5)

);		
	
//第6次展开：6/16

  

	
	pool_2 pool_inst_6( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_6),
	 
		.data_a(data_a_unroll_6),
		.data_b(data_b_unroll_6),
		.rden_a(rden_a_unroll_6),
		.rden_b(rden_b_unroll_6),
		.wren_a(wren_a_unroll_6),
		.wren_b(wren_b_unroll_6),
		
		.address_a_t(address_a_t_unroll_6),
		.address_b_t(address_b_t_unroll_6),
		
		.q_a(q_a_unroll_6),
		.q_b(q_b_unroll_6)
		);
		
	pool_memo_2 pool_memo_inst_6(     
		.address_a(address_a_t_use_unroll_6),
		.address_b (address_b_t_use_unroll_6),
		.clock(clock),
		.data_a(data_a_unroll_6),
		.data_b(data_b_unroll_6),
		.rden_a(rden_a_use_unroll_6),
		.rden_b(rden_b_use_unroll_6),
		.wren_a(wren_a_use_unroll_6),
		.wren_b(wren_b_use_unroll_6),
		.q_a(q_a_unroll_6),
		.q_b(q_b_unroll_6) 
		);	


	relu_param_2 relu_param_6(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_6),
		.result_relu(result_after_relu_unroll_6)

);		
	
//第7次展开：7/16

  

	
	pool_2 pool_inst_7( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_7),
	 
		.data_a(data_a_unroll_7),
		.data_b(data_b_unroll_7),
		.rden_a(rden_a_unroll_7),
		.rden_b(rden_b_unroll_7),
		.wren_a(wren_a_unroll_7),
		.wren_b(wren_b_unroll_7),
		
		.address_a_t(address_a_t_unroll_7),
		.address_b_t(address_b_t_unroll_7),
		
		.q_a(q_a_unroll_7),
		.q_b(q_b_unroll_7)
		);
		
	pool_memo_2 pool_memo_inst_7(     
		.address_a(address_a_t_use_unroll_7),
		.address_b (address_b_t_use_unroll_7),
		.clock(clock),
		.data_a(data_a_unroll_7),
		.data_b(data_b_unroll_7),
		.rden_a(rden_a_use_unroll_7),
		.rden_b(rden_b_use_unroll_7),
		.wren_a(wren_a_use_unroll_7),
		.wren_b(wren_b_use_unroll_7),
		.q_a(q_a_unroll_7),
		.q_b(q_b_unroll_7) 
		);	


	relu_param_2 relu_param_7(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_7),
		.result_relu(result_after_relu_unroll_7)

);		
	
//第8次展开：8/16

  

	
	pool_2 pool_inst_8( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_8),
	 
		.data_a(data_a_unroll_8),
		.data_b(data_b_unroll_8),
		.rden_a(rden_a_unroll_8),
		.rden_b(rden_b_unroll_8),
		.wren_a(wren_a_unroll_8),
		.wren_b(wren_b_unroll_8),
		
		.address_a_t(address_a_t_unroll_8),
		.address_b_t(address_b_t_unroll_8),
		
		.q_a(q_a_unroll_8),
		.q_b(q_b_unroll_8)
		);
		
	pool_memo_2 pool_memo_inst_8(     
		.address_a(address_a_t_use_unroll_8),
		.address_b (address_b_t_use_unroll_8),
		.clock(clock),
		.data_a(data_a_unroll_8),
		.data_b(data_b_unroll_8),
		.rden_a(rden_a_use_unroll_8),
		.rden_b(rden_b_use_unroll_8),
		.wren_a(wren_a_use_unroll_8),
		.wren_b(wren_b_use_unroll_8),
		.q_a(q_a_unroll_8),
		.q_b(q_b_unroll_8) 
		);	


	relu_param_2 relu_param_8(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_8),
		.result_relu(result_after_relu_unroll_8)

);		
	
//第9次展开：9/16

  

	
	pool_2 pool_inst_9( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_9),
	 
		.data_a(data_a_unroll_9),
		.data_b(data_b_unroll_9),
		.rden_a(rden_a_unroll_9),
		.rden_b(rden_b_unroll_9),
		.wren_a(wren_a_unroll_9),
		.wren_b(wren_b_unroll_9),
		
		.address_a_t(address_a_t_unroll_9),
		.address_b_t(address_b_t_unroll_9),
		
		.q_a(q_a_unroll_9),
		.q_b(q_b_unroll_9)
		);
		
	pool_memo_2 pool_memo_inst_9(     
		.address_a(address_a_t_use_unroll_9),
		.address_b (address_b_t_use_unroll_9),
		.clock(clock),
		.data_a(data_a_unroll_9),
		.data_b(data_b_unroll_9),
		.rden_a(rden_a_use_unroll_9),
		.rden_b(rden_b_use_unroll_9),
		.wren_a(wren_a_use_unroll_9),
		.wren_b(wren_b_use_unroll_9),
		.q_a(q_a_unroll_9),
		.q_b(q_b_unroll_9) 
		);	


	relu_param_2 relu_param_9(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_9),
		.result_relu(result_after_relu_unroll_9)

);		
	
//第10次展开：10/16

  

	
	pool_2 pool_inst_10( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_10),
	 
		.data_a(data_a_unroll_10),
		.data_b(data_b_unroll_10),
		.rden_a(rden_a_unroll_10),
		.rden_b(rden_b_unroll_10),
		.wren_a(wren_a_unroll_10),
		.wren_b(wren_b_unroll_10),
		
		.address_a_t(address_a_t_unroll_10),
		.address_b_t(address_b_t_unroll_10),
		
		.q_a(q_a_unroll_10),
		.q_b(q_b_unroll_10)
		);
		
	pool_memo_2 pool_memo_inst_10(     
		.address_a(address_a_t_use_unroll_10),
		.address_b (address_b_t_use_unroll_10),
		.clock(clock),
		.data_a(data_a_unroll_10),
		.data_b(data_b_unroll_10),
		.rden_a(rden_a_use_unroll_10),
		.rden_b(rden_b_use_unroll_10),
		.wren_a(wren_a_use_unroll_10),
		.wren_b(wren_b_use_unroll_10),
		.q_a(q_a_unroll_10),
		.q_b(q_b_unroll_10) 
		);	


	relu_param_2 relu_param_10(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_10),
		.result_relu(result_after_relu_unroll_10)

);		
	
//第11次展开：11/16

  

	
	pool_2 pool_inst_11( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_11),
	 
		.data_a(data_a_unroll_11),
		.data_b(data_b_unroll_11),
		.rden_a(rden_a_unroll_11),
		.rden_b(rden_b_unroll_11),
		.wren_a(wren_a_unroll_11),
		.wren_b(wren_b_unroll_11),
		
		.address_a_t(address_a_t_unroll_11),
		.address_b_t(address_b_t_unroll_11),
		
		.q_a(q_a_unroll_11),
		.q_b(q_b_unroll_11)
		);
		
	pool_memo_2 pool_memo_inst_11(     
		.address_a(address_a_t_use_unroll_11),
		.address_b (address_b_t_use_unroll_11),
		.clock(clock),
		.data_a(data_a_unroll_11),
		.data_b(data_b_unroll_11),
		.rden_a(rden_a_use_unroll_11),
		.rden_b(rden_b_use_unroll_11),
		.wren_a(wren_a_use_unroll_11),
		.wren_b(wren_b_use_unroll_11),
		.q_a(q_a_unroll_11),
		.q_b(q_b_unroll_11) 
		);	


	relu_param_2 relu_param_11(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_11),
		.result_relu(result_after_relu_unroll_11)

);		
	
//第12次展开：12/16

  

	
	pool_2 pool_inst_12( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_12),
	 
		.data_a(data_a_unroll_12),
		.data_b(data_b_unroll_12),
		.rden_a(rden_a_unroll_12),
		.rden_b(rden_b_unroll_12),
		.wren_a(wren_a_unroll_12),
		.wren_b(wren_b_unroll_12),
		
		.address_a_t(address_a_t_unroll_12),
		.address_b_t(address_b_t_unroll_12),
		
		.q_a(q_a_unroll_12),
		.q_b(q_b_unroll_12)
		);
		
	pool_memo_2 pool_memo_inst_12(     
		.address_a(address_a_t_use_unroll_12),
		.address_b (address_b_t_use_unroll_12),
		.clock(clock),
		.data_a(data_a_unroll_12),
		.data_b(data_b_unroll_12),
		.rden_a(rden_a_use_unroll_12),
		.rden_b(rden_b_use_unroll_12),
		.wren_a(wren_a_use_unroll_12),
		.wren_b(wren_b_use_unroll_12),
		.q_a(q_a_unroll_12),
		.q_b(q_b_unroll_12) 
		);	


	relu_param_2 relu_param_12(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_12),
		.result_relu(result_after_relu_unroll_12)

);		
	
//第13次展开：13/16

  

	
	pool_2 pool_inst_13( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_13),
	 
		.data_a(data_a_unroll_13),
		.data_b(data_b_unroll_13),
		.rden_a(rden_a_unroll_13),
		.rden_b(rden_b_unroll_13),
		.wren_a(wren_a_unroll_13),
		.wren_b(wren_b_unroll_13),
		
		.address_a_t(address_a_t_unroll_13),
		.address_b_t(address_b_t_unroll_13),
		
		.q_a(q_a_unroll_13),
		.q_b(q_b_unroll_13)
		);
		
	pool_memo_2 pool_memo_inst_13(     
		.address_a(address_a_t_use_unroll_13),
		.address_b (address_b_t_use_unroll_13),
		.clock(clock),
		.data_a(data_a_unroll_13),
		.data_b(data_b_unroll_13),
		.rden_a(rden_a_use_unroll_13),
		.rden_b(rden_b_use_unroll_13),
		.wren_a(wren_a_use_unroll_13),
		.wren_b(wren_b_use_unroll_13),
		.q_a(q_a_unroll_13),
		.q_b(q_b_unroll_13) 
		);	


	relu_param_2 relu_param_13(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_13),
		.result_relu(result_after_relu_unroll_13)

);		
	
//第14次展开：14/16

  

	
	pool_2 pool_inst_14( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_14),
	 
		.data_a(data_a_unroll_14),
		.data_b(data_b_unroll_14),
		.rden_a(rden_a_unroll_14),
		.rden_b(rden_b_unroll_14),
		.wren_a(wren_a_unroll_14),
		.wren_b(wren_b_unroll_14),
		
		.address_a_t(address_a_t_unroll_14),
		.address_b_t(address_b_t_unroll_14),
		
		.q_a(q_a_unroll_14),
		.q_b(q_b_unroll_14)
		);
		
	pool_memo_2 pool_memo_inst_14(     
		.address_a(address_a_t_use_unroll_14),
		.address_b (address_b_t_use_unroll_14),
		.clock(clock),
		.data_a(data_a_unroll_14),
		.data_b(data_b_unroll_14),
		.rden_a(rden_a_use_unroll_14),
		.rden_b(rden_b_use_unroll_14),
		.wren_a(wren_a_use_unroll_14),
		.wren_b(wren_b_use_unroll_14),
		.q_a(q_a_unroll_14),
		.q_b(q_b_unroll_14) 
		);	


	relu_param_2 relu_param_14(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_14),
		.result_relu(result_after_relu_unroll_14)

);		
	
//第15次展开：15/16

  

	
	pool_2 pool_inst_15( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu_unroll_15),
	 
		.data_a(data_a_unroll_15),
		.data_b(data_b_unroll_15),
		.rden_a(rden_a_unroll_15),
		.rden_b(rden_b_unroll_15),
		.wren_a(wren_a_unroll_15),
		.wren_b(wren_b_unroll_15),
		
		.address_a_t(address_a_t_unroll_15),
		.address_b_t(address_b_t_unroll_15),
		
		.q_a(q_a_unroll_15),
		.q_b(q_b_unroll_15)
		);
		
	pool_memo_2 pool_memo_inst_15(     
		.address_a(address_a_t_use_unroll_15),
		.address_b (address_b_t_use_unroll_15),
		.clock(clock),
		.data_a(data_a_unroll_15),
		.data_b(data_b_unroll_15),
		.rden_a(rden_a_use_unroll_15),
		.rden_b(rden_b_use_unroll_15),
		.wren_a(wren_a_use_unroll_15),
		.wren_b(wren_b_use_unroll_15),
		.q_a(q_a_unroll_15),
		.q_b(q_b_unroll_15) 
		);	


	relu_param_2 relu_param_15(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_15),
		.result_relu(result_after_relu_unroll_15)

);		
	



//**********************************************/////
//generate for开始------------------------------->
//output_num:16
//第0次展开：0/16
  
  assign q_b_all_out[0* DATA_WIDTH + DATA_WIDTH - 1:0 * DATA_WIDTH]=q_b_unroll_0;
  assign q_a_all_out[0* DATA_WIDTH + DATA_WIDTH - 1:0 * DATA_WIDTH]=q_a_unroll_0;
//第1次展开：1/16
  
  assign q_b_all_out[1* DATA_WIDTH + DATA_WIDTH - 1:1 * DATA_WIDTH]=q_b_unroll_1;
  assign q_a_all_out[1* DATA_WIDTH + DATA_WIDTH - 1:1 * DATA_WIDTH]=q_a_unroll_1;
//第2次展开：2/16
  
  assign q_b_all_out[2* DATA_WIDTH + DATA_WIDTH - 1:2 * DATA_WIDTH]=q_b_unroll_2;
  assign q_a_all_out[2* DATA_WIDTH + DATA_WIDTH - 1:2 * DATA_WIDTH]=q_a_unroll_2;
//第3次展开：3/16
  
  assign q_b_all_out[3* DATA_WIDTH + DATA_WIDTH - 1:3 * DATA_WIDTH]=q_b_unroll_3;
  assign q_a_all_out[3* DATA_WIDTH + DATA_WIDTH - 1:3 * DATA_WIDTH]=q_a_unroll_3;
//第4次展开：4/16
  
  assign q_b_all_out[4* DATA_WIDTH + DATA_WIDTH - 1:4 * DATA_WIDTH]=q_b_unroll_4;
  assign q_a_all_out[4* DATA_WIDTH + DATA_WIDTH - 1:4 * DATA_WIDTH]=q_a_unroll_4;
//第5次展开：5/16
  
  assign q_b_all_out[5* DATA_WIDTH + DATA_WIDTH - 1:5 * DATA_WIDTH]=q_b_unroll_5;
  assign q_a_all_out[5* DATA_WIDTH + DATA_WIDTH - 1:5 * DATA_WIDTH]=q_a_unroll_5;
//第6次展开：6/16
  
  assign q_b_all_out[6* DATA_WIDTH + DATA_WIDTH - 1:6 * DATA_WIDTH]=q_b_unroll_6;
  assign q_a_all_out[6* DATA_WIDTH + DATA_WIDTH - 1:6 * DATA_WIDTH]=q_a_unroll_6;
//第7次展开：7/16
  
  assign q_b_all_out[7* DATA_WIDTH + DATA_WIDTH - 1:7 * DATA_WIDTH]=q_b_unroll_7;
  assign q_a_all_out[7* DATA_WIDTH + DATA_WIDTH - 1:7 * DATA_WIDTH]=q_a_unroll_7;
//第8次展开：8/16
  
  assign q_b_all_out[8* DATA_WIDTH + DATA_WIDTH - 1:8 * DATA_WIDTH]=q_b_unroll_8;
  assign q_a_all_out[8* DATA_WIDTH + DATA_WIDTH - 1:8 * DATA_WIDTH]=q_a_unroll_8;
//第9次展开：9/16
  
  assign q_b_all_out[9* DATA_WIDTH + DATA_WIDTH - 1:9 * DATA_WIDTH]=q_b_unroll_9;
  assign q_a_all_out[9* DATA_WIDTH + DATA_WIDTH - 1:9 * DATA_WIDTH]=q_a_unroll_9;
//第10次展开：10/16
  
  assign q_b_all_out[10* DATA_WIDTH + DATA_WIDTH - 1:10 * DATA_WIDTH]=q_b_unroll_10;
  assign q_a_all_out[10* DATA_WIDTH + DATA_WIDTH - 1:10 * DATA_WIDTH]=q_a_unroll_10;
//第11次展开：11/16
  
  assign q_b_all_out[11* DATA_WIDTH + DATA_WIDTH - 1:11 * DATA_WIDTH]=q_b_unroll_11;
  assign q_a_all_out[11* DATA_WIDTH + DATA_WIDTH - 1:11 * DATA_WIDTH]=q_a_unroll_11;
//第12次展开：12/16
  
  assign q_b_all_out[12* DATA_WIDTH + DATA_WIDTH - 1:12 * DATA_WIDTH]=q_b_unroll_12;
  assign q_a_all_out[12* DATA_WIDTH + DATA_WIDTH - 1:12 * DATA_WIDTH]=q_a_unroll_12;
//第13次展开：13/16
  
  assign q_b_all_out[13* DATA_WIDTH + DATA_WIDTH - 1:13 * DATA_WIDTH]=q_b_unroll_13;
  assign q_a_all_out[13* DATA_WIDTH + DATA_WIDTH - 1:13 * DATA_WIDTH]=q_a_unroll_13;
//第14次展开：14/16
  
  assign q_b_all_out[14* DATA_WIDTH + DATA_WIDTH - 1:14 * DATA_WIDTH]=q_b_unroll_14;
  assign q_a_all_out[14* DATA_WIDTH + DATA_WIDTH - 1:14 * DATA_WIDTH]=q_a_unroll_14;
//第15次展开：15/16
  
  assign q_b_all_out[15* DATA_WIDTH + DATA_WIDTH - 1:15 * DATA_WIDTH]=q_b_unroll_15;
  assign q_a_all_out[15* DATA_WIDTH + DATA_WIDTH - 1:15 * DATA_WIDTH]=q_a_unroll_15;
 

endmodule  


