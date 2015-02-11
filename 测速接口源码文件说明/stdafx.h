// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 类
#include <afxodlgs.h>       // MFC OLE 对话框类
#include <afxdisp.h>        // MFC 自动化类
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 数据库类
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 数据库类
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT
#define WM_MSG WM_USER + 102

#include "string"
#include <vector>
using namespace std;
extern double Speed;//测速值
extern CString gl_ipQueue;//账号反查地址
extern int ConnCount;
extern vector<std::string> ipQueue;//测速服务器IP列表
extern double sumSpeed;//用于计算平均速度
extern CString account;//用户名
extern CString ipAddress;//用户IP
extern CString city;//用户归属地
extern double bandWidth;//购买带宽
extern double avgSpeed;//平均速度
extern CString speedLogs;//瞬时速度


char *w2c(wchar_t *lpcwszStr);
wchar_t *c2w(char *lpcszStr);

extern vector<SOCKET> socks;
extern vector<HANDLE> handles;
CString UTF8ToUnicode(LPCSTR szU8Buf);
LPCSTR UnicodeToUTF8(LPCWSTR pUnicode);

extern HWND m_hWnd;//调用程序父窗口句柄
extern UINT msgName;//消息名
extern int msgValue;//消息值
extern char* ip;//服务器IP

extern int isTested;//是否已经测试

extern double m_nRecvBytesPS;//接收数据统计变量