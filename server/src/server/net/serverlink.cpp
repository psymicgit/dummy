///<------------------------------------------------------------------------------
//< @file:   server\net\serverlink.cpp
//< @author: 洪坤安
//< @date:   2015年3月9日 11:44:44
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "serverlink.h"
#include "server.h"

void ServerLink::OnDisconnect(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	int svrId = Server::instance->getServerId(m_remoteSvrType, m_svrId);
	Server::instance->eraseSvrLink(svrId);

	Server::instance->OnDisconnect(link, localAddr, peerAddr);
	Server::instance->OnDisconnectServer(*link, m_remoteSvrType, m_svrId);

	if (link->m_isCreateByConnector) {
		Server::instance->m_lan.connect(peerAddr.toIp(), peerAddr.toPort(), *Server::instance);
	}
}

void ServerLink::OnRecv(Link *link, Buffer &buf)
{
	Server::instance->OnRecv(link, buf);
}
