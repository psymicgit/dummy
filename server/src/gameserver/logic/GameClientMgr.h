//------------------------------------------------------------------------------
// ����: ������
// ����: 2017��5��1��
// ˵��: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

#include "GameClient.h"
#include <net/msgdispatcher.h>

class AuthReq;
class LoginReq;

// �ͻ��˹�����������ǰͨ�������뱾��Ϸ�����ӵ������������
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