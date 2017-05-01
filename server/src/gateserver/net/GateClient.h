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
	std::string name();

	// @override
	virtual void		onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void		onRecv(Link*);

	virtual TaskQueue&	getTaskQueue() { return *m_taskQueue; }

public:
	bool Init();

	// ִ�б��ͻ������ӳɹ�����ʱ����Ҫִ�еĳ�ʼ������
	void onEstablish();

	// ������Ϣ��
	bool SendMsg(int msgId, Message&);

	// ������Ϣ��
	bool Send(int msgId, const char* msg, int msgSize);

	// �رձ��ͻ�������
	void close();

	// �������ͻ����������յ�������
	void handleMsg();

	// �Ƿ���Ҫת������Ϸ��
	bool NeedRouteToGame(int msgId);

public:

	Link*			m_link;			// ��ͻ��˵�tcpԭʼ����
	int				m_clientId;
	TaskQueue*		m_taskQueue;
	std::string		m_encryptKey;	// ������Կ
	std::string		m_authKey;		// ��֤��Կ

public:
	int			m_pingCount;
	int			m_speedTestCount;
	int			m_latencyTestCount;
};

#endif //_client_h_