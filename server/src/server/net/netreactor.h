///<------------------------------------------------------------------------------
//< @file:   server\net\netreactor.h
//< @author: 洪坤安
//< @date:   2015年1月28日 15:36:14
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _netreactor_h_
#define _netreactor_h_

class Link;
class NetAddress;
class Buffer;
class TaskQueue;

class INetReactor
{
public:
	virtual void OnConnected(Link*, const NetAddress& localAddr, const NetAddress& peerAddr) {}

	virtual void OnAccepted(Link*, const NetAddress& localAddr, const NetAddress& peerAddr) {}

	virtual void OnDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr) = 0;

	virtual void OnRecv(Link*, Buffer&) = 0;

	virtual TaskQueue& GetTaskQueue() = 0;
};

#endif //_netreactor_h_