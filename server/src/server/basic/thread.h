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

class thread_t
{
#ifdef WIN
	typedef vector<HANDLE> ThreadIdList;
#else
	typedef list<pthread_t> ThreadIdList;
#endif

public:
#ifdef WIN
	static unsigned int __stdcall thread_func(void* p);
#else
	static void* thread_func(void* p);
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
	int create_thread(task_t func, int num);
	int join();

private:
	ThreadIdList m_tid_list;
};

#endif //_thread_h_