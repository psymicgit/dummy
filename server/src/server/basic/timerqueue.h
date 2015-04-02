///<------------------------------------------------------------------------------
//< @file:   server\basic\timerqueue.h
//< @author: 洪坤安
//< @date:   2015年1月25日 19:3:59
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _timerqueue_h_
#define _timerqueue_h_

#include <queue>

#include "tool/timetool.h"
#include "basic/taskqueue.h"

class TimerQueue;

class Timer
{
	friend class TimerQueue;

public:
	Timer()
		: m_interval(0)
		, m_life(0)
		, m_expired(0)
	{

	}

public:
	virtual void run() = 0;

	void cancel()
	{
		m_life = 0;
	}

public:
	void gotoDead()
	{
		--m_life;
	}

	bool operator<(const Timer& rhs) const
	{
		return m_expired < rhs.m_expired;
	}
	bool operator==(const Timer& rhs) const
	{
		return m_expired == rhs.m_expired;
	}
	bool operator>(const Timer& rhs) const
	{
		return m_expired > rhs.m_expired;
	}

public:
	Timestamp m_interval;
	int m_life; // 剩余的生命次数

private:
	Timestamp m_expired;
};

// 该定时器时间到时将执行指定的函数
class TaskTimer : public Timer
{
public:
	TaskTimer(const task_t &task)
		: m_task(task)
	{
	}

	virtual void run()
	{
		m_task.run();
	}

private:
	task_t m_task;
};

class TimerQueue
{
public:
	static Timestamp cachedNow;

public:
	void run();

	// 指定在xx日xx小时xx秒时间点上执行
	void runAt(Timer*, Timestamp clock);

	// 延时一段时间执行task任务
	void runAfter(const task_t&, TimeInMs delay);

	// 延时一段时间执行task任务
	void runAfter(Timer*, TimeInMs delay);

	// 每隔interval毫秒执行一次，直到执行完指定次数才终结
	void runEvery(Timer*, TimeInMs interval, int times/* 触发次数 */);

	// 每隔interval毫秒执行一次，不会终结
	void runEvery(Timer*, TimeInMs interval);

private:
	struct cmp {
		bool operator() ( Timer* a, Timer* b )
		{
			return *a > *b;
		}
	};

private:
	typedef std::priority_queue<Timer*, std::vector<Timer*>, cmp> TaskPriorityQueue;
	TaskPriorityQueue m_taskqueue;
};

#endif //_timerqueue_h_