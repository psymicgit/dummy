///<------------------------------------------------------------------------------
//< @file:   server\net\listener.h
//< @author: 洪坤安
//< @date:   2014年11月25日 19:14:27
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _listener_h_
#define _listener_h_

#include "netdefine.h"
#include "netaddress.h"

class INetLogic;
class Link;
class NetAddress;
class Net;

// 网络监听器，监听指定端口上接收到的新连接
class Listener : public IFd
{
public:
	Listener(NetModel*, INetLogic*, Net*);

	// 开始监听
	virtual bool open(const string& ip, int port);

	// 停止监听
	virtual void close();

	// 删除本监听器
	virtual void erase();

	// 返回监听套接字
	virtual socket_t socket() const {return m_listenFd;}

	// 处理可读事件：对于非阻塞listen，当接收到新连接时，描述符变成可读
	virtual void handleRead();

	// 处理可写事件（正常是接收不到的）
	virtual void handleWrite();

	// 处理异常事件（正常是接收不到的）
	virtual void handleError();

private:
	// 申请一个新的连接实例
	Link* allocLink(socket_t newfd, NetAddress &peerAddr);

public:
	// 当前正在监听的地址
	NetAddress m_listenAddr;

private:
	// 网络中心
	NetModel *m_netModel;

	// 与本监听器绑定的逻辑实例
	INetLogic *m_logic;

	// 正在监听的socket
	socket_t m_listenFd;

	Net *m_net;
};

#endif //_listener_h_