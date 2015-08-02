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

Robot::Robot()
	: m_link(NULL)
	, m_robotMgr(NULL)
	, m_isEncrypt(false)
	, m_robotId(0)
	, m_cliversion(110)
{
	bzero(m_encryptKey, sizeof(m_encryptKey));

	randomRobot();
}

Robot::~Robot()
{
	LOG_WARN << "robot ~robot";
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
	m_link = link;

	LOG_INFO << "robot <" << localAddr.toIpPort() << "> connect to <" << peerAddr.toIpPort() << "> success";

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

	m_robotMgr->onRobotDisconnect(this);
}

void Robot::onRecv(Link *link, Buffer &buf)
{
	while(true) {
		// 检测半包
		size_t bytes = buf.readableBytes();
		if (bytes < sizeof(NetMsgHead)) {
			return;
		}

		NetMsgHead *msgHead = (NetMsgHead*)buf.peek();
		uint16 msgId = endiantool::networkToHost16(msgHead->msgId);
		uint32 msgLen = endiantool::networkToHost32(msgHead->msgLen);

		if (msgLen > bytes) {
			return;
		}

		// 未加密
		if (!m_isEncrypt) {
			Buffer copyBuf;
			copyBuf.append(buf.peek() + sizeof(NetMsgHead), msgLen - sizeof(NetMsgHead));

			m_robotMgr->m_taskQueue.put(boost::bind(&RobotMgr::handleMsg, m_robotMgr, this, msgId, copyBuf, 0));
			buf.skip(msgLen);
			continue;
		}

		//先解密
		uint8* encryptBuf =  (uint8*)(buf.peek() + sizeof(NetMsgHead));
		int encryptBufLen = msgLen - sizeof(NetMsgHead);

		if(!encrypttool::decrypt(encryptBuf, encryptBufLen, m_encryptKey, sizeof(m_encryptKey))) {
			LOG_ERROR << "robot [" << link->m_localAddr.toIpPort() << "] <-> gatesvr [" << link->m_peerAddr.toIpPort()
			          << "] receive invalid msg[len=" << encryptBufLen << "]";
			return;
		}

		char *msg = (char*)buf.peek() + sizeof(NetMsgHead) + EncryptHeadLen;

		Buffer copyBuf;
		copyBuf.append(msg, msgLen - sizeof(NetMsgHead) - EncryptHeadLen - EncryptTailLen);

		// 直接本地进行处理
		m_robotMgr->m_taskQueue.put(boost::bind(&RobotMgr::handleMsg, m_robotMgr, this, msgId, copyBuf, 0));
		buf.skip(msgLen);
	}
}

TaskQueue& Robot::getTaskQueue()
{
	return m_robotMgr->m_taskQueue;
}

bool Robot::send(int msgId, Message &msg)
{
	if (!m_link->isopen()) {
		return false;
	}

	if (!m_isEncrypt) {
		m_link->send(msgId, msg);
		return true;
	}

	int size = msg.ByteSize();

	Buffer buf(size);

	bool ok = msg.SerializeToArray((void*)buf.beginWrite(), size);
	if (!ok) {
		LOG_ERROR << "robot [" << m_link->m_localAddr.toIpPort() << "] <-> gatesvr [" << m_link->m_peerAddr.toIpPort()
		          << "] send msg failed, SerializeToArray error, [len=" << size << "] failed, content = [" << msgtool::getMsgString(msg) << "]";

		return false;
	}

	buf.hasWritten(size);

	this->send(msgId, buf.peek(), buf.readableBytes());
	return true;
}

bool Robot::send(int msgId, const char* data, int len)
{
	if (!m_link->isopen()) {
		return false;
	}

	if (!m_isEncrypt) {
		m_link->send(msgId, data, len);
		return true;
	}

	//先加密
	char *netBuf = global::g_packetBuf;
	uint32 headSize = sizeof(NetMsgHead);

	memcpy(netBuf + headSize + EncryptHeadLen, data, len);

	uint8* decryptBuf = (uint8*)(netBuf + headSize);
	int decryptBufLen = len + EncryptHeadLen + EncryptTailLen;//添加加解密头尾

	encrypttool::encrypt(decryptBuf, decryptBufLen, m_encryptKey, sizeof(m_encryptKey));

	NetMsgHead* pHeader = (NetMsgHead*)netBuf;

	int packetLen = msgtool::buildNetHeader(pHeader, msgId, decryptBufLen);
	m_link->send(netBuf, packetLen);

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