///<------------------------------------------------------------------------------
//< @file:   server\basic\timerqueue.h
//< @author: 洪坤安
//< @date:   2015年1月25日 19:3:59
//< @brief:	 定时器
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _timerqueue_h_
#define _timerqueue_h_

#include <queue>

#include "tool/timetool.h"
#include "basic/taskqueue.h"

#define FOREVER_ALIVE -1

class TimerQueue;

// 单个的定时器
class Timer
{
	friend class TimerQueue;

public:
	Timer()
		: m_interval(0)
		, m_life(0)
		, m_at(0)
	{

	}

	virtual ~Timer() {}

public:
	virtual void run() = 0;

	void cancel()
	{
		m_life = 0;
	}

public:
	// 递减本定时器的剩余生命次数
	void gotoDead()
	{
		// 如果本定时器已被定义为无限存活，则跳过
		if(m_life == FOREVER_ALIVE) {
			return;
		}

		--m_life;

		if (m_life < 0) {
			m_life = 0;
		}
	}

	bool operator<(const Timer& rhs) const
	{
		return m_at < rhs.m_at;
	}
	bool operator==(const Timer& rhs) const
	{
		return m_at == rhs.m_at;
	}
	bool operator>(const Timer& rhs) const
	{
		return m_at > rhs.m_at;
	}

public:
	// 触发间隔
	Timestamp m_interval;

	// 剩余的生命次数（每个生命代表1次触发次数，为-1时表示本定时器将无限存活并定时触发）
	int m_life;

private:
	// 本定时器的触发时间
	Timestamp m_at;
};

// 任务定时器：本定时器触发时将执行预先存储的Task
class TaskTimer : public Timer
{
public:
	TaskTimer(const Task &task)
		: m_task(task)
	{
	}

	virtual void run()
	{
		m_task.run();
	}

private:
	Task m_task;
};

// 定时器队列中心
class TimerQueue
{
public:
	// 返回当前与最近的一个定时器的时间差（毫秒），没有定时器则返回-1
	int run();

	// 指定在xx日xx小时xx秒时间点上执行
	void runAt(Timer*, Timestamp clock);

	// 延时一段时间执行task任务
	void runAfter(const Task&, TimeInMs delay);

	// 延时一段时间执行task任务
	void runAfter(Timer*, TimeInMs delay);

	// 每隔interval毫秒执行一次，直到执行完指定次数才终结
	void runEvery(Timer*, TimeInMs interval, int times/* 触发次数 */);

	// 每隔interval毫秒执行一次，不会终结
	void runEvery(Timer*, TimeInMs interval);

	int size() { return m_timers.size(); }

private:
	struct cmp {
		bool operator() ( Timer* a, Timer* b )
		{
			return *a > *b;
		}
	};

private:
	typedef std::priority_queue<Timer*, std::vector<Timer*>, cmp> TimerPriorityQueue;

	// 定时器
	TimerPriorityQueue m_timers;
};

#endif //_timerqueue_h_