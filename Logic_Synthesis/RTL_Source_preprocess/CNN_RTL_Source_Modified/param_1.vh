
//3个特征图放在3个M9K存储，输出8个特征图交个2个mult做运算

//输入特征图大小13*13，卷积核大小3*3，步长是2，输出特征图是6*6，池化后特征图是3*3.



//******* ADD for resource `define
`define INFEA_ONEMEM	  1    
`define INFEATURE_NUM   1
`define INPUT_NUM_MEM  1  //   num of input memory 	

`define NUM_ONEMULT  1
`define NUM_MULT  5
`define OUFEATURE_NUM 5 	
	//data width `define		
`define	ACCUM_DATA_WIDTH  44  		
`define	DATA_WIDTH  16  		
// number of input output feature map
	
`define IFMAP_PAR 1
`define  NUM_DSP 5   //NUM_MULT*IFMAP_PAR

	
// input output feature map size ,stride of conv
`define	IN_FEATURE_WIDTH  29
`define	KERNEL_WIDTH  5 	
`define	STRIDE  2
`define	OUT_FEATURE_WIDTH  13
`define	OUT_FEATURE_WIDTH_POOL  0
// OUT_FEATURE_WIDTH = (IN_FEATURE_WIDTH- KERNEL_WIDTH)/STRIDE +1
//addr width
`define	IN_FEATURE_ADDR_WIDTH  10 		
`define	WEIGHT_ADDR_WIDTH  9  		
`define	POOL_ADDR_WIDTH  9  		

//	mainly control `define
`define	NUM_ONE_PIXEL_CYCLE_INTER	  	13  
`define	NUM_ONE_PIXEL_CYCLE	  	13
//NUM_ONE_PIXEL_CYCLE_INTER*INPUT_NUM_MEM /IFMAP_PAR 
`define	CNT1_BITWIDTH  4  		
`define	CNT3_BITWIDTH  4  		
`define	COUNT_SLOAD_BITWIDTH  4  		
`define	COUNT_CONV_DONE_BITWIDTH  8  			
 //count_sload == NUM_ONE_PIXEL_CYCLE-1		
//	inc_cnt1 = cnt1 < NUM_ONE_PIXEL_CYCLE  ;

//	inc_cnt3 = cnt3 < OUT_FEATURE_WIDTH_POOL;
//count_conv_done==(OUT_FEATURE_WIDTH*OUT_FEATURE_WIDTH*NUM_ONEMULT+1))
		
`define COUNT_WRITEBIT 4

//mainly addr generate `define	
`define	I_WIDTH  4 		
`define	J_WIDTH  4  		
`define	KM_WIDTH  3  		
`define	KN_WIDTH  3  		
		
`define	KERNEL_VALUE_WIDTH  1  	
//input_num_mem < (INPUT_NUM_MEM/IFMAP_PAR-1);

`define NUM_MULTCOMP_BITWIDTH  2  
//	inc_num_multcomp=num_multcomp < (NUM_ONEMULT - 1);
`define OUTPIXEL_BITWIDTH  8  
	//inc_num_outpixel =num_outpixel <(OUT_FEATURE_WIDTH*OUT_FEATURE_WIDTH-1);
`define   ONE_MULT_COMPU_BITWIDTH  2 
//	inc_onemult_compu = onemult_compu < NUM_ONEMULT ; pool
`define INFEA_ONEMEM_BITWIDTH 2
//	inc_infea_onemem=infea_onemem < (INFEA_ONEMEM-1);
`define NUM_PIXEL_BITWIDTH 8
//	inc_num_pixel =num_pixel <NUM_OUT_PIXEL;
`define NUM_OUT_PIXEL  169
	
//	inc_km = km < (KERNEL_WIDTH- 1);
//	inc_kn = (km[0]==0)&&(kn < (KERNEL_WIDTH- 1))||(km[0]==1)&&(kn < (KERNEL_WIDTH- 1)-1);
//	inc_j = j < (IN_FEATURE_WIDTH - KERNEL_WIDTH)/STRIDE;
//	inc_i = i < (IN_FEATURE_WIDTH - KERNEL_WIDTH)/STRIDE;
//	inc_input_num_mem = input_num_mem < (INPUT_NUM_MEM-1);
//	inc_infea_onemem=infea_onemem < (INFEA_ONEMEM-1);
//	inc_num_multcomp=num_multcomp < (NUM_ONEMULT - 1);