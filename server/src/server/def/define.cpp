///<------------------------------------------------------------------------------
//< @file:   server\def\define.cpp
//< @author: 洪坤安
//< @date:   2015年4月7日 20:34:48
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "define.h"

#include <google/protobuf/message.h>

#include "tool/randtool.h"
#include "protocol/message.h"

namespace global
{
	// 网络
	// 预先申请的消息包内存：用于构造接收到的消息包
	char* g_recvPacketBuf = new char[MAX_PACKET_LEN];
	uint32 g_recvPacketBufSize = MAX_PACKET_LEN;

	// 预先申请的消息包内存: 用于构造返回的消息包
	char* g_packetBuf = new char[MAX_PACKET_LEN];
	uint32 g_packetBufSize = MAX_PACKET_LEN;
	Message *g_lastMessage = NULL;

	// 预先申请的加密缓冲区: 用于发送和接收数据时进行加解密运算
	thread_local char g_encryptBuf[MAX_PACKET_LEN] = {0};

	//
	thread_local char g_recvBuf[MAX_PACKET_LEN] = {0};
	thread_local char g_sendBuf[MAX_PACKET_LEN] = {0};

	// 数据库
	char g_sql[2048] = {0};

	// 时间
	char g_formatBuf[32] = {0};

	// 打印
	char g_sprintfBuf[4096] = {0};

	// 全局缓冲区区
	// ObjectPool<Buffer, mutex_t> g_bufferPool;
}

namespace global
{
	bool g_inited = false;

	void init()
	{
		if (g_inited) {
			return;
		}

		g_inited = true;
		// g_bufferPool.init(1000, 500);

		randtool::initSeed();
	}

	void uninit()
	{
		delete[] g_recvPacketBuf;

		msgtool::freePacket(global::g_lastMessage);
		delete[] g_packetBuf;

		google::protobuf::ShutdownProtobufLibrary();
		logging::uninit();
		// g_bufferPool.clear();
	}
}