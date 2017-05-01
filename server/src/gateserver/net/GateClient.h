///<------------------------------------------------------------------------------
//< @file:   gateserver\net\client.h
//< @author: ������
//< @date:   2015��3��14�� 20:29:53
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _client_h_
#define _client_h_

#include <net/netdefine.h>
#include <client.pb.h>

class Link;
class GateClientMgr;

// �ͻ�������
class GateClient : public INetLogic
{
public:
	GateClient();

public:
	std::string name();

	// @override
	virtual void		onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void		onRecv(Link*);

	virtual TaskQueue&	getTaskQueue() { return *m_taskQueue; }

public:
	// ִ�б��ͻ������ӳɹ�����ʱ����Ҫִ�еĳ�ʼ������
	void onEstablish();

	// ������Ϣ��
	bool send(int msgId, Message&);

	// �رձ��ͻ�������
	void close();

	// �������ͻ����������յ�������
	void handleMsg();

	// �Ƿ���Ҫת������Ϸ��
	bool NeedRouteToGame(int msgId);

public:

	Link*			m_link;			// ��ͻ��˵�tcpԭʼ����
	uint32			m_clientId;
	TaskQueue*		m_taskQueue;
	GateClientMgr*		m_clientMgr;
	std::string		m_encryptKey;	// ������Կ
	std::string		m_authKey;		// ��֤��Կ

public:
	int			m_pingCount;
	int			m_speedTestCount;
	int			m_latencyTestCount;
};

#endif //_client_h_