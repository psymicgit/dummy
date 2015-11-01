///<------------------------------------------------------------------------------
//< @file:   server\net\net.h
//< @author: hongkunan
//< @date:   2015年1月13日 14:10:49
//< @brief:	 网络中心，提供连接指定<ip:端口>、监听指定<端口>等网络操作接口
//< Copyright (c) 2015 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _net_h_
#define _net_h_

#include "basic/thread.h"

class NetModel;
class TaskQueuePool;
class Thread;
class Listener;
class Link;
class Connector;
class INetLogic;

// 网络通信中心，提供连接指定<ip:端口>、监听指定<端口>等网络操作接口
class Net
{
	friend class Listener;
	friend class Connector;

	typedef std::vector<Listener*> ListenerVec;
	typedef std::vector<Connector*> ConnectorVec;

private:
	// 每个网络线程启动后将开始执行本方法
	static void runNet(void *e);

public:
	Net();

	// 初始化网络：定义开启线程数、连接池空间
	bool init(int threadCnt, int initLinkCnt = 100);

	// 开始执行网络操作
	void start();

	// 停止执行网络操作（将阻塞直到所有网络线程关闭）
	void stop();

	// 监听指定的ip和端口，由传入的INetLogic处理新连接
	Listener* listen(const string& ip, int port, INetLogic&);

	// 主动连接指定的ip和端口，由传入的INetLogic执行连接接收成功后的操作
	Connector* connect(const string& ip, int port, INetLogic&, const char* remoteHostName);

private:
	// 从第2个网络线程起依次获取下一个网络线程（仅有1个网络线程时则只返回1个）
	NetModel *nextNetModel();

public:
	// 网络模型: linux下epoll / windows下select
	std::vector<NetModel*> m_nets;

	// 下一次分配的网络线程索引
	int m_allocNetIdx;

	// 网络线程
	Thread m_netThread;

	// 是否已启动标志
	bool m_started;

	// 网络监听器列表
	ListenerVec m_listeners;

	// 网络连接器列表
	ConnectorVec m_connectors;
};

#endif //_net_h_