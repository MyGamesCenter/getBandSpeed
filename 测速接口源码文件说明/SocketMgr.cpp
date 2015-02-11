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

///线程数据同步控制变量
CCriticalSection critical_Section;
CRITICAL_SECTION g_csA;
/*
author liangyong
*****
下载测速线程创建
*****
*/
DWORD WINAPI  DownloadSpeedThread(LPVOID param)
{
	CSocketMgr* pOwner = (CSocketMgr*)param;
	
	
	
	//_cprintf("ip = %s\r\n",ip);
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0); 
	SOCKADDR_IN addrSrv;  
	addrSrv.sin_addr.S_un.S_addr=inet_addr(pOwner->ip);//服务器端的地址  
	addrSrv.sin_family=AF_INET;  
	addrSrv.sin_port=htons(12345);  
	int conn = connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	//将线程开启的Socket保存，用于测速结束后关闭尚在连接的Socket
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
		///向服务器发送数据包
		int Send = send(sockClient,dataBuff,3,0);

		if(Send<0)//数据发送失败
		{
			_cprintf("send fail,stop!\r\n");
			return -1;
		}
		
		int nRecvCount =recv(sockClient,ReadBuf,1024*256,0);//接收服务器端发送的数据
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
			///从下标值3，即第4个测速值开始到下标为12(即第13个测速值)的测速值结束，用于计算平均值
			if(i>2&&i<=12)
			{
				sumSpeed += m_nRecvBytesPS;
			}
			///保存15个测速值
			speedLogs.AppendFormat(_T("%.2f,"),(double)m_nRecvBytesPS*8/1000000);
			m_nRecvBytesPS = 0;//将数据统计变量置0
			i++;
		}
		//
	}
	//CString avgSpeedTmp = _T("");
	//avgSpeedTmp.Format(_T("%.2f"),(double)sumSpeed*8/(1000000*10));//计算平均值，保留2位小数
	//_cprintf("%ws\r\n",speedLogs);
	//double avgSpeed = _tcstod(avgSpeedTmp,0);//类型转换

	///获取测速结果JSON字符串
	//CString str = //pOwner->jsonUtil.resultJson(_T("PC"),account,ipAddress,city,bandWidth,avgSpeed,speedLogs);
	
	///TODO：在此处添加上传数据代码

		
	closesocket(sockClient);
	
	//当第一个测速线程成功执行结束后，把剩下的所有socket关闭
	/*for(int i=0;i<socks.size();i++)
	{
		closesocket(socks[i]);
	}*/
	return 0;
	
}

/****
*author liangyong 
******
上传测速线程创建
******
******/
DWORD WINAPI  UploadSpeedThread(LPVOID param)
{
	CSocketMgr* pOwner = (CSocketMgr*)param;

	//socks.clear();///清空sock链表

	double m_nRecvBytesPS = 0;
	double nRecvCount = 0;
		  
	SOCKET sockClient=socket(AF_INET,SOCK_STREAM,0);  
  
	SOCKADDR_IN addrSrv;  
	addrSrv.sin_addr.S_un.S_addr=inet_addr(pOwner->ip);//服务器端的地址  
	addrSrv.sin_family=AF_INET;  
	addrSrv.sin_port=htons(12346);  //上传测速端口12346

	char dataBuff[3] = {88,10,0};///测速端需向服务器发送的认证字符数组
	char SendBuf[1024*10];//测速端向服务器发送的随机数据包
	memset(SendBuf,0,1024*10);//随机数据数组初始化
	//给随机数据数组赋值
	for(int j=0;j<1024*10;j++)
	{
		SendBuf[j] = abs(rand()*254+1);
	}

	DWORD time = 0;
	int conn = connect(sockClient,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));//连接上传服务器

	_cprintf("conn = %d\r\n",conn);
	if(conn<0)
	{
		return -1;
	}
	else
	{
		socks.push_back(sockClient);//将开启线程连接的Socket保存到链表
	}
	
//	critical_Section.Lock();
	int m = 0;
	int sumRet = 0;
	while(m<3)
	{///循环向服务器发送三次认证字符数组
		int ret = send(sockClient,dataBuff,3,0);
		sumRet += ret;
		m ++;
	}
	
	int i =0;//计数器
	
	while(i<15)
	{
		DWORD start = timeGetTime();
		nRecvCount = send(sockClient,SendBuf,1024*10,0);//发送数据包给服务器
		if(nRecvCount<0&&isTested==1)
		{//测试失败
			_cprintf("send fail! isTested = %d\r\n",isTested);
			::SendMessage(m_hWnd,msgName,msgValue,-1);
			return -1;
		}
		//_cprintf("isTested = %d\r\n",isTested);
		m_nRecvBytesPS += nRecvCount;//将服务器返回的接收字节码累加，统计测速值
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
			///保存15个测速值
			speedLogs.AppendFormat(_T("%.2f,"),(double)m_nRecvBytesPS*8/1000000);
			m_nRecvBytesPS = 0;//将数据统计变量置0
			i++;
		}
	}
	//CString avgSpeedTmp = _T("");
	//avgSpeedTmp.Format(_T("%.2f"),(double)sumSpeed*8/(1000000*10));//计算平均值，保留2位小数
	
	//double avgSpeed = _tcstod(avgSpeedTmp,0);//类型转换
	///获取测速结果JSON字符串
	//CString str = //pOwner->jsonUtil.resultJson(_T("PC"),account,ipAddress,city,bandWidth,avgSpeed,speedLogs);
	
	//_cprintf("str = %ws\r\n",str);

	isTested = 1;
	///TODO：在此处添加上传数据代码
//	critical_Section.Unlock();
	closesocket(sockClient);
	//当第一个测速线程成功执行结束后，把剩下的所有socket关闭
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
	///获取账号信息
	GetUserInfo();
	
}


CSocketMgr::~CSocketMgr(void)
{
	WSACleanup(); 
	DeleteCriticalSection(&g_csA);
}

///获取账号反查信息
void CSocketMgr::GetUserInfo()
{
	////ADSL_12.0_0.5_gzDSL38056189_26_广州市_218.19.41.92_183.63.146.250,218.15.238.166,59.39.183.126,61.145.85.202_183.63.146.250
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
	
	account = vec[3].c_str();//宽带账号
	bandWidth = atof(vec[1].c_str());//签约带宽
	ipAddress = vec[6].c_str();//测试IP地址
	city = vec[5].c_str();//IP归属地

	//_cprintf("account = %ws,bandWidth = %d,ipAddress = %ws,city = %ws\r\n",account,bandWidth,ipAddress,city);
}

void CSocketMgr::StartDownloadSpeedTest(HWND hWnd,UINT type,int msg,char* ip)
{
	this->ip = ip;
	m_hWnd = hWnd;//父窗口句柄
	msgName = type;//消息名
	msgValue = msg;//消息值
	DWORD ThreadID;//线程ID
	HANDLE handle = CreateThread(NULL,0,DownloadSpeedThread,this,0,&ThreadID);
}

void CSocketMgr::StartUploadSpeedTest(HWND hWnd,UINT type,int msg,char* ip)
{
	this->ip = ip;
	m_hWnd = hWnd;//父窗口句柄
	msgName = type;//消息名
	msgValue = msg;//消息值
	_cprintf("msgName = %d, msgValue = %d\r\n",msgName,msgValue);
	DWORD ThreadID;//线程ID
	HANDLE handle = CreateThread(NULL,0,UploadSpeedThread,this,0,&ThreadID);
}
