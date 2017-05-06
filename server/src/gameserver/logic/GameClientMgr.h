//------------------------------------------------------------------------------
// ����: ������
// ����: 2017��5��1��
// ˵��: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

#include "GameClient.h"
#include <net/msgdispatcher.h>
#include <client.pb.h>

// �ͻ��˹�����������ǰͨ�������뱾��Ϸ�����ӵ������������
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