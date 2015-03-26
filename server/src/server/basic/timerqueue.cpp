///<------------------------------------------------------------------------------
//< @file:   server\basic\timerqueue.cpp
//< @author: 洪坤安
//< @date:   2015年1月25日 19:4:12
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "timerqueue.h"

#define FOREVER_ALIVE -1

Timestamp TimerQueue::cachedNow = 0;

void TimerQueue::run()
{
	if(m_taskqueue.empty()) {
		return;
	}

	Timestamp now = timetool::GetTimeOfDay();
	cachedNow = now;

	while(!m_taskqueue.empty()) {
		Timer &task = *m_taskqueue.top();
		if(task.m_expired > now) {
			break;
		}

		m_taskqueue.pop();

		if(task.m_life == 0) {
			delete &task;
			continue;
		}

		task.run();

		if(task.m_life != FOREVER_ALIVE) {
			task.gotoDead();
		}

		if(task.m_life != 0) {
			task.m_expired = now + task.m_interval;
			m_taskqueue.push(&task);
		}
		else {
			delete &task;
		}
	}
}

void TimerQueue::runAt(Timer *pTask, Timestamp at)
{
	pTask->m_expired = at;
	m_taskqueue.push(pTask);
}

void TimerQueue::runAfter(const task_t &task, TimeInMs delay)
{
	TaskTimer *taskTimer = new TaskTimer(task);
	runAfter(taskTimer, delay);
}

void TimerQueue::runAfter(Timer *pTask, TimeInMs delay)
{
	Timestamp now = timetool::GetTimeOfDay();
	pTask->m_life = 1;
	runAt(pTask, now + delay);
}

void TimerQueue::runEvery(Timer *pTask, TimeInMs interval, int times/* 触发次数 */)
{
	pTask->m_life = times;
	runAfter(pTask, interval);
}

void TimerQueue::runEvery(Timer *pTask, TimeInMs interval)
{
	pTask->m_life = -1;
	runAfter(pTask, interval);
}