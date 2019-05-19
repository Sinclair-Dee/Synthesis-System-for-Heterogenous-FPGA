`include "facereco.vh"


module facereco(


clock,
enable,
reset,

final

);

parameter LAYER1_IN_FEATURE_ADDR_WIDTH=10;
parameter LAYER2_IN_FEATURE_ADDR_WIDTH=9;
parameter LAYER3_IN_FEATURE_ADDR_WIDTH=9;



parameter DATA_WIDTH=16;
parameter LAYER1_NUM_MULT=10;
parameter LAYER2_NUM_MULT=25;




parameter LAYER3_PO =4;



 output wire  [DATA_WIDTH-1: 0] final;

input clock;
input reset;
input enable;


wire layer1_enable;
wire layer1_reset;
wire layer1_pool_done;

wire layer2_enable;
wire layer2_reset;
wire layer2_pool_done;

wire layer3_enable;
wire layer3_reset;
wire layer3_pool_done;




//layer1


 
//communication with next layer
	 
	wire [LAYER2_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer1_address_a_t_out;
	wire [LAYER2_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer1_address_b_t_out;

	wire layer1_rden_a_out;
	wire layer1_rden_b_out;
	wire layer1_wren_a_out;
	wire layer1_wren_b_out;


//use
	wire [LAYER2_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer1_address_a_t_use_out;
	wire [LAYER2_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer1_address_b_t_use_out;

wire layer1_rden_a_use_out;
wire layer1_rden_b_use_out;
wire layer1_wren_a_use_out;
wire layer1_wren_b_use_out;
wire  [DATA_WIDTH*LAYER1_NUM_MULT-1: 0]layer1_2_q_a_all;
wire  [DATA_WIDTH*LAYER1_NUM_MULT-1: 0]layer1_2_q_b_all;

//*****************layer2*********************

	
	wire [LAYER2_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer2_in_feature_addra;
	wire [LAYER2_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer2_in_feature_addrb;
	
	 wire layer2_in_feature_rden_a;
	 wire layer2_in_feature_rden_b;
	 wire layer2_in_feature_wren_a;
	 wire layer2_in_feature_wren_b;
 
//communication with next layer
	 
	wire [LAYER3_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer2_address_a_t_out;
	wire [LAYER3_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer2_address_b_t_out;

	wire layer2_rden_a_out;
	wire layer2_rden_b_out;
	wire layer2_wren_a_out;
	wire layer2_wren_b_out;


//use
	wire [LAYER3_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer2_address_a_t_use_out;
	wire [LAYER3_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer2_address_b_t_use_out;

wire layer2_rden_a_use_out;
wire layer2_rden_b_use_out;
wire layer2_wren_a_use_out;
wire layer2_wren_b_use_out;
wire  [DATA_WIDTH*LAYER2_NUM_MULT-1: 0]layer2_3_q_a_all;
wire  [DATA_WIDTH*LAYER2_NUM_MULT-1: 0]layer2_3_q_b_all;


//******************layer3***************

	
	wire [LAYER3_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer3_in_neuron_addra;
	wire [LAYER3_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer3_in_neuron_addrb;
	
	 wire layer3_in_neuron_rden_a;
	 wire layer3_in_neuron_rden_b;
	 wire layer3_in_neuron_wren_a;
	 wire layer3_in_neuron_wren_b;

//////////////////////////////////////////

	

 
	



	
 	reg  [DATA_WIDTH-1: 0] in_feature_data_a ;
	reg  [DATA_WIDTH-1: 0] in_feature_data_b ;
	/////////////
 	wire  [DATA_WIDTH-1: 0]layer1_in_feature_q_a_all;
	wire  [DATA_WIDTH-1: 0]layer1_in_feature_q_b_all;
	
wire [LAYER1_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer1_in_feature_addra;
wire [LAYER1_IN_FEATURE_ADDR_WIDTH - 1 : 0]layer1_in_feature_addrb;
	
wire layer1_in_feature_rden_a;
wire layer1_in_feature_rden_b;
wire layer1_in_feature_wren_a;
wire layer1_in_feature_wren_b;
/////////////////////////


	

	in_feature	in_feature_inst (
		.address_a (layer1_in_feature_addra),
		.address_b (layer1_in_feature_addrb),
		.clock ( clock ),
		.data_a ( in_feature_data_a ),
		.data_b ( in_feature_data_b ),
		.rden_a ( layer1_in_feature_rden_a ),
		.rden_b ( layer1_in_feature_rden_b ),
		.wren_a ( layer1_in_feature_wren_a ),
		.wren_b ( layer1_in_feature_wren_b ),
		.q_a ( layer1_in_feature_q_a_all ),
		.q_b ( layer1_in_feature_q_b_all )
		);	
		
			
 
	 
//layer4	 

///////////*********layer1***********/////////////
layer_1 layer1(
// Global signals
	.clock(clock),
	.enable(layer1_enable),
	.reset(layer1_reset),
	.pool_done(layer1_pool_done),
//	communication with former layer
	.in_feature_q_a_all(layer1_in_feature_q_a_all),
	.in_feature_q_b_all(layer1_in_feature_q_b_all),
	
	.in_feature_addra(layer1_in_feature_addra),
	.in_feature_addrb(layer1_in_feature_addrb),
	
	 .in_feature_rden_a (layer1_in_feature_rden_a),
	 .in_feature_rden_b (layer1_in_feature_rden_b),
	 .in_feature_wren_a (layer1_in_feature_wren_a),
	 .in_feature_wren_b (layer1_in_feature_wren_b),
 
//communication with next layer
	 
	.address_a_t_out(layer1_address_a_t_out),
	.address_b_t_out(layer1_address_b_t_out),

	.rden_a_out	(layer1_rden_a_out),
	.rden_b_out	(layer1_rden_b_out),
	.wren_a_out	(layer1_wren_a_out),
	.wren_b_out	(layer1_wren_b_out),


//use
	.address_a_t_use_out(layer1_address_a_t_use_out),
	.address_b_t_use_out(layer1_address_b_t_use_out),

	.rden_a_use_out	(layer1_rden_a_use_out),
	.rden_b_use_out	(layer1_rden_b_use_out),
	.wren_a_use_out	(layer1_wren_a_use_out),
	.wren_b_use_out	(layer1_wren_b_use_out),

	.q_a_all_out (layer1_2_q_a_all),
	.q_b_all_out (layer1_2_q_b_all)
	);	
//****************layer12control********************/
layer_inter_1_2_control layer1_2_control(

		 .enable(enable),
		 .reset(reset),
		 .clock(clock),

	//layer_former 2 enable reset signals
		  .layer_former_enable(layer1_enable),
		  .layer_former_reset(layer1_reset),	
		
		
		 . layer_next_enable(layer2_enable),
		  .layer_next_reset(layer2_reset),	

		 .layer_former_done(layer1_pool_done),
		 .layer_next_done(layer2_pool_done),
   //layer_former read write address signal
		 . rden_a_layer_former(layer1_rden_a_out),
		  .rden_b_layer_former(layer1_rden_b_out),
		 . wren_a_layer_former(layer1_wren_a_out),
		  .wren_b_layer_former(layer1_wren_b_out),

			.address_a_layer_former(layer1_address_a_t_out),
			.address_b_layer_former(layer1_address_b_t_out),
	
	
		  .rden_a_former_after_mux(layer1_rden_a_use_out),
		 . rden_b_former_after_mux(layer1_rden_b_use_out),
		  .wren_a_former_after_mux(layer1_wren_a_use_out),
		 . wren_b_former_after_mux(layer1_wren_b_use_out),
		

	
			.address_a_former_after_mux(layer1_address_a_t_use_out),
			.address_b_former_after_mux(layer1_address_b_t_use_out),
		
//layer_next 	read write address signal	
			.address_a_layer_next (layer2_in_feature_addra),
			.address_b_layer_next (layer2_in_feature_addrb),


		 .rden_a_layer_next  (layer2_in_feature_rden_a) ,
		. rden_b_layer_next  (layer2_in_feature_rden_b),
		. wren_a_layer_next  (layer2_in_feature_wren_a),
		 .wren_b_layer_next  (layer2_in_feature_wren_b),

//		
		.layer_nextnext_done(layer3_pool_done)
	


);	
	////////****************layer2******************
	
layer_2 layer2 (
// Global signals
	.clock(clock),
	.enable(layer2_enable),
	.reset(layer2_reset),
	.pool_done(layer2_pool_done),
//	communication with former layer
	.in_feature_q_a_all(layer1_2_q_a_all),
	.in_feature_q_b_all(layer1_2_q_b_all),
	
	.in_feature_addra(layer2_in_feature_addra),
	.in_feature_addrb(layer2_in_feature_addrb),
	
	 .in_feature_rden_a (layer2_in_feature_rden_a),
	 .in_feature_rden_b (layer2_in_feature_rden_b),
	 .in_feature_wren_a (layer2_in_feature_wren_a),
	 .in_feature_wren_b (layer2_in_feature_wren_b),
 
//communication with next layer
	 
	.address_a_t_out(layer2_address_a_t_out),
	.address_b_t_out(layer2_address_b_t_out),

	.rden_a_out	(layer2_rden_a_out),
	.rden_b_out	(layer2_rden_b_out),
	.wren_a_out	(layer2_wren_a_out),
	.wren_b_out	(layer2_wren_b_out),



//use
	.address_a_t_use_out(layer2_address_a_t_use_out),
	.address_b_t_use_out(layer2_address_b_t_use_out),

	.rden_a_use_out	(layer2_rden_a_use_out),
	.rden_b_use_out	(layer2_rden_b_use_out),
	.wren_a_use_out	(layer2_wren_a_use_out),
	.wren_b_use_out	(layer2_wren_b_use_out),

	.q_a_all_out (layer2_3_q_a_all),
	.q_b_all_out (layer2_3_q_b_all)
	);	
//////////**************layer23control*********************//
layer_inter_final_control layer2_3_control(

		 .enable(enable),
		 .reset(reset),
		 .clock(clock),

	//layer_former 2 enable reset signals
		  .layer_former_enable(layer2_enable),
		  .layer_former_reset(layer2_reset),	
		
		
		  .layer_next_enable(layer3_enable),
		  .layer_next_reset(layer3_reset),	

		 .layer_former_done(layer2_pool_done),
		 .layer_next_done(layer3_pool_done),
 	 . rden_a_layer_former(layer2_rden_a_out),
		  .rden_b_layer_former(layer2_rden_b_out),
		 . wren_a_layer_former(layer2_wren_a_out),
		  .wren_b_layer_former(layer2_wren_b_out),

			.address_a_layer_former(layer2_address_a_t_out),
			.address_b_layer_former(layer2_address_b_t_out),
	
	
		  .rden_a_former_after_mux(layer2_rden_a_use_out),
		 . rden_b_former_after_mux(layer2_rden_b_use_out),
		  .wren_a_former_after_mux(layer2_wren_a_use_out),
		 . wren_b_former_after_mux(layer2_wren_b_use_out),
		

	
			.address_a_former_after_mux(layer2_address_a_t_use_out),
			.address_b_former_after_mux(layer2_address_b_t_use_out),
		
//layer_next 	read write address signal	
			.address_a_layer_next (layer3_in_neuron_addra),
			.address_b_layer_next (layer3_in_neuron_addrb),


		 .rden_a_layer_next  (layer3_in_neuron_rden_a) ,
		. rden_b_layer_next  (layer3_in_neuron_rden_b),
		. wren_a_layer_next  (layer3_in_neuron_wren_a),
		 .wren_b_layer_next  (layer3_in_neuron_wren_b)


//		

	


);	

/////////////////////////////////

	
fc_layer_1 layer_3(
// Global signals
	.clock(clock),
	.enable(layer3_enable),
	.reset(layer3_reset),
	.done(layer3_pool_done),
//	communication with POrmer layer
	.in_neuron_q_a_all(layer2_3_q_a_all),
	.in_neuron_q_b_all(layer2_3_q_b_all),
	
	.in_neuron_addra(layer3_in_neuron_addra),
	.in_neuron_addrb(layer3_in_neuron_addrb),
	
	 .in_neuron_rden_a(layer3_in_neuron_rden_a) ,
	 .in_neuron_rden_b(layer3_in_neuron_rden_b) ,
	 .in_neuron_wren_a(layer3_in_neuron_wren_a) ,
	 .in_neuron_wren_b(layer3_in_neuron_wren_b) ,
	.final(final)

	 

		
	);	
	


endmodule 