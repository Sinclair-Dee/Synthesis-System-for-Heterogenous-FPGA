`include "param_1.vh"
module weight_addrgener_param_1 (
// Global signals
	 clk,
    enable,
	 reset,	
	 addra,
	 addrb 
	);	
//******* ADD for resource parameter


parameter NUM_ONEMULT=1;//ONE MULT COMPUTE HOW MUCH OUT FEATURE MAP

	//data width parameter		
	
// number of input output feature map
	

// input output feature map size ,stride of conv
parameter	IN_FEATURE_WIDTH=36;	

	
// input output feature map size ,stride of conv
parameter	OUT_FEATURE_WIDTH=32;	
	parameter OUT_FEATURE_WIDTH_POOL=16;
// input output feature map size ,stride of conv


//addr width
		
parameter	WEIGHT_ADDR_WIDTH=9;		
		

//	mainly control parameter

parameter	NUM_ONE_PIXEL_CYCLE	=	13;
		

//mainly addr generate parameter	

parameter NUM_MULTCOMP_BITWIDTH=2;
parameter OUTPIXEL_BITWIDTH=11;

parameter IFMAP_PAR= 1;
// exteral signals
	input clk;
	input enable;
	input reset;
	
//internal signals	
	output reg [WEIGHT_ADDR_WIDTH - 1 : 0] addra;
	output reg [WEIGHT_ADDR_WIDTH - 1 : 0] addrb;
	
	reg [WEIGHT_ADDR_WIDTH - 1 : 0] addra_temp;
	reg [WEIGHT_ADDR_WIDTH - 1 : 0] addrb_temp;

	
	// add for deep param
	reg over=0;
	reg inc_addr;
	reg inc_num_multcomp;
	reg [NUM_MULTCOMP_BITWIDTH-1:0]num_multcomp;
	reg [OUTPIXEL_BITWIDTH-1:0]num_outpixel;
	reg inc_num_outpixel;
always @(*)
begin
	inc_addr = addra_temp < ( NUM_ONE_PIXEL_CYCLE * 2- 2);

	inc_num_multcomp=num_multcomp < (NUM_ONEMULT - 1);
	inc_num_outpixel =num_outpixel <(OUT_FEATURE_WIDTH*OUT_FEATURE_WIDTH-1);
end

//


//main logic
always @ (posedge clk or posedge reset)
begin
	if(reset) 
		begin
			num_multcomp<=0;
			addra_temp<=0;
			addrb_temp<=1;
			num_outpixel <=0;
			over <=0;
		end		  
	else if(!over&&enable)
			begin
				    
				if (inc_addr)
				  begin
					addra_temp <= addra_temp + 2;
					addrb_temp <= addrb_temp + 2;
					end
								

					else if(inc_num_outpixel)
							 begin
								addra_temp<=0;
								addrb_temp<=1;
								num_outpixel <= num_outpixel + 1;
							end
							
						else if(inc_num_multcomp)
							 begin
								addra_temp<=0;
								addrb_temp<=1;
								num_outpixel <= 0;
								num_multcomp <= num_multcomp + 1;
								
							end						
					else 
						over <= 1;
		end  
end
//*******//		

// compute address


always @ (posedge clk )
  begin
  addra<=addra_temp + num_multcomp*NUM_ONE_PIXEL_CYCLE * 2 ;
  addrb<=addrb_temp + num_multcomp*NUM_ONE_PIXEL_CYCLE * 2;
  
  end

endmodule


