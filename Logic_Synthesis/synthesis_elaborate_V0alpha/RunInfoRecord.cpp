// RunInfoRecord.cpp : Defines the entry point for the console application.
/*
     ������  ������Ϣ��¼�� CRunInfoRecord
	 ���ã�  ��¼��������ڼ���Ҫ��־��Ϣ�������֪ͨ���н�����Ϣ���û���ʾ��Ϣ�ȣ�Ҳ�ɼ�¼������Ϣ�Ա���λbugs
	 �汾��  V0.2.02
	 ���ڣ�  2013.1.21
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

//�ڸ����ִ�strǰ����tabPrix��tab��׷��д���ļ����Ƿ��ַ�����ӡ����Ļ��isPrintToScreenָ����true��ӡ�����򲻴�ӡ��
 bool CRunInfoRecord::Write(const char * str,int tabPrix ,bool isPrintToScreen)
	 {
	   if(str ==NULL)
		  return true;
		//׼���ַ���
	   memset(buff,0,8092);	   
	   int c=0;
	   while(c<tabPrix)
	   {
	     // WriteToFile("\t",fp);
		  strcat(buff,"\t");
		  c++;
	   }
        strcat(buff,str);

        //�������Ļ
		if(isPrintToScreen)
			 printf("%s",buff);
		
		//������ļ�
	    FILE *fp=OpenFile();
	   if(fp==NULL)
		   return false;
	    WriteToFile(buff,fp); 	   
	 	CloseFile(fp);
       return true;
	 }
	 //���ַ���strд���ļ���isPrintToScreenΪtrue���������Ļ���������
 bool CRunInfoRecord::Write(const char * str,bool isPrintToScreen)
	 {
       return  Write(str,0  , isPrintToScreen);
	 }
	 //���ַ���strд���ļ������������Ļ
 bool CRunInfoRecord::Write(const char * str)
	 {
		return  Write(str,0 ,true);
	 }
	
	 
	 //�ڸ����ִ�strLineǰ����tabPrix��tab������һ��׷��д���ļ�ĩβ���Ƿ��ַ�����ӡ����Ļ��isPrintToScreenָ����true��ӡ�����򲻴�ӡ��
 bool CRunInfoRecord::WriteLine(const char * strLine,int tabPrix ,bool isPrintToScreen)
	 {
		 if(strLine==NULL)
			 return true;
		 //׼���ַ���
		memset(buff,0,8092);	   
	   int c=0;
	   while(c<tabPrix)
	   {
	     // WriteToFile("\t",fp);
		  strcat(buff,"\t");
		  c++;
	   }
        strcat(buff,strLine);
        //�������Ļ
		if(isPrintToScreen)
			 printf("%s\n",buff);
       //������ļ�
	   FILE *fp=OpenFile();
	   if(fp==NULL)
		   return false;      
	    WriteToFile(buff,fp); 
	    WriteToFile("\n",fp);
	 	CloseFile(fp);
	    memset(buff,0,8092);	
       return true;
	 }
	 //�ڸ����ִ�strLine����һ��׷��д���ļ�ĩβ���Ƿ��ַ�����ӡ����Ļ��isPrintToScreenָ����true��ӡ�����򲻴�ӡ��
 bool CRunInfoRecord::WriteLine(const char * strLine ,bool isPrintToScreen)
	 {
        return  WriteLine(strLine,0,isPrintToScreen);
      }
 //�ڸ����ִ�strLine����һ��׷��д���ļ�ĩβ������ӡ����Ļ
 bool CRunInfoRecord::WriteLine(const char * strLine )
	 {
        return  WriteLine(strLine,0,true);
      }



  

  //�ڸ����ִ�Info��tabPrix���ո�͡� Info:����Ȼ��׷��д���ļ�ĩβ���Ƿ��ַ�����ӡ����Ļ��isPrintToScreenָ����true��ӡ�����򲻴�ӡ��
 bool CRunInfoRecord::WriteInfo(const char * Info,int tabPrix ,bool isPrintToScreen)
	 {	
	   if(Info==NULL)
			 return true;  
		 //׼������
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
	    //�������Ļ
		if(isPrintToScreen)
			 printf("%s",buff);
       //������ļ�

	   FILE *fp=OpenFile();
	   if(fp==NULL)
		   return false;	  
	   WriteToFile(buff,fp);
	   CloseFile(fp);
	   sendMessageToCaller(buff);
	  // memset(buff,0,8092);	
       return true;
	 }





 //�ڸ����ִ�Infoǰ���롰 Info:����Ȼ��׷��д���ļ�ĩβ���Ƿ��ַ�����ӡ����Ļ��isPrintToScreenָ����true��ӡ�����򲻴�ӡ��
 bool CRunInfoRecord::WriteInfo(const char * Info,bool isPrintToScreen)
	{ 
		return WriteInfo( Info,0 , isPrintToScreen);
    }
 //�ڸ����ִ�Infoǰ���롰 Info:����Ȼ��׷��д���ļ�ĩβ������ӡ����Ļ
 bool CRunInfoRecord::WriteInfo(const char * Info )
	 { 
		return  WriteInfo( Info,0 ,true);
     }
	 

 //�ڸ����ִ�warning��tabPrix���ո�͡� Warning:����Ȼ��׷��д���ļ�ĩβ���Ƿ��ַ�����ӡ����Ļ��isPrintToScreenָ����true��ӡ�����򲻴�ӡ��
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
	    //�������Ļ
		if(isPrintToScreen)
		   printf("%s",buff);
       //������ļ�
	  
	   FILE *fp=OpenFile();
	   if(fp==NULL)
		   return false;   
	   WriteToFile(buff,fp);
	   CloseFile(fp);
	   sendMessageToCaller(buff);
       return true;
	 }

 //�ڸ����ִ�warningǰ���롰 Warning:����Ȼ��׷��д���ļ�ĩβ���Ƿ��ַ�����ӡ����Ļ��isPrintToScreenָ����true��ӡ�����򲻴�ӡ��
 bool CRunInfoRecord::WriteWarning(const char * warning,bool isPrintToScreen)
	 { 
          return  WriteWarning( warning,0,isPrintToScreen);
     }
 //�ڸ����ִ�warningǰ���롰 Warning:����Ȼ��׷��д���ļ�ĩβ������ӡ����Ļ
 bool CRunInfoRecord::WriteWarning(const char * warning)
	 { 
          return  WriteWarning( warning,0,true);
     }



 
 //�ڸ����ִ�erroǰ����tabPrix���ո�͡� Erro:����Ȼ��׷��д���ļ�ĩβ���Ƿ��ַ�����ӡ����Ļ��isPrintToScreenָ����true��ӡ�����򲻴�ӡ��
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
	    //�������Ļ
	   if(isPrintToScreen)
		  printf("%s",buff);

       //������ļ�	  
	   FILE *fp=OpenFile();
	   if(fp==NULL)
		   return false;	  
	   WriteToFile(buff,fp);
	   CloseFile(fp);
	   sendMessageToCaller(buff);
       return true;	 
	 }
 //�ڸ����ִ�erroǰ���롰 Erro:����Ȼ��׷��д���ļ�ĩβ���Ƿ��ַ�����ӡ����Ļ��isPrintToScreenָ����true��ӡ�����򲻴�ӡ��
 bool CRunInfoRecord::WriteErro(const char * erro,bool isPrintToScreen)
	 {
		return WriteErro(erro,0,isPrintToScreen);

	 }
  //�ڸ����ִ�erroǰ���롰 Erro:��������ӡ����Ļ
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
   CRunInfoRecord::WriteLine ("//\t ������Ϣ���� \n");
   CRunInfoRecord::Write(dateTime);
   //CRunInfoRecord::Write("//  2012.12.6 17:10:33\n");
   CRunInfoRecord::Write("//\tpasskey2.6 alpha\n");
   CRunInfoRecord::Write("// �й���ѧԺ����ѧ�о���ʮһ��\n\n" );
   CRunInfoRecord::Write("-----------------------------------------\n" );

   CRunInfoRecord::Write("synthesis......\n" );

  
}