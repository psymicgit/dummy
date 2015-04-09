#pragma once

#define VC_EXTRALEAN		
#include <afxwin.h>         
#include <WinSock2.h>
#include <afxext.h>      
#include <afxdisp.h>     
#include <afxdtctl.h>	
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>		
#endif

bool IsNumber(const char *pszIn);
bool IsHex(const char *pszIn);	//16进制
bool IsOctal(const char *pszIn);	//8进制
#define  WM_COMMAND_LOG_MSG              WM_USER + 101

#ifdef _GENERATE_DLL_
#include "sdtoolkitdefines.h"
#else
enum LOG_LEVEL
{
	LOG_WARNING = 0 ,
	LOG_ERROR ,
	LOG_CRITIL,
	LOG_INFO ,
};
#endif
#include "sdtype.h"

typedef int                 SDBOOL;


enum emCharSet
{
	em_ASCII = 0,
	em_GBK,
	em_UTF16LE,
	em_UTF8,
};

void ConvertUtf8ToGBK(CString& strUtf8);
void ConvertGBKToUtf8(CString& strGBK);

extern emCharSet g_emCharSetHost;
extern 	BOOL		m_bCompare;