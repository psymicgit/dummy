///<------------------------------------------------------------------------------
//< @file:   server\log\log.h
//< @author: 洪坤安
//< @date:   2014年11月25日 15:10:13
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _log_h_
#define _log_h_

#define GLOG_NO_ABBREVIATED_SEVERITIES
#define GOOGLE_GLOG_DLL_DECL

#ifdef WIN
	#pragma warning( disable : 4355 )
#endif

#define  __STDC_FORMAT_MACROS
#include <glog/logging.h>

#define LOG_TRACE LOG(INFO) << "<" _FUNC_ " > "
#define LOG_INFO LOG(INFO) << "<" _FUNC_ "> "
#define LOG_DEBUG LOG(INFO) << "<" _FUNC_ "> "
#define LOG_WARN LOG(WARNING) << "<" _FUNC_ "> "
#define LOG_ERROR LOG(ERROR) << "<" _FUNC_ "> "
#define LOG_FATAL LOG(FATAL) << "<" _FUNC_ "> "

#ifdef WIN
	const char* strerror_threadlocal(int savedErrno);

	#define LOG_SOCKET_ERR LOG_ERROR << "[[err=<" << WSAGetLastError() << "> errmsg=<" << strerror_threadlocal(WSAGetLastError()) << ">]]:"
#else
	#define LOG_SOCKET_ERR LOG_ERROR << "[[socket err = <" << errno << ">--errno = <" << strerror(errno) << ">]]"
#endif

namespace logging
{
	void init(const char* program, const char* log_path);
}

#endif //_log_h_