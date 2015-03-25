#ifndef DBThread_h__
#define DBThread_h__

#include <base/BlockingQueue.h>

class SqlExecParticle;

namespace sql
{
    class Connection;
}

// 数据库线程，每个线程分配一个数据库连接
class DBThread
{
public:
    DBThread(sql::Connection &connection);

    void Run();

    void Put(SqlExecParticle* particel);

private:
    sql::Connection &m_connection;

    typedef base::BlockingQueue<SqlExecParticle*> SqlExecParticleQueue;
    SqlExecParticleQueue m_execQueue;
};

#endif // DBThread_h__
