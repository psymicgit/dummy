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
class NetFactory;

class Listener : public IFd
{
public:
	Listener(NetModel*, INetReactor*, NetFactory*);

	virtual bool open(const string& ip, int port);
	virtual void close();
	virtual void erase();

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
	NetModel *m_net;
	INetReactor *m_pNetReactor;

	socket_t m_listenFd;
	NetFactory *m_netFactory;
};

#endif //_listener_h_