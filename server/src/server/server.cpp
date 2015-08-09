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
	: m_svrType(eNullServer)
	, m_zoneId(0)
	, m_isquit(false)
{
	assert(NULL == instance);
	instance = this;
}

bool Server::init()
{
	global::init();

	m_bufferPool.init(5000, 5000);

	m_dispatcher.addMsgHandler(new NetMsgHandler(&m_dispatcher));
	return true;
}

bool Server::uninit()
{
	m_dispatcher.clear();
	global::uninit();
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

void Server::onRecvBlock(Link *link, RingBufferBlock *block)
{
	int bytes = block->getTotalLength();

	Buffer buf(sizeof(NetMsgHead));

	while(bytes > 0) {
		if (bytes < sizeof(NetMsgHead)) {
			break;
		}

		NetMsgHead *msgHead = NULL;
		buf.clear();

		// 有效数据部分
		if (block->size() < sizeof(NetMsgHead)) {
			block->take(buf, sizeof(NetMsgHead));
			msgHead = (NetMsgHead*)buf.peek();
		}
		else {
			msgHead = (NetMsgHead*)block->begin();
		}

		msgHead->msgId = endiantool::networkToHost16(msgHead->msgId);
		msgHead->msgLen = endiantool::networkToHost32(msgHead->msgLen);

		buf.clear();

		// 检测半包
		if ((int)msgHead->msgLen > bytes) {
			break;
		}

		block = block->skip(sizeof(NetMsgHead));

		//先解密
		char* encryptBuf =  NULL;
		int encryptBufLen = msgHead->msgLen - sizeof(NetMsgHead);

		// 检测当前block是否已包含整个消息包
		if (block->size() >= encryptBufLen) {
			encryptBuf = block->begin();
		}
		else {
			block->take(buf, encryptBufLen);
			encryptBuf = (char*)buf.peek();
		}

		block = block->skip(encryptBufLen);
		link->m_head = block;

		if (link->m_head == NULL) {
			link->m_tail = NULL;
		}

		bytes -= msgHead->msgLen;

		Buffer *msg = m_bufferPool.alloc(encryptBuf, msgHead->msgLen - sizeof(NetMsgHead));
		m_taskQueue.put(boost::bind(&Server::handleMsg, this, link, msgHead->msgId, msg));
	}
}

void Server::onRecv(Link *link, Buffer& buf, RingBufferBlock &block)
{
// 	onRecvBlock(link, &block);
// 	while(true) {
// 		return;
// 	}

	while(true) {
		// 检测半包
		size_t bytes = buf.readableBytes();
		if (bytes < sizeof(NetMsgHead)) {
			return;
		}

		NetMsgHead *msgHead = (NetMsgHead*)buf.peek();
		msgHead->msgId = endiantool::networkToHost16(msgHead->msgId);
		msgHead->msgLen = endiantool::networkToHost32(msgHead->msgLen);

		if (msgHead->msgLen > bytes) {
			return;
		}

		Buffer *msg = m_bufferPool.alloc(msgHead->msgLen - sizeof(NetMsgHead));
		msg->append(buf.peek() + sizeof(NetMsgHead), msgHead->msgLen - sizeof(NetMsgHead));

		buf.skip(msgHead->msgLen);

		m_taskQueue.put(boost::bind(&Server::handleMsg, this, link, msgHead->msgId, msg));
	}
}

void Server::handleMsg(Link* link, int msgId, Buffer *buf)
{
	// LOG_INFO << "<Server> recv msg from <" << link->m_peerAddr.toIpPort() << "> :" << buffer->retrieveAllAsString();
	m_dispatcher.dispatch(*link, msgId, buf->peek(), buf->readableBytes(), 0);
	m_bufferPool.free(buf);
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

void Server::delSvrLink(int svrId)
{
	m_svrLinkMap.erase(svrId);
}

void Server::start()
{
	m_lan.start();

	while(!m_isquit) {
		run();
	}

	uninit();
	LOG_WARN << "stop <" << getServerName() << "> successfully!";
}

std::string& Server::getServerName()
{
	return svrtool::getSvrName(m_svrType);
}

void Server::stop()
{
	LOG_WARN << "start closing " << getServerName() << " ...";
	LOG_WARN << "	<m_taskQueue.size() = " << m_taskQueue.size() << ">";

	m_taskQueue.put(boost::bind(&Server::stopping, this));
}

void Server::stopping()
{
	m_isquit = true;

	run();
	m_lan.stop();
}

void Server::run()
{
	m_taskQueue.run();
}