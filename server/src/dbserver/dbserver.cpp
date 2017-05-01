#include "dbserver.h"

#include "db/db.h"
#include "db/dbfactory.h"
#include "db/dbmgr.h"

#include <net/serverlink.h>

bool DBServer::init(const char* jsonConfig)
{
	if (!Server::init()) {
		return false;
	}

	m_svrType = eDBServer;
	m_dbmgr = &DBMgr::Instance();

	logging::init("dbserver", "log_dbsvr_");

	if (!m_config.load(jsonConfig)) {
		LOG_INFO << name() << " load config failed! aborted!";
		return false;
	}

	// ��ʼ����������
	m_lan.init(m_config.m_lanThreadNum);

	// ��ʼ������������������������
	std::vector<IpPort> &lanListens = m_config.m_lanListens;
	for(size_t i = 0; i < lanListens.size(); i++) {
		if (!m_lan.listen(lanListens[i].ip, lanListens[i].port, *this)) {
			LOG_ERROR << "listen at <" << lanListens[i].ip << ":" << lanListens[i].port <<  "> failed, aborted!";
			return false;
		}
	}

	if (!m_dbmgr->init(m_config)) {
		LOG_ERROR << "dbmgr init failed, aborted";
		return false;
	}

	return true;
}

void DBServer::start()
{
	Server::start();
}

void DBServer::stoppping()
{
	LOG_WARN << "stopping game server ...";
	Server::stopping();
	m_dbmgr->stop();

	// ���ر�����ʱ��������������ִ����
	run();

	LOG_WARN << "stop game server successfully!";
}

void DBServer::run()
{
	Server::run();

	m_dbmgr->run();
	sleep(10);
}

ServerLink* DBServer::onAcceptServer(Link &tcpLink, ServerType peerSvrType, int peerSvrId)
{
	ServerLink *svrLink = NULL;

	switch(peerSvrType) {
	// ��Ϸ��
	case eGameServer:
		svrLink = m_gameLink = new ServerLink;
		LOG_INFO << "dbserver <-> gammserver <svrId = " << peerSvrId << "> connection established";
		break;

	default:
		LOG_ERROR << "reject dbserver <-> unknown server <svrType = " << peerSvrType << "> <svrId = " << peerSvrId << "> connection";
		break;
	}

	return svrLink;
}

void DBServer::onDisconnectServer(Link &tcpLink, ServerType svrType, int peerSvrId)
{
	ServerLink *link = NULL;

	switch(svrType) {
	// ��Ϸ��
	case eGameServer:
		link = m_gameLink;
		m_gameLink = NULL;
		LOG_INFO << "dbserver <-> gameserver <svrId = " << peerSvrId << "> connection broken";
		break;

	default:
		LOG_ERROR << "dbserver <-> unknown server <svrType=" << svrType << ",svrId=" << peerSvrId << "> connection broken";
		break;
	}

	delete link;
}