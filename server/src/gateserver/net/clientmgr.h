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
#include <hash_map>
#include <net/msgdispatcher.h>
#include <net/client.h>

class Client;

class ClientMgr : public INetReactor
{
private:
	typedef stdext::hash_map<uint32, Client*> ClientMap;

public:
	ClientMgr();

public:
	virtual void OnAccepted(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void OnDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void OnRecv(Link*, Buffer&);

	virtual TaskQueue& GetTaskQueue() { return *m_taskQueue; }

	void DelClient(Client*);

	void handleMsg(Client &client, int msgId, Buffer&, Timestamp receiveTime);

private:
	uint32 allocClientId();

public:
	TaskQueue *m_taskQueue;

	uint32 m_allocClientId;

	ClientMap m_clientMap;

	MsgDispatcher<Client> m_dispatcher;
};

#endif //_clientmgr_h_