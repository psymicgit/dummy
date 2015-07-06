///<------------------------------------------------------------------------------
//< @file:   server\tool\timetool.cpp
//< @author: 洪坤安
//< @date:   2015年1月16日 22:55:27
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "timetool.h"

#ifndef WIN
	#include <sys/time.h>
#endif

namespace timetool
{
#ifdef WIN
	Timestamp FileTimeToInt64(const FILETIME& time)
	{
		ULARGE_INTEGER tt;
		tt.LowPart = time.dwLowDateTime;
		tt.HighPart = time.dwHighDateTime;
		return(tt.QuadPart);
	}

	FILETIME Int64ToFileTime(const Timestamp& stamp)
	{
		ULARGE_INTEGER tt;
		tt.QuadPart = stamp;

		FILETIME filetime;
		filetime.dwLowDateTime = tt.LowPart;
		filetime.dwHighDateTime = tt.HighPart;

		return filetime;
	}

	Timestamp getTimeOfDay()
	{
		FILETIME now;
		GetSystemTimeAsFileTime (&now); // 获得系统UTC格式时间

		int64 nowIn100ns = FileTimeToInt64(now);// 单位100ns
		return nowIn100ns / 10000; //转化为ms单位
	}

	Timestamp getLocalTime()
	{
		FILETIME local;
		GetSystemTimeAsFileTime (&local); // 获得系统UTC格式时间
		FileTimeToLocalFileTime(&local, &local); // 转换为本地时间

		int64 localIn100ns = FileTimeToInt64(local);// 单位100ns
		return localIn100ns / 10000;
	}

	const char* FormatTime(const Timestamp& time)
	{
		Timestamp utc = time * 10000;

		FILETIME filetime = Int64ToFileTime(utc);

		SYSTEMTIME systime;
		FileTimeToSystemTime(&filetime, &systime);

		sprintf_s(global::g_formatBuf, sizeof global::g_formatBuf, "%02d/%02d/%d-%02d:%02d:%02d:%-6d",
		          systime.wDay, systime.wMonth, systime.wYear,
		          systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds
		         );

		return global::g_formatBuf;
	}

	const char* FormatNow()
	{
		FILETIME local;
		SYSTEMTIME systime;
		GetSystemTimeAsFileTime (&local); // 获得系统UTC格式时间
		FileTimeToLocalFileTime(&local, &local); // 转换为本地时间
		FileTimeToSystemTime(&local, &systime);

		sprintf_s(global::g_formatBuf, sizeof global::g_formatBuf, "%02d/%02d/%02d-%02d:%02d:%02d:%-6d",
		          systime.wYear, systime.wMonth, systime.wDay,
		          systime.wHour, systime.wMinute, systime.wSecond, systime.wMilliseconds
		         );

		return global::g_formatBuf;
	}

#else
	Timestamp getTimeOfDay()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		Timestamp seconds = tv.tv_sec;
		return seconds * 1000 + tv.tv_usec / 1000;
	}

	const char* FormatNow()
	{
		time_t now;
		time(&now);// time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now

		tm *localnow = localtime(&now); // 转为本时区时间

		sprintf_s(global::g_formatBuf, sizeof global::g_formatBuf, "%02d/%02d/%02d-%02d:%02d:%02d",
		          localnow->tm_year, localnow->tm_mon, localnow->tm_mday,
		          localnow->tm_hour, localnow->tm_min, localnow->tm_sec
		         );

		return global::g_formatBuf;
	}
#endif
}