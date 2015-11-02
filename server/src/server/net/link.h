///<------------------------------------------------------------------------------
//< @file:   server\net\link.h
//< @author: 洪坤安
//< @date:   2015年1月14日 23:3:23
//< @brief:	 Link = tcp连接
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _link_h_
#define _link_h_

#include "netdefine.h"
#include "netaddress.h"
#include "basic/buffer.h"
#include "basic/evbuffer-internal.h"
#include "protocol/protocol.h"

class NetAddress;
class INetLogic;
struct RingBufferBlock;

struct evbuffer;

namespace google {namespace protobuf {class Message;}}

// 网络连接：Link = tcp连接，相当于connection
class Link : public IFd
{
public:
	explicit Link(int sockfd, NetAddress &localAddr, NetAddress &peerAddr, NetModel *pNet, INetLogic *logic)
		: m_localAddr(localAddr)
		, m_peerAddr(peerAddr)
		, m_logic(logic)
		, m_isAutoReconnect(false)
		, m_closed(false)
		, m_error(false)
		, m_sockfd(sockfd)
		, m_net(pNet)
		, m_isWaitingWrite(false)
		, m_isWaitingRead(false)
		, m_isWaitingClose(false)
		, m_isPeerClosed(false)
	{
	}

	~Link();

public:
	virtual socket_t socket() const {return m_sockfd;}

	virtual void handleRead();
	virtual void handleWrite();
	virtual void handleError();
	virtual void close();
	virtual void erase();

private:
	void closing();

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

	// 业务层开始读取数据：将接收缓冲区内的数据全部取出存到readto并置空
	void beginRead(evbuffer *readto);

	// 业务层读取数据完毕：将业务层未处理完的数据重新存入接收缓冲区的头部
	void endRead(evbuffer *remain);

	// 本连接是否打开
	inline bool isopen() const { return !m_closed; }

	// 获取本连接的本地地址
	NetAddress getLocalAddr() const;

	// 获取本连接的对端地址
	NetAddress getPeerAddr() const;

private:
	// 发送本连接尚未发送完的数据
	void sendBuffer();

private:
	// 网络层执行,：尝试一次性发送数据，返回尚未发送的数据长度
	int trySend(evbuffer*);

	// 业务层执行：处理本连接的关闭
	void onLogicClose();

	// 网络层执行：开始发送数据
	void onSend();

public:
	const NetAddress m_localAddr;
	const NetAddress m_peerAddr;

	INetLogic*	m_logic;			// 本连接所对应的逻辑实例

	bool		m_isAutoReconnect;	// 自动重连标志位：标示本连接断开后是否需要自动重连

public:

	bool		m_closed;			// 是否已关闭标志位：防止重复close

	bool		m_error;			// 错误标志位：标示本连接是否曾发生过导致需要关闭连接的错误

	bool		m_isWaitingClose;	// 是否正在等待关闭中

	socket_t	m_sockfd;			// 套接字

	NetModel*	m_net;				// 网络中心

	evbuffer	m_sendBuf;			// 发送缓冲区

	evbuffer	m_recvBuf;			// 接收缓冲区

	mutex_t		m_sendBufLock;		// 发送缓冲区锁

	mutex_t		m_recvBufLock;		// 接收缓冲区锁

	bool		m_isWaitingWrite;	// 是否已在等待网络层发送数据

	bool		m_isWaitingRead;	// 是否已在等待业务层读取数据

	bool		m_isPeerClosed;		// 对端是否已关闭
};

#endif //_link_h_