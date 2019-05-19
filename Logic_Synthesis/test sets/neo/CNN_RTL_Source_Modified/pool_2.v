
`include "param_2.vh"

module pool_2 (
		data_in,
		conv_done,
		conv_start,
		clk,
		reset,
	
		q_a,
		q_b,

		data_a,
		data_b,
	
	
		rden_a,
		rden_b,
		wren_a,
		wren_b,
		
		address_a_t,
		address_b_t
	

	);
//******* ADD for resource parameter


parameter NUM_ONEMULT=1;//ONE MULT COMPUTE HOW MUCH OUT FEATURE MAPparameter NUM_MULT=16;

	//data width parameter		

parameter	DATA_WIDTH=16;		
// number of input output feature map
	


	
// input output feature map size ,stride of conv

parameter	OUT_FEATURE_WIDTH_POOL=6;	

//addr width

parameter	POOL_ADDR_WIDTH=9;		

//	mainly control parameter

parameter	NUM_ONE_PIXEL_CYCLE	=	26;
parameter	CNT1_BITWIDTH=5;		
parameter	CNT3_BITWIDTH=4;		


parameter ONE_MULT_COMPU_BITWIDTH=	2;

localparam NUM_PIXEL_BITWIDTH=`NUM_PIXEL_BITWIDTH;
localparam NUM_OUT_PIXEL=`NUM_OUT_PIXEL;
//*********************************
	input [DATA_WIDTH-1:0] data_in;
	input conv_done;
	input conv_start;
	input clk;
	input reset;
	
	input [DATA_WIDTH-1:0] q_a;
	input [DATA_WIDTH-1:0] q_b;

	output [DATA_WIDTH-1:0] data_a;
	output [DATA_WIDTH-1:0] data_b;
	
	
	output rden_a;
	output rden_b;
	output wren_a;
	output wren_b;
		
	output [POOL_ADDR_WIDTH-1:0]address_a_t;
	output [POOL_ADDR_WIDTH-1:0]address_b_t;	

	
	/************************************************************************
	              the first processing
	************************************************************************/
	
	/********************************************************
	              the controler of timing
	********************************************************/

//****************ADD FOR DEEP PARAMETER
	reg [CNT1_BITWIDTH-1:0] cnt1;
	reg [1:0] cnt2;
	reg [CNT3_BITWIDTH-1:0] cnt3;
	reg [1:0] cnt4;	
reg inc_cnt1;
reg inc_cnt2;
reg inc_cnt3;
reg inc_cnt4;

reg [ONE_MULT_COMPU_BITWIDTH-1:0]onemult_compu;
reg inc_onemult_compu;
reg inc_num_pixel;
reg [NUM_PIXEL_BITWIDTH-1:0] num_pixel;
reg [NUM_PIXEL_BITWIDTH-1:0] delay_num_pixel;
always @(*)
begin
	inc_cnt1 = cnt1 < NUM_ONE_PIXEL_CYCLE  ;
	inc_cnt2 = cnt2 < 2'd2;
	inc_cnt3 = cnt3 < OUT_FEATURE_WIDTH_POOL;
	inc_cnt4 = cnt4<2'd2; 
	inc_onemult_compu = onemult_compu < NUM_ONEMULT ;
	inc_num_pixel =num_pixel <NUM_OUT_PIXEL;
end

always @ (posedge clk or posedge reset)
begin
	if(reset) 
		begin
			cnt1 <= 1;
			cnt2 <= 1; 
			cnt3 <= 1;
			cnt4 <= 1;
	
		end		  
	else if(conv_start)
			begin
			 
				    
				if (inc_cnt1)
					cnt1 <= cnt1 + 1;
								
				else if(inc_cnt2)
					begin
						cnt2 <= cnt2 + 1;
						cnt1 <=  1;
								  
					end
					else  if(inc_cnt3)
							begin
								cnt3 <= cnt3 + 1;
								cnt1 <=  1;
								cnt2 <=  1; 
							end
					
						else  if(inc_cnt4)
							begin
								cnt4<=cnt4+1;
								cnt1 <= 1;
								cnt2 <= 1;
								cnt3 <= 1;
							end
							
						else
							begin
								cnt4 <= 1;
								cnt1 <= 1;
								cnt2 <= 1;
								cnt3 <= 1;
								
							end
							
		end  
end
always @ (posedge clk or posedge reset)
begin
	if(reset) 
		begin
			num_pixel<=1;
			onemult_compu <= 1;
		end		  
	else if(conv_start)
			begin
			   if(inc_cnt1==0)
				begin
				    
				if (inc_num_pixel)
					num_pixel <= num_pixel + 1;
								
				else if(inc_onemult_compu)
					begin
						num_pixel <=  1;
						onemult_compu <= onemult_compu+1;
								  
					end
				
						else	
							begin

								onemult_compu <= 1;
								num_pixel <=  1;
							end
				end			
		end  
end
//***************ADD FOR DEEP PARAMETER
	reg conv_done_dl;
	reg conv_rst;
	
	always @(posedge clk or posedge reset)
		begin
			if(reset)
				conv_done_dl <= 1'b0;
			else 
				conv_done_dl <= conv_done;
		end
		
	always @(posedge clk or posedge reset)
		begin
			if(reset)
				conv_rst <= 1'b0;
			else if(conv_done==1'b0 && conv_done_dl==1'b1)
				conv_rst <= 1'b1;
			else
				conv_rst <= 1'b0;
		end
				

		  
  /*********************************************************
                1st data and compare
  *********************************************************/	  
	reg [DATA_WIDTH-1:0] data_a_t1;
	

always @(posedge clk or posedge reset )
	
		begin
			 if(reset)
			  data_a_t1 <= 0;
			  else if(num_pixel==1 && delay_num_pixel==NUM_OUT_PIXEL)
				data_a_t1 <= 0;
			else if(cnt1>=NUM_ONE_PIXEL_CYCLE-5 && cnt1 <= NUM_ONE_PIXEL_CYCLE-3 && cnt2 == 2'd1 && cnt4 == 2'd1)
			  data_a_t1 <= 0;
			else if(cnt1==NUM_ONE_PIXEL_CYCLE-1)
				if(data_in>q_a)
					data_a_t1 <= data_in;
				else
					data_a_t1 <= q_a;
			else
				data_a_t1=data_a_t1;
		end
  /**********************************************************
              1st address
  **********************************************************/
  reg [POOL_ADDR_WIDTH-1:0] address_a_t1;

  always @(posedge clk  )
		begin
			delay_num_pixel <= num_pixel;
		end
	always @(posedge clk or posedge reset or posedge conv_rst)
		begin
			if(reset)
				address_a_t1 <= 0;
			else if(conv_rst)
				address_a_t1 <= 0;
				else if(num_pixel==1 && delay_num_pixel==NUM_OUT_PIXEL)
				address_a_t1 <= 0;
			else if(cnt1==NUM_ONE_PIXEL_CYCLE && cnt2==2 && cnt3==OUT_FEATURE_WIDTH_POOL && cnt4==2'd1)
				address_a_t1 <= address_a_t1-(OUT_FEATURE_WIDTH_POOL-1);
			else if(cnt1==NUM_ONE_PIXEL_CYCLE && cnt2==2 && cnt3==OUT_FEATURE_WIDTH_POOL && cnt4==2'd2)
				address_a_t1 <= address_a_t1+1;
			else if(cnt1==NUM_ONE_PIXEL_CYCLE && cnt2==2 && cnt3!=OUT_FEATURE_WIDTH_POOL)
				address_a_t1 <= address_a_t1+1;
			else
				address_a_t1 <= address_a_t1;
		end
    
    

  /***********************************************************************
              the controler of write and read
  ***********************************************************************/
	reg wr_en,rd_en;
  
	always @(posedge clk or posedge reset or posedge conv_rst)
		begin
			if(reset)
				wr_en <= 1'b0;
			else if(conv_rst)
				wr_en <= 1'b0;
			else if(conv_done==1'b0 && cnt1==NUM_ONE_PIXEL_CYCLE-5 && cnt2==2'd1 && cnt4==2'd1)
			  wr_en <= 1'b1;
			else if(conv_done==1'b0 && cnt1==NUM_ONE_PIXEL_CYCLE-1)
				wr_en <= 1'b1;
			else
				wr_en <= 1'b0;
		end
    
	always @(posedge clk or posedge reset or posedge conv_rst)
		begin
			if(reset)
				rd_en <= 1'b0;
			else if(conv_rst)
				rd_en <= 1'b0;
			else if(conv_done==1'b0 && cnt1>=NUM_ONE_PIXEL_CYCLE-4 && cnt1<=NUM_ONE_PIXEL_CYCLE-2)
				rd_en <= 1'b1;
			else
				rd_en <= 1'b0;
		end
    
	assign data_a=data_a_t1;
	assign address_a_t=address_a_t1+(onemult_compu-1)*OUT_FEATURE_WIDTH_POOL*OUT_FEATURE_WIDTH_POOL;
	
	assign wren_a=wr_en;
	assign rden_a=rd_en;
	
	assign wren_b=1'b0;
	assign rden_b=1'b0;	
	 
	 
    
endmodule
        
        
        
        
	  
	
	      
						  






