///<------------------------------------------------------------------------------
//< @file:   src\robot\robot.h
//< @author: 洪坤安
//< @date:   2015年3月16日 11:32:28
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _robot_h_
#define _robot_h_

#include <net/netreactor.h>
#include <client.pb.h>

class TaskQueue;
class RobotMgr;

class Robot : public INetReactor
{
public:
	Robot();

public:
	virtual void onConnected(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onRecv(Link*, Buffer&);

	bool send(int msgId, const char* data, int len);

	bool send(int msgId, Message&);

	void close();

	virtual TaskQueue& getTaskQueue() { return *m_taskQueue;}

public:
	Link *m_link;
	TaskQueue *m_taskQueue;
	RobotMgr *m_robotMgr;

	uint8 m_encryptKey[EncryptKeyLen];
	bool m_isEncrypt;
	uint32 m_robotId;
};

#endif //_robot_h_