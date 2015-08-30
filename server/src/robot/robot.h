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

enum RobotState {
	ROBOT_STATE_AUTHED, // 已经过认证
};

class Robot : public INetReactor
{
public:
	Robot();

	~Robot();

public:
	virtual void onConnected(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onRecv(Link*, Buffer&);

	virtual TaskQueue& getTaskQueue();

	bool send(int msgId, const char* data, int len);

	bool send(int msgId, Message&);

	void start();

	// 1. 与认证中心进行握手
	void handshake();

	// 2. 到认证中心进行认证
	void auth();

	// 3. 登录到游戏服
	void login();

public:
	void pingpongTest();

	void speedTest();

	void latencyTest();

private:
	void randomRobot();

public:
	Link *m_link;
	RobotMgr *m_robotMgr;

	uint8 m_encryptKey[EncryptKeyLen];
	bool m_isEncrypt;
	uint32 m_robotId;

	std::string m_publickey; // 认证中心发送过来的公钥

public:
	int m_cliversion;
	std::string m_deviceid;
	std::string m_username;
	std::string m_password;
	std::string m_ip;

	int m_pingpongCount;
};

#endif //_robot_h_