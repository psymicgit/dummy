///<------------------------------------------------------------------------------
//< @file:   server\net\netfactory.h
//< @author: hongkunan
//< @date:   2015Äê1ÔÂ13ÈÕ 14:10:49
//< @brief:
//< Copyright (c) 2015 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _netfactory_h_
#define _netfactory_h_

#include "basic/thread.h"
#include "basic/objectpool.h"
#include "net.h"
#include "link.h"

class Epoll;
class Select;
class task_queue_pool_t;
class thread_t;
class Listener;
class Link;
class Connector;
class INetReactor;

class NetFactory
{
	friend class Listener;
	friend class Connector;

	typedef std::vector<Listener*> ListenerVec;
	typedef std::vector<Connector*> ConnectorVec;

private:
	static void runNet(void *e);

public:
	NetFactory();

	bool init(int threadCnt, int initLinkCnt = 1000);

	void start();

	void stop();

	Listener* listen(const string& ip, int port, INetReactor&);

	Connector* connect(const string& ip, int port, INetReactor&, const char* remoteHostName);

public:
	NetModel m_net;
	task_queue_pool_t* m_taskQueuePool;
	thread_t m_thread;

	bool m_started;

	int m_threadCnt; /* = 2 */

	ListenerVec m_listeners;
	ConnectorVec m_connectors;
};

#endif //_netfactory_h_