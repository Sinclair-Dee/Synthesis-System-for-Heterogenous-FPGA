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


parameter	OUTNEURON=`OUTNEURON;
parameter	INNEURON=`INNEURON;
parameter 	PI=`PI;
parameter	PO=`PO;
			
parameter	ACCUM_DATA_WIDTH_FC=`ACCUM_DATA_WIDTH_FC;  	
parameter	DATA_WIDTH_FC=`DATA_WIDTH_FC; 	


parameter	FC_INNEURON_ADDR_WIDTH=`FC_INNEURON_ADDR_WIDTH;		
parameter	FC_WEIGHT_ADDR_WIDTH=`FC_WEIGHT_ADDR_WIDTH;   		
		
parameter	FC_OUTNEURON_ADDR_WIDTH=`FC_OUTNEURON_ADDR_WIDTH; 

		
parameter	FC_COUNT_SLOAD_BITWIDTH=`FC_COUNT_SLOAD_BITWIDTH;  
parameter	FC_COUNT_OUT_NEURON_BITWIDTH=`FC_COUNT_OUT_NEURON_BITWIDTH;		
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
 	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_a[PI-1:0];
	wire [DATA_WIDTH_FC-1: 0] in_neuron_q_b[PI-1:0];


	output reg [FC_COUNT_SLOAD_BITWIDTH-1:0] count_sload;

	
	reg enable_in_weight;
	reg enable_delay;
	reg enable_delay1;
	reg enable_delay2;
	reg enable_delay3;
	reg enable_sload;
	output reg done;
	output reg [FC_COUNT_OUT_NEURON_BITWIDTH-1:0]count_out;
	
genvar pi;
generate
for (pi = 0; pi < PI; pi = pi+1) begin : q_a_q_b
  
  assign in_neuron_q_a[pi]=in_neuron_q_a_all[pi*DATA_WIDTH_FC+DATA_WIDTH_FC-1:pi*DATA_WIDTH_FC];
  assign in_neuron_q_b[pi]=in_neuron_q_b_all[pi*DATA_WIDTH_FC+DATA_WIDTH_FC-1:pi*DATA_WIDTH_FC];
end // for

endgenerate  
	
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

			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <= in_neuron_q_a[0];
	else if(count_sload <INNEURON/2/PI*2)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <= in_neuron_q_a[1];
		else if(count_sload <INNEURON/2/PI*3)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <= in_neuron_q_a[2];
		else if(count_sload <INNEURON/2/PI*4)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[3];	
		else if(count_sload <INNEURON/2/PI*5)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[4];	
		else if(count_sload <INNEURON/2/PI*6)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[5];	
		else if(count_sload <INNEURON/2/PI*7)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[6];	
		else if(count_sload <INNEURON/2/PI*8)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[7];	
		else if(count_sload <INNEURON/2/PI*9)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[8];	
		else if(count_sload <INNEURON/2/PI*10)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[9];	
		else if(count_sload <INNEURON/2/PI*11)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[10];			
		else if(count_sload <INNEURON/2/PI*12)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[11];	
		else if(count_sload <INNEURON/2/PI*13)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[12];	
		else if(count_sload <INNEURON/2/PI*14)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[13];	
		else if(count_sload <INNEURON/2/PI*15)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[14];	
		else if(count_sload <INNEURON/2/PI*16)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[15];	
		else if(count_sload <INNEURON/2/PI*17)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[16];	
		else if(count_sload <INNEURON/2/PI*18)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[17];	
		else if(count_sload <INNEURON/2/PI*19)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[18];	
		else if(count_sload <INNEURON/2/PI*20)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[19];	
		else if(count_sload <INNEURON/2/PI*21)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[20];			
		else if(count_sload <INNEURON/2/PI*22)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[21];	
		else if(count_sload <INNEURON/2/PI*23)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[22];	
		else if(count_sload <INNEURON/2/PI*24)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[23];	
		else if(count_sload <INNEURON/2/PI*25)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_a[24];	


	
	else in_neuron_q_a_mux<=0;
end

always@(*)
begin
	if(count_sload <INNEURON/2/PI)

			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <= in_neuron_q_b[0];
	else if(count_sload <INNEURON/2/PI*2)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <= in_neuron_q_b[1];
		else if(count_sload <INNEURON/2/PI*3)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <= in_neuron_q_b[2];
		else if(count_sload <INNEURON/2/PI*4)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[3];	
		else if(count_sload <INNEURON/2/PI*5)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[4];	
		else if(count_sload <INNEURON/2/PI*6)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[5];	
		else if(count_sload <INNEURON/2/PI*7)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[6];	
		else if(count_sload <INNEURON/2/PI*8)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[7];	
		else if(count_sload <INNEURON/2/PI*9)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[8];	
		else if(count_sload <INNEURON/2/PI*10)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[9];	
		else if(count_sload <INNEURON/2/PI*11)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[10];			
		else if(count_sload <INNEURON/2/PI*12)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[11];	
		else if(count_sload <INNEURON/2/PI*13)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[12];	
		else if(count_sload <INNEURON/2/PI*14)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[13];	
		else if(count_sload <INNEURON/2/PI*15)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[14];	
		else if(count_sload <INNEURON/2/PI*16)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[15];	
		else if(count_sload <INNEURON/2/PI*17)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[16];	
		else if(count_sload <INNEURON/2/PI*18)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[17];	
		else if(count_sload <INNEURON/2/PI*19)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[18];	
		else if(count_sload <INNEURON/2/PI*20)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[19];	
		else if(count_sload <INNEURON/2/PI*21)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[20];			
		else if(count_sload <INNEURON/2/PI*22)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[21];	
		else if(count_sload <INNEURON/2/PI*23)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[22];	
		else if(count_sload <INNEURON/2/PI*24)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[23];	
		else if(count_sload <INNEURON/2/PI*25)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <=in_neuron_q_b[24];	


	
	else in_neuron_q_b_mux<=0;
end

/*always @(*)
begin
	if(count_sload <INNEURON/2/PI)

			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <= 16'd1;
		else if(count_sload <INNEURON/2/PI*2)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <= 16'd2;
		else if(count_sload <INNEURON/2/PI*3)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <= 16'd3;
		else if(count_sload <INNEURON/2/PI*4)
			in_neuron_q_a_mux[ DATA_WIDTH_FC - 1 : 0] <= 16'd4;			
	else in_neuron_q_a_mux<=0;
end

always@(*)
begin
	if(count_sload <INNEURON/2/PI)

			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <= 16'd1;
		else if(count_sload <INNEURON/2/PI*2)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <= 16'd2;
		else if(count_sload <INNEURON/2/PI*3)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <= 16'd3;
		else if(count_sload <INNEURON/2/PI*4)
			in_neuron_q_b_mux[ DATA_WIDTH_FC - 1 : 0] <= 16'd4;			
	else in_neuron_q_b_mux<=0;
end		*/	 
endmodule	
