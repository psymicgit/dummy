///<------------------------------------------------------------------------------
//< @file:   server\net\link.h
//< @author: 洪坤安
//< @date:   2015年1月14日 23:3:23
//< @brief:	 Link = tcp连接
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _link_h_
#define _link_h_

#include "net.h"
#include "netaddress.h"
#include "basic/buffer.h"
#include "protocol/protocol.h"

class NetAddress;
class INetReactor;
struct RingBufferBlock;

namespace google {namespace protobuf {class Message;}}

// 网络连接：Link = tcp连接，相当于connection
class Link : public IFd
{
public:
	explicit Link(int sockfd, NetAddress &localAddr, NetAddress &peerAddr, NetModel *pNet, INetReactor *pNetReactor)
		: m_localAddr(localAddr)
		, m_peerAddr(peerAddr)
		, m_pNetReactor(pNetReactor)
		, m_isAutoReconnect(false)
		, m_closed(false)
		, m_error(false)
		, m_sockfd(sockfd)
		, m_net(pNet)
		, m_head(NULL)
		, m_tail(NULL)
		, m_isWaitingWrite(false)
		, m_isWaitingRead(false)
		, m_isWaitingClose(false)
	{
	}

	~Link() {}

public:
	virtual socket_t socket() const {return m_sockfd;}

	virtual int handleRead();
	virtual int handleWrite();
	virtual int handleError();
	virtual void close();
	virtual void erase();

public:
	// 设置本连接参数，并注册到网络
	void open();

	// 发送数据
	void send(const char *data, int len);

	// 发送文本
	void send(const char *text);

	// 发送未加密的消息包
	void send(int msgId, Message &msg);

	// 发送经过加密或调整的消息包
	void send(int msgId, const char *data, int len);

	// 本连接是否打开
	inline bool isopen() { return !m_closed; }

private:
	// 发送本连接尚未发送完的数据
	void sendBuffer();

	// 获取本连接的本地地址
	NetAddress getLocalAddr() const;

	// 获取本连接的对端地址
	NetAddress getPeerAddr() const;

private:
	// 网络层：尝试一次性发送数据，返回尚未发送的数据长度
	int trySend(Buffer&);

	// 业务层：处理本连接的关闭
	void onLogicClose();

	// 网络层：开始发送数据
	void onSend();

public:
	const NetAddress m_localAddr;
	const NetAddress m_peerAddr;

	// 本连接所对应的逻辑实例
	INetReactor *m_pNetReactor;

	// 自动重连标志位：标示本连接断开后是否需要自动重连
	bool m_isAutoReconnect;

	RingBufferBlock *m_head;
	RingBufferBlock *m_tail;

public:
	// 是否已关闭标志位：防止重复close
	bool m_closed;

	// 错误标志位：标示本连接是否曾发生过导致需要关闭连接的错误
	bool m_error;

	// 是否正在等待关闭中
	bool m_isWaitingClose;

	// 套接字
	socket_t m_sockfd;

	// 网络中心
	NetModel *m_net;

	// 网络任务队列
	ITaskQueue *m_taskQueue;

	// 发送缓冲区
	Buffer m_sendBuf;

	// 接收缓冲区
	Buffer m_recvBuf;

	// 发送缓冲区锁
	mutex_t m_sendBufLock;

	// 接收缓冲区锁
	mutex_t m_recvBufLock;

	// 是否已在等待写
	bool m_isWaitingWrite;

	// 是否已在等待读
	bool m_isWaitingRead;
};

#endif //_link_h_