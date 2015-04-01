///<------------------------------------------------------------------------------
//< @file:   src\server\server.cpp
//< @author: 洪坤安
//< @date:   2014年11月20日 10:44:0
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#include "server.h"
#include "net/netaddress.h"
#include "net/link.h"
#include "protocol/message.h"
#include "protocol/netmsghandler.h"
#include "tool/randtool.h"

#include <net.pb.h>

Server* Server::instance = NULL;

Server::Server()
	: m_isquit(false)
	, m_svrType(eNullServer)
{
	assert(NULL == instance);
	instance = this;
}

bool Server::init()
{
	randtool::initSeed();

	NetMsgHandler *netMsgMgr = new NetMsgHandler(&m_dispatcher);
	return true;
}

bool Server::uninit()
{
	//google::protobuf::ShutdownProtobufLibrary();
	return false;
}

void Server::onConnected(Link *link, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	ConnectReq req;
	req.set_svrtype(m_svrType);
	req.set_authkey(g_serverHandshakeAuthKey);
	req.set_svrid(m_zoneId);

	link->send(eConnectSvrReq, req);
	LOG_INFO << "<" << localAddr.toIpPort() << "> connect to <" << peerAddr.toIpPort() << "> success";
}

void Server::onAccepted(Link*, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	LOG_INFO << "<" << localAddr.toIpPort() << "> receive new connection from <" << peerAddr.toIpPort() << ">";
}

void Server::onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr)
{
	LOG_INFO << "<" << localAddr.toIpPort() << "> peer connection <" << peerAddr.toIpPort() << "> broken";
}

void Server::onRecv(Link *link, Buffer& buf)
{
	// 检测半包
	int bytes = buf.readableBytes();
	if (bytes < sizeof(NetMsgHead)) {
		return;
	}

	NetMsgHead *msgHead = (NetMsgHead*)buf.peek();
	msgHead->msgId = endiantool::networkToHost16(msgHead->msgId);
	msgHead->msgLen = endiantool::networkToHost32(msgHead->msgLen);

	if (msgHead->msgLen > bytes) {
		return;
	}

	Buffer msg;
	msg.append(buf.peek() + sizeof(NetMsgHead), msgHead->msgLen - sizeof(NetMsgHead));

	buf.retrieve(msgHead->msgLen);

	m_taskQueue.put(task_binder_t::gen(&Server::handleMsg, this, link, msgHead->msgId, msg));
}

void Server::handleMsg(Link* link, int msgId, Buffer &buf)
{
	// LOG_INFO << "<Server> recv msg from <" << link->m_peerAddr.toIpPort() << "> :" << buffer->retrieveAllAsString();

	m_dispatcher.dispatch(*link, msgId, buf.peek(), buf.readableBytes(), 0);
}

int Server::getServerId(ServerType svrType, int zoneId)
{
	int intSvrType = svrType;
	int svrId = (int)(intSvrType << 16) + zoneId;
	return svrId;
}

bool Server::isSvrLinkExist(ServerType svrType, int zoneId)
{
	int svrId = getServerId(svrType, zoneId);
	return m_svrLinkMap.find(svrId) != m_svrLinkMap.end();
}

void Server::addSvrLink(int svrId, ServerLink *svrLink)
{
	m_svrLinkMap[svrId] = svrLink;
}

void Server::eraseSvrLink(int svrId)
{
	m_svrLinkMap.erase(svrId);
}

void Server::start()
{
	m_lan.start();

	while(!m_isquit) {
		run();
	}
}

void Server::run()
{
	m_taskQueue.run();
}