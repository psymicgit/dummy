///<------------------------------------------------------------------------------
//< @file:   src\gateserver\gateserver.cpp
//< @author: 洪坤安
//< @date:   2015年3月9日 16:12:8
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "gateserver.h"
#include "net/gamesvrlink.h"

#include "test.h"

GateServer::GateServer()
	: Server()
	, m_gamesvrLink(NULL)
{
	m_svrType = eGateServer;
	m_clientMgr.m_taskQueue = &m_taskQueue;
}

bool GateServer::init(const char* jsonConfig)
{
	if (!Server::init()) {
		return false;
	}

	logging::init("gateserver", "log_gatesvr_");

	if (!m_config.load(jsonConfig)) {
		LOG_INFO << name() << "load config failed! aborted!";
		return false;
	}

	m_lan.init(m_config.m_lanThreadNum);
	m_wan.init(m_config.m_wanThreadNum);

	// test();
	m_wan.listen(m_config.m_wanListen.ip, m_config.m_wanListen.port, m_clientMgr);

	std::vector<IpPort> &lanListens = m_config.m_lanListens;
	for(size_t i = 0; i < lanListens.size(); i++) {
		m_wan.listen(lanListens[i].ip, lanListens[i].port, *this);
	}

	return true;
}

void GateServer::start()
{
	m_wan.start();

	Server::start();
}

void GateServer::stopping()
{
	LOG_WARN << "stopping gate server ...";
	m_clientMgr.close();

	Server::stopping();
	m_wan.stop();

	// 将关闭网络时产生的网络任务执行完
	run();
}

void GateServer::run()
{
	Server::run();
	sleep(10);
}

ServerLink* GateServer::onAcceptServer(Link &tcpLink, ServerType peerSvrType, int peerSvrId)
{
	ServerLink *svrLink = NULL;

	switch(peerSvrType) {
	// 游戏服
	case eGameServer:
		svrLink = m_gamesvrLink = new GameSvrLink;
		LOG_INFO << "gateserver <-> gatmserver <svrId = " << peerSvrId << "> connection established";
		break;

	default:
		LOG_ERROR << "reject gateserver <-> unknown server <svrType = " << peerSvrType << "> <svrId = " << peerSvrId << "> connection";
		// LOG_ERROR << "receive unknown server connection <svrType = " << peerSvrType << "> <svrId = " << peerSvrId << ">";
		break;
	}

	return svrLink;
}

void GateServer::onDisconnectServer(Link &tcpLink, ServerType svrType, int serverId)
{
	ServerLink *link = NULL;

	switch(svrType) {
	// 游戏服
	case eGameServer:
		link = m_gamesvrLink;
		m_gamesvrLink = NULL;
		LOG_INFO << "gateserver <-> gameserver <svrId = " << serverId << "> connection broken";
		break;

	default:
		LOG_ERROR << "gateserver <-> unknown server <svrType=" << svrType << ",svrId=" << serverId << "> connection broken";
		break;
	}

	delete link;
}

void GateServer::sendToGameServer(uint32 clientId, uint16 msgId, const char* data, uint32 len)
{
	m_gamesvrLink->send(clientId, msgId, data, len);
}
