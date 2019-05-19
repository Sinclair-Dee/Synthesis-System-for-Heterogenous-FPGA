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
parameter IFMAP_PAR= `IFMAP_PAR ;
parameter NUM_DSP =`NUM_DSP;
//******* ADD for resource parameter

parameter INPUT_NUM_MEM=`INPUT_NUM_MEM;	


parameter NUM_MULT=`NUM_MULT;

	//data width parameter		
parameter	ACCUM_DATA_WIDTH=`ACCUM_DATA_WIDTH;		
parameter	DATA_WIDTH=`DATA_WIDTH;		
// number of input output feature map

	

//addr width
   parameter	IN_FEATURE_ADDR_WIDTH=`IN_FEATURE_ADDR_WIDTH;		
parameter	WEIGHT_ADDR_WIDTH=`WEIGHT_ADDR_WIDTH;		
parameter	POOL_ADDR_WIDTH=`POOL_ADDR_WIDTH;		

	
parameter	COUNT_SLOAD_BITWIDTH=`COUNT_SLOAD_BITWIDTH;		
			
 
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
	
	
	wire signed [ACCUM_DATA_WIDTH-1:0]  result_relu_use[NUM_MULT-1:0] ;
	wire signed [ACCUM_DATA_WIDTH-1:0]  result[ NUM_DSP -1:0] ;
	

//	
	output wire pool_done;
	wire pool_start;
	wire [DATA_WIDTH-1: 0] result_after_relu[NUM_MULT-1:0];	


	//internal signals  weight_feature
	wire weight_rden_a ;
	wire weight_rden_b ;
   wire weight_wren_a ;
   wire weight_wren_b ;

	//weight ram 
	wire [DATA_WIDTH-1:0] weight_q_a[NUM_DSP-1:0];
	wire [DATA_WIDTH-1:0] weight_q_b[NUM_DSP-1:0];
 
 	reg  [DATA_WIDTH-1: 0] in_feature_data_a [INPUT_NUM_MEM-1:0];
	reg  [DATA_WIDTH-1: 0] in_feature_data_b [INPUT_NUM_MEM-1:0];
 
 	reg  [DATA_WIDTH-1: 0] weight_data_a[ NUM_DSP-1:0] ;
	reg  [DATA_WIDTH-1: 0] weight_data_b[ NUM_DSP-1:0] ;
 

	
	wire [WEIGHT_ADDR_WIDTH-1:0] weight_addra;
	wire [WEIGHT_ADDR_WIDTH-1:0] weight_addrb;
 
	//in feature ram
   wire [DATA_WIDTH-1: 0] in_feature_q_a[INPUT_NUM_MEM-1:0];
	wire [DATA_WIDTH-1: 0] in_feature_q_b[INPUT_NUM_MEM-1:0];
	
	wire [DATA_WIDTH-1:0]in_feature_q_a_dsp_use [NUM_DSP -1 :0];
	wire [DATA_WIDTH-1:0]in_feature_q_b_dsp_use [NUM_DSP -1 :0];	
	


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
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t[NUM_MULT-1:0];
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t[NUM_MULT-1:0];
	wire	[DATA_WIDTH-1: 0] data_a[NUM_MULT-1:0];
	wire	[DATA_WIDTH-1: 0] data_b[NUM_MULT-1:0];
	wire	 rden_a	[NUM_MULT-1:0];
	wire	 rden_b	[NUM_MULT-1:0];
	wire	 wren_a	[NUM_MULT-1:0];
	wire	 wren_b	[NUM_MULT-1:0];
	wire	[DATA_WIDTH-1: 0] q_a[NUM_MULT-1:0];
	wire	[DATA_WIDTH-1: 0] q_b[NUM_MULT-1:0];
//use
	wire	[POOL_ADDR_WIDTH-1:0] address_a_t_use[NUM_MULT-1:0];
	wire	[POOL_ADDR_WIDTH-1:0] address_b_t_use[NUM_MULT-1:0];

	wire	 rden_a_use	[NUM_MULT-1:0];
	wire	 rden_b_use	[NUM_MULT-1:0];
	wire	 wren_a_use	[NUM_MULT-1:0];
	wire	 wren_b_use	[NUM_MULT-1:0];
//**************************************** *************in feature map*******************
	wire [DATA_WIDTH*NUM_MULT-1:0]q_b_all;



assign address_a_t_out=address_a_t[0];
assign address_b_t_out=address_b_t[0];
assign rden_a_out=rden_a[0];
assign rden_b_out=rden_b[0];
assign wren_a_out=wren_a[0];
assign wren_b_out=wren_b[0];



	
genvar wire_ur;
generate
for (wire_ur = 0; wire_ur < NUM_MULT; wire_ur= wire_ur+1) begin : addrwrre
	
assign address_a_t_use[wire_ur]=address_a_t_use_out;
assign address_b_t_use[wire_ur]=address_b_t_use_out;
assign rden_a_use[wire_ur]=rden_a_use_out;
assign rden_b_use[wire_ur]=rden_b_use_out;
assign wren_a_use[wire_ur]=wren_a_use_out;
assign wren_b_use[wire_ur]=wren_b_use_out;	
		
end 
endgenerate	
	
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
 
genvar num;
generate
for (num = 0; num < NUM_DSP; num = num + 1) begin : ff
  
  assign   in_feature_q_a_dsp_use[num] = in_feature_q_a_mux_all[DATA_WIDTH* (num%IFMAP_PAR+1)-1  : DATA_WIDTH* (num%IFMAP_PAR)];
  assign   in_feature_q_b_dsp_use[num] = in_feature_q_b_mux_all[DATA_WIDTH* (num%IFMAP_PAR+1)-1  : DATA_WIDTH* (num%IFMAP_PAR)];
end // for

endgenerate  
 

 
genvar dsp_count;
generate
for (dsp_count = 0; dsp_count < NUM_DSP; dsp_count = dsp_count+1) begin : inst_dsp
  
// *************half DSP
	mult	mult_inst (
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_feature_q_a_dsp_use[dsp_count] ),
		.datab_0 ( weight_q_a[dsp_count]),
		.dataa_1 ( in_feature_q_b_dsp_use[dsp_count] ),
		.datab_1 ( weight_q_b[dsp_count]),
		.ena0 ( enable_mult ),
		.result ( result[dsp_count] ),
		.aclr3(clear_mult)
		);
	
// *************weight feature map
	weight_layer2	weight_inst (
		.address_a ( weight_addra),
		.address_b ( weight_addrb),
		.clock ( clock ),
		.data_a ( weight_data_a[dsp_count] ),
		.data_b ( weight_data_b[dsp_count] ),
		.rden_a ( weight_rden_a ),
		.rden_b ( weight_rden_b ),
		.wren_a ( weight_wren_a ),
		.wren_b ( weight_wren_b ),
		.q_a ( weight_q_a[dsp_count] ),
		.q_b ( weight_q_b[dsp_count] )
		);
			
	
	
end // for
endgenerate


genvar num1;
generate
for (num1 = 0; num1 < NUM_MULT; num1 = num1 + 1) begin : ffff
  
 assign   result_relu_use[num1] = result[num1* IFMAP_PAR+2] + result[num1* IFMAP_PAR+1] + result[num1* IFMAP_PAR ];

end // for

endgenerate  






genvar pool_relu_count;
generate
for (pool_relu_count = 0; pool_relu_count < NUM_MULT; pool_relu_count = pool_relu_count+1) begin : inst_mult
  

	
	pool_2 pool_inst( 
   	
		.clk(clock),
		.reset(reset),
	
		.conv_done(pool_done),
		.conv_start(pool_start),
	
		.data_in(result_after_relu[pool_relu_count]),
	 
		.data_a(data_a[pool_relu_count]),
		.data_b(data_b[pool_relu_count]),
		.rden_a(rden_a[pool_relu_count]),
		.rden_b(rden_b[pool_relu_count]),
		.wren_a(wren_a[pool_relu_count]),
		.wren_b(wren_b[pool_relu_count]),
		
		.address_a_t(address_a_t[pool_relu_count]),
		.address_b_t(address_b_t[pool_relu_count]),
		
		.q_a(q_a[pool_relu_count]),
		.q_b(q_b[pool_relu_count])
		);
		
	pool_memo_2 pool_memo_inst (     
		.address_a(address_a_t_use[pool_relu_count]),
		.address_b (address_b_t_use[pool_relu_count]),
		.clock(clock),
		.data_a(data_a[pool_relu_count]),
		.data_b(data_b[pool_relu_count]),
		.rden_a(rden_a_use[pool_relu_count]),
		.rden_b(rden_b_use[pool_relu_count]),
		.wren_a(wren_a_use[pool_relu_count]),
		.wren_b(wren_b_use[pool_relu_count]),
		.q_a(q_a[pool_relu_count]),
		.q_b(q_b[pool_relu_count]) 
		);	
/*	pool_memo pool_memo_inst (     
		.address_a(address_a_t[pool_relu_count]),
		.address_b (address_b_t[pool_relu_count]),
		.clock(clock),
		.data_a(data_a[pool_relu_count]),
		.data_b(data_b[pool_relu_count]),
		.rden_a(rden_a[pool_relu_count]),
		.rden_b(rden_b[pool_relu_count]),
		.wren_a(wren_a[pool_relu_count]),
		.wren_b(wren_b[pool_relu_count]),
		.q_a(q_a[pool_relu_count]),
		.q_b(q_b[pool_relu_count]) 
		);	*/


	relu_param_2 relu_param(
		.clock(clock),
		.enable(enable),
		.reset(reset),	
		.count_sload(count_sload),
	 
	 
		.result(result_relu_use[pool_relu_count]),
		.result_relu(result_after_relu[pool_relu_count])

);		
	
end // for
endgenerate



//**********************************************/////
genvar output_num;
generate
for (output_num = 0; output_num < NUM_MULT; output_num = output_num + 1) begin : q_b_relative
  
  assign q_b_all_out[output_num* DATA_WIDTH + DATA_WIDTH - 1:output_num * DATA_WIDTH]=q_b[output_num];
  assign q_a_all_out[output_num* DATA_WIDTH + DATA_WIDTH - 1:output_num * DATA_WIDTH]=q_a[output_num];
end // for

endgenerate  
 

endmodule  