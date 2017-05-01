///<------------------------------------------------------------------------------
//< @file:   src\robot\robot.h
//< @author: ������
//< @date:   2015��3��16�� 11:32:28
//< @brief:  ������
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _robot_h_
#define _robot_h_

#include <client.pb.h>
#include <net/netdefine.h>

class TaskQueue;
class RobotMgr;

enum RobotState {
	ROBOT_STATE_AUTHED, // �Ѿ�����֤
};

// ������
class Robot : public INetLogic
{
public:
	Robot();

	virtual ~Robot() {}

	virtual std::string name();

public:
	virtual void onConnected(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	virtual void onRecv(Link*);

	void handleMsg();

	virtual TaskQueue& getTaskQueue();

	bool send(int msgId, Message&);

	void start();

	// 1. ����֤���Ľ�������
	void handshake();

	// 2. ����֤���Ľ�����֤
	void auth();

	// 3. ��¼����Ϸ��
	void Login();

public:
	// ��ʼping-pong����
	void pingpongTest();

	// ��ʼ�ٶȲ���
	void speedTest();

	// ��ʼ�ӳٲ���
	void latencyTest();

	// ��ʼ�ƶ�����
	void MoveTest();

private:
	void randomRobot();

public:
	Link*		m_link;
	RobotMgr*	m_robotMgr;

	std::string	m_encryptKey;
	bool		m_isEncrypt;
	uint32		m_robotId;

	int			m_clientVersion;
	int			m_pingpongCount;

	std::string m_publickey; // ��֤���ķ��͹����Ĺ�Կ
	std::string m_deviceid;
	std::string m_username;
	std::string m_password;
	std::string m_ip;
};

#endif //_robot_h_