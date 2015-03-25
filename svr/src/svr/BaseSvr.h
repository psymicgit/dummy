/*=============================================================================
#
#     FileName: sql_join.h
#         Desc: sql语句的拼装器
#
#       Author: dantezhu
#        Email: zny2008@gmail.com
#     HomePage: http://www.vimer.cn
#
#      Created: 2011-03-12 19:55:42
#      Version: 0.0.1
#      History:
#               0.0.1 | dantezhu | 2011-03-12 19:55:42 | initialization
#
=============================================================================*/

#ifndef BaseSvr_h__
#define BaseSvr_h__

/**
 * @brief   用流处理的方式，添加一个列名
 *
 * @param   key         列名
 *
 * @return  0
 */

#include <base/Atomic.h>
#include <base/Logging.h>
#include <base/Thread.h>
#include <base/Timestamp.h>
#include <base/BlockingQueue.h>

#include <net/EventLoop.h>
#include <net/InetAddress.h>
#include <net/TcpServer.h>

#include <base/DelayTaskQueue.h>

namespace net
{
    class TcpClient;
    class Buffer;
}

class Message;
class DB;

using namespace net;

// 服务器的基类，各类服务器均从此派生
class BaseSvr
{
public:
    BaseSvr(EventLoop* loop, const InetAddress& listenAddr);
    
    ~BaseSvr();

    virtual const std::string Name(){ return "BaseSvr"; }

    void Run();

    void Start();

    void ConnectToPeer(const InetAddress& peerAddr);

    void Quit();

    void ConnectToDB(const char* url, const char* usr, const char* pwd, const char* dbname, int numThreads);

    DelayTaskQueue& GetDelayTaskQueue(){ return m_delayTaskQueue; }

private:
    void onConnectionEstablished(const TcpConnectionPtr& conn);

    void onAcceptConnection(const TcpConnectionPtr& conn);

    void onMessage(const TcpConnectionPtr& conn, Buffer* buf);

    bool processRequest(const TcpConnectionPtr& conn, const string& request);

    void MsgLoop();

    EventLoop* m_loop;
    TcpServer m_tcpSvr;
    TcpClient* m_tcpClient;
    DB *m_db;

    Timestamp m_startTime;
    Thread *m_workerThread;

    typedef base::BlockingQueue<Buffer*> MsgBlockingQueue;
    MsgBlockingQueue m_msgQueue;

    DelayTaskQueue m_delayTaskQueue;
};

#endif // BaseSvr_h__
