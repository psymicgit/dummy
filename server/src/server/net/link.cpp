///<------------------------------------------------------------------------------
//< @file:   server\net\link.cpp
//< @author: 洪坤安
//< @date:   2015年1月14日 23:7:5
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "link.h"

#include <google/protobuf/message.h>

#include "netaddress.h"
#include "netreactor.h"
#include "listener.h"
#include "session.h"

#include "log/log.h"
#include "tool/sockettool.h"
#include "protocol/message.h"

#include "tool/atomictool.h"

#define READ_BLOCK_SIZE 40960

void Link::open()
{
	socktool::setNonBlocking(m_sockfd);
	socktool::setKeepAlive(m_sockfd, true, 120);

	// socktool::setSendBufSize(m_sockfd, 256 * 1024);
	// socktool::setRecvBufSize(m_sockfd, 256 * 1024);

	if (!socktool::setTcpNoDelay(m_sockfd)) {
		LOG_WARN << "local port = " << m_localAddr.toPort();
	}


	m_net->addFd(this);
}

void Link::enableRead()
{
	m_net->enableRead(this);
}

void Link::close()
{
	// LOG_INFO << "Link::close, socket = " << m_sockfd;
	if (m_closed) {
		return;
	}

	if (!m_sendBuf.empty()) {
		LOG_ERROR << "Link::close, m_sendBuf != empty(), left size = " << m_sendBuf.readableBytes() << "socket = " << m_sockfd;
	}

	m_closed = true;

	socktool::closeSocket(m_sockfd);
	m_net->disableAll(this);

	// LOG_WARN << "close socket<" << m_sockfd << ">";
	m_pNetReactor->getTaskQueue().put(boost::bind(&Link::onLogicClose, this));
}

void Link::erase()
{
	LinkPool &linkPool = m_net->getLinkPool();
	linkPool.free(this);
}

void Link::onLogicClose()
{
	m_pNetReactor->onDisconnect(this, m_localAddr, m_peerAddr);

#ifdef WIN
	m_net->getTaskQueue()->put(boost::bind(&Link::onNetClose, this));
#else
	onNetClose();
#endif
}

void Link::onNetClose()
{
	// LOG_INFO << "Link::onNetClose, socket = " << m_sockfd;
	m_net->delFd(this);
}

void Link::onSend()
{
	if (!isopen()) {
		return;
	}

	// LOG_INFO << "Link::onSend, socket = " << m_sockfd;

	{
		lock_guard_t<fast_mutex> lock(m_sendBufLock);
		if (m_sendBuf.empty()) {
			LOG_ERROR << "Link::onSend, m_sendBuf.empty(), socket = " << m_sockfd;
			return;
		}
	}

	Buffer buf;

	{
		lock_guard_t<fast_mutex> lock(m_sendBufLock);
		buf.swap(m_sendBuf);
	}

	int len = buf.readableBytes();

	int left = trySend(buf);
	if (left < 0) {
		LOG_ERROR << "socket<" << m_sockfd << "> trySend fail, ret = " << left;
		this ->close();
		return;
	} else if (left > 0) {
		// LOG_WARN << "m_net->enableWrite <" << m_sockfd << ">";
		LOG_ERROR << "Link::onSend, register write, socket = " << m_sockfd;
		{
			lock_guard_t<fast_mutex> lock(m_sendBufLock);
			if (!m_sendBuf.empty()) {
				buf.append(m_sendBuf.peek(), m_sendBuf.readableBytes());
				m_sendBuf.swap(buf);
			} else {
				m_sendBuf.swap(buf);
			}
		}

		m_net->enableWrite(this);
	} else {
		{
			// 发送成功
			lock_guard_t<fast_mutex> lock(m_sendBufLock);
			m_isWaitingWrite = false;
		}

		// 检测期间是否有新的数据被添加到发送缓冲区
		if (!m_sendBuf.empty()) {
			sendBuffer();
			LOG_ERROR << "Link::onSend, m_sendBuf != empty(), left size = " << m_sendBuf.readableBytes() << "socket = " << m_sockfd;
		}
	}
}

void Link::sendBuffer()
{
	if (!isopen()) {
		return;
	}

	{
		lock_guard_t<fast_mutex> lock(m_sendBufLock);
		if (m_isWaitingWrite) {
			return;
		}

		m_isWaitingWrite = true;
	}

	m_net->getTaskQueue()->put(boost::bind(&Link::onSend, this));
}

void Link::send(const char *data, int len)
{
	if (!isopen()) {
		return;
	}

	{
		lock_guard_t<fast_mutex> lock(m_sendBufLock);
		m_sendBuf.append(data, len);
	}

	this->sendBuffer();
}

void Link::send(const char *text)
{
	send(text, strlen(text));
}

void Link::send(int msgId, Message & msg)
{
	if (!isopen()) {
		return;
	}

	int size = msg.ByteSize();

	NetMsgHead msgHead = {0, 0};
	msgtool::buildNetHeader(&msgHead, msgId, size);

	memcpy(global::g_sendBuf, (const char*)&msgHead, sizeof(msgHead));
	msg.SerializeToArray(global::g_sendBuf + sizeof(msgHead), size);

	{
		lock_guard_t<fast_mutex> lock(m_sendBufLock);
		m_sendBuf.append(global::g_sendBuf, sizeof(msgHead) + size);
	}

	this->sendBuffer();
}

void Link::send(int msgId, const char *data, int len)
{
	if (!isopen()) {
		return;
	}

	NetMsgHead msgHead = {0, 0};
	msgtool::buildNetHeader(&msgHead, msgId, len);

	memcpy(global::g_sendBuf, (const char*)&msgHead, sizeof(msgHead));
	memcpy(global::g_sendBuf + sizeof(msgHead), data, len);

	{
		lock_guard_t<fast_mutex> lock(m_sendBufLock);
		m_sendBuf.append(global::g_sendBuf, sizeof(msgHead) + len);
	}

	this->sendBuffer();
}

int Link::handleRead()
{
	// LOG_WARN << "socket<" << m_sockfd << "> read task";

	if (!isopen()) {
		return 0;
	}

	int nread = 0;
	do {
		nread = ::recv(m_sockfd, global::g_recvBuf, MAX_PACKET_LEN, NULL);
		if (nread > 0) {
			{
				lock_guard_t<fast_mutex> lock(m_recvBufLock);
				m_recvBuf.append(global::g_recvBuf, nread);
			}

			if (nread < MAX_PACKET_LEN) {
				break; // 相当于EWOULDBLOCK
			}
		} else if (0 == nread) { // eof
			// LOG_WARN << "socket<" << m_sockfd << "> read 0, closed! buffer len = " << MAX_PACKET_LEN;
			this->close();
			return -1;
		} else {
			int err = socktool::geterrno();
			if(EINTR == err) {
				// LOG_WARN << "socket<" << m_sockfd << "> error = EINTR " << err;
				continue;
			} else if(EAGAIN == err || EWOULDBLOCK == err) {
				// LOG_WARN << "read task socket<" << m_sockfd << "> EWOULDBLOCK || EAGAIN, err = " << err;
				break;
			} else {
				LOG_SOCKET_ERR(m_sockfd, err) << "socket<" << m_sockfd << "> recv fail, err = " << err << ", history recv buf size = " << m_recvBuf.readableBytes();
				this->close();
				return -1;
			}
		}
	} while(true);

	//m_pNetReactor->getTaskQueue().put(boost::bind(&INetReactor::onRecv, m_pNetReactor, this, m_recvBuf));

	{
		lock_guard_t<fast_mutex> lock(m_recvBufLock);
		if (m_isWaitingRead) {
			return 0;
		}

		m_isWaitingRead = true;
	}

	m_pNetReactor->onRecv(this, m_recvBuf);
	return 0;
}

int Link::handleWrite()
{
	LOG_INFO << "socket <" << m_sockfd << "> is writable";
	if (!isopen()) {
		return 0;
	}

#ifdef WIN
	// windows下select模型属于LT，需要屏蔽可写事件
	m_net->disableWrite(this);
#endif

	onSend();
	return 0;
}

int Link::handleError()
{
	LOG_WARN << "socket<" << m_sockfd << "> error";
	this->close();
	return 0;
}

int Link::trySend(Buffer &buffer)
{
	size_t nleft = buffer.readableBytes();
	int nwritten = 0;

	while(nleft > 0) {
		nwritten = ::send(m_sockfd, buffer.peek(), nleft, MSG_NOSIGNAL);
		if (nwritten > 0) {
			nleft  -= nwritten;
			buffer.skip(nwritten);
		} else if(SOCKET_ERROR == nwritten) {
			int err = socktool::geterrno();
			switch(err) {
			case EINTR:
				// 忽略(正常来说非阻塞socket进行send操作并不会触发EINTR信号，这里以防万一)
				break;

			case EAGAIN:
#ifdef WIN
			case EWOULDBLOCK:
#endif
				return nleft;

			default:
				LOG_SOCKET_ERR(m_sockfd, err) << "socket<" << m_sockfd << "> send fail, err = " << err << ",nleft = " << nleft << ", nwritten = " << nwritten;
				return -1;
			}
		}
	}

	return nleft;
}

NetAddress Link::getLocalAddr()
{
	return NetAddress(socktool::getLocalAddr(m_sockfd));
}
