// Use of this source code is governed by a BSD-style license
// that can be found in the License file.
//
// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "Thread.h"
#include "ThisThread.h"
#include "Exception.h"
#include "Logging.h"

#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/weak_ptr.hpp>

#include <errno.h>
#include <stdio.h>
#include <sys/types.h>

namespace thread
{
    class ThreadNameInitializer
    {
    public:
        ThreadNameInitializer()
        {
            // CurrentThread::t_threadName = "main";
            // CurrentThread::tid();
            // pthread_atfork(NULL, NULL, &afterFork);
        }
    };

    ThreadNameInitializer init;

    struct ThreadData
    {
        typedef Thread::ThreadFunc ThreadFunc;
        ThreadFunc func_;
        string name_;
        boost::weak_ptr<pid_t> wkTid_;

        ThreadData(const ThreadFunc& func,
            const string& name,
            const boost::shared_ptr<pid_t>& tid)
            : func_(func),
            name_(name),
            wkTid_(tid)
        { }

        void runInThread()
        {
            thread::id tid = this_thread::get_id();

            boost::shared_ptr<pid_t> ptid = wkTid_.lock();
            if (ptid)
            {
                *ptid = tid;
                ptid.reset();
            }

            try
            {
                func_();
            }
            catch (const Exception& ex)
            {
                fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
                fprintf(stderr, "reason: %s\n", ex.what());
                fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
                abort();
            }
            catch (const std::exception& ex)
            {
                fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
                fprintf(stderr, "reason: %s\n", ex.what());
                abort();
            }
            catch (...)
            {
                fprintf(stderr, "unknown exception caught in Thread %s\n", name_.c_str());
                throw; // rethrow
            }
        }
    };

    void* startThread(void* obj)
    {
        ThreadData* data = static_cast<ThreadData*>(obj);
        data->runInThread();
        delete data;
        return NULL;
    }

}

AtomicInt32 Thread::numCreated_;

Thread::Thread(const ThreadFunc& func, const string& n)
: started_(false),
joined_(false),
tid_(new pid_t(0)),
func_(func),
name_(n)
{
    numCreated_.increment();
    pthreadId_.p = NULL;
    pthreadId_.x = 0;
}

Thread::~Thread()
{
    if (started_ && !joined_)
    {
        pthread_detach(pthreadId_);
    }
}

void Thread::start()
{
    assert(!started_);
    started_ = true;

    thread::ThreadData* data = new thread::ThreadData(func_, name_, tid_);
    if (pthread_create(&pthreadId_, NULL, &thread::startThread, data))
    {
        started_ = false;
        delete data; // or no delete?
        LOG_SYSFATAL << "Failed in pthread_create";
    }
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, NULL);
}

