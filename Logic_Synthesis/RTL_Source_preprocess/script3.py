#code=utf-8

import os	
import re


def main():
    temp_file = open('temp.v',"w",encoding='utf-8')
    for index in range(16):
        temp_file.writelines("//第"+ str(index) +"次展开" + "："+str(index) + "//16"+ "\n")
        temp_file.writelines("assign   result_relu_use_unroll_"+str(index) + " = " + "result_unroll_" + str(index*3+2) + "+ result_unroll_" + str(index*3+1)+ "+ result_unroll_" + str(index*3) + ";"+ "\n")        

    temp_file.close()
	
if __name__ == "__main__":
    main()
    
