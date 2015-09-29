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
{
	m_svrType = eGameServer;
}

bool GameServer::init(const char* jsonConfig)
{
	if (!Server::init()) {
		return false;
	}

	logging::init("gameserver", "log_gamesvr_");

	if (!m_config.load(jsonConfig)) {
		LOG_INFO << name() << " load config failed! aborted!";
		return false;
	}

	m_lan.init(m_config.m_lanThreadNum);

	std::vector<IpPort> &lanConnects = m_config.m_lanConnects;
	for(size_t i = 0; i < lanConnects.size(); i++) {
		if (!m_lan.connect(lanConnects[i].ip, lanConnects[i].port, *this, lanConnects[i].peerName.c_str())) {
			LOG_ERROR << name() << " connect to " << lanConnects[i].peerName << "<" << lanConnects[i].ip << ":" << lanConnects[i].port <<  "> failed, aborted!";
			return false;
		}
	}

// 	if (!m_dbmgr.init()) {
// 		LOG_ERROR << "dbmgr init failed, aborted";
// 		return false;
// 	}

	if (!m_httpMgr.init()) {
		LOG_ERROR << "gamehttpmgr init failed, aborted";
		return false;
	}

	return true;
}

void GameServer::start()
{
	Server::start();
}

void GameServer::stoppping()
{
	LOG_WARN << "stopping game server ...";
	Server::stopping();
	m_dbmgr.stop();

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
	case eGateServer:
		svrLink = m_gateLink = new GateLink;
		LOG_INFO << "gameserver -> gateserver <svrId = " << peerSvrId << "> connection established";
		break;

	default:
		LOG_ERROR << "gameserver -> unknown server <svrType=" << peerSvrType << ",svrId=" << peerSvrId << "> connection established";
		break;
	}

	return svrLink;
}

void GameServer::onDisconnectServer(Link &tcpLink, ServerType svrType, int serverId)
{
	ServerLink *link = NULL;

	switch(svrType) {
	case eGateServer:
		link = m_gateLink;
		m_gateLink = NULL;

		LOG_INFO << "gameserver -> gateserver<svrId = " << serverId << "> connection broken";
		break;

	default:
		LOG_ERROR << "gameserver -> unknown server <svrType=" << svrType << ",svrId=" << serverId << "> connection broken";
		break;
	}

	delete link;
}
