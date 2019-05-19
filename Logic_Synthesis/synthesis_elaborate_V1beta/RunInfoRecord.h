/*
     ������  ������Ϣ��¼�� CRunInfoRecord
	 ���ã�  ��¼��������ڼ���Ҫ��־��Ϣ�������֪ͨ���н�����Ϣ���û���ʾ��Ϣ�ȣ�Ҳ�ɼ�¼������Ϣ�Ա���λbugs
	 �汾��  V0.2.02
	 ���ڣ�  2013.1.21
*/
#pragma once
#include"windows.h"
#define WM_TEST WM_USER+1//�Զ�����Ϣ

struct LParamDataSruct
{
public:
	char* lParamData;       //Ҫ���͵���Ϣ
};

void InitRunInfoRecordStatic(char* fileName,HWND hwnd);

class CRunInfoRecord
{
  public:
	CRunInfoRecord(void);
	//�ļ���
	 static char fileName[8192];
	 static HWND mainHwnd; //����ľ��������淢����Ϣʱʹ��
	//�ڸ����ִ�strǰ����tabPrix��tab��׷��д���ļ����Ƿ��ַ�����ӡ����Ļ��isPrintToScreenָ����true��ӡ�����򲻴�ӡ��
	static bool Write(const char * str,int tabPrix,bool isPrintToScreen=true);
	//���ַ���strд���ļ���isPrintToScreenΪtrue���������Ļ���������
    static bool Write(const char * str,bool isPrintToScreen);
	//���ַ���strд���ļ������������Ļ
    static bool Write(const char * str);

	//д��һ���ַ�����tabPrixΪ����ǰ׺tab�ĸ������ú����ڲ������tabPrix���ַ���ǰ��������ǰ׺tab��Ȼ��д���ļ����Ƿ�ﵽ��Ļ��Ĭ�ϴ���Ļ
	static bool WriteLine(const char * strLine,int tabPrix,bool isPrintToScreen=true);
    //дһ�е��ַ������ļ���isPrintToSreenΪtrue��ʾ�������Ļ���������
	static bool WriteLine(const char * strLine,bool isPrintToScreen);
	//дһ�е��ַ������ļ������������Ļ
	static bool WriteLine(const char * strLine);


   /*д����ʾ��Ϣ��
   �����ڲ����Զ�����Infoͷ�����ա�����ǰ׺tab Info����ʾ��Ϣ���ݡ��ĸ�ʽд���ļ�
   tabPrixΪ����ǰ׺tab�ĸ������ú����ڲ������tabPrix���ַ���ǰ��������ǰ׺tab��Ȼ��д���ļ�
   isPrintToScreenΪtrue��ʾ�������Ļ���������
	*/
	static bool WriteInfo(const char * Info,int tabPrix,bool isPrintToScreen=true);
	 //�ڸ����ִ�Infoǰ���롰 Info:����Ȼ��׷��д���ļ�ĩβ���Ƿ��ַ�����ӡ����Ļ��isPrintToScreenָ����true��ӡ�����򲻴�ӡ��
	static bool WriteInfo(const char * Info,bool isPrintToScreen);
	 //�ڸ����ִ�Infoǰ���롰 Info:����Ȼ��׷��д���ļ�ĩβ������ӡ����Ļ
	static bool WriteInfo(const char * Info);

	/*д��warning��Ϣ	
	�����ڲ����Զ�����Warningͷ�����ա�����ǰ׺tab Warning����ʾ��Ϣ���ݡ��ĸ�ʽд���ļ�
	tabPrixΪ����ǰ׺tab�ĸ������ú����ڲ������tabPrix���ַ���ǰ��������ǰ׺tab��Ȼ��д���ļ�
    isPrintToScreenΪtrue��ʾ�������Ļ���������
	*/
   static bool WriteWarning(const char * warning,int tabPrix,bool isPrintToScreen=true);
    //�ڸ����ִ�warningǰ���롰 Warning:����Ȼ��׷��д���ļ�ĩβ���Ƿ��ַ�����ӡ����Ļ��isPrintToScreenָ����true��ӡ�����򲻴�ӡ��
   static bool WriteWarning(const char * warning,bool isPrintToScreen);
    //�ڸ����ִ�warningǰ���롰 Warning:����Ȼ��׷��д���ļ�ĩβ������ӡ����Ļ
   static bool WriteWarning(const char * warning);

	/*д��erro��Ϣ
	�����ڲ����Զ�����Erroͷ�����ա�����ǰ׺tab Erro����ʾ��Ϣ���ݡ��ĸ�ʽд���ļ�
	tabPrixΪ����ǰ׺tab�ĸ������ú����ڲ������tabPrix���ַ���ǰ��������ǰ׺tab��Ȼ��д���ļ�
	isPrintToScreenΪtrue��ʾ�������Ļ���������
	*/
	static bool WriteErro(const char * erro,int tabPrix,bool isPrintToScreen=true);
	 //�ڸ����ִ�erroǰ���롰 Erro:����Ȼ��׷��д���ļ�ĩβ���Ƿ��ַ�����ӡ����Ļ��isPrintToScreenָ����true��ӡ�����򲻴�ӡ��
    static bool WriteErro(const char * erro,bool isPrintToScreen);
	  //�ڸ����ִ�erroǰ���롰 Erro:����Ȼ��׷��д���ļ�ĩβ������ӡ����Ļ
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
  �÷���
    1. ��main�����Ŀ�ʼ�����ó�ʼ������
                    void InitRunInfoRecordStatic(char* fileName,HWND hwnd); //д���ļ���·�����ļ������������ľ�����ײ���Կ���ΪNULL,ֻ����һ��
  
	  ����
	   InitRunInfoRecordStatic("test.txt",0);//ֻ����һ�Σ���ֻ��ʼ��һ�μ���
  
   2. �ڴ�֮�󣬲����ĸ��������������������ַ�ʽ֮һ��д���м�¼�����ÿ����ļ�����رյĹ��̣��ù����Ѿ���װ����̬�������У�

   ����1��ͨ�� ����::������ ���þ�̬����
   CRunInfoRecord::WriteLine ("---------------װ�䱨��--------------");
   CRunInfoRecord::WriteInfo ("��vqm�ļ�\n");

   CRunInfoRecord::WriteLine ("out��5"��2); //��ʾ����"out��5"ǰ����2��tab���"		out��5"��д���ļ���������Ļ���
   CRunInfoRecord::WriteLine ("out��5"��2��true); //������CRunInfoRecord::WriteLine ("out��5"��2)���ù�����ͬ
   CRunInfoRecord::WriteLine ("out��5"��2��fasle); //��ʾ����"out��5"ǰ����2��tab���"		out��5"��д���ļ�����������Ļ���

   CRunInfoRecord::WriteInfo ("��net�ļ�");//��ʾ�� "��net�ļ�" ���"Info����net�ļ�"��д���ļ���������Ļ���
   CRunInfoRecord::WriteInfo ("��net�ļ�"��true);//���������� CRunInfoRecord::WriteInfo ("��net�ļ�")��ͬ
   CRunInfoRecord::WriteInfo ("��net�ļ�"��false);//��ʾ������д�ļ���ͬ����������Ļ���




   ����2��ͨ��  ����.����   ���þ�̬����
   CRunInfoRecord  runInfREcord;
   runInfREcord.WriteInfo ("����net�ļ�\n");

   
*/



/*
�÷�ʵ����

void main()
{
	//����
   void InitRunInfoRecordStatic(char* fileName,HWND hwnd);
   void test();
  //����
   InitRunInfoRecordStatic("test.txt",0);//ֻ����һ�Σ���ֻ��ʼ��һ�μ��ɣ��ڶ�������HWND hwndΪ������Ϣ�Ĳ������ײ����ʱ�ô���Ϊ0����
    test();  //������Ҫд���м�¼�ĺ���

}

void test() 
{ 
   //�����ֵ��÷�ʽ���������ǵȼ۵�
	//CRunInfoRecord::mainHwnd(this::h);
   //ֱ�Ӳ��� ����::������ ��ʽ����
   CRunInfoRecord::WriteLine ("---------------װ�䱨��--------------\n");
   CRunInfoRecord::WriteInfo ("��vqm�ļ�\n");
   CRunInfoRecord::WriteInfo ("��ȡvqm�ļ���le��Ϣ\n",1);//����һ��table
   CRunInfoRecord::WriteInfo ("��ȡvqm�ļ���M4K��Ϣ\n",1);
   CRunInfoRecord::WriteWarning("vqm�ļ���û��M4K��Ϣ\n",2��false);//��������tableд���ļ���������Ļ���������Ϣ
   CRunInfoRecord::WriteInfo ("��ȡvqm�ļ���DSP��Ϣ\n",1);
   CRunInfoRecord::WriteInfo ("�ر�vqm�ļ�\n");
   
   //ͨ�� ��������.���� ��ʽ����
   CRunInfoRecord  runInfREcord;
   runInfREcord.WriteInfo ("����net�ļ�\n");
   runInfREcord.WriteInfo ("д��le��Ϣ\n",1);
   runInfREcord.WriteInfo ("д��DSP��Ϣ\n",1);
   runInfREcord.WriteErro ("д��DSP��Ϣ����\n",1);
   runInfREcord.WriteInfo ("�ر�net�ļ�\n");

   CRunInfoRecord::WriteLine ("------------װ�䱨�����--------------\n");
	return  ;
}

*/