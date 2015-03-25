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

typedef google::protobuf::Message Message;

class Robot : public INetReactor
{
public:
	Robot();

public:
	virtual void OnConnected(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void OnDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void OnRecv(Link*, Buffer&);

	bool send(int msgId, const char* data, int len);

	bool send(int msgId, Message&);

	virtual TaskQueue& GetTaskQueue() { return *m_taskQueue;}

public:
	Link *m_link;
	TaskQueue *m_taskQueue;
	RobotMgr *m_robotMgr;

	UINT8 m_encryptKey[EncryptKeyLen];
	bool m_isEncrypt;
};

#endif //_robot_h_