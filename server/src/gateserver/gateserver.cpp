///<------------------------------------------------------------------------------
//< @file:   src\gateserver\gateserver.cpp
//< @author: 洪坤安
//< @date:   2015年3月9日 16:12:8
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "gateserver.h"
#include "net/gamesvrlink.h"

#include <tool/ticktool.h>
#include <tool/atomictool.h>
#include <basic/lock.h>
//#include <basic/bind.h>

GateServer::GateServer()
	: Server()
	, m_gamesvrLink(NULL)
{
	m_svrType = eGateServer;
	m_clientMgr.m_taskQueue = &m_taskQueue;
}

void GateServer::addnum(std::string *str)
{
	m_num *= 2;
	m_num += 1;

	if (m_num > 10000000) {
		m_num = 1;
	}

	m_num++;
	m_num += str->size();
}

void GateServer::test()
{
	size_t times = 100;
	{
		Tick tick("mutex performance test");

		int num = 0;
		mutex_t lock;
		for(int i = 0; i < times; i++) {
			lock.lock();
			num++;
			lock.unlock();
		}

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}

	{
		Tick tick("atomic test");

		int num = 0;
		for(int i = 0; i < times; i++) {
			atomictool::inc(&num);
		}

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}

	Buffer buf;
	char text[] = "hello word, test";
	buf.append(text, strlen(text));

	std::string str = text;

	times = 10000;
	{
		Tick tick("task_t test");

		for(int i = 0; i < times; i++) {
			Task t = boost::bind(&GateServer::addnum, this, &str);
			t.run();
		}

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}

// 	{
// 		Tick tick("bi::function test");
//
// 		for(int i = 0; i < times; i++) {
// 			bi::function<void (std::string*)> func = bi::bind(&GateServer::addnum, this, _1);
// 			func(&str);
// 		}
//
// 		double speed = tick.endTick() / times;
// 		double count = 1.0f / speed;
// 		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
// 	}

	times = 10000;
	Buffer copyBuf;
	{
		Tick tick("buffer pool test");

		for(int i = 0; i < times; i++) {
			Buffer *buf = m_bufferPool.alloc(1024);
			buf->append(text, strlen(text));
			m_bufferPool.free(buf);
		}

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}

	{
		Tick tick("stack alloc test");

		for(int i = 0; i < times; i++) {
			Buffer *buf = new Buffer(1024);
			buf->append(text, strlen(text));
			delete buf;
		}

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}
}

bool GateServer::init()
{
	m_num = 0;

	if (!Server::init()) {
		return false;
	}

	logging::init("gateserver", "log_gatesvr_");

	m_lan.init(1);
	m_wan.init(4);

	test();

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

void GateServer::sendToGameServer(uint32 clientId, uint16 msgId, const char* data, uint32 len)
{
	m_gamesvrLink->send(clientId, msgId, data, len);
}
