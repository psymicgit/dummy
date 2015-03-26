///<------------------------------------------------------------------------------
//< @file:   server\log\log.cpp
//< @author: 洪坤安
//< @date:   2014年11月25日 16:45:18
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#include "log.h"

thread_local char t_errnobuf[512];

const char* strerror_threadlocal(int savedErrno)
{
	strerror_s(t_errnobuf, sizeof t_errnobuf, savedErrno);
	return t_errnobuf;
}

//将信息输出到单独的文件和 LOG(ERROR)
void SignalHandle(const char* data, int size)
{
	std::string str = std::string(data, size);
	LOG_ERROR << str;

	// std::ofstream fs("glog_dump.log", std::ios::app);
	// fs << str;
	// fs.close();
}

namespace logging
{
	void init(const char* program, const char* log_path)
	{
		google::InitGoogleLogging(program); // 初始化
		google::SetLogDestination(google::GLOG_INFO, log_path);//大于INFO级别的，记录在当前目录，文件以为log_前缀
		google::SetStderrLogging(google::GLOG_INFO); //配置输出到标准错误输出的最低日记级别

		FLAGS_colorlogtostderr = true;

// 		google::InstallFailureSignalHandler();
// 		// 默认捕捉 SIGSEGV 信号信息输出会输出到 stderr，可以通过下面的方法自定义输出方式：
// 		google::InstallFailureWriter(&SignalHandle);
	}
}