///<------------------------------------------------------------------------------
//< @file:   server\net\serverlink.cpp
//< @author: 洪坤安
//< @date:   2015年3月9日 11:44:44
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "serverlink.h"
#include "server.h"

#include "protocol/message.h"
#include "tool/servertool.h"

void ServerLink::onDisconnect(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	int svrId = Server::instance->getServerId(m_remoteSvrType, m_svrId);
	Server::instance->delSvrLink(svrId);

	Server::instance->onDisconnect(link, localAddr, peerAddr);
	Server::instance->onDisconnectServer(*link, m_remoteSvrType, m_svrId);

	if (link->m_isAutoReconnect) {
		Server::instance->m_lan.connect(peerAddr.toIp(), peerAddr.toPort(), *Server::instance, svrtool::getSvrName(m_remoteSvrType).c_str());
	}
}

void ServerLink::onRecv(Link *link, Buffer &buf)
{
	Server::instance->onRecv(link, buf);
}

void ServerLink::send(uint32 routeId, uint16 msgId, const char *data, int len)
{
	char *netBuf = global::g_netBuf;
	LanMsgHead *msgHead = (LanMsgHead*)netBuf;

	memcpy(netBuf + sizeof(LanMsgHead), data, len);
	msgtool::buildLanMsgHeader(msgHead, routeId, msgId, len);

	m_link->send(netBuf, msgHead->msgLen);
}
