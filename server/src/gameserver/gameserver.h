///<------------------------------------------------------------------------------
//< @file:   src\gameserver\gameserver.h
//< @author: 洪坤安
//< @date:   2015年3月2日 21:11:31
//< @brief:	 游戏服务器
//< Copyright (c) 2015 服务器. All rights reserved.
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

// 游戏服务器: 负责运行游戏逻辑
class GameServer : public Singleton<GameServer>, public Server
{
public:
	virtual void stoppping();

	virtual ServerLink* onAcceptServer(Link&, ServerType, int serverId);

	virtual void onDisconnectServer(Link&, ServerType, int serverId);

	bool init(const char* jsonConfig);

	void start();

	void run();

	// 发送消息到db服务器
	bool sendToDBServer(uint16 msgId, Message&);
	
	bool SendToGate(int msgId, Message&);

public:
	MsgDispatcher<GameClient> m_clientDispatcher;
	MsgDispatcher<GateLink> m_gateDispatcher;

private:
	GateLink*		m_gateLink;

	ServerLink*		m_dbLink;

	GameHttpMgr		m_httpMgr;

	GameConfig		m_config;		// 游戏服务器配置

	DbAgent			m_gamedb;       // 游戏库代理

	DbAgent			m_logdb;        // 日志库代理
};

#endif //_gameserver_h_