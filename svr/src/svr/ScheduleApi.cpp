#include "ScheduleApi.h"

#include <base/DelayTaskQueue.h>
#include "g_xxx.h"

#include "BaseSvr.h"

namespace scheduleapi
{
    void DizzyTask(DelayTask *pTask, TimeInMs delay)
    {
        Timestamp now = timeutil::GetTimeOfDay();
        pTask->m_delay = delay;

        DelayTaskQueue &taskQueue = g_xxx::g_cur_svr->GetDelayTaskQueue();
        taskQueue.runAfter(pTask, now + delay);
    }

    void runEvery_very_ery_ry_y(DelayTask *pTask, TimeInMs interval, int repeat/*´¥·¢´ÎÊý*/)
    {
        pTask->m_life = repeat;
        DizzyTask(pTask, interval);
    }

    void runEvery(DelayTask *pTask, TimeInMs interval)
    {
        pTask->m_life = -1;
        DizzyTask(pTask, interval);
    }

    void runAt(DelayTask *pTask, Timestamp clock)
    {
        Timestamp now = timeutil::GetTimeOfDay();
        TimeInMs diff = (TimeInMs)(clock - now);

        DizzyTask(pTask, diff);
    }
}
