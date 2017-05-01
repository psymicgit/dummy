///<------------------------------------------------------------------------------
//< @file:   gateserver\net\gamesvrlink.h
//< @author: ������
//< @date:   2015��3��9�� 16:14:42
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gamesvrlink_h_
#define _gamesvrlink_h_

#include <net/netdefine.h>
#include <net/serverlink.h>

class Link;
class RouteToClientMsg;

class GameSvrLink : public ServerLink
{
public:
	bool Init() override;

	void onRecv(Link*) override;

public:
	static void OnRouteToClient(GameSvrLink* gameLink, RouteToClientMsg* routeToClientMsg, int64 receiveTime);
};

#endif //_gamesvrlink_h_