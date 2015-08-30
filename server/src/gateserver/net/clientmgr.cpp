///<------------------------------------------------------------------------------
//< @file:   gateserver\net\clientmgr.cpp
//< @author: 洪坤安
//< @date:   2015年3月14日 20:16:43
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "clientmgr.h"

#include <net/netaddress.h>
#include <net/link.h>
#include <server.h>

#include <tool/sockettool.h>
#include <tool/encrypttool.h>

#include "client.h"
#include "clientmsghandler.h"

ClientMgr::ClientMgr()
	: m_taskQueue(NULL)
	, m_allocClientId(0)
{
	m_dispatcher.addMsgHandler(new ClientMsgHandler(&m_dispatcher));
}

void ClientMgr::close()
{
	for(ClientMap::iterator itr = m_clientMap.begin(); itr != m_clientMap.end(); ++itr) {
		Client *client = itr->second;
		client->close();
	}

	clear();
}

void ClientMgr::onAccepted(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	LOG_INFO << "<" << localAddr.toIpPort() << "> accept new client from <" << peerAddr.toIpPort() << ">";

	uint32 newClientId = allocClientId();

	Client *client = m_clientPool.alloc();
	client->m_link = link;
	client->m_clientId = newClientId;
	client->m_clientMgr = this;
	client->m_taskQueue = m_taskQueue;

	link->m_pNetReactor = client;
	client->onEstablish();

	m_clientMap[newClientId] = client;

	static int clientNum = 0;
	clientNum++;

	if (clientNum % 10 == 0) {
		LOG_INFO << "clientNum = " << clientNum << ", m_clientMap.size() = " << m_clientMap.size();
	}
}

void ClientMgr::onDisconnect(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{

}

void ClientMgr::onRecv(Link *link, Buffer &buf)
{
	Server::instance->onRecv(link, buf);
}

void ClientMgr::handleMsg(Client *client)
{
	Link *link = client->m_link;
	Buffer &buf = link->m_recvBuf;

	lock_guard_t<fast_mutex> lock(link->m_recvBufLock);
	link->m_isWaitingRead = false;

	while(true) {
		// 检测包头长度
		size_t bytes = buf.readableBytes();
		if (bytes < sizeof(NetMsgHead)) {
			return;
		}

		NetMsgHead *msgHead = (NetMsgHead*)buf.peek();
		uint16 msgId = endiantool::networkToHost16(msgHead->msgId);
		uint32 dataLen = endiantool::networkToHost32(msgHead->msgLen);

		// 检测半包
		if (dataLen > bytes) {
			// 			LOG_WARN << "gatesvr [" << link->m_localAddr.toIpPort() << "] <-> client [" << link->m_peerAddr.toIpPort()
			// 			          << "] msgLen(" << msgLen << ") > bytes(" << bytes << ")";
			return;
		}

		//先解密
		uint8* encryptBuf =  (uint8*)(buf.peek() + sizeof(NetMsgHead));
		int encryptBufLen = dataLen - sizeof(NetMsgHead);

		if(!encrypttool::decrypt(encryptBuf, encryptBufLen, client->m_encryptKey, sizeof(client->m_encryptKey))) {
			LOG_ERROR << "gatesvr [" << link->m_localAddr.toIpPort() << "] <-> client [" << link->m_peerAddr.toIpPort()
			          << "] decrypt msg [len=" << encryptBufLen << "] failed";
			buf.skip(dataLen);
			continue;
		}

		char *msg = (char*)buf.peek() + sizeof(NetMsgHead) + EncryptHeadLen;
		uint32 msgLen = dataLen - sizeof(NetMsgHead) - EncryptHeadLen - EncryptTailLen;

		// 判断是否需要转发，
		if (client->needRoute(msgId)) {
			// 转发给游戏服务器
			//GateServer::Instance().sendToGameServer(client->m_clientId, msgId, msg, msgLen);
		} else {
			// 直接本地进行处理
			m_dispatcher.dispatch(*client, msgId, msg, msgLen, 0);
		}

		buf.skip(dataLen);
	};


// 	{
// 		lock_guard_t<fast_mutex> lock(client->m_link->m_recvBufLock);
// 		char *msg = client->m_link->m_recvBuf.begin() + start;
// 	}
}

uint32 ClientMgr::allocClientId()
{
	return ++m_allocClientId;
}

void ClientMgr::delClient(Client *client)
{
	// LOG_INFO << "client <" << client->m_link->m_peerAddr.toIpPort() << "> disconnet";

	m_clientMap.erase(client->m_clientId);
	m_clientPool.free(client);
}

void ClientMgr::clear()
{
	m_clientPool.clear();
	m_dispatcher.clear();
}