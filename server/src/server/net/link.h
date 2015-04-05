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

namespace google {namespace protobuf {class Message;}}

// Link = tcp连接，相当于connection
class Link : public IFd
{
public:
	explicit Link(int sockfd, NetAddress &localAddr, NetAddress &peerAddr, task_queue_i *pQueue, INet *pNet, INetReactor *pNetReactor)
		: m_localAddr(localAddr)
		, m_peerAddr(peerAddr)
		, m_pNetReactor(pNetReactor)
		, m_isCreateByConnector(false)
		, m_isClosing(false)
		, m_sockfd(sockfd)
		, m_net(pNet)
		, m_taskQueue(pQueue)
	{
	}

	~Link() {}

public:
	virtual socket_t socket() const {return m_sockfd;}
	virtual int handleRead();
	virtual int handleWrite();
	virtual int handleError();
	virtual void close();

public:
	void open();

	void enableRead();

	void send(Buffer&);
	void send(const char *data, int len);
	void send(const char *msg);
	void send(string &msg);
	void send(int msgId, google::protobuf::Message &msg);
	void send(int msgId, const char *data, int len);

	NetAddress getLocalAddr();

private:
	// 尝试一次性发送数据，返回尚未发送的数据长度
	int trySend(Buffer&);

	void onLogicClose();
	void onNetClose();
	void onSend(Buffer &buff);

	int handleReadTask();
	int handleWriteTask();

	bool isopen() { return !m_isClosing; }

public:
	const NetAddress m_localAddr;
	const NetAddress m_peerAddr;

	INetReactor *m_pNetReactor;

	bool m_isCreateByConnector;

private:
	volatile bool m_isClosing;

	socket_t m_sockfd;
	INet *m_net;

	task_queue_i *m_taskQueue;

	Buffer m_recvBuf;
	Buffer m_sendBuf;
};

#endif //_link_h_