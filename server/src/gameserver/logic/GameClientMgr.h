//------------------------------------------------------------------------------
// 作者: 洪坤安
// 日期: 2017年5月1日
// 说明: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

#include "GameClient.h"
#include <net/msgdispatcher.h>

class AuthReq;
class LoginReq;

// 客户端管理器，管理当前通过网关与本游戏服连接的所有玩家连接
class GameClientMgr : public Singleton<GameClientMgr>
{
private:
	typedef std::tr1::unordered_map<uint32, GameClient*> ClientMap;

public:
	GameClientMgr();

	GameClient* FindClient(uint32 playerId);

	void handleMsg(Link* link);

private:
	static void OnAuthReq(GameClient* client, AuthReq *req, Timestamp receiveTime);

	static void OnLoginReq(GameClient* client, LoginReq *req, Timestamp receiveTime);

private:
	ClientMap m_clients;

	MsgDispatcher<GameClient> m_dispatcher;
};