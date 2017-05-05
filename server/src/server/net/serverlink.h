///<------------------------------------------------------------------------------
//< @file:   server\net\serverlink.h
//< @author: 洪坤安
//< @date:   2015年3月9日 11:42:53
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _serverlink_h_
#define _serverlink_h_

#include "protocol/protocol.h"
#include "tool/servertool.h"

#include "net/netdefine.h"

// 服务器与服务器间的连接
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

	// 对端服务器断开连接
	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onRecv(Link*) override;

	virtual TaskQueue& getTaskQueue() { return *m_taskQueue; }

	void SendMsg(int msgId, const Message& msg);

public:
	virtual void onEstablish() {}

	virtual void onTerminate() {}

public:
	Link*		m_link;

	ServerType	m_localSvrType;	// 本地服务器的类别

	ServerType	m_peerSvrType;	// 对端服务器的类别

	int			m_peerSvrId;	// 对端服务器的id

	TaskQueue*	m_taskQueue;
};

#endif //_serverlink_h_