/****author liangyong
*2014-7-14
***/
#pragma once
#include "HttpClient.h"
#include "JsonUtil.h"
class CSocketMgr
{
public:
	CSocketMgr(void);
	~CSocketMgr(void);

public:
	int ConnCount;
	
	double sumSpeed;

	char* ip;
	int times;
	int flag;
	HWND hWnd;
	UINT type;
	int msg;
	CJsonUtil jsonUtil;

	WORD wVersionRequested;  
    WSADATA wsaData;  
    int err;

	CHttpClient httpClient;
	SOCKET sockClient;


public:
	//��ʼ���ز���
	void StartDownloadSpeedTest(HWND hWnd,UINT type,int msg,char* ip);
	//��ʼ�ϴ�����
	void StartUploadSpeedTest(HWND hWnd,UINT type,int msg,char* ip);
	//��ȡ�˺ŷ�����Ϣ
	void GetUserInfo();
	//���Ͳ��ٽ��ֵ
	//void SendResult(HWND hWnd,UINT msgName,WPARAM msgValue,int time,int count,double result);
};

