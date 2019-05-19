`include "fc_param_2.vh"
module fc_layer_2 (
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
		finalresult

	 

		
	);	
parameter	FC_INNEURON_ADDR_WIDTH=`FC_INNEURON_ADDR_WIDTH;		
parameter	OUTNEURON=`OUTNEURON;
parameter	INNEURON=`INNEURON;
parameter 	PI=`PI;
parameter	PO=`PO;
			
parameter	ACCUM_DATA_WIDTH_FC=`ACCUM_DATA_WIDTH_FC;  	
parameter	DATA_WIDTH_FC=`DATA_WIDTH_FC; 	
// number of input output feature map

parameter	FC_WEIGHT_ADDR_WIDTH=`FC_WEIGHT_ADDR_WIDTH;   		
		
parameter	FC_OUTNEURON_ADDR_WIDTH=`FC_OUTNEURON_ADDR_WIDTH; 

	parameter FC_COUNT_OUT_NEURON_BITWIDTH	=`FC_COUNT_OUT_NEURON_BITWIDTH;
parameter	FC_COUNT_SLOAD_BITWIDTH=`FC_COUNT_SLOAD_BITWIDTH;  	 
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
output wire [44-1: 0] finalresult;
	wire [DATA_WIDTH_FC*PO-1: 0] fc_q_a_all_out;
	wire [DATA_WIDTH_FC*PO-1: 0] fc_q_b_all_out;
	//**************
//**********************
	wire [FC_COUNT_SLOAD_BITWIDTH-1:0] count_sload;

	wire signed [ACCUM_DATA_WIDTH_FC-1:0]  fc_result[ PO -1:0] ;
	wire signed [DATA_WIDTH_FC-1:0]  data[ PO	-1:0] ;	
	
	wire [FC_COUNT_OUT_NEURON_BITWIDTH-1:0] count_out;
//	
	output wire done;

	//internal signals  weight_neuron
	wire fc_weight_rden_a ;
	wire fc_weight_rden_b ;
   wire fc_weight_wren_a ;
   wire fc_weight_wren_b ;

	//weight ram 
	wire [DATA_WIDTH_FC-1:0] fc_weight_q_a[PO-1:0];
	wire [DATA_WIDTH_FC-1:0] fc_weight_q_b[PO-1:0];
 
 	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_a [PI-1:0];
	reg  [DATA_WIDTH_FC-1: 0] in_neuron_data_b [PI-1:0];
 
 	reg  [DATA_WIDTH_FC-1: 0] fc_weight_data_a[ PO-1:0] ;
	reg  [DATA_WIDTH_FC-1: 0] fc_weight_data_b[ PO-1:0] ;
 

	
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
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_a_t[PO-1:0];
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_b_t[PO-1:0];
	wire	[DATA_WIDTH_FC-1: 0] data_a[PO-1:0];
	wire	[DATA_WIDTH_FC-1: 0] data_b[PO-1:0];
	wire	 rden_a	[PO-1:0];
	wire	 rden_b	[PO-1:0];
	wire	 wren_a	[PO-1:0];
	wire	 wren_b	[PO-1:0];
	wire	[DATA_WIDTH_FC-1: 0] fc_q_a[PO-1:0];
	wire	[DATA_WIDTH_FC-1: 0] fc_q_b[PO-1:0];
//use
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_a_t_use[PO-1:0];
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] address_b_t_use[PO-1:0];

	wire	 rden_a_use	[PO-1:0];
	wire	 rden_b_use	[PO-1:0];
	wire	 wren_a_use	[PO-1:0];
	wire	 wren_b_use	[PO-1:0];
//**************************************** *************in neuron map*******************
	wire	[DATA_WIDTH_FC-1:0] data_write[PO-1:0];
	wire	[FC_OUTNEURON_ADDR_WIDTH-1:0] write_address[PO-1:0];


	wire fc_outneuron_rden_a[PO-1:0];
	wire fc_outneuron_rden_b[PO-1:0];
	wire fc_outneuron_wren_a[PO-1:0];
	wire fc_outneuron_wren_b[PO-1:0];
//**********************************************	
	
	 assign finalresult=fc_result[0];


fc_control_param_2  fc_control_param_inst(

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

fc_addrgener_param_2  fc_addrgener_param(
 	 .clk(clock),
    .enable(enable_fc_addrger),
	 .reset(reset),	
	 .addra(in_neuron_addra),
	 .addrb(in_neuron_addrb)

 );
fc_weight_addrgener_param_2 fc_weight_addrgener_param_inst(
	.clk(clock),
	.enable(enable_fc_weightaddrger),
	.reset(reset),	
	.addra(fc_weight_addra),
	.addrb(fc_weight_addrb)
 );
 

 

 
genvar po_num;
generate
for (po_num = 0; po_num < PO; po_num = po_num+1) begin : inst_dsp
  
// *************half DSP
	mult	mult_inst (
		.clock0 ( clock ),
		.accum_sload ( accum_sload ),
		.dataa_0 ( in_neuron_q_a_mux),
		.datab_0 ( fc_weight_q_a[po_num]),
		.dataa_1 ( in_neuron_q_b_mux ),
		.datab_1 ( fc_weight_q_b[po_num]),
		.ena0 ( enable_mult ),
		.result ( fc_result[po_num] ),
		.aclr3(clear_mult)
		);
	
// *************weight neuron map
	fc_weight_layer2	weight_inst (
		
		.address_a ( fc_weight_addra),
		.address_b ( fc_weight_addrb),
		.clock ( clock ),
		.data_a ( fc_weight_data_a[po_num] ),
		.data_b ( fc_weight_data_b[po_num] ),
		.rden_a ( fc_weight_rden_a ),
		.rden_b ( fc_weight_rden_b ),
		.wren_a ( fc_weight_wren_a ),
		.wren_b ( fc_weight_wren_b ),
		.q_a ( fc_weight_q_a[po_num] ),
		.q_b ( fc_weight_q_b[po_num] )
		);
		
	write_result_2 write_result(
	
	.clock(clock),

	.reset(reset),
	.count_sload(count_sload),
	.result(fc_result[po_num]),
	.count_out(count_out),
	.dataout(data_write[po_num]),
	.enable(enable_write),
	.address(write_address[po_num]),
	.fc_outneuron_wren_a(fc_outneuron_wren_a[po_num])
	)	;	
	
	outneuron_2	outneuron (
		.address_a ( write_address[po_num]),
		.address_b ( ),
		.clock ( clock ),
		.data_a ( data_write[po_num]),
		.data_b ( ),
		.rden_a ( fc_outneuron_rden_a[po_num] ),
		.rden_b ( fc_outneuron_rden_b[po_num] ),
		.wren_a ( fc_outneuron_wren_a[po_num]),
		.wren_b ( fc_outneuron_wren_b[po_num] ),
		.q_a ( fc_q_a[po_num] ),
		.q_b ( fc_q_b[po_num] )
		);
	
	
end // POr
endgenerate


	


//**********************************************/////
genvar output_num;
generate
for (output_num = 0; output_num < PO; output_num = output_num + 1) begin : q_b_relative
  
  assign fc_q_b_all_out[output_num* DATA_WIDTH_FC + DATA_WIDTH_FC - 1:output_num * DATA_WIDTH_FC]=fc_q_b[output_num];
  assign fc_q_a_all_out[output_num* DATA_WIDTH_FC + DATA_WIDTH_FC - 1:output_num * DATA_WIDTH_FC]=fc_q_a[output_num];
end // POr

endgenerate  
 

endmodule  