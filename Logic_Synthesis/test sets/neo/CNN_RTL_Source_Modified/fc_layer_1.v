`include "fc_param_1.vh"
module fc_layer_1 (
// Global signals
	clock,
	enable,
	reset,
	done,
//	communication with POrmer layer
	in_neuron_q_a_all,
	in_neuron_q_b_all,
	
	in_neuron_addra,
	in_neuron_addrb,
	
	 in_neuron_rden_a ,
	 in_neuron_rden_b ,
	 in_neuron_wren_a ,
	 in_neuron_wren_b ,
	final

	 

		
	);	
parameter	FC_INNEURON_ADDR_WIDTH=9;		
parameter	OUTNEURON=10;
parameter	INNEURON=576;
parameter 	PI=16;
parameter	PO=1;
			
parameter	ACCUM_DATA_WIDTH_FC=44;  	
parameter	DATA_WIDTH_FC=16; 	
// number of input output feature map

parameter	FC_WEIGHT_ADDR_WIDTH=13;   		
		
parameter	FC_OUTNEURON_ADDR_WIDTH=9; 

	parameter FC_COUNT_OUT_NEURON_BITWIDTH	=4;
parameter	FC_COUNT_SLOAD_BITWIDTH=9;  	 
 //global signals
	input wire clock;
	input wire enable;
	input wire reset;

input	wire [DATA_WIDTH_FC*PI-1: 0] in_neuron_q_a_all;
input	wire [DATA_WIDTH_FC*PI-1: 0] in_neuron_q_b_all;
	
output	wire [FC_INNEURON_ADDR_WIDTH - 1 : 0] in_neuron_addra;
output	wire [FC_INNEURON_ADDR_WIDTH - 1 : 0] in_neuron_addrb;
	
output	wire   in_neuron_rden_a ;
output	wire   in_neuron_rden_b ;
output	wire   in_neuron_wren_a ;
output	wire   in_neuron_wren_b ;
output wire [DATA_WIDTH_FC-1: 0] final;	
	wire [DATA_WIDTH_FC*PO-1: 0] fc_q_a_all_out;
	wire [DATA_WIDTH_FC*PO-1: 0] fc_q_b_all_out;
	//**************
//**********************
	wire [FC_COUNT_SLOAD_BITWIDTH-1:0] count_sload;

//wire signed [ACCUM_DATA_WIDTH_FC-1:0]  fc_result[ PO -1:0] ;展开-->1
	wire signed [ACCUM_DATA_WIDTH_FC-1:0]  fc_result_unroll_0;
//wire signed [DATA_WIDTH_FC-1:0]  data[ PO	-1:0] ;	展开-->1
	wire signed [DATA_WIDTH_FC-1:0]  data_unroll_0;

	wire [FC_COUNT_OUT_NEURON_BITWIDTH-1:0] count_out;

	output wire done;

	//internal signals  weight_neuron
	wire fc_weight_rden_a ;
	wire fc_weight_rden_b ;
   wire fc_weight_wren_a ;
   wire fc_weight_wren_b ;

	//weight ram 
//wire [DATA_WIDTH_FC-1:0] fc_weight_q_a[PO-1:0];展开-->1
	wire [DATA_WIDTH_FC-1:0] fc_weight_q_a_unroll_0;
//wire [DATA_WIDTH_FC-1:0] fc_weight_q_b[PO-1:0];展开-->1
	wire [DATA_WIDTH_FC-1:0] fc_weight_q_b_unroll_0;
 
//reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a [PI-1:0];展开-->16
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_0;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_1;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_2;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_3;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_4;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_5;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_6;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_7;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_8;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_9;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_10;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_11;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_12;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_13;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_14;
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a_unroll_15;
//reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b [PI-1:0];展开-->16
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_0;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_1;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_2;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_3;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_4;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_5;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_6;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_7;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_8;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_9;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_10;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_11;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_12;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_13;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_14;
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b_unroll_15;
 
//reg  [DATA_WIDTH_FC-1: 0] fc_weight_data_a[ PO-1:0] ;展开-->1
 	reg  [DATA_WIDTH_FC-1: 0] fc_weight_data_a_unroll_0;
//reg  [DATA_WIDTH_FC-1: 0] fc_weight_data_b[ PO-1:0] ;展开-->1
	reg  [DATA_WIDTH_FC-1: 0] fc_weight_data_b_unroll_0;
 

	
	wire [FC_WEIGHT_ADDR_WIDTH-1:0] fc_weight_addra;
	wire [FC_WEIGHT_ADDR_WIDTH-1:0] fc_weight_addrb;
 
	//in neuron ram

	


	wire enable_fc_addrger ;
	wire accum_sload;
	wire enable_fc_weightaddrger;
	wire enable_mult;
	wire clear_mult;
	wire enable_write;
//*****out reda write address
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_a_t_out;
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_b_t_out;

	wire	 rden_a_out;
	wire	 rden_b_out;
	wire	 wren_a_out;
	wire	 wren_b_out;

//use
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_a_t_use_out;
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_b_t_use_out;

	wire	 rden_a_use_out;
	wire	 rden_b_use_out;
	wire	 wren_a_use_out;
	wire	 wren_b_use_out;

//*********	
 	wire  [DATA_WIDTH_FC-1: 0] in_neuron_q_a_mux;
 	wire  [DATA_WIDTH_FC-1: 0] in_neuron_q_b_mux;	


//***********
//wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_a_t[PO-1:0];展开-->1
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_a_t_unroll_0;
//wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_b_t[PO-1:0];展开-->1
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_b_t_unroll_0;
//wire	[DATA_WIDTH_FC-1: 0] data_a[PO-1:0];展开-->1
	wire	[DATA_WIDTH_FC-1: 0] data_a_unroll_0;
//wire	[DATA_WIDTH_FC-1: 0] data_b[PO-1:0];展开-->1
	wire	[DATA_WIDTH_FC-1: 0] data_b_unroll_0;
//wire	 rden_a	[PO-1:0];展开-->1
	wire	 rden_a_unroll_0;
//wire	 rden_b	[PO-1:0];展开-->1
	wire	 rden_b_unroll_0;
//wire	 wren_a	[PO-1:0];展开-->1
	wire	 wren_a_unroll_0;
//wire	 wren_b	[PO-1:0];展开-->1
	wire	 wren_b_unroll_0;
//wire	[DATA_WIDTH_FC-1: 0] fc_q_a[PO-1:0];展开-->1
	wire	[DATA_WIDTH_FC-1: 0] fc_q_a_unroll_0;
//wire	[DATA_WIDTH_FC-1: 0] fc_q_b[PO-1:0];展开-->1
	wire	[DATA_WIDTH_FC-1: 0] fc_q_b_unroll_0;
//use
//wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_a_t_use[PO-1:0];展开-->1
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_a_t_use_unroll_0;
//wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_b_t_use[PO-1:0];展开-->1
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_b_t_use_unroll_0;

//wire	 rden_a_use	[PO-1:0];展开-->1
	wire	 rden_a_use_unroll_0;
//wire	 rden_b_use	[PO-1:0];展开-->1
	wire	 rden_b_use_unroll_0;
//wire	 wren_a_use	[PO-1:0];展开-->1
	wire	 wren_a_use_unroll_0;
//wire	 wren_b_use	[PO-1:0];展开-->1
	wire	 wren_b_use_unroll_0;
//**************************************** *************in neuron map*******************
//wire	[DATA_WIDTH_FC-1:0] data_write[PO-1:0];展开-->1
	wire	[DATA_WIDTH_FC-1:0] data_write_unroll_0;
//wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] write_address[PO-1:0];展开-->1
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] write_address_unroll_0;


//wire fc_outneuron_rden_a[PO-1:0];展开-->1
	wire fc_outneuron_rden_a_unroll_0;
//wire fc_outneuron_rden_b[PO-1:0];展开-->1
	wire fc_outneuron_rden_b_unroll_0;
//wire fc_outneuron_wren_a[PO-1:0];展开-->1
	wire fc_outneuron_wren_a_unroll_0;
//wire fc_outneuron_wren_b[PO-1:0];展开-->1
	wire fc_outneuron_wren_b_unroll_0;
//**********************************************	
		assign final=data_write_unroll_0;
	


fc_control_param_1  fc_control_param_inst(

	.clock(clock),
	.enable(enable),
	.reset(reset),	
   
	.enable_fc_addrger(enable_fc_addrger),
	.enable_fc_weightaddrger(enable_fc_weightaddrger),
	.enable_write(enable_write),
	//in neuron
	.in_neuron_rden_a(in_neuron_rden_a),
	.in_neuron_rden_b(in_neuron_rden_b),
	.in_neuron_wren_a(in_neuron_wren_a),
	.in_neuron_wren_b(in_neuron_wren_b),
	//mult
	.accum_sload(accum_sload),
	.enable_mult(enable_mult),
	.clear_mult(clear_mult),
	.count_sload(count_sload),
	// weight neuron	
	.fc_weight_rden_a(fc_weight_rden_a) ,
   .fc_weight_rden_b(fc_weight_rden_b) ,
	.fc_weight_wren_a(fc_weight_wren_a) ,
	.fc_weight_wren_b(fc_weight_wren_b) ,
	.in_neuron_q_a_mux(in_neuron_q_a_mux) ,
   .in_neuron_q_b_mux(in_neuron_q_b_mux) ,
	
   .in_neuron_q_a_all(in_neuron_q_a_all),
	.in_neuron_q_b_all(in_neuron_q_b_all),
	.done(done),
	.count_out(	count_out)
		);

fc_addrgener_param_1  fc_addrgener_param(
 	 .clk(clock),
    .enable(enable_fc_addrger),
	 .reset(reset),	
	 .addra(in_neuron_addra),
	 .addrb(in_neuron_addrb)

 );
fc_weight_addrgener_param_1 fc_weight_addrgener_param_inst(
	.clk(clock),
	.enable(enable_fc_weightaddrger),
	.reset(reset),	
	.addra(fc_weight_addra),
	.addrb(fc_weight_addrb)
 );
 

 

 
//generate for开始------------------------------->
//po_num:1
//第0次展开：0/1
  
// *************half DSP
	mult	mult_inst_0(
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_neuron_q_a_mux),
		.datab_0 ( fc_weight_q_a_unroll_0),
		.dataa_1 ( in_neuron_q_b_mux ),
		.datab_1 ( fc_weight_q_b_unroll_0),
		.ena0 ( enable_mult ),
		.result ( fc_result_unroll_0 ),
		.aclr3(clear_mult)
		);
	
// *************weight neuron map
	fc_weight_layer1	weight_inst_0(
		
		.address_a ( fc_weight_addra),
		.address_b ( fc_weight_addrb),
		.clock ( clock ),
		.data_a ( fc_weight_data_a_unroll_0 ),
		.data_b ( fc_weight_data_b_unroll_0 ),
		.rden_a ( fc_weight_rden_a ),
		.rden_b ( fc_weight_rden_b ),
		.wren_a ( fc_weight_wren_a ),
		.wren_b ( fc_weight_wren_b ),
		.q_a ( fc_weight_q_a_unroll_0 ),
		.q_b ( fc_weight_q_b_unroll_0 )
		);
		
	write_result_1 write_result_0(
	
	.clock(clock),

	.reset(reset),
	.count_sload(count_sload),
	.result(fc_result_unroll_0),
	.count_out(count_out),
	.dataout(data_write_unroll_0),
	.enable(enable_write),
	.address(write_address_unroll_0),
	.fc_outneuron_wren_a(fc_outneuron_wren_a_unroll_0)
	)	;	
	
	outneuron_1	outneuron_0(
		.address_a ( write_address_unroll_0),
		.address_b ( ),
		.clock ( clock ),
		.data_a ( data_write_unroll_0),
		.data_b ( ),
		.rden_a ( fc_outneuron_rden_a_unroll_0 ),
		.rden_b ( fc_outneuron_rden_b_unroll_0 ),
		.wren_a ( fc_outneuron_wren_a_unroll_0),
		.wren_b ( fc_outneuron_wren_b_unroll_0 ),
		.q_a ( fc_q_a_unroll_0 ),
		.q_b ( fc_q_b_unroll_0 )
		);
	
	


	


//**********************************************/////
//generate for开始------------------------------->
//output_num:1
//第0次展开：0/1
  
  assign fc_q_b_all_out[0* DATA_WIDTH_FC + DATA_WIDTH_FC - 1:0 * DATA_WIDTH_FC]=fc_q_b_unroll_0;
  assign fc_q_a_all_out[0* DATA_WIDTH_FC + DATA_WIDTH_FC - 1:0 * DATA_WIDTH_FC]=fc_q_a_unroll_0;
 

endmodule  


