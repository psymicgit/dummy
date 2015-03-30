///<------------------------------------------------------------------------------
//< @file:   server\tool\sockettool.cpp
//< @author: 洪坤安
//< @date:   2014年11月25日 14:4:45
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#include "sockettool.h"

#include <errno.h>

#include "log/log.h"

#include "net/netaddress.h"

#ifndef WIN
	#include <fcntl.h>
#endif

namespace socktool
{
	socket_t createSocket()
	{
		socket_t sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sockfd < 0) {
			LOG_SOCKET_ERR << "sockets::createNonblockingOrDie";
		}

		return sockfd;
	}

	void closeSocket(socket_t sockfd)
	{
#ifndef WIN
		close(sockfd);
#else
		closesocket(sockfd);
#endif
	}

	bool setNonBlocking(socket_t sockfd)
	{
#ifdef WIN
		u_long nonblocking = 1;
		if (ioctlsocket(sockfd, FIONBIO, &nonblocking) == SOCKET_ERROR) {
			return false;
		}
#else
		int flags;
		if ((flags = fcntl(sockfd, F_GETFL, NULL)) < 0) {
			LOG_SOCKET_ERR << "socket <" << sockfd << "> F_GETFL failed";
			return false;
		}

		if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
			LOG_SOCKET_ERR << "socket " << sockfd << " set nonblock failed";
			return false;
		}
#endif

		return true;
	}

	void setReuseAddr(socket_t sockfd, bool on)
	{
#ifndef WIN
		int optval = on ? 1 : 0;

		/* REUSEADDR on Unix means, "don't hang on to this address after the
		 * listener is closed."  On Windows, though, it means "don't keep other
		 * processes from binding to this address while we're using it. */
		int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
		                       &optval, static_cast<socklen_t>(sizeof optval));

		if (-1 == ret) {
			LOG_SOCKET_ERR << "acceptor_impl_t::open when setsockopt";
		}
#endif
	}

	void setReusePort(socket_t sockfd, bool on)
	{
#ifdef SO_REUSEPORT
		int optval = on ? 1 : 0;
		int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT,
		                       &optval, static_cast<socklen_t>(sizeof optval));
		if (ret < 0) {
			LOG_SYSERR << "SO_REUSEPORT failed.";
		}
#else
		if (on) {
			LOG_ERROR << "SO_REUSEPORT is not supported.";
		}
#endif
	}

	int getSocketError(socket_t sockfd)
	{
		char opt  = 0;
		socklen_t optlen = static_cast<socklen_t>(sizeof opt);

		if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &opt, &optlen) < 0) {
			return errno;
		}

		return opt;
	}

	int geterrno()
	{
#ifdef WIN
		return WSAGetLastError();
#else
		return errno;
#endif
	}

	struct sockaddr_in getLocalAddr(int sockfd)
	{
		struct sockaddr_in localaddr;
		bzero(&localaddr, sizeof localaddr);
		socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
		if (::getsockname(sockfd, (struct sockaddr*)(&localaddr), &addrlen) < 0) {
			LOG_SOCKET_ERR << "sockets::getLocalAddr";
		}
		return localaddr;
	}

	struct sockaddr_in getPeerAddr(int sockfd)
	{
		struct sockaddr_in peeraddr;
		bzero(&peeraddr, sizeof peeraddr);
		socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
		if (::getpeername(sockfd, (struct sockaddr*)(&peeraddr), &addrlen) < 0) {
			LOG_SOCKET_ERR << "sockets::getPeerAddr";
		}
		return peeraddr;
	}

	void setKeepAlive(socket_t sockfd, bool on)
	{
		int optval = on ? 1 : 0;
		::setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&optval, static_cast<socklen_t>(sizeof optval));
	}

	bool bindAddress(socket_t sockfd, const NetAddress& localaddr)
	{
		int ret = bind(sockfd, (struct sockaddr *)&localaddr.m_addr, sizeof(localaddr.m_addr));
		if(ret == -1) {
			LOG_SOCKET_ERR << "bindAddress failed address=<" << localaddr.toIpPort() << ">";
			return false;
		}

		return true;
	}

	bool listen(socket_t sockfd)
	{
		int ret = ::listen(sockfd, SOMAXCONN);
		if (ret < 0) {
			LOG_SOCKET_ERR << "sockets::listen fail, fd = " << sockfd;
			return false;
		}

		return true;
	}

	socket_t accept(socket_t listensock, NetAddress &peerAddr)
	{
		sockaddr_in remoteAddr;
		bzero(&remoteAddr, sizeof remoteAddr);

		socklen_t addrlen = (socklen_t)(sizeof remoteAddr);
		socket_t connfd = ::accept(listensock, (struct sockaddr *)&remoteAddr, &addrlen);

		if (connfd < 0) {
			int err = errno;
			switch (err) {
			case EAGAIN:
			case ECONNABORTED:
			case EINTR:
			case EPROTO: // ???
			case EPERM:
			case EMFILE: // per-process lmit of open file desctiptor ???
				// expected errors
				break;

			default:
				LOG_SOCKET_ERR << "unknown error of ::accept " << err;
			}
		}

		if (connfd >= 0) {
			LOG_INFO << "Accept connection from " << inet_ntoa(remoteAddr.sin_addr);
			peerAddr.setSockAddr(remoteAddr);
		}

		return connfd;
	}

	int connect(int sockfd, const struct sockaddr_in& addr)
	{
		return ::connect(sockfd, (struct sockaddr *)&addr, static_cast<socklen_t>(sizeof addr));
	}
}