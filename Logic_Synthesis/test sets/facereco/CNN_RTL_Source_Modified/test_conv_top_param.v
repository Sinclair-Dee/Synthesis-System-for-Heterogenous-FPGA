`timescale 1ns/1ps

module test_conv_top_param();


//******* ADD for resource parameter

parameter INPUT_NUM_MEM=1;	

parameter	DATA_WIDTH=16;		
// number of input output feature map


//addr width
parameter	IN_FEATURE_ADDR_WIDTH=10;		


	reg clock=1;
	reg enable;
	reg reset;

wire  [DATA_WIDTH-1: 0] final;


facereco facereco(

.clock(clock),
.enable(enable),     
.reset(reset),
.final(final)

)	;	
		





		
//signals assignment
	always #5 clock=~clock; 
	initial
	begin
    
		enable=0;
		reset=0;
		

	 
		#10
		reset=1;
		#10
		reset=0;
		#10
		enable=1;

 
	end
  
endmodule
