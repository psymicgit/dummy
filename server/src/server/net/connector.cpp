///<------------------------------------------------------------------------------
//< @file:   server\net\connector.cpp
//< @author: 洪坤安
//< @date:   2015年3月4日 12:26:11
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "connector.h"

#include <stdlib.h>

#include "tool/sockettool.h"
#include "net/netaddress.h"
#include "net/netreactor.h"
#include "net/link.h"
#include "basic/timerqueue.h"
#include "basic/taskqueue.h"

Connector::Connector(NetAddress &peerAddr, INetReactor *netReactor, INet *net, task_queue_pool_t *taskQueuePool)
	: m_peerAddr(peerAddr)
	, m_pNetReactor(netReactor)
	, m_net(net)
	, m_taskQueuePool(taskQueuePool)
	, m_retryDelayMs(InitRetryDelayMs)
	, m_state(kDisconnected)
	, m_errno(0)
{
	m_sockfd = socktool::createSocket();
	socktool::setNonBlocking(m_sockfd);
	socktool::setTcpNoDelay(m_sockfd);
}

bool Connector::connect()
{
	int ret = socktool::connect(m_sockfd, m_peerAddr.getSockAddr());

	int err = ((ret == SOCKET_ERROR) ? socktool::geterrno() : 0);
	switch (err) {
	case 0:
		// 连接成功（当客户端和服务器端在同一台主机上的时候，connect会马上返回0）
		LOG_WARN << "warn: socket<" << m_sockfd << "> connect to localhost<" << m_peerAddr.toIpPort() << "> success";
		onConnected();
		break;

#ifdef WIN
	// linux下的EAGAIN和EWOULDBLOCK是同一个值，编译会报错
	case EAGAIN:
#endif
	case EWOULDBLOCK:
	case EINTR:
	case EISCONN:
	case EINPROGRESS:
		// LOG_WARN << "warn: socket <" << m_sockfd << "> starting connecting err = <" << err << ">";

		// 正在连接（tcp三次握手正在进行）
		connecting();
		break;

	case EADDRINUSE:
	case EADDRNOTAVAIL:
	case ECONNREFUSED:
	case ENETUNREACH:
		// 连接失败，重试
		retry();
		break;

	case EACCES:
	case EPERM:
	case EAFNOSUPPORT:
	case EALREADY:
	case EBADF:
	case EFAULT:
	case ENOTSOCK:
		// 连接异常，中断
		retry();
		break;

	default:
		// 发生未识别异常
		// LOG_SOCKET_ERR << "socket<" << m_sockfd << "> connect to peer<" << m_peerAddr.toIpPort() << "> fail, unexpected error = " << err;
		retry();
		break;
	}

	return true;
}

int Connector::handleRead()
{
	return handleWrite();
}

int Connector::handleWrite()
{
	// 先检测套接字是否发生异常
	m_errno = socktool::getSocketError(m_sockfd);
	if (m_errno > 0) {
		// LOG_SYSTEM_ERR << "Connector::handleWrite - SOCK_ERROR = " << err;
		retry();

		return -1;
	}

	// 成功建立连接

	onConnected();
	return 0;
}

int Connector::handleError()
{
	// LOG_ERROR << "Connector::handleError socket<" << m_sockfd << ">";
	retry();
	return true;
}

void Connector::close()
{
	m_net->delFd(this);
}

void OpenLink(Link *link)
{
	link->open();
}

bool Connector::onConnected()
{
	// 成功连接上对端
	Link* link = createLink(m_sockfd, m_peerAddr);
	this->close();

	link->open();
	m_pNetReactor->getTaskQueue().put(task_binder_t::gen(&INetReactor::onConnected, m_pNetReactor, link, link->m_localAddr, m_peerAddr));
	m_pNetReactor->getTaskQueue().put(task_binder_t::gen(&Link::enableRead, link));

	return true;
}

bool Connector::connecting()
{
	if (m_state == kDisconnected) {
		m_net->addFd(this);
		m_net->enableWrite(this);

		m_state = kConnecting;
	}
	else if(m_state == kConnecting) {
		m_net->enableWrite(this);
	}

	return true;
}

bool Connector::retry()
{
	// 注意：这里打印的错误码仅供参考，因为非阻塞式的connect很难获取到实际的连接失败原因
	if (0 == m_errno) {
		m_errno = socktool::getSocketError(m_sockfd);

		if (0 == m_errno) {
			m_errno = socktool::geterrno();
		}
	}

	LOG_SOCKET_ERR(m_sockfd, m_errno) << "socket<" << m_sockfd << "> connect to peer<" << m_peerAddr.toIpPort() << "> fail, retry after <" << m_retryDelayMs << "> ms";
	m_errno = 0;

	if (m_state == kConnecting) {
		m_net->disableAll(this);

#ifndef WIN
		m_state = kDisconnected;
#endif
	}

	TimerQueue &timerQueue = m_net->getTimerQueue();
	timerQueue.runAfter(task_binder_t::gen(&Connector::connect, this), m_retryDelayMs);

	m_retryDelayMs = MIN(m_retryDelayMs * 2, MaxRetryDelayMs);
	return true;
}

Link* Connector::createLink(socket_t newfd, NetAddress &peerAddr)
{
	NetAddress localAddr(socktool::getLocalAddr(newfd));
	Link *link = new Link(newfd, localAddr, peerAddr, m_taskQueuePool->alloc(newfd), m_net, m_pNetReactor);
	link->m_isCreateByConnector = true;
	return link;
}