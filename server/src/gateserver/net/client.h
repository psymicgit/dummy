///<------------------------------------------------------------------------------
//< @file:   gateserver\net\client.h
//< @author: 洪坤安
//< @date:   2015年3月14日 20:29:53
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _client_h_
#define _client_h_

#include <net/netdefine.h>
#include <client.pb.h>

class Link;
class ClientMgr;

// 客户端连接
class Client : public INetLogic
{
public:
	Client();

public:
	std::string name();

	// @override
	virtual void		onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void		onRecv(Link*);

	virtual TaskQueue&	getTaskQueue() { return *m_taskQueue; }

public:
	// 执行本客户端连接成功建立时所需要执行的初始化操作
	void onEstablish();

	// 发送消息包
	bool send(int msgId, Message&);

	// 关闭本客户端连接
	void close();

	// 出来本客户端连接已收到的数据
	void handleMsg();

	// 是否需要转发到游戏服
	bool NeedRouteToGame(int msgId);

public:

	Link*		m_link;							// 与客户端的tcp原始连接

	uint32		m_clientId;

	TaskQueue*	m_taskQueue;

	ClientMgr*	m_clientMgr;

	uint8		m_encryptKey[EncryptKeyLen];	// 加密密钥

	uint8		m_authKey[AuthKeyLen];			// 认证密钥

public:
	int			m_pingCount;
	int			m_speedTestCount;
	int			m_latencyTestCount;
};

#endif //_client_h_