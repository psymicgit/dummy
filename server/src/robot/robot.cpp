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

Robot::Robot()
	: m_isEncrypt(false)
	, m_link(NULL)
{
}

void Robot::onConnected(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	m_link = link;

	LOG_INFO << "robot <" << localAddr.toIpPort() << "> connect to <" << peerAddr.toIpPort() << "> success";

// 	LoginReq req;
// 	req.set_clientversion(100);
// 	req.set_deviceid("1273ab23c3390fe840a9e0");
// 	req.set_notifyid("notifyid-00134678");
// 	req.set_username("psy_micgit");
// 	req.set_zoneid(1);
// 	req.set_userid(100000001);
// 	req.set_playerid(14560034);
// 	req.set_job("sword");
// 	req.set_logintype(LoginReq_LoginType_NewRole);
// 	req.set_authtype(1);
// 	req.set_authkey("2ab456b6b2b1b6b1bb2b");
//
// 	link->send(eLoginReq, req);
}

void Robot::onDisconnect(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	if (link->m_isCreateByConnector) {
		Robot *robot = new Robot;
		robot->m_taskQueue = m_taskQueue;
		robot->m_robotMgr = m_robotMgr;

		m_robotMgr->m_wan.connect(peerAddr.toIp(), peerAddr.toPort(), *robot);
	}

	delete this;
}

void Robot::onRecv(Link *link, Buffer &buf)
{
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

		m_robotMgr->m_taskQueue.put(task_binder_t::gen(&RobotMgr::handleMsg, m_robotMgr, *this, msgId, copyBuf, 0));
		buf.retrieve(msgLen);
		return;
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
	m_robotMgr->m_taskQueue.put(task_binder_t::gen(&RobotMgr::handleMsg, m_robotMgr, *this, msgId, copyBuf, 0));
	buf.retrieve(msgLen);
}

bool Robot::send(int msgId, Message &msg)
{
	if (!m_isEncrypt) {
		m_link->send(msgId, msg);
		return true;
	}

	int size = msg.ByteSize();

	Buffer buf;

	msg.SerializeToArray((void*)buf.beginWrite(), size);
	buf.hasWritten(size);

	this->send(msgId, buf.peek(), buf.readableBytes());
	return true;
}

bool Robot::send(int msgId, const char* data, int len)
{
	if (!m_isEncrypt) {
		m_link->send(msgId, data, len);
		return true;
	}

	//先加密
	char netBuf[102400] = {0};
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
