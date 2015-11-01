///<------------------------------------------------------------------------------
//< @file:   gateserver\net\userlink.h
//< @author: 洪坤安
//< @date:   2015年3月14日 20:14:51
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _userlink_h_
#define _userlink_h_

#include <net/netdefine.h>
#include <net/serverlink.h>

class Link;
class NetAddress;

class UserLink : public INetLogic
{
	virtual void OnDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void OnRecv(Link*);

	virtual TaskQueue& GetTaskQueue();
};


#endif //_userlink_h_