///<------------------------------------------------------------------------------
//< @file:   gateserver\net\userlink.cpp
//< @author: 洪坤安
//< @date:   2015年3月14日 20:14:56
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "userlink.h"

#include "server.h"

void UserLink::OnDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr)
{

}

void UserLink::OnRecv(Link *link)
{

}

TaskQueue& UserLink::GetTaskQueue()
{
	return Server::instance->getTaskQueue();
}