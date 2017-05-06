//------------------------------------------------------------------------------
// 作者: 洪坤安
// 日期: 2017年5月1日
// 说明: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

#include "GameClient.h"
#include <net/msgdispatcher.h>
#include <client.pb.h>

// 客户端管理器，管理当前通过网关与本游戏服连接的所有玩家连接
class GameClientMgr : public IModule, public Singleton<GameClientMgr>
{
private:
	typedef std::map<uint32, GameClient*> ClientMap;

public:
	bool Init();

	GameClient* FindClient(int clientId);

	GameClient* AddClient(int clientId);

public:
	static void OnAuthRequest(GameClient& client, AuthReq& req, Timestamp receiveTime);

	static void OnLoginRequest(GameClient& client, LoginReq& req, Timestamp receiveTime);
	
	static void OnMoveRequest(GameClient& client, MoveRequest& req, Timestamp receiveTime);
	
	static void OnReadyRequest(GameClient& client, ReadyRequest& req, Timestamp receiveTime);

private:
	ClientMap m_clients;
};