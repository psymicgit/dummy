///<------------------------------------------------------------------------------
//< @file:   server\basic\taskqueue.h
//< @author: 洪坤安
//< @date:   2014年11月27日 16:26:55
//< @brief:	 任务队列
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _taskqueue_h_
#define _taskqueue_h_

#include <stdexcept>
#include "lock.h"
#include "bind.h"

// #define NDEBUG
// #include "readerwriterqueue.h"

// 阻塞任务队列
class BlockingTaskQueue
{
public:
public:
	typedef std::vector<Task> TaskList;

	BlockingTaskQueue()
		: m_closed(false)
		, m_cond(m_mutex)
	{
	}

	~BlockingTaskQueue()
	{
	}

	void close()
	{
		lock_guard_t<> lock(m_mutex);
		m_closed = true;
		m_cond.broadcast();
	}

	void put(const TaskList& tasks)
	{
		lock_guard_t<> lock(m_mutex);
		bool needNotify = m_tasklist.empty();

		for(size_t i = 0; i < tasks.size(); i++) {
			m_tasklist.push_back(tasks[i]);
		}

		if (needNotify) {
			m_cond.notify();
		}
	}

	void put(const Task& task_)
	{
		lock_guard_t<> lock(m_mutex);
		bool needNotify = m_tasklist.empty();

		m_tasklist.push_back(task_);
		if (needNotify) {
			m_cond.notify();
		}
	}

// 	int consume(Task& task_)
// 	{
// 		lock_guard_t<> lock(m_mutex);
// 		while (m_tasklist.empty()) {
// 			if (m_closed) {
// 				return -1;
// 			}
// 			m_cond.wait();
// 		}
//
// 		task_ = m_tasklist.front();
// 		m_tasklist.pop_front();
//
// 		return 0;
// 	}

// 	int run()
// 	{
// 		Task t;
// 		while (0 == consume(t)) {
// 			t.run();
// 		}
//
// 		return 0;
// 	}

	int take(TaskList& tasks)
	{
		lock_guard_t<> lock(m_mutex);

		while (m_tasklist.empty()) {
			if (m_closed) {
				return -1;
			}

			m_cond.wait();
		}

		tasks.swap(m_tasklist);
		return tasks.size();
	}

	int run()
	{
		TaskList tasks;
		int ret = take(tasks);

		while (ret > 0) {
			for (TaskList::iterator it = tasks.begin(); it != tasks.end(); ++it) {
				(*it).run();
			}

			tasks.clear();
			ret = take(tasks);
		}

		return 0;
	}
private:
	volatile bool   m_closed;
	TaskList		m_tasklist;
	mutex_t         m_mutex;
	condition_var_t m_cond;
};

// 非阻塞任务队列
class TaskQueue //MutexTaskQueue
{
public:
	typedef std::vector<Task> TaskList;

public:
	void put(TaskList& tasks)
	{
		lock_guard_t<> lock(m_mutex);

		for (size_t i = 0; i < tasks.size(); ++i) {
			m_tasklist.push_back(tasks[i]);
		}
	}

	void put(Task task)
	{
		lock_guard_t<> lock(m_mutex);
		m_tasklist.push_back(task);
	}

	int run()
	{
		TaskList tasks;
		int ret = takeAll(tasks);

		for (size_t i = 0; i < tasks.size(); i++) {
			Task &t = tasks[i];
			t.run();
			t.release();
		}

		return ret;
	}

	int takeAll(TaskList& tasks)
	{
		{
			lock_guard_t<> lock(m_mutex);
			if (m_tasklist.empty()) {
				return 0;
			}

			tasks.swap(m_tasklist);
		}

		return tasks.size();
	}

	int size()
	{
		lock_guard_t<> lock(m_mutex);
		return m_tasklist.size();
	}

private:
	TaskList m_tasklist;
	mutex_t m_mutex;
};

/*
// 读写队列
class LockFreeQueue
{
public:
	typedef std::vector<Task> TaskList;

public:
	void put(TaskList& tasks)
	{
		for (size_t i = 0; i < tasks.size(); ++i) {
			m_tasklist.enqueue(tasks[i]);
		}
	}

	void put(Task task)
	{
		m_tasklist.enqueue(task);
	}

	int run()
	{
		TaskList tasks;
		int ret = takeAll(tasks);

		for (size_t i = 0; i < tasks.size(); i++) {
			Task &t = tasks[i];
			t.run();
			t.release();
		}

		return ret;
	}

	int takeAll(TaskList& tasks)
	{
		Task task;
		while(m_tasklist.try_dequeue(task)) {
			tasks.push_back(task);
		}

		if(m_tasklist.size_approx() != 0) {
			LOG_ERROR << "m_tasklist.size_approx() = " << m_tasklist.size_approx();
		}

		return tasks.size();
	}

	int size()
	{
		return m_tasklist.size_approx();
	}

private:
	moodycamel::ReaderWriterQueue<Task> m_tasklist;
};
*/

// 任务队列池，含多个线程，每个线程将运行一个阻塞任务队列
class TaskQueuePool
{
	typedef BlockingTaskQueue::TaskList TaskList;
	typedef std::vector<BlockingTaskQueue*> TaskQueueVec;

public:
	TaskQueuePool() :
		m_index(0)
	{
	}

	bool init(int n)
	{
		for (int i = 0; i < n; ++i) {
			BlockingTaskQueue* q = new BlockingTaskQueue();
			m_queues.push_back(q);
		}

		return true;
	}

	void run()
	{
		BlockingTaskQueue* p = NULL;
		{
			lock_guard_t<> lock(m_mutex);
			if (m_index >= (int)m_queues.size()) {
				throw std::runtime_error("too more thread running!!");
			}
			p = m_queues[m_index++];
		}

		p->run();
	}

	~TaskQueuePool()
	{
		TaskQueueVec::iterator it = m_queues.begin();
		for (; it != m_queues.end(); ++it) {
			delete (*it);
		}

		m_queues.clear();
	}

	void close()
	{
		TaskQueueVec::iterator it = m_queues.begin();
		for (; it != m_queues.end(); ++it) {
			(*it)->close();
		}
	}

	size_t size() const { return m_queues.size(); }

	BlockingTaskQueue* alloc(long id)
	{
		return m_queues[id % m_queues.size()];
	}

	BlockingTaskQueue* randAlloc()
	{
		static unsigned long id_ = 0;
		return m_queues[++id_ %  m_queues.size()];
	}
private:
	mutex_t               m_mutex;
	TaskQueueVec       m_queues;
	int					  m_index;
};

#endif //_taskqueue_h_