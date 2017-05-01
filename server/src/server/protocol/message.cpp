///<------------------------------------------------------------------------------
//< @file:   server\protocol\message.cpp
//< @author: ������
//< @date:   2015��3��9�� 12:22:32
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#include "message.h"

#include <google/protobuf/message.h>
#include "tool/endiantool.h"

namespace msgtool
{
	string getMsgDebugString(const Message &msg)
	{
		string text = msg.GetDescriptor()->DebugString();
		text += "\nvalue = {\n" + msg.DebugString() + "}";

		return text;
	}

	// ���������ͷ
	int BuildNetHeader(NetMsgHead *msgHead, uint16 msgId, uint32 msgLen)
	{
		uint32 packetLen = msgLen + sizeof(NetMsgHead);

		msgHead->msgId	= endiantool::hostToNetwork(msgId);
		msgHead->msgLen	= endiantool::hostToNetwork(packetLen);

		return packetLen;
	}

	int BuildLanMsgHeader(LanMsgHead *msgHead, uint32 clientId, uint16 msgId, uint32 msgLen)
	{
		uint32 packetLen = msgLen + sizeof(LanMsgHead);

		msgHead->clientId = endiantool::hostToNetwork(clientId);
		msgHead->msgId	  = endiantool::hostToNetwork(msgId);
		msgHead->msgLen	  = endiantool::hostToNetwork(packetLen);

		return packetLen;
	}
}
