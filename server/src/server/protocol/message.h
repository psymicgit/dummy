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
#include "net/link.h"
#include "net/msgdispatcher.h"

// namespace google {namespace protobuf { class Message; }}

// 外网消息头
struct NetMsgHead {
	int msgSize;
	int16 msgId;
};

namespace msgtool
{
	// 获取msg的详细内容
	string getMsgDebugString(const Message &msg);

	// 构建网络包头
	int BuildNetHeader(NetMsgHead *msgHead, int msgId, int msgLen);

	// 在预先分配好的接收消息包内存上申请一个Message
	template<typename T>
	T* allocRecvPacket()
	{
		if (global::g_recvPacketBufSize < sizeof(T)) {
			LOG_WARN << "global::g_recvPacketBufSize old = " << global::g_recvPacketBufSize << ", new = " << sizeof(T);

			delete[] global::g_recvPacketBuf;
			global::g_recvPacketBuf		= new char[sizeof(T)];
			global::g_recvPacketBufSize = sizeof(T);
		}

		T* t = new ((T*)global::g_recvPacketBuf)T();
		return t;
	}


	// 在预先分配好的内存上申请一个Message
	template<typename T>
	T* allocPacket()
	{
		if (global::g_lastMessage) {
			global::g_lastMessage->~Message();
		}

		if (global::g_packetBufSize < sizeof(T)) {
			LOG_WARN << "global::g_packetBufSize old = " << global::g_packetBufSize << ", new = " << sizeof(T);

			delete[] global::g_packetBuf;
			global::g_packetBuf		= new char[sizeof(T)];
			global::g_packetBufSize = sizeof(T);
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
	}

	template <typename T>
	void DispatchMsg(Link* link, T &t, MsgDispatcher<T>& dispatcher)
	{
		// 1. 将接收缓冲区的数据全部取出
		evbuffer recvSwapBuf;
		evbuffer *dst = &recvSwapBuf;

		link->beginRead(dst);

		while (true) {
			// 检测包头长度
			size_t bytes = evbuffer_get_length(dst);
			if (bytes < sizeof(NetMsgHead)) {
				break;
			}

			NetMsgHead *head = (NetMsgHead *)evbuffer_pullup(dst, sizeof(NetMsgHead));
			uint16 msgId = endiantool::NetworkToHost16(head->msgId);
			uint32 rawMsgSize = endiantool::NetworkToHost32(head->msgSize);

			// 检测半包
			if (rawMsgSize > bytes) {
				break;
			}

			char *peek = (char*)evbuffer_pullup(dst, rawMsgSize);

			char *msg = peek + sizeof(NetMsgHead);
			int msgSize = rawMsgSize - sizeof(NetMsgHead);

			dispatcher.dispatch(t, msgId, msg, msgSize, 0);
			evbuffer_drain(dst, rawMsgSize);
		}

		// 3. 处理完毕后，若有残余的消息体，则将残余消息体重新拷贝到接收缓冲区的头部以保持正确的数据顺序
		link->endRead(dst);
	}
}

#endif //_message_h_