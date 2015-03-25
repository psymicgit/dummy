// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "EventLoop.h"

#include <base/Logging.h>
#include <base/Mutex.h>
#include "Channel.h"
#include "Poller.h"
#include "SocketsOps.h"

#include <boost/bind.hpp>

#include <event2/event.h>
#include <event2/bufferevent.h>

#include <event2/event_struct.h>

#include <net/Timer.h>


using namespace net;

namespace
{
    __declspec(thread) EventLoop* t_loopInThisThread = NULL;

    const int kPollTimeMs = 10000;

    int createEventfd()
    {
        /*
        int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        if (evtfd < 0)
        {
        LOG_SYSERR << "Failed in eventfd";
        abort();
        }
        return evtfd;
        */

        return -1;
    }

    class IgnoreSigPipe
    {
    public:
        IgnoreSigPipe()
        {
            /*
            ::signal(SIGPIPE, SIG_IGN);
            */
            LOG_TRACE << "Ignore SIGPIPE";
        }
    };

    IgnoreSigPipe initObj;
}

EventLoop& EventLoop::getEventLoopOfCurrentThread()
{
    return *t_loopInThisThread;
}

EventLoop::EventLoop()
:   looping_(false),
    quit_(false),
    callingPendingFunctors_(false),
    threadId_(this_thread::get_id()),
    m_wakeupFd(0),
    m_wakeupChannel(new Channel(this, m_wakeupFd))
{
    LOG_DEBUG << "EventLoop created " << this << " in thread " << threadId_;
    if (t_loopInThisThread)
    {
        LOG_FATAL << "Another EventLoop " << t_loopInThisThread
            << " exists in this thread " << threadId_;
    }
    else
    {
        t_loopInThisThread = this;
    }
    
    m_eventBase = event_base_new();
    assert(m_eventBase != NULL);
    if(m_eventBase == NULL)
    {
        oops("ERROR: event_base_new()");
    }

    evutil_socket_t fd[2];

    evutil_socketpair(AF_INET, SOCK_STREAM, 0, fd);
    m_wakeupFd = fd[0];

    m_wakeupChannel->m_sockfd = fd[1];
    m_wakeupChannel->Init();
    m_wakeupChannel->setReadCallback(boost::bind(&EventLoop::handleRead, this, _1));
    // we are always enable reading the wakeupfd
    m_wakeupChannel->enableReading();
}

EventLoop::~EventLoop()
{
    LOG_DEBUG << "EventLoop " << this << " of thread " << threadId_
        << " destructs in thread " << this_thread::get_id();
    // ::close(wakeupFd_);
    t_loopInThisThread = NULL;
}

void EventLoop::loop()
{
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;
    LOG_TRACE << "EventLoop " << this << " start looping";

    event_base_dispatch(m_eventBase);

    LOG_TRACE << "EventLoop " << this << " stop looping";
    looping_ = false;
}

void EventLoop::quit()
{
    quit_ = true;
    runAfter(1, boost::bind(&EventLoop::breakLoop, this));
    
    if (!isInLoopThread())
    {
        wakeup();
    }
}

void EventLoop::breakLoop()
{
    event_base_loopbreak(m_eventBase);
}

void EventLoop::runInLoop(const Functor& cb)
{
    if (isInLoopThread())
    {
        cb();
    }
    else
    {
        queueInLoop(cb);
    }
}

void EventLoop::queueInLoop(const Functor& cb)
{
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(cb);
    }

    wakeup();
}

void timeout_cb(evutil_socket_t fd, short what, void *argc)  
{  
    if(what&EV_TIMEOUT)
    {
        Timer *pTimer = (Timer*)argc;
        pTimer->run();

        if(pTimer->IsRepeat())
        {
            pTimer->Restart();
        }
        else
        {
            delete pTimer;
        }
    }
}  

Timer* EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
{
    Timestamp now = timeutil::GetTimeOfDay();
    return runAfter((TimeInMs)(time - now), cb);
}


Timer* EventLoop::runAfter(TimeInMs delay, const TimerCallback& cb)
{
    struct timeval tv;
    tv.tv_sec = delay / MsPerSecond;
    tv.tv_usec = (delay % MsPerSecond) * 1000;

    Timer *pTimer = new Timer(cb, delay);

    struct event *connect_timeout_ev = event_new(m_eventBase, -1, EV_TIMEOUT, timeout_cb, pTimer);
    event_add(connect_timeout_ev, &tv);

    //Timestamp time = timeutil::GetTimeOfDay() + delay * MsPerSecond;
    return pTimer;
}

Timer* EventLoop::runEvery(TimeInMs interval, const TimerCallback& cb)
{
    Timer *pTimer = runAfter(interval, cb);
    pTimer->SetRepeat();

    return pTimer;
}

void EventLoop::abortNotInLoopThread()
{
    LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
        << " was created in threadId_ = " << threadId_
        << ", current thread id = " <<  this_thread::get_id();
}

void EventLoop::wakeup()
{
    size_t one = 1;
    char buf = 1;

    ssize_t n = sockets::write(m_wakeupFd, &buf, one);
    if (n != one)
    {
        LOG_SOCKET_FATAL << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
    }
}

void EventLoop::handleRead(struct bufferevent &incoming)
{
    size_t one = 1;
    char buf = 0;

    ssize_t n = bufferevent_read(&incoming, &buf, one);
    if (n != one)
    {
        LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
    }

    doPendingFunctors();
}

void EventLoop::doPendingFunctors()
{
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;

    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for (size_t i = 0; i < functors.size(); ++i)
    {
        functors[i]();
    }
    callingPendingFunctors_ = false;
}