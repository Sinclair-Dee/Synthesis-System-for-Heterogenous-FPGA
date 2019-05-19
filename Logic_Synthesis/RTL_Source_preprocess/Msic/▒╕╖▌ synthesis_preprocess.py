#code=utf-8

#******************************************************
# 整个LeNet5 CNN RTL 预处理 正常输出
#remove:`ifndef ALTERA_RESERVED_QIS-->>
#26
#remove:// synopsys translate_off-->>
#26
#remove:`endif-->>
#26
#remove:aclr3,-->>
#3
#3
#3
#.aclr3(clear)-->>
#6
#16
#4
#replace：altmult_accum -->>
#4
#*******************************************************

import os	
import codecs
import re


def merge1():
	#获取source_12_14修改版文件夹中的文件列表
	filedir = os.getcwd() + '/CNN_RTL_Source_Modified'
	filenames = os.listdir(filedir)
	
	#合成到cnn_top.v中
	outfile = open("cnn_top1.v","w")
	
	#遍历文件名
	for filename in filenames:
		filepath = filedir + '/' + filename
		print(filepath)
		
		#遍历单个文件，按行拷贝
		with codecs.open(filepath,encoding='utf-8') as infile:
			for line in infile:
				outfile.writelines(line)
		outfile.write('\n')

	outfile.close()


def merge2():
    outfile  = open("cnn_top2.v","w")
    #打开生成的cnn_top1
    infile = open("cnn_top1.v","r")
    for line in infile:
        if line.count('\n') == len(line):
            pass
        else:            
            outfile.writelines(line)
            
    outfile.write('\n')
    outfile.close()
    infile.close()

def remove():
    outfile  = open("cnn_top3.v","w")
    #打开生成的cnn_top2
    infile = open("cnn_top2.v","r")

    rem1 = rem2 = rem3 =0 
    rem4 = rem5 = rem6 = 0
    rem7 = rem8 = rem9 = 0   
    rem10 =0 
    for line in infile:
        if "`ifndef ALTERA_RESERVED_QIS" in line:
            rem1 +=1
            pass
        elif "// synopsys translate_off" in line:
            rem2 +=1
            pass
        elif "`endif" in line:
            rem3 +=1
            pass
        elif "aclr3," in line:
            rem4 +=1
            pass
        elif "input	  aclr3" in line:
            rem5 +=1
            pass
        elif "tri0	  aclr3" in line:
            rem6 +=1
            pass 
        elif ".aclr3(clear_mult)" in line:
            rem7 +=1
            pass
        elif ".aclr3(clear_layer2_mult)" in line:
            rem8 +=1
            pass
        elif ".aclr3(clear_layer3_mult)" in line:
            rem9 +=1
            pass 
        elif "altmult_add_component.lpm_type = \"altmult_add\"" in line:
            rem10 +=1
            temp ="		altmult_add_component.lpm_type = \"altmult_accum\",\n"    
            outfile.writelines(temp)  
        else:            
            outfile.writelines(line)

    outfile.write('\n')
    outfile.close()
    infile.close()		
    print("remove:`ifndef ALTERA_RESERVED_QIS-->>")
    print(rem1) 
    print("remove:// synopsys translate_off-->>" )
    print(rem2) 
    print("remove:`endif-->>")
    print(rem3)
    print("remove:aclr3,-->>")
    print(rem4)
    print(rem5)
    print(rem6)
    print(".aclr3(clear)-->>")
    print(rem7)
    print(rem8)
    print(rem9)    
    print("replace：altmult_accum -->>")
    print(rem10)

def matr():#ok!!!
    outfile  = open("cnn_top4.v","w")
    #打开生成的cnn_top3
    infile = open("cnn_top3.v","r")
    
    for line in infile:
        outfile.writelines(line)
        match0 = re.match('.*[wire,reg].*\[.*\].*=.*\[.*\].*;',line) #处理仅仅是赋值语句的情况。
        if match0:
            #print(match0.group())
            #pass
            continue
        match1 = re.match('[//,assign].*[wire,reg].*\[.*\].*\[.*\].*;',line) #'reg[1: 0]  layer2_weight_data_a[1ad :ewt]  ,   '
        if match1:
            #print(match1.group())
            continue
            #pass
        
        match2 = re.match('.*<=\s*{.*\[.*\],.*\[.*\]}.*;',line) #layer2_result_jiewei_6 <=  {layer2_result_6[43],layer2_result_6[26:12]};   '
        if match2:
            #print(match2.group())
            continue
            #pass
        
        match3 = re.match('.*[wire,reg].*\[.*\].*\[.*\].*;',line) #'reg[1: 0]  layer2_weight_data_a[1ad :ewt]  ,   '
        if match3:
            print(match3.group())
            #pass
            #continue

    outfile.write('\n')
    outfile.close()
    infile.close()


def error():
    outfile  = open("cnn_top.v","w")
    #打开生成的cnn_top3
    infile = open("cnn_top4.v","r")
    
    for line in infile:
        match0 = re.match('.*\.result\s*\(\s*layer._result_.*_\d*\s*\),',line) #'  .result ( layer3_result_ur_0 ),'
        if match0:
            print(match0.group())
            temp = line.replace(',','')
            print(temp)
            outfile.writelines(temp)
        else:
            outfile.writelines(line)        
    outfile.write('\n')
    outfile.close()
    infile.close()
        
def main():
	report=open("synReport.txt","w")
	report.writelines('预处理RTL代码...\n\n')
	report.writelines('merge...\n\n')	
	merge1()
	merge2()
	report.writelines('修改电路端口...\n\n')	
	remove()
	report.writelines('二维矩阵转化...\n\n')		
	matr()
	report.writelines('错误检测与修改...\n\n')	
	error()
	report.writelines('预处理结束!\n\n')
	os.remove('cnn_top1.v')
	os.remove('cnn_top2.v')
	os.remove('cnn_top3.v')
	os.remove('cnn_top4.v')
	#os.remove('cnn_top.clk') #删除其他的文件
	#os.remove('syn_st')
	report.close()
main()
