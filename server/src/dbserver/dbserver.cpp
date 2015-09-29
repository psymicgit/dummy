#include "dbserver.h"

#include "db/db.h"
#include "db/dbfactory.h"

#include <net/serverlink.h>

DBServer::DBServer()
	: Server()
{
	m_svrType = eDBServer;
}

bool DBServer::init(const char* jsonConfig)
{
	if (!Server::init()) {
		return false;
	}

	logging::init("dbserver", "log_dbsvr_");

	if (!m_config.load(jsonConfig)) {
		LOG_INFO << name() << " load config failed! aborted!";
		return false;
	}

	// 初始化内网中心
	m_lan.init(m_config.m_lanThreadNum);

	// 开始监听来自内网服务器的连接
	std::vector<IpPort> &lanListens = m_config.m_lanListens;
	for(size_t i = 0; i < lanListens.size(); i++) {
		if (!m_lan.listen(lanListens[i].ip, lanListens[i].port, *this)) {
			LOG_ERROR << "listen at <" << lanListens[i].ip << ":" << lanListens[i].port <<  "> failed, aborted!";
			return false;
		}
	}

	if (!m_dbmgr.init(m_config)) {
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
	m_dbmgr.stop();

	// 将关闭网络时产生的网络任务执行完
	run();

	LOG_WARN << "stop game server successfully!";
}

void DBServer::run()
{
	Server::run();

	m_dbmgr.run();
	sleep(10);
}

ServerLink* DBServer::onAcceptServer(Link &tcpLink, ServerType peerSvrType, int peerSvrId)
{
	ServerLink *svrLink = NULL;

	switch(peerSvrType) {
	// 游戏服
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

void DBServer::onDisconnectServer(Link &tcpLink, ServerType svrType, int serverId)
{
	ServerLink *link = NULL;

	switch(svrType) {
	// 游戏服
	case eGameServer:
		link = m_gameLink;
		m_gameLink = NULL;
		LOG_INFO << "dbserver <-> gameserver <svrId = " << serverId << "> connection broken";
		break;

	default:
		LOG_ERROR << "dbserver <-> unknown server <svrType=" << svrType << ",svrId=" << serverId << "> connection broken";
		break;
	}

	delete link;
}