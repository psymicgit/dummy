///<------------------------------------------------------------------------------
//< @file:   server\net\serverlink.h
//< @author: 洪坤安
//< @date:   2015年3月9日 11:42:53
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _serverlink_h_
#define _serverlink_h_

#include "net/netreactor.h"
#include "protocol/protocol.h"
#include "tool/servertool.h"

// 服务器与服务器间的连接
class ServerLink : public INetReactor
{
public:
	ServerLink()
		: m_link(NULL)
		, m_peerSvrType(eNullServer)
		, m_peerSvrId(0)
		, m_taskQueue(NULL)
	{
	}

	virtual ~ServerLink() {}

public:
	virtual std::string name();

	// 对端服务器断开连接
	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onRecv(Link*, Buffer&);

	virtual TaskQueue& getTaskQueue() { return *m_taskQueue; }

	void send(uint32 routeId, uint16 msgId, const char *data, int len);

public:
	virtual void onEstablish() {}

	virtual void onTerminate() {}

public:
	Link *m_link;

	// 对端服务器的类别
	ServerType m_peerSvrType;

	// 对端服务器的id
	int m_peerSvrId;

	TaskQueue *m_taskQueue;
};

#endif //_serverlink_h_