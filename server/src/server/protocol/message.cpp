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
	string getMsgString(const google::protobuf::Message &msg)
	{
		string text = msg.GetDescriptor()->DebugString();
		text += "\nvalue = {\n" + msg.DebugString() + "}";

		return text;
	}

	// 构建网络包头
	int buildNetHeader(NetMsgHead *pstHead, uint16 msgId, uint32 msgLen)
	{
		uint32 packetLen = msgLen + sizeof(NetMsgHead);

		pstHead->msgId	= endiantool::hostToNetwork16(msgId);
		pstHead->msgLen	= endiantool::hostToNetwork32(packetLen);

		return packetLen;
	}
}
