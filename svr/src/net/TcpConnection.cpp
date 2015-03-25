// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "TcpConnection.h"

#include <base/Logging.h>
#include "Channel.h"
#include "EventLoop.h"
#include "Socket.h"
#include "SocketsOps.h"

#include <boost/bind.hpp>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

#include <errno.h>
#include <stdio.h>

using namespace net;

#define EWOULDBLOCK 10035L

void net::defaultConnectionCallback(const TcpConnectionPtr& conn)
{
    LOG_TRACE << conn->localAddress().IpAndPort() << " -> "
        << conn->peerAddress().IpAndPort() << " is "
        << (conn->connected() ? "UP" : "DOWN");
}

void net::defaultMessageCallback(const TcpConnectionPtr&, Buffer* buf)
{
    buf->retrieveAll();
}

TcpConnection::TcpConnection(EventLoop* loop,
                             const string& nameArg,
                             int sockfd,
                             const InetAddress& localAddr,
                             const InetAddress& peerAddr)
                             : loop_(CHECK_NOTNULL(loop)),
                             name_(nameArg),
                             state_(kConnecting),
                             socket_(new Socket(sockfd)),
                             m_channel(new Channel(loop, sockfd)),
                             localAddr_(localAddr),
                             peerAddr_(peerAddr),
                             highWaterMark_(64*1024*1024)
{
    m_channel->setReadCallback(
        boost::bind(&TcpConnection::handleRead, this, _1));
    m_channel->setWriteCallback(
        boost::bind(&TcpConnection::handleWriteComplete, this, _1));
    m_channel->setCloseCallback(
        boost::bind(&TcpConnection::handleClose, this));
    m_channel->setErrorCallback(
        boost::bind(&TcpConnection::handleError, this));
    LOG_DEBUG << "TcpConnection::ctor[" <<  name_ << "] at " << this
        << " fd=" << sockfd;
    socket_->setKeepAlive(true);
}

TcpConnection::~TcpConnection()
{
    LOG_DEBUG << "TcpConnection::dtor[" <<  name_ << "] at " << this
        << " fd=" << m_channel->fd();
}

void TcpConnection::send(const void* data, size_t len)
{
    if (state_ == kConnected)
    {
        if (loop_->isInLoopThread())
        {
            sendInLoop(data, len);
        }
        else
        {
            string message(static_cast<const char*>(data), len);
            loop_->runInLoop(
                boost::bind(&TcpConnection::sendInLoop,
                this,     // FIXME
                message));
        }
    }
}

void TcpConnection::send(const StringPiece& message)
{
    if (state_ == kConnected)
    {
        if (loop_->isInLoopThread())
        {
            sendInLoop(message);
        }
        else
        {
            loop_->runInLoop(
                boost::bind(&TcpConnection::sendInLoop,
                this,     // FIXME
                message.as_string()));
            //std::forward<string>(message)));
        }
    }
}

// FIXME efficiency!!!
void TcpConnection::send(Buffer* buf)
{
    if (state_ == kConnected)
    {
        if (loop_->isInLoopThread())
        {
            sendInLoop(buf->peek(), buf->readableBytes());
            buf->retrieveAll();
        }
        else
        {
            loop_->runInLoop(
                boost::bind(&TcpConnection::sendInLoop,
                this,     // FIXME
                buf->retrieveAllAsString()));
            //std::forward<string>(message)));
        }
    }
}

void TcpConnection::sendInLoop(const StringPiece& message)
{
    sendInLoop(message.data(), message.size());
}

void TcpConnection::sendInLoop(const void* data, size_t len)
{
    if (state_ == kDisconnected)
    {
        LOG_WARN << "disconnected, give up writing";
        return;
    }

    loop_->assertInLoopThread();

    bool success = m_channel->send(data, len);
    if(false == success)
    {
        LOG_SYSERR << "TcpConnection::sendInLoop";
    }
}

void TcpConnection::shutdown()
{
    // FIXME: use compare and swap
    if (state_ == kConnected)
    {
        setState(kDisconnecting);
        // FIXME: shared_from_this()?
        loop_->runInLoop(boost::bind(&TcpConnection::shutdownInLoop, this));
    }
}

void TcpConnection::shutdownInLoop()
{
    loop_->assertInLoopThread();
    if (!m_channel->isWriting())
    {
        // we are not writing
        socket_->shutdownWrite();
    }
}

void TcpConnection::setTcpNoDelay(bool on)
{
    socket_->setTcpNoDelay(on);
}

void TcpConnection::connectEstablished()
{
    loop_->assertInLoopThread();
    assert(state_ == kConnecting);
    setState(kConnected);
    m_channel->tie(shared_from_this());

    m_channel->Init();
    m_channel->enableReading();
    m_channel->enableWriting();

    connectionCallback_(shared_from_this());
}

void TcpConnection::connectDestroyed()
{
    loop_->assertInLoopThread();
    if (state_ == kConnected)
    {
        setState(kDisconnected);
        m_channel->disableAll();

        connectionCallback_(shared_from_this());
    }
}

void TcpConnection::handleRead(struct bufferevent &incoming)
{
    loop_->assertInLoopThread();
    int savedErrno = 0;

    const int MAX_LINE = 256;

    evutil_socket_t client_socket = bufferevent_getfd(&incoming);  
    size_t n = bufferevent_read(&incoming, inputBuffer_.beginWrite(), MAX_LINE);
    if (n > 0)
    {
        inputBuffer_.hasWritten(n);
        messageCallback_(shared_from_this(), &inputBuffer_);
    }
    else
    {
        handleClose();
    }
}

// 当输出缓冲区被读完的时候，回调此函数
void TcpConnection::handleWriteComplete(struct bufferevent &bev)
{
    loop_->assertInLoopThread();
   
    if (writeCompleteCallback_)
    {
        loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));
    }

    struct evbuffer *output = bufferevent_get_output(&bev);
    int n = evbuffer_get_length(output);

    // LOG_INFO << "write complete, the remain data size is <" << n << ">";
}

void TcpConnection::handleClose()
{
    loop_->assertInLoopThread();

    LOG_TRACE << "connection <fd = " << m_channel->fd() << "> closed";
    setState(kDisconnected);

    m_channel->disableAll();

    TcpConnectionPtr guardThis(shared_from_this());
    connectionCallback_(guardThis);
    
    // must be the last line
    closeCallback_(guardThis);
}

void TcpConnection::handleError()
{
    int err = sockets::getSocketError(m_channel->fd());
    LOG_ERROR << "TcpConnection::handleError [" << name_
        << "] - SO_ERROR = " << err << " " << strerror_tl(err);
}

