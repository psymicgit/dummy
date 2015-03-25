// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is an internal header file, you should not include this.

#ifndef MUDUO_NET_TIMER_H
#define MUDUO_NET_TIMER_H

#include <boost/noncopyable.hpp>

#include <base/Atomic.h>
#include <base/Timestamp.h>

#include "Callbacks.h"

namespace net
{
    class EventLoop;

    ///
    /// Internal class for timer event.
    ///
    class Timer : boost::noncopyable
    {
    public:
        Timer(const TimerCallback& cb, TimeInMs interval) : 
            m_callback(cb),
            m_interval(interval),
            m_repeat(interval > 0.0)
        { }

        void run() const
        {
            m_callback();
        }

        bool IsRepeat() const { return m_repeat; }

        void Restart();

        void SetRepeat() { m_repeat = true; }

    private:
        const TimerCallback m_callback;
        const TimeInMs m_interval;
        bool m_repeat;
    };
}
#endif  // MUDUO_NET_TIMER_H
