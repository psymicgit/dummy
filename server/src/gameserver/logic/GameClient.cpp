///<------------------------------------------------------------------------------
//< @file:   gameserver\logic\client.cpp
//< @author: 洪坤安
//< @date:   2015年6月10日 22:36:23
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "GameClient.h"
#include "gameserver.h"
#include <game_to_gate.pb.h>

bool GameClient::Init()
{
	return true;
}

bool GameClient::SendMsg(int msgId, Message &msg)
{
	int size = msg.ByteSize();

	std::string buf;
	buf.resize(size);

	// 将消息包序列化
	bool ok = msg.SerializeToArray((void*)buf.c_str(), size);
	if (!ok) {
		return false;
	}

	return Send(msgId, buf.c_str(), size);
}

// 发送消息包
bool GameClient::Send(int msgId, const char* msg, int msgSize)
{
	RouteToClientMsg routeMsg;
	routeMsg.set_client_id(m_clientId);
	routeMsg.set_msg_id(msgId);
	routeMsg.set_msg(msg, msgSize);

	GameServer::Instance().SendToGate(GameToGate_RouteToClient, routeMsg);
	return true;
}