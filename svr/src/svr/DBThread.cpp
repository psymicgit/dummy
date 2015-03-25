#include "DBThread.h"

#include "SqlExecParticle.h"

DBThread::DBThread(sql::Connection &connection) :
    m_connection(connection)
{

}

void DBThread::Run()
{
    SqlExecParticle *particel = NULL;
    while(particel = m_execQueue.take())
    {
        particel->exec();
    }
}

void DBThread::Put(SqlExecParticle* particel)
{
    m_execQueue.put(particel);
}