
//3涓壒寰佸浘鏀惧湪3涓狹9K瀛樺偍锛岃緭鍑涓壒寰佸浘浜や釜2涓猰ult鍋氳繍绠

//杈撳叆鐗瑰緛鍥惧ぇ灏3*13锛屽嵎绉牳澶у皬3*3锛屾闀挎槸2锛岃緭鍑虹壒寰佸浘鏄*6锛屾睜鍖栧悗鐗瑰緛鍥炬槸3*3.



//******* ADD for resource `define
`define INFEA_ONEMEM	  1
`define INFEATURE_NUM   20
`define INPUT_NUM_MEM  20  //   num of input memory 	

`define NUM_ONEMULT  10 //ONE MULT COMPUTE HOW MUCH OUT FEATURE MAP
`define NUM_MULT  2  
`define OUFEATURE_NUM  20
	//data width `define		
`define	ACCUM_DATA_WIDTH  44  		
`define	DATA_WIDTH  16  		
// number of input output feature map
	
`define IFMAP_PAR 1
`define  NUM_DSP 2   //NUM_MULT*IFMAP_PAR

	
// input output feature map size ,stride of conv
`define	IN_FEATURE_WIDTH  5
`define	KERNEL_WIDTH  3  	
`define	STRIDE  1 	
`define	OUT_FEATURE_WIDTH  3
`define	OUT_FEATURE_WIDTH_POOL  0	
// OUT_FEATURE_WIDTH = (IN_FEATURE_WIDTH- KERNEL_WIDTH)/STRIDE +1
//addr width
`define	IN_FEATURE_ADDR_WIDTH  9  		
`define	WEIGHT_ADDR_WIDTH  11  		
`define	POOL_ADDR_WIDTH  9  		

//	mainly control `define
`define	NUM_ONE_PIXEL_CYCLE_INTER	  	5  
`define	NUM_ONE_PIXEL_CYCLE	  	100
//NUM_ONE_PIXEL_CYCLE_INTER*INPUT_NUM_MEM  
`define	CNT1_BITWIDTH  7  		
`define	CNT3_BITWIDTH  3  		
`define	COUNT_SLOAD_BITWIDTH  7  		
`define	COUNT_CONV_DONE_BITWIDTH  7  			
 //count_sload == NUM_ONE_PIXEL_CYCLE-1		
//	inc_cnt1 = cnt1 < NUM_ONE_PIXEL_CYCLE  ;

//	inc_cnt3 = cnt3 < OUT_FEATURE_WIDTH_POOL;
//count_conv_done==(OUT_FEATURE_WIDTH*OUT_FEATURE_WIDTH*NUM_ONEMULT+1))
		
				  
//mainly addr generate `define	
`define	I_WIDTH  3  		
`define	J_WIDTH  3  		
`define	KM_WIDTH  3  		
`define	KN_WIDTH  3  		
		
`define	KERNEL_VALUE_WIDTH  5  	
//input_num_mem < (INPUT_NUM_MEM/IFMAP_PAR-1);20

`define NUM_MULTCOMP_BITWIDTH  2  
//	inc_num_multcomp=num_multcomp < (NUM_ONEMULT - 1);8
`define OUTPIXEL_BITWIDTH  4  
`define   ONE_MULT_COMPU_BITWIDTH 5 
//	inc_onemult_compu = onemult_compu < NUM_ONEMULT ; pool
`define INFEA_ONEMEM_BITWIDTH 5
//	inc_infea_onemem=infea_onemem < (INFEA_ONEMEM-1);
`define NUM_PIXEL_BITWIDTH 4
`define NUM_OUT_PIXEL  3*3
	
//	inc_km = km < (KERNEL_WIDTH- 1);
//	inc_kn = (km[0]==0)&&(kn < (KERNEL_WIDTH- 1))||(km[0]==1)&&(kn < (KERNEL_WIDTH- 1)-1);
//	inc_j = j < (IN_FEATURE_WIDTH - KERNEL_WIDTH)/STRIDE;
//	inc_i = i < (IN_FEATURE_WIDTH - KERNEL_WIDTH)/STRIDE;
//	inc_input_num_mem = input_num_mem < (INPUT_NUM_MEM-1);
//	inc_infea_onemem=infea_onemem < (INFEA_ONEMEM-1);
//	inc_num_multcomp=num_multcomp < (NUM_ONEMULT - 1);