///<------------------------------------------------------------------------------
//< @file:   src\gameserver\gameserver.cpp
//< @author: 洪坤安
//< @date:   2015年3月2日 21:11:22
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "gameserver.h"

#include "net/gatelink.h"
#include "db/db.h"
#include "db/dbfactory.h"

GameServer::GameServer()
	: Server()
	, m_gateLink(NULL)
	, m_dbLink(NULL)
	, m_gamedb(GameDB)
	, m_logdb(LogDB)
{
	m_svrType = eGameServer;
}

bool GameServer::init(const char* jsonConfig)
{
	if (!Server::init()) {
		return false;
	}

	logging::init("gameserver", "log_gamesvr_");

	// 读取游戏服配置
	if (!m_config.load(jsonConfig)) {
		LOG_INFO << name() << " load config failed! aborted!";
		return false;
	}

	// 初始化内网中心
	if (!m_lan.init(m_config.m_lanThreadNum)) {
		LOG_INFO << name() << " init wan failed! aborted!";
		return false;
	}

	// 开始连接到其他服务器
	std::vector<IpPort> &lanConnects = m_config.m_lanConnects;
	for(size_t i = 0; i < lanConnects.size(); i++) {
		if (!m_lan.connect(lanConnects[i].ip, lanConnects[i].port, *this, lanConnects[i].peerName.c_str())) {
			LOG_ERROR << name() << " connect to " << lanConnects[i].peerName << "<" << lanConnects[i].ip << ":" << lanConnects[i].port <<  "> failed, aborted!";
			return false;
		}
	}

	// 初始化http管理器
	if (!m_httpMgr.init()) {
		LOG_ERROR << "gamehttpmgr init failed, aborted";
		return false;
	}

	return true;
}

void GameServer::start()
{
	Server::start();

	m_gamedb.start();
	m_logdb.start();
}

void GameServer::stoppping()
{
	LOG_WARN << "stopping game server ...";
	Server::stopping();

	// 将关闭网络时产生的网络任务执行完
	run();

	LOG_WARN << "stop game server successfully!";
}

void GameServer::run()
{
	Server::run();

	// m_dbmgr.run();
	m_httpMgr.run();
	sleep(10);
}

ServerLink* GameServer::onAcceptServer(Link &tcpLink, ServerType peerSvrType, int peerSvrId)
{
	ServerLink *svrLink = NULL;

	switch(peerSvrType) {
	// 网关服务器
	case eGateServer:
		svrLink = m_gateLink = new GateLink;
		LOG_INFO << "gameserver -> gateserver <svrId = " << peerSvrId << "> connection established";
		break;

	// db服务器
	case eDBServer:
		svrLink = m_dbLink = new ServerLink;
		LOG_INFO << "gameserver -> dbserver <svrId = " << peerSvrId << "> connection established";
		break;

	default:
		LOG_ERROR << "gameserver -> unknown server <svrType=" << peerSvrType << ",svrId=" << peerSvrId << "> connection established";
		break;
	}

	return svrLink;
}

void GameServer::onDisconnectServer(Link &tcpLink, ServerType svrType, int peerSvrId)
{
	ServerLink *link = NULL;

	switch(svrType) {
	// 网关服务器
	case eGateServer:
		link = m_gateLink;
		m_gateLink = NULL;

		LOG_INFO << "gameserver -> gateserver<svrId = " << peerSvrId << "> connection broken";
		break;

	// db服务器
	case eDBServer:
		link = m_dbLink;
		m_dbLink = NULL;

		LOG_INFO << "gameserver -> dbserver <svrId = " << peerSvrId << "> connection broken";
		break;

	default:
		LOG_ERROR << "gameserver -> unknown server <svrType=" << svrType << ",svrId=" << peerSvrId << "> connection broken";
		break;
	}

	delete link;
}

bool GameServer::sendToDBServer(uint16 msgId, Message &msg)
{
	if (NULL == m_dbLink) {
		return false;
	}

	m_dbLink->m_link->send(msgId, msg);
	return true;
}
