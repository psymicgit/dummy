#include "DelayTaskQueue.h"

#define FOREVER_ALIVE -1

Timestamp DelayTaskQueue::cachedNow = 0;

void DelayTaskQueue::Run()
{
    if(m_taskqueue.empty())
    {
        return;
    }

    Timestamp now = timeutil::GetTimeOfDay();
    cachedNow = now;

    while(!m_taskqueue.empty())
    {
        DelayTask &task = *m_taskqueue.top();
        if(task.m_expired > now)
        {
            break;
        }

        m_taskqueue.pop();

        if(task.m_life == 0)
        {
            delete &task;
            continue;
        }

        task.Run();

        if(task.m_life != FOREVER_ALIVE)
        {
            task.GotoDead();
        }

        if(task.m_life != 0)
        {
            task.m_expired = now + task.m_delay;
            m_taskqueue.push(&task);
        }
        else
        {
            delete &task;
        }
    }
}

void DelayTaskQueue::runAfter(DelayTask *pTask, Timestamp expired)
{
    pTask->m_expired = expired;
    m_taskqueue.push(pTask);
}