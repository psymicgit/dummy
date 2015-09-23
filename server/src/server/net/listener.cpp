///<------------------------------------------------------------------------------
//< @file:   server\net\listener.cpp
//< @author: 洪坤安
//< @date:   2014年11月25日 19:14:38
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#include "listener.h"

#include "tool/sockettool.h"
#include "net/netaddress.h"
#include "net/link.h"
#include "net/netreactor.h"
#include "net/netfactory.h"

Listener::Listener(NetModel *pNet, INetReactor *pNetReactor, NetFactory *pNetFactory)
	: m_net(pNet)
	, m_pNetReactor(pNetReactor)
	, m_listenFd(0)
	, m_netNetFactory(pNetFactory)
{

}

bool Listener::open(const string & ip, int port)
{
	LOG_DEBUG << m_pNetReactor->name() << " start listening at <" << ip << ": " << port << ">";

	m_listenAddr = NetAddress(ip, port);

	m_listenFd = socktool::createSocket();
	if (m_listenFd <= 0) {
		return false;
	}

	// 设置端口复用
	if(!socktool::setReuseAddr(m_listenFd, true)) {
		return false;
	}

	// 设置为非阻塞
	if(!socktool::setNonBlocking(m_listenFd)) {
		return false;
	}

	if(!socktool::bindAddress(m_listenFd, m_listenAddr)) {
		return false;
	}

	// 开始监听
	if(!socktool::listen(m_listenFd)) {
		return false;
	}

	// 将本监听器注册到网络
	m_net->addFd(this);

	// 注册可读事件：对于非阻塞listen，当接收到新连接时，描述符变成可读
	m_net->enableRead(this);
	return true;
}

void Listener::close()
{
	LOG_DEBUG << m_pNetReactor->name() << " stop listening at <" << m_listenAddr.toIpPort() << ">";

	socktool::closeSocket(m_listenFd);
	m_net->delFd(this);
}

void Listener::erase()
{
	delete this;
}

void Listener::handleRead()
{
	sockaddr addr;
	socklen_t addrlen = sizeof(addr);

	// 循环接收新连接直到连接队列为空
	int newfd = -1;
	do {
		// 注意：这里首先要知道建立socket连接的过程：
		// 	1.client发syn请求给server
		// 	2.server收到后把请求存放在SYN queue里，这个半连接队列的最大值是系统参数tcp_max_syn_backlog定义的
		// 	3.存放在半连接队列后发送syn+ack给client，client收到后再发syn+ack给server完成三次握手，然后server把连接存放在accept queue，这个队列长度就是程序里调用socket的时候定义的backlog定义大小。
		// 	4.应用程序通过调用accept()到accept queue里获取连接。

		// 所以在accept这里要注意，需要修改内核参数，否则当大量客户端同时连接时，会导致服务器的accept队列溢出，使得客户端误以为成功建立连接，但服务端却accept不到连接
		// 需要在/etc/sysctl.conf添加内核参数如下:
		// net.core.somaxconn = 51200（accept queue则由somaxconn决定，listen(fd, backlog)中的backlog上限也由somaxconn决定）
		// net.ipv4.tcp_max_syn_backlog = 512（SYN queue长度由tcp_max_syn_backlog指定）
		// net.ipv4.tcp_abort_on_overflow = 1（如果accept队列已满，则会发出拒绝客户端的终止讯息并断开连接，否则对于溢出队列的连接，linux仍会继续正常建立连接，但却无法accept取得）
		// 然后执行sysctl -p

		// 如何查看accept queue溢出
		// netstat -s | grep LISTEN 对比前后是否有增长
		if ((newfd = ::accept(m_listenFd, (struct sockaddr *)&addr, &addrlen)) == -1) {
			int err = socktool::geterrno();
			// LOG_ERROR<< "err = " << err;
			if (err == EAGAIN || err == EWOULDBLOCK) {
				// 在non-blocking模式下，如果accept返回值为-1，且errno == EAGAIN或errno == EWOULDBLOCK表示没有新连接请求
				break;
			} else if (err == ECONNABORTED && err == EPROTO && err == EINTR) {
				LOG_ERROR << "errno == " << err;
				continue;
			} else {
				LOG_SOCKET_ERR(m_listenFd, err) << m_pNetReactor->name() << " accept failed, restart listenning now";
				//! if too many open files occur, need to restart epoll event
				m_net->reopen(this);
				break;
			}
		}

		const struct sockaddr_in *addr_in = (sockaddr_in*)&addr;
		NetAddress peerAddr(*addr_in);

		Link* link = createLink(newfd, peerAddr);
		if (NULL == link) {
			socktool::closeSocket(newfd);

			LOG_ERROR << m_pNetReactor->name() << " listener create link failed, new socket = " << newfd;
			break;
		}

		link->open();

		TaskQueue::TaskList taskList;

		// 将接收到新连接的消息投到业务层
		taskList.push_back(boost::bind(&INetReactor::onAccepted, m_pNetReactor, link, m_listenAddr, peerAddr));

		// 等业务层处理完新连接后，才允许该连接开始读
		taskList.push_back(boost::bind(&NetModel::enableRead, link->m_net, link));

		m_pNetReactor->getTaskQueue().put(taskList);
	} while (true);
}

void Listener::handleWrite()
{
	LOG_ERROR << m_pNetReactor->name();
}

void Listener::handleError()
{
	LOG_ERROR << m_pNetReactor->name();
}

Link* Listener::createLink(socket_t newfd, NetAddress &peerAddr)
{
	NetModel *net = m_netNetFactory->nextNet();

	LinkPool &linkPool = net->getLinkPool();
	return linkPool.alloc(newfd, m_listenAddr, peerAddr, net, m_pNetReactor);
}
