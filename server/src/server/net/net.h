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

#ifndef WIN
public:
	uint16 m_events;
#endif
};

//! 网络事件分发器
class INet
{
public:
	virtual ~INet() {}

	virtual int eventLoop() 		    = 0;
	virtual void close() 				= 0;
	virtual void reopen(IFd*)			= 0;

	virtual void addFd(IFd*)			= 0;
	virtual void delFd(IFd*)			= 0;

	virtual void enableRead(IFd*)		= 0;
	virtual void disableRead(IFd*)		= 0;

	virtual void enableWrite(IFd*)		= 0;
	virtual void disableWrite(IFd*)		= 0;

	virtual void enableAll(IFd*)		= 0;
	virtual void disableAll(IFd*)		= 0;

	virtual TimerQueue& getTimerQueue() = 0;
	virtual TaskQueue& getTaskQueue()	= 0;
};

#ifndef WIN

// linux下epoll
class Epoll : public INet
{
public:
	Epoll();
	~Epoll();

	virtual int eventLoop();
	virtual void close();
	virtual void reopen(IFd*);

	virtual void addFd(IFd*);
	virtual void delFd(IFd*);

	virtual void enableRead(IFd*);
	virtual void disableRead(IFd*);

	virtual void enableWrite(IFd*);
	virtual void disableWrite(IFd*);

	virtual void enableAll(IFd*);
	virtual void disableAll(IFd*);

	virtual TimerQueue& getTimerQueue() { return m_timers; }
	virtual TaskQueue& getTaskQueue() { return m_tasks; }

protected:
	void recycleFds();
	int interruptLoop();
	void mod(IFd*, uint16 events);

	volatile bool            m_running;
	int                      m_efd;
	task_queue_i*            m_task_queue;
	int                      m_interupt_sockets[2];

	//! 待销毁的error socket
	list<IFd*>   		     m_deletingFdList;
	mutex_t                  m_mutex;

	TaskQueue m_tasks;
	TimerQueue m_timers;
};

#else

// windows下iocp
class Select : public INet
{
private:
	enum FDOperator {
		FD_ADD,
		FD_DEL,
		FD_ENABLE_READ,
		FD_DISABLE_READ,
		FD_ENABLE_WRITE,
		FD_DISABLE_WRITE,
		FD_ENABLE_ALL,
		FD_DISABLE_ALL
	};

	typedef std::vector<IFd*> LinkerList;


public:
	Select();
	~Select() {}

	virtual int eventLoop();
	virtual void close() {}
	virtual void reopen(IFd*) {}

	virtual void addFd(IFd*);
	virtual void delFd(IFd*);

	virtual void enableRead(IFd*);
	virtual void disableRead(IFd*);

	virtual void enableWrite(IFd*);
	virtual void disableWrite(IFd*);

	virtual void enableAll(IFd*);
	virtual void disableAll(IFd*);

	virtual TimerQueue& getTimerQueue() { return m_timers; }
	virtual TaskQueue& getTaskQueue() { return m_tasks; }

private:
	void updateFd(IFd*, FDOperator);

private:
	LinkerList m_links; // 当前维持的连接
	TaskQueue m_tasks;
	TimerQueue m_timers;

	int m_maxfd;

	fd_set m_rset;
	fd_set m_wset;
	fd_set m_eset;
};

#endif

#endif //_net_h_