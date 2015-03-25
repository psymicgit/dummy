#ifndef DB_h__
#define DB_h__

#include <net/EventLoopThreadPool.h>
#include "ConnectionPool.h"
#include <base/Mutex.h>

using net::EventLoopThreadPool;
using net::EventLoop;

class SqlExecParticle;
class DBThread;

class DB
{
public:
    DB(EventLoop* mainLoop, const char* url, const char* usr, const char* pwd, const char* dbname, int maxSize);

    void start();

    void put(SqlExecParticle*);

private:
    DBThread* GetNextDBThread();

    void NewDBConn(EventLoop* loop);

private:
    EventLoopThreadPool m_threadPool;
    ConnPool m_connPool;

    mutable MutexLock m_mutex;
    std::vector<DBThread*> m_threads;

    EventLoop* m_loop;  // the main loop

    size_t m_next;

    int m_numThreads;
};

#endif // DB_h__
