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
#include "net/netmodel.h"

std::string ServerLink::name()
{
	std::string ret = "[" + svrtool::getSvrName(m_peerSvrType) + "<->" + svrtool::getSvrName(m_peerSvrType) + "Link]";
	return ret;
}

bool ServerLink::Init()
{
	return true;
}

void ServerLink::onDisconnect(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	ServerType peerSvrType = m_peerSvrType;

	int svrId = Server::instance->getServerId(m_peerSvrType, m_peerSvrId);
	Server::instance->unregisterServer(svrId);

	Server::instance->onDisconnect(link, localAddr, peerAddr);
	Server::instance->onDisconnectServer(*link, m_peerSvrType, m_peerSvrId);

	// 从这里开始this已经被删除了
	if (link->m_isAutoReconnect) {
		Server::instance->m_lan.connect(peerAddr.toIp(), peerAddr.toPort(), *Server::instance, svrtool::getSvrName(peerSvrType).c_str());
	}
}

void ServerLink::onRecv(Link *link)
{
	Server::instance->onRecv(link);
}

void ServerLink::SendMsg(int msgId, const Message& msg)
{
	m_link->send(msgId, msg);
}
