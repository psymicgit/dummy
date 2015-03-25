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
	virtual void OnDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void OnRecv(Link*, Buffer&);

	virtual TaskQueue& GetTaskQueue() { return *m_taskQueue; }

public:
	void onEstablish();

	bool send(int msgId, const char* data, int len);

private:
	bool needRoute(int msgId);

public:
	Link *m_link;
	uint32 m_clientId;

	TaskQueue *m_taskQueue;
	ClientMgr *m_clientMgr;

	UINT8 m_encryptKey[EncryptKeyLen];

};

#endif //_client_h_