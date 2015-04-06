///<------------------------------------------------------------------------------
//< @file:   src\gateserver\gateserver.h
//< @author: 洪坤安
//< @date:   2015年3月9日 16:11:49
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gateserver_h_
#define _gateserver_h_

#include "server.h"
#include "net/clientmgr.h"

class GameSvrLink;

class GateServer : public Singleton<GateServer>, public Server
{
public:
	GateServer();

	bool init();

	void start();

	virtual void stop();

	virtual void run();

	virtual ServerLink* onAcceptServer(Link&, ServerType, int serverId);

	virtual void onDisconnectServer(Link&, ServerType, int serverId);

private:
	NetFactory m_wan;
	GameSvrLink *m_gamesvrLink;
	ClientMgr m_clientMgr;
};

#endif //_gateserver_h_