`include "fc_param_1.vh"
module  fc_weight_addrgener_param_1 (
// Global signals
	 clk,
    enable,
	 reset,	
	  addra,
	  addrb 
	);	

parameter	FC_WEIGHT_ADDR_WIDTH=`FC_WEIGHT_ADDR_WIDTH;		
parameter	OUTNEURON=`OUTNEURON;
parameter	INNEURON=`INNEURON;
parameter 	PI=`PI;
parameter	PO=`PO;

// exteral signals
	input clk;
	input enable;
	input reset;
	
//internal signals	
	output reg [FC_WEIGHT_ADDR_WIDTH - 1 : 0] addra;
	output reg [FC_WEIGHT_ADDR_WIDTH - 1 : 0] addrb;
	reg [FC_WEIGHT_ADDR_WIDTH - 1 : 0] addra_temp;
	reg [FC_WEIGHT_ADDR_WIDTH - 1 : 0] addrb_temp;
		
// compute address
always @ (posedge clk or posedge reset)
begin
    if(reset)  
	    begin
		    addra_temp<=0;
			 addrb_temp<=1;
		 end
	 else if(enable)
	        if(addra_temp<(OUTNEURON*INNEURON/PO-2))
	               begin
		             addra_temp<=(addra_temp+2)%25;
		             addrb_temp<=(addrb_temp+2)%25;
		            end
		    else 
		            begin
		             addra_temp<=0;
		             addrb_temp<=1;
		            end		 
end
always @ (posedge clk )
  begin
  addra<=addra_temp;
  addrb<=addrb_temp;
  
  end
	



endmodule










