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
{
	m_sockfd = socktool::createSocket();
	socktool::setNonBlocking(m_sockfd);
}

bool Connector::connect()
{
	int ret = socktool::connect(m_sockfd, m_peerAddr.getSockAddr());

	int err = ((ret == SOCKET_ERROR) ? socktool::geterrno() : 0);
	switch (err) {
	case 0:
		// 连接成功（当客户端和服务器端在同一台主机上的时候，connect会马上返回0）
		//onConnected();
		break;

	case EWOULDBLOCK:
	case EINPROGRESS:
	case EINTR:
	case EISCONN:
		// 正在连接（tcp三次握手正在进行）
		connecting(m_sockfd);
		break;

#ifdef WIN
	// linux下的EAGAIN和EWOULDBLOCK是同一个值，编译会报错
	case EAGAIN:
#endif
	case EADDRINUSE:
	case EADDRNOTAVAIL:
	case ECONNREFUSED:
	case ENETUNREACH:
		// 连接失败，重试
		logError();
		retry(m_sockfd);
		break;

	case EACCES:
	case EPERM:
	case EAFNOSUPPORT:
	case EALREADY:
	case EBADF:
	case EFAULT:
	case ENOTSOCK:
		// 连接异常，中断
		logError();
		retry(m_sockfd);
		break;

	default:
		// 发生未识别异常
		LOG_SOCKET_ERR << "socket<" << m_sockfd << "> connect to peer<" << m_peerAddr.toIpPort() << "> fail, unexpected error";
		this->close();
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
	int err = socktool::getSocketError(m_sockfd);
	if (err) {
		LOG_SOCKET_ERR << "Connector::handleWrite - SOCK_ERROR = " << err;
		retry(m_sockfd);

		return -1;
	}

	// 成功建立连接
	onConnected();
	return 0;
}

int Connector::handleError()
{
	logError();

	retry(m_sockfd);
	return true;
}

void Connector::logError()
{
	LOG_ERROR << "socket<" << m_sockfd << "> connect to peer<" << m_peerAddr.toIpPort() << "> fail, retry after <" << m_retryDelayMs << "> ms";
}

void Connector::close()
{
	m_net->getTaskQueue().put(task_binder_t::gen(&INet::disableAll, m_net, m_sockfd));
	m_net->getTaskQueue().put(task_binder_t::gen(&INet::delFd, m_net, this));
}


void OpenLink(Link *link)
{
	link->open();
}

bool Connector::onConnected()
{
	this->close();

	// 成功连接上对端
	Link* link = createLink(m_sockfd, m_peerAddr);

	m_pNetReactor->GetTaskQueue().put(task_binder_t::gen(&INetReactor::OnConnected, m_pNetReactor, link, link->m_localAddr, m_peerAddr));
	m_pNetReactor->GetTaskQueue().put(task_binder_t::gen(&OpenLink, link));

	return true;
}

bool Connector::connecting(socket_t sockfd)
{
	if (m_state == kDisconnected) {
		//m_net->registerFd(this);

		// LOG_DEBUG << "INet::registerFd addr = " << this;

		m_net->getTaskQueue().put(task_binder_t::gen(&INet::addFd, m_net, this));
		m_net->getTaskQueue().put(task_binder_t::gen(&INet::enableWrite, m_net, this));

		m_state = kConnecting;
	}
	else if(m_state == kConnecting) {
		m_net->getTaskQueue().put(task_binder_t::gen(&INet::enableWrite, m_net, this));
	}

	return true;
}

bool Connector::retry(socket_t sockfd)
{
	m_net->getTaskQueue().put(task_binder_t::gen(&INet::disableAll, m_net, m_sockfd));

	TimerQueue &timerQueue = m_net->getTimerQueue();
	timerQueue.runAfter(task_binder_t::gen(&Connector::connect, this), m_retryDelayMs);

	m_retryDelayMs = MIN(m_retryDelayMs * 2, MaxRetryDelayMs);
	return true;
}

Link* Connector::createLink(socket_t newfd, NetAddress &peerAddr)
{
	NetAddress localAddr(socktool::getLocalAddr(m_sockfd));
	Link *link = new Link(newfd, localAddr, peerAddr, m_taskQueuePool->alloc(newfd), m_net, m_pNetReactor);
	link->m_isCreateByConnector = true;
	return link;
}