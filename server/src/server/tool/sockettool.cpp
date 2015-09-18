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
	#include <netinet/tcp.h>
#endif

namespace socktool
{
	socket_t createSocket()
	{
		socket_t sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sockfd < 0) {
			LOG_SYSTEM_ERR << "socktool::createSocket";
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
			LOG_SYSTEM_ERR << "socket <" << sockfd << "> F_GETFL failed";
			return false;
		}

		if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
			LOG_SYSTEM_ERR << "socket " << sockfd << " set nonblock failed";
			return false;
		}
#endif

		return true;
	}

	bool setTcpNoDelay(socket_t sockfd)
	{
		socklen_t on = 1;

		// 禁用Nagle算法
		int ret = setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (const char*)&on, sizeof(on));
		if (ret == -1) {
			LOG_SYSTEM_ERR << "set socket " << sockfd << " TCP_NODELAY failed";
			return false;
		}

		return true;
	}

	void setKeepAlive(socket_t sockfd, bool on, int keepIdleTime)
	{
		socklen_t keepAlive = on ? 1 : 0;
		int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (const char*)&keepAlive, sizeof(keepAlive));
		if (ret == -1) {
			LOG_SYSTEM_ERR << "set socket " << sockfd << " SO_KEEPALIVE failed";
			return;
		}

#ifndef WIN
		// 如该连接在指定秒内没有任何数据往来,则进行探测
		setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, (void*)(&keepIdleTime), (socklen_t)sizeof(keepIdleTime));
#endif
	}

	void setSendBufSize(socket_t sock, int size)
	{
		socklen_t sendBuf  = 0;
		socklen_t optlen = static_cast<socklen_t>(sizeof sendBuf);

		if (::getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char*)&sendBuf, &optlen) < 0) {
			LOG_SYSTEM_ERR << "socket " << sock << " SO_SNDBUF = [" << size << "]";
			return;
		}

		LOG_INFO << "socket " << sock << " SO_SNDBUF = [" << sendBuf << "]";


		int ret = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (char *)&size, sizeof(size) );
		if (ret == -1) {
			LOG_SYSTEM_ERR << "set socket " << sock << " SO_SNDBUF to [" << size << "] failed";
		}
	}

	void setRecvBufSize(socket_t sock, int size)
	{
		socklen_t recvBuf  = 0;
		socklen_t optlen = static_cast<socklen_t>(sizeof recvBuf);

		if (::getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&recvBuf, &optlen) < 0) {
			LOG_SYSTEM_ERR << "socket " << sock << " SO_RCVBUF = [" << size << "]";
			return;
		}

		LOG_INFO << "socket " << sock << " SO_RCVBUF = [" << recvBuf << "]";

		int ret = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char *)&size, sizeof(size) );
		if (ret == -1) {
			LOG_SYSTEM_ERR << "set socket " << sock << " SO_RCVBUF to [" << size << "] failed";
		}
	}

	void setLinger(socket_t sock, int waitTime)
	{
		linger ling;

		ling.l_onoff = 1; // (在closesocket()调用, 在还有数据没发送完毕的时候容许逗留)
		ling.l_linger = waitTime; // 延迟关闭socket的时间
		setsockopt(sock, SOL_SOCKET, SO_LINGER, (const char*)&ling, sizeof(linger));
	}

	bool setReuseAddr(socket_t sockfd, bool on)
	{
		int optval = on ? 1 : 0;

		/* REUSEADDR on Unix means, "don't hang on to this address after the
		 * listener is closed."  On Windows, though, it means "don't keep other
		 * processes from binding to this address while we're using it. */
		int ret = ::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
		                       (const char*)&optval, static_cast<socklen_t>(sizeof optval));

		if (-1 == ret) {
			LOG_SYSTEM_ERR << "acceptor_impl_t::open when setsockopt";
			return false;
		}

		return true;
	}

	int getSocketError(socket_t sockfd)
	{
		socklen_t err  = 0;
		socklen_t optlen = static_cast<socklen_t>(sizeof err);

		if (::getsockopt(sockfd, SOL_SOCKET, SO_ERROR, (char*)&err, &optlen) < 0) {
			return err;
		}

		return err;
	}

#ifdef WIN
	thread_local char t_errnobuf[512];

	const char* strerror_threadlocal(int savedErrno)
	{
		strerror_s(t_errnobuf, sizeof t_errnobuf, savedErrno);
		return t_errnobuf;
	}
#endif

	const char* getErrorMsg(int err)
	{
#ifdef WIN
//		return strerror_threadlocal(err);
		LPVOID lpMsgBuf;

		FormatMessage(
		    FORMAT_MESSAGE_ALLOCATE_BUFFER |
		    FORMAT_MESSAGE_FROM_SYSTEM |
		    FORMAT_MESSAGE_IGNORE_INSERTS,
		    NULL,
		    err,
		    MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
		    (LPTSTR) &lpMsgBuf,
		    0,
		    NULL );

		return (const char*)lpMsgBuf;
#else
		return strerror(err);
#endif
	}

	int geterrno()
	{
		int err = 0;

#ifdef WIN
		err = WSAGetLastError();
#else
		err = errno;
#endif

		// clearerrno();
		return err;
	}

	void clearerrno()
	{
#ifdef WIN
		WSASetLastError(0);
#else
		errno = 0;
#endif
	}

	struct sockaddr_in getLocalAddr(int sockfd)
	{
		struct sockaddr_in localaddr;
		bzero(&localaddr, sizeof localaddr);
		socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
		if (::getsockname(sockfd, (struct sockaddr*)(&localaddr), &addrlen) < 0) {
			LOG_SYSTEM_ERR << "sockets::getLocalAddr";
		}
		return localaddr;
	}

	struct sockaddr_in getPeerAddr(int sockfd)
	{
		struct sockaddr_in peeraddr;
		bzero(&peeraddr, sizeof peeraddr);
		socklen_t addrlen = static_cast<socklen_t>(sizeof peeraddr);
		if (::getpeername(sockfd, (struct sockaddr*)(&peeraddr), &addrlen) < 0) {
			LOG_SYSTEM_ERR << "sockets::getPeerAddr";
		}
		return peeraddr;
	}

	bool bindAddress(socket_t sockfd, const NetAddress& localaddr)
	{
		int ret = bind(sockfd, (struct sockaddr *)&localaddr.m_addr, sizeof(localaddr.m_addr));
		if(ret == SOCKET_ERROR) {
			LOG_SYSTEM_ERR << "bindAddress failed address=<" << localaddr.toIpPort() << ">";
			return false;
		}

		return true;
	}

	bool listen(socket_t sockfd)
	{
		// 此处记得还需要在/etc/sysctl.conf添加内核参数:
		// net.core.somaxconn = 51200（accept queue则由somaxconn决定，listen(fd, backlog)中的backlog上限也由somaxconn决定）
		// 然后执行sysctl -p
		int ret = ::listen(sockfd, 51200);
		// int ret = ::listen(sockfd, SOMAXCONN);
		if (ret < 0) {
			LOG_SYSTEM_ERR << "sockets::listen fail, fd = " << sockfd;
			return false;
		}

		return true;
	}

	int connect(socket_t sockfd, NetAddress &peerAddr)
	{
		const struct sockaddr_in &addr = peerAddr.getSockAddr();

		int ret = ::connect(sockfd, (struct sockaddr *)&addr, static_cast<socklen_t>(sizeof addr));
		return ret;
	}
}