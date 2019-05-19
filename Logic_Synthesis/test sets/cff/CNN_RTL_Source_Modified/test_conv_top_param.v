`timescale 1ns/1ps

module test_conv_top_param();


//******* ADD for resource parameter



parameter	DATA_WIDTH=16;		
// number of input output feature map


//addr width
parameter	IN_FEATURE_ADDR_WIDTH=11;		


	reg clock=1;
	reg enable;
	reg reset;


	
	wire [DATA_WIDTH-1: 0] finalresult;
	


cff cff(

.clock(clock),
.enable(enable),     
.reset(reset),

.finalresult(finalresult)	
)	;	
		




		
//signals assignment
	always #3.3333333333333 clock=~clock; 
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
