///<------------------------------------------------------------------------------
//< @file:   server\net\link.cpp
//< @author: ������
//< @date:   2015��1��14�� 23:7:5
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#include "link.h"

#include <google/protobuf/message.h>

#include "netmodel.h"
#include "netaddress.h"
#include "listener.h"

#include "protocol/message.h"

#include "tool/sockettool.h"
#include "tool/atomictool.h"

#include "basic/evbuffer.h"

void Link::open()
{
	socktool::setNonBlocking(m_sockfd);
	socktool::setKeepAlive(m_sockfd, true, 120);

	// socktool::setSendBufSize(m_sockfd, 256 * 1024);
	// socktool::setRecvBufSize(m_sockfd, 256 * 1024);

	if (!socktool::setTcpNoDelay(m_sockfd)) {
		LOG_ERROR << m_logic->name() << " " << getLocalAddr().toIpPort() << "<-->" << getPeerAddr().toIpPort() << " setTcpNoDelay failed!";
	}

	m_net->addFd(this);
}

Link::~Link()
{
}

void Link::close()
{
	if (m_closed) {
		return;
	}

	// Ͷ�ݵ������߳�ִ��close����
	m_net->getTaskQueue()->put(boost::bind(&Link::closing, this));
}

void Link::closing()
{
	// ����Ƿ��ظ�close
	if (m_closed) {
		LOG_ERROR << m_logic->name() << " double closed, canceled, m_sendBuf left size = " << evbuffer_get_length(&m_sendBuf);
		return;
	}

	// ���δ�����������Ƚ�δ���͵����ݷ������
	if (!m_error && !m_isPeerClosed) {
		lock_guard_t<> lock(m_sendBufLock);

		// ������δ������ϣ����ݻ��رգ�ֹֻͣ�������ݣ��ȴ�֮ǰ�ķ��Ͳ���ִ����Ϻ��ٹر�
		if (evbuffer_get_length(&m_sendBuf) >  0) {
			if (m_isWaitingClose) {
				// LOG_ERROR << m_logic->name() << " is waiting close";
				return;
			}

			m_isWaitingClose = true;
			// shutdown(m_sockfd, SHUT_RD);

			// ��ֹ��ȡ����
			m_net->disableRead(this);

			if (!m_isWaitingWrite) {
				LOG_ERROR << m_logic->name() << " m_sendBuf != empty(), left size = " << evbuffer_get_length(&m_sendBuf) << ", socket = " << m_sockfd;
			}

			// LOG_ERROR << m_logic->name() << " is waiting write";
			return;
		}
	}

	// LOG_INFO << m_logic->name() << " Link::closing";

	{
		lock_guard_t<> lock(m_sendBufLock);
		if (evbuffer_get_length(&m_sendBuf) > 0) {
			LOG_ERROR << m_logic->name() << " close, left size = " << evbuffer_get_length(&m_sendBuf) << ", socket = " << m_sockfd;
		}
	}

	m_closed = true;

	// shutdown(m_sockfd, SHUT_RDWR);

	// �������α������ϵ������������
	socktool::closeSocket(m_sockfd);

#ifdef WIN
	// ����windowsƽ̨������Ҫ�ٴӶ�д����ȡ��ע��
	m_net->disableAll(this);
#endif

	// ��ҵ��㴦��ùرղ���
	m_logic->getTaskQueue().put(boost::bind(&Link::onLogicClose, this));
}

void Link::erase()
{
	LinkPool &linkPool = m_net->getLinkPool();
	linkPool.free(this);
}

void Link::onLogicClose()
{
	m_logic->onDisconnect(this, m_localAddr, m_peerAddr);
	m_net->delFd(this);
}

void Link::onSend()
{
	if (!isopen()) {
		return;
	}

	if (!m_isWaitingWrite) {
		return;
	}

	// LOG_INFO << "Link::onSend, socket = " << m_sockfd;
// 	{
// 		lock_guard_t<> lock(m_sendBufLock);
// 		if(m_sendBuf.empty()) {
// 			LOG_ERROR << m_logic->name() << " m_sendBuf.empty(), m_isWaitingWrite = " << m_isWaitingWrite;
// 			return;
// 		}
// 	}

	// 1. �����ͻ�����������ȫ��ȡ��
	evbuffer sendSwapBuf;
	evbuffer *buf = &sendSwapBuf;

	{
		lock_guard_t<> lock(m_sendBufLock);
		evbuffer_add_buffer(buf, &m_sendBuf);
		m_isWaitingWrite = false;
	}

	// 2. ���Է������ݣ�������δ��ȫ�����ͣ���ע��д�¼�
	int left = trySend(buf);
	if (left < 0) {
		// �����쳣: ��ر�����
		// LOG_ERROR << m_logic->name() << " = socket<" << m_sockfd << "> trySend fail, ret = " << left;
		m_error = true;
		this ->close();
		return;
	} else if (left > 0) {
		// ����δ��ȫ������: �򽫲����������¿��������ͻ�������ͷ���Ա�����ȷ�ķ���˳��
		// LOG_ERROR << m_logic->name() << " register write, socket = " << m_sockfd;
		{
			lock_guard_t<> lock(m_sendBufLock);
			int size = evbuffer_get_length(&m_sendBuf);

			if (size > 0) {
				evbuffer_prepend_buffer(&m_sendBuf, buf);
			} else {
				evbuffer_add_buffer(&m_sendBuf, buf);
			}

			m_isWaitingWrite = true;
		}

		// ע��д�¼����Դ��������ӿ�дʱ�ٳ��Է���
		m_net->enableWrite(this);
#ifdef WIN
		// LOG_INFO << m_logic->name() << " register write, m_sendBuf.readableBytes() = " << m_sendBuf.readableBytes();
#endif
	} else {
		// ���������ѷ��ͳɹ�: ��鱾�����Ƿ���<���ر�>���ǵĻ���������������ȫ�����ͳɹ����ڴ��ڼ�û���µ����ݵȴ����ͣ���ִ��close����
		bool isWaitingClose = false;
		{
			lock_guard_t<> lock(m_sendBufLock);
			isWaitingClose = (m_isWaitingClose && (0 == evbuffer_get_length(&m_sendBuf)));

// 			if(isWaitingClose) {
// 				LOG_ERROR << m_logic->name() << " isWaitingClose = true, m_sendBuf.size() = " << m_sendBuf.readableBytes() << " && m_isWaitingWrite = " << m_isWaitingWrite;
// 			}
		}

		if(isWaitingClose) {
			close();
		}
	}
}

void Link::sendBuffer()
{
	if (!isopen()) {
		return;
	}

// 	{
// 		lock_guard_t<> lock(m_sendBufLock);
// 		if (m_isWaitingWrite) {
// 			LOG_ERROR << "m_isWaitingWrite = true";
// 			return;
// 		}
//
// 		if (evbuffer_get_length(&m_sendBuf) == 0) {
// 			LOG_ERROR << m_logic->name() << " m_sendBuf.empty(), socket = " << m_sockfd;
// 			return;
// 		}
//
// 		m_isWaitingWrite = true;
// 	}

	// m_net->interruptLoop();
	m_net->getTaskQueue()->put(boost::bind(&Link::onSend, this));
}

void Link::send(const char *data, int len)
{
	if (!isopen()) {
		return;
	}

	{
		lock_guard_t<> lock(m_sendBufLock);
		evbuffer_add(&m_sendBuf, data, len);

		if (m_isWaitingWrite) {
			return;
		}

		m_isWaitingWrite = true;
	}

	this->sendBuffer();
}

void Link::send(const char *text)
{
	send(text, strlen(text));
}

void Link::send(int msgId, const Message & msg)
{
	if (!isopen()) {
		return;
	}

	int size = msg.ByteSize();

	std::string buf;
	buf.resize(size);

	msg.SerializeToArray((void*)buf.c_str(), size);

	this->send(msgId, buf.c_str(), size);
}

void Link::send(int msgId, const char *data, int len)
{
	if (!isopen()) {
		return;
	}

	NetMsgHead msgHead;
	msgtool::BuildNetHeader(&msgHead, msgId, len);

	{
		lock_guard_t<> lock(m_sendBufLock);
		evbuffer_add(&m_sendBuf, &msgHead, sizeof(msgHead));
		evbuffer_add(&m_sendBuf, data, len);

		if (m_isWaitingWrite) {
			return;
		}
		m_isWaitingWrite = true;
	}

	this->sendBuffer();
}

void Link::beginRead(evbuffer *readto)
{
	// m_recvBuf����ȡ����readto
	lock_guard_t<> lock(m_recvBufLock);
	evbuffer_add_buffer(readto, &m_recvBuf);
	m_isWaitingRead = false;
}

void Link::endRead(evbuffer *remain)
{
	// remain�����ƶ���m_recvBufǰ��
	int left = evbuffer_get_length(remain);
	if (left > 0) {
		lock_guard_t<> lock(m_recvBufLock);
		int size = evbuffer_get_length(&m_recvBuf);
		if (size > 0) {
			evbuffer_prepend_buffer(&m_recvBuf, remain);
		} else {
			evbuffer_add_buffer(&m_recvBuf, remain);
		}
	}
}

void Link::handleRead()
{
	if (!isopen()) {
		LOG_ERROR << m_logic->name() << " != open";
		return;
	}

	// ��¼���ν������ݵ��ܳ���
	int totalRecvLen = 0;

	// ѭ����������ֱ���޷��ٽ���
	int nread = 0;
	do {
		//nread = evbuffer_read(buf, m_sockfd, -1);
		nread = ::recv(m_sockfd, m_net->g_recvBuf, MAX_PACKET_LEN, 0);
		if (nread > 0) {
			// �ɹ����յ����ݣ������ν��յ������ݿ��������ջ�����ĩβ
			{
				lock_guard_t<> lock(m_recvBufLock);
				evbuffer_add(&m_recvBuf, m_net->g_recvBuf, nread);
			}

			totalRecvLen += nread;
		} else if (0 == nread) { // eof
			// ���յ�0�ֽڵ�����: ˵���Ѽ�⵽�Զ˹رգ���ʱֱ�ӹرձ����ӣ����ٴ���δ���͵�����
			// LOG_WARN << m_logic->name() << " read 0, closed! buffer len = " << m_recvBuf.readableBytes() << ", g_closecnt = " << g_closecnt;
			m_isPeerClosed = true;
			this->close();
			break;
		} else {
			// �����쳣��EAGAIN��EWOULDBLOCK�ź�˵���ѽ�����ϣ�EINTR�ź�Ӧ���ԣ������ź�˵�������ӷ�������
			int err = socktool::geterrno();
			if(EINTR == err) {
				// LOG_WARN << "socket<" << m_sockfd << "> error = EINTR " << err;
				continue;
			} else if(EAGAIN == err || EWOULDBLOCK == err) {
				// LOG_WARN << "read task socket<" << m_sockfd << "> EWOULDBLOCK || EAGAIN, err = " << err;
				break;
			} else {
				// LOG_SOCKET_ERR(m_sockfd, err) << m_logic->name() << " recv fail, err = " << err << ", history recv buf size = " << m_recvBuf.readableBytes();
				m_error = true;
				this->close();
				break;
			}
		}
	} while(true);

	if(0 == totalRecvLen) {
		return;
	}

	// ���ҵ����Ƿ��Ѵ��ڴ������ӽ������ݵ����������������ٴ�֪ͨҵ��㣬��֮ǰ�����ݴ�������һ�������ν��յ�����������
	{
		lock_guard_t<> lock(m_recvBufLock);
		if (m_isWaitingRead) {
			return;
		}

		m_isWaitingRead = true;
	}

	// ��ҵ���������ݽ��մ���
	m_logic->onRecv(this);
}

void Link::handleWrite()
{
	// LOG_INFO << m_logic->name() << " socket <" << m_sockfd << "> is writable";
	if (!isopen()) {
		return;
	}

#ifdef WIN
	// windows��selectģ������LT����Ҫ���ο�д�¼�
	m_net->disableWrite(this);
#endif

	onSend();
	return;
}

void Link::handleError()
{
	if (m_isPeerClosed || m_error || m_isWaitingClose || m_closed) {
		return;
	}

	int err = socktool::getSocketError(m_sockfd);
	LOG_SOCKET_ERR(m_sockfd, err) << m_logic->name() << " socket<" << m_sockfd << "> error, m_error = " << m_error << ", m_isWaitingClose=" << m_isWaitingClose
	                              << ", m_closed = " << m_closed;

	m_error = true;
	this->close();
	return;
}

int Link::trySend(evbuffer *buffer)
{
	size_t nleft = evbuffer_get_length(buffer);
	int nwritten = 0;

	// ѭ����������ֱ���޷��ٷ���
	while(nleft > 0) {
		nwritten = evbuffer_write(buffer, m_sockfd);
		if (nwritten > 0) {
			nleft  -= nwritten;
		} else if(SOCKET_ERROR == nwritten) {
			int err = socktool::geterrno();
			switch(err) {
			case EINTR:
				// ����(������˵������socket����send���������ᴥ��EINTR�źţ������Է���һ)
				break;

			case EAGAIN:
#ifdef WIN
			case EWOULDBLOCK:
#endif
				// ˵�����޷��ٷ��ͣ��жϷ���
				return nleft;

			default:
				// LOG_SOCKET_ERR(m_sockfd, err) << m_logic->name() << "  send fail, err = " << err << ",nleft = " << nleft << ", nwritten = " << nwritten;
				return -1;
			}
		}
	}

	return nleft;
}

NetAddress Link::getLocalAddr() const
{
	return NetAddress(socktool::getLocalAddr(m_sockfd));
}

NetAddress Link::getPeerAddr() const
{
	return NetAddress(socktool::getPeerAddr(m_sockfd));
}
