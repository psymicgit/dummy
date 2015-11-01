///<------------------------------------------------------------------------------
//< @file:   server\net\netdefine.h
//< @author: 洪坤安
//< @date:   2015年1月28日
//< @brief:	 定义网络通用接口
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _netdefine_h_
#define _netdefine_h_

class Link;
class NetAddress;
class TaskQueue;

// 定义允许网络消息包的最大大小 = 128KB
static const int MAX_PACKET_LEN = 128 * 1024;

// 网络逻辑实例，用于捕获网络上发生的事件
class INetLogic
{
public:
	virtual ~INetLogic() {}

	// 获取本网络逻辑实例的名称
	virtual std::string name() = 0;

	// 成功主动连接上对端
	virtual void onConnected(Link*, const NetAddress& localAddr, const NetAddress& peerAddr) {}

	// 成功接收到新的连接
	virtual void onAccepted(Link*, const NetAddress& localAddr, const NetAddress& peerAddr) {}

	// 对端连接关闭
	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr) = 0;

	// 接收到新的数据
	virtual void onRecv(Link*) = 0;

	// 获取本实例所处的业务层的任务队列
	virtual TaskQueue& getTaskQueue() = 0;
};

// 文件描述符接口(file descriptor)，连接Link、监听器Listener和连接器Connector将从此类派生
class IFd
{
public:
	virtual ~IFd() {}

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

#endif