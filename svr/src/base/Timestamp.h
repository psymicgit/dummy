#ifndef MUDUO_BASE_TIMESTAMP_H
#define MUDUO_BASE_TIMESTAMP_H

#include <common/Platform.h>


typedef int64 Timestamp; // utc格式时间，sin 1900 in ms毫秒单位
typedef int32 TimeInMs; // in ms毫秒单位
typedef int32 Sec; // 秒

#define MsPerSecond 1000 // 毫秒
#define UsPerSecond (1000 * 1000) // 微秒

namespace timeutil
{
    Timestamp GetTimeOfDay();

    Timestamp GetLocalTime();

    const char* FormatTime(const Timestamp& time);

    const char* FormatNow();

    void EchoNow();
}

#endif  // MUDUO_BASE_TIMESTAMP_H
