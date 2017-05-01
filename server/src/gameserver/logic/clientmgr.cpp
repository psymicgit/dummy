///<------------------------------------------------------------------------------
//< @file:   gameserver\logic\clientmgr.cpp
//< @author: ������
//< @date:   2015��6��10�� 22:36:32
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#include "clientmgr.h"
#include "net/clientmsghandler.h"

ClientMgr::ClientMgr()
{
	m_dispatcher.addMsgHandler(new ClientMsgHandler(&m_dispatcher));
}

Client* ClientMgr::FindClient(uint32 playerId)
{
	ClientMap::iterator itr = m_clients.find(playerId);
	if (itr == m_clients.end()) {
		return NULL;
	}

	return itr->second;
}

void ClientMgr::handleMsg(Link* link)
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

		Client *client = ClientMgr::Instance().FindClient(clientId);
		if (NULL == client) {
			return;
		}

		int msgSize = rawMsgSize - sizeof(LanMsgHead);

		const char *peek = (const char*)evbuffer_pullup(buf, msgSize);
		const char* msg = peek + sizeof(LanMsgHead);

		m_dispatcher.dispatch(*client, msgId, msg, msgSize, 0);
		evbuffer_drain(buf, rawMsgSize);
	}

	// 3. ������Ϻ����в������Ϣ�壬�򽫲�����Ϣ�����¿��������ջ�������ͷ���Ա�����ȷ������˳��
	link->endRead(buf);
}
