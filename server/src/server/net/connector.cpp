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
#include "net/netfactory.h"
#include "net/link.h"
#include "basic/timerqueue.h"
#include "basic/taskqueue.h"

Connector::Connector(NetAddress &peerAddr, INetReactor *netReactor, NetModel *net, const char* remoteHostName, NetFactory *pNetFactory)
	: m_peerAddr(peerAddr)
	, m_logic(netReactor)
	, m_net(net)
	, m_retryDelayMs(InitRetryDelayMs)
	, m_state(StateDisconnected)
	, m_errno(0)
	, m_remoteHostName(remoteHostName)
	, m_netNetFactory(pNetFactory)
{
	m_sockfd = socktool::createSocket();
	socktool::setNonBlocking(m_sockfd);
	//socktool::setTcpNoDelay(m_sockfd);
}

bool Connector::open()
{
	m_sockfd = socktool::createSocket();
	if (m_sockfd <= 0) {
		return false;
	}

	if (!socktool::setNonBlocking(m_sockfd)) {
		return false;
	}

	return true;
}

void Connector::connect()
{
	// 直接连接
	// 注意：这里是非阻塞connect，一般会立即返回 EINPROGRESS 错误，表示连接操作正在进行中，但是仍未完成，与此同时 TCP 三次握手操作会同时进行
	// 在这之后，我们可以捕获可写事件以及error事件来检查这个链接是否建立成功

	int ret = ::connect(m_sockfd, (struct sockaddr *)&m_peerAddr.getSockAddr(), static_cast<socklen_t>(sizeof m_peerAddr.getSockAddr()));
	// int ret = socktool::connect(m_sockfd, m_peerAddr);

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
}

void Connector::handleRead()
{
	// 一般是不会接收到读事件的，所以这里打印error
	LOG_ERROR << m_logic->name() << "recv read event, socket = " << m_sockfd;
}

void Connector::handleWrite()
{
	// 先检测套接字是否发生异常
	m_errno = socktool::getSocketError(m_sockfd);
	if (m_errno > 0) {
		// 若检测到异常，则重连
		LOG_SOCKET_ERR(m_sockfd, m_errno) << m_logic->name();
		retry();

		return;
	}

	// 成功建立连接
	onConnected();
}

void Connector::handleError()
{
	// LOG_SYSTEM_ERR << m_pNetReactor->name() << " socket<" << m_sockfd << ">";
	retry();
}

void Connector::close()
{
	//LOG_INFO << "Connector::close, socket = " << m_sockfd;
	m_net->disableAll(this);
	m_net->delFd(this);
}

void Connector::erase()
{
	delete this;
}

bool Connector::onConnected()
{
	// 成功连接上对端
	Link* link = createLink(m_sockfd, m_peerAddr);
	if (NULL == link) {
		LOG_ERROR << m_logic->name() << " connector create link failed, socket = " << m_sockfd;
		this->close();
		socktool::closeSocket(m_sockfd);
		return false;
	}

	this->close();

	link->open();

	TaskQueue::TaskList taskList;

	// 将连接成功的消息投到业务层
	taskList.push_back(boost::bind(&INetReactor::onConnected, m_logic, link, link->getLocalAddr(), m_peerAddr));

	// 等业务层处理完新连接后，才允许该连接开始读
	taskList.push_back(boost::bind(&NetModel::enableRead, link->m_net, link));

	m_logic->getTaskQueue().put(taskList);
	return true;
}

bool Connector::connecting()
{
	if (m_state == StateDisconnected) {
		// 将本连接器注册到网络
		m_net->addFd(this);

		// 注册写事件
		m_net->enableWrite(this);

		m_state = StateConnecting;
	} else if(m_state == StateConnecting) {
		// 重新注册写事件（但不需要将本连接器重新注册到网络）
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

	LOG_SOCKET_ERR(m_sockfd, m_errno) << m_logic->name() << " connect to " << m_remoteHostName << "<" << m_peerAddr.toIpPort() << "> fail, retry after <" << m_retryDelayMs << "> ms";
	m_errno = 0;

	if (m_state == StateConnecting) {
		// 屏蔽所有事件，防止无限触发exception
		m_net->disableAll(this);

#ifndef WIN
		m_state = StateDisconnected;
#endif
	}

	// 设置定时任务：隔一段时间后重新连接
	TimerQueue &timerQueue = m_net->getTimerQueue();
	timerQueue.runAfter(boost::bind(&Connector::connect, this), m_retryDelayMs);

	m_net->interruptLoop();

	// 重连时间 = 重连时间 * 2
	m_retryDelayMs = MIN(m_retryDelayMs + InitRetryDelayMs, MaxRetryDelayMs);
	return true;
}

Link* Connector::createLink(socket_t newfd, NetAddress &peerAddr)
{
	NetModel *net = m_netNetFactory->nextNet();

	LinkPool &linkPool = net->getLinkPool();
	NetAddress localAddr(socktool::getLocalAddr(newfd));

	Link *link = linkPool.alloc(newfd, localAddr, peerAddr, net, m_logic);
	if (NULL == link) {
		return NULL;
	}

	link->m_isAutoReconnect = true;
	return link;
}