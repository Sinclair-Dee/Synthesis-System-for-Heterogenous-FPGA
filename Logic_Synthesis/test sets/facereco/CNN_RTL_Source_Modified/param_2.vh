
//3涓壒寰佸浘鏀惧湪3涓狹9K瀛樺偍锛岃緭鍑涓壒寰佸浘浜や釜2涓猰ult鍋氳繍绠

//杈撳叆鐗瑰緛鍥惧ぇ灏3*13锛屽嵎绉牳澶у皬3*3锛屾闀挎槸2锛岃緭鍑虹壒寰佸浘鏄*6锛屾睜鍖栧悗鐗瑰緛鍥炬槸3*3.



//******* ADD for resource `define
`define INFEA_ONEMEM	  2
`define INFEATURE_NUM   20
`define INPUT_NUM_MEM  10  //   num of input memory 	

`define NUM_ONEMULT  1 //ONE MULT COMPUTE HOW MUCH OUT FEATURE MAP
`define NUM_MULT  25
`define OUFEATURE_NUM  25	
	//data width `define		
`define	ACCUM_DATA_WIDTH  44  		
`define	DATA_WIDTH  16  		
// number of input output feature map
	
`define IFMAP_PAR 2
`define  NUM_DSP 50  //NUM_MULT*IFMAP_PAR

	
// input output feature map size ,stride of conv
`define	IN_FEATURE_WIDTH  14
`define	KERNEL_WIDTH  3 	
`define	STRIDE  1 	
`define	OUT_FEATURE_WIDTH  12
`define	OUT_FEATURE_WIDTH_POOL  6	

`define	IN_FEATURE_WIDTH_W  14
`define	IN_FEATURE_WIDTH_H  12
`define	OUT_FEATURE_WIDTH_W  12
`define	OUT_FEATURE_WIDTH_H  10
`define	OUT_FEATURE_WIDTH_POOL_W  6	
`define	OUT_FEATURE_WIDTH_POOL_H  5



// OUT_FEATURE_WIDTH = (IN_FEATURE_WIDTH- KERNEL_WIDTH)/STRIDE +1
//addr width
`define	IN_FEATURE_ADDR_WIDTH  9  		
`define	WEIGHT_ADDR_WIDTH 9  		
`define	POOL_ADDR_WIDTH  9  		

//	mainly control `define
`define	NUM_ONE_PIXEL_CYCLE_INTER	  	10  
`define	NUM_ONE_PIXEL_CYCLE	  	50






//NUM_ONE_PIXEL_CYCLE_INTER*INPUT_NUM_MEM /IFMAP_PAR 
`define	CNT1_BITWIDTH  6  		
`define	CNT3_BITWIDTH  3  		
`define	COUNT_SLOAD_BITWIDTH  6  		
`define	COUNT_CONV_DONE_BITWIDTH  8  			
 //count_sload == NUM_ONE_PIXEL_CYCLE-1		
//	inc_cnt1 = cnt1 < NUM_ONE_PIXEL_CYCLE  ;

//	inc_cnt3 = cnt3 < OUT_FEATURE_WIDTH_POOL;
//count_conv_done==(OUT_FEATURE_WIDTH*OUT_FEATURE_WIDTH*NUM_ONEMULT+1))
		
				  
//mainly addr generate `define	
`define	I_WIDTH  4  		
`define	J_WIDTH  4  		
`define	KM_WIDTH  2  		
`define	KN_WIDTH  2  		
		
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
`define NUM_OUT_PIXEL  12*10
	
//	inc_km = km < (KERNEL_WIDTH- 1);
//	inc_kn = (km[0]==0)&&(kn < (KERNEL_WIDTH- 1))||(km[0]==1)&&(kn < (KERNEL_WIDTH- 1)-1);
//	inc_j = j < (IN_FEATURE_WIDTH - KERNEL_WIDTH)/STRIDE;
//	inc_i = i < (IN_FEATURE_WIDTH - KERNEL_WIDTH)/STRIDE;
//	inc_input_num_mem = input_num_mem < (INPUT_NUM_MEM-1);
//	inc_infea_onemem=infea_onemem < (INFEA_ONEMEM-1);
//	inc_num_multcomp=num_multcomp < (NUM_ONEMULT - 1);