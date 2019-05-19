
`define INFEA_ONEMEM	  1
`define INFEATURE_NUM   4
`define INPUT_NUM_MEM  4  //   num of input memory 	

`define NUM_ONEMULT  1 //ONE MULT COMPUTE HOW MUCH OUT FEATURE MAP
`define NUM_MULT  14
`define OUFEATURE_NUM  14 	
	//data width `define		
`define	ACCUM_DATA_WIDTH  44  		
`define	DATA_WIDTH  16  		
// number of input output feature map
	
`define IFMAP_PAR 4
`define  NUM_DSP 56   //NUM_MULT*IFMAP_PAR

	
// input output feature map size ,stride of conv
`define	IN_FEATURE_WIDTH  16
`define	KERNEL_WIDTH  3 	
`define	STRIDE  1 	
`define	OUT_FEATURE_WIDTH  14
`define	OUT_FEATURE_WIDTH_POOL  7	
// OUT_FEATURE_WIDTH = (IN_FEATURE_WIDTH- KERNEL_WIDTH)/STRIDE +1
//addr width
`define	IN_FEATURE_ADDR_WIDTH  9  		
`define	WEIGHT_ADDR_WIDTH  9  		
`define	POOL_ADDR_WIDTH  9  		

//	mainly control `define
`define	NUM_ONE_PIXEL_CYCLE_INTER	  	5  
`define	NUM_ONE_PIXEL_CYCLE	  	5
//NUM_ONE_PIXEL_CYCLE_INTER*INPUT_NUM_MEM /IFMAP_PAR 
`define	CNT1_BITWIDTH  5  		
`define	CNT3_BITWIDTH  4  		
`define	COUNT_SLOAD_BITWIDTH  5 		
`define	COUNT_CONV_DONE_BITWIDTH  8  			
`define	I_WIDTH  4  		
`define	J_WIDTH  4  		
`define	KM_WIDTH  3  		
`define	KN_WIDTH  3  		
		
`define	KERNEL_VALUE_WIDTH  3  	
//input_num_mem < (INPUT_NUM_MEM/IFMAP_PAR-1);

`define NUM_MULTCOMP_BITWIDTH  2  
//	inc_num_multcomp=num_multcomp < (NUM_ONEMULT - 1);
`define OUTPIXEL_BITWIDTH  8  
`define   ONE_MULT_COMPU_BITWIDTH  2 
//	inc_onemult_compu = onemult_compu < NUM_ONEMULT ; pool
`define INFEA_ONEMEM_BITWIDTH 2
//	inc_infea_onemem=infea_onemem < (INFEA_ONEMEM-1);
`define NUM_PIXEL_BITWIDTH 8
`define NUM_OUT_PIXEL  196
