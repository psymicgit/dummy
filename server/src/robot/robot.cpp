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
#include <client.pb.h>
#include <protocol/message.h>
#include <tool/encrypttool.h>
#include <protocol.pb.h>
#include "robotmgr.h"

#include "http/handshakehttpcmd.h"
#include <tool/randtool.h>
#include <tool/echotool.h>
#include <tool/ticktool.h>

#include <protocol.pb.h>
#include <client.pb.h>
#include <basic/evbuffer.h>


Robot::Robot()
	: m_link(NULL)
	, m_robotMgr(NULL)
	, m_isEncrypt(false)
	, m_robotId(0)
	, m_cliversion(110)
	, m_pingpongCount(0)
{
	bzero(m_encryptKey, sizeof(m_encryptKey));

	randomRobot();
}

std::string Robot::name()
{
	return echotool::getmsg("robot<%d>", m_robotId);
}

void Robot::randomRobot()
{
	uint8 randNum[64] = {0};

	m_cliversion = randtool::random(1, 1200);

	randtool::secureRandom(randNum, sizeof(randNum) - 1, 65, 97);
	m_deviceid = std::string("deviceid_") + (const char*)randNum;
	m_username = std::string("username_") + (const char*)randNum;
	m_password = std::string("password_") + (const char*)randNum;
	m_ip = std::string("127.0.0.") + echotool::getmsg("%u", randtool::random(1, 254));
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
	m_robotMgr->m_taskQueue.put(boost::bind(&Robot::handleMsg, this));
}

void Robot::handleMsg()
{

	Link *link = m_link;

	// 1. 将接收缓冲区的数据全部取出
	evbuffer recvSwapBuf;
	evbuffer_init(recvSwapBuf);

	evbuffer *dst = &recvSwapBuf;

	{
		lock_guard_t<> lock(link->m_recvBufLock);
		int size = evbuffer_get_length(&link->m_recvBuf);
		evbuffer_remove_buffer(&link->m_recvBuf, dst, size);
		link->m_isWaitingRead = false;
	}

	while(true) {
		// 检测包头长度
		size_t bytes = evbuffer_get_length(dst);
		if (bytes < sizeof(NetMsgHead)) {
			break;
		}

		NetMsgHead *head = (NetMsgHead *)evbuffer_pullup(dst, sizeof(NetMsgHead));
		uint16 msgId = endiantool::networkToHost16(head->msgId);
		uint32 msgLen = endiantool::networkToHost32(head->msgLen);

		if (msgLen > bytes) {
			break;
		}

		char *peek = (char*)evbuffer_pullup(dst, msgLen);

		// 未加密
		if (!m_isEncrypt) {
			m_robotMgr->m_dispatcher.dispatch(*this, msgId, peek + sizeof(NetMsgHead), msgLen - sizeof(NetMsgHead), 0);
			evbuffer_drain(dst, msgLen);
			continue;
		}

		//先解密
		uint8* encryptBuf =  (uint8*)(peek + sizeof(NetMsgHead));
		int encryptBufLen = msgLen - sizeof(NetMsgHead);

		if(!encrypttool::decrypt(encryptBuf, encryptBufLen, m_encryptKey, sizeof(m_encryptKey))) {
			LOG_ERROR << "robot [" << link->getLocalAddr().toIpPort() << "] <-> gatesvr [" << link->getPeerAddr().toIpPort()
			          << "] receive invalid msg[len=" << encryptBufLen << "]";
			evbuffer_drain(dst, msgLen);
			continue;
		}

		char *msg = (char*)peek + sizeof(NetMsgHead) + EncryptHeadLen;

		// 直接本地进行处理
		m_robotMgr->m_dispatcher.dispatch(*this, msgId, msg, msgLen - sizeof(NetMsgHead) - EncryptHeadLen - EncryptTailLen, 0);
		evbuffer_drain(dst, msgLen);
	}

	// 3. 处理完毕后，若有残余的消息体，则将残余消息体重新拷贝到接收缓冲区的头部以保持正确的数据顺序
	int leftSize = evbuffer_get_length(dst);
	if (leftSize > 0) {
		lock_guard_t<> lock(link->m_recvBufLock);
		int size = evbuffer_get_length(&link->m_recvBuf);

		if (size > 0) {
			evbuffer_prepend_buffer(&link->m_recvBuf, dst);
		} else {
			evbuffer_add_buffer(&link->m_recvBuf, dst);
		}
	}

	evbuffer_free(recvSwapBuf);
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

	if (!m_isEncrypt) {
		m_link->send(msgId, msg);
		return true;
	}

	uint32 headSize = sizeof(NetMsgHead);
	int size = msg.ByteSize();

	bool ok = msg.SerializeToArray(m_link->m_net->g_encryptBuf + headSize + EncryptHeadLen, size);
	if (!ok) {
		LOG_ERROR << "robot<" << m_robotId << "> [" << m_link->getLocalAddr().toIpPort() << "] <-> gatesvr [" << m_link->getPeerAddr().toIpPort()
		          << "] send msg failed, SerializeToArray error, [len=" << size << "] failed, content = [" << msgtool::getMsgDebugString(msg) << "]";

		return false;
	}

	// 添加加解密头尾
	uint8* decryptBuf = (uint8*)(m_link->m_net->g_encryptBuf + headSize);
	int decryptBufLen = size + EncryptHeadLen + EncryptTailLen;

	encrypttool::encrypt(decryptBuf, decryptBufLen, m_encryptKey, sizeof(m_encryptKey));

	NetMsgHead* pHeader = (NetMsgHead*)m_link->m_net->g_encryptBuf;

	int packetLen = msgtool::buildNetHeader(pHeader, msgId, decryptBufLen);
	if (packetLen <= 0) {
		LOG_ERROR << "robot<" << m_robotId << "> [" << m_link->getLocalAddr().toIpPort() << "] <-> gatesvr [" << m_link->getPeerAddr().toIpPort()
		          << "] pakcetLen = " << packetLen;
		return false;
	}

	m_link->send(m_link->m_net->g_encryptBuf, packetLen);

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

void Robot::login()
{

}

void Robot::pingpongTest()
{
	PingPong p;
	p.set_pingpong("123456789|");
	p.set_time(0);

	LOG_INFO << "robot <" << m_robotId << "> start pingpong test, pingpong packet size = " << p.ByteSize();

	send(ePing, p);
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

	for (int i = 0; i < m_robotMgr->m_config.m_speedTestPacketNum; i++) {
		send(eSpeedTest, p);
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
		send(eLatencyTest, p);
	}

	m_link->close();
}