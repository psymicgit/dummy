///<------------------------------------------------------------------------------
//< @file:   server\basic\log.h
//< @author: 洪坤安
//< @date:   2014年11月25日 15:10:13
//< @brief:	 日志操作api
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _log_h_
#define _log_h_

#include "tool/sockettool.h"
#include "tool/echotool.h"

#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL
// #define GOOGLE_STRIP_LOG 0

#ifdef WIN
	#pragma warning( disable : 4355 )
#endif

#define  __STDC_FORMAT_MACROS
#include <glog/logging.h>
#include <glog/raw_logging.h>

// 记录成功日志（绿色字体）
#define LOG_OK(...) echotool::changeToColor(COLOR_GREEN); LOG(INFO) << "" _FUNC_ "() " << __VA_ARGS__; echotool::changeToOldColor();

// 记录运行时日志
#define LOG_INFO LOG(INFO) << "" _FUNC_ "() "

// 记录调试日志
#define LOG_DEBUG LOG(INFO) << "" _FUNC_ "() "

// 记录警告（黄色字体）
#define LOG_WARN LOG(WARNING) << "" _FUNC_ "() "

// 记录警告（红色字体）
#define LOG_ERROR LOG(ERROR) << "" _FUNC_ "() "

// 记录致命操作
#define LOG_FATAL LOG(FATAL) << "" _FUNC_ "() "

// 记录系统错误
#define LOG_SYSTEM_ERR LOG_ERROR << "[[socket --err=<" << socktool::geterrno() << "> --errmsg=<" << socktool::getErrorMsg(socktool::geterrno()) << ">]]"

// 记录socket错误
#define LOG_SOCKET_ERR(sockfd, errnum) LOG_ERROR << "[[socket<" << sockfd << "> --err=<" << errnum << "> --errmsg=<" << socktool::getErrorMsg(errnum) << ">]]"

// 更加线程安全的log
#define SAFE_LOG(...) RAW_LOG(WARNING, __VA_ARGS__)

namespace logging
{
	void init(const char* program, const char* log_path);
	void uninit();
}

#endif //_log_h_