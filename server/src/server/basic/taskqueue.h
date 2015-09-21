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

class ITaskQueue
{
public:
	typedef std::list<Task> task_list_t;
public:
	virtual ~ITaskQueue() {}
	virtual void close() = 0;
	virtual void produce(const Task& task_) = 0;
	virtual void multi_produce(const task_list_t& task_) = 0;
	virtual int  consume(Task& task_) = 0;
	virtual int  consume_all(task_list_t&) = 0;
	virtual int run() = 0;
	virtual int batch_run() = 0;
};

// 阻塞任务队列
class BlockingTaskQueue: public ITaskQueue
{
public:
	BlockingTaskQueue():
		m_closed(false),
		m_cond(m_mutex)
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

	void multi_produce(const task_list_t& task_)
	{
		lock_guard_t<> lock(m_mutex);
		bool need_sig = m_tasklist.empty();

		for(task_list_t::const_iterator it = task_.begin(); it != task_.end(); ++it) {
			m_tasklist.push_back(*it);
		}

		if (need_sig) {
			m_cond.notify();
		}
	}
	void produce(const Task& task_)
	{
		lock_guard_t<> lock(m_mutex);
		bool need_sig = m_tasklist.empty();

		m_tasklist.push_back(task_);
		if (need_sig) {
			m_cond.notify();
		}
	}

	int   consume(Task& task_)
	{
		lock_guard_t<> lock(m_mutex);
		while (m_tasklist.empty()) {
			if (m_closed) {
				return -1;
			}
			m_cond.wait();
		}

		task_ = m_tasklist.front();
		m_tasklist.pop_front();

		return 0;
	}

	int run()
	{
		Task t;
		while (0 == consume(t)) {
			t.run();
		}
		return 0;
	}

	int consume_all(task_list_t& tasks_)
	{
		lock_guard_t<> lock(m_mutex);

		while (m_tasklist.empty()) {
			if (m_closed) {
				return -1;
			}
			m_cond.wait();
		}

		tasks_ = m_tasklist;
		m_tasklist.clear();

		return 0;
	}

	int batch_run()
	{
		task_list_t tasks;
		int ret = consume_all(tasks);
		while (0 == ret) {
			for (task_list_t::iterator it = tasks.begin(); it != tasks.end(); ++it) {
				(*it).run();
			}
			tasks.clear();
			ret = consume_all(tasks);
		}
		return 0;
	}
private:
	volatile bool                   m_closed;
	task_list_t                     m_tasklist;
	mutex_t                         m_mutex;
	condition_var_t                 m_cond;
};

// 非阻塞任务队列
class TaskQueue
{
private:
	typedef std::vector<Task> TaskList;

public:
	void put(const Task& task)
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

// 任务队列池，含多个线程，每个线程将运行一个阻塞任务队列
class TaskQueuePool
{
	typedef ITaskQueue::task_list_t task_list_t;
	typedef std::vector<BlockingTaskQueue*>    task_queue_vt_t;
	static void task_func(void* pd_)
	{
		TaskQueuePool* t = (TaskQueuePool*)pd_;
		t->run();
	}
public:
	static Task gen_task(TaskQueuePool* p)
	{
		return Task(&task_func, p);
	}
public:
	TaskQueuePool(int n) :
		m_index(0)
	{
		for (int i = 0; i < n; ++i) {
			BlockingTaskQueue* p = new BlockingTaskQueue();
			m_tqs.push_back(p);
		}
	}

	void run()
	{
		BlockingTaskQueue* p = NULL;
		{
			lock_guard_t<> lock(m_mutex);
			if (m_index >= (int)m_tqs.size()) {
				throw std::runtime_error("too more thread running!!");
			}
			p = m_tqs[m_index++];
		}

		p->batch_run();
	}

	~TaskQueuePool()
	{
		task_queue_vt_t::iterator it = m_tqs.begin();
		for (; it != m_tqs.end(); ++it) {
			delete (*it);
		}
		m_tqs.clear();
	}

	void close()
	{
		task_queue_vt_t::iterator it = m_tqs.begin();
		for (; it != m_tqs.end(); ++it) {
			(*it)->close();
		}
	}

	size_t size() const { return m_tqs.size(); }

	ITaskQueue* alloc(long id_)
	{
		return m_tqs[id_ %  m_tqs.size()];
	}
	ITaskQueue* rand_alloc()
	{
		static unsigned long id_ = 0;
		return m_tqs[++id_ %  m_tqs.size()];
	}
private:
	mutex_t               m_mutex;
	task_queue_vt_t       m_tqs;
	int					  m_index;
};

#endif //_taskqueue_h_