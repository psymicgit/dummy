///<------------------------------------------------------------------------------
//< @file:   server\net\listener.cpp
//< @author: ������
//< @date:   2014��11��25�� 19:14:38
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#include "listener.h"

#include "tool/sockettool.h"
#include "net/netaddress.h"
#include "net/link.h"
#include "net/net.h"
#include "net/netmodel.h"

Listener::Listener(NetModel *netModel, INetLogic *logic, Net *net)
	: m_netModel(netModel)
	, m_logic(logic)
	, m_listenFd(0)
	, m_net(net)
{

}

bool Listener::open(const string & ip, int port)
{
	LOG_DEBUG << m_logic->name() << " start listening at <" << ip << ": " << port << ">";

	m_listenAddr	= NetAddress(ip, port);
	m_listenFd		= socktool::createSocket();

	if (m_listenFd <= 0) {
		return false;
	}

	/*

	// ���ö˿ڸ���
	if(!socktool::setReuseAddr(m_listenFd, true)) {
		return false;
	}

	*/

	// ����Ϊ������
	if(!socktool::setNonBlocking(m_listenFd)) {
		return false;
	}

	// �󶨵�ַ
	if(!socktool::bindAddress(m_listenFd, m_listenAddr)) {
		return false;
	}

	// ��ʼ����
	if(!socktool::listen(m_listenFd)) {
		return false;
	}

	// ����������ע�ᵽ����
	m_netModel->addFd(this);

	// ע��ɶ��¼������ڷ�����listen�������յ�������ʱ����������ɿɶ�
	m_netModel->enableRead(this);
	return true;
}

void Listener::close()
{
	LOG_DEBUG << m_logic->name() << " stop listening at <" << m_listenAddr.toIpPort() << ">";

	socktool::closeSocket(m_listenFd);
	m_netModel->delFd(this);
}

void Listener::erase()
{
	delete this;
}

void Listener::handleRead()
{
	sockaddr addr;
	socklen_t addrlen = sizeof(addr);

	// ѭ������������ֱ�����Ӷ���Ϊ��
	int newfd = -1;
	do {
		// ע�⣺��������Ҫ֪������socket���ӵĹ��̣�
		// 	1.client��syn�����server
		// 	2.server�յ������������SYN queue���������Ӷ��е����ֵ��ϵͳ����tcp_max_syn_backlog�����
		// 	3.����ڰ����Ӷ��к���syn+ack��client��client�յ����ٷ�syn+ack��server����������֣�Ȼ��server�����Ӵ����accept queue��������г��Ⱦ��ǳ��������socket��ʱ�����backlog�����С��
		// 	4.Ӧ�ó���ͨ������accept()��accept queue���ȡ���ӡ�

		// ������accept����Ҫע�⣬��Ҫ�޸��ں˲��������򵱴����ͻ���ͬʱ����ʱ���ᵼ�·�������accept���������ʹ�ÿͻ�������Ϊ�ɹ��������ӣ��������ȴaccept��������
		// ��Ҫ��/etc/sysctl.conf����ں˲�������:
		// net.core.somaxconn = 51200��accept queue����somaxconn������listen(fd, backlog)�е�backlog����Ҳ��somaxconn������
		// net.ipv4.tcp_max_syn_backlog = 512��SYN queue������tcp_max_syn_backlogָ����
		// net.ipv4.tcp_abort_on_overflow = 1�����accept������������ᷢ���ܾ��ͻ��˵���ֹѶϢ���Ͽ����ӣ��������������е����ӣ�linux�Ի���������������ӣ���ȴ�޷�acceptȡ�ã�
		// Ȼ��ִ��sysctl -p

		// ��β鿴accept queue���
		// netstat -s | grep LISTEN �Ա�ǰ���Ƿ�������
		if ((newfd = ::accept(m_listenFd, (struct sockaddr *)&addr, &addrlen)) == -1) {
			int err = socktool::geterrno();
			// LOG_ERROR<< "err = " << err;
			if (err == EAGAIN || err == EWOULDBLOCK) {
				// ��non-blockingģʽ�£����accept����ֵΪ-1����errno == EAGAIN��errno == EWOULDBLOCK��ʾû������������
				break;
			} else if (err == ECONNABORTED && err == EPROTO && err == EINTR) {
				LOG_ERROR << "errno == " << err;
				continue;
			} else {
				LOG_SOCKET_ERR(m_listenFd, err) << m_logic->name() << " accept failed, restart listenning now";
				//! if too many open files occur, need to restart epoll event
				m_netModel->reopen(this);
				break;
			}
		}

		const struct sockaddr_in *addr_in = (sockaddr_in*)&addr;
		NetAddress peerAddr(*addr_in);

		// �����µ����Ӷ���
		Link* link = allocLink(newfd, peerAddr);
		if (NULL == link) {
			socktool::closeSocket(newfd);

			LOG_ERROR << m_logic->name() << " listener create link failed, new socket = " << newfd;
			break;
		}

		// ִ�г�ʼ����ע�ᵽ����
		link->open();

		TaskQueue::TaskList tasks;

		// �����յ������ӵ���ϢͶ��ҵ���
		tasks.push_back(boost::bind(&INetLogic::onAccepted, m_logic, link, m_listenAddr, peerAddr));

		// ��ҵ��㴦���������Ӻ󣬲���������ӿ�ʼ��
		tasks.push_back(boost::bind(&NetModel::enableRead, link->m_net, link));

		m_logic->getTaskQueue().put(tasks);
	} while (true);
}

void Listener::handleWrite()
{
	LOG_ERROR << m_logic->name();
}

void Listener::handleError()
{
	LOG_ERROR << m_logic->name();
}

Link* Listener::allocLink(socket_t newfd, NetAddress &peerAddr)
{
	NetModel *netModel = m_net->nextNetModel();

	LinkPool &linkPool = netModel->getLinkPool();
	return linkPool.alloc(newfd, m_listenAddr, peerAddr, netModel, m_logic);
}
