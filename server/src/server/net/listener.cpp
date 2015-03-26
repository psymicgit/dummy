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

Listener::Listener(INet *pNet, INetReactor *pNetReactor, task_queue_pool_t *taskQueuePool)
	: m_net(pNet)
	, m_pNetReactor(pNetReactor)
	, m_taskQueuePool(taskQueuePool)
	, m_listenFd(0)
{

}

bool Listener::open(const string& ip, int port)
{
	NetAddress listenAddr(ip, port);
	m_listenAddr = NetAddress(ip, port);

	m_listenFd = socktool::createSocket();

	socktool::setReuseAddr(m_listenFd, true);
	socktool::setNonBlocking(m_listenFd);

	if(!socktool::bindAddress(m_listenFd, m_listenAddr)) {
		return false;
	}

	if(!socktool::listen(m_listenFd)) {
		return false;
	}

	m_net->getTaskQueue().put(task_binder_t::gen(&INet::addFd, m_net, this));
	m_net->getTaskQueue().put(task_binder_t::gen(&INet::enableRead, m_net, this));
	return true;
}

void Listener::close()
{
	if (m_listenFd > 0) {
		m_net->getTaskQueue().put(task_binder_t::gen(&INet::disableAll, m_net, m_listenFd));
		m_net->getTaskQueue().put(task_binder_t::gen(&INet::delFd, m_net, this));
		socktool::closeSocket(m_listenFd);
		m_listenFd = -1;
	}
}

int Listener::handleRead()
{
	struct sockaddr_storage addr;
	socklen_t addrlen = sizeof(addr);

	int newfd = -1;
	do {
		if ((newfd = ::accept(m_listenFd, (struct sockaddr *)&addr, &addrlen)) == -1) {
			if (errno == EWOULDBLOCK) {
				return 0;
			}
			else if (errno == EINTR || errno == EMFILE || errno == ECONNABORTED || errno == ENFILE ||
			         errno == EPERM || errno == ENOBUFS || errno == ENOMEM) {
				perror("accept");//! if too many open files occur, need to restart epoll event
				m_net->reopen(this);
				return 0;
			}

			return -1;
		}

		NetAddress peerAddr(*((struct sockaddr_in*)&addr));

		Link* link = createLink(newfd, peerAddr);
		link->open();

		m_pNetReactor->GetTaskQueue().put(task_binder_t::gen(&INetReactor::OnAccepted, m_pNetReactor, link, m_listenAddr, peerAddr));

		// m_pNetReactor->GetTaskQueue().put(task_binder_t::gen(&Listener::OnAccepted, this, link));
	}
	while (true);
	return 0;
}

void Listener::onAccepted(Link &link)
{
// 	ISession *session = m_sessionFactory->createSession(link);
// 	if (NULL == session) {
// 		link.close();
// 		return;
// 	}
//
// 	session->setLink(link);
// 	session->onEstablish();
}

Link* Listener::createLink(socket_t newfd, NetAddress &peerAddr)
{
	return new Link(newfd, m_listenAddr, peerAddr, m_taskQueuePool->alloc(newfd), m_net, m_pNetReactor);
}
