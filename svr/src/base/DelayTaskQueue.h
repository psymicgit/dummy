#ifndef DelayTaskQueue_h__
#define DelayTaskQueue_h__

#include <queue>

#include "DelayTask.h"

class DelayTaskQueue
{
public:
    static Timestamp cachedNow;

public:
    void Run();

    void runAfter(DelayTask*, Timestamp expired);

private:
    struct cmp
    {
        bool operator() ( DelayTask* a, DelayTask* b )  
        {  
            return *a > *b;
        }  
    };

private:
    typedef std::priority_queue<DelayTask*, std::vector<DelayTask*>, cmp> TaskPriorityQueue;
    TaskPriorityQueue m_taskqueue;
};

#endif // DelayTaskQueue_h__
