///<------------------------------------------------------------------------------
//< @file:   server\net\net.h
//< @author: 洪坤安
//< @date:   2014年11月26日 10:42:10
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _net_h_
#define _net_h_

#include "basic/timerqueue.h"
#include "basic/lock.h"
#include "basic/taskqueue.h"

class Listener;
class Link;

// 文件描述符接口(file descriptor)，套接字和监听器将从此类派生
typedef int socket_t;
class IFd
{
public:
	virtual socket_t socket() const = 0;
	virtual int handleRead()  = 0;
	virtual int handleWrite() = 0;
	virtual int handleError()   = 0;

	virtual void close() = 0;
};

//! 网络事件分发器
class INet
{
public:
	virtual ~INet() {}

	virtual int event_loop() 		    = 0;
	virtual void close() 				= 0;
	virtual void reopen(IFd*)            = 0;

	virtual void addFd(IFd*)				= 0;
	virtual void delFd(IFd*)				= 0;

	virtual void enableRead(IFd*)		= 0;
	virtual void disableRead(IFd*)		= 0;

	virtual void enableWrite(IFd*)		= 0;
	virtual void disableWrite(IFd*)		= 0;

	virtual void enableAll(IFd*)		= 0;
	virtual void disableAll(socket_t)		= 0;

	virtual TimerQueue& getTimerQueue() = 0;
	virtual TaskQueue& getTaskQueue()   = 0;
};

#ifndef WIN

// linux下epoll
class Epoll : public INet
{
public:
	Epoll();
	~Epoll();

	virtual int event_loop();
	virtual int close();
	virtual int registerFd(IFd*);
	virtual int unregisterFd(IFd*);
	virtual int reopen(IFd*);

	int interupt_loop(); //! 中断事件循环

protected:
	void fd_del_callback();

	volatile bool            m_running;
	int                      m_efd;
	task_queue_i*            m_task_queue;
	int                      m_interupt_sockets[2];
	//! 待销毁的error socket
	list<IFd*>   		     m_error_fd_set;
	mutex_t                  m_mutex;
	TimerQueue m_timers;
};

#else

// windows下iocp
class Select : public INet
{
private:
	typedef std::vector<IFd*> LinkerList;

public:
	Select();
	~Select() {}

	virtual int event_loop();
	virtual void close() {}

	virtual void addFd(IFd*);
	virtual void delFd(IFd*);

	virtual void enableRead(IFd*);
	virtual void disableRead(IFd*);

	virtual void enableWrite(IFd*);
	virtual void disableWrite(IFd*);

	virtual void enableAll(IFd*);
	virtual void disableAll(socket_t);

	virtual void reopen(IFd*);

	virtual TimerQueue& getTimerQueue() { return m_timerQueue; }
	virtual TaskQueue& getTaskQueue() { return m_taskQueue; }

protected:

private:
	LinkerList m_links;
	TaskQueue m_taskQueue;
	TimerQueue m_timerQueue;

	int m_maxfd;

	fd_set m_rset;
	fd_set m_wset;
	fd_set m_eset;
};

#endif

#endif //_net_h_