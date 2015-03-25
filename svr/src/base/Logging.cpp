#include "Logging.h"

#include <string.h>

__declspec(thread) char t_errnobuf[512];

const char* strerror_tl(int savedErrno)
{
    strerror_s(t_errnobuf, sizeof t_errnobuf, savedErrno);
    return t_errnobuf;
}

namespace logging
{
    void Init(const char* program_name, const char* log_path)
    {
        google::InitGoogleLogging(program_name); // 初始化
        google::SetLogDestination(google::GLOG_INFO, log_path);//大于INFO级别的，记录在当前目录，文件以为log_前缀
        google::SetStderrLogging(google::GLOG_INFO); //配置输出到标准错误输出的最低日记级别
    }
}