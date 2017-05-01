///<------------------------------------------------------------------------------
//< @file:   src\gateserver\net\clientmgr.h
//< @author: ������
//< @date:   2015��3��14�� 20:14:21
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _clientmgr_h_
#define _clientmgr_h_

#include <net/netdefine.h>
#include <net/msgdispatcher.h>
#include <net/GateClient.h>
#include <basic/objectpool.h>

class GateClient;

// �ͻ��˹������ģ�����ǰ���ߵĿͻ�������ʵ��
class GateClientMgr : public INetLogic
{
private:
	typedef std::tr1::unordered_map<uint32, GateClient*> ClientMap;

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

	int getClientCount() { return m_clientMap.size(); }

private:
	uint32 allocClientId();

public:
	TaskQueue*				m_taskQueue;

	uint32					m_allocClientId;

	ClientMap				m_clientMap;		// ��ǰ���ߵĿͻ����б�

	MsgDispatcher<GateClient>	m_dispatcher;		// �ͻ�����Ϣ�ɷ������ɷ��ͻ��˷�������Ϣ

	ObjectPool<GateClient>		m_clientPool;		// �ͻ��˳�
};

#endif //_clientmgr_h_