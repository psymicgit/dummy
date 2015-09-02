///<------------------------------------------------------------------------------
//< @file:   gateserver\net\client.cpp
//< @author: 洪坤安
//< @date:   2015年3月14日 21:4:7
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "client.h"

#include <client.pb.h>
#include <protocol.pb.h>
#include <protocol/message.h>

#include "clientmgr.h"
#include "gateserver.h"
#include <net/netaddress.h>
#include <server.h>
#include <tool/encrypttool.h>
#include <tool/randtool.h>

Client::Client()
	: m_link(NULL)
	, m_clientId(0)
	, m_taskQueue(NULL)
	, m_clientMgr(NULL)
	, m_pingCount(0)
	, m_speedTestCount(0)
	, m_latencyTestCount(0)
{
	memset(m_encryptKey, 0, sizeof(m_encryptKey));
}

void Client::onEstablish()
{
	// 随机生成认证串
	randtool::secureRandom(m_authKey, sizeof(m_authKey), '0', 'z');
	randtool::secureRandom(m_encryptKey, sizeof(m_encryptKey), '0', 'z');

	//发送加解密密钥
	EncryptKeyNtf *ntf = msgtool::allocPacket<EncryptKeyNtf>();
	ntf->set_publickey((const char*)m_encryptKey, sizeof(m_encryptKey));
	ntf->set_privatekey((const char*)m_encryptKey, sizeof(m_encryptKey));
	ntf->set_authkey((const char*)m_authKey, sizeof(m_authKey));

	m_link->send(eEncryptKeyNtf, *ntf);
//
// 	Message *msg = ntf;
// 	msgtool::freePacket(msg);
}

std::string Client::name()
{
	return echotool::getmsg("client<%u>", m_clientId);
}

void Client::onDisconnect(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	LOG_INFO << name() << " [" << peerAddr.toIpPort() << "] <-> gatesvr [" << localAddr.toIpPort() << "] broken";
	m_clientMgr->delClient(this);
}

void Client::onRecv(Link *link, Buffer &buf)
{
	m_taskQueue->put(boost::bind(&Client::handleMsg, this));
}

void Client::handleMsg()
{
	Link *link = m_link;
	Buffer buf;

	{
		lock_guard_t<> lock(link->m_recvBufLock);
		link->m_isWaitingRead = false;
		buf.swap(link->m_recvBuf);
	}

	while(true) {
		// 检测包头长度
		size_t bytes = buf.readableBytes();
		if (bytes < sizeof(NetMsgHead)) {
			break;
		}

		NetMsgHead *msgHead = (NetMsgHead*)buf.peek();
		uint16 msgId = endiantool::networkToHost16(msgHead->msgId);
		uint32 dataLen = endiantool::networkToHost32(msgHead->msgLen);

		// 检测半包
		if (dataLen > bytes) {
			// 			LOG_WARN << "gatesvr [" << link->m_localAddr.toIpPort() << "] <-> client [" << link->m_peerAddr.toIpPort()
			// 			          << "] msgLen(" << msgLen << ") > bytes(" << bytes << ")";
			break;
		}

		//先解密
		uint8* encryptBuf =  (uint8*)(buf.peek() + sizeof(NetMsgHead));
		int encryptBufLen = dataLen - sizeof(NetMsgHead);

		if(!encrypttool::decrypt(encryptBuf, encryptBufLen, m_encryptKey, sizeof(m_encryptKey))) {
			LOG_ERROR << "gatesvr [" << link->m_localAddr.toIpPort() << "] <-> client [" << link->m_peerAddr.toIpPort()
			          << "] decrypt msg [len=" << encryptBufLen << "] failed";
			buf.skip(dataLen);
			continue;
		}

		char *msg = (char*)buf.peek() + sizeof(NetMsgHead) + EncryptHeadLen;
		uint32 msgLen = dataLen - sizeof(NetMsgHead) - EncryptHeadLen - EncryptTailLen;

		// 判断是否需要转发，
		if (needRoute(msgId)) {
			// 转发给游戏服务器
			//GateServer::Instance().sendToGameServer(client->m_clientId, msgId, msg, msgLen);
		} else {
			// 直接本地进行处理
			m_clientMgr->m_dispatcher.dispatch(*this, msgId, msg, msgLen, 0);
		}

		buf.skip(dataLen);
	};

	if (!buf.empty()) {
		{
			lock_guard_t<> lock(link->m_recvBufLock);
			if (!link->m_recvBuf.empty()) {
				buf.append(link->m_recvBuf.peek(), link->m_recvBuf.readableBytes());
				link->m_recvBuf.swap(buf);
			} else {
				link->m_recvBuf.swap(buf);
			}
		}
	}
}

bool Client::needRoute(int msgId)
{
	// 判断是否需要转发
	if (eClientGateMsgIdMin < msgId && msgId < eClientGateMsgIdMax) {
		return false;
	}

	return true;
}

bool Client::send(int msgId, Message &msg)
{
	if (!m_link->isopen()) {
		return false;
	}

	uint32 headSize = sizeof(NetMsgHead);
	int size = msg.ByteSize();

	bool ok = msg.SerializeToArray(global::g_encryptBuf + headSize + EncryptHeadLen, size);
	if (!ok) {
		LOG_ERROR << "client [" << m_link->m_peerAddr.toIpPort()
		          << "] send msg failed, SerializeToArray error, [len=" << size << "] failed, content = [" << msgtool::getMsgString(msg) << "]";

		return false;
	}

	// 添加加解密头尾
	uint8* decryptBuf = (uint8*)(global::g_encryptBuf + headSize);
	int decryptBufLen = size + EncryptHeadLen + EncryptTailLen;

	encrypttool::encrypt(decryptBuf, decryptBufLen, m_encryptKey, sizeof(m_encryptKey));

	NetMsgHead* pHeader = (NetMsgHead*)global::g_encryptBuf;

	int packetLen = msgtool::buildNetHeader(pHeader, msgId, decryptBufLen);
	m_link->send(global::g_encryptBuf, packetLen);

	return true;
}

void Client::close()
{
	m_link->close();
}
