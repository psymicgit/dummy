///<------------------------------------------------------------------------------
//< @file:   server\protocol\message.h
//< @author: 洪坤安
//< @date:   2015年3月8日 15:53:38
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _message_h_
#define _message_h_

#include <google/protobuf/message.h>

// namespace google {namespace protobuf { class Message; }}

// 外网消息头
struct NetMsgHead {
	uint16 msgId;
	uint32 msgLen;
};

// 内网消息头
struct LanMsgHead {
	uint32 clientId;
	uint16 msgId;
	uint32 msgLen;
};

namespace msgtool
{
	string getMsgString(const Message &msg);

	// 构建网络包头
	int buildNetHeader(NetMsgHead *msgHead, uint16 msgId, uint32 msgLen);

	// 构建内网消息包头
	int buildLanMsgHeader(LanMsgHead *msgHead, uint32 clientId, uint16 msgId, uint32 msgLen);

	// 在预先分配好的内存上申请一个Message
	template<typename T>
	T* allocPacket()
	{
		if (global::g_packetBufSize < sizeof(T)) {
			delete[] global::g_packetBuf;
			global::g_packetBuf = new char[sizeof(T)];
			global::g_packetBufSize = sizeof(T);
		}

		if (global::g_lastMessage) {
			global::g_lastMessage->~Message();
		}

		T* t = new ((T*)global::g_packetBuf)T();
		global::g_lastMessage = t;
		return t;
	}

	template<typename T>
	inline void freePacket(T *obj)
	{
		if (obj) {
			obj->~T();
		}

		if(global::g_lastMessage == obj) {
			global::g_lastMessage = NULL;
		}
	}
}

#endif //_message_h_