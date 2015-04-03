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

void Link::open()
{
	socktool::setNonBlocking(m_sockfd);
	socktool::setTcpNoDelay(m_sockfd);

	m_net->addFd(this);
	m_net->enableRead(this);
}

void Link::close()
{
	if (m_isClosing) {
		return;
	}

	m_isClosing = true;

	// LOG_WARN << "close socket<" << m_sockfd << ">";
	m_pNetReactor->getTaskQueue().put(task_binder_t::gen(&Link::onLogicClose, this));
}

void Link::onLogicClose()
{
	m_pNetReactor->onDisconnect(this, m_localAddr, m_peerAddr);
	m_taskQueue->produce(task_binder_t::gen(&Link::onNetClose, this));
}

void Link::onNetClose()
{
	socktool::closeSocket(m_sockfd);
	m_net->delFd(this);
}

void Link::onSend(Buffer &buf)
{
// 	if (!isopen()) {
// 		return;
// 	}

	// 如果发送缓存区仍有数据未发送，则直接append
	if (m_sendBuf.readableBytes() > 0) {
		LOG_WARN << "socket<" << m_sockfd << "> m_sendBuf.append(buf.peek(), buf.readableBytes());";
		m_sendBuf.append(buf.peek(), buf.readableBytes());
		return;
	}

	int ret = trySend(buf);

	if (ret < 0) {
		this ->close();
	}
	else if (ret > 0) {
		m_net->enableWrite(this);
		m_sendBuf.append(buf.peek(), buf.readableBytes());
	}
	else {
		// 发送成功
	}
}

void Link::send(Buffer & buf)
{
	m_taskQueue->produce(task_binder_t::gen(&Link::onSend, this, buf));
}

void Link::send(const char *data, int len)
{
	Buffer buf(len);
	buf.append(data, len);
	send(buf);
}

void Link::send(string & msg)
{
	send(msg.data(), msg.size());
}

void Link::send(const char *msg)
{
	send(msg, strlen(msg));
}

void Link::send(int msgId, google::protobuf::Message & msg)
{
	int size = msg.ByteSize();

	NetMsgHead msgHead = {0, 0};
	msgtool::buildNetHeader(&msgHead, msgId, size);

	Buffer buf;
	buf.append((const char*)&msgHead, sizeof(msgHead));

	msg.SerializeToArray((void*)buf.beginWrite(), size);
	buf.hasWritten(size);

	this->send(buf);
}

void Link::send(int msgId, const char *data, int len)
{
	NetMsgHead msgHead = {0, 0};
	msgtool::buildNetHeader(&msgHead, msgId, len);

	Buffer buf(msgHead.msgLen);
	buf.append((const char*)&msgHead, sizeof(msgHead));
	buf.append(data, len);

	this->send(buf);
}

int Link::handleRead()
{
#ifdef WIN32
	handleReadTask();
#else
	m_taskQueue->produce(task_binder_t::gen(&Link::handleReadTask, this));
#endif

	return 0;
}

int Link::handleWrite()
{
#ifdef WIN32
	handleWriteTask();
#else
	m_taskQueue->produce(task_binder_t::gen(&Link::handleWriteTask, this));
#endif

	return 0;
}

int Link::handleError()
{
	LOG_WARN << "socket<" << m_sockfd << "> error";
	this->close();
	return 0;
}

int Link::handleReadTask()
{
	// LOG_WARN << "socket<" << m_sockfd << "> read task";

// 	if (!isopen()) {
// 		return 0;
// 	}

	int nread = 0;
	char recvBuf[8096];

	do {
		nread = ::recv(m_sockfd, recvBuf, sizeof(recvBuf) - 1, NULL);
		if (nread > 0) {
			m_recvBuf.append(recvBuf, nread);

			// LOG_WARN << "read task socket<" << m_sockfd << "> recv nread = " << nread;

			if (nread < int(sizeof(recvBuf) - 1)) {
				break; // 相当于EWOULDBLOCK
			}
		}
		else if (0 == nread) {   //! eof
			// LOG_WARN << "socket<" << m_sockfd << "> read 0, closed";
			this->close();
			return -1;
		}
		else {
			int err = errno;
			if (err == EINTR) {
				continue;
			}
			else if (err == EWOULDBLOCK || err == EAGAIN) {
				// LOG_WARN << "read task socket<" << m_sockfd << "> EWOULDBLOCK || EAGAIN, err = " << err;

				break;
			}
			else {
				LOG_WARN << "socket<" << m_sockfd << "> error = " << err;
				this->close();
				return -1;
			}
		}
	}
	while(true);

	m_pNetReactor->onRecv(this, m_recvBuf);
	return 0;
}

int Link::handleWriteTask()
{
	LOG_INFO << "socket <" << m_sockfd << "> is writable";

// 	if (!isopen()) {
// 		return 0;
// 	}

	int ret = 0;
	string left_buff;

#ifdef WIN
	// windows下select模型属于LT，需要屏蔽可写事件
	m_net->disableWrite(this);
#endif

	if (m_sendBuf.empty()) {
		return 0;
	}

	do {
		ret = trySend(m_sendBuf);

		if (ret < 0) {
			LOG_WARN << "close socket<" << m_sockfd << ">";

			this->close();
			return -1;
		}
	}
	while (m_sendBuf.readableBytes() > 0);
	return 0;
}

int Link::trySend(Buffer &buffer)
{
	size_t nleft     = buffer.readableBytes();
	const char* buff = buffer.peek();

	int nwritten = 0;

	while(nleft > 0) {
		if((nwritten = ::send(m_sockfd, buff, nleft, MSG_NOSIGNAL)) <= 0) {
			if (EINTR == errno) {
				nwritten = 0;
			}
			else if (EWOULDBLOCK == errno || EAGAIN == errno) {
				break;
			}
			else {
				LOG_WARN << "close socket<" << m_sockfd << ">";
				return -1;
			}
		}

		nleft  -= nwritten;
		buff   += nwritten;

		buffer.retrieve(nwritten);
	}

	return nleft;
}

NetAddress Link::getLocalAddr()
{
	return NetAddress(socktool::getLocalAddr(m_sockfd));
}
