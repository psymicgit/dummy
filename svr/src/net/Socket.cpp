// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "Socket.h"

#include <base/Logging.h>
#include "InetAddress.h"
#include "SocketsOps.h"

#include <assert.h>

using namespace net;

Socket::Socket(evutil_socket_t sockfd)
: m_sockfd(sockfd)
{

}

Socket::Socket()
{
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(m_sockfd == INVALID_SOCKET)
    {
        printf("errno = %d\n", WSAGetLastError());
        LOG_SYSFATAL << "sockets::createNonblockingOrDie";
    }

    NonBlocking();
}

Socket::~Socket()
{
    evutil_closesocket(m_sockfd);
}

void Socket::bindAddr(const InetAddress& addr)
{
    int ret = bind(m_sockfd, (struct sockaddr *)&addr.m_addr, sizeof(addr.m_addr));
    if(ret == -1)
    {
        LOG_SOCKET_FATAL << "sockets::bindAddr fail";
        printf("errno=%d\n",errno);
    }
}

void Socket::listen()
{
    int ret = ::listen(m_sockfd, SOMAXCONN);
    if (ret < 0)
    {
        LOG_SOCKET_FATAL << "sockets::listen fail";
        printf("errno=%d\n",errno);
    }
}

int Socket::accept(InetAddress& peeraddr)
{
    struct sockaddr_in client_addr;  
    memset(&client_addr, 0, sizeof client_addr);

    socklen_t slen = sizeof(sockaddr);

    evutil_socket_t client_socket = ::accept(m_sockfd, (struct sockaddr *)&client_addr, &slen);  
    if (INVALID_SOCKET == client_socket)
    {
        LOG_ERROR << "ERROR: accept client fail: ";
        printf("errno=%d\n", WSAGetLastError());

        return INVALID_SOCKET;
    }

    /*
    if (client_socket > FD_SETSIZE)
    {
        oops("ERROR: fd > FD_SETSIZE");
        return;
    }
    */

    LOG_INFO <<"Accept connection from "<< inet_ntoa(client_addr.sin_addr);
    peeraddr.setSockAddrInet(client_addr);

    return client_socket;
}

void Socket::shutdownWrite()
{
    sockets::shutdownWrite(m_sockfd);
}

void Socket::setTcpNoDelay(bool on)
{
    /*int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY,
    &optval, static_cast<socklen_t>(sizeof optval));*/
    // FIXME CHECK
}

void Socket::NonBlocking()
{
    int nonblock_return = evutil_make_socket_nonblocking(m_sockfd);
    assert(nonblock_return != -1);
}

void Socket::enableReuseAddr()
{
    evutil_make_listen_socket_reuseable(m_sockfd);
}

void Socket::setReusePort(bool on)
{
#ifdef SO_REUSEPORT
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEPORT,
        &optval, static_cast<socklen_t>(sizeof optval));
    if (ret < 0)
    {
        LOG_SYSERR << "SO_REUSEPORT failed.";
    }
#else
    if (on)
    {
        LOG_ERROR << "SO_REUSEPORT is not supported.";
    }
#endif
}

void Socket::setKeepAlive(bool on)
{
    /*
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
    &optval, static_cast<socklen_t>(sizeof optval));
    */
    // FIXME CHECK
}

