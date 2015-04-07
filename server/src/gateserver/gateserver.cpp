///<------------------------------------------------------------------------------
//< @file:   src\gateserver\gateserver.cpp
//< @author: 洪坤安
//< @date:   2015年3月9日 16:12:8
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "gateserver.h"
#include "net/gamesvrlink.h"

GateServer::GateServer()
	: Server()
	, m_gamesvrLink(NULL)
{
	m_svrType = eGateServer;
	m_clientMgr.m_taskQueue = &m_taskQueue;
}

bool GateServer::init()
{
	if (!Server::init()) {
		return false;
	}

	logging::init("gateserver", "log_gatesvr_");

	m_lan.init(1);
	m_wan.init(4);

	m_lan.listen("127.0.0.1", 10001, *this);
	m_wan.listen("127.0.0.1", 20001, m_clientMgr);

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
	Server::stopping();
	m_wan.stop();
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
