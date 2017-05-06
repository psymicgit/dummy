///<------------------------------------------------------------------------------
//< @file:   src\gameserver\gameserver.cpp
//< @author: ������
//< @date:   2015��3��2�� 21:11:22
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#include "gameserver.h"

#include "net/gatelink.h"
#include "db/db.h"
#include "db/dbfactory.h"

bool GameServer::init(const char* jsonConfig)
{
	if (!Server::init()) {
		return false;
	}

	m_svrType = eGameServer;

	m_gateLink = nullptr;
	m_dbLink = nullptr;
	m_gamedb.Init(GameDB);
	m_logdb.Init(LogDB);

	logging::init("gameserver", "log_gamesvr_");

	// ��ȡ��Ϸ������
	if (!m_config.load(jsonConfig)) {
		LOG_INFO << name() << " load config failed! aborted!";
		return false;
	}

	// ��ʼ����������
	if (!m_lan.init(m_config.m_lanThreadNum)) {
		LOG_INFO << name() << " init wan failed! aborted!";
		return false;
	}

	// ��ʼ���ӵ�����������
	std::vector<IpPort> &lanConnects = m_config.m_lanConnects;
	for(size_t i = 0; i < lanConnects.size(); i++) {
		if (!m_lan.connect(lanConnects[i].ip, lanConnects[i].port, *this, lanConnects[i].peerName.c_str())) {
			LOG_ERROR << name() << " connect to " << lanConnects[i].peerName << "<" << lanConnects[i].ip << ":" << lanConnects[i].port <<  "> failed, aborted!";
			return false;
		}
	}

	// ��ʼ��http������
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

	// ���ر�����ʱ��������������ִ����
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
	// ���ط�����
	case eGateServer:
		svrLink = m_gateLink = new GateLink;
		LOG_INFO << "gameserver -> gateserver <svrId = " << peerSvrId << "> connection established";
		break;

	// db������
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
	// ���ط�����
	case eGateServer:
		link = m_gateLink;
		m_gateLink = NULL;

		LOG_INFO << "gameserver -> gateserver<svrId = " << peerSvrId << "> connection broken";
		break;

	// db������
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
	if (m_dbLink) {
		m_dbLink->m_link->send(msgId, msg);
	}

	return true;
}

bool GameServer::SendToGate(int msgId, Message &msg)
{
	if (m_gateLink) {
		m_gateLink->m_link->send(msgId, msg);
	}

	return true;
}
