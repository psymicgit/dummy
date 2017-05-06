///<------------------------------------------------------------------------------
//< @file:   gateserver\net\gamesvrlink.cpp
//< @author: ������
//< @date:   2015��3��9�� 16:14:33
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#include "gamesvrlink.h"

#include <net/netaddress.h>
#include <net/link.h>
#include <basic/taskqueue.h>
#include <server.h>
#include "GateLogic.h"

bool GameSvrLink::Init()
{
	GateLogic::RegisterGameMsg(GameToGate_RouteToClient, GameSvrLink::OnRouteToClient);
	return true;
}

void GameSvrLink::onRecv(Link *link)
{
	ServerLink::onRecv(link);
}

void GameSvrLink::OnRouteToClient(GameSvrLink& gameLink, RouteToClientMsg& routeToClientMsg, int64 receiveTime)
{
	int clientId = routeToClientMsg.client_id();

	GateClient* client = GateClientMgr::instance->FindClient(clientId);
	if (nullptr == client)
	{
		return;
	}

	const std::string& routeMsg = routeToClientMsg.msg();
	int msgId = routeToClientMsg.msg_id();
	int msgSize = routeMsg.size();

	client->Send(msgId, routeMsg.c_str(), msgSize);
}