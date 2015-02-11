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
	//开始下载测速
	void StartDownloadSpeedTest(HWND hWnd,UINT type,int msg,char* ip);
	//开始上传测速
	void StartUploadSpeedTest(HWND hWnd,UINT type,int msg,char* ip);
	//获取账号反查信息
	void GetUserInfo();
	//发送测速结果值
	//void SendResult(HWND hWnd,UINT msgName,WPARAM msgValue,int time,int count,double result);
};

