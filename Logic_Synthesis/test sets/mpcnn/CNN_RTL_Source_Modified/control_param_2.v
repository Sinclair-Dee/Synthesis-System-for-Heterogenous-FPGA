`include "param_2.vh"
module control_param_2(

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


parameter INPUT_NUM_MEM=`INPUT_NUM_MEM;	

parameter NUM_ONEMULT=`NUM_ONEMULT;//ONE MULT COMPUTE HOW MUCH OUT FEATURE MAP
parameter OUT_FEATURE_WIDTH_POOL=`OUT_FEATURE_WIDTH_POOL;
parameter IN_FEATURE_WIDTH =`IN_FEATURE_WIDTH;
	//data width parameter		

parameter	DATA_WIDTH=`DATA_WIDTH;		
// number of input output feature map
		
// input output feature map size ,stride of conv	
parameter	OUT_FEATURE_WIDTH=`OUT_FEATURE_WIDTH;	

//addr width	

//	mainly control parameter
parameter	NUM_ONE_PIXEL_CYCLE_INTER	=	`NUM_ONE_PIXEL_CYCLE_INTER;
parameter	NUM_ONE_PIXEL_CYCLE	=	`NUM_ONE_PIXEL_CYCLE;
	
parameter	COUNT_SLOAD_BITWIDTH=`COUNT_SLOAD_BITWIDTH;		
parameter	COUNT_CONV_DONE_BITWIDTH=`COUNT_CONV_DONE_BITWIDTH;			
	
parameter IFMAP_PAR =`IFMAP_PAR;
parameter NUM_DSP =`NUM_DSP;
	
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
 	wire [DATA_WIDTH-1: 0] in_feature_q_a [INPUT_NUM_MEM-1:0];
	wire [DATA_WIDTH-1: 0] in_feature_q_b[INPUT_NUM_MEM-1:0];




	
	reg enable_in_weight;
	reg enable_delay;
	reg enable_delay1;
	reg enable_delay2;
	reg enable_delay3;

	reg enable_sload;



	reg [COUNT_CONV_DONE_BITWIDTH-1:0] count_conv_done;
	

	
genvar input_num_mem;
generate
for (input_num_mem = 0; input_num_mem < INPUT_NUM_MEM; input_num_mem = input_num_mem+1) begin : q_a_q_b
  
  assign in_feature_q_a[input_num_mem]=in_feature_q_a_all[input_num_mem*DATA_WIDTH+DATA_WIDTH-1:input_num_mem*DATA_WIDTH];
  assign in_feature_q_b[input_num_mem]=in_feature_q_b_all[input_num_mem*DATA_WIDTH+DATA_WIDTH-1:input_num_mem*DATA_WIDTH];
end // for

endgenerate  

/*genvar num;
generate
for (num = 0; num < IFMAP_PAR; num = num+1) begin : ff
  
  assign in_feature_q_a_mux_all[num]=in_feature_q_a_mux[num*DATA_WIDTH+DATA_WIDTH-1:num*DATA_WIDTH];
  assign in_feature_q_b_mux_all[num]=in_feature_q_b_mux[num*DATA_WIDTH+DATA_WIDTH-1:num*DATA_WIDTH];
end // for

endgenerate  */
 




	
//	assign accum_sload=(count_sload==0)?1:0;
assign accum_sload=(|count_sload)?0:1;

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
 				
// mux signal
/*always@(*)
begin
	if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER)
		begin 
			in_feature_q_a_mux_all[ DATA_WIDTH*IFMAP_PAR - 1 : DATA_WIDTH * IFMAP_PAR - DATA_WIDTH] <= in_feature_q_a[1];
			in_feature_q_a_mux_all[ DATA_WIDTH*(IFMAP_PAR-1) - 1 : 0] <= in_feature_q_a[0];
		end
	else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*2)
			begin in_feature_q_a_mux_all[ DATA_WIDTH*IFMAP_PAR - 1 : DATA_WIDTH * IFMAP_PAR - DATA_WIDTH] <= in_feature_q_a[3];
			in_feature_q_a_mux_all[ DATA_WIDTH*(IFMAP_PAR-1) - 1 : 0] <= in_feature_q_a[2];end

	else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*3)
			begin in_feature_q_a_mux_all[ DATA_WIDTH*IFMAP_PAR - 1 : DATA_WIDTH * IFMAP_PAR - DATA_WIDTH] <= in_feature_q_a[5];
			in_feature_q_a_mux_all[ DATA_WIDTH*(IFMAP_PAR-1) - 1 : 0] <= in_feature_q_a[4];end
	else in_feature_q_a_mux_all<=1;
end


always@(*)
begin
	if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER)
		begin in_feature_q_b_mux_all[ DATA_WIDTH*IFMAP_PAR - 1 : DATA_WIDTH * IFMAP_PAR - DATA_WIDTH] <= in_feature_q_b[1];
			in_feature_q_b_mux_all[ DATA_WIDTH*(IFMAP_PAR-1) - 1 : 0] <= in_feature_q_b[0];end
	else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*2)
			begin in_feature_q_b_mux_all[ DATA_WIDTH*IFMAP_PAR - 1 : DATA_WIDTH * IFMAP_PAR - DATA_WIDTH] <= in_feature_q_b[3];
			in_feature_q_b_mux_all[ DATA_WIDTH*(IFMAP_PAR-1) - 1 : 0] <= in_feature_q_b[2];end

	else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*3)
			begin in_feature_q_b_mux_all[ DATA_WIDTH*IFMAP_PAR - 1 : DATA_WIDTH * IFMAP_PAR - DATA_WIDTH] <= in_feature_q_b[5];
			in_feature_q_b_mux_all[ DATA_WIDTH*(IFMAP_PAR-1) - 1 : 0] <= in_feature_q_b[4];end
	else in_feature_q_b_mux_all<=1;
end*/
always@(*)
begin
	if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER)


			begin in_feature_q_a_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_a[0]; 
			in_feature_q_a_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ] <= in_feature_q_a[1]; end
			 
			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*2)
				begin in_feature_q_a_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_a[2];
			in_feature_q_a_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ] <= in_feature_q_a[3]; end

			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*3)
				begin in_feature_q_a_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_a[4];
			in_feature_q_a_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ] <= in_feature_q_a[5]; 	end

			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*4)
				begin in_feature_q_a_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_a[6];	
			in_feature_q_a_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_a[7]; end
			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*5)
				begin in_feature_q_a_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_a[8]; 
				in_feature_q_a_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_a[9]; end
			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*6)
				begin in_feature_q_a_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_a[10];
				in_feature_q_a_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_a[11]; end
			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*7)
				begin in_feature_q_a_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_a[12];
				in_feature_q_a_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_a[13]; end
			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*8)
				begin in_feature_q_a_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_a[14];
			in_feature_q_a_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_a[15]; end

			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*9)
				begin in_feature_q_a_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_a[16];
				in_feature_q_a_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_a[17]; end

			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*10)
				begin in_feature_q_a_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_a[18];
			in_feature_q_a_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_a[19]; end

	else in_feature_q_a_mux_all<=1;
end
always@(*)
begin
	if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER)


			begin in_feature_q_b_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_b[0]; 
			in_feature_q_b_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ] <= in_feature_q_b[1]; end
			 
			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*2)
				begin in_feature_q_b_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_b[2];
			in_feature_q_b_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ] <= in_feature_q_b[3]; end

			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*3)
				begin in_feature_q_b_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_b[4];
			in_feature_q_b_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ] <= in_feature_q_b[5]; 	end

			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*4)
				begin in_feature_q_b_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_b[6];	
			in_feature_q_b_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_b[7]; end
			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*5)
				begin in_feature_q_b_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_b[8]; 
				in_feature_q_b_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_b[9]; end
			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*6)
				begin in_feature_q_b_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_b[10];
				in_feature_q_b_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_b[11]; end
			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*7)
				begin in_feature_q_b_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_b[12];
				in_feature_q_b_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_b[13]; end
			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*8)
				begin in_feature_q_b_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_b[14];
			in_feature_q_b_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_b[15]; end

			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*9)
				begin in_feature_q_b_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_b[16];
				in_feature_q_b_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_b[17]; end

			else if(count_sload<NUM_ONE_PIXEL_CYCLE_INTER*10)
				begin in_feature_q_b_mux_all[ DATA_WIDTH - 1 : 0] <= in_feature_q_b[18];
			in_feature_q_b_mux_all[ DATA_WIDTH *2- 1 : DATA_WIDTH ]  <= in_feature_q_b[19]; end

	else in_feature_q_b_mux_all<=1;
end


			 
endmodule			 