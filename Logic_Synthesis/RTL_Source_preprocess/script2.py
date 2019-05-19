#code=utf-8

import os	
import re


def main():
    temp_file = open('temp.v',"w",encoding='utf-8')
    for index in range(20):
        temp_file.writelines("//第"+ str(index) +"次展开" + "："+str(index) + "//20"+ "\n")
        temp_file.writelines("assign   result_relu_use_unroll_"+str(index) + " = " + "result_unroll_" + str(index*2) + "+ result_unroll_" + str(index*2+1) + ";"+ "\n")
        
    temp_file.close()
	
if __name__ == "__main__":
    main()
    
