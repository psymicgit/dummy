
//< @file:   server\net\net.cpp
//< @author: 洪坤安
//< @date:   2014年11月26日 10:48:35
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#include "net.h"

#include "tool/sockettool.h"
#include "tool/atomictool.h"
#include "listener.h"
#include "link.h"

#include <iostream>

#ifndef WIN

#define EPOLL_EVENTS_SIZE  1024

//! 毫秒
#define EPOLL_WAIT_TIME    10

Epoll::Epoll()
	: m_running(true)
	, m_efd(-1)
	  // , m_curFdCount(0)
{
	// 创建一个epoll的句柄
	m_efd = ::epoll_create(1);

	m_pipe[0] = -1;
	m_pipe[1] = -1;

	int err = ::socketpair(AF_LOCAL, SOCK_STREAM, 0, m_pipe);
	assert(0 == err);
	if (err) {
		LOG_SYSTEM_ERR << "socketpair failed";
	}

	struct epoll_event ee = { 0, { 0 } };
	ee.data.ptr  = this;
	ee.events    = EPOLLIN | EPOLLPRI | EPOLLOUT | EPOLLHUP | EPOLLET;;
	::epoll_ctl(m_efd, EPOLL_CTL_ADD, m_pipe[0], &ee);

	// 如果客户端关闭套接字close，而服务器调用一次write, 服务器会接收一个RST segment（tcp传输层）
	// 如果服务器端再次调用了write，这个时候就会产生SIGPIPE信号，默认终止进程。
	// 这里直接在程序中直接忽略掉SIGPIPE信号
	signal(SIGPIPE, SIG_IGN);
}

bool Epoll::init(int initLinkCount, int linkGrowCount)
{
	m_linkPool.init(initLinkCount, linkGrowCount);
	return true;
}

Epoll::~Epoll()
{
	m_linkPool.clear();

	::close(m_pipe[0]);
	::close(m_pipe[1]);
	::close(m_efd);
	m_efd = -1;
}

void Epoll::stop()
{
	recycleFds();

	LOG_WARN << "	<link pool size = " << m_linkPool.m_totalSize << ", remain size = " << m_linkPool.size() << ", growSize = " << m_linkPool.m_growSize << ">";
	LOG_WARN << "close net successful";
}

int Epoll::eventLoop()
{
	struct epoll_event ev_set[EPOLL_EVENTS_SIZE];

	int waitTime = EPOLL_WAIT_TIME;
	do {
		waitTime = EPOLL_WAIT_TIME;

		// LOG_WARN << "waitTime = " << waitTime;
		int nfds = ::epoll_wait(m_efd, ev_set, EPOLL_EVENTS_SIZE, waitTime);

		bool running = true;

		for (int i = 0; i < nfds; ++i) {
			epoll_event& cur_ev = ev_set[i];
			if (cur_ev.data.ptr == this) {
				continue;
			}

			IFd* pfd = (IFd*)cur_ev.data.ptr;

			if (cur_ev.events & (EPOLLIN | EPOLLPRI)) {
				pfd->handleRead();
			}

			if(cur_ev.events & EPOLLOUT) {
				pfd->handleWrite();
			}

			if (cur_ev.events & (EPOLLERR | EPOLLHUP)) {
				pfd->handleError();
			}
		}

		m_tasks.run();
		waitTime = m_timers.run();
	} while(m_running);

	stop();
	return 0;
}

void Epoll::close()
{
	m_tasks.put(boost::bind(&Epoll::closing, this));
}

void Epoll::closing()
{
	LOG_WARN << "closing net...";
	// LOG_WARN << "	<link count = " << m_curFdCount << ", timer size = " << m_timers.size() << ">";

	m_running = false;
	interruptLoop();
}

int Epoll::interruptLoop()
{
	struct epoll_event ee = { 0, { 0 } };
	ee.data.ptr  = this;
	ee.events    = EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLHUP | EPOLLET;;

	return ::epoll_ctl(m_efd, EPOLL_CTL_MOD, m_pipe[0], &ee);
}

void Epoll::addFd(IFd* pfd)
{
	// LOG_INFO << "add fd " << pfd->socket();

	struct epoll_event ee = { 0, { 0 } };
	ee.data.ptr  = pfd;
	ee.events    = EPOLLERR | EPOLLPRI | EPOLLHUP | EPOLLET;;

	pfd->m_events = ee.events;
	::epoll_ctl(m_efd, EPOLL_CTL_ADD, pfd->socket(), &ee);

	// atomictool::inc(&m_curFdCount);
}

void Epoll::delFd(IFd* pfd)
{
	m_tasks.put(boost::bind(&IFd::erase, pfd));
}

void Epoll::reopen(IFd* pfd)
{
	mod(pfd, (uint16)(EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLHUP | EPOLLET));
}

void Epoll::enableRead(IFd *pfd)
{
	mod(pfd, pfd->m_events | EPOLLIN);
}

void Epoll::enableWrite(IFd *pfd)
{
	mod(pfd, pfd->m_events | EPOLLOUT);
}

void Epoll::enableAll(IFd *pfd)
{
	mod(pfd, pfd->m_events | EPOLLIN | EPOLLOUT);
}

void Epoll::disableRead(IFd *pfd)
{
	mod(pfd, pfd->m_events & ~EPOLLIN);
}

void Epoll::disableWrite(IFd *pfd)
{
	mod(pfd, pfd->m_events & ~EPOLLOUT);
}

void Epoll::disableAll(IFd *pfd)
{
	if (pfd->socket() > 0) {
		// 这里通过取消监听来达到disableAll的效果，因为epoll默认监听EPOLLERR事件
		// 不取消监听的话，当发生EPOLLERR错误时，EPOLLERR事件将无限循环触发
		struct epoll_event ee;

		ee.data.ptr  = (void*)0;
		::epoll_ctl(m_efd, EPOLL_CTL_DEL, pfd->socket(), &ee);
	}
}

void Epoll::mod(IFd *pfd, uint32 events)
{
	if (pfd->m_events == events) {
		return;
	}

	pfd->m_events = events;

	struct epoll_event ee = { 0, { 0 } };
	ee.data.ptr  = pfd;
	ee.events    = events;

	::epoll_ctl(m_efd, EPOLL_CTL_MOD, pfd->socket(), &ee);
}

void Epoll::recycleFds()
{
	FdList delFds;

	{
		lock_guard_t<> lock(m_mutex);
		delFds.swap(m_deletingFdList);
	}

	for(size_t i = 0; i < delFds.size(); ++i) {
		IFd *pfd = delFds[i];
		pfd->erase();
	}
}

#else

Select::Select()
	: m_maxfd(0)
{
	// windows下需先执行WSAStartup进行初始化
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	// 初始化fd_set
	FD_ZERO(&m_rset);
	FD_ZERO(&m_wset);
	FD_ZERO(&m_eset);

	m_running = true;
}

bool Select::init(int initLinkCount, int linkGrowCount)
{
	m_linkPool.init(initLinkCount, linkGrowCount);
	m_links.reserve(initLinkCount);
	return true;
}

void Select::addFd(IFd *pfd)
{
	m_tasks.put(boost::bind(&Select::updateFd, this, pfd, FD_ADD));
}

void Select::delFd(IFd *pfd)
{
	m_tasks.put(boost::bind(&Select::updateFd, this, pfd, FD_DEL));
}

void Select::enableRead(IFd *pfd)
{
	m_tasks.put(boost::bind(&Select::updateFd, this, pfd, FD_ENABLE_READ));
}

void Select::enableWrite(IFd *pfd)
{
	m_tasks.put(boost::bind(&Select::updateFd, this, pfd, FD_ENABLE_WRITE));
}

void Select::enableAll(IFd *pfd)
{
	m_tasks.put(boost::bind(&Select::updateFd, this, pfd, FD_ENABLE_ALL));
}

void Select::disableRead(IFd *pfd)
{
	m_tasks.put(boost::bind(&Select::updateFd, this, pfd, FD_DISABLE_READ));
}

void Select::disableWrite(IFd *pfd)
{
	m_tasks.put(boost::bind(&Select::updateFd, this, pfd, FD_DISABLE_WRITE));
}

void Select::disableAll(IFd *pfd)
{
	m_tasks.put(boost::bind(&Select::updateFd, this, pfd, FD_DISABLE_ALL));
}

void Select::updateFd(IFd *pfd, FDOperator op)
{
	socket_t fd = pfd->socket();

	switch(op) {
	case FD_ADD:
		if (fd > m_maxfd) {
			m_maxfd = fd;
		}

		m_links.push_back(pfd);
		break;

	case FD_DEL:
		for (size_t i = 0; i < m_links.size(); i++) {
			IFd *link = m_links[i];

			if (pfd == link) {
				m_links[i] = m_links.back();
				m_links.pop_back();
				break;
			}
		}

		// m_links.erase(remove(m_links.begin(), m_links.end(), pfd), m_links.end());
		pfd->erase();
		break;

	case FD_ENABLE_READ:
		FD_SET(fd, &m_rset);
		FD_SET(fd, &m_eset);
		break;

	case FD_DISABLE_READ:
		FD_CLR(fd, &m_rset);

		if (!FD_ISSET(fd, &m_wset)) {
			FD_CLR(fd, &m_eset);
		}
		break;

	case FD_ENABLE_WRITE:
		FD_SET(fd, &m_wset);
		FD_SET(fd, &m_eset);
		break;

	case FD_DISABLE_WRITE:
		FD_CLR(fd, &m_wset);

		if (!FD_ISSET(fd, &m_rset)) {
			FD_CLR(fd, &m_eset);
		}
		break;

	case FD_ENABLE_ALL:
		FD_SET(fd, &m_wset);
		FD_SET(fd, &m_rset);
		FD_SET(fd, &m_eset);
		break;

	case FD_DISABLE_ALL:
		FD_CLR(fd, &m_rset);
		FD_CLR(fd, &m_wset);
		FD_CLR(fd, &m_eset);
		break;
	}
}

int Select::eventLoop()
{
	int n = 0;
	size_t i = 0;
	fd_set rset;
	fd_set wset;
	fd_set eset;

	//这里我们打算让select等待50豪秒后返回，避免被锁死，也避免马上返回
	struct timeval tv = {0, 50};

	while(m_running) {
		rset = m_rset;
		wset = m_wset;
		eset = m_eset;

		n = select(m_maxfd + 1, &rset, &wset, &eset, &tv);
		// 检测到异常
		if(n < 0) {
			//LOG_SOCKET_ERR << "select()";
			if(EINTR == errno) {
				LOG_DEBUG << "get EINTR. We continue.";
				continue;
			}

			//return 4;
		}
		// 检测到网络事件
		else if(n) {
			int readyfds = n;

			for(i = 0; i < m_links.size(); ++i) {
				IFd *pfd = m_links[i];
				int fd = pfd->socket();

				if(FD_ISSET(fd, &rset)) {
					pfd->handleRead();
					if(--readyfds == 0) break;
				}
				if(FD_ISSET(fd, &wset)) {
					pfd->handleWrite();
					if(--readyfds == 0) break;
				}
				if(FD_ISSET(fd, &eset)) {
					pfd->handleError();
					if(--readyfds == 0) break;
				}
			}

			if(readyfds != 0) {
				LOG_DEBUG << "Are there any fd left?";
			}
		}
		// 超时
		else {
			// LOG_DEBUG << "Timeout.";
		}

		m_tasks.run();
		m_timers.run();
	}

	return 0;
}

void Select::close()
{
	for(size_t i = 0; i < m_links.size(); ++i) {
		IFd *pfd = m_links[i];
		pfd->close();
	}

	m_tasks.put(boost::bind(&Select::closing, this));
}

void Select::closing()
{
	LOG_WARN << "closing net...";
	LOG_INFO << "	<link count = " << m_links.size() << ", task size = " << m_tasks.size() << ", timer size = " << m_timers.size() << ">";
	LOG_WARN << "	<link pool size = " << m_linkPool.m_totalSize << ", remain size = " << m_linkPool.size() << ", growSize = " << m_linkPool.m_growSize << ">";

	FD_ZERO(&m_rset);
	FD_ZERO(&m_wset);
	FD_ZERO(&m_eset);

	m_running = false;
	m_links.clear();
	m_linkPool.clear();
	LOG_WARN << "close net successful";
}

#endif
