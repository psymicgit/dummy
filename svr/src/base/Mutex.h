#ifndef Mutex_h__
#define Mutex_h__

/*
#include <assert.h>
#include <boost/noncopyable.hpp>

#include <common/Platform.h>

#include "ThisThread.h"

class MutexLock : boost::noncopyable
{
public:
    MutexLock()
        : holder_(0)
    {
    #ifdef WIN32
        InitializeCriticalSection(&m_mutex);
    #else
        int ret = pthread_mutex_init(&m_mutex, NULL);
        assert(ret == 0); (void) ret;
    #endif        
    }

    ~MutexLock()
    {
        assert(holder_ == 0);
    #ifdef WIN32
        LeaveCriticalSection(&m_mutex);
    #else
        int ret = pthread_mutex_destroy(&m_mutex);
        assert(ret == 0); (void) ret;
    #endif
    }

    bool isLockedByThisThread() const
    {
        return holder_ == this_thread::get_id();
    }

    void assertLocked() const
    {
        assert(isLockedByThisThread());
    }

    // internal usage

    void lock()
    {
    #ifdef WIN32
        EnterCriticalSection(&m_mutex);
    #else
        pthread_mutex_lock(&m_mutex);
        assignHolder();
    #endif
    }

    void unlock()
    {
        unassignHolder();

    #ifdef WIN32
        LeaveCriticalSection(&m_mutex);
    #else
        pthread_mutex_unlock(&m_mutex);
    #endif
    }

    mutex_type& getPthreadMutex() // non-const
    {
        return m_mutex;
    }

private:
    friend class Condition;

    class UnassignGuard : boost::noncopyable
    {
    public:
        UnassignGuard(MutexLock& owner)
            : m_owner(owner)
        {
            m_owner.unassignHolder();
        }

        ~UnassignGuard()
        {
            m_owner.assignHolder();
        }

    private:
        MutexLock& m_owner;
    };

    void unassignHolder()
    {
        holder_ = 0;
    }

    void assignHolder()
    {
        holder_ = this_thread::get_id();
    }

    mutex_type m_mutex;

    thread::id holder_;
};

class MutexLockGuard : boost::noncopyable
{
public:
    explicit MutexLockGuard(MutexLock& mutex)
        : m_mutex(mutex)
    {
        m_mutex.lock();
    }

    ~MutexLockGuard()
    {
        m_mutex.unlock();
    }

private:

    MutexLock& m_mutex;
};

// Prevent misuse like:
// MutexLockGuard(mutex_);
// A tempory object doesn't hold the lock for long!
#define MutexLockGuard(x) error "Missing guard object name"
*/

#include <boost/noncopyable.hpp>
#include <assert.h>
#include <pthread.h>

#include "ThisThread.h"

class MutexLock : boost::noncopyable
{
public:
    MutexLock()
        : holder_(0)
    {
        int ret = pthread_mutex_init(&mutex_, NULL);
        assert(ret == 0); (void) ret;
    }

    ~MutexLock()
    {
        assert(holder_ == 0);
        int ret = pthread_mutex_destroy(&mutex_);
        assert(ret == 0); (void) ret;
    }

    bool isLockedByThisThread() const
    {
        return holder_ == this_thread::get_id();
    }

    void assertLocked() const
    {
        assert(isLockedByThisThread());
    }

    // internal usage

    void lock()
    {
        pthread_mutex_lock(&mutex_);
        assignHolder();
    }

    void unlock()
    {
        unassignHolder();
        pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t* getPthreadMutex() /* non-const */
    {
        return &mutex_;
    }

private:
    friend class Condition;

    class UnassignGuard : boost::noncopyable
    {
    public:
        UnassignGuard(MutexLock& owner)
            : owner_(owner)
        {
            owner_.unassignHolder();
        }

        ~UnassignGuard()
        {
            owner_.assignHolder();
        }

    private:
        MutexLock& owner_;
    };

    void unassignHolder()
    {
        holder_ = 0;
    }

    void assignHolder()
    {
        holder_ = this_thread::get_id();
    }

    pthread_mutex_t mutex_;
    pid_t holder_;
};

class MutexLockGuard : boost::noncopyable
{
public:
    explicit MutexLockGuard(MutexLock& mutex)
        : mutex_(mutex)
    {
        mutex_.lock();
    }

    ~MutexLockGuard()
    {
        mutex_.unlock();
    }

private:

    MutexLock& mutex_;
};

// Prevent misuse like:
// MutexLockGuard(mutex_);
// A tempory object doesn't hold the lock for long!
#define MutexLockGuard(x) error "Missing guard object name"

#endif // Mutex_h__
