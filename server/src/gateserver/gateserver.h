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
#include "config/gateconfig.h"

class GameSvrLink;

class GateServer : public Singleton<GateServer>, public Server
{
public:
	GateServer();

	bool init(const char* jsonConfig);

	void start();

	virtual void stopping();

	virtual void run();

	// 接收到新的服务器连接
	virtual ServerLink* onAcceptServer(Link&, ServerType, int serverId);

	// 与服务器断开连接
	virtual void onDisconnectServer(Link&, ServerType, int serverId);

public:
	void sendToGameServer(uint32 clientId, uint16 msgId, const char* data, uint32 len);

private:
	// 外网中心：管理与外网的连接，如：与玩家的连接
	NetFactory m_wan;

	// 与游戏服的连接
	GameSvrLink *m_gamesvrLink;

	// 客户端管理中心
	ClientMgr m_clientMgr;

	// 网关配置
	GateConfig m_config;
};

#endif //_gateserver_h_