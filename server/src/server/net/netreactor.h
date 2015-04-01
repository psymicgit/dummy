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
	virtual void onConnected(Link*, const NetAddress& localAddr, const NetAddress& peerAddr) {}

	virtual void onAccepted(Link*, const NetAddress& localAddr, const NetAddress& peerAddr) {}

	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr) = 0;

	virtual void onRecv(Link*, Buffer&) = 0;

	virtual TaskQueue& getTaskQueue() = 0;
};

#endif //_netreactor_h_