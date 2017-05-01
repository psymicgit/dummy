///<------------------------------------------------------------------------------
//< @file:   gameserver\net\gatelink.cpp
//< @author: ������
//< @date:   2015��3��8�� 21:15:50
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
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
