///<------------------------------------------------------------------------------
//< @file:   gameserver\net\gatelink.h
//< @author: ������
//< @date:   2015��3��8�� 21:15:34
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gatelink_h_
#define _gatelink_h_

#include <net/netdefine.h>
#include <net/serverlink.h>

class Link;
class NetAddress;
class Buffer;

class GateLink : public ServerLink
{
	virtual void onRecv(Link*) override;

	void handleMsg(Link* link);
};

#endif //_gatelink_h_