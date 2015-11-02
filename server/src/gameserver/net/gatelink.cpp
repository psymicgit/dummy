///<------------------------------------------------------------------------------
//< @file:   gameserver\net\gatelink.cpp
//< @author: 洪坤安
//< @date:   2015年3月8日 21:15:50
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "gatelink.h"

#include <net/netaddress.h>
#include <net/link.h>

#include <server.h>
#include <protocol/message.h>

#include "logic/clientmgr.h"

void GateLink::OnRecv(Link *link, Buffer &buf)
{
	while(true) {
		// 检测包头长度
		size_t bytes = buf.readableBytes();
		if (bytes < sizeof(LanMsgHead)) {
			return;
		}

		LanMsgHead *msgHead	= (LanMsgHead*)buf.peek();
		uint32 clientId		= endiantool::networkToHost(msgHead->clientId);
		uint16 msgId			= endiantool::networkToHost(msgHead->msgId);
		uint32 dataLen			= endiantool::networkToHost(msgHead->msgLen);

		// 检测半包
		if (dataLen > bytes) {
			// 			LOG_WARN << "gatesvr [" << link->m_localAddr.toIpPort() << "] <-> client [" << link->m_peerAddr.toIpPort()
			// 			          << "] msgLen(" << msgLen << ") > bytes(" << bytes << ")";
			return;
		}

		char *msg		= (char*)buf.peek() + sizeof(LanMsgHead);
		uint32 msgLen	= dataLen - sizeof(LanMsgHead);

		Client *client = ClientMgr::Instance().FindClient(clientId);
		if (NULL == client) {
			return;
		}

		// 直接本地进行处理
		Buffer deepCopyBuf;
		deepCopyBuf.append(msg, msgLen);
		Server::instance->getTaskQueue().put(boost::bind(&ClientMgr::handleMsg, &ClientMgr::Instance(), *client, msgId, deepCopyBuf, 0));

		buf.skip(dataLen);
	};
}
