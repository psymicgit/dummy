///<------------------------------------------------------------------------------
//< @file:   server\tool\sockettool.h
//< @author: 洪坤安
//< @date:   2014年11月25日 14:4:34
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _sockettool_h_
#define _sockettool_h_

#include "net/net.h"

class NetAddress;

namespace socktool
{
	socket_t createSocket();

	void closeSocket(socket_t);

	// 设置socket为非阻塞
	bool setNonBlocking(socket_t);

	// 关闭Nagle算法
	bool setTcpNoDelay(socket_t);

	bool checkNonBlocking(socket_t);

	void setReuseAddr(socket_t, bool on);

	void setReusePort(socket_t, bool on);

	void setKeepAlive(socket_t, bool on, int keepAliveTime);

	// 延缓面向连接的socket的close操作
	void setLinger(socket_t, bool on);

	// 获取socket的错误码并置错误码为0
	int getSocketError(socket_t sockfd);

	const char* getErrorMsg(int err);

	int geterrno();

	struct sockaddr_in getLocalAddr(int sockfd);
	struct sockaddr_in getPeerAddr(int sockfd);

	bool bindAddress(socket_t sockfd, const NetAddress& localaddr);

	bool listen(socket_t);

	socket_t accept(socket_t listensock, NetAddress &peerAddr);

	int connect(int sockfd, const struct sockaddr_in& addr);
}

#endif //_sockettool_h_