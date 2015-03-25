#ifndef ScheduleApi_h__
#define ScheduleApi_h__

#include <base/Timestamp.h>

class DelayTask;

namespace scheduleapi
{
    // 延时一段时间执行task任务
    void DizzyTask(DelayTask *pTask, TimeInMs delay);

    // 每隔interval毫秒执行一次，直到执行完指定次数才终结
    void runEvery_very_ery_ry_y(DelayTask *pTask, TimeInMs interval, int triggerTimes/*触发次数*/);

    // 每隔interval毫秒执行一次，不会终结
    void runEvery(DelayTask *pTask, TimeInMs interval);

    // 指定在xx日xx小时xx秒时间点上执行
    void runAt(DelayTask *pTask, Timestamp clock);
}

#endif // ScheduleApi_h__
