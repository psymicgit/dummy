///<------------------------------------------------------------------------------
//< @file:   server\def\define.h
//< @author: 洪坤安
//< @date:   2015年4月7日 20:32:19
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _define_h_
#define _define_h_

#include "basic/objectpool.h"
#include "basic/buffer.h"

#define MAX_PACKET_LEN (1024 * 1024)

namespace global
{
// 网络消息定义
	// 预先申请的消息包内存
	static char* g_packetBuf = new char[MAX_PACKET_LEN];
	static uint32 g_packetBufSize = MAX_PACKET_LEN;
	static Message *g_lastMessage = NULL;

// 数据库
	static thread_local char g_sql[2048] = {0};
	static thread_local char g_recvBuf[80960] = {0};

// 时间
	static char g_formatBuf[32] = {0};

// 打印
	static thread_local char g_sprintfBuf[4096] = {0};

// 全局缓冲区区
	static ObjectPool<Buffer> g_bufferPool;

	void init();
	void uninit();
}

#endif //_define_h_