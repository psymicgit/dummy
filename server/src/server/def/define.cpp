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
	bool g_inited = false;

	void init()
	{
		if (g_inited) {
			return;
		}

		g_inited = true;
		g_bufferPool.init(1000, 500);

		randtool::initSeed();
	}

	void uninit()
	{
		msgtool::freePacket(global::g_lastMessage);
		delete[] g_packetBuf;

		google::protobuf::ShutdownProtobufLibrary();
	}
}