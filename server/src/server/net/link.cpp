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
	m_net->getTaskQueue().put(task_binder_t::gen(&INet::addFd, m_net, this));
	m_net->getTaskQueue().put(task_binder_t::gen(&INet::enableRead, m_net, this));
}

void Link::close()
{
	if (m_isClosing) {
		return;
	}

	m_isClosing = true;

	m_net->getTaskQueue().put(task_binder_t::gen(&INet::disableAll, m_net, m_sockfd));
	m_pNetReactor->GetTaskQueue().put(task_binder_t::gen(&Link::onLogicClose, this));
}

void Link::onLogicClose()
{
	m_pNetReactor->OnDisconnect(this, m_localAddr, m_peerAddr);
	m_net->getTaskQueue().put(task_binder_t::gen(&Link::onClose, this));
}

void Link::onClose()
{
	socktool::closeSocket(m_sockfd);

	m_net->disableAll(m_sockfd);
	m_net->delFd(this);
}

void Link::onSend(Buffer & srcBuff)
{
	// 如果发送缓存区仍有数据未发送，则直接append
	if (m_sendBuf.readableBytes() > 0) {
		m_sendBuf.append(srcBuff.peek(), srcBuff.readableBytes());
		return;
	}

	int ret = trySend(srcBuff);

	if (ret < 0) {
		this ->close();
	}
	else if (ret > 0) {
		m_net->getTaskQueue().put(task_binder_t::gen(&INet::enableWrite, m_net, this));
		m_sendBuf.append(srcBuff.peek(), srcBuff.readableBytes());
	}
	else {
		//! send ok
	}
}

void Link::send(Buffer & buf)
{
	m_tq->produce(task_binder_t::gen(&Link::onSend, this, buf));
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

	NetMsgHead msgHead = {0};
	msgtool::buildNetHeader(&msgHead, msgId, size);

	Buffer buf;
	buf.append((const char*)&msgHead, sizeof(msgHead));

	msg.SerializeToArray((void*)buf.beginWrite(), size);
	buf.hasWritten(size);

	this->send(buf);
}

void Link::send(int msgId, const char *data, int len)
{
	NetMsgHead msgHead = {0};
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
	m_tq->produce(task_binder_t::gen(&Link::handleReadTask, this));
#endif

	return 0;
}

int Link::handleWrite()
{
#ifdef WIN32
	handleWriteTask();
#else
	m_tq->produce(task_binder_t::gen(&Link::handleWriteTask, this));
#endif

	return 0;
}

int Link::handleError()
{
#ifdef WIN32
	handleErrorTask();
#else
	m_tq->produce(task_binder_t::gen(&Link::handleErrorTask, this));
#endif

	return 0;
}

int Link::handleReadTask()
{
	if (isOpen()) {
		int nread = 0;
		char recv_buffer[8096];

		do {
			nread = ::recv(m_sockfd, recv_buffer, sizeof(recv_buffer) - 1, NULL);
			if (nread > 0) {
				m_recvBuf.append(recv_buffer, nread);

				if (nread < int(sizeof(recv_buffer) - 1)) {
					break;//! equal EWOULDBLOCK
				}
			}
			else if (0 == nread) {   //! eof
				this->close();
				return -1;
			}
			else {
				if (errno == EINTR) {
					continue;
				}
				else if (errno == EWOULDBLOCK) {
					break;
				}
				else {
					this->close();
					return -1;
				}
			}
		}
		while(1);
	}

	m_pNetReactor->OnRecv(this, m_recvBuf);
	return 0;
}

int Link::handleWriteTask()
{
	int ret = 0;
	string left_buff;

	m_net->getTaskQueue().put(task_binder_t::gen(&INet::disableWrite, m_net, this));

	if (m_sendBuf.readableBytes() == 0) {
		return 0;
	}

	do {
		ret = trySend(m_sendBuf);

		if (ret < 0) {
			this->close();
			return -1;
		}
	}
	while (m_sendBuf.readableBytes() > 0);
	return 0;
}

int Link::handleErrorTask()
{
	this->close();
	return 0;
}

int Link::trySend(Buffer & buffer)
{
	size_t nleft     = buffer.readableBytes();
	const char* buff = buffer.peek();

	int nwritten = 0;

	while(nleft > 0) {
		if((nwritten = ::send(m_sockfd, buff, nleft, MSG_NOSIGNAL)) <= 0) {
			if (EINTR == errno) {
				nwritten = 0;
			}
			else if (EWOULDBLOCK == errno) {
				// buffer.hasWritten(nwritten);
				// left_buff_.assign(buff_.c_str() + (buff_.size() - nleft), nleft);
				return 1;
			}
			else {
				this->close();
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
