
//3个特征图放在3个M9K存储，输出8个特征图交个2个mult做运算

//输入特征图大小13*13，卷积核大小3*3，步长是2，输出特征图是6*6，池化后特征图是3*3.


`define	OUTNEURON 300
`define	INNEURON 180
`define	PI 2
`define	PO 4
`define  DATA_WIDTH_FC 16
`define  ACCUM_DATA_WIDTH_FC 44

	
// number of input output feature map

`define	FC_INNEURON_ADDR_WIDTH  9  		
`define	FC_WEIGHT_ADDR_WIDTH  14		
		
`define FC_OUTNEURON_ADDR_WIDTH 9

		
`define	FC_COUNT_SLOAD_BITWIDTH  7  		
		
`define FC_COUNT_OUT_NEURON_BITWIDTH 7