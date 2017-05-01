///<------------------------------------------------------------------------------
//< @file:   gateserver\net\clientmgr.cpp
//< @author: 洪坤安
//< @date:   2015年3月14日 20:16:43
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "GateClientMgr.h"

#include <net/netaddress.h>
#include <net/link.h>
#include <server.h>
#include <tool/sockettool.h>
#include <tool/encrypttool.h>

#include "GateClient.h"
#include "GateLogic.h"
#include "clientmsghandler.h"

GateClientMgr::GateClientMgr()
	: m_taskQueue(NULL)
	, m_allocClientId(0)
{
	
}

GateClientMgr::~GateClientMgr()
{
	m_clientPool.clear();
	m_dispatcher.clear();
}

bool GateClientMgr::Init()
{
	GateLogic::RegisterClientMsg(ClientMsg_LoginRequest, ClientMsgHandler::OnLoginReq);
	GateLogic::RegisterClientMsg(ClientMsg_AuthRequest, ClientMsgHandler::OnAuthReq);

	// 测试
	GateLogic::RegisterClientMsg(ClientMsg_PingRequest, ClientMsgHandler::OnPingTest);
	GateLogic::RegisterClientMsg(ClientMsg_SpeedTestRequest, ClientMsgHandler::OnSpeedTest);
	GateLogic::RegisterClientMsg(ClientMsg_LatencyTestRequest, ClientMsgHandler::OnLatencyTest);

	return true;
}

void GateClientMgr::close()
{
	for(ClientMap::iterator itr = m_clientMap.begin(); itr != m_clientMap.end(); ++itr) {
		GateClient *client = itr->second;
		client->close();
	}

	// m_clientMap.clear();
}

void GateClientMgr::onAccepted(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	uint32 newClientId = allocClientId();

	GateClient *client			= m_clientPool.alloc();
	client->m_link			= link;
	client->m_clientId		= newClientId;
	client->m_clientMgr		= this;
	client->m_taskQueue		= m_taskQueue;

	link->m_logic = client;
	client->onEstablish();

	m_clientMap[newClientId] = client;

// 	static int clientNum = 0;
// 	clientNum++;
//
// 	if (clientNum % 10 == 0) {
// 		LOG_INFO << "clientNum = " << clientNum << ", m_clientMap.size() = " << m_clientMap.size();
// 	}

	if (m_clientMap.size() % 100 == 0) {
		LOG_INFO << "<" << localAddr.toIpPort() << "> accept new client from <" << peerAddr.toIpPort() << ">, m_clientMap.size()=" << m_clientMap.size();
	}
}

void GateClientMgr::onDisconnect(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{

}

void GateClientMgr::onRecv(Link *link)
{
	Server::instance->onRecv(link);
}

uint32 GateClientMgr::allocClientId()
{
	return ++m_allocClientId;
}

void GateClientMgr::delClient(GateClient *client)
{
	// LOG_INFO << "client <" << client->m_link->m_peerAddr.toIpPort() << "> disconnet";

	m_clientMap.erase(client->m_clientId);
	m_clientPool.free(client);
}