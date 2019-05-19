`include "fc_param_1.vh"
module fc_control_param_1(

// Global signals
	clock,
	enable,
	reset,	
   
	enable_fc_addrger,
	enable_fc_weightaddrger,
	enable_write,
	
//in  neuron
	in_neuron_rden_a,
	in_neuron_rden_b,
	in_neuron_wren_a,
	in_neuron_wren_b,
	
//mult
	accum_sload,
	enable_mult,
	clear_mult,
	count_sload,
	count_out,
	
// weight  neuron	
	fc_weight_rden_a ,
   fc_weight_rden_b ,
	fc_weight_wren_a ,
	fc_weight_wren_b ,
	
	in_neuron_q_a_mux,
	in_neuron_q_b_mux,
	
	
	in_neuron_q_a_all,
	in_neuron_q_b_all,
	
	done
	);	


parameter	OUTNEURON=10;
parameter	INNEURON=576;
parameter 	PI=16;
parameter	PO=1;
			
parameter	ACCUM_DATA_WIDTH_FC=44;  	
parameter	DATA_WIDTH_FC=16; 	


parameter	FC_INNEURON_ADDR_WIDTH=9;		
parameter	FC_WEIGHT_ADDR_WIDTH=13;   		
		
parameter	FC_OUTNEURON_ADDR_WIDTH=9; 

		
parameter	FC_COUNT_SLOAD_BITWIDTH=9;  
parameter	FC_COUNT_OUT_NEURON_BITWIDTH=4;		
// input signal 
 	input wire clock;
	input wire enable;
	input wire reset;


	input wire [DATA_WIDTH_FC*PI-1: 0] in_neuron_q_a_all;
	input wire [DATA_WIDTH_FC*PI-1: 0] in_neuron_q_b_all;	
	
// output signal
	output reg   in_neuron_rden_a ;
	output reg   in_neuron_rden_b ;
	output reg   in_neuron_wren_a ;
	output reg   in_neuron_wren_b ;


	output reg fc_weight_rden_a ;
	output reg fc_weight_rden_b ;
   output reg fc_weight_wren_a ;
   output reg fc_weight_wren_b ;

	output reg enable_fc_addrger;
	output reg enable_fc_weightaddrger;
	
// mult
	output wire accum_sload ;
	output reg enable_mult ;
	output reg clear_mult;
	output reg enable_write;
 	output  reg [DATA_WIDTH_FC-1: 0] in_neuron_q_a_mux;
 	output  reg [DATA_WIDTH_FC-1: 0] in_neuron_q_b_mux;	
//internal signal
//wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a[PI-1:0];展开-->16
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_0;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_1;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_2;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_3;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_4;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_5;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_6;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_7;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_8;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_9;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_10;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_11;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_12;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_13;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_14;
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a_unroll_15;
//wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b[PI-1:0];展开-->16
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_0;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_1;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_2;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_3;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_4;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_5;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_6;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_7;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_8;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_9;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_10;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_11;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_12;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_13;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_14;
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b_unroll_15;


	output reg [FC_COUNT_SLOAD_BITWIDTH-1:0] count_sload;

	
	reg enable_in_weight;
	reg enable_delay;
	reg enable_delay1;
	reg enable_delay2;
	reg enable_delay3;
	reg enable_sload;
	output reg done;
	output reg [FC_COUNT_OUT_NEURON_BITWIDTH-1:0]count_out;
	
//generate for开始------------------------------->
//pi:16
//第0次展开：0/16
  
  assign in_neuron_q_a_unroll_0=in_neuron_q_a_all[0*DATA_WIDTH_FC+DATA_WIDTH_FC-1:0*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_0=in_neuron_q_b_all[0*DATA_WIDTH_FC+DATA_WIDTH_FC-1:0*DATA_WIDTH_FC];
//第1次展开：1/16
  
  assign in_neuron_q_a_unroll_1=in_neuron_q_a_all[1*DATA_WIDTH_FC+DATA_WIDTH_FC-1:1*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_1=in_neuron_q_b_all[1*DATA_WIDTH_FC+DATA_WIDTH_FC-1:1*DATA_WIDTH_FC];
//第2次展开：2/16
  
  assign in_neuron_q_a_unroll_2=in_neuron_q_a_all[2*DATA_WIDTH_FC+DATA_WIDTH_FC-1:2*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_2=in_neuron_q_b_all[2*DATA_WIDTH_FC+DATA_WIDTH_FC-1:2*DATA_WIDTH_FC];
//第3次展开：3/16
  
  assign in_neuron_q_a_unroll_3=in_neuron_q_a_all[3*DATA_WIDTH_FC+DATA_WIDTH_FC-1:3*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_3=in_neuron_q_b_all[3*DATA_WIDTH_FC+DATA_WIDTH_FC-1:3*DATA_WIDTH_FC];
//第4次展开：4/16
  
  assign in_neuron_q_a_unroll_4=in_neuron_q_a_all[4*DATA_WIDTH_FC+DATA_WIDTH_FC-1:4*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_4=in_neuron_q_b_all[4*DATA_WIDTH_FC+DATA_WIDTH_FC-1:4*DATA_WIDTH_FC];
//第5次展开：5/16
  
  assign in_neuron_q_a_unroll_5=in_neuron_q_a_all[5*DATA_WIDTH_FC+DATA_WIDTH_FC-1:5*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_5=in_neuron_q_b_all[5*DATA_WIDTH_FC+DATA_WIDTH_FC-1:5*DATA_WIDTH_FC];
//第6次展开：6/16
  
  assign in_neuron_q_a_unroll_6=in_neuron_q_a_all[6*DATA_WIDTH_FC+DATA_WIDTH_FC-1:6*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_6=in_neuron_q_b_all[6*DATA_WIDTH_FC+DATA_WIDTH_FC-1:6*DATA_WIDTH_FC];
//第7次展开：7/16
  
  assign in_neuron_q_a_unroll_7=in_neuron_q_a_all[7*DATA_WIDTH_FC+DATA_WIDTH_FC-1:7*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_7=in_neuron_q_b_all[7*DATA_WIDTH_FC+DATA_WIDTH_FC-1:7*DATA_WIDTH_FC];
//第8次展开：8/16
  
  assign in_neuron_q_a_unroll_8=in_neuron_q_a_all[8*DATA_WIDTH_FC+DATA_WIDTH_FC-1:8*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_8=in_neuron_q_b_all[8*DATA_WIDTH_FC+DATA_WIDTH_FC-1:8*DATA_WIDTH_FC];
//第9次展开：9/16
  
  assign in_neuron_q_a_unroll_9=in_neuron_q_a_all[9*DATA_WIDTH_FC+DATA_WIDTH_FC-1:9*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_9=in_neuron_q_b_all[9*DATA_WIDTH_FC+DATA_WIDTH_FC-1:9*DATA_WIDTH_FC];
//第10次展开：10/16
  
  assign in_neuron_q_a_unroll_10=in_neuron_q_a_all[10*DATA_WIDTH_FC+DATA_WIDTH_FC-1:10*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_10=in_neuron_q_b_all[10*DATA_WIDTH_FC+DATA_WIDTH_FC-1:10*DATA_WIDTH_FC];
//第11次展开：11/16
  
  assign in_neuron_q_a_unroll_11=in_neuron_q_a_all[11*DATA_WIDTH_FC+DATA_WIDTH_FC-1:11*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_11=in_neuron_q_b_all[11*DATA_WIDTH_FC+DATA_WIDTH_FC-1:11*DATA_WIDTH_FC];
//第12次展开：12/16
  
  assign in_neuron_q_a_unroll_12=in_neuron_q_a_all[12*DATA_WIDTH_FC+DATA_WIDTH_FC-1:12*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_12=in_neuron_q_b_all[12*DATA_WIDTH_FC+DATA_WIDTH_FC-1:12*DATA_WIDTH_FC];
//第13次展开：13/16
  
  assign in_neuron_q_a_unroll_13=in_neuron_q_a_all[13*DATA_WIDTH_FC+DATA_WIDTH_FC-1:13*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_13=in_neuron_q_b_all[13*DATA_WIDTH_FC+DATA_WIDTH_FC-1:13*DATA_WIDTH_FC];
//第14次展开：14/16
  
  assign in_neuron_q_a_unroll_14=in_neuron_q_a_all[14*DATA_WIDTH_FC+DATA_WIDTH_FC-1:14*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_14=in_neuron_q_b_all[14*DATA_WIDTH_FC+DATA_WIDTH_FC-1:14*DATA_WIDTH_FC];
//第15次展开：15/16
  
  assign in_neuron_q_a_unroll_15=in_neuron_q_a_all[15*DATA_WIDTH_FC+DATA_WIDTH_FC-1:15*DATA_WIDTH_FC];
  assign in_neuron_q_b_unroll_15=in_neuron_q_b_all[15*DATA_WIDTH_FC+DATA_WIDTH_FC-1:15*DATA_WIDTH_FC];
	
always @ (posedge clock or posedge reset)
begin
	   
		if(reset) 
		    count_sload<=0;
	    else if (enable_sload&&count_sload<INNEURON/2-1)
			  	  count_sload<=count_sload+1;
				 else count_sload<=0;						 
end	
	



always @ (posedge clock or posedge reset)
begin
	   
	if(reset) 
		count_out<= 0;
	else if (enable && count_sload == INNEURON/2-1)
			count_out <= count_out + 1;
end	

always @ (posedge clock or posedge reset)
begin
	   
		if(reset) 
		  		  done <= 0;
			else if ((count_out==(OUTNEURON/PO))&&(count_sload==6))
			  	  done <= 1;
				  
end	
	
assign accum_sload=(count_sload==0)?1:0;

//enable 
always @ (posedge clock or posedge reset)
begin
	if(reset) 
		begin
			enable_fc_addrger			<= 0;
			enable_fc_weightaddrger <= 0;
			enable_mult	<= 0;
			enable_write <= 0;
		end
	else if(enable)
			begin
				enable_fc_addrger			<= 1;
				enable_fc_weightaddrger <= 1;		     
				enable_mult	<= 1;		
				enable_write <= 1;
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
			in_neuron_rden_a<=0;
			in_neuron_rden_b<=0;
			in_neuron_wren_a<=0;
			in_neuron_wren_b<=0;
	
			fc_weight_rden_a<=0;
			fc_weight_rden_b<=0;
			fc_weight_wren_a<=0;
			fc_weight_wren_b<=0;
	
		end
	else if(enable_in_weight) 
			begin
			in_neuron_rden_a<=1;
			in_neuron_rden_b<=1;
			in_neuron_wren_a<=0;
			in_neuron_wren_b<=0;
	
			fc_weight_rden_a<=1;
			fc_weight_rden_b<=1;
			fc_weight_wren_a<=0;
			fc_weight_wren_b<=0;	
			end

			

always@(*)
begin
	if(count_sload <INNEURON/2/PI)

			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <= in_neuron_q_a_unroll_0;
	else if(count_sload <INNEURON/2/PI*2)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <= in_neuron_q_a_unroll_1;
		else if(count_sload <INNEURON/2/PI*3)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <= in_neuron_q_a_unroll_2;
		else if(count_sload <INNEURON/2/PI*4)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a_unroll_3;
		else if(count_sload <INNEURON/2/PI*5)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a_unroll_4;
		else if(count_sload <INNEURON/2/PI*6)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a_unroll_5;
		else if(count_sload <INNEURON/2/PI*7)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a_unroll_6;
		else if(count_sload <INNEURON/2/PI*8)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a_unroll_7;
		else if(count_sload <INNEURON/2/PI*9)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a_unroll_8;
		else if(count_sload <INNEURON/2/PI*10)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a_unroll_9;
		else if(count_sload <INNEURON/2/PI*11)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a_unroll_10;
		else if(count_sload <INNEURON/2/PI*12)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a_unroll_11;
		else if(count_sload <INNEURON/2/PI*13)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a_unroll_12;
		else if(count_sload <INNEURON/2/PI*14)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a_unroll_13;
			
		else if(count_sload <INNEURON/2/PI*15)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a_unroll_14;
		else if(count_sload <INNEURON/2/PI*16)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a_unroll_15;



	
	else in_neuron_q_a_mux<=0;
end



always@(*)
begin
	if(count_sload <INNEURON/2/PI)

			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <= in_neuron_q_b_unroll_0;
	else if(count_sload <INNEURON/2/PI*2)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <= in_neuron_q_b_unroll_1;
		else if(count_sload <INNEURON/2/PI*3)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <= in_neuron_q_b_unroll_2;
		else if(count_sload <INNEURON/2/PI*4)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b_unroll_3;
		else if(count_sload <INNEURON/2/PI*5)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b_unroll_4;
		else if(count_sload <INNEURON/2/PI*6)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b_unroll_5;
		else if(count_sload <INNEURON/2/PI*7)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b_unroll_6;
		else if(count_sload <INNEURON/2/PI*8)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b_unroll_7;
		else if(count_sload <INNEURON/2/PI*9)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b_unroll_8;
		else if(count_sload <INNEURON/2/PI*10)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b_unroll_9;
		else if(count_sload <INNEURON/2/PI*11)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b_unroll_10;
		else if(count_sload <INNEURON/2/PI*12)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b_unroll_11;
		else if(count_sload <INNEURON/2/PI*13)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b_unroll_12;
		else if(count_sload <INNEURON/2/PI*14)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b_unroll_13;



	
	else in_neuron_q_b_mux<=0;
end

endmodule	



