///<------------------------------------------------------------------------------
//< @file:   src\gateserver\test.cpp
//< @author: 洪坤安
//< @date:   2015年8月4日 20:56:20
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "test.h"

#include <tool/ticktool.h>
#include <tool/atomictool.h>
#include <basic/lock.h>

#include <basic/ringbuffer.h>
#include <net/link.h>

void ringbufferTest()
{
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

	//ringbuffer.add(text, sizeof(text), &link);
	//ringbuffer.add(text, sizeof(text), &link);


// 	ringbuffer.allocFreeBlock(40);
// 	ringbuffer.allocFreeBlock(40);
// 	ringbuffer.allocFreeBlock(40);
// 	ringbuffer.allocFreeBlock(40);
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

void test()
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

	//ringbufferTest();
}