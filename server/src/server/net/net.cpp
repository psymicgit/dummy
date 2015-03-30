
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

int Epoll::event_loop()
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

				//! 删除那些已经出现error的socket 对象
				fd_del_callback();
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
	LOG_INFO << "add fd" << pfd->socket();

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
		m_error_fd_set.push_back(pfd);
	}

	interruptLoop();
}

void Epoll::reopen(IFd* pfd)
{
	mod(pfd, EPOLLIN | EPOLLOUT | EPOLLPRI | EPOLLHUP | EPOLLET);
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

void Epoll::fd_del_callback()
{
	lock_guard_t<> lock(m_mutex);
	list<IFd*>::iterator it = m_error_fd_set.begin();
	for (; it != m_error_fd_set.end(); ++it) {
		delete *it;
	}

	m_error_fd_set.clear();
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
	socket_t fd = pfd->socket();
	if (fd > m_maxfd) {
		m_maxfd = fd;
	}

	m_links.push_back(pfd);
}

void Select::delFd(IFd *pfd)
{
	for (size_t i = 0; i < m_links.size(); i++) {
		IFd *link = m_links[i];

		if (pfd == link) {
			m_links[i] = m_links.back();
			m_links.pop_back();
			break;
		}
	}

	// m_links.erase(remove(m_links.begin(), m_links.end(), pfd), m_links.end());
	delete pfd;
}

void Select::enableRead(IFd *pfd)
{
	socket_t fd = pfd->socket();
	FD_SET(fd, &m_rset);
	FD_SET(fd, &m_eset);
}

void Select::enableWrite(IFd *pfd)
{
	socket_t fd = pfd->socket();
	FD_SET(fd, &m_wset);
	FD_SET(fd, &m_eset);
}

void Select::enableAll(IFd *pfd)
{
	socket_t fd = pfd->socket();
	FD_SET(fd, &m_wset);
	FD_SET(fd, &m_rset);
	FD_SET(fd, &m_eset);
}

void Select::disableRead(IFd *pfd)
{
	socket_t fd = pfd->socket();
	FD_CLR(fd, &m_rset);

	if (!FD_ISSET(fd, &m_wset)) {
		FD_CLR(fd, &m_eset);
	}
}

void Select::disableWrite(IFd *pfd)
{
	socket_t fd = pfd->socket();
	FD_CLR(fd, &m_wset);

	if (!FD_ISSET(fd, &m_rset)) {
		FD_CLR(fd, &m_eset);
	}
}

void Select::disableAll(IFd *pfd)
{
	socket_t sock = pfd->socket();

	FD_CLR(sock, &m_rset);
	FD_CLR(sock, &m_wset);
	FD_CLR(sock, &m_eset);
}

void Select::reopen(IFd *pfd)
{
}

int Select::event_loop()
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
				IFd *link = m_links[i];
				int fd = link->socket();

				if(FD_ISSET(fd, &rset)) {
					link->handleRead();
					if(--readyfds == 0) break;
				}
				else if(FD_ISSET(fd, &wset)) {
					// LOG_DEBUG << "write fds";
					link->handleWrite();
					if(--readyfds == 0) break;
				}
				else if(FD_ISSET(fd, &eset)) {
					LOG_DEBUG << "exception fd";
					link->handleError();
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
