///<------------------------------------------------------------------------------
//< @file:   src\gameserver\gameserver.h
//< @author: ������
//< @date:   2015��3��2�� 21:11:31
//< @brief:	 ��Ϸ������
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gameserver_h_
#define _gameserver_h_

#include "server.h"

#include "logic/GameClientMgr.h"
#include "net/http/gamehttpmgr.h"
#include "net/msgdispatcher.h"
#include "config/gameconfig.h"

#include "db/dbagent.h"

class GateLink;
class GameClient;

// ��Ϸ������: ����������Ϸ�߼�
class GameServer : public Singleton<GameServer>, public Server
{
public:
	GameServer();

	virtual void stoppping();

	virtual ServerLink* onAcceptServer(Link&, ServerType, int serverId);

	virtual void onDisconnectServer(Link&, ServerType, int serverId);

	bool init(const char* jsonConfig);

	void start();

	void run();

	// ������Ϣ��db������
	bool sendToDBServer(uint16 msgId, Message&);

public:
	MsgDispatcher<GameClient> m_dispatcher;

private:
	GateLink*		m_gateLink;

	ServerLink*		m_dbLink;

	GameClientMgr		m_clientMgr;

	GameHttpMgr		m_httpMgr;

	GameConfig		m_config;		// ��Ϸ����������

	DbAgent			m_gamedb;       // ��Ϸ�����

	DbAgent			m_logdb;        // ��־�����
};

#endif //_gameserver_h_