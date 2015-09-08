///<------------------------------------------------------------------------------
//< @file:   server\basic\thread.h
//< @author: 洪坤安
//< @date:   2015年1月13日 17:40:25
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _thread_h_
#define _thread_h_

#include "taskqueue.h"

// 线程类：可管理多个系统线程
class Thread
{
#ifdef WIN
	typedef vector<HANDLE> ThreadIdList;
#else
	typedef list<pthread_t> ThreadIdList;
#endif

public:
#ifdef WIN
	static unsigned int __stdcall run(void* p);
#else
	static void* run(void* p);
#endif


	inline void yield()
	{
#if defined(WIN)
		Sleep(0);
#else
		sched_yield();
#endif
	}

public:
	// 创建num个线程，每个线程一启动将执行传入的任务
	int createThread(Task task, int num);

	// 等待直至所有线程退出
	int join();

private:
	// 持有的系统线程id列表
	ThreadIdList m_tidList;
};

#endif //_thread_h_