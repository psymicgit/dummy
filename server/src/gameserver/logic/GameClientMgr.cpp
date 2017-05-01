///<------------------------------------------------------------------------------
//< @file:   gameserver\logic\clientmgr.cpp
//< @author: 洪坤安
//< @date:   2015年6月10日 22:36:32
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "GameClientMgr.h"
#include "GameLogic.h"
#include <client.pb.h>
#include <protocol/message.h>

GameClientMgr::GameClientMgr()
{
	GameLogic::RegisterClientMsg(ClientMsg_LoginRequest, OnLoginReq);
	GameLogic::RegisterClientMsg(ClientMsg_AuthRequest, OnAuthReq);
}

GameClient* GameClientMgr::FindClient(uint32 playerId)
{
	ClientMap::iterator itr = m_clients.find(playerId);
	if (itr == m_clients.end()) {
		return NULL;
	}

	return itr->second;
}

void GameClientMgr::handleMsg(Link* link)
{
	// 1. 将接收缓冲区的数据全部取出
	evbuffer recvSwapBuf;
	evbuffer *buf = &recvSwapBuf;

	link->beginRead(buf);

	// 2. 循环处理消息数据
	while (true) {
		// 检测包头长度
		int bytes = (int)evbuffer_get_length(buf);
		if (bytes <= sizeof(NetMsgHead)) {
			break;
		}

		LanMsgHead *head = (LanMsgHead*)evbuffer_pullup(buf, sizeof(LanMsgHead));
		int clientId = endiantool::networkToHost(head->clientId);
		int msgId = endiantool::networkToHost(head->msgId);
		int rawMsgSize = endiantool::networkToHost(head->msgLen);

		// 检测半包
		if (rawMsgSize > bytes) {
			break;
		}

		GameClient *client = GameClientMgr::Instance().FindClient(clientId);
		if (NULL == client) {
			return;
		}

		int msgSize = rawMsgSize - sizeof(LanMsgHead);

		const char *peek = (const char*)evbuffer_pullup(buf, msgSize);
		const char* msg = peek + sizeof(LanMsgHead);

		m_dispatcher.dispatch(*client, msgId, msg, msgSize, 0);
		evbuffer_drain(buf, rawMsgSize);
	}

	// 3. 处理完毕后，若有残余的消息体，则将残余消息体重新拷贝到接收缓冲区的头部以保持正确的数据顺序
	link->endRead(buf);
}

void GameClientMgr::OnAuthReq(GameClient* client, AuthReq *req, Timestamp receiveTime)
{
}

void GameClientMgr::OnLoginReq(GameClient* client, LoginReq *req, Timestamp receiveTime)
{
}