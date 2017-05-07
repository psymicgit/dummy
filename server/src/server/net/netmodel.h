///<------------------------------------------------------------------------------
//< @file:   server\net\netmodel.h
//< @author: 洪坤安
//< @date:   2014年11月26日 10:42:10
//< @brief:	 提供跨平台网络模型的封装，linux下仅支持epoll，windows下仅支持select
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _netmodel_h_
#define _netmodel_h_

#include "basic/timerqueue.h"
#include "basic/lock.h"
#include "basic/taskqueue.h"
#include "basic/objectpool.h"
#include "basic/buffer.h"

#include "netdefine.h"

class Listener;
class Link;

typedef ObjectPool<Link, mutex_t> LinkPool;

#ifndef WIN

// linux下epoll
class Epoll
{
private:
	typedef std::vector<IFd*> FdList;

public:
	Epoll();
	~Epoll();

	bool init(int initLinkCount, int linkGrowCount);

	int eventLoop();
	void close();

	void reopen(IFd*);

	void addFd(IFd*);
	void delFd(IFd*);

	void enableRead(IFd*);
	void disableRead(IFd*);

	void enableWrite(IFd*);
	void disableWrite(IFd*);

	void enableAll(IFd*);
	void disableAll(IFd*);

	inline TaskQueue* getTaskQueue() { return &m_tasks;}
	inline TimerQueue& getTimerQueue() { return m_timers; }
	inline LinkPool& getLinkPool() { return m_linkPool; }
	int interruptLoop();

protected:
	void recycleFds();
	void mod(IFd*, uint32 events);
	void stop();
	void closing();

	bool		m_running;			// <网络是否正在运行>标志位

	int			m_efd;				// epoll句柄

	int			m_wakeup[2];		// 用于唤醒epoll的socketpair，实际上是半双工通信，但这里我们只用来单工通信

	FdList		m_deletingFdList;	// 待销毁的socket列表

	mutex_t		m_mutex;			// 删除socket列表的锁

	TaskQueue	m_tasks;			// 任务队列

	TimerQueue	m_timers;			// 定时器列表

	LinkPool	m_linkPool;			// 连接池，保存预先分配好的连接

public:
	char g_recvBuf[MAX_PACKET_LEN];		// 预先申请的接收缓冲区
};

#else

// windows下select
class Select
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

	bool init(int initLinkCount, int linkGrowCount);

	int eventLoop();
	void reopen(IFd*) {}
	void close();

	void addFd(IFd*);
	void delFd(IFd*);

	void enableRead(IFd*);
	void disableRead(IFd*);

	void enableWrite(IFd*);
	void disableWrite(IFd*);

	void enableAll(IFd*);
	void disableAll(IFd*);

	inline TaskQueue* getTaskQueue() { return &m_tasks;}
	inline TimerQueue& getTimerQueue() { return m_timers; }
	inline LinkPool& getLinkPool() { return m_linkPool; }
	int interruptLoop() { return 0; }

private:
	void updateFd(IFd*, FDOperator);
	void closing();

private:

	LinkerList	m_links;	// 当前维持的连接

	LinkPool	m_linkPool;	// 连接池，保存预先分配好的连接

	TaskQueue	m_tasks;	// 任务队列

	TimerQueue	m_timers;	// 定时器队列

	int			m_maxfd;	// 当前最大fd

	fd_set		m_rset;		// 读文件描述符集

	fd_set		m_wset;		// 写文件描述符集

	fd_set		m_eset;		// exception文件描述符集

	bool		m_running;	// <网络是否正在运行>标志位

public:
	char g_recvBuf[MAX_PACKET_LEN];		// 预先申请的接收缓冲区
};

#endif

#endif //_net_h_