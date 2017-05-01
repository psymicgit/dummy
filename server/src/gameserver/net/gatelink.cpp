///<------------------------------------------------------------------------------
//< @file:   gameserver\net\gatelink.cpp
//< @author: 洪坤安
//< @date:   2015年3月8日 21:15:50
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "gatelink.h"

#include <net/netaddress.h>
#include <net/link.h>

#include <server.h>
#include <protocol/message.h>

#include "logic/clientmgr.h"

void GateLink::onRecv(Link *link)
{
	Server::instance->getTaskQueue().put(boost::bind(&ClientMgr::handleMsg, &ClientMgr::Instance(), link));
}
