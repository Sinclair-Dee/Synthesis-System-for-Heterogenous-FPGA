
`include "param_3.vh"

module no_pool (
		data_in,
		conv_done,
		conv_start,
		clock,
		reset,
	

		data_a,
		data_b,
	
	
		rden_a,
		rden_b,
		wren_a,
		wren_b,
		
		address_a_t,
		address_b_t
	

	);




parameter	DATA_WIDTH=`DATA_WIDTH;		
parameter	NUM_ONE_PIXEL_CYCLE	=	`NUM_ONE_PIXEL_CYCLE;
parameter	POOL_ADDR_WIDTH =`POOL_ADDR_WIDTH;
parameter	COUNT_WRITEBIT = `COUNT_WRITEBIT;
//*********************************
	input [DATA_WIDTH-1:0] data_in;
	input conv_done;
	input conv_start;
	input clock;
	input reset;


	output [DATA_WIDTH-1:0] data_a;
	output [DATA_WIDTH-1:0] data_b;
	
	
	output rden_a;
	output rden_b;
	output wren_a;
	output wren_b;
		
	output [POOL_ADDR_WIDTH-1:0]address_a_t;
	output [POOL_ADDR_WIDTH-1:0]address_b_t;	

	


	reg [DATA_WIDTH-1:0] data;
   reg [POOL_ADDR_WIDTH-1:0] address;
	reg wr_en,rd_en;
	reg [COUNT_WRITEBIT-1:0]count_write;
always @ (posedge clock or posedge reset)
begin
    if(reset)  
	    
		    count_write <= 0;
		
	 else if(conv_start && count_write < NUM_ONE_PIXEL_CYCLE-1)
				count_write <= count_write + 1;		
					else count_write <= 0;		
end 


 
always @ (posedge clock or posedge reset)
begin
    if(reset)  
	   
		    address <= 0;
	
	 else if(count_write==NUM_ONE_PIXEL_CYCLE-1  )
	        address <= address + 1 ;
		
end
always @ (posedge clock or posedge reset)
begin
    if(reset)  
	   
		    data <=  0;
		
	 else 
	      data <=  data_in;
end
always @ (posedge clock or posedge reset)
begin
    if(reset)  
	    begin
		    wr_en <=  0;
		 end
	 else if(count_write>=2 && count_write<= 4 && conv_done==0)
	      wr_en <= 1;
			else  wr_en <=  0;
end		  




 
	assign data_a=data;
	assign address_a_t=address;
	
	assign wren_a=wr_en;
	assign rden_a=0;
	
	assign wren_b=1'b0;
	assign rden_b=1'b0;	
	 
	 
    
endmodule
        
        
        
        
	  
	
	      
						  



