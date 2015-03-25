#include "TcpClient.h"

#include <base/Logging.h>
#include "Connector.h"
#include "EventLoop.h"
#include "SocketsOps.h"

#include <boost/bind.hpp>

#include <stdio.h>  // snprintf

using namespace net;

namespace net
{

    void removeConnection(EventLoop* loop, const TcpConnectionPtr& conn)
    {
        loop->queueInLoop(boost::bind(&TcpConnection::connectDestroyed, conn));
    }

    void removeConnector(const ConnectorPtr& connector)
    {
        //connector->
    }

}

TcpClient::TcpClient(EventLoop* loop,
                     const InetAddress& serverAddr,
                     const string& name)
                     : loop_(CHECK_NOTNULL(loop)),
                     connector_(new Connector(loop, serverAddr)),
                     name_(name),
                     connectionCallback_(defaultConnectionCallback),
                     messageCallback_(defaultMessageCallback),
                     retry_(false),
                     m_connect(true),
                     nextConnId_(1)
{
    connector_->setNewConnectionCallback( boost::bind(&TcpClient::newConnection, this, _1));
    // FIXME setConnectFailedCallback
    LOG_INFO << "TcpClient::TcpClient[" << name_
        << "] - connector " << get_pointer(connector_);
}

TcpClient::~TcpClient()
{
    LOG_INFO << "TcpClient::~TcpClient[" << name_
        << "] - connector " << get_pointer(connector_);
    TcpConnectionPtr conn;
    {
        MutexLockGuard lock(mutex_);
        conn = m_connection;
    }
    if (conn)
    {
        // FIXME: not 100% safe, if we are in different thread
        CloseCallback cb = boost::bind(&net::removeConnection, loop_, _1);
        loop_->runInLoop(boost::bind(&TcpConnection::setCloseCallback, conn, cb));
    }
    else
    {
        connector_->stop();
        // FIXME: HACK
        loop_->runAfter(1, boost::bind(&net::removeConnector, connector_));
    }
}

void TcpClient::connect()
{
    // FIXME: check state
    LOG_INFO << "TcpClient::connect[" << name_ << "] - connecting to "
        << connector_->serverAddress().IpAndPort();
    
    m_connect = true;
    connector_->start();
}

void TcpClient::disconnect()
{
    m_connect = false;

    {
        MutexLockGuard lock(mutex_);
        if (m_connection)
        {
            m_connection->shutdown();
        }
    }
}

void TcpClient::stop()
{
    m_connect = false;
    connector_->stop();
}

void TcpClient::newConnection(int sockfd)
{
    loop_->assertInLoopThread();

    net::InetAddress peerAddr(sockets::getPeerAddr(sockfd));
    
    char buf[32];
    sprintf_s(buf, sizeof buf, ":%s#%d", peerAddr.IpAndPort().c_str(), nextConnId_);
    ++nextConnId_;

    string connName = name_ + buf;

    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    // FIXME poll with zero timeout to double confirm the new connection
    // FIXME use make_shared if necessary
    TcpConnectionPtr conn(new TcpConnection(loop_, connName, sockfd, localAddr, peerAddr));

    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(boost::bind(&TcpClient::removeConnection, this, _1)); // FIXME: unsafe

    {
        MutexLockGuard lock(mutex_);
        m_connection = conn;
    }
    conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr& conn)
{
    loop_->assertInLoopThread();
    assert(loop_ == conn->getLoop());

    {
        MutexLockGuard lock(mutex_);
        assert(m_connection == conn);
        m_connection.reset();
    }

    loop_->queueInLoop(boost::bind(&TcpConnection::connectDestroyed, conn));
    if (retry_ && m_connect)
    {
        LOG_INFO << "TcpClient::connect[" << name_ << "] - Reconnecting to "
            << connector_->serverAddress().IpAndPort();
        connector_->restart();
    }
}

