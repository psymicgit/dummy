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

void ClientMgr::handleMsg(Client *client, int msgId, Buffer &buf, Timestamp receiveTime)
{
	m_dispatcher.dispatch(*client, msgId, buf.peek(), buf.readableBytes(), receiveTime);
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