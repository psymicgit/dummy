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
	int BuildNetHeader(NetMsgHead *msgHead, int msgId, int msgSize)
	{
		int packetSize = msgSize + sizeof(NetMsgHead);

		msgHead->msgSize = endiantool::HostToNetwork32((uint32)packetSize);
		msgHead->msgId = endiantool::HostToNetwork16((uint16)msgId);

		return packetSize;
	}
}
