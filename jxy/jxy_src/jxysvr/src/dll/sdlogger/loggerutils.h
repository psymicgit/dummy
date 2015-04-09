// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#ifndef __SDLOGGER_UTILS_H__
#define __SDLOGGER_UTILS_H__

#if (defined(WIN32) || defined(WIN64))
	#include <errno.h>
	#include <stdio.h>
	#include <cassert>
#else
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <string.h>
	#include <errno.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <assert.h>
	
	#include <linux/limits.h>
	#ifndef MAX_PATH
		#define MAX_PATH	PATH_MAX
	#endif

	#define _vsnprintf	vsnprintf
	#define _snprintf	snprintf
#endif

#include "sdtype.h"


const UINT16 FILE_NAME_LEN = 1024;
const UINT16 RECV_BUFSIZE  = 1024;
const UINT16 SEND_BUFSIZE = 8192;
//
// Log Opt
//
extern INT32	LOGTHRD_BUF_SIZE;
extern UINT32	LOGTHRD_SLEEP_TIME;



#endif

