#include "breakpad.h"
#include "common/server/utility.h"

#ifndef __linux__

google_breakpad::ExceptionHandler *CBreakPad::sm_pHandler=NULL;

//windows平台下的break回调代码
bool CBreakPad::DumpsCallback(const wchar_t *pStrDumpPath, const wchar_t *pStrID,  
	void *pContext, EXCEPTION_POINTERS *pExceptInfo,  
	MDRawAssertionInfo *pAssertion,  
	bool bSucceeded)
{  
	if (bSucceeded) 
	{  
		wstring strDes=L"dump路径：";
		strDes+=pStrDumpPath;
		strDes+=L"\\";
		strDes+=pStrID;
		strDes+=L".dmp";
		//::MessageBoxW( NULL, strDes.c_str(), L"程序已经崩溃", NULL ); 
	} 
	else 
	{  
		//::MessageBoxW( NULL, L"Minidump失败", L"程序已经崩溃", NULL );  
	}  
	fflush(stdout);
	return bSucceeded;  
}

#else

google_breakpad::MinidumpDescriptor *CBreakPad::sm_pDumpDescript(NULL);
google_breakpad::ExceptionHandler *CBreakPad::sm_apHandler(NULL);

//linux平台下的break回调代码
bool CBreakPad::DumpsCallback(const google_breakpad::MinidumpDescriptor& descriptor,
	void* pContext,
	bool bSucceeded)
{
	printf("Dump path: %s\n", descriptor.path());
	return bSucceeded;
}

#endif

VOID CBreakPad::Init()
{
#ifndef __linux__
	wchar_t szPath[MAX_PATH];
	GetCurrentDirectoryW( MAX_PATH, szPath );
	sm_pHandler=new google_breakpad::ExceptionHandler(
		szPath, NULL, DumpsCallback, NULL,  
		google_breakpad::ExceptionHandler::HANDLER_ALL) ;  
#else
	sm_pDumpDescript=new google_breakpad::MinidumpDescriptor("");
	sm_pHandler=new ( *sm_pDumpDescript,NULL,
		DumpsCallback, NULL, true, -1 );
#endif
}


VOID CBreakPad::UnInit()
{
	SAFE_DELETE( sm_pHandler );
#ifdef __linux__
	SAFE_DELETE( sm_pDumpDescript );
#endif
}