///<------------------------------------------------------------------------------
//< @file:   gateserver\net\gamesvrlink.h
//< @author: 洪坤安
//< @date:   2015年3月9日 16:14:42
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gamesvrlink_h_
#define _gamesvrlink_h_

#include <net/netreactor.h>
#include <net/serverlink.h>

class Link;
class NetAddress;
class Buffer;

class GameSvrLink : public ServerLink
{
	virtual void onRecv(Link*, Buffer &buf);
};

#endif //_gamesvrlink_h_