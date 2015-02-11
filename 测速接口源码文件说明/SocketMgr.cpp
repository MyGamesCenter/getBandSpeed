#include "StdAfx.h"
#include "SocketMgr.h"
#include <WINSOCK2.H>  
#include <stdio.h> 
#include <time.h>
#include <math.h>
#include <conio.h>
#include <afxmt.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"ws2_32.lib")

///�߳�����ͬ�����Ʊ���
CCriticalSection critical_Section;
CRITICAL_SECTION g_csA;
/*
author liangyong
*****
���ز����̴߳���
*****
*/
DWORD WINAPI  DownloadSpeedThread(LPVOID param)
{
	CSocketMgr* pOwner = (CSocketMgr*)param;
	
	
	
	//_cprintf("ip = %s\r\n",ip);
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0); 
	SOCKADDR_IN addrSrv;  
	addrSrv.sin_addr.S_un.S_addr=inet_addr(pOwner->ip);//�������˵ĵ�ַ  
	addrSrv.sin_family=AF_INET;  
	addrSrv.sin_port=htons(12345);  
	int conn = connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	//���߳̿�����Socket���棬���ڲ��ٽ�����ر��������ӵ�Socket
	socks.push_back(sockClient);

	char dataBuff[3] = {88,1,0};
	char ReadBuf[1024*256];
	memset(ReadBuf,0,1024*256);	

	DWORD time = 0;

	double nRecvCount = 0;
		
	int i=1;
	
	while(i<=15)
	{
		DWORD start = timeGetTime();
		///��������������ݰ�
		int Send = send(sockClient,dataBuff,3,0);

		if(Send<0)//���ݷ���ʧ��
		{
			_cprintf("send fail,stop!\r\n");
			return -1;
		}
		
		int nRecvCount =recv(sockClient,ReadBuf,1024*256,0);//���շ������˷��͵�����
		//DWORD id = GetCurrentThreadId();
		//_cprintf("id = %d\r\n",id);
		critical_Section.Lock();
		//EnterCriticalSection(&g_csA);
		m_nRecvBytesPS += nRecvCount;
		//LeaveCriticalSection(&g_csA);
		critical_Section.Unlock();
		time += timeGetTime() - start;
		if(time>1000*i)
		{
			::SendMessage(m_hWnd,msgName,msgValue,(double)m_nRecvBytesPS);
			//_cprintf("msgName = %d, msgValue = %d\r\n",msgName,msgValue);
			//_cprintf("pOwner->type = %d,pOwner->msg = %d\r\n",pOwner->type,pOwner->msg);
			_cprintf("i = %d,\tm_nRecvBytesPS = %.2f\r\n",i,m_nRecvBytesPS);
			///���±�ֵ3������4������ֵ��ʼ���±�Ϊ12(����13������ֵ)�Ĳ���ֵ���������ڼ���ƽ��ֵ
			if(i>2&&i<=12)
			{
				sumSpeed += m_nRecvBytesPS;
			}
			///����15������ֵ
			speedLogs.AppendFormat(_T("%.2f,"),(double)m_nRecvBytesPS*8/1000000);
			m_nRecvBytesPS = 0;//������ͳ�Ʊ�����0
			i++;
		}
		//
	}
	//CString avgSpeedTmp = _T("");
	//avgSpeedTmp.Format(_T("%.2f"),(double)sumSpeed*8/(1000000*10));//����ƽ��ֵ������2λС��
	//_cprintf("%ws\r\n",speedLogs);
	//double avgSpeed = _tcstod(avgSpeedTmp,0);//����ת��

	///��ȡ���ٽ��JSON�ַ���
	//CString str = //pOwner->jsonUtil.resultJson(_T("PC"),account,ipAddress,city,bandWidth,avgSpeed,speedLogs);
	
	///TODO���ڴ˴�����ϴ����ݴ���

		
	closesocket(sockClient);
	
	//����һ�������̳߳ɹ�ִ�н����󣬰�ʣ�µ�����socket�ر�
	/*for(int i=0;i<socks.size();i++)
	{
		closesocket(socks[i]);
	}*/
	return 0;
	
}

/****
*author liangyong 
******
�ϴ������̴߳���
******
******/
DWORD WINAPI  UploadSpeedThread(LPVOID param)
{
	CSocketMgr* pOwner = (CSocketMgr*)param;

	//socks.clear();///���sock����

	double m_nRecvBytesPS = 0;
	double nRecvCount = 0;
		  
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);  
  
	SOCKADDR_IN addrSrv;  
	addrSrv.sin_addr.S_un.S_addr=inet_addr(pOwner->ip);//�������˵ĵ�ַ  
	addrSrv.sin_family=AF_INET;  
	addrSrv.sin_port=htons(12346);  //�ϴ����ٶ˿�12346

	char dataBuff[3] = {88,10,0};///���ٶ�������������͵���֤�ַ�����
	char SendBuf[1024*10];//���ٶ�����������͵�������ݰ�
	memset(SendBuf,0,1024*10);//������������ʼ��
	//������������鸳ֵ
	for(int j=0;j<1024*10;j++)
	{
		SendBuf[j] = abs(rand()*254+1);
	}

	DWORD time = 0;
	int conn = connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//�����ϴ�������

	_cprintf("conn = %d\r\n",conn);
	if(conn<0)
	{
		return -1;
	}
	else
	{
		socks.push_back(sockClient);//�������߳����ӵ�Socket���浽����
	}
	
//	critical_Section.Lock();
	int m = 0;
	int sumRet = 0;
	while(m<3)
	{///ѭ�������������������֤�ַ�����
		int ret = send(sockClient,dataBuff,3,0);
		sumRet += ret;
		m ++;
	}
	
	int i =0;//������
	
	while(i<15)
	{
		DWORD start = timeGetTime();
		nRecvCount = send(sockClient,SendBuf,1024*10,0);//�������ݰ���������
		if(nRecvCount<0&&isTested==1)
		{//����ʧ��
			_cprintf("send fail! isTested = %d\r\n",isTested);
			::SendMessage(m_hWnd,msgName,msgValue,-1);
			return -1;
		}
		//_cprintf("isTested = %d\r\n",isTested);
		m_nRecvBytesPS += nRecvCount;//�����������صĽ����ֽ����ۼӣ�ͳ�Ʋ���ֵ
		time += timeGetTime() - start;
		if(time>1000*i)
		{
			::SendMessage(m_hWnd,msgName,msgValue,(double)m_nRecvBytesPS);
			//_cprintf("msgName = %d, msgValue = %d\r\n",msgName,msgValue);
			_cprintf("i = %d,\tm_nRecvBytesPS = %.2f\r\n",i,m_nRecvBytesPS);
			if(i>2&&i<=12)
			{
				sumSpeed += m_nRecvBytesPS;
			}
			///����15������ֵ
			speedLogs.AppendFormat(_T("%.2f,"),(double)m_nRecvBytesPS*8/1000000);
			m_nRecvBytesPS = 0;//������ͳ�Ʊ�����0
			i++;
		}
	}
	//CString avgSpeedTmp = _T("");
	//avgSpeedTmp.Format(_T("%.2f"),(double)sumSpeed*8/(1000000*10));//����ƽ��ֵ������2λС��
	
	//double avgSpeed = _tcstod(avgSpeedTmp,0);//����ת��
	///��ȡ���ٽ��JSON�ַ���
	//CString str = //pOwner->jsonUtil.resultJson(_T("PC"),account,ipAddress,city,bandWidth,avgSpeed,speedLogs);
	
	//_cprintf("str = %ws\r\n",str);

	isTested = 1;
	///TODO���ڴ˴�����ϴ����ݴ���
//	critical_Section.Unlock();
	closesocket(sockClient);
	//����һ�������̳߳ɹ�ִ�н����󣬰�ʣ�µ�����socket�ر�
	//_cprintf("size is %d\r\n",socks.size());
	//for(int i=0;i<socks.size();i++)
	//{
	//	closesocket(socks[i]);
	//}
	return 0;
}

CSocketMgr::CSocketMgr(void)
{
	AllocConsole();
	ConnCount = 0;
	
	ip = "";
	times = 0;
	flag = 0;
	srand(time(NULL));
    wVersionRequested = MAKEWORD( 1, 1 );  
      
    err = WSAStartup( wVersionRequested, &wsaData );  
    if ( err != 0 ) {  
        return;  
    }  
      
    if ( LOBYTE( wsaData.wVersion ) != 1 ||  
        HIBYTE( wsaData.wVersion ) != 1 ) {  
        WSACleanup( );  
        return;   
    } 
	InitializeCriticalSection(&g_csA);
	///��ȡ�˺���Ϣ
	GetUserInfo();
	
}


CSocketMgr::~CSocketMgr(void)
{
	WSACleanup(); 
	DeleteCriticalSection(&g_csA);
}

///��ȡ�˺ŷ�����Ϣ
void CSocketMgr::GetUserInfo()
{
	////ADSL_12.0_0.5_gzDSL38056189_26_������_218.19.41.92_183.63.146.250,218.15.238.166,59.39.183.126,61.145.85.202_183.63.146.250
	CString result = httpClient.doGet(gl_ipQueue);
	string str = CT2A(result.GetBuffer());
	vector<std::string> vec;
	std::string token = "_";
    int nend=0;   
    int nbegin=0;   
    while(nend != -1)   
    {   
        nend = str.find_first_of(token, nbegin);   
        if(nend == -1)   
            vec.push_back(str.substr(nbegin, str.length()-nbegin));   
        else  
            vec.push_back(str.substr(nbegin, nend-nbegin));   
        nbegin = nend + 1;   
    } 
	
	account = vec[3].c_str();//����˺�
	bandWidth = atof(vec[1].c_str());//ǩԼ����
	ipAddress = vec[6].c_str();//����IP��ַ
	city = vec[5].c_str();//IP������

	//_cprintf("account = %ws,bandWidth = %d,ipAddress = %ws,city = %ws\r\n",account,bandWidth,ipAddress,city);
}

void CSocketMgr::StartDownloadSpeedTest(HWND hWnd,UINT type,int msg,char* ip)
{
	this->ip = ip;
	m_hWnd = hWnd;//�����ھ��
	msgName = type;//��Ϣ��
	msgValue = msg;//��Ϣֵ
	DWORD ThreadID;//�߳�ID
	HANDLE handle = CreateThread(NULL,0,DownloadSpeedThread,this,0,&ThreadID);
}

void CSocketMgr::StartUploadSpeedTest(HWND hWnd,UINT type,int msg,char* ip)
{
	this->ip = ip;
	m_hWnd = hWnd;//�����ھ��
	msgName = type;//��Ϣ��
	msgValue = msg;//��Ϣֵ
	_cprintf("msgName = %d, msgValue = %d\r\n",msgName,msgValue);
	DWORD ThreadID;//�߳�ID
	HANDLE handle = CreateThread(NULL,0,UploadSpeedThread,this,0,&ThreadID);
}
