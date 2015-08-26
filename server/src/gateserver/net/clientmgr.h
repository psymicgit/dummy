///<------------------------------------------------------------------------------
//< @file:   src\gateserver\net\clientmgr.h
//< @author: 洪坤安
//< @date:   2015年3月14日 20:14:21
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _clientmgr_h_
#define _clientmgr_h_

#include <net/netreactor.h>
#include <net/msgdispatcher.h>
#include <net/client.h>
#include <basic/objectpool.h>

class Client;

class ClientMgr : public INetReactor
{
private:
	typedef std::tr1::unordered_map<uint32, Client*> ClientMap;

public:
	ClientMgr();

	void close();

public:
	virtual void onAccepted(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onRecv(Link*, Buffer&, RingBufferBlock&);

	virtual TaskQueue& getTaskQueue() { return *m_taskQueue; }

	void delClient(Client*);

	void handleMsg(Client *client, int msgId, Buffer&, Timestamp receiveTime);

	void clear();

private:
	uint32 allocClientId();

public:
	TaskQueue *m_taskQueue;

	uint32 m_allocClientId;

	ClientMap m_clientMap;

	MsgDispatcher<Client> m_dispatcher;

	ObjectPool<Client> m_clientPool;

	Buffer m_msgBuf;
};

#endif //_clientmgr_h_