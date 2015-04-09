#ifndef _SDDB_CONFIG_H_
#define _SDDB_CONFIG_H_

// Windows 头文件:
#if ( defined(WIN32) || defined(WIN64) )
	#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
	//#include <windows.h>
	//#include <WinSock2.h>
	//#include <windows.h>
#else
	#include <string.h>
	#define _vsnprintf	vsnprintf
	#define _snprintf	snprintf
#endif

#include <assert.h>
#include "sddb.h"
#include "sdbase.h"
#include "sdlogger.h"
#include "sdstring.h"
#include "config.h"
#include "sdtype.h"
extern UINT32	g_dwDBLogLevel;
extern SGDP::CSDLogger	g_oDBLogger;

#define CRITICAL	if(g_dwDBLogLevel & LOGLV_CRITICAL)	g_oDBLogger.Critical
#define WARN		if(g_dwDBLogLevel & LOGLV_WARN)		g_oDBLogger.Warn
#define INFO		if(g_dwDBLogLevel & LOGLV_INFO)		g_oDBLogger.Info
#define DBG		if(g_dwDBLogLevel & LOGLV_DEBUG)	g_oDBLogger.Debug

#endif

