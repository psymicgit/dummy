///<------------------------------------------------------------------------------
//< @file:   server\basic\thread.cpp
//< @author: 洪坤安
//< @date:   2015年1月13日 17:41:1
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "thread.h"
#include "taskqueue.h"

#ifdef WIN
	unsigned int __stdcall thread_t::thread_func(void* p)
#else
	void* thread_t::thread_func(void* p)
#endif
{
	task_t* t = (task_t*)p;
	t->run();
	delete t;
	return 0;
}

int thread_t::create_thread(task_t func, int num)
{
	for (int i = 0; i < num; ++i) {
		task_t* t = new task_t(func);

#ifdef WIN
		HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, thread_func, t, 0, NULL);
		m_tid_list.push_back(handle);
#else
		pthread_t ntid;

		if (0 == ::pthread_create(&ntid, NULL, thread_func, t)) {
			m_tid_list.push_back(ntid);
		}
#endif
	}
	return 0;
}

int thread_t::join()
{
	if (m_tid_list.empty()) {
		return 0;
	}

#ifdef WIN
	size_t threadCnt = m_tid_list.size();
	HANDLE thread0 = m_tid_list[0];

	WaitForMultipleObjects(threadCnt, &thread0, TRUE, INFINITE);
#else
	for (ThreadIdList::iterator itr = m_tid_list.begin(); itr != m_tid_list.end(); ++itr) {
		pthread_t tid = *itr;
		pthread_join(tid, NULL);
	}
#endif

	m_tid_list.clear();
	return 0;
}
