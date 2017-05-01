///<------------------------------------------------------------------------------
//< @file:   src\gateserver\net\clientmgr.h
//< @author: 洪坤安
//< @date:   2015年3月14日 20:14:21
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _clientmgr_h_
#define _clientmgr_h_

#include <net/netdefine.h>
#include <net/msgdispatcher.h>
#include <net/GateClient.h>
#include <basic/objectpool.h>

class GateClient;

// 客户端管理中心，管理当前在线的客户端连接实例
class GateClientMgr : public INetLogic, public Singleton<GateClientMgr>
{
private:
	typedef std::map<int, GateClient*> ClientMap;

public:
	GateClientMgr();
	~GateClientMgr();

	bool Init();

	void close();

public:
	virtual std::string name() { return "clientmgr"; }

	virtual void onAccepted(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onRecv(Link*);

	virtual TaskQueue& getTaskQueue() { return *m_taskQueue; }

	void delClient(GateClient*);

	int getClientCount() { return m_clients.size(); }

	GateClient* FindClient(int clientId);

private:
	int allocClientId();

public:
	TaskQueue* m_taskQueue;
	int m_allocClientId;
	ClientMap m_clients;						// 当前在线的客户端列表
	MsgDispatcher<GateClient> m_dispatcher;		// 客户端消息派发器，派发客户端发来的消息
	ObjectPool<GateClient> m_clientPool;		// 客户端池
};

#endif //_clientmgr_h_