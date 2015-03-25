#include "DB.h"

#include <boost/bind.hpp>
#include "DBThread.h"

DB::DB(EventLoop* mainLoop, const char* url, const char* usr, const char* pwd, const char* dbname, int maxSize) :
    m_numThreads(maxSize),
    m_threadPool(mainLoop),
    m_connPool(url, usr, pwd, dbname, maxSize),
    m_next(0)
{
    m_connPool.Init();
}

void DB::start()
{
    m_threadPool.setThreadNum(m_numThreads);
    m_threadPool.start(boost::bind(&DB::NewDBConn, this, _1));
}

void DB::NewDBConn(EventLoop* loop)
{
    Connection* pConn = m_connPool.GetConnection();

    DBThread *pDBThread = new DBThread(*pConn);

    {
        MutexLockGuard lock(m_mutex);
        m_threads.push_back(pDBThread);
    }

    pDBThread->Run();
}

void DB::put(SqlExecParticle* particle)
{
    DBThread *dbthread = GetNextDBThread();
    dbthread->Put(particle);
}

DBThread* DB::GetNextDBThread()
{
    DBThread *dbthread = m_threads[m_next];

    ++m_next;
    if (m_next >= m_threads.size())
    {
        m_next = 0;
    }    

    return dbthread;
}
