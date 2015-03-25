#include "timetools.h"
#include <string>
#include <vector>
using std::string;
using std::vector;

long long TimeTools::s_deviationTime = 0;
int TimeTools::s_deviationZone = 0;

TimeTools::TimeTools() {
#if defined WIN32
	_tzset();
#elif defined __LINUX__
	tzset();
#endif
}

TimeTools::~TimeTools() {

}

time64_t TimeTools::getCurrentTime() {
	return time64_t((unsigned int)time(NULL));
}

const tm* TimeTools::getCurrentTM() {
	long long ct = (long long)getCurrentTime();
	time64_t tt = ct + s_deviationTime;
	time_t tt_original = (unsigned int)tt;
	return localtime(&tt_original);
}

time64_t TimeTools::conversionStrTimeToTime_t(const char* pStr) {
	if (NULL == pStr) {
		return -1;
	}
	
	// 验证
	string str = pStr;
	if (-1 != str.find_first_not_of("0123456789-: ")) {
		return -1;
	}
	
	int count = 0;
	string::size_type pos;
	while (-1 != (pos = str.find_first_of("-: "))) {
		++count;
		if (count >= 1 && count <= 2) {
			if ('-' != str[pos]) {
				return -1;
			}
		}
		else if (3 == count) {
			if (' ' != str[pos]) {
				return -1;
			}
		}
		else if (count >= 4) {
			if (':' != str[pos]) {
				return -1;
			}
		}
		str.erase(0, pos + 1);
	}

	if (count != 5) {
		return -1;
	}

	// 转换
	struct tm t;
	sscanf(pStr,"%d-%d-%d %d:%d:%d",&t.tm_year,&t.tm_mon,&t.tm_mday,&t.tm_hour,&t.tm_min,&t.tm_sec);
	t.tm_year -= 1900;
	t.tm_mon -= 1;
	long long result = mktime(&t);
	return result;
}

time64_t TimeTools::conversionStrTimeToTime_tEx(const char* pStr) {
	string str = pStr;
	
	// 验证
	if (-1 != str.find_first_not_of("0123456789,")) {
		return -1;
	}

	// 转换成 2010-01-01 01:01:01 格式
	int count = 0;
	string::size_type pos;
	while (-1 != (pos = str.find(',')))
	{
		++count;
		if (count >= 1 && count <= 2) {
			str.replace(pos, 1, "-");
		}
		else if (3 == count) {
			str.replace(pos, 1, " ");
		}
		else if (count >= 4) {
			str.replace(pos, 1, ":");
		}
	}
	if (count > 5) {
		return -1;
	}

	return TimeTools::conversionStrTimeToTime_t(str.c_str());
}

time64_t TimeTools::conversionStrTimeToSecond(const char* pStr) {
	string str = pStr;

	// 验证
	if (-1 != str.find_first_not_of("0123456789,")) {
		return -1;
	}

	string::size_type pos;
	string tmp;
	vector<int> vTime;
	while (!str.empty())
	{
		pos = str.find(',');
		if (-1 != pos) {
			tmp = str.substr(0, pos);
			str.erase(0, pos+1);
		} else {
			tmp = str;
			str.clear();
		}
		vTime.push_back(atoi(tmp.c_str()));
	}

	time64_t second = -1;

	if (6 == vTime.size()) {
		second = 0;
		for (int i = 5; i >= 0; --i) {
			switch (i)
			{
			case 0:
				{
					second += vTime[0] * 60 * 60 * 24 * 30 * 365;
					break;
				}
			case 1:
				{
					second += vTime[1] * 60 * 60 * 24 * 30;
					break;
				}
			case 2:
				{
					second += vTime[2] * 60 * 60 * 24;
					break;
				}
			case 3:
				{
					second += vTime[3] * 60 * 60;
					break;
				}
			case 4:
				{
					second += vTime[4] * 60;
					break;
				}
			case 5:
				{
					second += vTime[5];
					break;
				}
			default:
				{
//					assert(0);
					second = -1;
					break;
				}
			}
		}
	} 

	return second;
}

const char* TimeTools::conversionTime_tToStr(time64_t t) {
	static char buf[500];

	const tm *pTm = getTMFromTime_t(t);
	memset(buf, 0, sizeof(buf));
	sprintf_s(buf, sizeof(buf), "%d-%d-%d %d:%d:%d", pTm->tm_year + 1900, pTm->tm_mon + 1, 
		pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);

	return buf;
}

const char* TimeTools::conversionTime_tToStrEx(time64_t t) {
	static char buf[500];

	const tm *pTm = getTMFromTime_t(t);
	memset(buf, 0, sizeof(buf));
	sprintf_s(buf, sizeof(buf), "%d,%d,%d,%d,%d,%d", pTm->tm_year + 1900, pTm->tm_mon + 1, 
		pTm->tm_mday, pTm->tm_hour, pTm->tm_min, pTm->tm_sec);

	return buf;
}

long TimeTools::getTimeZone() {
#if defined WIN32
	long timeZone = 0;
	_get_timezone(&timeZone);
	return timeZone/-3600;
#elif defined __LINUX__
	return timezone/-3600;
#endif 
}

long TimeTools::StrTimeToNum(const char* pszTime) {
	if (NULL == pszTime)
		return 0;

	string strTime = pszTime;
	char szTime[10]  = {0};
	char szHour[4]	 = {0};
	char szMinute[4] = {0};
	char szSec[4]	 = {0};

	strcpy_s(szTime, strTime.c_str());
	size_t pos = strTime.find(":");

	strncpy_s(szHour, szTime, pos);
	size_t pos2 = strTime.rfind(":");

	long lTime = 0;

	if ( pos != pos2 )
	{
		strncpy_s(szMinute, &szTime[pos+1], pos2 - pos - 1 );
		strncpy_s(szSec, &szTime[pos2+1], strTime.size() - pos2 - 1);

		lTime= atoi(szHour)*3600 + atoi(szMinute)*60 + atoi(szSec);
	}
	else
	{
		strncpy_s(szMinute, &szTime[pos+1], strTime.size() - pos - 1);
		lTime= atoi(szHour)*3600 + atoi(szMinute)*60;

	}

	return lTime;
}

char* TimeTools::LocalTimeToStr(bool bSec) {
	static char szTimeFormat[10];

	memset(szTimeFormat, 0, sizeof(szTimeFormat));
	const struct tm *stTm = getCurrentTM();

	if ( bSec ) {
		strftime(szTimeFormat, sizeof(szTimeFormat), "%H:%M:%S", stTm);
	} else {
		strftime(szTimeFormat, sizeof(szTimeFormat), "%H:%M", stTm);
	}

	return szTimeFormat;
}
// 
// time64_t TimeTools::getLocalTime() {
// 	return getCurrentTime() + 3600 * getTimeZone() + s_deviationTime;
// }

void TimeTools::init(int deviationZone) {
#if defined WIN32
	_tzset();
#elif defined __LINUX__
	tzset();
#endif
	s_deviationTime = deviationZone * 3600;
	s_deviationZone = deviationZone;
}

char * TimeTools::getLocalDateTimeStr() {
	static char szTimeFormat[32];
	memset(szTimeFormat, 0, sizeof(szTimeFormat));

	strftime(szTimeFormat, sizeof(szTimeFormat), "%Y-%m-%d %H:%M:%S", getCurrentTM());

	return szTimeFormat;
}

int TimeTools::getDeviationZone() {
	return s_deviationZone;
}

char * TimeTools::getLocalDateTimeStrSeq() {
	static char szTimeFormat[32];
	memset(szTimeFormat, 0, sizeof(szTimeFormat));

	strftime(szTimeFormat, sizeof(szTimeFormat), "%Y%m%d%H%M%S", getCurrentTM());

	return szTimeFormat;
}

time64_t TimeTools::getLastMondayTime(time64_t t) {
	const tm* pTm = getOperatingTMFromTime_t(t);

	int wday = 0;
	if (0 == pTm->tm_wday) {
		wday = 6 * 24 * 60 * 60;
	} else if (1 != pTm->tm_wday) {
		wday = (pTm->tm_wday - 1) * 24 * 60 * 60;
	}
	int hour = pTm->tm_hour * 60 * 60;
	int min = pTm->tm_min * 60;
	int sec = pTm->tm_sec;

	int all = wday + hour + min + sec;

	return (long long)t - all;
}

time64_t TimeTools::getLastMondayTime(time64_t t, const tm* pTm) {
	if (NULL == pTm) {
		return 0;
	}

	int wday = 0;
	if (0 == pTm->tm_wday) {
		wday = 6 * 24 * 60 * 60;
	} else if (1 != pTm->tm_wday) {
		wday = (pTm->tm_wday - 1) * 24 * 60 * 60;
	}
	int hour = pTm->tm_hour * 60 * 60;
	int min = pTm->tm_min * 60;
	int sec = pTm->tm_sec;

	int all = wday + hour + min + sec;

	return (long long)t - all;
}

time64_t TimeTools::getTodayBeginTime(time64_t t) {
	const tm* pTm = getOperatingTMFromTime_t(t);

	int hour = pTm->tm_hour * 60 * 60;
	int min = pTm->tm_min * 60;
	int sec = pTm->tm_sec;

	int all = hour + min + sec;

	return (long long)t - all;
}

char * TimeTools::getDateTimeStr(time64_t tt) {
	static char szTimeFormat[32];
	memset(szTimeFormat, 0, sizeof(szTimeFormat));
	strftime(szTimeFormat, sizeof(szTimeFormat), "%Y-%m-%d %H:%M:%S", getOperatingTMFromTime_t(tt));

	return szTimeFormat;
}

const tm* TimeTools::getTMFromTime_t(time64_t tt) {
	time_t tt_original = (time_t)(long long)tt;
	return localtime(&tt_original);
}

const tm* TimeTools::getOperatingTMFromTime_t(time64_t tt) {
	time64_t optt = (long long)tt + s_deviationTime;
	time_t optt_orginal = (time_t)(long long)optt;
	return localtime(&optt_orginal);
}

time64_t TimeTools::getOperatingTime() {
	return (long long)getCurrentTime() + s_deviationTime;
}
