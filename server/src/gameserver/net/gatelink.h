///<------------------------------------------------------------------------------
//< @file:   gameserver\net\gatelink.h
//< @author: 洪坤安
//< @date:   2015年3月8日 21:15:34
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gatelink_h_
#define _gatelink_h_

#include <net/netdefine.h>
#include <net/serverlink.h>

class Link;
class NetAddress;
class Buffer;
class RouteFromClientMsg;
class RouteLoginRequest;

class GateLink : public ServerLink
{
	bool Init() override;

	virtual void onRecv(Link*) override;

	void handleMsg(Link* link);

public:
	static void OnRouteFromClient(GateLink* gateLink, RouteFromClientMsg *msg, int64 receiveTime);
	static void OnRouteLogin(GateLink* gateLink, RouteLoginRequest* msg, int64 receiveTime);
};

#endif //_gatelink_h_