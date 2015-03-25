// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "TcpServer.h"

#include <stdio.h>  // snprintf

#include <boost/bind.hpp>

#include <base/Logging.h>

#include "Acceptor.h"
#include "EventLoop.h"
#include "EventLoopThreadPool.h"
#include "SocketsOps.h"

using namespace net;

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr, const string& name, Option option)
   : m_loop(CHECK_NOTNULL(loop)),
     m_name(name),
     acceptor_(new Acceptor(loop, listenAddr, option == kReusePort)),
     threadPool_(new EventLoopThreadPool(loop)),
     connectionCallback_(defaultConnectionCallback),
     messageCallback_(defaultMessageCallback),
     started_(false),
     nextConnId_(1),
     m_listenAddr(listenAddr)
{
    acceptor_->setNewConnectionCallback(
        boost::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
    m_loop->assertInLoopThread();
    LOG_TRACE << "TcpServer::~TcpServer [" << m_name << "] destructing";

    for (ConnectionMap::iterator it(connections_.begin());
        it != connections_.end(); ++it)
    {
        TcpConnectionPtr conn = it->second;
        it->second.reset();
        conn->getLoop()->runInLoop(
            boost::bind(&TcpConnection::connectDestroyed, conn));
        conn.reset();
    }
}

void TcpServer::setThreadNum(int numThreads)
{
    assert(0 <= numThreads);
    threadPool_->setThreadNum(numThreads);
}

void TcpServer::start()
{
    if (!started_)
    {
        started_ = true;
        threadPool_->start(threadInitCallback_);
    }

    if (!acceptor_->listenning())
    {
        m_loop->runInLoop(boost::bind(&Acceptor::listen, get_pointer(acceptor_)));
    }
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
    m_loop->assertInLoopThread();
    
    char buf[32];
    sprintf_s(buf, sizeof buf, ":%s#%d", m_listenAddr.IpAndPort().c_str(), nextConnId_);

    ++nextConnId_;

    LOG_INFO << "TcpServer<" << m_name << " - " << buf << "> receive new connection from <" << peerAddr.IpAndPort() << ">";
    
    EventLoop* ioLoop = threadPool_->getNextLoop();
    string connName = m_name + buf;

    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    TcpConnectionPtr conn(new TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));
        connections_[connName] = conn;
        conn->setConnectionCallback(connectionCallback_);
        conn->setMessageCallback(messageCallback_);
        conn->setWriteCompleteCallback(writeCompleteCallback_);
        conn->setCloseCallback(boost::bind(&TcpServer::removeConnection, this, _1)); // FIXME: unsafe

    ioLoop->runInLoop(boost::bind(&TcpConnection::connectEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
    // FIXME: unsafe
    m_loop->runInLoop(boost::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
    m_loop->assertInLoopThread();
    LOG_INFO << "TcpServer::removeConnectionInLoop [" << m_name
        << "] - connection " << conn->name();
    size_t n = connections_.erase(conn->name());
    (void)n;
    assert(n == 1);
    EventLoop* ioLoop = conn->getLoop();
    ioLoop->queueInLoop(
        boost::bind(&TcpConnection::connectDestroyed, conn));
}

