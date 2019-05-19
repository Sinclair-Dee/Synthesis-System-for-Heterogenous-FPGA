`include "param_1.vh"
module layer_1 (
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
parameter IFMAP_PAR= 1;
parameter NUM_DSP =4;
//******* ADD for resource parameter

parameter INPUT_NUM_MEM=1;	


parameter NUM_MULT=4;

	//data width parameter		
parameter	ACCUM_DATA_WIDTH=44;		
parameter	DATA_WIDTH=16;		
// number of input output feature map

	

//addr width
   parameter	IN_FEATURE_ADDR_WIDTH=11;		
parameter	WEIGHT_ADDR_WIDTH=9;		
parameter	POOL_ADDR_WIDTH=9;		

	
parameter	COUNT_SLOAD_BITWIDTH=4;	

			
 
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
	
	
//wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use[NUM_MULT-1:0] ;展开-->4
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_0;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_1;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_2;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use_unroll_3;
//wire signed [ACCUM_DATA_WIDTH-1:0]  result[ NUM_DSP -1:0] ;展开-->4
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_0;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_1;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_2;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_unroll_3;
	

//	
	output wire pool_done;
	wire pool_start;
//wire [DATA_WIDTH-1: 0] result_after_relu[NUM_MULT-1:0];	展开-->4
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_0;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_1;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_2;
	wire [DATA_WIDTH-1: 0] result_after_relu_unroll_3;


	//internal signals  weight_feature
	wire weight_rden_a ;
	wire weight_rden_b ;
   wire weight_wren_a ;
   wire weight_wren_b ;

	//weight ram 
//wire [DATA_WIDTH-1:0] weight_q_a[NUM_DSP-1:0];展开-->4
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_0;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_1;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_2;
	wire [DATA_WIDTH-1:0] weight_q_a_unroll_3;
//wire [DATA_WIDTH-1:0] weight_q_b[NUM_DSP-1:0];展开-->4
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_0;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_1;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_2;
	wire [DATA_WIDTH-1:0] weight_q_b_unroll_3;
 
//reg  [DATA_WIDTH-1: 0] in_feature_data_a [INPUT_NUM_MEM-1:0];展开-->1
 	reg  [DATA_WIDTH-1: 0] in_feature_data_a_unroll_0;
//reg  [DATA_WIDTH-1: 0] in_feature_data_b [INPUT_NUM_MEM-1:0];展开-->1
	reg  [DATA_WIDTH-1: 0] in_feature_data_b_unroll_0;
 
//reg  [DATA_WIDTH-1: 0] weight_data_a[ NUM_DSP-1:0] ;展开-->4
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_0;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_1;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_2;
 	reg  [DATA_WIDTH-1: 0] weight_data_a_unroll_3;
//reg  [DATA_WIDTH-1: 0] weight_data_b[ NUM_DSP-1:0] ;展开-->4
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_0;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_1;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_2;
	reg  [DATA_WIDTH-1: 0] weight_data_b_unroll_3;
 

	
	wire [WEIGHT_ADDR_WIDTH-1:0] weight_addra;
	wire [WEIGHT_ADDR_WIDTH-1:0] weight_addrb;
 
	//in feature ram
//wire [DATA_WIDTH-1: 0] in_feature_q_a[INPUT_NUM_MEM-1:0];展开-->1
   wire [DATA_WIDTH-1: 0] in_feature_q_a_unroll_0;
//wire [DATA_WIDTH-1: 0] in_feature_q_b[INPUT_NUM_MEM-1:0];展开-->1
	wire [DATA_WIDTH-1: 0] in_feature_q_b_unroll_0;
	
//wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use [NUM_DSP -1 :0];展开-->4
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_0;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_1;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_2;
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use_unroll_3;
//wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use [NUM_DSP -1 :0];	展开-->4
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_0;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_1;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_2;
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use_unroll_3;
	


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
//wire	[POOL_ADDR_WIDTH-1:0] address_a_t[NUM_MULT-1:0];展开-->4
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_0;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_1;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_2;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_unroll_3;
//wire	[POOL_ADDR_WIDTH-1:0] address_b_t[NUM_MULT-1:0];展开-->4
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_0;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_1;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_2;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_unroll_3;
//wire	[DATA_WIDTH-1: 0] data_a[NUM_MULT-1:0];展开-->4
	wire	[DATA_WIDTH-1: 0] data_a_unroll_0;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_1;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_2;
	wire	[DATA_WIDTH-1: 0] data_a_unroll_3;
//wire	[DATA_WIDTH-1: 0] data_b[NUM_MULT-1:0];展开-->4
	wire	[DATA_WIDTH-1: 0] data_b_unroll_0;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_1;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_2;
	wire	[DATA_WIDTH-1: 0] data_b_unroll_3;
//wire	 rden_a	[NUM_MULT-1:0];展开-->4
	wire	 rden_a_unroll_0;
	wire	 rden_a_unroll_1;
	wire	 rden_a_unroll_2;
	wire	 rden_a_unroll_3;
//wire	 rden_b	[NUM_MULT-1:0];展开-->4
	wire	 rden_b_unroll_0;
	wire	 rden_b_unroll_1;
	wire	 rden_b_unroll_2;
	wire	 rden_b_unroll_3;
//wire	 wren_a	[NUM_MULT-1:0];展开-->4
	wire	 wren_a_unroll_0;
	wire	 wren_a_unroll_1;
	wire	 wren_a_unroll_2;
	wire	 wren_a_unroll_3;
//wire	 wren_b	[NUM_MULT-1:0];展开-->4
	wire	 wren_b_unroll_0;
	wire	 wren_b_unroll_1;
	wire	 wren_b_unroll_2;
	wire	 wren_b_unroll_3;
//wire	[DATA_WIDTH-1: 0] q_a[NUM_MULT-1:0];展开-->4
	wire	[DATA_WIDTH-1: 0] q_a_unroll_0;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_1;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_2;
	wire	[DATA_WIDTH-1: 0] q_a_unroll_3;
//wire	[DATA_WIDTH-1: 0] q_b[NUM_MULT-1:0];展开-->4
	wire	[DATA_WIDTH-1: 0] q_b_unroll_0;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_1;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_2;
	wire	[DATA_WIDTH-1: 0] q_b_unroll_3;
//use
//wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use[NUM_MULT-1:0];展开-->4
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_0;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_1;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_2;
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use_unroll_3;
//wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use[NUM_MULT-1:0];展开-->4
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_0;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_1;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_2;
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use_unroll_3;

//wire	 rden_a_use	[NUM_MULT-1:0];展开-->4
	wire	 rden_a_use_unroll_0;
	wire	 rden_a_use_unroll_1;
	wire	 rden_a_use_unroll_2;
	wire	 rden_a_use_unroll_3;
//wire	 rden_b_use	[NUM_MULT-1:0];展开-->4
	wire	 rden_b_use_unroll_0;
	wire	 rden_b_use_unroll_1;
	wire	 rden_b_use_unroll_2;
	wire	 rden_b_use_unroll_3;
//wire	 wren_a_use	[NUM_MULT-1:0];展开-->4
	wire	 wren_a_use_unroll_0;
	wire	 wren_a_use_unroll_1;
	wire	 wren_a_use_unroll_2;
	wire	 wren_a_use_unroll_3;
//wire	 wren_b_use	[NUM_MULT-1:0];展开-->4
	wire	 wren_b_use_unroll_0;
	wire	 wren_b_use_unroll_1;
	wire	 wren_b_use_unroll_2;
	wire	 wren_b_use_unroll_3;
//**************************************** *************in feature map*******************
	wire [DATA_WIDTH*NUM_MULT-1:0]q_b_all;



assign address_a_t_out=address_a_t_unroll_0;
assign address_b_t_out=address_b_t_unroll_0;
assign rden_a_out=rden_a_unroll_0;
assign rden_b_out=rden_b_unroll_0;
assign wren_a_out=wren_a_unroll_0;
assign wren_b_out=wren_b_unroll_0;



	
//generate for开始------------------------------->
//wire_ur:4
//第0次展开：0/4
	
assign address_a_t_use_unroll_0=address_a_t_use_out;
assign address_b_t_use_unroll_0=address_b_t_use_out;
assign rden_a_use_unroll_0=rden_a_use_out;
assign rden_b_use_unroll_0=rden_b_use_out;
assign wren_a_use_unroll_0=wren_a_use_out;
assign wren_b_use_unroll_0=wren_b_use_out;	
		
//第1次展开：1/4
	
assign address_a_t_use_unroll_1=address_a_t_use_out;
assign address_b_t_use_unroll_1=address_b_t_use_out;
assign rden_a_use_unroll_1=rden_a_use_out;
assign rden_b_use_unroll_1=rden_b_use_out;
assign wren_a_use_unroll_1=wren_a_use_out;
assign wren_b_use_unroll_1=wren_b_use_out;	
		
//第2次展开：2/4
	
assign address_a_t_use_unroll_2=address_a_t_use_out;
assign address_b_t_use_unroll_2=address_b_t_use_out;
assign rden_a_use_unroll_2=rden_a_use_out;
assign rden_b_use_unroll_2=rden_b_use_out;
assign wren_a_use_unroll_2=wren_a_use_out;
assign wren_b_use_unroll_2=wren_b_use_out;	
		
//第3次展开：3/4
	
assign address_a_t_use_unroll_3=address_a_t_use_out;
assign address_b_t_use_unroll_3=address_b_t_use_out;
assign rden_a_use_unroll_3=rden_a_use_out;
assign rden_b_use_unroll_3=rden_b_use_out;
assign wren_a_use_unroll_3=wren_a_use_out;
assign wren_b_use_unroll_3=wren_b_use_out;	
		
	
//**********************************************	
	
	


control_param_1  control_param_inst(

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

addrgener_param_1  addrgener_param(
 	 .clk(clock),
    .enable(enable_addrger),
	 .reset(reset),	
	 .addra_temp(in_feature_addra),
	 .addrb_temp(in_feature_addrb)

 );
weight_addrgener_param_1 weight_addrgener_param_inst(
	.clk(clock),
	.enable(enable_weightaddrger),
	.reset(reset),	
	.addra(weight_addra),
	.addrb(weight_addrb)
 );
 
//generate for开始------------------------------->
//num:4
//第0次展开：0/4
  
  assign   in_feature_q_a_dsp_use_unroll_0 = in_feature_q_a_mux_all[DATA_WIDTH* (0%IFMAP_PAR+1)-1  : DATA_WIDTH* (0%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_0 = in_feature_q_b_mux_all[DATA_WIDTH* (0%IFMAP_PAR+1)-1  : DATA_WIDTH* (0%IFMAP_PAR)];
//第1次展开：1/4
  
  assign   in_feature_q_a_dsp_use_unroll_1 = in_feature_q_a_mux_all[DATA_WIDTH* (1%IFMAP_PAR+1)-1  : DATA_WIDTH* (1%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_1 = in_feature_q_b_mux_all[DATA_WIDTH* (1%IFMAP_PAR+1)-1  : DATA_WIDTH* (1%IFMAP_PAR)];
//第2次展开：2/4
  
  assign   in_feature_q_a_dsp_use_unroll_2 = in_feature_q_a_mux_all[DATA_WIDTH* (2%IFMAP_PAR+1)-1  : DATA_WIDTH* (2%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_2 = in_feature_q_b_mux_all[DATA_WIDTH* (2%IFMAP_PAR+1)-1  : DATA_WIDTH* (2%IFMAP_PAR)];
//第3次展开：3/4
  
  assign   in_feature_q_a_dsp_use_unroll_3 = in_feature_q_a_mux_all[DATA_WIDTH* (3%IFMAP_PAR+1)-1  : DATA_WIDTH* (3%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use_unroll_3 = in_feature_q_b_mux_all[DATA_WIDTH* (3%IFMAP_PAR+1)-1  : DATA_WIDTH* (3%IFMAP_PAR)];
 

 
//generate for开始------------------------------->
//dsp_count:4
//第0次展开：0/4

  
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
	www	weight_inst_0(
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
			
	
	
//第1次展开：1/4

  
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
	www	weight_inst_1(
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
			
	
	
//第2次展开：2/4

  
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
	www	weight_inst_2(
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
			
	
	
//第3次展开：3/4

  
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
	www	weight_inst_3(
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
			
	
	


//generate for开始------------------------------->
//num1:4
//第0次展开：0/4
  
  assign   result_relu_use_unroll_0 = result[0* IFMAP_PAR ];

//第1次展开：1/4
  
  assign   result_relu_use_unroll_1 = result[1* IFMAP_PAR ];

//第2次展开：2/4
  
  assign   result_relu_use_unroll_2 = result[2* IFMAP_PAR ];

//第3次展开：3/4
  
  assign   result_relu_use_unroll_3 = result[3* IFMAP_PAR ];




//generate for开始------------------------------->
//pool_relu_count:4
//第0次展开：0/4

  

	
	pool_1 pool_inst_0( 
   	
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
		
	pool_memo_1 pool_memo_inst_0(     
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


	relu_param_1 relu_param_0(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_0),
		.result_relu(result_after_relu_unroll_0)

);		
	
//第1次展开：1/4

  

	
	pool_1 pool_inst_1( 
   	
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
		
	pool_memo_1 pool_memo_inst_1(     
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


	relu_param_1 relu_param_1(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_1),
		.result_relu(result_after_relu_unroll_1)

);		
	
//第2次展开：2/4

  

	
	pool_1 pool_inst_2( 
   	
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
		
	pool_memo_1 pool_memo_inst_2(     
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


	relu_param_1 relu_param_2(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_2),
		.result_relu(result_after_relu_unroll_2)

);		
	
//第3次展开：3/4

  

	
	pool_1 pool_inst_3( 
   	
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
		
	pool_memo_1 pool_memo_inst_3(     
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


	relu_param_1 relu_param_3(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use_unroll_3),
		.result_relu(result_after_relu_unroll_3)

);		
	



//**********************************************/////
//generate for开始------------------------------->
//output_num:4
//第0次展开：0/4
  
  assign q_b_all_out[0* DATA_WIDTH + DATA_WIDTH - 1:0 * DATA_WIDTH]=q_b_unroll_0;
  assign q_a_all_out[0* DATA_WIDTH + DATA_WIDTH - 1:0 * DATA_WIDTH]=q_a_unroll_0;
//第1次展开：1/4
  
  assign q_b_all_out[1* DATA_WIDTH + DATA_WIDTH - 1:1 * DATA_WIDTH]=q_b_unroll_1;
  assign q_a_all_out[1* DATA_WIDTH + DATA_WIDTH - 1:1 * DATA_WIDTH]=q_a_unroll_1;
//第2次展开：2/4
  
  assign q_b_all_out[2* DATA_WIDTH + DATA_WIDTH - 1:2 * DATA_WIDTH]=q_b_unroll_2;
  assign q_a_all_out[2* DATA_WIDTH + DATA_WIDTH - 1:2 * DATA_WIDTH]=q_a_unroll_2;
//第3次展开：3/4
  
  assign q_b_all_out[3* DATA_WIDTH + DATA_WIDTH - 1:3 * DATA_WIDTH]=q_b_unroll_3;
  assign q_a_all_out[3* DATA_WIDTH + DATA_WIDTH - 1:3 * DATA_WIDTH]=q_a_unroll_3;
 

endmodule  


