`include "param_1.vh"
module control_param_1(

// Global signals
	clock,
	enable,
	reset,	
   
	enable_addrger,
	enable_weightaddrger,
	
//in feature
	in_feature_rden_a,
	in_feature_rden_b,
	in_feature_wren_a,
	in_feature_wren_b,
	
//mult
	accum_sload,
	enable_mult,
	clear_mult,
	
// weight feature	
	weight_rden_a ,
   weight_rden_b ,
	weight_wren_a ,
	weight_wren_b ,
	
	in_feature_q_a_mux_all,
	in_feature_q_b_mux_all,
	
	
	in_feature_q_a_all,
	in_feature_q_b_all,
	
	
	start,
	conv_done,
	count_sload

	);	
//******* ADD for resource parameter


parameter INPUT_NUM_MEM=1;	

parameter NUM_ONEMULT=1;//ONE MULT COMPUTE HOW MUCH OUT FEATURE MAP
parameter OUT_FEATURE_WIDTH_POOL=16;
parameter IN_FEATURE_WIDTH =36;
	//data width parameter		

parameter	DATA_WIDTH=16;		
// number of input output feature map
		
// input output feature map size ,stride of conv	
parameter	OUT_FEATURE_WIDTH=32;	

//addr width	

//	mainly control parameter
parameter	NUM_ONE_PIXEL_CYCLE_INTER	=	13;
parameter	NUM_ONE_PIXEL_CYCLE	=	13;
	
parameter	COUNT_SLOAD_BITWIDTH=4;		
parameter	COUNT_CONV_DONE_BITWIDTH=11;			
	
parameter IFMAP_PAR =1;
parameter NUM_DSP =4;
	
// input signal 
 	input wire clock;
	input wire enable;
	input wire reset;


	input wire [DATA_WIDTH*INPUT_NUM_MEM-1: 0] in_feature_q_a_all;
	input wire [DATA_WIDTH*INPUT_NUM_MEM-1: 0] in_feature_q_b_all;	
	
// output signal
	output reg   in_feature_rden_a ;
	output reg   in_feature_rden_b ;
	output reg   in_feature_wren_a ;
	output reg   in_feature_wren_b ;


	output reg weight_rden_a ;
	output reg weight_rden_b ;
   output reg weight_wren_a ;
   output reg weight_wren_b ;

	output reg enable_addrger;
	output reg enable_weightaddrger;
	
// mult
	output wire accum_sload ;
	output reg enable_mult ;
	output reg clear_mult;
		

	
//signals for start done
	output reg start;
	output reg conv_done;
	output reg [COUNT_SLOAD_BITWIDTH-1:0] count_sload;		

 	output  reg [DATA_WIDTH*IFMAP_PAR-1: 0] in_feature_q_a_mux_all;
 	output  reg [DATA_WIDTH*IFMAP_PAR-1: 0] in_feature_q_b_mux_all;	
//internal signal
//wire [DATA_WIDTH-1: 0] in_feature_q_a [INPUT_NUM_MEM-1:0];展开-->1
 	wire [DATA_WIDTH-1: 0] in_feature_q_a_unroll_0;
//wire [DATA_WIDTH-1: 0] in_feature_q_b[INPUT_NUM_MEM-1:0];展开-->1
	wire [DATA_WIDTH-1: 0] in_feature_q_b_unroll_0;




	
	reg enable_in_weight;
	reg enable_delay;
	reg enable_delay1;
	reg enable_delay2;
	reg enable_delay3;

	reg enable_sload;



	reg [COUNT_CONV_DONE_BITWIDTH-1:0] count_conv_done;
	

	
//generate for开始------------------------------->
//input_num_mem:1
//第0次展开：0/1
  
  assign in_feature_q_a_unroll_0=in_feature_q_a_all[0*DATA_WIDTH+DATA_WIDTH-1:0*DATA_WIDTH];
  assign in_feature_q_b_unroll_0=in_feature_q_b_all[0*DATA_WIDTH+DATA_WIDTH-1:0*DATA_WIDTH];

 




	
	assign accum_sload=(count_sload==0)?1:0;


//enable 
always @ (posedge clock or posedge reset)
begin
	if(reset) 
		begin
			enable_addrger			<= 0;
			enable_weightaddrger <= 0;
			enable_mult	<= 0;
		end
	else if(enable)
			begin
				enable_addrger			<= 1;
				enable_weightaddrger <= 1;		     
				enable_mult	<= 1;			 
			end
end

//enable 
always @(posedge clock )
	begin
		if(reset)
			begin
    
				enable_in_weight <= 0;
				enable_delay1 <= 0;
				enable_delay2 <= 0;
				enable_delay3 <= 0;

				enable_sload  <= 0;

			end
		else
			begin
    
				enable_in_weight<=enable;
				enable_delay1<=enable;
				enable_delay2<=enable_delay1;
				enable_delay3<=enable_delay2;

				enable_sload <=enable_delay3;

			end
end
	 
//read write signals for M9K
always @(posedge clock or posedge reset)
	if(reset)
		begin
			in_feature_rden_a<=0;
			in_feature_rden_b<=0;
			in_feature_wren_a<=0;
			in_feature_wren_b<=0;
	
			weight_rden_a<=0;
			weight_rden_b<=0;
			weight_wren_a<=0;
			weight_wren_b<=0;
	
		end
	else if(enable_in_weight) 
			begin
			in_feature_rden_a<=1;
			in_feature_rden_b<=1;
			in_feature_wren_a<=0;
			in_feature_wren_b<=0;
	
			weight_rden_a<=1;
			weight_rden_b<=1;
			weight_wren_a<=0;
			weight_wren_b<=0;	
			end

			
//		 
always @ (posedge clock or posedge reset)
begin
	   
	if(reset) 
		count_sload <= 0;
	else if (enable_sload&&count_sload < NUM_ONE_PIXEL_CYCLE-1)
			count_sload <= count_sload + 1;
			else count_sload <= 0;						 
end

//璁＄畻start done淇″彿鎵€闇€瑕佺殑閫昏緫
always @ (posedge clock or posedge reset)
begin
	   
	if(reset) 
		count_conv_done <= 0;
	else if (enable && count_sload == NUM_ONE_PIXEL_CYCLE-1)
			count_conv_done <= count_conv_done + 1;
end	

always @ (posedge clock or posedge reset)
begin
	   
		if(reset) 
		  		  conv_done <= 0;
			else if ((count_conv_done==(OUT_FEATURE_WIDTH*OUT_FEATURE_WIDTH*NUM_ONEMULT+1))&&(count_sload==2))
			  	  conv_done <= 1;
				  
end		
			 
always @ (posedge clock or posedge reset)
begin
	   
		if(reset) 
		  		  start <= 0;
			else if (count_conv_done==1)
					start <= 1;				 
end	
 				
always@(*)
begin
	if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER)
		begin 

			in_feature_q_a_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_a_unroll_0;
		end

	else in_feature_q_a_mux_all<=0;
end


always@(*)
begin
	if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER-1)

			in_feature_q_b_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_b_unroll_0;

	else in_feature_q_b_mux_all<=1;
end
			 
endmodule			 


