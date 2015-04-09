
// stdafx.cpp : 只包括标准包含文件的源文件
// lookup.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

void ConvertUtf8ToGBK(CString& strUtf8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL, 0);
    unsigned short * wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);
    MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte (CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);

    strUtf8 = szGBK;
    delete[] szGBK;
    delete[] wszGBK;
}


void ConvertGBKToUtf8(CString& strGBK)
{
    int len = MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, NULL, 0);
    unsigned short * wszUtf8 = new unsigned short[len + 1];
    memset(wszUtf8, 0, len * 2 + 2);
    MultiByteToWideChar(CP_ACP, 0, (LPCTSTR)strGBK, -1, (LPWSTR)wszUtf8, len);

    len = WideCharToMultiByte(CP_UTF8, 0, (LPWSTR)wszUtf8, -1, NULL, 0, NULL, NULL);
    char *szUtf8 = new char[len + 1];
    memset(szUtf8, 0, len + 1);
    WideCharToMultiByte (CP_UTF8, 0, (LPWSTR)wszUtf8, -1, szUtf8, len, NULL, NULL);

    strGBK = szUtf8;
    delete[] szUtf8;
    delete[] wszUtf8;
}

/********************************************************************/
/*																	*/
/* Function name : GetAppDir										*/
/* Description   : Get application directory.						*/
/*																	*/
/********************************************************************/
void GetAppDir(CString& strAppDir)
{
	TCHAR	szFullPath[MAX_PATH];
	TCHAR	szDir[_MAX_DIR];
	TCHAR	szDrive[_MAX_DRIVE];

	// Get application's full path.
	::GetModuleFileName(NULL, szFullPath, MAX_PATH);

	// Break full path into seperate components.
	_splitpath(szFullPath, szDrive, szDir, NULL, NULL);

	// Store application's drive and path
	strAppDir.Format(_T("%s%s"), szDrive, szDir);
}
