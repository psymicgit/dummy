///<------------------------------------------------------------------------------
//< @file:   server\protocol\message.cpp
//< @author: 洪坤安
//< @date:   2015年3月9日 12:22:32
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
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

	// 构建网络包头
	int buildNetHeader(NetMsgHead *msgHead, uint16 msgId, uint32 msgLen)
	{
		uint32 packetLen = msgLen + sizeof(NetMsgHead);

		msgHead->msgId	= endiantool::hostToNetwork16(msgId);
		msgHead->msgLen	= endiantool::hostToNetwork32(packetLen);

		return packetLen;
	}

	int buildLanMsgHeader(LanMsgHead *msgHead, uint32 clientId, uint16 msgId, uint32 msgLen)
	{
		uint32 packetLen = msgLen + sizeof(LanMsgHead);

		msgHead->clientId = endiantool::hostToNetwork16(clientId);
		msgHead->msgId	  = endiantool::hostToNetwork16(msgId);
		msgHead->msgLen	  = endiantool::hostToNetwork32(packetLen);

		return packetLen;
	}
}
