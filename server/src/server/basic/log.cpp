///<------------------------------------------------------------------------------
//< @file:   server\basic\log.cpp
//< @author: 洪坤安
//< @date:   2014年11月25日 16:45:18
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#include "log.h"


//将信息输出到单独的文件和 LOG(ERROR)
void SignalHandle(const char* data, int size)
{
	std::string str(data, size);
	LOG_ERROR << str;

	// std::ofstream fs("glog_dump.log", std::ios::app);
	// fs << str;
	// fs.close();
}

namespace logging
{
	void init(const char* program, const char* log_path)
	{
#ifdef DEBUG
		google::SetStderrLogging(google::GLOG_INFO); //设置级别高于 google::INFO 的日志同时输出到屏幕
#else
		google::SetStderrLogging(google::GLOG_INFO);//设置级别高于 google::FATAL 的日志同时输出到屏幕
#endif

		google::InitGoogleLogging(program); // 初始化

		google::SetLogDestination(google::GLOG_INFO, log_path);//大于INFO级别的，记录在当前目录，文件以为log_前缀

		/*
		google::SetLogDestination(google::GLOG_FATAL, "./log/log_fatal_"); // 设置 google::FATAL 级别的日志存储路径和文件名前缀
		google::SetLogDestination(google::GLOG_ERROR, "./log/log_error_"); //设置 google::ERROR 级别的日志存储路径和文件名前缀
		google::SetLogDestination(google::GLOG_WARNING, "./log/log_warning_"); //设置 google::WARNING 级别的日志存储路径和文件名前缀
		google::SetLogDestination(google::GLOG_INFO, "./log/log_info_"); //设置 google::INFO 级别的日志存储路径和文件名前缀
		*/

		FLAGS_colorlogtostderr			= true; //设置输出到屏幕的日志显示相应颜色
		FLAGS_logbufsecs				= 0; //缓冲日志输出，默认为30秒，此处改为立即输出
		FLAGS_max_log_size				= 100; //最大日志大小为 100MB
		FLAGS_stop_logging_if_full_disk = true; //当磁盘被写满时，停止日志输出

#ifndef WIN
		// 默认捕捉 SIGSEGV 信号信息输出会输出到 stderr，可以通过下面的方法自定义输出方式：
		google::InstallFailureSignalHandler();
		google::InstallFailureWriter(&SignalHandle);
#endif
	}

	void uninit()
	{
		// google::ShutdownGoogleLogging();
	}
}