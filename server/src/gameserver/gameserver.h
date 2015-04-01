///<------------------------------------------------------------------------------
//< @file:   src\gameserver\gameserver.h
//< @author: 洪坤安
//< @date:   2015年3月2日 21:11:31
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gameserver_h_
#define _gameserver_h_

#include "server.h"
#include "db/dbmgr.h"

class GateLink;

class GameServer : public Singleton<GameServer>, public Server
{
public:
	GameServer();

	bool init();

	void start();

	void run();

	virtual ServerLink* onAcceptServer(Link&, ServerType, int serverId);

	virtual void onDisconnectServer(Link&, ServerType, int serverId);

private:
	GateLink *m_gateLink;

	DBMgr m_dbmgr;
};

#endif //_gameserver_h_