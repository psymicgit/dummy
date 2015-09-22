///<------------------------------------------------------------------------------
//< @file:   server\basic\timerqueue.cpp
//< @author: 洪坤安
//< @date:   2015年1月25日 19:4:12
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "timerqueue.h"

int TimerQueue::run()
{
	if(m_timers.empty()) {
		return -1;
	}

	// 获取当前时间
	Timestamp now = timetool::getTimeOfDay();

	// 依次取出离已到期的定时器并执行
	while(!m_timers.empty()) {
		Timer &timer = *m_timers.top();
		if(timer.m_at > now) {
			return (int)(timer.m_at - now);
		}

		m_timers.pop();

		timer.run();
		timer.gotoDead();

		if(timer.isAlive()) {
			timer.m_at = now + timer.m_interval;
			m_timers.push(&timer);
		} else {
			delete &timer;
		}
	}

	return -1;
}

void TimerQueue::runAt(Timer *pTask, Timestamp at)
{
	pTask->m_at = at;
	m_timers.push(pTask);
}

void TimerQueue::runAfter(const Task &task, TimeInMs delay)
{
	TaskTimer *taskTimer = new TaskTimer(task);
	runAfter(taskTimer, delay);
}

void TimerQueue::runEvery(const Task &task, TimeInMs interval)
{
	TaskTimer *taskTimer = new TaskTimer(task);
	runEvery(taskTimer, interval);
}

void TimerQueue::runAfter(Timer *pTask, TimeInMs delay)
{
	Timestamp now = timetool::getTimeOfDay();
	pTask->m_life = 1;
	runAt(pTask, now + delay);
}

void TimerQueue::runEvery(Timer *pTask, TimeInMs interval, int times/* 触发次数 */)
{
	runAfter(pTask, interval);
	pTask->m_life = times;
}

void TimerQueue::runEvery(Timer *pTask, TimeInMs interval)
{
	runAfter(pTask, interval);
	pTask->m_life = FOREVER_ALIVE;
	pTask->m_interval = interval;
}