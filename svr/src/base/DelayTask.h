#ifndef DelayTask_h__
#define DelayTask_h__

#include "Timestamp.h"

class DelayTaskQueue;

class DelayTask
{
    friend class DelayTaskQueue;

public:
    virtual void Run() = 0;

    void Cancel()
    {
        m_life = 0;
    }

public:
    void GotoDead()
    {
        --m_life;
    }

    int Life()
    {
        return m_life;
    }

    inline Timestamp GetDelay()
    {
        return m_delay;
    }

    bool operator<(const DelayTask& rhs) const 
    {
        return m_expired < rhs.m_expired;
    }
    bool operator==(const DelayTask& rhs) const 
    {
        return m_expired == rhs.m_expired;
    }
    bool operator>(const DelayTask& rhs) const 
    {
        return m_expired > rhs.m_expired;
    }

public:
    Timestamp m_delay;
    int m_life; // 剩余的生命次数

private:
    Timestamp m_expired;
};

#endif // DelayTask_h__
