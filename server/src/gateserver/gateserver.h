///<------------------------------------------------------------------------------
//< @file:   src\gateserver\gateserver.h
//< @author: 洪坤安
//< @date:   2015年3月9日 16:11:49
//< @brief:	 网关服务器
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gateserver_h_
#define _gateserver_h_

#include "server.h"
#include "net/clientmgr.h"
#include "config/gateconfig.h"

class GameSvrLink;

// 网关服务器：负责充当玩家和游戏服的中转服务器，起路由功能，并负责对消息包进行加解密
class GateServer : public Singleton<GateServer>, public Server
{
public:
	GateServer();

	// 根据配置初始化网关服务器
	bool init(const char* jsonConfig);

	// 启动网关服务器
	void start();

	// 回收服务器资源
	virtual void stopping();

	// 每次服务器循环所执行的任务
	virtual void run();

	// 接收到新的服务器连接
	virtual ServerLink* onAcceptServer(Link&, ServerType, int serverId);

	// 与服务器断开连接
	virtual void onDisconnectServer(Link&, ServerType, int serverId);

public:
	// 将指定玩家的消息发给游戏服
	void sendToGameServer(uint32 clientId, uint16 msgId, const char* data, uint32 len);

private:
	Net				m_wan;			// 外网通信中心：负责与外网进行通信，如：管理玩家的连接
	GameSvrLink*	m_gamesvrLink;	// 与游戏服的连接
	ClientMgr		m_clientMgr;	// 客户端管理中心
	GateConfig		m_config;		// 网关配置
};

#endif //_gateserver_h_