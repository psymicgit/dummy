///<------------------------------------------------------------------------------
//< @file:   server\net\connector.h
//< @author: 洪坤安
//< @date:   2015年3月4日 12:25:54
//< @brief:  网络连接器
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _connector_h_
#define _connector_h_

#include "netaddress.h"
#include "netdefine.h"

class Net;
class NetModel;

// 网络连接器：用于主动向指定的<ip,端口>发起连接，如果连接不上将定时尝试重连直到成功
class Connector : public IFd
{
	static const int max_retry_delay_ms = 30 * 1000; // 尝试重连的最大间隔
	static const int init_retry_delay_ms = 3000;	  // 第一次尝试重连的间隔

	// 连接所处的状态
	enum ConnectState {
		StateDisconnected, // 未连接
		StateConnecting,   // 连接中
		StateConnected	   // 已连接成功
	};

public:
	Connector(NetAddress &peerAddr, INetLogic*, NetModel*, const char* remoteHostName, Net*);

	// 初始化本连接器
	bool open();

	virtual socket_t socket() const {return m_sockfd;}

	// 处理可读事件
	virtual void handleRead();

	// 处理可写事件：对于非阻塞connect，当连接成功建立时，描述符变成可写
	virtual void handleWrite();

	// 处理异常：发生异常将隔一段时间尝试重连
	virtual void handleError();

	// 关闭本连接器：将屏蔽本连接器上的所有网络事件，并准备回收内存
	virtual void close();

	// 删除本连接器
	virtual void erase();

	// 开始连接
	void connect();

	// 连接成功
	bool onConnected();

	// 正在连接中：注册事件等待返回连接成功或失败的网络信号
	bool connecting();

	// 隔一断时间后尝试重连
	bool retry();

private:
	// 申请一个新的连接实例
	Link* allocLink(socket_t newfd, NetAddress &peerAddr);

private:
	// 对端的网络地址
	NetAddress m_peerAddr;

	// 与本连接器绑定的逻辑实例
	INetLogic *m_logic;

	// 网络中心
	NetModel *m_netModel;

	// 本连接器的套接字
	socket_t m_sockfd;

	// 尝试重连的间隔
	int m_retryDelayMs;

	// 当前所处的连接状态：未连接、连接中、已连接成功
	ConnectState m_state;

	// 当前连接的socket错误码
	int m_errno;

	// 对端的业务名称
	std::string m_remoteHostName;

	// 网络中心
	Net *m_net;
};

#endif //_connector_h_