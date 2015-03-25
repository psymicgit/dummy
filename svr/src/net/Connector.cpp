// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)
//

#include "Connector.h"

#include <base/Logging.h>
#include "net/Channel.h"
#include "EventLoop.h"
#include "SocketsOps.h"
#include "Socket.h"

#include <boost/bind.hpp>

#include <errno.h>

#include <event2/event.h>
#include <event2/event_struct.h>
#include <event2/util.h>
#include <event2/bufferevent.h>

using namespace net;

const int Connector::kMaxRetryDelayMs = 30*1000;
const int Connector::kInitRetryDelayMs = 500;

Connector::Connector(EventLoop* loop, const InetAddress& serverAddr)
: m_loop(loop),
m_serverAddr(serverAddr),
m_enableConnect(false),
state_(kDisconnected),
retryDelayMs_(kInitRetryDelayMs)
{
    LOG_DEBUG << "Connector ctor[" << this << "]";
}

Connector::~Connector()
{
    LOG_DEBUG << "Connector dtor[" << this << "]";
}

void Connector::start()
{
    m_enableConnect = true;
    m_loop->runInLoop(boost::bind(&Connector::startInLoop, this)); // FIXME: unsafe
}

void Connector::startInLoop()
{
    m_loop->assertInLoopThread();
    assert(state_ == kDisconnected);
    if (m_enableConnect)
    {
        connect();
    }
    else
    {
        LOG_DEBUG << "do not connect";
    }
}

void Connector::stop()
{
    m_enableConnect = false;
    m_loop->queueInLoop(boost::bind(&Connector::stopInLoop, this)); // FIXME: unsafe
    // FIXME: cancel timer
}

void Connector::stopInLoop()
{
    m_loop->assertInLoopThread();
    if (state_ == kConnecting)
    {
        setState(kDisconnected);
        int sockfd = removeAndResetChannel();
        retry();
    }
}

void time_cb(int fd, short event, void *argc)  
{  
    Connector *pConnector = (Connector*)argc;
    pConnector->retry();
}  

void eventcb(struct bufferevent *bev, short events, void *arg)
{
    Connector *pConnector = (Connector*)arg;
    pConnector->m_loop->assertInLoopThread();

    evutil_socket_t peer_socket = bufferevent_getfd(bev);
    if (events & BEV_EVENT_CONNECTED) 
    {
        /* We're connected to 127.0.0.1:8080. Ordinarily we'd do
        something here, like start reading or writing. */
        LOG_INFO << "connection<fd = %u> Connect okay.";
        pConnector->connected(peer_socket);
    } 
    else if (events & BEV_EVENT_ERROR) 
    {
        int err = sockets::getSocketError(peer_socket);
        if (err)
        {
            LOG_WARN << "Connector::handleWrite - SO_ERROR = "
                << err << " " << strerror_tl(err);
        }



        /* An error occured while connecting. */
        LOG_SOCKET_ERR << "An error occured while Connector::connecting, reconnect after <" << 2 << "> seconds";
        pConnector->retry();
        sockets::close(peer_socket);
    }
}

void Connector::connect()
{
    struct bufferevent *bev = bufferevent_socket_new(m_loop->m_eventBase, -1, BEV_OPT_CLOSE_ON_FREE);
    bufferevent_setcb(bev, NULL, NULL, eventcb, this);
  
    int ret = bufferevent_socket_connect(
        bev, (struct sockaddr*)&m_serverAddr.getSockAddrInet(), sizeof(sockaddr_in));//cast from sockaddr_in to sockaddr is ok
    
    if(ret < 0)
    {// fail
        bufferevent_free(bev);
        LOG_SOCKET_FATAL << "connect error in Connector::connect";

        return;
    }

    setState(kConnecting);
}

void Connector::connected(int sockfd)
{
    if (state_ != kConnecting)
    {
        LOG_ERROR << "error occur in Connector::connected: state != connecting";
        return;
    }

    setState(kConnected);

    if (m_enableConnect)
    {
        m_newConnectionCallback(sockfd);
    }
    else
    {
        sockets::close(sockfd);
    }
}

void Connector::restart()
{
    m_loop->assertInLoopThread();
    setState(kDisconnected);

    retryDelayMs_ = kInitRetryDelayMs;
    m_enableConnect = true;

    startInLoop();
}

int Connector::removeAndResetChannel()
{
//    channel_->disableAll();

    // int sockfd = channel_->fd();
    // Can't reset channel_ here, because we are inside Channel::handleEvent
    m_loop->queueInLoop(boost::bind(&Connector::resetChannel, this)); // FIXME: unsafe
    return 0;//sockfd;
}

void Connector::resetChannel()
{
    //channel_.reset();
}

void Connector::retry()
{
    setState(kDisconnected);

    if (m_enableConnect)
    {
        LOG_INFO << "Connector::retry - Retry connecting to " << m_serverAddr.IpAndPort()
            << " in " << retryDelayMs_ << " milliseconds. ";

        m_loop->runAfter(retryDelayMs_/1000.0, boost::bind(&Connector::startInLoop, shared_from_this()));

        retryDelayMs_ = min(retryDelayMs_ * 2, kMaxRetryDelayMs);
    }
    else
    {
        LOG_DEBUG << "Connector::retry do not retry";
    }
}

