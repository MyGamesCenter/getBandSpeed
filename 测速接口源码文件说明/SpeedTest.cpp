/******
*author liangyong 
*��������
***********/

#include "stdafx.h"
#include "Header.h"
#include "conio.h"
#include "IniManager.h"
#include "FileUtil.h"
#include "SocketMgr.h"
#include "HttpClient.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

/****
*@author liangyong
*@param hWnd �������߳�
*@param msgName ��Ϣ��
*@param msgValue ��Ϣֵ
*@param isDownload �ϴ�/���ر�ʶ��TRUEΪ���أ�FALSEΪ�ϴ�
******/
void StartSpeed(HWND hWnd,UINT msgName,int msgValue,bool isDownload)
{
	AllocConsole();
	CIniManager iniManager;
	CSocketMgr sock;
	CHttpClient httpClient;

	///��ȡ�˺���Ϣ�Լ����Է�����IP�б�
	//ADSL_12.0_0.5_gzDSL38056189_26_������_218.19.41.92_183.63.146.250,218.15.238.166,59.39.183.126,61.145.85.202_183.63.146.250
	CString result_temp = httpClient.doGet(gl_ipQueue);
	CString result = _T("");
	//result.Format(_T("%s"),ipTmp);
	result = iniManager.GetAllTestIp(result_temp);
  	CT2A http_result_temp(result);
	std::string str(http_result_temp);
	std::string temp_str = "";
	temp_str = iniManager.getIpQueues(str);

	//���Է�����IP�б�
	//183.63.146.250,218.15.238.166,59.39.183.126,61.145.85.202
	ipQueue = iniManager.split(temp_str);///����IP�б�
	//_cprintf("startSpeed::msgName = %d,startSpeed::msgValue = %d\r\n",msgName,msgValue);
	socks.clear();
	if(isDownload)//���ز���
	{
		for(int i=0;i<ipQueue.size();i++)
		{
			if(i<ipQueue.size())
			{
				const char* ip_temp;
				char* ip;
				ip_temp = ipQueue[i].c_str();
				ip = new char[strlen(ip_temp)+1];
				strcpy(ip,ip_temp);
				_cprintf("ip = %s\r\n",ip);
				sock.StartDownloadSpeedTest(hWnd,msgName,msgValue,ip);
				//_cprintf("msgName = %d,msgValue = %d\r\n",msgName,msgValue);
			}
			else
			{
				const char* ip_temp;
				char* ip;
				ip_temp = ipQueue[0].c_str();
				ip = new char[strlen(ip_temp)+1];
				strcpy(ip,ip_temp);
				_cprintf("ip = %s\r\n",ip);
				sock.StartDownloadSpeedTest(hWnd,msgName,msgValue,ip);
				//_cprintf("msgName = %d,msgValue = %d\r\n",msgName,msgValue);
			}
			
		}
		_cprintf("size is %d\r\n",handles.size());
	}
	else//�ϴ�����
	{
		for(int i=0;i<ipQueue.size();i++)
		{
			if(i<ipQueue.size())
			{
				const char* ip_temp;
				char* ip;
				ip_temp = ipQueue[i].c_str();
				ip = new char[strlen(ip_temp)+1];
				strcpy(ip,ip_temp);
				sock.StartUploadSpeedTest(hWnd,msgName,msgValue,"192.168.1.118");
			}
			else
			{
				const char* ip_temp;
				char* ip;
				ip_temp = ipQueue[0].c_str();
				ip = new char[strlen(ip_temp)+1];
				strcpy(ip,ip_temp);
				sock.StartUploadSpeedTest(hWnd,msgName,msgValue,"192.168.1.118");
			}
			
		}
	}
}