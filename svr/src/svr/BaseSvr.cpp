#include "BaseSvr.h"

#include <boost/bind.hpp>

#include <net/TcpClient.h>
#include <net/Message.h>

#include <event2\event.h>

#include <utility>
#include <stdio.h>

#include "ScheduleApi.h"

#include "../packet/LoginReqMsg.pb.h"

#include "ProtobufApi.h"

#include "DB.h"

#include "g_xxx.h"

BaseSvr::BaseSvr(EventLoop* loop, const InetAddress& listenAddr) : 
    m_loop(loop),
    m_tcpSvr(loop, listenAddr, "BaseSvr"),
    m_tcpClient(NULL),
    m_workerThread(NULL),
    m_startTime(timeutil::GetTimeOfDay())
{
    m_tcpSvr.setThreadNum(1);
    m_tcpSvr.setConnectionCallback(boost::bind(&BaseSvr::onAcceptConnection, this, _1));
    m_tcpSvr.setMessageCallback(boost::bind(&BaseSvr::onMessage, this, _1, _2));
}

BaseSvr::~BaseSvr()
{
    delete m_db;
}

void BaseSvr::onConnectionEstablished(const TcpConnectionPtr& conn)
{
    LOG_TRACE << "onConnectionEstablished: " << conn->peerAddress().IpAndPort() << " -> "
        << conn->localAddress().IpAndPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");
}

void BaseSvr::onAcceptConnection(const TcpConnectionPtr& conn)
{
    LOG_TRACE << "onAcceptConnection" << conn->peerAddress().IpAndPort() << " -> "
        << conn->localAddress().IpAndPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");
}

void BaseSvr::onMessage(const TcpConnectionPtr& conn, Buffer* buf)
{
    LOG_INFO << "accept msg from <" << conn->name() << ">";

    string request(buf->peek(), buf->beginWrite());
    LOG_INFO << "received msg from client<" << conn->peerAddress().IpAndPort() << "> : <" << request << ">";

    conn->send(request);

    size_t len = 0;
    while((len = buf->readableBytes()) > 1)
    {
        // 本消息包除头部外剩余部分的长度
        size_t packetLen = endian::networkToHost16(buf->peekInt16());
        if(packetLen > len)
        {///< 缓冲区剩余空间反而大于消息包长度了 >
            conn->send("peek msg fail: packetLen > len of buffer\r\n");
            conn->shutdown();
            break;
        }

        if(false == protobufapi::checksum(buf->peek() + HEADER_LENGTH, packetLen))
        {///< 校验结果不匹配 >
            conn->send("server check sum error. your msg is discard");
            conn->shutdown();
            break;
        }

        Buffer *pMsg = new Buffer();
        pMsg->retrieveAll();
        pMsg->append(buf->peek() + sizeof int16_t, sizeof int16_t + packetLen - TAIL_LENGTH);

        m_msgQueue.put(pMsg);

        buf->retrieve(packetLen + HEADER_LENGTH);
    }
}

void BaseSvr::ConnectToPeer(const InetAddress& peerAddr)
{
    m_tcpClient = new TcpClient(m_loop, peerAddr, "This is BaseSvr");
    m_tcpClient->setConnectionCallback(boost::bind(&BaseSvr::onConnectionEstablished, this, _1));
    m_tcpClient->setMessageCallback(boost::bind(&BaseSvr::onMessage, this, _1, _2));

    m_tcpClient->connect();
}

void BaseSvr::Quit()
{
}

void BaseSvr::ConnectToDB(const char* url, const char* usr, const char* pwd, const char* dbname, int numThreads)
{
    m_db = new DB(m_loop, url, usr, pwd, dbname, numThreads);
    m_db->start();
}

void BaseSvr::MsgLoop()
{
    typedef std::deque<Buffer*> MsgQueue;
    MsgQueue msgQueue;
    {
        MutexLockGuard lock(m_msgQueue.m_mutex);
        if(m_msgQueue.m_queue.empty())
        {
            return;
        }

        msgQueue.swap(m_msgQueue.m_queue);
        m_msgQueue.m_queue.clear();
    }

    for(MsgQueue::iterator itr = msgQueue.begin(); itr != msgQueue.end(); ++itr)
    {
        Buffer *pMsg = *itr;

        uint16_t msgType = endian::networkToHost16(pMsg->peekInt16());
        if(msgType == 20)
        {
            LoginReq loginReq;
            loginReq.ParseFromArray(pMsg->peek() + sizeof(uint16_t), pMsg->readableBytes());

            LOG_INFO << "LoginReq loginReq :" << loginReq.DebugString();

            m_loop->quit();
        }

        if(-1 == msgType)
        {
            InetAddress localAddr(0);

            string name = "@@@@";
            TcpConnectionPtr conn(new TcpConnection(m_loop, name, 0, localAddr, localAddr));
            conn->send("Bad Request!\r\n");
            conn->shutdown();
        }

        delete pMsg;
    }
}

class EchoClockTask : public DelayTask
{
    void Run()
    {
        LOG_INFO << "present time is ：" << timeutil::FormatNow();
    }
};

class DetectTask : public DelayTask
{
public:
    DetectTask() :
      m_times(0)
      {
      }

      void Run()
      {
          LOG_INFO << "run times = " << ++m_times;
      }

private:
    int m_times;
};

class MinutesCountTask : public DelayTask
{
public:
    MinutesCountTask() : m_minutes(0)
    {
    }

    void Run()
    {
        if((DelayTaskQueue::cachedNow / 1000) % 60 == 0)
        {
            LOG_INFO << "passed minutes = " << ++m_minutes;
        }
    }

private:
    int m_minutes;
};

void BaseSvr::Run()
{
    /*
    DelayTask *pTask = new EchoClockTask;
    scheduleapi::TickTickTask(pTask, 4000);

    DelayTask *pDetectTask = new DetectTask;
    scheduleapi::TickTickTask(pDetectTask, 2000);

    MinutesCountTask *pCountMinuteTask = new MinutesCountTask;
    scheduleapi::TickTickTask(pCountMinuteTask, 1000);
    */

    bool quit = false;
    while(!quit)
    {
        MsgLoop();
        m_delayTaskQueue.Run();

        Sleep(20);
    }   
}

void BaseSvr::Start()
{
    g_xxx::g_cur_svr = this;

    m_tcpSvr.start();

    m_workerThread = new Thread(boost::bind(&BaseSvr::Run, this));
    m_workerThread->start();

}

bool BaseSvr::processRequest(const TcpConnectionPtr& conn, const string& request)
{
    return true;
}
