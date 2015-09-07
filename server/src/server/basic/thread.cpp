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
	unsigned int __stdcall Thread::run(void* p)
#else
	void* Thread::run(void* p)
#endif
{
	Task* t = (Task*)p;
	t->run();
	delete t;
	return 0;
}

int Thread::createThread(Task &task, int num)
{
	for (int i = 0; i < num; ++i) {
		Task* t = new Task(task);

#ifdef WIN
		HANDLE handle = (HANDLE)_beginthreadex(NULL, 0, run, t, 0, NULL);
		m_tidList.push_back(handle);
#else
		pthread_t tid;

		if (0 == ::pthread_create(&tid, NULL, run, t)) {
			m_tidList.push_back(tid);
		} else {
			LOG_FATAL << "createThread num = " << num << "failed";
		}
#endif
	}
	return 0;
}

int Thread::join()
{
	if (m_tidList.empty()) {
		return 0;
	}

#ifdef WIN
	size_t threadCnt = m_tidList.size();

	WaitForMultipleObjects(threadCnt, &m_tidList[0], TRUE, INFINITE);

	for (size_t i = 0; i < threadCnt; i++) {
		HANDLE thread = m_tidList[i];
		CloseHandle(thread);
	}
#else
	for (ThreadIdList::iterator itr = m_tidList.begin(); itr != m_tidList.end(); ++itr) {
		pthread_t tid = *itr;
		pthread_join(tid, NULL);
	}
#endif

	m_tidList.clear();
	return 0;
}
