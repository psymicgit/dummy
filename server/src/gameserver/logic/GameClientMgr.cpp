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

bool GameClientMgr::Init()
{
	GameLogic::RegisterClientMsg(ClientMsg_LoginRequest, OnLoginRequest);
	GameLogic::RegisterClientMsg(ClientMsg_AuthRequest, OnAuthRequest);
	GameLogic::RegisterClientMsg(ClientMsg_MoveRequest, OnMoveRequest);
	GameLogic::RegisterClientMsg(ClientMsg_ReadyRequest, OnReadyRequest);

	return true;
}

GameClient* GameClientMgr::FindClient(int clientId)
{
	ClientMap::iterator itr = m_clients.find(clientId);
	if (itr == m_clients.end()) {
		return NULL;
	}

	return itr->second;
}

GameClient* GameClientMgr::AddClient(int clientId)
{
	GameClient *oldClient = FindClient(clientId);
	if (oldClient)
	{
		return oldClient;
	}

	GameClient *client = new GameClient;
	client->Init();

	client->m_clientId = clientId;
	m_clients[clientId] = client;
	return client;
}

void GameClientMgr::OnAuthRequest(GameClient* client, AuthReq *req, Timestamp receiveTime)
{
}

void GameClientMgr::OnLoginRequest(GameClient* client, LoginReq *req, Timestamp receiveTime)
{
}

void GameClientMgr::OnMoveRequest(GameClient* client, MoveRequest *req, Timestamp receiveTime)
{

}

void GameClientMgr::OnReadyRequest(GameClient* client, ReadyRequest *req, Timestamp receiveTime)
{

}
