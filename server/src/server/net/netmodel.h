///<------------------------------------------------------------------------------
//< @file:   server\net\netmodel.h
//< @author: ������
//< @date:   2014��11��26�� 10:42:10
//< @brief:	 �ṩ��ƽ̨����ģ�͵ķ�װ��linux�½�֧��epoll��windows�½�֧��select
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

// linux��epoll
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

	bool		m_running;			// <�����Ƿ���������>��־λ

	int			m_efd;				// epoll���

	int			m_wakeup[2];		// ���ڻ���epoll��socketpair��ʵ�����ǰ�˫��ͨ�ţ�����������ֻ��������ͨ��

	FdList		m_deletingFdList;	// �����ٵ�socket�б�

	mutex_t		m_mutex;			// ɾ��socket�б����

	TaskQueue	m_tasks;			// �������

	TimerQueue	m_timers;			// ��ʱ���б�

	LinkPool	m_linkPool;			// ���ӳأ�����Ԥ�ȷ���õ�����

public:
	char g_recvBuf[MAX_PACKET_LEN];		// Ԥ������Ľ��ջ�����
};

#else

// windows��select
class Select
{
private:
	// ����Ե����ļ��������Ĳ������
	enum FDOperator {
		FD_ADD,			  // ���fd
		FD_DEL,			  // ɾ��fd
		FD_ENABLE_READ,   // �ɶ�
		FD_DISABLE_READ,  // ���ɶ�
		FD_ENABLE_WRITE,  // ��д
		FD_DISABLE_WRITE, // ����д
		FD_ENABLE_ALL,	  // ���������¼�
		FD_DISABLE_ALL	  // ȡ�����������¼�
	};

	// fd�б�
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

	LinkerList	m_links;	// ��ǰά�ֵ�����

	LinkPool	m_linkPool;	// ���ӳأ�����Ԥ�ȷ���õ�����

	TaskQueue	m_tasks;	// �������

	TimerQueue	m_timers;	// ��ʱ������

	int			m_maxfd;	// ��ǰ���fd

	fd_set		m_rset;		// ���ļ���������

	fd_set		m_wset;		// д�ļ���������

	fd_set		m_eset;		// exception�ļ���������

	bool		m_running;	// <�����Ƿ���������>��־λ

public:
	char g_recvBuf[MAX_PACKET_LEN];		// Ԥ������Ľ��ջ�����
};

#endif

#endif //_net_h_