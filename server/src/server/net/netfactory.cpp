///<------------------------------------------------------------------------------
//< @file:   server\net\netfactory.cpp
//< @author: hongkunan
//< @date:   2015Äê1ÔÂ13ÈÕ 14:11:56
//< @brief:
//< Copyright (c) 2015 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "netfactory.h"
#include "basic/taskqueue.h"
#include "net/net.h"
#include "net/listener.h"
#include "net/connector.h"

void NetFactory::runNet(void *e)
{
	NetFactory *netFactory = (NetFactory*)e;
	netFactory->m_net.eventLoop();
}

NetFactory::NetFactory()
	: m_started(false)
{

}

bool NetFactory::init(int threadCnt)
{
	assert(threadCnt > 0);

	m_threadCnt = threadCnt;
	m_taskQueuePool = new task_queue_pool_t(m_threadCnt);

	return true;
}

void NetFactory::start()
{
	if (m_started) {
		return;
	}

	m_started = true;

	m_thread.create_thread(task_t(&runNet, this), 1);
	m_thread.create_thread(task_queue_pool_t::gen_task(m_taskQueuePool), m_threadCnt);
}

void NetFactory::stop()
{
	if (m_started) {
		m_taskQueuePool->close();
		m_net.close();
		m_thread.join();
		delete m_taskQueuePool;
		m_taskQueuePool = NULL;
	}
}

Listener* NetFactory::listen(const string& ip, int port, INetReactor &netReactor)
{
	NetAddress listenAddr(ip, port);
	Listener* listener = new Listener(&m_net, &netReactor, m_taskQueuePool);
	LOG_DEBUG << "start listening at <" << ip << ":" << port << ">";

	if (!listener->open(ip, port)) {
		LOG_SOCKET_ERR << "listen at <" << ip << ":" << port << "> failed";
		delete listener;
		return listener;
	}

	return listener;
}

Connector* NetFactory::connect(const string& ip, int port, INetReactor &netReactor)
{
	NetAddress peerAddr(ip, port);
	Connector* connector = new Connector(peerAddr, &netReactor, &m_net, m_taskQueuePool);
	// LOG_DEBUG << "NetFactory::connect addr = " << connector;

	connector->connect();

	return connector;
}
