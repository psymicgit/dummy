///<------------------------------------------------------------------------------
//< @file:   server\net\serverlink.h
//< @author: ������
//< @date:   2015��3��9�� 11:42:53
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _serverlink_h_
#define _serverlink_h_

#include "protocol/protocol.h"
#include "tool/servertool.h"

#include "net/netdefine.h"

// ��������������������
class ServerLink : public INetLogic
{
public:
	ServerLink()
		: m_link(NULL)
		, m_localSvrType(eNullServer)
		, m_peerSvrType(eNullServer)
		, m_peerSvrId(0)
		, m_taskQueue(NULL)
	{
	}

	virtual ~ServerLink() {}

public:
	virtual std::string name();

	virtual bool Init();

	// �Զ˷������Ͽ�����
	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onRecv(Link*) override;

	virtual TaskQueue& getTaskQueue() { return *m_taskQueue; }

	void SendMsg(int msgId, const Message& msg);

public:
	virtual void onEstablish() {}

	virtual void onTerminate() {}

public:
	Link*		m_link;

	ServerType	m_localSvrType;	// ���ط����������

	ServerType	m_peerSvrType;	// �Զ˷����������

	int			m_peerSvrId;	// �Զ˷�������id

	TaskQueue*	m_taskQueue;
};

#endif //_serverlink_h_