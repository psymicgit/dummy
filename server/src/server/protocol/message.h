///<------------------------------------------------------------------------------
//< @file:   server\protocol\message.h
//< @author: ������
//< @date:   2015��3��8�� 15:53:38
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _message_h_
#define _message_h_

#include <google/protobuf/message.h>
#include "net/link.h"
#include "net/msgdispatcher.h"

// namespace google {namespace protobuf { class Message; }}

// ������Ϣͷ
struct NetMsgHead {
	int msgSize;
	int16 msgId;
};

namespace msgtool
{
	// ��ȡmsg����ϸ����
	string getMsgDebugString(const Message &msg);

	// ���������ͷ
	int BuildNetHeader(NetMsgHead *msgHead, int msgId, int msgLen);

	// ��Ԥ�ȷ���õĽ�����Ϣ���ڴ�������һ��Message
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


	// ��Ԥ�ȷ���õ��ڴ�������һ��Message
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
		// 1. �����ջ�����������ȫ��ȡ��
		evbuffer recvSwapBuf;
		evbuffer *dst = &recvSwapBuf;

		link->beginRead(dst);

		while (true) {
			// ����ͷ����
			size_t bytes = evbuffer_get_length(dst);
			if (bytes < sizeof(NetMsgHead)) {
				break;
			}

			NetMsgHead *head = (NetMsgHead *)evbuffer_pullup(dst, sizeof(NetMsgHead));
			uint16 msgId = endiantool::NetworkToHost16(head->msgId);
			uint32 rawMsgSize = endiantool::NetworkToHost32(head->msgSize);

			// �����
			if (rawMsgSize > bytes) {
				break;
			}

			char *peek = (char*)evbuffer_pullup(dst, rawMsgSize);

			char *msg = peek + sizeof(NetMsgHead);
			int msgSize = rawMsgSize - sizeof(NetMsgHead);

			dispatcher.dispatch(t, msgId, msg, msgSize, 0);
			evbuffer_drain(dst, rawMsgSize);
		}

		// 3. ������Ϻ����в������Ϣ�壬�򽫲�����Ϣ�����¿��������ջ�������ͷ���Ա�����ȷ������˳��
		link->endRead(dst);
	}
}

#endif //_message_h_