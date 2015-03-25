#ifndef Logging_h__
#define Logging_h__

#include "LogStream.h"
#include "Timestamp.h"

#include <glog/logging.h>

const char* strerror_tl(int savedErrno);

#define oops(msg) { perror("oops: "msg); }

#define LOG_TRACE LOG(INFO) 
#define LOG_INFO LOG(INFO) 
#define LOG_DEBUG LOG(INFO)
#define LOG_WARN LOG(WARNING)
#define LOG_ERROR LOG(ERROR)
#define LOG_SYSERR LOG(FATAL)
#define LOG_FATAL LOG(FATAL)
#define LOG_SYSFATAL LOG(FATAL)

#define LOG_SOCKET_ERR LOG_ERROR << "socket err = <" << WSAGetLastError() << ">--errno = <" << strerror_tl(errno) << ">: "
#define LOG_SOCKET_FATAL LOG_FATAL << "socket err = <" << WSAGetLastError() << ">--errno = <" << strerror_tl(errno) << ">: "

namespace logging
{
    void Init(const char* program_name, const char* log_path);
}

#endif // Logging_h__
