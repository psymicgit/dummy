///<------------------------------------------------------------------------------
//< @file:   src\gateserver\test.cpp
//< @author: 洪坤安
//< @date:   2015年8月4日 20:56:20
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "test.h"

#include <tool/ticktool.h>
#include <tool/sockettool.h>
#include <tool/atomictool.h>

#include <protocol/message.h>
#include <client.pb.h>

#include <basic/lock.h>

#include <basic/ringbuffer.h>
#include <net/link.h>
#include <net/listener.h>

#include <server.h>

void ringbufferTest()
{
	/*
	Tick tick("ringbuffer test");

	char text[] = "hello word, ringbuffer test";

	NetAddress localhost("127.0.0.1", 80);
	Link link(0, localhost, localhost, NULL, NULL);

	RingBuffer ringbuffer(120);
	ringbuffer.add(text, sizeof(text), &link);
	ringbuffer.statistic();

	ringbuffer.add(text, sizeof(text), &link);
	ringbuffer.statistic();

	link.m_head = link.m_head->skip(10);
	link.m_head = link.m_head->skip(sizeof(text) + 1);
	if (link.m_head == NULL) {
		link.m_tail = NULL;
	}
	*/
}

class BindTest
{
public:
	BindTest()
		: m_num(0)
	{
	}

public:
	void addnum(std::string *str)
	{
		m_num *= 2;
		m_num += 1;

		if (m_num > 10000000) {
			m_num = 1;
		}

		m_num++;
		m_num += str->size();
	}

	int m_num;
};


union epoll_data_fake {
	void *ptr;
	int fd;
	uint32_t u32;
	uint64_t u64;
} ;

struct epoll_event_fake {
	uint32_t events;  /* Epoll events */
	epoll_data_fake data;    /* User data variable */
};

void test()
{
	int times = 100;
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
		BindTest bt;
		Tick tick("task_t test");

		for(int i = 0; i < times; i++) {
			Task t = boost::bind(&BindTest::addnum, &bt, &str);
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
		ObjectPool<Buffer> m_bufferPool;
		m_bufferPool.init(5000, 5000);

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

	{
		Tick tick("公共缓存");

		Buffer publicBuf(strlen(text) * times);

		for(int i = 0; i < times; i++) {
			publicBuf.append(text, strlen(text));
		}

		double speed = tick.endTick() / times;
		double count = 1.0f / speed;
		LOG_WARN << "平均每条耗时 = " << speed << ", 每秒可执行" << count;
	}

	int variableTestNum = 10000;
	{
		Tick tick("global packet test");

		for(int i = 0; i < variableTestNum; i++) {
			LoginReq *req = msgtool::allocRecvPacket<LoginReq>();
			req->set_clientversion(100);
			req->set_deviceid("1273ab23c3390fe840a9e0");
			req->set_notifyid("notifyid-00134678");
			req->set_username("psy_micgit");
			req->set_zoneid(1);
			req->set_userid(100000001);
			req->set_playerid(14560034);
			req->set_job("sword");
			req->set_logintype(LoginReq_LoginType_NewRole);
			req->set_authtype(1);
			req->set_authkey("2ab456b6b2b1b6b1bb2b");

			msgtool::freePacket(req);
		}

		double speed = tick.endTick() / variableTestNum;
		double count = 1.0f / speed;
		LOG_WARN << "avg cost = " << speed << ", per second = " << count;
	}

	{
		Tick tick("stack allocate packet test");

		for(int i = 0; i < variableTestNum; i++) {
			LoginReq localreq;

			LoginReq *req = &localreq;
			req->set_clientversion(100);
			req->set_deviceid("1273ab23c3390fe840a9e0");
			req->set_notifyid("notifyid-00134678");
			req->set_username("psy_micgit");
			req->set_zoneid(1);
			req->set_userid(100000001);
			req->set_playerid(14560034);
			req->set_job("sword");
			req->set_logintype(LoginReq_LoginType_NewRole);
			req->set_authtype(1);
			req->set_authkey("2ab456b6b2b1b6b1bb2b");
		}

		double speed = tick.endTick() / variableTestNum;
		double count = 1.0f / speed;
		LOG_WARN << "avg cost = " << speed << ", per second = " << count;
	}

	{
		int getTimes = 1000;

		Tick tick("socket get address test");

// 		Listener *listener = Server::instance->m_lan.listen("127.0.0.1", 21005, *Server::instance);
//
// 		for(int i = 0; i < getTimes; i++) {
// 			NetAddress peeraddr(socktool::getLocalAddr(listener->socket()));
// 		}

		double speed = tick.endTick() / getTimes;
		double per = 1.0f / speed;
		LOG_WARN << "avg cost = " << speed << ", per second = " << per;
	}


	//ringbufferTest();
}