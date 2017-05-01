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

// namespace google {namespace protobuf { class Message; }}

// ������Ϣͷ
struct NetMsgHead {
	uint16 msgId;
	uint32 msgLen;
};

// ������Ϣͷ
struct LanMsgHead {
	uint32 clientId;
	uint16 msgId;
	uint32 msgLen;
};

namespace msgtool
{
	// ��ȡmsg����ϸ����
	string getMsgDebugString(const Message &msg);

	// ���������ͷ
	int BuildNetHeader(NetMsgHead *msgHead, uint16 msgId, uint32 msgLen);

	// ����������Ϣ��ͷ
	int buildLanMsgHeader(LanMsgHead *msgHead, uint32 clientId, uint16 msgId, uint32 msgLen);

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
}

#endif //_message_h_