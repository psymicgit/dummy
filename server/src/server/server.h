///<------------------------------------------------------------------------------
//< @file:   server\server\server.h
//< @author: 洪坤安
//< @date:   2014年11月20日 10:36:54
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _server_h_
#define _server_h_

#include "protocol/protocol.h"

#include "net/netreactor.h"
#include "net/netfactory.h"
#include "net/msgdispatcher.h"

#include "basic/taskqueue.h"
#include "tool/servertool.h"

class Buffer;
class ConnectReq;
class ServerLink;

class Server : public INetReactor
{
public:
	static Server *instance;

	typedef std::map<int, ServerLink*> ServerLinkMap;

public:
	Server();

	bool init();
	bool uninit();

	virtual ServerLink* OnAcceptServer(Link&, ServerType, int svrId) = 0;

	virtual void OnDisconnectServer(Link&, ServerType, int svrId) = 0;

	virtual void OnConnected(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void OnAccepted(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void OnDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void OnRecv(Link*, Buffer&);

	virtual TaskQueue& GetTaskQueue() { return m_taskQueue; }

	virtual void HandleMsg(Link*, int msgId, Buffer&);

	void start();

	virtual void run();

	bool isquit() { return m_isquit; }

	int getServerId(ServerType, int zoneId);

	bool isSvrLinkExist(ServerType, int zoneId);
	void addSvrLink(int svrId, ServerLink*);
	void eraseSvrLink(int svrId);

public:
	ServerType m_svrType;
	int m_zoneId;
	NetFactory m_lan;

protected:
	TaskQueue m_taskQueue;
	bool m_isquit;

	MsgDispatcher<Link> m_dispatcher;

	ServerLinkMap m_svrLinkMap;
};

#endif //_server_h_