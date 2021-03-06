///<------------------------------------------------------------------------------
//< @file:   gateserver\net\client.cpp
//< @author: 洪坤安
//< @date:   2015年3月14日 21:4:7
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "GateClient.h"


#include <client.pb.h>
#include <game_to_gate.pb.h>
#include <protocol.pb.h>
#include <protocol/message.h>
#include <net/netaddress.h>
#include <net/netmodel.h>
#include <tool/encrypttool.h>
#include <tool/randtool.h>
#include <basic/evbuffer.h>
#include <server.h>

#include "GateClientMgr.h"
#include "gateserver.h"
#include "GateLogic.h"

bool GateClient::Init()
{
	m_link = nullptr;
	m_clientId = 0;
	m_taskQueue = nullptr;
	m_pingCount = 0;
	m_speedTestCount = 0;
	m_latencyTestCount = 0;

	return true;
}

void GateClient::onEstablish()
{
	// 随机生成认证串
	// m_encryptKey = randtool::rand_string(EncryptKeyLen);
	m_authKey = randtool::rand_string(AuthKeyLen);

	//发送加解密密钥
	EncryptKeyNtf ntf;
	ntf.set_publickey(m_encryptKey);
	ntf.set_privatekey(m_encryptKey);
	ntf.set_authkey(m_authKey);

	m_link->send(ServerMsg_EncryptKeyNotify, ntf);
}

std::string GateClient::name()
{
	return echotool::getmsg("client<%u>", m_clientId);
}

void GateClient::onDisconnect(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	int clientCount = GateClientMgr::Instance().getClientCount();
	if ((clientCount - 1) % 100 == 0) {
		LOG_INFO << name() << " [" << peerAddr.toIpPort() << "] <-> gatesvr [" << localAddr.toIpPort() << "] broken! current client cnt = " << clientCount - 1;
	}

	ClientDisconnectMsg msg;
	msg.set_client_id(m_clientId);

	GateLogic::SendToGame(GateToGame_ClientDisconnect, msg);
}

void GateClient::onRecv(Link *link)
{
	m_taskQueue->put(boost::bind(&GateClient::handleMsg, this));
}

void GateClient::handleMsg()
{
	Link *link = m_link;

	// 1. 将接收缓冲区的数据全部取出
	evbuffer recvSwapBuf;
	evbuffer *dst = &recvSwapBuf;

	link->beginRead(dst);

	// 2. 循环处理消息数据
	while(true) {
		// 检测包头长度
		size_t bytes = evbuffer_get_length(dst);
		if (bytes < sizeof(NetMsgHead)) {
			break;
		}

		NetMsgHead *head	= (NetMsgHead *)evbuffer_pullup(dst, sizeof(NetMsgHead));

		uint32 rawMsgSize = endiantool::NetworkToHost32(head->msgSize);
		uint16 msgId = endiantool::NetworkToHost16(head->msgId);

		// 检测半包
		if (rawMsgSize > bytes) {
			// 			LOG_WARN << "gatesvr [" << link->m_localAddr.toIpPort() << "] <-> client [" << link->m_peerAddr.toIpPort()
			// 			          << "] msgLen(" << msgLen << ") > bytes(" << bytes << ")";
			break;
		}

		unsigned char *peek = evbuffer_pullup(dst, rawMsgSize);
		
		char *msg = nullptr;
		int msgSize = 0;

		// 不加密
		if (m_encryptKey.empty())
		{
			msg = (char*)peek + sizeof(NetMsgHead);
			msgSize = rawMsgSize - sizeof(NetMsgHead);
		}
		// 加密
		else
		{
			// 取到的是加密串
			uint8 *encryptBuf = (uint8*)(peek + sizeof(NetMsgHead));
			int encryptBufLen = rawMsgSize - sizeof(NetMsgHead);

			// 解密
			if (!encrypttool::xor_decrypt(encryptBuf, encryptBufLen, (uint8*)m_encryptKey.c_str(), m_encryptKey.size())) {
				LOG_ERROR << "gatesvr [" << link->getPeerAddr().toIpPort() << "] <-> " << name() << " [" << link->getLocalAddr().toIpPort()
					<< "] decrypt msg [len=" << encryptBufLen << "] failed";
				evbuffer_drain(dst, rawMsgSize);
				continue;
			}

			msg = (char*)peek + sizeof(NetMsgHead) + EncryptHeadLen;
			msgSize = rawMsgSize - sizeof(NetMsgHead) - EncryptHeadLen - EncryptTailLen;
		}

		// 转发给游戏服
		if (NeedRouteToGame(msgId)) {
			GateLogic::SendToGameByClient(m_clientId, msgId, msg, msgSize);
		} 
		// 直接处理
		else 
		{
			GateServer::Instance().m_clientDispatcher.dispatch(*this, msgId, msg, msgSize, 0);
		}

		evbuffer_drain(dst, rawMsgSize);
	};

	// 3. 处理完毕后，若有残余的消息体，则将残余消息体重新拷贝到接收缓冲区的头部以保持正确的数据顺序
	link->endRead(dst);
}

bool GateClient::NeedRouteToGame(int msgId)
{
	return msgId > ClientMsg_RouteToGate;
}

bool GateClient::SendMsg(int msgId, Message &msg)
{
	if (!m_link->isopen()) {
		return false;
	}

	// 不加密
	if (m_encryptKey.empty())
	{
		m_link->send(msgId, msg);
	}
	// 加密
	else
	{
		/*
		int size = msg.ByteSize();

		// 将消息包序列化
		bool ok = msg.SerializeToArray(m_link->m_net->g_encryptBuf + sizeof(NetMsgHead) + EncryptHeadLen, size);
		if (!ok) {
			LOG_ERROR << "client [" << m_link->getPeerAddr().toIpPort()
				<< "] send msg failed, SerializeToArray error, [len=" << size << "] failed, content = [" << msgtool::getMsgDebugString(msg) << "]";

			return false;
		}

		// 添加加密头、加密尾
		uint8* decryptBuf = (uint8*)(m_link->m_net->g_encryptBuf + sizeof(NetMsgHead));
		int decryptBufLen = size + EncryptHeadLen + EncryptTailLen;

		encrypttool::xor_encrypt(decryptBuf, decryptBufLen, (uint8*)m_encryptKey.c_str(), m_encryptKey.size());

		NetMsgHead* header = (NetMsgHead*)m_link->m_net->g_encryptBuf;
		int packetLen = msgtool::BuildNetHeader(header, msgId, decryptBufLen);

		m_link->send(m_link->m_net->g_encryptBuf, packetLen);
		*/
	}
	
	return true;
}

// 发送消息包
bool GateClient::Send(int msgId, const char* msg, int msgSize)
{
	if (!m_link->isopen()) {
		return false;
	}

	// 不加密
	if (m_encryptKey.empty())
	{
		m_link->send(msgId, msg, msgSize);
	}

	return true;
}

void GateClient::close()
{
	m_link->close();
}
