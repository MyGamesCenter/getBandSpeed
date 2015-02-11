// stdafx.cpp : 只包括标准包含文件的源文件
// SpeedTest02.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


double Speed = 0;
CString gl_ipQueue = _T("http://10000.gd.cn/getAccountInfo2.php");
vector<std::string> ipQueue;
int ConnCount = 0;
vector<SOCKET> socks;
vector<HANDLE> handles;

double sumSpeed = 0;
CString account = _T("");//用户名
CString ipAddress = _T("");//用户IP
CString city = _T("");//用户归属地
double bandWidth = 0;//购买带宽
double avgSpeed = 0;//平均速度
CString speedLogs = _T("");//瞬时速度

int isTested = 0;

double m_nRecvBytesPS = 0;

HWND m_hWnd = NULL;
UINT msgName = 0;
int msgValue = 0;
char* ip = "";
char *w2c(wchar_t *lpcwszStr)
{
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,FALSE);
	char *psText;
	psText = new char[dwNum+1];
	memset(psText,0,dwNum+1);
	if(psText)
	{
		WideCharToMultiByte (CP_OEMCP,NULL,lpcwszStr,-1,psText,dwNum,NULL,FALSE);
		return psText;
	}
	else return NULL;
}
wchar_t *c2w(char *lpcszStr)
{
	DWORD dwNum = MultiByteToWideChar(CP_OEMCP,NULL,lpcszStr,-1,NULL,0);
	wchar_t *psText;
	psText = new wchar_t[dwNum+1];
	memset(psText,0,dwNum+1);
	if(psText)
	{
		MultiByteToWideChar(CP_OEMCP,NULL,lpcszStr,-1,psText,dwNum);
		return psText;
	}
	else return NULL;
}

CString UTF8ToUnicode(LPCSTR szU8Buf)
{
	int len;
	len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)szU8Buf, -1, NULL,0);
	WCHAR * wszUtf8 = new WCHAR[len+1]; 
	memset(wszUtf8, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)szU8Buf, -1, wszUtf8, len);
	CString strMsg(wszUtf8);
	delete[] wszUtf8;
	return strMsg;
	//MessageBoxW(NULL,u8,u8,MB_OK);
}
LPCSTR UnicodeToUTF8(LPCWSTR pUnicode)
{
	int len;
	len = WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, NULL,0,NULL,NULL);
	char * wszUtf8 = new char[len+1];
	memset(wszUtf8, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, pUnicode, -1, wszUtf8, len,NULL,NULL);
	return (LPSTR)wszUtf8;
}
