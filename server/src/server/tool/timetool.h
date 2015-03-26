///<------------------------------------------------------------------------------
//< @file:   server\tool\timetool.h
//< @author: 洪坤安
//< @date:   2015年1月16日 22:54:23
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _timetool_h_
#define _timetool_h_

typedef int64 Timestamp; // utc格式时间，sin 1900 in ms毫秒单位
typedef int32 TimeInMs; // in ms毫秒单位

#define MsPerSecond 1000 // 毫秒
#define UsPerSecond (1000 * 1000) // 微秒

namespace timetool
{
	Timestamp GetTimeOfDay();

	Timestamp GetLocalTime();

	const char* FormatTime(const Timestamp& time);

	const char* FormatNow();

	void EchoNow();
}

#endif //_timetool_h_