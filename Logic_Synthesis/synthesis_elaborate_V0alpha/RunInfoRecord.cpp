// RunInfoRecord.cpp : Defines the entry point for the console application.
/*
     类名：  运行信息记录类 CRunInfoRecord
	 作用：  记录软件运行期间重要日志信息、向界面通知运行进度信息、用户提示信息等，也可记录调试信息以便查错定位bugs
	 版本：  V0.2.02
	 日期：  2013.1.21
*/
#include"time.h"
#include<string>
#include "RunInfoRecord.h"

HWND CRunInfoRecord::mainHwnd;
char CRunInfoRecord::fileName[8192];
char   CRunInfoRecord::buff[8092];
CRunInfoRecord::CRunInfoRecord(void)
{
}

CRunInfoRecord::~CRunInfoRecord(void)
{
}

//在给定字串str前加入tabPrix个tab，追加写入文件，是否将字符串打印到屏幕由isPrintToScreen指出（true打印，否则不打印）
 bool CRunInfoRecord::Write(const char * str,int tabPrix ,bool isPrintToScreen)
	 {
	   if(str ==NULL)
		  return true;
		//准备字符串
	   memset(buff,0,8092);	   
	   int c=0;
	   while(c<tabPrix)
	   {
	     // WriteToFile("\t",fp);
		  strcat(buff,"\t");
		  c++;
	   }
        strcat(buff,str);

        //输出到屏幕
		if(isPrintToScreen)
			 printf("%s",buff);
		
		//输出到文件
	    FILE *fp=OpenFile();
	   if(fp==NULL)
		   return false;
	    WriteToFile(buff,fp); 	   
	 	CloseFile(fp);
       return true;
	 }
	 //将字符串str写入文件，isPrintToScreen为true则输出到屏幕，否则不输出
 bool CRunInfoRecord::Write(const char * str,bool isPrintToScreen)
	 {
       return  Write(str,0  , isPrintToScreen);
	 }
	 //将字符串str写入文件，并输出到屏幕
 bool CRunInfoRecord::Write(const char * str)
	 {
		return  Write(str,0 ,true);
	 }
	
	 
	 //在给定字串strLine前加入tabPrix个tab，当作一行追加写入文件末尾，是否将字符串打印到屏幕由isPrintToScreen指出（true打印，否则不打印）
 bool CRunInfoRecord::WriteLine(const char * strLine,int tabPrix ,bool isPrintToScreen)
	 {
		 if(strLine==NULL)
			 return true;
		 //准备字符串
		memset(buff,0,8092);	   
	   int c=0;
	   while(c<tabPrix)
	   {
	     // WriteToFile("\t",fp);
		  strcat(buff,"\t");
		  c++;
	   }
        strcat(buff,strLine);
        //输出到屏幕
		if(isPrintToScreen)
			 printf("%s\n",buff);
       //输出到文件
	   FILE *fp=OpenFile();
	   if(fp==NULL)
		   return false;      
	    WriteToFile(buff,fp); 
	    WriteToFile("\n",fp);
	 	CloseFile(fp);
	    memset(buff,0,8092);	
       return true;
	 }
	 //在给定字串strLine当作一行追加写入文件末尾，是否将字符串打印到屏幕由isPrintToScreen指出（true打印，否则不打印）
 bool CRunInfoRecord::WriteLine(const char * strLine ,bool isPrintToScreen)
	 {
        return  WriteLine(strLine,0,isPrintToScreen);
      }
 //在给定字串strLine当作一行追加写入文件末尾，并打印到屏幕
 bool CRunInfoRecord::WriteLine(const char * strLine )
	 {
        return  WriteLine(strLine,0,true);
      }



  

  //在给定字串Info加tabPrix个空格和“ Info:”，然后追加写入文件末尾，是否将字符串打印到屏幕由isPrintToScreen指出（true打印，否则不打印）
 bool CRunInfoRecord::WriteInfo(const char * Info,int tabPrix ,bool isPrintToScreen)
	 {	
	   if(Info==NULL)
			 return true;  
		 //准备数据
	   memset(buff,0,8092);	   
	   int c=0;
	   while(c<tabPrix)
	   {
	     // WriteToFile("\t",fp);
		  strcat(buff,"\t");
		  c++;
	   }
	   strcat(buff,"Info: ");
	   strcat(buff,Info);
	    //输出到屏幕
		if(isPrintToScreen)
			 printf("%s",buff);
       //输出到文件

	   FILE *fp=OpenFile();
	   if(fp==NULL)
		   return false;	  
	   WriteToFile(buff,fp);
	   CloseFile(fp);
	   sendMessageToCaller(buff);
	  // memset(buff,0,8092);	
       return true;
	 }





 //在给定字串Info前插入“ Info:”，然后追加写入文件末尾，是否将字符串打印到屏幕由isPrintToScreen指出（true打印，否则不打印）
 bool CRunInfoRecord::WriteInfo(const char * Info,bool isPrintToScreen)
	{ 
		return WriteInfo( Info,0 , isPrintToScreen);
    }
 //在给定字串Info前插入“ Info:”，然后追加写入文件末尾，并打印到屏幕
 bool CRunInfoRecord::WriteInfo(const char * Info )
	 { 
		return  WriteInfo( Info,0 ,true);
     }
	 

 //在给定字串warning加tabPrix个空格和“ Warning:”，然后追加写入文件末尾，是否将字符串打印到屏幕由isPrintToScreen指出（true打印，否则不打印）
 bool CRunInfoRecord::WriteWarning(const char * warning,int tabPrix ,bool isPrintToScreen)
	 { 
		if(warning==NULL)
			 return true; 
		memset(buff,0,8092);	
	   int c=0;
	   while(c<tabPrix)
	   {
	     // WriteToFile("\t",fp);
		  strcat(buff,"\t");
		  c++;
	   }
	   strcat(buff,"Warning: ");
	   strcat(buff,warning);
	    //输出到屏幕
		if(isPrintToScreen)
		   printf("%s",buff);
       //输出到文件
	  
	   FILE *fp=OpenFile();
	   if(fp==NULL)
		   return false;   
	   WriteToFile(buff,fp);
	   CloseFile(fp);
	   sendMessageToCaller(buff);
       return true;
	 }

 //在给定字串warning前插入“ Warning:”，然后追加写入文件末尾，是否将字符串打印到屏幕由isPrintToScreen指出（true打印，否则不打印）
 bool CRunInfoRecord::WriteWarning(const char * warning,bool isPrintToScreen)
	 { 
          return  WriteWarning( warning,0,isPrintToScreen);
     }
 //在给定字串warning前插入“ Warning:”，然后追加写入文件末尾，并打印到屏幕
 bool CRunInfoRecord::WriteWarning(const char * warning)
	 { 
          return  WriteWarning( warning,0,true);
     }



 
 //在给定字串erro前插入tabPrix个空格和“ Erro:”，然后追加写入文件末尾，是否将字符串打印到屏幕由isPrintToScreen指出（true打印，否则不打印）
 bool CRunInfoRecord::WriteErro(const char * erro,int tabPrix ,bool isPrintToScreen)
	 {
	   if(erro==NULL)
			 return true; 
       memset(buff,0,8092);	   
	   int c=0;
	   while(c<tabPrix)
	   {
	     // WriteToFile("\t",fp);
		  strcat(buff,"\t");
		  c++;
	   }
	   strcat(buff,"Erro: " );
	   strcat(buff,erro);
	    //输出到屏幕
	   if(isPrintToScreen)
		  printf("%s",buff);

       //输出到文件	  
	   FILE *fp=OpenFile();
	   if(fp==NULL)
		   return false;	  
	   WriteToFile(buff,fp);
	   CloseFile(fp);
	   sendMessageToCaller(buff);
       return true;	 
	 }
 //在给定字串erro前插入“ Erro:”，然后追加写入文件末尾，是否将字符串打印到屏幕由isPrintToScreen指出（true打印，否则不打印）
 bool CRunInfoRecord::WriteErro(const char * erro,bool isPrintToScreen)
	 {
		return WriteErro(erro,0,isPrintToScreen);

	 }
  //在给定字串erro前插入“ Erro:”，并打印到屏幕
 bool CRunInfoRecord::WriteErro(const char * erro)
	 {
		return WriteErro(erro,0,true);
	  }

	
 FILE* CRunInfoRecord::OpenFile()
	{ // CRunInfoRecord r;
		//strcpy( r.fileName,"test.txt");
	   if(fileName==NULL || strcmp(fileName,"")==0)
		    return false;	  
	   FILE *fp=fopen(fileName,"a+");
	   return fp;   
	}

 void CRunInfoRecord::WriteToFile(const char* string,FILE *fp)
	{
	   fwrite(string,sizeof(char),strlen(string),fp);
	}
 void CRunInfoRecord::CloseFile(FILE *fp)
	{
	   fclose(fp);
	   fp=0;
	}

 void InitRunInfoRecordStatic(char* fileName,HWND hwnd)
{
    struct tm t1;
	time_t now;
	char buff[30]={0};
	char dateTime[512]={0};
	time(&now);
	_localtime64_s(&t1,&now);
	asctime_s(buff,sizeof(buff),&t1);
	
	sprintf(dateTime,"//\t%d-%d-%d %d:%d:%d\n",t1.tm_year-100+2000,t1.tm_mon+1,t1.tm_mday,t1.tm_hour,t1.tm_min,t1.tm_sec);

	CRunInfoRecord::mainHwnd=hwnd;
   strcpy(CRunInfoRecord::fileName,fileName);
   CRunInfoRecord::WriteLine ("//\t 运行信息报告 \n");
   CRunInfoRecord::Write(dateTime);
   //CRunInfoRecord::Write("//  2012.12.6 17:10:33\n");
   CRunInfoRecord::Write("//\tpasskey2.6 alpha\n");
   CRunInfoRecord::Write("// 中国科学院电子学研究所十一室\n\n" );
   CRunInfoRecord::Write("-----------------------------------------\n" );

   CRunInfoRecord::Write("synthesis......\n" );

  
}