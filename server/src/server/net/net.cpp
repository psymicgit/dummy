
//< @file:   server\net\net.cpp
//< @author: 洪坤安
//< @date:   2014年11月26日 10:48:35
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#include "net.h"

#include "tool/sockettool.h"
#include "listener.h"
#include "link.h"

#ifndef WIN

#define CREATE_EPOLL_SIZE  4096
#define EPOLL_EVENTS_SIZE  128
//! 100 ms
#define EPOLL_WAIT_TIME    100

Epoll::Epoll()
	: m_running(true)
	, m_efd(-1)
{
	m_efd = ::epoll_create(CREATE_EPOLL_SIZE);
	m_interupt_sockets[0] = -1;
	m_interupt_sockets[1] = -1;
	assert( 0 == ::socketpair(AF_LOCAL, SOCK_STREAM, 0, m_interupt_sockets));
	struct epoll_event ee = { 0, { 0 } };
	ee.data.ptr  = this;
	ee.events    = EPOLLIN | EPOLLPRI | EPOLLOUT | EPOLLHUP | EPOLLET;;
	::epoll_ctl(m_efd, EPOLL_CTL_ADD, m_interupt_sockets[0], &ee);
}

Epoll::~Epoll()
{
	::close(m_interupt_sockets[0]);
	::close(m_interupt_sockets[1]);
	::close(m_efd);
	m_efd = -1;
}

int Epoll::eventLoop()
{
	int i = 0, nfds = 0;
	struct epoll_event ev_set[EPOLL_EVENTS_SIZE];

	do {
		nfds  = ::epoll_wait(m_efd, ev_set, EPOLL_EVENTS_SIZE, EPOLL_WAIT_TIME);

		if (nfds < 0 && EINTR == errno) {
			nfds = 0;
			continue;
		}
		for (i = 0; i < nfds; ++i) {
			epoll_event& cur_ev = ev_set[i];
			IFd* fd_ptr	    = (IFd*)cur_ev.data.ptr;
			if (cur_ev.data.ptr == this) { //! iterupte event
				if (false == m_running) {
					return 0;
				}

				//! 删除那些已被标记为垃圾的socket对象
				recycleFds();
				continue;
			}

			if (cur_ev.events & (EPOLLIN | EPOLLPRI)) {
				fd_ptr->handleRead();
			}

			if(cur_ev.events & EPOLLOUT) {
				fd_ptr->handleWrite();
			}

			if (cur_ev.events & (EPOLLERR | EPOLLHUP)) {
				fd_ptr->handleError();
			}
		}

		m_tasks.run();
		m_timers.run();
	}
	while(nfds >= 0);

	return 0;
}

void Epoll::close()
{
	m_running = false;

	interruptLoop();
}

int Epoll::interruptLoop()
{
	struct epoll_event ee = { 0, { 0 } };

	ee.data.ptr  = this;
	ee.events    = EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLHUP | EPOLLET;;

	return ::epoll_ctl(m_efd, EPOLL_CTL_MOD, m_interupt_sockets[0], &ee);
}

void Epoll::addFd(IFd* pfd)
{
	// LOG_INFO << "add fd " << pfd->socket();

	struct epoll_event ee = { 0, { 0 } };

	ee.data.ptr  = pfd;
	ee.events    = EPOLLERR | EPOLLPRI | EPOLLHUP | EPOLLET;;

	pfd->m_events = ee.events;
	::epoll_ctl(m_efd, EPOLL_CTL_ADD, pfd->socket(), &ee);
}

void Epoll::delFd(IFd* pfd)
{
	disableAll(pfd);

	{
		lock_guard_t<> lock(m_mutex);
		m_deletingFdList.push_back(pfd);
	}

	interruptLoop();
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

void Epoll::mod(IFd *pfd, uint16 events)
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
	lock_guard_t<> lock(m_mutex);
	list<IFd*>::iterator it = m_deletingFdList.begin();
	for (; it != m_deletingFdList.end(); ++it) {
		// LOG_INFO << "add fd " << (*it)->socket();
		delete *it;
	}

	m_deletingFdList.clear();
}

#else

Select::Select()
	: m_maxfd(0)
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	// 初始化fd_set
	FD_ZERO(&m_rset);
	FD_ZERO(&m_wset);
	FD_ZERO(&m_eset);
}

void Select::addFd(IFd *pfd)
{
	m_tasks.put(task_binder_t::gen(&Select::updateFd, this, pfd, FD_ADD));
}

void Select::delFd(IFd *pfd)
{
	m_tasks.put(task_binder_t::gen(&Select::updateFd, this, pfd, FD_DEL));
}

void Select::enableRead(IFd *pfd)
{
	m_tasks.put(task_binder_t::gen(&Select::updateFd, this, pfd, FD_ENABLE_READ));
}

void Select::enableWrite(IFd *pfd)
{
	m_tasks.put(task_binder_t::gen(&Select::updateFd, this, pfd, FD_ENABLE_WRITE));
}

void Select::enableAll(IFd *pfd)
{
	m_tasks.put(task_binder_t::gen(&Select::updateFd, this, pfd, FD_ENABLE_ALL));
}

void Select::disableRead(IFd *pfd)
{
	m_tasks.put(task_binder_t::gen(&Select::updateFd, this, pfd, FD_DISABLE_READ));
}

void Select::disableWrite(IFd *pfd)
{
	m_tasks.put(task_binder_t::gen(&Select::updateFd, this, pfd, FD_DISABLE_WRITE));
}

void Select::disableAll(IFd *pfd)
{
	m_tasks.put(task_binder_t::gen(&Select::updateFd, this, pfd, FD_DISABLE_ALL));
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

		updateFd(pfd, FD_DISABLE_ALL);
		// m_links.erase(remove(m_links.begin(), m_links.end(), pfd), m_links.end());
		delete pfd;
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

	//这里我们打算让select等待两秒后返回，避免被锁死，也避免马上返回
	struct timeval tv = {2, 0};

	while(true) {
		rset = m_rset;
		wset = m_wset;
		eset = m_eset;

		n = select(m_maxfd + 1, &rset, &wset, &eset, &tv);
		// 检测到异常
		if(n < 0) {
			//LOG_SOCKET_ERR << "select()";
			if(EINTR == errno) {
				LOG_DEBUG << "get EINTR. We cotinue.";
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
				else if(FD_ISSET(fd, &wset)) {
					// LOG_DEBUG << "write fds";
					pfd->handleWrite();
					if(--readyfds == 0) break;
				}
				else if(FD_ISSET(fd, &eset)) {
					LOG_DEBUG << "exception fd";
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

#endif
