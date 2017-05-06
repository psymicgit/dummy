///<------------------------------------------------------------------------------
//< @file:   gameserver\net\gatelink.cpp
//< @author: 洪坤安
//< @date:   2015年3月8日 21:15:50
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "gatelink.h"
#include "gameserver.h"
#include "GameLogic.h"
#include "logic/GameClientMgr.h"

#include <net/netaddress.h>
#include <net/link.h>
#include <server.h>
#include <protocol/message.h>

void GateLink::onRecv(Link *link)
{
	Server::instance->getTaskQueue().put(boost::bind(&GateLink::handleMsg, this, link));
}

bool GateLink::Init()
{
	GameLogic::RegisterGateMsg(GateToGame_RouteFromClient, GateLink::OnRouteFromClient);
	GameLogic::RegisterGateMsg(GateToGame_RouteLoginRequest, GateLink::OnRouteLogin);

	return true;
}

void GateLink::handleMsg(Link* link)
{
	msgtool::DispatchMsg(link, *this, GameServer::Instance().m_gateDispatcher);
}

void GateLink::OnRouteFromClient(GateLink* gateLink, RouteFromClientMsg *routeFromClientMsg, int64 receiveTime)
{
	int clientId = routeFromClientMsg->client_id();

	GameClient* client = GameClientMgr::instance->FindClient(clientId);
	if (nullptr == client)
	{
		return;
	}

	const std::string& routeMsg = routeFromClientMsg->msg();

	int msgId = routeFromClientMsg->msg_id();
	int msgSize = routeMsg.size();
	GameServer::Instance().m_clientDispatcher.dispatch(*client, msgId, routeMsg.c_str(), msgSize, 0);
}

void GateLink::OnRouteLogin(GateLink* gateLink, RouteLoginRequest* msg, int64 receiveTime)
{
	int clientId = msg->client_id();
	const LoginReq& loginReq = msg->loginreq();

	GameClient* oldClient = GameClientMgr::instance->FindClient(clientId);
	if (oldClient)
	{
		return;
	}

	GameClient* client = GameClientMgr::instance->AddClient(clientId);
	if (nullptr == client)
	{
		return;
	}
}
