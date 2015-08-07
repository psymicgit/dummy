///<------------------------------------------------------------------------------
//< @file:   server\server\server.h
//< @author: ������
//< @date:   2014��11��20�� 10:36:54
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

	typedef tr1::unordered_map<int, ServerLink*> ServerLinkMap;

public:
	Server();

	bool init();
	virtual bool uninit();

	virtual ServerLink* onAcceptServer(Link&, ServerType, int svrId) = 0;

	virtual void onDisconnectServer(Link&, ServerType, int svrId) = 0;

	virtual void onConnected(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onAccepted(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	void onRecvBlock(Link*, RingBufferBlock*);

	virtual void onRecv(Link*, Buffer&, RingBufferBlock&);

	virtual TaskQueue& getTaskQueue() { return m_taskQueue; }

	virtual void handleMsg(Link*, int msgId, Buffer*);

	void start();

	void stop();

	virtual void stopping();

	virtual void run();

	bool isquit() { return m_isquit; }

	int getServerId(ServerType, int zoneId);

	bool isSvrLinkExist(ServerType, int zoneId);
	void addSvrLink(int svrId, ServerLink*);
	void delSvrLink(int svrId);

	std::string& getServerName();

public:
	ServerType m_svrType;
	int m_zoneId;
	NetFactory m_lan;

protected:
	bool m_isquit;

	TaskQueue m_taskQueue;
	MsgDispatcher<Link> m_dispatcher;
	ServerLinkMap m_svrLinkMap;

	ObjectPool<Buffer> m_bufferPool;
};

#endif //_server_h_