///<------------------------------------------------------------------------------
//< @file:   server\net\netfactory.cpp
//< @author: hongkunan
//< @date:   2015年1月13日 14:11:56
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

bool NetFactory::init(int threadCnt, int initLinkCnt)
{
	assert(threadCnt > 0);

	m_threadCnt = threadCnt;
	// m_taskQueuePool = new task_queue_pool_t(m_threadCnt);

	m_net.init(initLinkCnt, 1000);
	return true;
}

void NetFactory::start()
{
	if (m_started) {
		return;
	}

	m_started = true;

#ifndef WIN
	// m_thread.create_thread(task_queue_pool_t::gen_task(m_taskQueuePool), m_threadCnt);
#endif

	m_thread.createThread(Task(&runNet, this), 1);
}

void NetFactory::stop()
{
	LOG_WARN << "stopping net ..." << "m_started=" << m_started;

	if (!m_started) {
		return;
	}

	LOG_WARN << "stopping net ...";

	// m_taskQueuePool->close();

#ifndef WIN
	// 关闭所有监听器
	for(size_t i = 0; i < m_listeners.size(); i++) {
		Listener *listener = m_listeners[i];
		listener->close();
	}

// 	for(size_t i = 0; i < m_connectors.size(); i++) {
// 		Connector *connector = m_connectors[i];
// 		connector->close();
// 	}
#endif

	// 关闭网络，并等待直到网络线程退出
	m_net.close();
	m_thread.join();

	// 	delete m_taskQueuePool;
	// 	m_taskQueuePool = NULL;

	m_started = false;

	m_listeners.clear();
	m_connectors.clear();

	LOG_WARN << "stop net successful";
}

Listener* NetFactory::listen(const string& ip, int port, INetReactor &netReactor)
{
	Listener* listener = new Listener(&m_net, &netReactor);
	if (!listener->open(ip, port)) {
		LOG_SYSTEM_ERR << "listen at <" << ip << ": " << port << "> failed";

		delete listener;
		return listener;
	}

	m_listeners.push_back(listener);
	return listener;
}

Connector* NetFactory::connect(const string& ip, int port, INetReactor &netReactor, const char* remoteHostName)
{
	NetAddress peerAddr(ip, port);

	Connector* connector = new Connector(peerAddr, &netReactor, &m_net, remoteHostName);
	connector->connect();

	m_connectors.push_back(connector);
	return connector;
}
