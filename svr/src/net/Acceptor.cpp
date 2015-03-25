// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "Acceptor.h"

#include "EventLoop.h"
#include "InetAddress.h"
#include "SocketsOps.h"

#include <base/Logging.h>

#include <boost/bind.hpp>

#include <errno.h>
#include <fcntl.h>

#include <event2/event.h>

using namespace net;

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport)
  : m_loop(loop),
    m_acceptSocket(),
    m_listenning(false),
    m_listenAddr(listenAddr)
{

    m_acceptSocket.enableReuseAddr();
    m_acceptSocket.bindAddr(listenAddr);

    // m_acceptChannel.setReadCallback( boost::bind(&Acceptor::handleRead, this, _1, _2, _3) );
}

Acceptor::~Acceptor()
{
    // m_acceptChannel.disableAll();
}


void do_accept(evutil_socket_t svr_socket, short event_type, void *arg)
{
    Acceptor *pAcceptor = static_cast<Acceptor*>(arg);

    pAcceptor->m_loop->assertInLoopThread();

    InetAddress peerAddr(0);
    int connfd = pAcceptor->m_acceptSocket.accept(peerAddr);
    if (connfd >= 0)
    {
        // string hostport = peerAddr.toIpPort();
        // LOG_TRACE << "Accepts of " << hostport;
        if (pAcceptor->newConnectionCallback_)
        {
            pAcceptor->newConnectionCallback_(connfd, peerAddr);
        }
        else
        {
            closesocket(connfd);
        }
    }
    else
    {
        // Read the section named "The special problem of
        // accept()ing when you can't" in libev's doc.
        // By Marc Lehmann, author of livev.
        if (errno == EMFILE)
        {
        }
    }
}

void Acceptor::listen()
{
    LOG_INFO <<"Server <"<< m_listenAddr.IpAndPort() <<"> is listening......";  

    m_loop->assertInLoopThread();
    m_listenning = true;

    m_acceptSocket.listen();
    m_acceptSocket.NonBlocking();

    struct event *listen_event = event_new(m_loop->m_eventBase, m_acceptSocket.m_sockfd, EV_READ|EV_PERSIST, do_accept, (void*)this);
    event_base_set(m_loop->m_eventBase, listen_event);
    event_add(listen_event, NULL); ///< NULL表示无超时设置>
}


