///<------------------------------------------------------------------------------
//< @file:   gateserver\net\gamesvrlink.cpp
//< @author: 洪坤安
//< @date:   2015年3月9日 16:14:33
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "gamesvrlink.h"

#include <net/netaddress.h>
#include <net/link.h>
#include <basic/taskqueue.h>
#include <protocol/message.h>

#include <server.h>

void GameSvrLink::OnRecv(Link *link, Buffer &buf)
{
	ServerLink::OnRecv(link, buf);
}
