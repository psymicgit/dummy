///<------------------------------------------------------------------------------
//< @file:   src\robot\robot.cpp
//< @author: 洪坤安
//< @date:   2015年3月16日 11:43:14
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "robot.h"

#include <net/netaddress.h>
#include <net/link.h>
#include <net/netmodel.h>

#include <protocol.pb.h>
#include <client.pb.h>
#include <protocol/message.h>
#include <protocol.pb.h>
#include "http/handshakehttpcmd.h"
#include <tool/randtool.h>
#include <tool/echotool.h>
#include <tool/ticktool.h>
#include <tool/encrypttool.h>

#include <basic/evbuffer.h>

#include "robotmgr.h"

Robot::Robot()
	: m_link(NULL)
	, m_robotMgr(NULL)
	, m_isEncrypt(false)
	, m_robotId(0)
	, m_clientVersion(110)
	, m_pingpongCount(0)
{
	randomRobot();
}

std::string Robot::name()
{
	return echotool::getmsg("robot<%d>", m_robotId);
}

void Robot::randomRobot()
{
	m_clientVersion = randtool::rand_int_between(1, 1200);

	std::string randKey = randtool::rand_string(64);

	m_deviceid = std::string("deviceid_") + randKey;
	m_username = std::string("username_") + randKey;
	m_password = std::string("password_") + randKey;
	m_ip	   = std::string("127.0.0.") + echotool::getmsg("%u", randtool::rand_int_between(1, 254));
}


void Robot::onConnected(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	static int g_connectedRobotCnt = 0;
	++g_connectedRobotCnt;

	m_link = link;

	if (g_connectedRobotCnt % 100 == 0) {
		LOG_INFO << name() << " <" << localAddr.toIpPort() << "> connect to <" << peerAddr.toIpPort() << "> success, current robot cnt = " << m_robotMgr->m_robotMap.size();
	}

	// m_link->send("1\r\n");
}

void Robot::onDisconnect(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	/*
	if (link->m_isCreateByConnector) {
		Robot *robot = m_robotMgr->createRobot();
		m_robotMgr->m_wan.connect(peerAddr.toIp(), peerAddr.toPort(), *robot);
	}
	*/

	//if (m_robotMgr->m_robotMap.size() >= 0) {
	LOG_INFO << "robot<" << m_robotId << "> <" << localAddr.toIpPort() << "> to gateserver <" << peerAddr.toIpPort() << "> closed! current robot cnt = " << m_robotMgr->m_robotMap.size() - 1;
	//}

	m_robotMgr->onRobotDisconnect(this);
}

void Robot::onRecv(Link *link)
{
	// 直接本地进行处理
	m_robotMgr->m_taskQueue.put(boost::bind(&Robot::HandleMsg, this));
}

void Robot::HandleMsg()
{
	msgtool::DispatchMsg(m_link, *this, m_robotMgr->m_dispatcher);
}

TaskQueue& Robot::getTaskQueue()
{
	return m_robotMgr->m_taskQueue;
}

bool Robot::send(int msgId, Message &msg)
{
	if (!m_link->isopen()) {
		// LOG_ERROR << "robot<" << m_robotId << "> [" << m_link->m_localAddr.toIpPort() << "] <-> gatesvr [" << m_link->m_peerAddr.toIpPort() << "] is not open";
		return false;
	}

	// 不加密
	if (m_encryptKey.empty()) {
		m_link->send(msgId, msg);
	}
	// 加密
	else
	{
	}

	return true;
}

void Robot::start()
{
	handshake();
}

void Robot::handshake()
{
	HandShakeHttpCmd *handshakeHttpCmd = new HandShakeHttpCmd;
	handshakeHttpCmd->m_robot = this;
	handshakeHttpCmd->init();

	m_robotMgr->m_httpMgr.addCmd(handshakeHttpCmd);
}

void Robot::auth()
{

}

void Robot::Login()
{
	LoginReq msg;
	msg.set_clientversion(100);
	msg.set_deviceid("1273ab23c3390fe840a9e0");
	msg.set_notifyid("notifyid-00134678");
	msg.set_username("psy_micgit");
	msg.set_zoneid(1);
	msg.set_userid(100000001);
	msg.set_playerid(14560034);
	msg.set_job("sword");
	msg.set_logintype(LoginReq_LoginType_NewRole);
	msg.set_authtype(1);
	msg.set_authkey("2ab456b6b2b1b6b1bb2b");

	send(ClientMsg_LoginRequest, msg);
}

void Robot::pingpongTest()
{
	PingPong p;
	p.set_pingpong("123456789|");
	p.set_time(0);

	LOG_INFO << "robot <" << m_robotId << "> start pingpong test, pingpong packet size = " << p.ByteSize();

	send(ClientMsg_PingRequest, p);
}

void Robot::speedTest()
{
	static int g_speedTestCnt = 0;

	g_speedTestCnt++;

	PingPong p;
	p.set_pingpong("0", m_robotMgr->m_config.m_speedTestPacketSize);
	p.set_time(0);

	if (g_speedTestCnt % 100 == 0) {
		LOG_INFO << name() << " start speed test, g_speedTestCnt = " << g_speedTestCnt << ", speed packet size = " << p.ByteSize();
	}

	// Tick tick("speed test", m_robotMgr->m_config.m_speedTestPacketNum);

	for (int i = 0; i < m_robotMgr->m_config.m_speedTestPacketNum; i++) {
		send(ClientMsg_SpeedTestRequest, p);
	}

	m_link->close();
	// RobotMgr::Instance().stop();
	// 	double costTime = tick.endTick();
	// 	double avgTime = costTime / count;
	// 	double speed = 1.0f / avgTime;
	// 	LOG_WARN << "send packet count = " << count << ", avg cost time = " << avgTime << ", total cost time = " << costTime << ", exe count per second = " << speed;
	// 	exit(0);
}

void Robot::latencyTest()
{
	static int g_latencyTestCnt = 0;

	g_latencyTestCnt++;

	PingPong p;
	p.set_pingpong("12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");

	if (g_latencyTestCnt % 100 == 0) {
		LOG_INFO << "robot <" << m_robotId << "> start latency test, g_latencyTestCnt = " << g_latencyTestCnt << ", latency packet size = " << p.ByteSize();
	}

	for (int i = 0; i < 1000; i++) {
		p.set_time(ticktool::tick());
		send(ClientMsg_LatencyTestRequest, p);
	}

	m_link->close();
}

// 开始移动测试
void Robot::MoveTest()
{
	float randX = randtool::rand_float_between(-10, 10);
	float randY = randtool::rand_float_between(-5, 5);
	float randZ = randtool::rand_float_between(-2, 40);

	MoveRequest msg;
	msg.set_x(randX);
	msg.set_y(randY);
	msg.set_z(randZ);
	send(ClientMsg_MoveRequest, msg);
}