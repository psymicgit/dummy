///<------------------------------------------------------------------------------
//< @file:   gameserver\logic\client.h
//< @author: 洪坤安
//< @date:   2015年6月10日 22:36:13
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _client_h_
#define _client_h_

// 客户端，对应于网关的一个玩家连接
class GameClient
{
public:
	bool Init();

	// 发送消息包
	bool SendMsg(int msgId, Message&);

	// 发送消息包
	bool Send(int msgId, const char* msg, int msgSize);

public:
	int m_clientId;
};

#endif //_client_h_