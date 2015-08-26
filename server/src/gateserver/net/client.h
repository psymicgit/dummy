///<------------------------------------------------------------------------------
//< @file:   gateserver\net\client.h
//< @author: 洪坤安
//< @date:   2015年3月14日 20:29:53
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _client_h_
#define _client_h_

#include <net/netreactor.h>
#include <client.pb.h>

class Link;
class ClientMgr;

class Client : public INetReactor
{
public:
	Client();

public:
	// @override
	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	void onRecvBlock(Link*, RingBufferBlock*);

	virtual void onRecv(Link*, Buffer&, RingBufferBlock&);

	virtual TaskQueue& getTaskQueue() { return *m_taskQueue; }

public:
	void onEstablish();

	bool send(int msgId, const char* data, int len);

	bool send(int msgId, Message&);

private:
	bool needRoute(int msgId);

public:
	Link *m_link;
	uint32 m_clientId;

	TaskQueue *m_taskQueue;
	ClientMgr *m_clientMgr;

	uint8 m_encryptKey[EncryptKeyLen];

	uint8 m_authKey[AuthKeyLen];

public:
	int m_pingCount;
	int m_speedTestCount;
	int m_latencyTestCount;
};

#endif //_client_h_