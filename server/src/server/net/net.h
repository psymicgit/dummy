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
class IFd
{
public:
	// 返回socket值
	virtual socket_t socket() const = 0;

	// 处理可读信号
	virtual void handleRead()  = 0;

	// 处理可写信号
	virtual void handleWrite() = 0;

	// 处理异常信号
	virtual void handleError() = 0;

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

#define MAX_PACKET_LEN (1 * 1024 * 1024) // 2MB

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

	// <网络是否正在运行>标志位
	bool m_running;

	// epoll句柄
	int m_efd;

	// 用于唤醒epoll的socketpair，实际上是半双工通信，但这里我们只用来单工通信，所以命名为pipe
	int m_pipe[2];

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

public:
	// 预先申请的加密缓冲区: 用于发送和接收数据时进行加解密运算
	char g_encryptBuf[MAX_PACKET_LEN];
	char g_recvBuf[MAX_PACKET_LEN];
	char g_sendBuf[MAX_PACKET_LEN];
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
	// 当前维持的连接
	LinkerList m_links;

	// 连接池，保存预先分配好的连接
	LinkPool m_linkPool;

	// 任务队列
	TaskQueue m_tasks;

	// 定时器队列
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

public:
	// 预先申请的加密缓冲区: 用于发送和接收数据时进行加解密运算
	char g_encryptBuf[MAX_PACKET_LEN];
	char g_recvBuf[MAX_PACKET_LEN];
	char g_sendBuf[MAX_PACKET_LEN];
};

#endif

#endif //_net_h_