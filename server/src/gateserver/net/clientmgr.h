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

// 客户端管理中心，管理当前在线的客户端连接实例
class ClientMgr : public INetReactor
{
private:
	typedef std::tr1::unordered_map<uint32, Client*> ClientMap;

public:
	ClientMgr();
	~ClientMgr();

	void close();

public:
	virtual std::string name() { return "clientmgr"; }

	virtual void onAccepted(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onRecv(Link*, Buffer&);

	virtual TaskQueue& getTaskQueue() { return *m_taskQueue; }

	void delClient(Client*);

	int getClientCount() { return m_clientMap.size(); }

private:
	uint32 allocClientId();

public:
	TaskQueue *m_taskQueue;

	uint32 m_allocClientId;

	// 当前在线的客户端列表
	ClientMap m_clientMap;

	// 客户端消息派发器，派发客户端发来的消息
	MsgDispatcher<Client> m_dispatcher;

	// 客户端池
	ObjectPool<Client> m_clientPool;
};

#endif //_clientmgr_h_