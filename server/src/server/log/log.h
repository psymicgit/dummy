///<------------------------------------------------------------------------------
//< @file:   server\log\log.h
//< @author: 洪坤安
//< @date:   2014年11月25日 15:10:13
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _log_h_
#define _log_h_

#include "tool/sockettool.h"
#include "tool/echotool.h"

#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL

#ifdef WIN
	#pragma warning( disable : 4355 )
#endif

#define  __STDC_FORMAT_MACROS
#include <glog/logging.h>

#define LOG_OK(...) echotool::changeToColor(COLOR_GREEN); LOG(INFO) << "" _FUNC_ "() " << __VA_ARGS__; echotool::changeToOldColor();
#define LOG_TRACE LOG(INFO) << "" _FUNC_ "() "
#define LOG_INFO LOG(INFO) << "" _FUNC_ "() "
#define LOG_DEBUG LOG(INFO) << "" _FUNC_ "() "
#define LOG_WARN LOG(WARNING) << "" _FUNC_ "() "
#define LOG_ERROR LOG(ERROR) << "" _FUNC_ "() "
#define LOG_FATAL LOG(FATAL) << "" _FUNC_ "() "

#define LOG_SYSTEM_ERR LOG_ERROR << "[[socket --err=<" << socktool::geterrno() << "> --errmsg=<" << socktool::getErrorMsg(socktool::geterrno()) << ">]]"

#define LOG_SOCKET_ERR(sockfd, errnum) LOG_ERROR << "[[socket<" << sockfd << "> --err=<" << errnum << "> --errmsg=<" << socktool::getErrorMsg(errnum) << ">]]"

namespace logging
{
	void init(const char* program, const char* log_path);
}

#endif //_log_h_