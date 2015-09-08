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
	// 返回socket值
	virtual socket_t socket() const = 0;

	// 处理可读信号
	virtual int handleRead()  = 0;

	// 处理可写信号
	virtual int handleWrite() = 0;

	// 处理异常信号
	virtual int handleError()   = 0;

	// 关闭
	virtual void close() = 0;

	// 删除本fd
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

	// 网络事件处理循环
	virtual int eventLoop() 		    = 0;
	virtual void close() 				= 0;
	virtual int interruptLoop()			= 0;

	virtual void addFd(IFd*)			= 0;
	virtual void delFd(IFd*)			= 0;

	virtual void enableRead(IFd*)		= 0;
	virtual void disableRead(IFd*)		= 0;

	virtual void enableWrite(IFd*)		= 0;
	virtual void disableWrite(IFd*)		= 0;

	virtual void enableAll(IFd*)		= 0;
	virtual void disableAll(IFd*)		= 0;

	virtual void reopen(IFd*)			= 0;

	virtual TaskQueue* getTaskQueue()	= 0;
	virtual TimerQueue& getTimerQueue() = 0;
	virtual LinkPool& getLinkPool()		= 0;
};

#ifndef WIN

// linux下epoll
class Epoll : public INet
{
private:
	typedef std::vector<IFd*> FdList;

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

	virtual TaskQueue* getTaskQueue() { return &m_tasks;}
	virtual TimerQueue& getTimerQueue() { return m_timers; }
	inline virtual LinkPool& getLinkPool() { return m_linkPool; }
	int interruptLoop();

protected:
	void recycleFds();
	void mod(IFd*, uint32 events);
	void stop();
	void closing();

	// <网络是否正在运行>标志位
	bool m_running;

	// epoll句柄
	int m_efd;

	// 用于唤醒epoll的socketpair
	int m_interupt_sockets[2];

	// 待销毁的socket列表
	FdList m_deletingFdList;

	// 删除socket列表
	mutex_t m_mutex;

	// 任务队列
	TaskQueue m_tasks;

	// 定时器列表
	TimerQueue m_timers;

	// 连接池，保存预先分配好的连接
	LinkPool m_linkPool;
};

#else

// windows下select
class Select : public INet
{
private:
	// 定义对单个文件描述符的操作类别
	enum FDOperator {
		FD_ADD,			  // 添加fd
		FD_DEL,			  // 删除fd
		FD_ENABLE_READ,   // 可读
		FD_DISABLE_READ,  // 不可读
		FD_ENABLE_WRITE,  // 可写
		FD_DISABLE_WRITE, // 不可写
		FD_ENABLE_ALL,	  // 监听所有事件
		FD_DISABLE_ALL	  // 取消监听所有事件
	};

	// fd列表
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
	int interruptLoop() { return 0; }

private:
	void updateFd(IFd*, FDOperator);
	void closing();

private:
	// 当前维持的连接
	LinkerList m_links;

	// 连接池，保存预先分配好的连接
	LinkPool m_linkPool;

	// 任务队列
	TaskQueue m_tasks;

	// 定时器
	TimerQueue m_timers;

	// 当前最大fd
	int m_maxfd;

	// 读文件描述符集
	fd_set m_rset;

	// 写文件描述符集
	fd_set m_wset;

	// exception文件描述符集
	fd_set m_eset;

	// <网络是否正在运行>标志位
	bool m_running;
};

#endif

#endif //_net_h_