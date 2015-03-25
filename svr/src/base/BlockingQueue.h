// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#ifndef MUDUO_BASE_BLOCKINGQUEUE_H
#define MUDUO_BASE_BLOCKINGQUEUE_H

#include "Condition.h"
#include "Mutex.h"

#include <deque>
#include <assert.h>

namespace base
{

    template<typename T>
    class BlockingQueue
    {
    public:
        BlockingQueue()
            : m_mutex(),
            notEmpty_(m_mutex),
            m_queue()
        {
        }

        void put(const T& x)
        {
            MutexLockGuard lock(m_mutex);
            m_queue.push_back(x);
            notEmpty_.notify(); // wait morphing saves us
            // http://www.domaigne.com/blog/computing/condvars-signal-with-mutex-locked-or-not/
        }

        T take()
        {
            MutexLockGuard lock(m_mutex);
            // always use a while-loop, due to spurious wakeup
            while (m_queue.empty())
            {
                notEmpty_.wait();
            }
            assert(!m_queue.empty());
            T front(m_queue.front());
            m_queue.pop_front();
            return front;
        }

        size_t size() const
        {
            MutexLockGuard lock(m_mutex);
            return m_queue.size();
        }


    public:
        mutable MutexLock m_mutex;
        std::deque<T> m_queue;

    private:
        Condition         notEmpty_;
    };

}

#endif  // MUDUO_BASE_BLOCKINGQUEUE_H
