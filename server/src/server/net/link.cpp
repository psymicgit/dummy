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

void Link::onSend(Buffer *buf)
{
	if (!isopen()) {
		return;
	}

	// LOG_INFO << "Link::onSend, socket = " << m_sockfd;

	// 如果发送缓存区仍有数据未发送，则直接append
	if (!m_sendBuf.empty()) {
		// LOG_WARN << "socket<" << m_sockfd << "> m_sendBuf.append(buf.peek(), buf.readableBytes());";
		m_sendBuf.append(buf->peek(), buf->readableBytes());
		global::g_bufferPool.free(buf);
		return;
	}

	int ret = trySend(*buf);

	if (ret < 0) {
		// LOG_ERROR << "socket<" << m_sockfd << "> trySend fail, ret = " << ret;
		this ->close();
	} else if (ret > 0) {
		// LOG_WARN << "m_net->enableWrite <" << m_sockfd << ">";
		m_net->enableWrite(this);
		m_sendBuf.append(buf->peek(), buf->readableBytes());
	} else {
		// 发送成功
	}

	global::g_bufferPool.free(buf);
}

void Link::sendBuffer(Buffer *buf)
{
	if (!isopen()) {
		return;
	}

	m_net->getTaskQueue()->put(boost::bind(&Link::onSend, this, buf));
}

void Link::send(const char *data, int len)
{
	if (!isopen()) {
		return;
	}

	Buffer *buf = global::g_bufferPool.alloc(data, len);
	this->sendBuffer(buf);
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

	Buffer *buf = global::g_bufferPool.alloc(sizeof(msgHead) + size);
	buf->append((const char*)&msgHead, sizeof(msgHead));

	msg.SerializeToArray((void*)buf->beginWrite(), size);
	buf->hasWritten(size);

	this->sendBuffer(buf);
}

void Link::send(int msgId, const char *data, int len)
{
	if (!isopen()) {
		return;
	}

	NetMsgHead msgHead = {0, 0};
	msgtool::buildNetHeader(&msgHead, msgId, len);

	Buffer *buf = global::g_bufferPool.alloc(msgHead.msgLen);
	buf->append((const char*)&msgHead, sizeof(msgHead));
	buf->append(data, len);

	this->sendBuffer(buf);
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
			m_recvBuf.append(global::g_recvBuf, nread);

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

	m_pNetReactor->onRecv(this, m_recvBuf, *m_head);
	return 0;
}

int Link::handleWrite()
{
	// LOG_INFO << "socket <" << m_sockfd << "> is writable";
	if (!isopen()) {
		return 0;
	}

#ifdef WIN
	// windows下select模型属于LT，需要屏蔽可写事件
	m_net->disableWrite(this);
#endif

	if (m_sendBuf.empty()) {
		return 0;
	}

	do {
		int ret = trySend(m_sendBuf);
		if (ret < 0) {
			// LOG_WARN << "close socket<" << m_sockfd << ">";
			this->close();
			return -1;
		}
	} while (m_sendBuf.readableBytes() > 0);

	m_sendBuf.clear();
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
