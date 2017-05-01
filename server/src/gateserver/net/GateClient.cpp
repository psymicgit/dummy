///<------------------------------------------------------------------------------
//< @file:   gateserver\net\client.cpp
//< @author: ������
//< @date:   2015��3��14�� 21:4:7
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#include "GateClient.h"


#include <client.pb.h>
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


GateClient::GateClient()
	: m_link(NULL)
	, m_clientId(0)
	, m_taskQueue(NULL)
	, m_clientMgr(NULL)
	, m_pingCount(0)
	, m_speedTestCount(0)
	, m_latencyTestCount(0)
{
}

void GateClient::onEstablish()
{
	// ���������֤��
	// m_encryptKey = randtool::rand_string(EncryptKeyLen);
	m_authKey = randtool::rand_string(AuthKeyLen);

	//���ͼӽ�����Կ
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
	if ((m_clientMgr->getClientCount() - 1) % 100 == 0) {
		LOG_INFO << name() << " [" << peerAddr.toIpPort() << "] <-> gatesvr [" << localAddr.toIpPort() << "] broken! current client cnt = " << m_clientMgr->getClientCount() - 1;
	}

	m_clientMgr->delClient(this);
}

void GateClient::onRecv(Link *link)
{
	m_taskQueue->put(boost::bind(&GateClient::handleMsg, this));
}

void GateClient::handleMsg()
{
	Link *link = m_link;

	// 1. �����ջ�����������ȫ��ȡ��
	evbuffer recvSwapBuf;
	evbuffer *dst = &recvSwapBuf;

	link->beginRead(dst);

	// 2. ѭ��������Ϣ����
	while(true) {
		// ����ͷ����
		size_t bytes = evbuffer_get_length(dst);
		if (bytes < sizeof(NetMsgHead)) {
			break;
		}

		NetMsgHead *head	= (NetMsgHead *)evbuffer_pullup(dst, sizeof(NetMsgHead));

		uint16 msgId = endiantool::networkToHost(head->msgId);
		uint32 rawMsgSize = endiantool::networkToHost(head->msgLen);

		// �����
		if (rawMsgSize > bytes) {
			// 			LOG_WARN << "gatesvr [" << link->m_localAddr.toIpPort() << "] <-> client [" << link->m_peerAddr.toIpPort()
			// 			          << "] msgLen(" << msgLen << ") > bytes(" << bytes << ")";
			break;
		}

		unsigned char *peek = evbuffer_pullup(dst, rawMsgSize);
		
		char *msg = nullptr;
		int msgSize = 0;

		// ������
		if (m_encryptKey.empty())
		{
			msg = (char*)peek + sizeof(NetMsgHead);
			msgSize = rawMsgSize - sizeof(NetMsgHead);
		}
		// ����
		else
		{
			// ȡ�����Ǽ��ܴ�
			uint8 *encryptBuf = (uint8*)(peek + sizeof(NetMsgHead));
			int encryptBufLen = rawMsgSize - sizeof(NetMsgHead);

			// ����
			if (!encrypttool::xor_decrypt(encryptBuf, encryptBufLen, (uint8*)m_encryptKey.c_str(), m_encryptKey.size())) {
				LOG_ERROR << "gatesvr [" << link->getPeerAddr().toIpPort() << "] <-> " << name() << " [" << link->getLocalAddr().toIpPort()
					<< "] decrypt msg [len=" << encryptBufLen << "] failed";
				evbuffer_drain(dst, rawMsgSize);
				continue;
			}

			msg = (char*)peek + sizeof(NetMsgHead) + EncryptHeadLen;
			msgSize = rawMsgSize - sizeof(NetMsgHead) - EncryptHeadLen - EncryptTailLen;
		}

		// �ж��Ƿ�ת��
		if (NeedRouteToGame(msgId)) {
			// ת������Ϸ��
			GateServer::Instance().sendToGameServer(m_clientId, msgId, msg, msgSize);
		} else {
			// ֱ�Ӵ���
			m_clientMgr->m_dispatcher.dispatch(*this, msgId, msg, msgSize, 0);
		}

		evbuffer_drain(dst, rawMsgSize);
	};

	// 3. ������Ϻ����в������Ϣ�壬�򽫲�����Ϣ�����¿��������ջ�������ͷ���Ա�����ȷ������˳��
	link->endRead(dst);
}

bool GateClient::NeedRouteToGame(int msgId)
{
	return msgId > ClientMsg_RouteToGate;
}

bool GateClient::send(int msgId, Message &msg)
{
	if (!m_link->isopen()) {
		return false;
	}

	// ������
	if (m_encryptKey.empty())
	{
		m_link->send(msgId, msg);
	}
	// ����
	else
	{
		int size = msg.ByteSize();

		// ����Ϣ�����л�
		bool ok = msg.SerializeToArray(m_link->m_net->g_encryptBuf + sizeof(NetMsgHead) + EncryptHeadLen, size);
		if (!ok) {
			LOG_ERROR << "client [" << m_link->getPeerAddr().toIpPort()
				<< "] send msg failed, SerializeToArray error, [len=" << size << "] failed, content = [" << msgtool::getMsgDebugString(msg) << "]";

			return false;
		}

		// ��Ӽ���ͷ������β
		uint8* decryptBuf = (uint8*)(m_link->m_net->g_encryptBuf + sizeof(NetMsgHead));
		int decryptBufLen = size + EncryptHeadLen + EncryptTailLen;

		encrypttool::xor_encrypt(decryptBuf, decryptBufLen, (uint8*)m_encryptKey.c_str(), m_encryptKey.size());

		NetMsgHead* header = (NetMsgHead*)m_link->m_net->g_encryptBuf;
		int packetLen = msgtool::BuildNetHeader(header, msgId, decryptBufLen);

		m_link->send(m_link->m_net->g_encryptBuf, packetLen);
	}
	
	return true;
}

void GateClient::close()
{
	m_link->close();
}
