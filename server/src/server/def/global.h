///<------------------------------------------------------------------------------
//< @file:   server\def\global.h
//< @author: 洪坤安
//< @date:   2015年4月7日 20:32:19
//< @brief:	 维护全局定义的变量或者全局申请的内存空间
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _global_h_
#define _global_h_

#include "basic/objectpool.h"
#include "basic/buffer.h"

namespace global
{
// 网络
	// 预先申请的消息包内存：用于构造接收到的消息包
	extern char* g_recvPacketBuf;
	extern uint32 g_recvPacketBufSize;

	// 预先申请的消息包内存: 用于构造返回的消息包
	extern char* g_packetBuf;
	extern uint32 g_packetBufSize;
	extern Message *g_lastMessage;

// 数据库
	extern char g_sql[2048];

// 时间
	extern char g_formatBuf[32];

// 打印
	extern char g_sprintfBuf[4096];

	// 全局初始化
	void init();

	// 全局回收资源
	void uninit();
}

#endif //_define_h_