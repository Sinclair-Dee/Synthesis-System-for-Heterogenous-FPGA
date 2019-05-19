`include "fc_param_1.vh"
module  write_result_1(
// Global signals
	 clock,
    enable,
	 reset,	
	  address,
	  result,
	  dataout,
	  count_sload,
	   fc_outneuron_wren_a,
	count_out
	);	
	
parameter DATA_WIDTH_FC=`DATA_WIDTH_FC;
parameter FC_OUTNEURON_ADDR_WIDTH =`FC_OUTNEURON_ADDR_WIDTH;
parameter FC_COUNT_OUT_NEURON_BITWIDTH=`FC_COUNT_OUT_NEURON_BITWIDTH;
parameter	ACCUM_DATA_WIDTH_FC=`ACCUM_DATA_WIDTH_FC;	
parameter OUTNEURON=`OUTNEURON;
parameter PO=`PO;
parameter FC_COUNT_SLOAD_BITWIDTH=`FC_COUNT_SLOAD_BITWIDTH;
parameter 	INNEURON = `INNEURON;
input clock;
input enable;
input reset;
input [FC_COUNT_SLOAD_BITWIDTH-1:0]count_sload;
input [ACCUM_DATA_WIDTH_FC-1:0]result;
input  [FC_COUNT_OUT_NEURON_BITWIDTH-1:0]count_out;
output reg[FC_OUTNEURON_ADDR_WIDTH-1: 0] address;
output reg[DATA_WIDTH_FC-1: 0] dataout;
output reg fc_outneuron_wren_a;	
	
always @ (posedge clock or posedge reset)
begin
    if(reset)  
	    begin
		    address <= 0;
		 end
	 else if(enable&&count_sload==1&&count_out>1)
	        if( address<(OUTNEURON/PO-1))
	              begin address <= address + 1 ;
			end
		    else 
		          address <= 0 ;
end
always @ (posedge clock or posedge reset)
begin
    if(reset)  
	    begin
		    dataout <=  0;
		 end
	 else if(count_sload==2)
	      dataout <=  {result[ACCUM_DATA_WIDTH_FC-1],result[26:12]};
end
always @ (posedge clock or posedge reset)
begin
    if(reset)  
	    begin
		    fc_outneuron_wren_a <=  0;
		 end
	 else if(count_sload>=3 && count_sload <= 5&&count_out>=1)
	      fc_outneuron_wren_a <= 1;
			else  fc_outneuron_wren_a <=  0;
end		  
endmodule
