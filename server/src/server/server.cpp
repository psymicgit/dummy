///<------------------------------------------------------------------------------
//< @file:   src\server\server.cpp
//< @author: ������
//< @date:   2014��11��20�� 10:44:0
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
#include <basic/evbuffer.h>

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

// ����ϵͳ�źţ���ֹһ���kill�������·������������ر�
void registerSignal()
{
#ifdef WIN
	signal(SIGTERM, &handleSignal);
	signal(SIGINT,	&handleSignal);
#else
	struct sigaction act;
	act.sa_handler = handleSignal;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigaction(SIGINT,	&act, 0);
	sigaction(SIGTERM,	&act, 0);
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

	// ע��ϵͳ�źţ���ֹһ���kill�������·������������ر�
	// registerSignal();
	//m_bufferPool.init(1000, 500);

	m_dispatcher.registerMsg(eConnectSvrReq, NetMsgHandler::OnConnectServerReq);	// ���ӷ���������
	m_dispatcher.registerMsg(eConnectSvrAck, NetMsgHandler::OnConnectServerAck);	// ���ӷ�������

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

void Server::onRecv(Link *link)
{
	m_taskQueue.put(boost::bind(&Server::HandleNetMsg, this, link));
}

void Server::HandleNetMsg(Link *link)
{
	// 1. �����ջ�����������ȫ��ȡ��
	evbuffer recvSwapBuf;
	evbuffer *buf = &recvSwapBuf;

	link->beginRead(buf);

	// 2. ѭ��������Ϣ����
	while(true) {
		// ����ͷ����
		size_t bytes = evbuffer_get_length(buf);
		if (bytes < sizeof(NetMsgHead)) {
			break;
		}

		NetMsgHead *head = (NetMsgHead *)evbuffer_pullup(buf, sizeof(NetMsgHead));

		uint16 msgId	= endiantool::networkToHost(head->msgId);
		uint32 msgLen	= endiantool::networkToHost(head->msgLen);

		// �����
		if (msgLen > bytes) {
			break;
		}

		const char *peek = (const char*)evbuffer_pullup(buf, msgLen);

		m_dispatcher.dispatch(*link, msgId, peek + sizeof(NetMsgHead), msgLen - sizeof(NetMsgHead), 0);
		evbuffer_drain(buf, msgLen);
	}

	// 3. ������Ϻ����в������Ϣ�壬�򽫲�����Ϣ�����¿��������ջ�������ͷ���Ա�����ȷ������˳��
	link->endRead(buf);

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

void Server::HandleServerMsg(Link* link)
{
}

void Server::start()
{
	// ��������ͨ��
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

	m_taskQueue.put(boost::bind(&Server::stopping, this));
}

void Server::stopping()
{
	m_isquit = true;

	// ֹͣ����ͨ��
	m_lan.stop();

	// ���ر�����ʱ��������������ִ����
	run();
}

void Server::run()
{
	m_taskQueue.run();
}