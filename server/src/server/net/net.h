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
#include "basic/objectpool.h"
#include "basic/buffer.h"
#include "basic/ringbuffer.h"

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
	virtual void erase() = 0;

#ifndef WIN
public:
	uint32 m_events;
#endif
};

typedef ObjectPool<Link> LinkPool;
typedef ObjectPool<Buffer> BufferPool;

//! 网络事件分发器
class INet
{
public:
	virtual ~INet() {}

	virtual bool init(int initLinkCount, int linkGrowCount) = 0;

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

	virtual TaskQueue* getTaskQueue()	= 0;
	virtual TimerQueue& getTimerQueue() = 0;
	virtual LinkPool& getLinkPool()		= 0;
};

#ifndef WIN

// linux下epoll
class Epoll : public INet
{
public:
	Epoll();
	~Epoll();

	virtual bool init(int initLinkCount, int linkGrowCount);

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

	virtual TaskQueue* getTaskQueue() { return NULL;}
	virtual TimerQueue& getTimerQueue() { return m_timers; }
	inline virtual LinkPool& getLinkPool() { return m_linkPool; }

protected:
	void recycleFds();
	int interruptLoop();
	void mod(IFd*, uint32 events);
	void stop();

	volatile bool            m_running;
	int                      m_efd;
	int                      m_interupt_sockets[2];

//! 待销毁的error socket
	std::list<IFd*>   		     m_deletingFdList;
	fast_mutex                  m_mutex;

	TimerQueue m_timers;
// volatile int m_curFdCount;
	LinkPool m_linkPool;

public:
	RingBuffer m_ringbuffer;
	Buffer m_recvBuf;
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

	virtual bool init(int initLinkCount, int linkGrowCount);

	virtual int eventLoop();
	virtual void reopen(IFd*) {}
	virtual void close();

	virtual void addFd(IFd*);
	virtual void delFd(IFd*);

	virtual void enableRead(IFd*);
	virtual void disableRead(IFd*);

	virtual void enableWrite(IFd*);
	virtual void disableWrite(IFd*);

	virtual void enableAll(IFd*);
	virtual void disableAll(IFd*);

	virtual TaskQueue* getTaskQueue() { return &m_tasks;}
	virtual TimerQueue& getTimerQueue() { return m_timers; }
	inline virtual LinkPool& getLinkPool() { return m_linkPool; }

private:
	void updateFd(IFd*, FDOperator);

	void closing();

private:
	LinkerList m_links; // 当前维持的连接
	LinkPool m_linkPool;

	TaskQueue m_tasks;
	TimerQueue m_timers;

	int m_maxfd;

	fd_set m_rset;
	fd_set m_wset;
	fd_set m_eset;

	bool m_running;

public:
	RingBuffer m_ringbuffer;
	Buffer m_recvBuf;
};

#endif

#endif //_net_h_