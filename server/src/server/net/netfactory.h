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
#include "net.h"

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
public:
	static void runNet(void *e);

	NetFactory();

	bool init(int threadCnt);

	void start();

	void stop();

	Listener* listen(const string& ip, int port, INetReactor&);

	Connector* connect(const string& ip, int port, INetReactor&);

public:
	NetModel m_net;
	task_queue_pool_t* m_taskQueuePool;
	thread_t m_thread;

	bool m_started;

	int m_threadCnt; /* = 2 */
};

#endif //_netfactory_h_