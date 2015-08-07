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

#define MAX_PACKET_LEN (1024 * 1024) // 1MB

namespace global
{
// 网络
	// 预先申请的消息包内存：用于构造接收到的消息包
	static char* g_recvPacketBuf = new char[MAX_PACKET_LEN];
	static uint32 g_recvPacketBufSize = MAX_PACKET_LEN;

	// 预先申请的消息包内存: 用于构造返回的消息包
	static char* g_packetBuf = new char[MAX_PACKET_LEN];
	static uint32 g_packetBufSize = MAX_PACKET_LEN;
	static Message *g_lastMessage = NULL;

	// 预先申请的网络数据缓冲区: 用于发送和接收数据时进行加解密运算
	static char g_netBuf[MAX_PACKET_LEN] = {0};

	//
	static thread_local char g_recvBuf[80 * 1024] = {0};

// 数据库
	static thread_local char g_sql[2048] = {0};

// 时间
	static char g_formatBuf[32] = {0};

// 打印
	static thread_local char g_sprintfBuf[4096] = {0};

// 全局缓冲区区
	static ObjectPool<Buffer, mutex_t> g_bufferPool;

	void init();
	void uninit();
}

#endif //_define_h_