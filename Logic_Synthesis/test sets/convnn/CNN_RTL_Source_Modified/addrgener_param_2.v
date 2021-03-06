
`include "param_2.vh"
module addrgener_param_2 (
// Global signals
	 clk,
    enable,
	 reset,	
	 addra_temp,
	 addrb_temp
	);	
	
//******* ADD for resource parameter
parameter INFEA_ONEMEM	=`INFEA_ONEMEM;

parameter INPUT_NUM_MEM=`INPUT_NUM_MEM;	

parameter NUM_ONEMULT=`NUM_ONEMULT;//ONE MULT COMPUTE HOW MUCH OUT FEATURE MAP

	
// number of input output feature map
	
parameter IN_FEATURE_ADDR_WIDTH=`IN_FEATURE_ADDR_WIDTH;
parameter OUT_FEATURE_WIDTH_POOL=`OUT_FEATURE_WIDTH_POOL;

	parameter OUT_FEATURE_WIDTH=`OUT_FEATURE_WIDTH;
// input output feature map size ,stride of conv
parameter	IN_FEATURE_WIDTH=`IN_FEATURE_WIDTH;	

parameter	IN_FEATURE_WIDTH_W=`IN_FEATURE_WIDTH_W;	
parameter	IN_FEATURE_WIDTH_H=`IN_FEATURE_WIDTH_H;	
parameter	KERNEL_WIDTH=`KERNEL_WIDTH;	
parameter	STRIDE=`STRIDE;	
	

//mainly addr generate parameter	
parameter	I_WIDTH=`I_WIDTH;		
parameter	J_WIDTH=`J_WIDTH;		
parameter	KM_WIDTH=`KM_WIDTH;		
parameter	KN_WIDTH=`KN_WIDTH;		

parameter	KERNEL_VALUE_WIDTH=`KERNEL_VALUE_WIDTH;	
	parameter NUM_MULTCOMP_BITWIDTH=`NUM_MULTCOMP_BITWIDTH;


parameter INFEA_ONEMEM_BITWIDTH=`INFEA_ONEMEM_BITWIDTH;

parameter IFMAP_PAR= `IFMAP_PAR ;

// exteral signals
	input clk;
	input enable;
	input reset;
	
//internal signals	
	reg [IN_FEATURE_ADDR_WIDTH - 1 : 0] addra;
	reg [IN_FEATURE_ADDR_WIDTH - 1 : 0] addrb;
	

	output reg [IN_FEATURE_ADDR_WIDTH - 1 : 0] addra_temp;
	output [IN_FEATURE_ADDR_WIDTH - 1 : 0]   addrb_temp;		
	wire [IN_FEATURE_ADDR_WIDTH - 1 : 0] addrb_plus;	
	
	
//state machine signals
	reg [I_WIDTH-1:0] i ;
	reg [J_WIDTH-1:0] j ;
	reg [KM_WIDTH-1:0] km ;
	reg [KN_WIDTH-1:0] kn ;
	reg inc_km ;
	reg inc_kn ;
	reg inc_j ;
	reg inc_i ;
   reg over=0;	
	
	reg inc_input_num_mem;
	reg [KERNEL_VALUE_WIDTH-1:0]input_num_mem;
	
	wire [I_WIDTH:0] sum_1 ;
	wire [J_WIDTH:0] sum_2 ;	
//************ADD FOR DEEP PARameter
	reg inc_infea_onemem;
	reg [INFEA_ONEMEM_BITWIDTH-1:0]infea_onemem;
	reg inc_num_multcomp;
	reg [NUM_MULTCOMP_BITWIDTH-1:0]num_multcomp;
always @(*)
begin
	inc_km = km < (KERNEL_WIDTH- 1);
	inc_kn = (km[0]==0)&&(kn < (KERNEL_WIDTH- 1))||(km[0]==1)&&(kn < (KERNEL_WIDTH- 1)-1);
	inc_j = j < (IN_FEATURE_WIDTH_W - KERNEL_WIDTH)/STRIDE;
	inc_i = i < (IN_FEATURE_WIDTH_H - KERNEL_WIDTH)/STRIDE;
	inc_input_num_mem = input_num_mem < (INPUT_NUM_MEM/IFMAP_PAR-1);
	inc_infea_onemem=infea_onemem < (INFEA_ONEMEM-1);
	inc_num_multcomp=num_multcomp < (NUM_ONEMULT - 1);
end

//main logic
always @ (posedge clk or posedge reset)
begin
	if(reset) 
		begin
			i <= 0;
			j <= 0; 
			km <= 0;
			kn <= 0;
			infea_onemem <= 0;	 
			input_num_mem<=0;
			over<=0;
			num_multcomp <= 0;		
		end		  
	else if(!over&&enable)
			begin
				    
				if (inc_kn)
					kn <= kn + 2;
								
				else if(inc_km)
					begin
						kn <= (km[0]==0)?1:0;
						km <= km + 1;
								  
					end
					//////**********
					else  if(inc_infea_onemem)
							begin
								infea_onemem <= infea_onemem + 1;
								km <= 0;
								kn <= 0;
							end
					
				///////////////////	
						else  if(inc_input_num_mem)
							begin
								input_num_mem<=input_num_mem+1;
								km <= 0;
								kn <= 0;
								infea_onemem <= 0;
							end
								else	if (inc_j)
									begin
										km <= 0;
										kn <= 0;
										j <= j + 1;
										input_num_mem <= 0;
										infea_onemem <= 0;
									end
										else if (inc_i)
											begin
												km <= 0;
												kn <= 0;
												j <= 0;
								 //吃了大亏，写代码要仔细，千万要小心
												input_num_mem <= 0;
												i<=i+1;	
												infea_onemem <= 0;
																
											end
												else if(inc_num_multcomp)
												 begin
												 km <= 0;
												kn <= 0;
												j <= 0;
												input_num_mem <= 0;
												i<=0;	
												infea_onemem <= 0;	
											   num_multcomp <= num_multcomp + 1;	
												 end
												
												else 
												over <= 1;
		end  
end

// compute address
assign sum_1 = i*STRIDE+km;
assign sum_2 = j*STRIDE+kn;

always @ (posedge clk)
begin
    
	addra_temp <= IN_FEATURE_WIDTH_W*sum_1+sum_2 + IN_FEATURE_WIDTH_H * IN_FEATURE_WIDTH_W * infea_onemem ;	
	addra      <= addra_temp;
end

assign addrb_plus = ((kn==1)&&(km[0]==1))?(IN_FEATURE_WIDTH_W - KERNEL_WIDTH+1):1;
assign addrb_temp = addra_temp+addrb_plus;

always @ (posedge clk) 
	addrb <= addrb_temp;
	  
endmodule

