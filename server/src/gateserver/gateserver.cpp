///<------------------------------------------------------------------------------
//< @file:   src\gateserver\gateserver.cpp
//< @author: ������
//< @date:   2015��3��9�� 16:12:8
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
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

	// ��ȡ����
	if (!m_config.load(jsonConfig)) {
		LOG_ERROR << name() << " load config failed! aborted!";
		return false;
	}

	// ��ʼ����������
	if (!m_lan.init(m_config.m_lanThreadNum)) {
		LOG_INFO << name() << " init lan failed! aborted!";
		return false;
	}

	// ��ʼ����������
	if (!m_wan.init(m_config.m_wanThreadNum)) {
		LOG_INFO << name() << " init wan failed! aborted!";
		return false;
	}

	test();

	// ��ʼ������������������
	if(!m_wan.listen(m_config.m_wanListen.ip, m_config.m_wanListen.port, m_clientMgr)) {
		LOG_ERROR << "listen at <" << m_config.m_wanListen.ip << ":" << m_config.m_wanListen.port <<  "> failed, aborted!";
		return false;
	}

	// ��ʼ������������������������
	std::vector<IpPort> &lanListens = m_config.m_lanListens;
	for(size_t i = 0; i < lanListens.size(); i++) {
		if (!m_lan.listen(lanListens[i].ip, lanListens[i].port, *this)) {
			LOG_ERROR << "listen at <" << lanListens[i].ip << ":" << lanListens[i].port <<  "> failed, aborted!";
			return false;
		}
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

	// ���ر�����ʱ��������������ִ����
	run();
}

void GateServer::run()
{
	Server::run();
	sleep(m_config.m_sleep);
}

ServerLink* GateServer::onAcceptServer(Link &tcpLink, ServerType peerSvrType, int peerSvrId)
{
	ServerLink *svrLink = NULL;

	switch(peerSvrType) {
	// ��Ϸ��
	case eGameServer:
		svrLink = m_gamesvrLink = new GameSvrLink;
		LOG_INFO << "gateserver <-> gameserver <svrId = " << peerSvrId << "> connection established";
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
	// ��Ϸ��
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
	if (m_gamesvrLink)
	{
		m_gamesvrLink->send(clientId, msgId, data, len);
	}
}
