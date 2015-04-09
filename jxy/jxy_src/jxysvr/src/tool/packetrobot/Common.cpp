// SHANDA GAMES PROPRIETARY INFORMATION
//
// This software is supplied under the terms of a license agreement or
// nondisclosure agreement with Shanda Games and may not 
// be copied or disclosed except in accordance with the terms of that 
// agreement.
//
//      Copyright (c) 1999-2010 Shanda Games.
//      All Rights Reserved.
//
// Shanda Games Limited, Shanghai, China, 201203
// http://www.shandagames.com

#include "stdafx.h"
#include "common.h"
#include "time.h"

#ifndef UNICODE

// asc字符串转utf-8字符串
INT32 ASC2UTF(TCHAR* _Output, TCHAR* _Input)
{
	wchar_t wczTemp[256] = {0};
	CHAR szTemp[256] = {0};
	if(NULL == _Input)
	{
		return 0;
	}
	INT32 nLen = MultiByteToWideChar(CP_ACP, 0 , _Input, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0 , _Input, -1, wczTemp, nLen);
	nLen = WideCharToMultiByte(CP_UTF8, 0, wczTemp, -1, NULL, 0, NULL, NULL); 
	WideCharToMultiByte (CP_UTF8, 0, wczTemp, -1, szTemp, nLen, NULL,NULL); 
	strcpy(_Output, szTemp);
	return nLen - 1;
}

// utf-8字符串转asc字符串
INT32 UTF2ASC(TCHAR* _Output, TCHAR* _Input)
{
	int len=MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)_Input, -1, NULL,0); 

	wchar_t * wszGBK = new wchar_t[len+1];
	memset(wszGBK, 0, len * 2 + 2); 

	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)_Input, -1, wszGBK, len); 
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL); 

	memset(_Output, 0, len + 1); 
	WideCharToMultiByte (CP_ACP, 0, wszGBK, -1, _Output, len, NULL,NULL); 

	delete[] wszGBK;
	return len + 1;
}
#endif

// 获取当前时间字符串
VOID TimeString(CHAR* _Output)
{
	time_t rawtime;
	struct tm* timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	sprintf(_Output, "%4d-%02d-%02d %02d:%02d:%02d", 1900 + timeinfo->tm_year, 1 + timeinfo->tm_mon, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

// 写入文件
INT32 WriteLog(TCHAR* _Data)
{
	CStdioFile sfFile;
	if(sfFile.Open(_T("Robot.log"), CFile::modeWrite|CFile::modeNoTruncate|CFile::typeText) == FALSE)
	{
		return -1;
	}
	sfFile.SeekToEnd();
	sfFile.WriteString(_Data);
	sfFile.Close();
	return 0;
}

