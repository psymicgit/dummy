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
	NetModel *net = (NetModel*)e;
	net->eventLoop();
}


NetFactory::NetFactory()
	: m_started(false)
	, m_curNetIdx(0)
{
}

bool NetFactory::init(int threadCnt, int initLinkCnt)
{
	assert(threadCnt > 0);

	// m_taskQueuePool = new task_queue_pool_t(m_threadCnt);

	// m_net.init(initLinkCnt, 1000);

	for (int i = 0; i < threadCnt; i++) {
		NetModel *net = new NetModel;
		if (NULL == net) {
			LOG_SYSTEM_ERR << "net resource allocate failed not enough memory, aborted!";
			return false;
		}

		net->init(initLinkCnt, initLinkCnt);
		m_nets.push_back(net);
	}

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

	// m_thread.createThread(Task(&runNet, this), 1);

	for (size_t i = 0; i < m_nets.size(); i++) {
		NetModel *net = m_nets[i];
		m_netThread.createThread(Task(&runNet, net), 1);
	}
}

void NetFactory::stop()
{
	if (!m_started) {
		return;
	}

	LOG_WARN << "stopping net ...";

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

	// 关闭网络
	for (size_t i = 0; i < m_nets.size(); i++) {
		NetModel *net = m_nets[i];
		net->close();
	}

	// 等待直到网络线程退出
	m_netThread.join();

	// 	delete m_taskQueuePool;
	// 	m_taskQueuePool = NULL;

	m_started = false;

	m_listeners.clear();
	m_connectors.clear();

	LOG_WARN << "stop net successful";
}

Listener* NetFactory::listen(const string& ip, int port, INetReactor &netReactor)
{
	// 创建一个网络监听器，该网络监听器将被注册到网络中
	Listener* listener = new Listener(nextNet(), &netReactor, this);
	if (NULL == listener) {
		LOG_SYSTEM_ERR << "listen at <" << ip << ": " << port << "> failed, not enough memory";
		return NULL;
	}

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

	// 创建一个网络连接器，该网络监听器将被注册到网络中
	Connector* connector = new Connector(peerAddr, &netReactor, nextNet(), remoteHostName, this);
	if (NULL == connector) {
		LOG_SYSTEM_ERR << "connect to <" << ip << ": " << port << "> failed, not enough memory";
		return NULL;
	}

	connector->connect();

	m_connectors.push_back(connector);
	return connector;
}

NetModel* NetFactory::nextNet()
{
	NetModel *net = m_nets[m_curNetIdx];
	m_curNetIdx = (m_curNetIdx + 1) % m_nets.size();

	return net;
}
