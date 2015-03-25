/********************************************************************
            Copyright (c) 2010, 欢乐连线工作室
                   All rights reserved
         
    创建日期:	2010年1月14日 13时49分
    文件名称:	TimeTools.h
    说    明:	时间类型工具

    当前版本:	1.00
    作    者:	wy
				czq 
    概    述:	

*********************************************************************/
#pragma once
#include <time.h>
#include <stdint.h>

typedef uint64_t time64_t;

class TimeTools
{
public:
	TimeTools();
	~TimeTools();
	// 初始化时区偏差
	static void init(int deviationZone);

	// 获得当前时间,格林威治时间
	static time64_t getCurrentTime();
	// 获得当前时间+偏移的时间，运营时间
	static time64_t getOperatingTime();

	// 获得当前时间, 运营时区
	static const tm* getCurrentTM();
	// 获得tm
	static const tm* getTMFromTime_t(time64_t tt);
	// 获取运营tm
	static const tm* getOperatingTMFromTime_t(time64_t tt);

	// 时间字符串转换成time64_t格式(字符串格式：2010-01-01 01:01:01)
	static time64_t conversionStrTimeToTime_t(const char* pStr);
	// 时间字符串转换成time64_t格式(字符串格式：2010,01,01,01,01,01)
	static time64_t conversionStrTimeToTime_tEx(const char* pStr);

	// 时间字符串转换成秒时间(字符串格式：2010,01,01,01,01,01)
	static time64_t conversionStrTimeToSecond(const char* pStr);

	// 将时间转换成字符串(字符串格式：2010-01-01 01:01:01)
	static const char* conversionTime_tToStr(time64_t t);
	// 将时间转换成字符串(字符串格式：2010,01,01,01,01,01)
	static const char* conversionTime_tToStrEx(time64_t t);

	// 获得时区
	static long getTimeZone();
	// 本地标准时间, 运营时区
	//static time64_t getLocalTime();

	// 时间格式 00:00:00 或者 00:00 返回秒
	static long	StrTimeToNum(const char* pszTime);
	// 格式%H:%M:%S
	static char *LocalTimeToStr(bool bSec = false);
	// 2010-01-01 01:01:01
	static char *getLocalDateTimeStr();
	// 2010-01-01 01:01:01
	static char *getDateTimeStr(time64_t tt);
	// 获得调整的时区
	static int getDeviationZone();
	// 获取时间字符串格式(序列形式) 20100102030405
	static char *getLocalDateTimeStrSeq();

	// 获取上一次星期一零点零分零秒的时间
	static time64_t getLastMondayTime(time64_t t);
	// 获取上一次星期一零点零分零秒的时间
	static time64_t getLastMondayTime(time64_t t, const tm* pTm);
	// 获取今天00:00:00的时间
	static time64_t getTodayBeginTime(time64_t t);

private:
	// 时区偏差
	static int s_deviationZone;
	// 时区偏差 * 3600
	static long long s_deviationTime;
};
