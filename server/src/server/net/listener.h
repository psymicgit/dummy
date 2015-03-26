///<------------------------------------------------------------------------------
//< @file:   server\net\listener.h
//< @author: 洪坤安
//< @date:   2014年11月25日 19:14:27
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _listener_h_
#define _listener_h_

#include "net.h"
#include "netaddress.h"

class INetReactor;
class Link;
class NetAddress;

class Listener : public IFd
{
public:
	Listener(INet*, INetReactor*, task_queue_pool_t*);

	bool open(const string& ip, int port);

	void close();

	virtual socket_t socket() const {return m_listenFd;}
	virtual int handleRead();
	virtual int handleWrite() {return -1;}
	virtual int handleError() {return -1;}

private:
	void onAccepted(Link &link);

	Link* createLink(socket_t newfd, NetAddress &peerAddr);

public:
	NetAddress m_listenAddr;

private:
	socket_t m_listenFd;
	INet *m_net;

	INetReactor *m_pNetReactor;
	task_queue_pool_t*  m_taskQueuePool;
};

#endif //_listener_h_