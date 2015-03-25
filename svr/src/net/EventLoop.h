// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//
// This is a public header file, it must only include public header files.

#ifndef MUDUO_NET_EVENTLOOP_H
#define MUDUO_NET_EVENTLOOP_H

#include <vector>

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>

#include <base/Mutex.h>
#include <base/Thread.h>
#include <base/Timestamp.h>

#include "Callbacks.h"

struct event_base;
struct bufferevent;

namespace net
{

    class Channel;
    class TimerQueue;
    class Timer;

    ///
    /// Reactor, at most one per thread.
    ///
    /// This is an interface class, so don't expose too much details.
    class EventLoop : boost::noncopyable
    {
    public:
        typedef boost::function<void()> Functor;

        EventLoop();
        ~EventLoop();  // force out-line dtor, for scoped_ptr members.

        ///
        /// Loops forever.
        ///
        /// Must be called in the same thread as creation of the object.
        ///
        void loop();

        void quit();

        /// Runs callback immediately in the loop thread.
        /// It wakes up the loop, and run the cb.
        /// If in the same loop thread, cb is run within the function.
        /// Safe to call from other threads.
        void runInLoop(const Functor& cb);
        /// Queues callback in the loop thread.
        /// Runs after finish pooling.
        /// Safe to call from other threads.
        void queueInLoop(const Functor& cb);

        // timers

        ///
        /// Runs callback at 'time'.
        /// Safe to call from other threads.
        ///
        Timer* runAt(const Timestamp& time, const TimerCallback& cb);
        ///
        /// Runs callback after @c delay seconds.
        /// Safe to call from other threads.
        ///
        Timer* runAfter(TimeInMs delay, const TimerCallback& cb);
        ///
        /// Runs callback every @c interval seconds.
        /// Safe to call from other threads.
        ///
        Timer* runEvery(TimeInMs interval, const TimerCallback& cb);

        // internal usage
        void wakeup();

        // pid_t threadId() const { return threadId_; }
        void assertInLoopThread()
        {
            if (!isInLoopThread())
            {
                abortNotInLoopThread();
            }
        }

        bool isInLoopThread() const { return threadId_ == this_thread::get_id(); }
        // bool callingPendingFunctors() const { return callingPendingFunctors_; }

        static EventLoop& getEventLoopOfCurrentThread();

    private:
        void breakLoop();

    private:
        void abortNotInLoopThread();
        void handleRead(struct bufferevent &incoming);  // waked up
        void doPendingFunctors();

        bool looping_; /* atomic */
        bool quit_; /* atomic */
        bool callingPendingFunctors_; /* atomic */
        
        const thread::id threadId_;
        int m_wakeupFd;
        // unlike in TimerQueue, which is an internal class,
        // we don't expose Channel to client.
        boost::scoped_ptr<Channel> m_wakeupChannel;
        
        MutexLock mutex_;
        std::vector<Functor> pendingFunctors_; // @BuardedBy mutex_

    public:
        struct event_base *m_eventBase;
    };

}
#endif  // MUDUO_NET_EVENTLOOP_H
