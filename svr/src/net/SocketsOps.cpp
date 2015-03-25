// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include "SocketsOps.h"

#include <base/Logging.h>
#include <base/Types.h>
#include <net/Endian.h>

#include <common/Platform.h>

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>  // snprintf

#include <assert.h>

#include <event2/util.h>

#include <WS2tcpip.h>
#include <io.h>

using namespace base;

namespace
{

    typedef struct sockaddr SA;

    const SA* sockaddr_cast(const struct sockaddr_in* addr)
    {
        return static_cast<const SA*>(implicit_cast<const void*>(addr));
    }

    SA* sockaddr_cast(struct sockaddr_in* addr)
    {
        return static_cast<SA*>(implicit_cast<void*>(addr));
    }

    void setNonBlockAndCloseOnExec(int sockfd)
    {
        /*
        // non-block
        int flags = ::fcntl(sockfd, F_GETFL, 0);
        flags |= O_NONBLOCK;
        int ret = ::fcntl(sockfd, F_SETFL, flags);
        // FIXME check

        // close-on-exec
        flags = ::fcntl(sockfd, F_GETFD, 0);
        flags |= FD_CLOEXEC;
        ret = ::fcntl(sockfd, F_SETFD, flags);
        // FIXME check

        (void)ret;
        (*/
    }

}

namespace sockets
{
    int SocketStartUp()
    {
#ifdef WIN32    //windows
        WSADATA wsa;
        return WSAStartup(MAKEWORD(2,2),&wsa);
#endif
        return -1;
    }
}

int sockets::createNonblockingOrDie()
{
    // socket
#if VALGRIND
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_SYSFATAL << "sockets::createNonblockingOrDie";
    }

    setNonBlockAndCloseOnExec(sockfd);
#else
    int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0)
    {
        LOG_SYSFATAL << "sockets::createNonblockingOrDie";
    }
#endif
    return sockfd;
}

void sockets::bindOrDie(int sockfd, const struct sockaddr_in& addr)
{
    int ret = ::bind(sockfd, sockaddr_cast(&addr), static_cast<socklen_t>(sizeof addr));
    if (ret < 0)
    {
        LOG_SYSFATAL << "sockets::bindOrDie";
    }
}

void sockets::listenOrDie(int sockfd)
{
    int ret = ::listen(sockfd, SOMAXCONN);
    if (ret < 0)
    {
        LOG_SYSFATAL << "sockets::listenOrDie";
    }
}

int sockets::accept(int sockfd, struct sockaddr_in* addr)
{
    socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);
#if VALGRIND
    int connfd = ::accept(sockfd, sockaddr_cast(addr), &addrlen);
    setNonBlockAndCloseOnExec(connfd);
#else
    int connfd = 0;
#endif
    if (connfd < 0)
    {
        int savedErrno = errno;
        LOG_SYSERR << "Socket::accept";
        switch (savedErrno)
        {
        case EAGAIN:
        case EINTR:
        case EPERM:
        case EMFILE: // per-process lmit of open file desctiptor ???
            // expected errors
            errno = savedErrno;
            break;
        case EBADF:
        case EFAULT:
        case EINVAL:
        case ENFILE:
        case ENOMEM:
            // unexpected errors
            LOG_FATAL << "unexpected error of ::accept " << savedErrno;
            break;
        default:
            LOG_FATAL << "unknown error of ::accept " << savedErrno;
            break;
        }
    }
    return connfd;
}

int sockets::connect(int sockfd, const struct sockaddr_in& addr)
{
    return ::connect(sockfd, sockaddr_cast(&addr), static_cast<socklen_t>(sizeof addr));
}

ssize_t sockets::read(int sockfd, void *buf, size_t count)
{
    return recv(sockfd, (char*)buf, count, NULL);
}

ssize_t sockets::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
    return 0;//::readv(sockfd, iov, iovcnt);
}

ssize_t sockets::write(int sockfd, const void *buf, size_t count)
{
    return send(sockfd, (const char*)buf, count, NULL);
}

void sockets::close(int sockfd)
{
    if (evutil_closesocket(sockfd) < 0)
    {
        LOG_SOCKET_FATAL << "sockets::close fail";
    }
}

void sockets::shutdownWrite(int sockfd)
{
    if (::shutdown(sockfd, SD_SEND) < 0)
    {
        LOG_SYSERR << "sockets::shutdownWrite";
    }
}

void sockets::toIpPort(char* buf, size_t size,
                       const struct sockaddr_in& addr)
{
    char host[INET_ADDRSTRLEN] = "INVALID";
    toIp(host, sizeof host, addr);

    uint16_t port = endian::networkToHost16(addr.sin_port);
    sprintf_s(buf, size, "%s:%u", host, port);
}

void sockets::toIp(char* buf, size_t size,
                   const struct sockaddr_in& addr)
{
    const char* name = inet_ntoa(addr.sin_addr);
    if(NULL == name)
    {
        return;
    }

    strcpy_s(buf, strlen(name) + 1, name);
    assert(size >= INET_ADDRSTRLEN);
}

void sockets::fromIpPort(const char* ip, uint16_t port,
struct sockaddr_in* addr)
{
    addr->sin_family = AF_INET;
    addr->sin_port = endian::hostToNetwork16(port);

    if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
    {
        LOG_SYSERR << "sockets::fromIpPort";
    }
}

int sockets::getSocketError(int sockfd)
{
    char optval;
    socklen_t optlen = static_cast<socklen_t>(sizeof optval);

    if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) < 0)
    {
        return errno;
    }
    else
    {
        return optval;
    }
}

struct sockaddr_in sockets::getLocalAddr(int sockfd)
{
    struct sockaddr_in localaddr;
    memset(&localaddr, 0, sizeof localaddr);
    socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
    if (::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen) < 0)
    {
        LOG_SYSERR << "sockets::getLocalAddr";
    }
    return localaddr;
}

struct sockaddr_in sockets::getPeerAddr(int sockfd)
{
    struct sockaddr_in peeraddr;
    memset(&peeraddr, 0, sizeof peeraddr);
    socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
    if (::getpeername(sockfd, sockaddr_cast(&peeraddr), &addrlen) < 0)
    {
        LOG_SYSERR << "sockets::getPeerAddr";
    }
    return peeraddr;
}

bool sockets::isSelfConnect(int sockfd)
{
    struct sockaddr_in localaddr = getLocalAddr(sockfd);
    struct sockaddr_in peeraddr = getPeerAddr(sockfd);
    return localaddr.sin_port == peeraddr.sin_port
        && localaddr.sin_addr.s_addr == peeraddr.sin_addr.s_addr;
}

