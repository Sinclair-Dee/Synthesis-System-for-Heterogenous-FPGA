/*
     类名：  运行信息记录类 CRunInfoRecord
	 作用：  记录软件运行期间重要日志信息、向界面通知运行进度信息、用户提示信息等，也可记录调试信息以便查错定位bugs
	 版本：  V0.2.02
	 日期：  2013.1.21
*/
#pragma once
#include"windows.h"
#define WM_TEST WM_USER+1//自定义消息

struct LParamDataSruct
{
public:
	char* lParamData;       //要发送的信息
};

void InitRunInfoRecordStatic(char* fileName,HWND hwnd);

class CRunInfoRecord
{
  public:
	CRunInfoRecord(void);
	//文件名
	 static char fileName[8192];
	 static HWND mainHwnd; //界面的句柄，向界面发送消息时使用
	//在给定字串str前加入tabPrix个tab，追加写入文件，是否将字符串打印到屏幕由isPrintToScreen指出（true打印，否则不打印）
	static bool Write(const char * str,int tabPrix,bool isPrintToScreen=true);
	//将字符串str写入文件，isPrintToScreen为true则输出到屏幕，否则不输出
    static bool Write(const char * str,bool isPrintToScreen);
	//将字符串str写入文件，并输出到屏幕
    static bool Write(const char * str);

	//写入一行字符串，tabPrix为加入前缀tab的个数，该函数内部会根据tabPrix在字符串前加入若干前缀tab，然后写入文件；是否达到屏幕，默认打到屏幕
	static bool WriteLine(const char * strLine,int tabPrix,bool isPrintToScreen=true);
    //写一行到字符串到文件，isPrintToSreen为true表示输出到屏幕，否则不输出
	static bool WriteLine(const char * strLine,bool isPrintToScreen);
	//写一行到字符串到文件，并输出到屏幕
	static bool WriteLine(const char * strLine);


   /*写入提示信息，
   函数内部会自动加上Info头，按照“若干前缀tab Info：提示信息内容”的格式写入文件
   tabPrix为加入前缀tab的个数，该函数内部会根据tabPrix在字符串前加入若干前缀tab，然后写入文件
   isPrintToScreen为true表示输出到屏幕，否则不输出
	*/
	static bool WriteInfo(const char * Info,int tabPrix,bool isPrintToScreen=true);
	 //在给定字串Info前插入“ Info:”，然后追加写入文件末尾，是否将字符串打印到屏幕由isPrintToScreen指出（true打印，否则不打印）
	static bool WriteInfo(const char * Info,bool isPrintToScreen);
	 //在给定字串Info前插入“ Info:”，然后追加写入文件末尾，并打印到屏幕
	static bool WriteInfo(const char * Info);

	/*写入warning信息	
	函数内部会自动加上Warning头，按照“若干前缀tab Warning：提示信息内容”的格式写入文件
	tabPrix为加入前缀tab的个数，该函数内部会根据tabPrix在字符串前加入若干前缀tab，然后写入文件
    isPrintToScreen为true表示输出到屏幕，否则不输出
	*/
   static bool WriteWarning(const char * warning,int tabPrix,bool isPrintToScreen=true);
    //在给定字串warning前插入“ Warning:”，然后追加写入文件末尾，是否将字符串打印到屏幕由isPrintToScreen指出（true打印，否则不打印）
   static bool WriteWarning(const char * warning,bool isPrintToScreen);
    //在给定字串warning前插入“ Warning:”，然后追加写入文件末尾，并打印到屏幕
   static bool WriteWarning(const char * warning);

	/*写入erro信息
	函数内部会自动加上Erro头，按照“若干前缀tab Erro：提示信息内容”的格式写入文件
	tabPrix为加入前缀tab的个数，该函数内部会根据tabPrix在字符串前加入若干前缀tab，然后写入文件
	isPrintToScreen为true表示输出到屏幕，否则不输出
	*/
	static bool WriteErro(const char * erro,int tabPrix,bool isPrintToScreen=true);
	 //在给定字串erro前插入“ Erro:”，然后追加写入文件末尾，是否将字符串打印到屏幕由isPrintToScreen指出（true打印，否则不打印）
    static bool WriteErro(const char * erro,bool isPrintToScreen);
	  //在给定字串erro前插入“ Erro:”，然后追加写入文件末尾，并打印到屏幕
    static bool WriteErro(const char * erro); 

  private:
	static void sendMessageToCaller(const char* info)
	{
	  if(info==NULL)
		  return; 
	  if(mainHwnd==NULL)
		  return;
	  //int l= strlen(info);
	  //memset(buff,0,8092);
	  strcpy(buff,info);
	 /* char* str=new char(l+5);
	  str[0]=0;
	  strcpy(str,info);*/
	  LParamDataSruct lparam;
	  lparam.lParamData = buff;
	  
	  ::SendMessage(mainHwnd,WM_TEST,/*wPARAM*/ 0,(LPARAM)&lparam); 
	 /* delete str;*/
	}
	static char buff[8092];
	static FILE* OpenFile();
	static void WriteToFile(const char* string,FILE *fp);
	static void CloseFile(FILE *fp);

public:
	~CRunInfoRecord(void);
};


/*
  用法：
    1. 在main函数的开始，调用初始化函数
                    void InitRunInfoRecordStatic(char* fileName,HWND hwnd); //写入文件的路径及文件名，主调方的句柄，底层调试可设为NULL,只生成一次
  
	  例如
	   InitRunInfoRecordStatic("test.txt",0);//只调用一次，即只初始化一次即可
  
   2. 在此之后，不论哪个函数都可以用以下两种方式之一来写运行记录（不用考虑文件打开与关闭的过程，该过程已经封装到静态函数当中）

   方法1：通过 类名::函数名 引用静态方法
   CRunInfoRecord::WriteLine ("---------------装箱报告--------------");
   CRunInfoRecord::WriteInfo ("打开vqm文件\n");

   CRunInfoRecord::WriteLine ("out：5"，2); //表示将在"out：5"前加入2个tab变成"		out：5"后写入文件，并在屏幕输出
   CRunInfoRecord::WriteLine ("out：5"，2，true); //与上述CRunInfoRecord::WriteLine ("out：5"，2)调用功能相同
   CRunInfoRecord::WriteLine ("out：5"，2，fasle); //表示将在"out：5"前加入2个tab变成"		out：5"后写入文件，但不在屏幕输出

   CRunInfoRecord::WriteInfo ("打开net文件");//表示将 "打开net文件" 变成"Info：打开net文件"后写入文件，并在屏幕输出
   CRunInfoRecord::WriteInfo ("打开net文件"，true);//功能与上述 CRunInfoRecord::WriteInfo ("打开net文件")相同
   CRunInfoRecord::WriteInfo ("打开net文件"，false);//表示将上述写文件相同，但不在屏幕输出




   方法2：通过  对象.函数   引用静态函数
   CRunInfoRecord  runInfREcord;
   runInfREcord.WriteInfo ("产生net文件\n");

   
*/



/*
用法实例：

void main()
{
	//声明
   void InitRunInfoRecordStatic(char* fileName,HWND hwnd);
   void test();
  //调用
   InitRunInfoRecordStatic("test.txt",0);//只调用一次，即只初始化一次即可，第二个参数HWND hwnd为发送消息的参数，底层调用时该处设为0即可
    test();  //根据需要写运行记录的函数

}

void test() 
{ 
   //有两种调用方式，其作用是等价的
	//CRunInfoRecord::mainHwnd(this::h);
   //直接采用 类名::函数名 方式调用
   CRunInfoRecord::WriteLine ("---------------装箱报告--------------\n");
   CRunInfoRecord::WriteInfo ("打开vqm文件\n");
   CRunInfoRecord::WriteInfo ("提取vqm文件中le信息\n",1);//缩进一个table
   CRunInfoRecord::WriteInfo ("提取vqm文件中M4K信息\n",1);
   CRunInfoRecord::WriteWarning("vqm文件中没有M4K信息\n",2，false);//缩进两个table写入文件，不在屏幕输出该行信息
   CRunInfoRecord::WriteInfo ("提取vqm文件中DSP信息\n",1);
   CRunInfoRecord::WriteInfo ("关闭vqm文件\n");
   
   //通过 对象引用.函数 方式调用
   CRunInfoRecord  runInfREcord;
   runInfREcord.WriteInfo ("产生net文件\n");
   runInfREcord.WriteInfo ("写入le信息\n",1);
   runInfREcord.WriteInfo ("写入DSP信息\n",1);
   runInfREcord.WriteErro ("写入DSP信息出错\n",1);
   runInfREcord.WriteInfo ("关闭net文件\n");

   CRunInfoRecord::WriteLine ("------------装箱报告结束--------------\n");
	return  ;
}

*/