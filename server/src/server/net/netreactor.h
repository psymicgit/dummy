///<------------------------------------------------------------------------------
//< @file:   server\net\netreactor.h
//< @author: 洪坤安
//< @date:   2015年1月28日 15:36:14
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _netreactor_h_
#define _netreactor_h_

class Link;
class NetAddress;
class Buffer;
class TaskQueue;

// 网络逻辑实例，用于捕获网络上发生的事件
class INetReactor
{
public:
	// 获取本网络逻辑实例的名称
	virtual std::string name() = 0;

	// 成功主动连接上对端
	virtual void onConnected(Link*, const NetAddress& localAddr, const NetAddress& peerAddr) {}

	// 成功接收到新的连接
	virtual void onAccepted(Link*, const NetAddress& localAddr, const NetAddress& peerAddr) {}

	// 对端连接关闭
	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr) = 0;

	// 接收到新的数据
	virtual void onRecv(Link*, Buffer&) = 0;

	// 获取本实例所处的业务层的任务队列
	virtual TaskQueue& getTaskQueue() = 0;
};

#endif //_netreactor_h_