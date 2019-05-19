#code=utf-8

import os	
import re

#from lib import *    lib中代码太少，后期调整放到本文件中

#全局变量
global start_point_arrow
global end_point_arrow
global all_file_path
global verilog_file_path
global unroll_char
start_point_arrow = ' start--------------------------------------------------->>>>>'
end_point_arrow   = ' end--------------------------------------------------->>>>>'
origin_file_path = '/CNN_RTL_Source_Modified'#要处理的源文件放在这个文件夹中。
verilog_file_path = '/all_verilog_file'
verilog_file_path_processed = '/all_processed_verilog_file'
unroll_char = '_unroll_'


#Stack
#功能：用于处理某些需要成对出现的语法元素并且有嵌套的情况
#输入参数：无
#输出参数：parameter_vh
#备注：
#
class Stack(object):
    def __init__(self):
        self.items = []

    def isEmpty(self):
        return len(self.items) ==0
    def push(self,item):
        self.items.append(item)
    def pop(self):
        return self.items.pop()
    def peek(self):
        if not isEmpty():
            return  self.items[len(self.items)-1]
    def size(self):
        return len(self.items)

##################################################################################################
##参数提取，不输出文件
##
################################################################################################## 

#vh2txt
#功能：将.vh文件中有用的信息提取到同名.txt文件中，并将.vh文件名存入parameter_vh列表中
#输入参数：无
#输出参数：parameter_vh
#备注：生成的.txt去掉了左右空格，方便后续处理
#
def vh2txt():
    print('vh2txt'+ start_point_arrow)
    #获取本目录下CNN_RTL_Source_Modified文件夹下的文件名列表
    filedir = os.getcwd() + origin_file_path
    filenames = os.listdir(filedir)
    count_para_at_vh = 0
    parameter_vh = []
    #遍历文件名列表
    for filename in filenames:
        match0 = re.match('.*.vh',filename) 
        if match0:#只处理.vh文件
            parameter_vh.append(filename[:-3])
            filepath  = os.getcwd() + origin_file_path + '/'+ filename
            infile = open(filepath,'r',encoding='utf-8')
            filename1=filename[:-3]+'.txt'
            filepath1 = os.getcwd() + verilog_file_path + '/'+ filename1            
            outfile = open(filepath1,"w",encoding='utf-8')
            #遍历单个.vh文件，按行处理
            for line in infile:
                match1 = re.match('`define.*',line)
                match2 = re.match('.*//.*', line)
                if match1:
                    count_para_at_vh = count_para_at_vh + 1
                    if match2:
                        line1 = (((line.split("//")[0])[8:]).lstrip()).rstrip()+'\n'
                        #print(line1)
                        outfile.writelines(line1)
                    else:
                        line1 = ((line[8:]).lstrip()).rstrip()+ '\n'
                        #print(line1)
                        outfile.writelines(line1)
            outfile.close()
            infile.close()
    print('提取出' + str(count_para_at_vh) + '个参数')        
    print('vh2txt'+ end_point_arrow)
    return parameter_vh #返回include索引列表


#parameter_vh_lib
#功能：提取.txt文件所有的信息存入二维字典parameter_vh_all中
#输入参数：parameter_vh
#输出参数：parameter_vh_all
#
def parameter_vh_lib(parameter_vh):
    print('parameter_vh_lib'+ start_point_arrow)
    #获取本目录下CNN_RTL_Source_Modified文件夹下的文件名列表
    filedir = os.getcwd() + verilog_file_path
    parameter_vh_all = {}
    count_para_at_txt = 0
    for parameter in parameter_vh:
        vh_filepath = filedir + '/' + parameter + '.txt'
        infile_vh = open(vh_filepath,'r',encoding='utf-8')
        parameter_vh_lib = {}
        for line_vh in infile_vh:
            #提取参数名
            #注意要去掉参数名的左右空格，否者后续匹配是会受到影响。
            parameter_key = ((line_vh.split(" ")[0]).lstrip()).rstrip()
            #提取参数名 
            parameter_value = 0
            for i in range(len(line_vh) - len(parameter_key) +1 ):
                if line_vh[i+len(parameter_key)-1] <= '9' and  line_vh[i+len(parameter_key)-1] >='0':
                    parameter_value = parameter_value*10+int(line_vh[i+len(parameter_key)-1])
            print(line_vh)
            print(parameter_key)
            print(str(parameter_value))
            parameter_vh_lib[parameter_key]=str(parameter_value)
            count_para_at_txt = count_para_at_txt + 1
            
            #正则匹配方法 效果不好    
            #p_value = re.compile('[0-9]')
            #parameter_value_temp = p_value.findall(line_vh)
            #parameter_value = 0
            #for i in range(len(parameter_value_temp)):
            #    parameter_value = parameter_value*10+int(parameter_value_temp[i])
            
            #p_key = re.compile('\w')
            #parameter_key_temp = p_key.findall(line_vh)
            #parameter_key = ''
            #for i in range(len(parameter_key_temp)-len(parameter_value_temp)):
            #    parameter_key = parameter_key + parameter_key_temp[i]
            
            #print(parameter_key)
            #print(str(parameter_value))
            #if parameter_key != line_vh.split(" ")[0]:
            #    print(parameter_key + '-----------------------------》》》》》' +line_vh.split(" ")[0])
            
        parameter_vh_all[parameter]=parameter_vh_lib
    print(parameter_vh_all)
    print('处理了' + str(count_para_at_txt) + '个参数')
    print('parameter_vh_lib'+ end_point_arrow)
    return parameter_vh_all #返回二维字典


##################################################################################################
##同时对单个Verilog以及合在一起的Verilog做处理
##
################################################################################################## 

#merge_verilog
#功能：合并verilog并替换参数
#输入参数：parameter_vh  parameter_vh_all
#输出参数：无
#备注
#cnn_top0.v:由所有的.文件合成
#cnn_top1：在cnn_top0的基础上替换掉参数
#0_.v：替换参数后的单个Verilog
#all_verilog_file 所有的Verilog替换参数后存放在这个文件夹中，之后所有的处理都在这个基础上进行 
#   仅仅替换参数，不会去掉"include .vh"因为后续要用到
def merge_verilog(parameter_vh,parameter_vh_all):
    print('merge_verilog' + start_point_arrow)
    count_verilog_file = 0
    outfile0 = open("cnn_top0.v","w",encoding='utf-8') #必须进行编码为utf-8。
    outfile1 = open("cnn_top1.v","w",encoding='utf-8') 

    #处理之前先清空all_verilog_file文件夹
    filedir_temp = os.getcwd() + verilog_file_path
    filenames_temp = os.listdir(filedir_temp)
    for filename_temp in filenames_temp:
        filepath_temp  = os.getcwd() + verilog_file_path + '/'+ filename_temp
        #print(filepath_temp)
        os.remove(filepath_temp)
    #获取本目录下CNN_RTL_Source_Modified文件夹下的文件名列表        
    filedir = os.getcwd() + origin_file_path
    filenames = os.listdir(filedir)
    #遍历文件名列表，只处理.v文件
    for filename in filenames:
        match0 = re.match('.*.vh',filename) 
        if match0:
            print(filename)
            continue
        match1 = re.match('.*.txt',filename) 
        if match1:
            print(filename)
            continue
        
        outfile_each_one = open("all_verilog_file/"+filename + '0_.v',"w",encoding='utf-8')
        count_verilog_file = count_verilog_file + 1
        
        include_vh = 'noneofinclude'
        line_number = 0
        include_find_index = 0
                
        filepath = filedir + '/' + filename
        #print(filepath)
        infile_temp = open(filepath,'r',encoding='utf-8')
        infile = open(filepath,'r',encoding='utf-8')

        #遍历单个.v文件，在前20行范围内判断是否有引用
        for line in infile_temp:
            include_find_index = include_find_index+1
            if '`include' in line:
                for parameter in parameter_vh: 
                    if parameter in line:
                        include_vh = parameter
                        print(filename+'--'+'第'+str(include_find_index)+"行"+'include'+' '+parameter+'.vh')
                        break
                break
            if include_find_index >=20:
                break
            
        #遍历单个.v文件，按行处理
        for line in infile:
            line_number = line_number+1        
            if line_number ==include_find_index and include_vh != 'noneofinclude':
                #include<>开始只是打印在每个Verilog上以及outfile0上，outfile1去掉了这行，
                #后来不需要这一步，就改成了每个都打印
                outfile_each_one.writelines(line) 
                outfile0.writelines(line)
                outfile1.writelines(line)
                continue
            #outfile0是该行直接打印
            outfile0.writelines(line)
            #若出现参数则替换参数
            if include_vh in parameter_vh_all and re.match('.*parameter.*=.*`.*;.*',line):#parameter INFEA_ONEMEM	=`INFEA_ONEMEM;
                for parameter_line_item in parameter_vh_all.get(include_vh): #判断所有参数是否出现这一行
                    parameter_line_item_temp =  '`'+ parameter_line_item
                    if parameter_line_item_temp in line:
                        print(parameter_line_item_temp)
                        parameter_line_item_num = (parameter_vh_all.get(include_vh)).get(parameter_line_item)#获取数值
                        print(parameter_line_item_num)

                        #以下三种方式都可以替换成功，注意获取的是返回值，line作为实参传入，本身不会改变
                        #方法1
                        #line_replace = re.compile(parameter_line_item_temp+'\s*;')
                        #line_replace.sub(parameter_line_item_num,line)
                        #方法2
                        #line.replace(parameter_line_item_temp,parameter_line_item_num)
                        #方法3
                        line = re.sub(parameter_line_item_temp+'\s*;',parameter_line_item_num+';',line)
                        print(line)
                        continue #找到与该行相匹配的参数就退出查找
            ##outfile1和每个Verilog上是替换参数后打印
            outfile1.writelines(line)
            outfile_each_one.writelines(line)
        infile_temp.close()
        infile.close()
        outfile_each_one.close()
        
        outfile0.writelines('\n')
        outfile1.writelines('\n')
        
    outfile0.close()
    outfile1.close()
    print('共有'+ str(count_verilog_file) + "个Verilog文件")
    print('merge_verilog' + end_point_arrow)

    
#matr_process
#功能：读取all_verilog_file文件中的.v文件进行二维矩阵展开
#输入参数：parameter_vh,parameter_vh_all
#输出参数：无
#备注
#.v0_.v0_.v 二维矩阵展开 后的Verilog 同样存放在all_verilog_file  文件夹中
#    展开使用_unroll_后缀，后期的处理都要与之一致
#cnn_top2:.v0_.v0_.v合到一块
#
def matr_process(parameter_vh,parameter_vh_all):
    print('matr_process' + start_point_arrow)
    filedir = os.getcwd() + verilog_file_path
    filenames = os.listdir(filedir)
    count_0_file = 0
    count_0_0_file = 0
    outfile_merge = open("cnn_top2.v","w",encoding='utf-8') #这个地方必须进行编码。
    various_context_table = {}
            
    #遍历文件名
    for filename in filenames:
        match0 = re.match('.*.v0_.v',filename) 
        if match0:
            count_0_file = count_0_file + 1
            infilepath = filedir + '/' + filename
            outfilepath = filedir + '/' + filename + '0_.v'
            include_vh = 'noneofinclude'
            include_find_index = 0
            various_context_table[filename[:-6]] = []
            #遍历单个infile_temp文件，只用来判断是否include.vh
            infile_temp = open(infilepath,'r',encoding='utf-8')
            print(filename)
            for line in infile_temp:
                include_find_index = include_find_index+1
                if '`include' in line:
                    for parameter in parameter_vh: 
                        if parameter in line:
                            include_vh = parameter
                            break
                    break
                if include_find_index >=10:
                    break
            infile_temp.close()
            #遍历单个文件，按行处理后打印到outfile中
            infile = open(infilepath,'r',encoding='utf-8')
            outfile = open(outfilepath,"w",encoding='utf-8')
            for line in infile:
                match0 = re.match('.*[wire,reg].*\[.*\].*=.*\[.*\].*;',line) #处理仅仅是赋值语句的情况。
                if match0:
                    #print(match0.group())
                    #pass
                    outfile.writelines(line)
                    outfile_merge.writelines(line)
                    continue
                match1 = re.match('[//,assign].*[wire,reg].*\[.*\].*\[.*\].*;',line) #'reg[1: 0]  layer2_weight_data_a[1ad :ewt]  ,   '
                if match1:
                    #print(match1.group())
                    outfile.writelines(line)
                    outfile_merge.writelines(line)
                    continue
                    #pass
                match2 = re.match('.*<=\s*{.*\[.*\],.*\[.*\]}.*;',line) #layer2_result_jiewei_6 <=  {layer2_result_6[43],layer2_result_6[26:12]};   '
                if match2:
                    #print(match2.group())
                    outfile.writelines(line)#跳过之前先打印
                    outfile_merge.writelines(line)
                    continue
                    #pass
                match3 = re.match('.*[wire,reg].*\[.*\].*\[.*\].*;',line) #'reg[1: 0]  layer2_weight_data_a[1ad :ewt]  ,   '
                if match3:
                    print(match3.group())
                    idx_flag = 0
                    idx_count = 0
                    
                    for i_idx in range(len(line)):
                        if line[i_idx] =="[" or line[i_idx] =="]":
                            idx_flag=idx_flag+1
                        idx_count = idx_count +1
                        if idx_flag >= 3:
                            break    
                    line_temp = line[idx_count-1:]
                    print(line_temp)
                    print(include_vh)
                    if include_vh in parameter_vh_all:
                        for parameter_line_item in parameter_vh_all.get(include_vh): #判断所有参数是否出现这一行
                            if parameter_line_item in line_temp:
                                print(parameter_line_item)
                                parameter_line_item_num = (parameter_vh_all.get(include_vh)).get(parameter_line_item)#获取数值
                                print(parameter_line_item_num)

                                #提取出来该变量，处理上下文相关的情况
                                #print("haha___________________________________________________________________>>>>>>>>")
                                #print(line)
                                #print(((((line[:idx_count-1]).lstrip()).rstrip()).split("[")[-1]).split()[-1])
                                various_context = ((((line[:idx_count-1]).lstrip()).rstrip()).split("[")[-1]).split()[-1]
                                various_context_table[filename[:-6]].append(various_context)
                                
                                
                    unroll_char = '_unroll_'
                    outfile.writelines("//"+(line.lstrip()).rstrip("\n")+"展开-->"+ parameter_line_item_num+'\n')
                    outfile_merge.writelines("//"+(line.lstrip()).rstrip("\n")+"展开-->"+ parameter_line_item_num +'\n')
            
                    for i_unroll_dex in range(int(parameter_line_item_num)):
                        line_unroll = (line[:idx_count-1]).rstrip() + unroll_char +str(i_unroll_dex) +";"+'\n'#注意去掉中间的空格
                        outfile.writelines(line_unroll)
                        outfile_merge.writelines(line_unroll)
                        #print(line_unroll)  
                    #pass
                    continue
                match4 = re.match('\s*wire\s*\w*\s*\[.*:.*\];',line)# wire	 rden_a_use	[PO-1:0];
                if match4:
                    #print("hehe------------------------------------------------------------>")
                    #print(line)
                    idx_count = 0
                    for i_idx in range(len(line)):
                        if line[i_idx] =="[":
                            break
                        idx_count = i_idx+1
                    #line_temp = line[idx_count:]
                    #print(line_temp)
                    if include_vh in parameter_vh_all:
                        for parameter_line_item in parameter_vh_all.get(include_vh): #判断所有参数是否出现这一行
                            if parameter_line_item in line:
                                print(parameter_line_item)
                                parameter_line_item_num = (parameter_vh_all.get(include_vh)).get(parameter_line_item)#获取数值
                                print(parameter_line_item_num)

                                #提取出来该变量，处理上下文相关的情况
                                #print("haha___________________________________________________________________>>>>>>>>")
                                #print(line)
                                #print((((line[:idx_count-1]).lstrip()).rstrip()).split()[-1])
                                various_context = (((line[:idx_count-1]).lstrip()).rstrip()).split()[-1]
                                various_context_table[filename[:-6]].append(various_context)
                                
                                
                    unroll_char = '_unroll_'
                    outfile.writelines("//"+(line.lstrip()).rstrip("\n") +"展开-->"+ parameter_line_item_num+'\n')
                    outfile_merge.writelines("//"+(line.lstrip()).rstrip("\n") +"展开-->"+ parameter_line_item_num+'\n')
                    
                    for i_unroll_dex in range(int(parameter_line_item_num)):
                        line_unroll = (line[:idx_count]).rstrip()+unroll_char+str(i_unroll_dex)+";"+'\n'#注意去掉中间的空格
                        outfile.writelines(line_unroll)
                        outfile_merge.writelines(line_unroll)
                        #print(line_unroll)  
                    #pass
                    continue                    	
                outfile.writelines(line)
                outfile_merge.writelines(line)
            outfile.writelines('\n')
            outfile_merge.writelines('\n')
            infile.close()
            outfile.close()
            count_0_0_file = count_0_0_file + 1        
    outfile_merge.close()
    print('count_0_file  ----------> ' + str(count_0_file))
    print('count_0_0_file  ----------> ' + str(count_0_0_file))
    print("count_0_0_file合到cnn_top2中")
    print("提取每个文件展开的变量名")
    #print(various_context_table)
    return various_context_table
    print('matr_process' + end_point_arrow)
    



#geberate_for_unroll
#功能：读取all_verilog_file文件中的.v文件进行二维矩阵展开
#输入参数：parameter_vh,parameter_vh_all
#输出参数：无
#备注
#.v0_.v0_.v0_.v 去掉部分注释以及展开generate for的部分。 存放在verilog_file_path_processed文件夹中
#    展开使用_unroll_后缀
#cnn_top3:.v0_.v0_.v0_.v合到一块
#verilog后处理去掉cnn_top3的空行
#
def geberate_for_unroll(parameter_vh,parameter_vh_all):
    print('geberate_for_process' + start_point_arrow)
    filedir = os.getcwd() + verilog_file_path
    filenames = os.listdir(filedir)
    count_0_0_file = 0
    count_0_0_0_file = 0
    geberate_for_unroll_assert =  Stack()#栈结构没有用到
    comment_remove_stack = Stack()
    outfile_merge = open("cnn_top3.v","w",encoding='utf-8') #这个地方必须进行编码。
    generate_for_cache_hold = open("generate_for_cache_hold.v",'w',encoding='utf-8')
  
    #遍历文件名列表
    for filename in filenames:
        match0 = re.match('.*.v0_.v0_.v',filename)
        comment_remove_flag = False
        assert(comment_remove_stack.isEmpty())
        if match0:
            count_0_0_file = count_0_0_file + 1
            infilepath = filedir + '/' + filename 
            outfilepath = filedir + '/' + filename + '0_.v'
            include_vh = 'noneofinclude'
            include_find_index = 0
            print(filename)
            #遍历单个infile_temp文件，只用来判断是否include.vh
            infile_temp = open(infilepath,'r',encoding='utf-8')
            for line in infile_temp:
                include_find_index = include_find_index + 1
                if '`include' in line:
                    for parameter in parameter_vh: 
                        if parameter in line:
                            include_vh = parameter
                            break
                    break
                if include_find_index >=10:
                    break
            infile_temp.close()
            #遍历单个文件，按行处理后打印到outfile中
            infile = open(infilepath,'r',encoding='utf-8')
            outfile = open(outfilepath,"w",encoding='utf-8')
            generate_for_cache = open("generate_for_cache.v",'w',encoding='utf-8')
            str_index = ''
            parameter_line_item_num = ""
            generate_for_unroll_flag = False #首字母大写
            for line in infile:
                #去掉文件中的注释
                #这个处理太危险，处理难度极大。
                #match_comment_start = re.match('\s*/\**.*',line)# /****
                #if match_comment_start:
                    #comment_remove_flag == True
                    #comment_remove_stack.push('#')
                #match_comment_end = re.match('.*\**\/.*',line) # ***/
                #if match_comment_end:
                    #print(line)
                    #t=comment_remove_stack.pop()
                #if comment_remove_stack.isEmpty():
                    #comment_remove_flag == False
                    #print(line)
                    #continue
                #if comment_remove_flag:
                    #print(line)
                    #continue
                
                # 采用了简单地版本，只处理其中一部分，可以处理其中大部分的注释。
                if line[0] == '/' and line[1] == '*' and comment_remove_flag ==False:
                    comment_remove_flag = True
                    #print('注释开始')
                if '*/'in line and comment_remove_flag ==True:
                    comment_remove_flag = False
                    #print(line)
                    #print('注释结束')
                    continue
                if comment_remove_flag ==True:
                    #print(line)
                    continue
                
                #处理generate部分
                match_genvar = re.match('\s*genvar\s*\w*;',line) #genvar wire_ur;
                match_endgenerate = re.match('endgenerate.*',line.lstrip()) #endgenerate
                if match_genvar and generate_for_unroll_flag == False:
                    print('generate for开始------------------------------->'+ '\n')
                    outfile.writelines('//generate for开始------------------------------->'+ '\n')
                    outfile_merge.writelines('//generate for开始------------------------------->'+ '\n')
                    generate_for_unroll_flag =True
                    #print(line)
                    #geberate_for_unroll_assert.push('#')
                    #str_genvar = ((line.lstrip()).rstrip()).split()[0]
                    str_index  = (((line.lstrip()).rstrip()).split()[1]).replace(';','')
                    continue
                if match_endgenerate and generate_for_unroll_flag == True:
                    generate_for_unroll_flag =False
                    #print(line)
                    print('generate for结束------------------------------->'+ '\n')
                    #generate_for_unroll_flag.pop()
                    continue
                
                if generate_for_unroll_flag == True:
                    line_temp_in_index = 0
                    #print(line)
                    match_for = re.match('\s*for\s*\(.*=.*;.*;.*\)',line)#for (pool_relu_count = 0; pool_relu_count < NUM_MULT; pool_relu_count = pool_relu_count+1)
                    match_assign = re.match('\s*assign\s*.*;',line)#assign in_neuron_q_a[pi]=in_neuron_q_a_all[pi*DATA_WIDTH_FC+DATA_WIDTH_FC-1:pi*DATA_WIDTH_FC];
                    match_generate = re.match('generate.*',line.lstrip()) #generate
                    if match_generate:#遇到generate跳过
                        continue
                    if match_for:#在for中提取参数
                        if include_vh in parameter_vh_all:
                            for parameter_line_item in parameter_vh_all.get(include_vh): #判断所有参数是否出现这一行
                                if parameter_line_item in line:
                                    parameter_line_item_num = (parameter_vh_all.get(include_vh)).get(parameter_line_item)#获取数值
                                    outfile.writelines("//"+ str_index + ":"+ str(parameter_line_item_num)+'\n')
                                    outfile_merge.writelines("//"+ str_index + ":"+ str(parameter_line_item_num)+'\n')
                        #line_temp_in_index = 0
                        continue
                    #if match_assign:
                        #generate_for_cache.writelines(line)
                        #generate_for_cache_hold.writelines(line)
                        #continue
                    generate_for_cache.writelines(line)
                    generate_for_cache_hold.writelines(line)
                    #line_temp_in_index = line_temp_in_index +1
                    print(include_vh)
                    print(str(parameter_line_item_num))
                    #开始循环展开
                    if "end" in line:
                        generate_for_cache.close()
                        for index in range(int(parameter_line_item_num)):
                            print('第'+str(index) + "次展开-------------->")
                            outfile.writelines('//第'+str(index) + "次展开："+ str(index) + "/" + str(parameter_line_item_num) + '\n')
                            outfile_merge.writelines('//第'+str(index) + "次展开："+ str(index) + "/" + str(parameter_line_item_num) + '\n')
                            generate_for_cache_read = open('generate_for_cache.v','r',encoding='utf-8')
                            for line_temp_in_cache in generate_for_cache_read:
                                if "end" in line_temp_in_cache:
                                    continue
                                else:
                                    #print(line_temp_in_cache)#展开
                                    match_assign_cache = re.match('\s*assign\s*.*;',line_temp_in_cache)
                                    match_mult_inst_cache = re.match('.*mult_inst\s*\(.*',line_temp_in_cache) #注意“\”也要用反义符
                                    match_weight_inst_cache = re.match('.*weight_inst\s*\(.*',line_temp_in_cache)
                                    match_write_result_cache = re.match('.*write_result\s*\(.*',line_temp_in_cache)
                                    match_outneuron_cache = re.match('.*outneuron\s*\(.*',line_temp_in_cache)
                                    match_pool_memo_inst_cache = re.match('.*pool_memo_inst\s*\(.*',line_temp_in_cache)
                                    match_pool_inst_cache = re.match('.*pool_inst\s*\(.*',line_temp_in_cache)
                                    match_relu_param_cache = re.match('.*relu_param\s*\(.*',line_temp_in_cache)
                                    if match_assign_cache:#处理for 语句中的 assign
                                        if "[" + str_index + "]" in line_temp_in_cache:#一下五种情况都需要用+'_unroll_'来展开，其中两种assign都可以在这个过程中识别出来。
                                            #wire	[DATA_WIDTH-1: 0] q_a[NUM_MULT-1:0];
                                            #.q_a(q_a[pool_relu_count]),
                                            #assign q_b_all_out[output_num* DATA_WIDTH + DATA_WIDTH - 1:output_num * DATA_WIDTH]=q_b[output_num];

                                            #assign   result_relu_use[num1] = result[num1* IFMAP_PAR ];
                                            #.result(result_relu_use[pool_relu_count]),
                                            print(line_temp_in_cache)
                                            line_temp_in_cache = line_temp_in_cache.replace("[" + str_index + "]",'_unroll_'+ str(index))#
                                            #line_temp = line_temp1.replace(str_index,str(index))
                                            print(line_temp_in_cache)
                                            #outfile.writelines(line_temp)
                                            #outfile_merge.writelines(line_temp)
                                            
                                        #其他的情况的直接替换，展开。
                                        line_temp_in_cache = line_temp_in_cache.replace(str_index,str(index))
                                        
                                        outfile.writelines(line_temp_in_cache)
                                        outfile_merge.writelines(line_temp_in_cache)
                                        print(line_temp_in_cache)
                                        continue
                                    if match_mult_inst_cache:
                                        #line_temp = line_temp_in_cache.replace("mult_inst","mult_inst"+"_"+str(index))
                                        line_temp = re.sub("mult_inst\s*\(" ,"mult_inst"+"_"+str(index) + "(",line_temp_in_cache)
                                        print("heheheh-------------------------------------------------------->")
                                        print(line_temp)
                                        outfile.writelines(line_temp)
                                        outfile_merge.writelines(line_temp)
                                        continue
                                    if match_weight_inst_cache:
                                        line_temp = re.sub("weight_inst\s*\(" ,"weight_inst"+"_"+str(index) + "(",line_temp_in_cache)
                                        print("heheheh-------------------------------------------------------->")
                                        print(line_temp)
                                        outfile.writelines(line_temp)
                                        outfile_merge.writelines(line_temp)
                                        continue
                                    if match_write_result_cache:
                                        line_temp = re.sub("write_result\s*\(" ,"write_result"+"_"+str(index) + "(",line_temp_in_cache)
                                        print("heheheh-------------------------------------------------------->")
                                        print(line_temp)
                                        outfile.writelines(line_temp)
                                        outfile_merge.writelines(line_temp)
                                        continue
                                    if match_outneuron_cache:
                                        line_temp = re.sub("outneuron\s*\(" ,"outneuron"+"_"+str(index) + "(",line_temp_in_cache)
                                        print("heheheh-------------------------------------------------------->")
                                        print(line_temp)
                                        outfile.writelines(line_temp)
                                        outfile_merge.writelines(line_temp)
                                        continue
                                    if match_pool_memo_inst_cache:
                                        line_temp = re.sub("pool_memo_inst\s*\(" ,"pool_memo_inst"+"_"+str(index) + "(",line_temp_in_cache)
                                        print("heheheh-------------------------------------------------------->")
                                        print(line_temp)
                                        outfile.writelines(line_temp)
                                        outfile_merge.writelines(line_temp)
                                        continue
                                    if match_pool_inst_cache:
                                        line_temp = re.sub("pool_inst\s*\(" ,"pool_inst"+"_"+str(index) + "(",line_temp_in_cache)
                                        print("heheheh-------------------------------------------------------->")
                                        print(line_temp)
                                        outfile.writelines(line_temp)
                                        outfile_merge.writelines(line_temp)
                                        continue
                                    if match_relu_param_cache:
                                        line_temp = re.sub("relu_param\s*\(" ,"relu_param"+"_"+str(index) + "(",line_temp_in_cache)
                                        print("heheheh-------------------------------------------------------->")
                                        print(line_temp)
                                        outfile.writelines(line_temp)
                                        outfile_merge.writelines(line_temp)
                                        continue

                                    
                                    if "[" + str_index + "]" in line_temp_in_cache:
                                        line_temp = line_temp_in_cache.replace("[" + str_index + "]", '_unroll_'+ str(index))#'_unroll_'+ str(index))
                                        print(line_temp)
                                        outfile.writelines(line_temp)
                                        outfile_merge.writelines(line_temp)
                                        continue
                                    print(line_temp_in_cache)
                                    outfile.writelines(line_temp_in_cache)#写入其他语句
                                    outfile_merge.writelines(line_temp_in_cache)
                            generate_for_cache_read.close()
                        generate_for_cache = open('generate_for_cache.v','w',encoding='utf-8')#重新写入for循环
                        #continue#在end这一行跳过
                    continue#generate_for_unroll_flag == True 这个过程应该跳过打印。
                outfile.writelines(line)
                outfile_merge.writelines(line)
            outfile.writelines('\n')
            outfile_merge.writelines('\n')
            infile.close()
            generate_for_cache.close()
            outfile.close()
            count_0_0_0_file = count_0_0_0_file + 1
        #assert(geberate_for_unroll_assert == False)
    outfile_merge.close()
    generate_for_cache_hold.close()
    print('count_0_0_file  ----------> ' + str(count_0_0_file))
    print('count_0_0_0_file  ----------> ' + str(count_0_0_0_file))
    print("count_0_0_0_file 合到cnn_top3中")
    print('geberate_for_process' + end_point_arrow)


#various_process
#功能：读取all_verilog_file文件中的.v文件进行二维矩阵展开
#输入参数：various_context_table
#输出参数：无
#备注
# .v 替换在循环和for语句展开的时候 与上下文相关的变量
#    替换同样使用_unroll_后缀
#cnn_top4: .v 合到一块
#检验正确后后续可以从cnn_top4处理，前边步骤中(1)有需要用#include<>的地方(2)有参数依赖需要替换 都需要对单个.v文件进行处理
#verilog后处理去掉cnn_top4的空行
#
def various_process(various_context_table):
    print('various_process--------------------------------------' + start_point_arrow)
    filedir = os.getcwd() + verilog_file_path
    filenames = os.listdir(filedir)
    count_0_0_0_file = 0
    verilog_file = 0
    outfile_merge = open("cnn_top4_temp.v","w",encoding='utf-8') #这个地方必须进行编码。

    #处理之前先清空verilog_file_path_processed文件夹
    filedir_out = os.getcwd() + verilog_file_path_processed
    filenames_out = os.listdir(filedir_out)
    for filename_out in filenames_out:
        filepath_out = os.getcwd() + verilog_file_path_processed + '/'+ filename_out
        os.remove(filepath_out)
        
    #遍历文件名列表
    for filename in filenames:
        match0 = re.match('.*.v0_.v0_.v0_.v',filename)
        if match0:
            count_0_0_0_file = count_0_0_0_file + 1
            infilepath = filedir + '/' + filename
            outfilepath = filedir_out + '/' + filename[:-13] + 'v'
            print(filename[:-14])
            infile = open(infilepath,'r',encoding='utf-8')
            outfile = open(outfilepath,"w",encoding='utf-8')
            line_count_index = 0
    
            #遍历单个文件，按行处理后打印到outfile中
            for line in infile:
                if filename[:-14] in various_context_table:
                    for variable in various_context_table.get(filename[:-14]):
                        match_variable0 = re.match(".*"+ variable + '\[\d*\]'+".*",line)#有需要替换的参数则特殊处理line,同时直接跳出查找
                        match_variable1 = re.match(".*"+ variable + '\[\d*\*\d*\]'+".*",line)#result[0*2]+result[0*2+1];
                        match_variable2 = re.match(".*"+ variable + '\[\d*\*\d*\+1\]'+".*",line)#result[0*2]+result[0*2+1];
                        if match_variable0:
                            #print(line)
                            if ((line.lstrip()).rstrip())[-4] == "[":
                                #print((line.rstrip())[:-4] + "_unroll_" + ((line.lstrip()).rstrip())[-3] + ";"+ "\n")
                                line = (line.rstrip())[:-4] + "_unroll_" + ((line.lstrip()).rstrip())[-3] + ";"+ "\n"
                            if ((line.lstrip()).rstrip())[-5] == "[":
                                #print((line.rstrip())[:-5] + "_unroll_" + ((line.lstrip()).rstrip())[-4] + ((line.lstrip()).rstrip())[-3] + ";"+ "\n")
                                line = (line.rstrip())[:-5] + "_unroll_" + ((line.lstrip()).rstrip())[-4] + ((line.lstrip()).rstrip())[-3] + ";"+ "\n"
                        if match_variable1:
                            print("第"+str(line_count_index)+"行："+"\n")
                            print(line)
                        if match_variable2:
                            print("第"+str(line_count_index)+"行："+"\n")
                            print(line)
                            
                line_count_index += 1
                outfile.writelines(line)
                outfile_merge.writelines(line)
            outfile.writelines("\n")
            outfile_merge.writelines("\n")
            outfile.close()
            verilog_file = verilog_file + 1
    outfile_merge.close()

    print('count_0_0_0_file  ----------> ' + str(count_0_0_0_file))
    print('verilog_file  ----------> ' + str(verilog_file))
    print("verilog_file 合到cnn_top4中")
    
    #去掉空行，使得文本更加紧凑
    print('verilog后处理' + start_point_arrow)
    infile_post = open("cnn_top4_temp.v","r",encoding='utf-8')
    outfile_post = open("cnn_top4.v","w",encoding='utf-8')
    for line_post in infile_post:
        if line.count('\n') == len(line_post):
            #print(line_post)
            continue
        outfile_post.writelines(line_post)      
    outfile_post.writelines('\n')
    infile_post.close()
    outfile_post.close()
    
    os.remove('cnn_top4_temp.v')
    print('verilog后处理' + end_point_arrow)
    
    print('various_process' + end_point_arrow)

##################################################################################################
##剩下的部分只对合在一起的Verilog做处理
##
################################################################################################## 

#remove
#功能：根据需求修改电路接口，去掉某些多余的行。
#输入参数：无
#输出参数：无
#备注
#cnn_top5:由cnn_top4处理得到
#
def remove():
    print('remove' + start_point_arrow)
    outfile  = open("cnn_top5.v","w",encoding='utf-8')
    #打开生成的cnn_top3
    infile = open("cnn_top4.v","r",encoding='utf-8')

    rem1 = rem2 = rem3 =0 
    rem4 = rem5 = rem6 = 0
    rem7 = rem8 = rem9 = 0   
    rem10 =0
    rem11 = 0
    for line in infile:
        match_top = re.match(".*module\s*neo\s*\(.*",line)#有需要替换的参数则特殊处理line,同时直接跳出查找    
        if match_top:
            line = "module cnn_top(" + "\n"
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
        elif "`include" in line:
            rem11 +=1
            pass 
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
    print("remove:.aclr3(clear)-->>")
    print(rem7)
    print(rem8)
    print(rem9)    
    print("replace：altmult_accum -->>")
    print(rem10)
    print("remove：include -->>")
    print(rem11)
    print('remove' + end_point_arrow)

#error
#功能：修改Verilog语法错误
#输入参数：无
#输出参数：无
#备注
#cnn_top:由cnn_top5处理得到
#
def error():#处理错误
    print('error' + start_point_arrow)
    outfile  = open("cnn_top.v","w",encoding='utf-8')
    #打开生成的cnn_top5
    infile = open("cnn_top5.v","r",encoding='utf-8')
    line_index = 0
    
    #预读记录右括号的位置
    right_brackets_situation = []
    infile_temp = open("cnn_top5.v","r",encoding='utf-8')
    for line_temp in infile_temp:
        match_brack = re.match('\s*\)\s*;\s*',line_temp) #	);
        if match_brack:
            #print(line_temp)
            right_brackets_situation.append(str(line_index))
        line_index = line_index + 1
    print(right_brackets_situation)
    
    #逐行处理   
    line_index = 0   
    for line in infile:
        if str(line_index+1) in right_brackets_situation and "," in line:
            print("error at \"，\" ------------------>>>"+ line)
            line = line.replace(',','')
        line_index = line_index +1
        outfile.writelines(line)
        
    outfile.writelines('\n')
    outfile.close()
    infile.close()
    infile_temp.close()
    print('error' + end_point_arrow)

   
#main
#功能：main函数
#输入参数：无
#输出参数：无
#备注
#       
def main():
    report=open("synReport.txt","w",encoding='utf-8')
    report.writelines('预处理RTL代码...\n\n')
    
    report.writelines('参数提取...\n\n')	
    parameter_vh = vh2txt()
    parameter_vh_all = parameter_vh_lib(parameter_vh)

    report.writelines('merge...\n\n')
    merge_verilog(parameter_vh,parameter_vh_all)

    report.writelines('二维矩阵处理...\n\n')
    various_context_table = matr_process(parameter_vh,parameter_vh_all)

    report.writelines('for语句处理...\n\n')
    geberate_for_unroll(parameter_vh,parameter_vh_all)

    various_process(various_context_table)
    
    report.writelines('修改电路结构...\n\n')
    remove()

    report.writelines('错误检测与修改...\n\n')
    error()

    report.writelines('预处理结束!\n\n')
    report.close()

    #删除中间文件
    remove_file1 = open('cnn_top.clk',"w",encoding='utf-8') 
    remove_file1.close()
    os.remove('cnn_top.clk')
    
    remove_file2 = open('syn_st',"w",encoding='utf-8')
    remove_file2.close()
    os.remove('syn_st')
    
    remove_file3 = open('comet_pll.info',"w",encoding='utf-8')
    remove_file3.close()
    os.remove('comet_pll.info')
    
    remove_file4 = open('generate_for_cache.v',"w",encoding='utf-8')
    remove_file4.close()
    os.remove('generate_for_cache.v')
 


	
if __name__ == "__main__":
    main()
    
