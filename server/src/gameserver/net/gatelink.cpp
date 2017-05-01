///<------------------------------------------------------------------------------
//< @file:   gameserver\net\gatelink.cpp
//< @author: ������
//< @date:   2015��3��8�� 21:15:50
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#include "gatelink.h"
#include "gameserver.h"
#include "logic/GameClientMgr.h"

#include <net/netaddress.h>
#include <net/link.h>
#include <server.h>
#include <protocol/message.h>

void GateLink::onRecv(Link *link)
{
	Server::instance->getTaskQueue().put(boost::bind(&GameClientMgr::handleMsg, &GameClientMgr::Instance(), link));
}

void GateLink::handleMsg(Link* link)
{
	// 1. �����ջ�����������ȫ��ȡ��
	evbuffer recvSwapBuf;
	evbuffer *buf = &recvSwapBuf;

	link->beginRead(buf);

	// 2. ѭ��������Ϣ����
	while (true) {
		// ����ͷ����
		int bytes = (int)evbuffer_get_length(buf);
		if (bytes <= sizeof(NetMsgHead)) {
			break;
		}

		LanMsgHead *head = (LanMsgHead*)evbuffer_pullup(buf, sizeof(LanMsgHead));
		int clientId = endiantool::networkToHost(head->clientId);
		int msgId = endiantool::networkToHost(head->msgId);
		int rawMsgSize = endiantool::networkToHost(head->msgLen);

		// �����
		if (rawMsgSize > bytes) {
			break;
		}

		GameClient *client = GameClientMgr::Instance().FindClient(clientId);
		if (NULL == client) {
			return;
		}

		int msgSize = rawMsgSize - sizeof(LanMsgHead);

		const char *peek = (const char*)evbuffer_pullup(buf, msgSize);
		const char* msg = peek + sizeof(LanMsgHead);

		GameServer::Instance().m_dispatcher.dispatch(*client, msgId, msg, msgSize, 0);
		evbuffer_drain(buf, rawMsgSize);
	}

	// 3. ������Ϻ����в������Ϣ�壬�򽫲�����Ϣ�����¿��������ջ�������ͷ���Ա�����ȷ������˳��
	link->endRead(buf);
}
