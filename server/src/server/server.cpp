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

#include <signal.h>

#include <net.pb.h>

Server* Server::instance = NULL;

void registerSignal();

void handleSignal(int sig)
{
	SAFE_LOG("%s pid = %u, handle signal = %u", Server::instance->name().c_str(), getpid(), sig);
	if (sig == SIGTERM) {
		Server::instance->stop();
	} else if(sig == SIGINT) {
		Server::instance->stop();
	}
}

// 监听系统信号：防止一般的kill操作导致服务器不正常关闭
void registerSignal()
{
#ifdef WIN
	signal(SIGTERM, &handleSignal);
	signal(SIGINT, &handleSignal);
#else
	struct sigaction act;
	act.sa_handler = handleSignal;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGINT, &act, 0);
	sigaction(SIGTERM, &act, 0);
#endif
}

Server::Server()
	: m_svrType(eNullServer)
	, m_zoneId(0)
	, m_isquit(false)
{
	assert(NULL == instance);
	instance = this;
}

std::string Server::name()
{
	return svrtool::getSvrName(m_svrType);
}

bool Server::init()
{
	global::init();

	// 注册系统信号：防止一般的kill操作导致服务器不正常关闭
	registerSignal();

	//m_bufferPool.init(1000, 500);

	m_dispatcher.addMsgHandler(new NetMsgHandler(&m_dispatcher));
	return true;
}

bool Server::uninit()
{
	m_dispatcher.clear();
	//m_bufferPool.clear();

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

void Server::onRecv(Link *link, Buffer& buf)
{
	{
		lock_guard_t<> lock(link->m_sendBufLock);
		if (link->m_isWaitingRead) {
			return;
		}

		link->m_isWaitingRead = true;
	}

	m_taskQueue.put(boost::bind(&Server::handleMsg, this, link));
}

void Server::handleMsg(Link *link)
{
	Buffer buf;

	// 1. 将接收缓冲区的数据全部取出
	{
		lock_guard_t<> lock(link->m_recvBufLock);
		link->m_isWaitingRead = false;
		buf.swap(link->m_recvBuf);
	}

	// 2. 循环处理消息数据
	while(true) {
		// 检测包头长度
		size_t bytes = buf.readableBytes();
		if (bytes < sizeof(NetMsgHead)) {
			break;
		}

		// 检测半包
		NetMsgHead *msgHead = (NetMsgHead*)buf.peek();
		msgHead->msgId = endiantool::networkToHost16(msgHead->msgId);
		msgHead->msgLen = endiantool::networkToHost32(msgHead->msgLen);

		if (msgHead->msgLen > bytes) {
			break;
		}

		m_dispatcher.dispatch(*link, msgHead->msgId, buf.peek() + sizeof(NetMsgHead), msgHead->msgLen - sizeof(NetMsgHead), 0);
		buf.skip(msgHead->msgLen);
	}

	// 3. 处理完毕后，若有残余的消息体，则将残余消息体重新拷贝到接收缓冲区的头部以保持正确的数据顺序
	if (!buf.empty()) {
		{
			lock_guard_t<> lock(link->m_recvBufLock);
			if (!link->m_recvBuf.empty()) {
				buf.append(link->m_recvBuf.peek(), link->m_recvBuf.readableBytes());
				link->m_recvBuf.swap(buf);
			} else {
				link->m_recvBuf.swap(buf);
			}
		}
	}

	// LOG_INFO << "<Server> recv msg from <" << link->m_peerAddr.toIpPort() << "> :" << buffer->retrieveAllAsString();
	// m_dispatcher.dispatch(*link, msgId, buf->peek(), buf->readableBytes(), 0);
	// m_bufferPool.free(buf);
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

void Server::registerServer(int svrId, ServerLink *svrLink)
{
	m_svrLinkMap[svrId] = svrLink;
}

void Server::unregisterServer(int svrId)
{
	m_svrLinkMap.erase(svrId);
}

void Server::start()
{
	// 启动内网通信
	m_lan.start();

	while(!m_isquit) {
		run();
	}

	uninit();
	LOG_INFO << "stop <" << name() << "> successfully!";
}

void Server::stop()
{
	if (m_isquit) {
		return;
	}

	LOG_WARN << "start closing " << name() << " ...";
	LOG_INFO << "	<m_taskQueue.size() = " << m_taskQueue.size() << ">";

	m_isquit = true;

	m_taskQueue.put(boost::bind(&Server::stopping, this));
	run();
}

void Server::stopping()
{
	// 停止内网通信
	m_lan.stop();

	// 将关闭网络时产生的网络任务执行完
	run();
}

void Server::run()
{
	m_taskQueue.run();
}