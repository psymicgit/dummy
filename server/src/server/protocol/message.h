///<------------------------------------------------------------------------------
//< @file:   server\protocol\message.h
//< @author: 洪坤安
//< @date:   2015年3月8日 15:53:38
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _message_h_
#define _message_h_

namespace google {namespace protobuf { class Message; }}

struct NetMsgHead {
	int msgId;
	int msgLen;
};

namespace msgtool
{
	string getMsgString(google::protobuf::Message &msg);

	// 构建网络包头
	int buildNetHeader(NetMsgHead *pstHead, uint16 msgId, uint32 msgLen);
}

#endif //_message_h_