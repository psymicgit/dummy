///<------------------------------------------------------------------------------
//< @file:   server\net\netfactory.h
//< @author: hongkunan
//< @date:   2015年1月13日 14:10:49
//< @brief:
//< Copyright (c) 2015 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _netfactory_h_
#define _netfactory_h_

#include "basic/thread.h"
#include "basic/objectpool.h"
#include "net.h"
#include "link.h"

class Epoll;
class Select;
class TaskQueuePool;
class Thread;
class Listener;
class Link;
class Connector;
class INetReactor;

// 网络通信中心，提供listen、connect等网络操作接口
class NetFactory
{
	friend class Listener;
	friend class Connector;

	typedef std::vector<Listener*> ListenerVec;
	typedef std::vector<Connector*> ConnectorVec;

private:
	// 网络线程启动后将开始执行本方法
	static void runNet(void *e);

public:
	NetFactory();

	// 初始化网络：定义开启线程数、连接池空间
	bool init(int threadCnt, int initLinkCnt = 200);

	// 开始执行网络操作
	void start();

	// 停止执行网络操作（将阻塞直到所有网络线程关闭）
	void stop();

	// 监听指定的ip和端口，由传入的INetReactor处理新连接
	Listener* listen(const string& ip, int port, INetReactor&);

	// 主动连接指定的ip和端口，由传入的INetReactor执行连接接收成功后的操作
	Connector* connect(const string& ip, int port, INetReactor&, const char* remoteHostName);

	// 从下一个网络线程中取出网络通信中心指针
	NetModel *nextNet();

public:
	// 网络模型: linux下epoll / windows下select
	std::vector<NetModel*> m_nets;

	int m_curNetIdx;

	// 网络线程
	Thread m_netThread;

	// 是否已启动标志
	bool m_started;

	// 网络监听器列表
	ListenerVec m_listeners;

	// 网络连接器列表
	ConnectorVec m_connectors;
};

#endif //_netfactory_h_