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

	if (m_isClosing) {
		return;
	}

	m_isClosing = true;

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
	// LOG_INFO << "Link::onLogicClose, socket = " << m_sockfd;

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
		return;
	}

	int ret = trySend(*buf);

	if (ret < 0) {
		LOG_ERROR << "socket<" << m_sockfd << "> trySend fail, ret = " << ret;
		this ->close();
	}
	else if (ret > 0) {
		// LOG_WARN << "m_net->enableWrite <" << m_sockfd << ">";
		m_net->enableWrite(this);
		m_sendBuf.append(buf->peek(), buf->readableBytes());
	}
	else {
		// 发送成功
	}

	global::g_bufferPool.free(buf);
}

void Link::sendBuffer(Buffer *buf)
{
	if (!isopen()) {
		return;
	}

#ifdef WIN
	m_net->getTaskQueue()->put(boost::bind(&Link::onSend, this, buf));
#else
	m_net->getTaskQueue()->put(boost::bind(&Link::onSend, this, buf));
	//onSend(buf);
#endif
}

void Link::send(const char *data, int len)
{
	if (!isopen()) {
		return;
	}

	Buffer *buf = global::g_bufferPool.alloc(len);
	buf->append(data, len);
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
		// RingBufferBlock *block = m_net->m_ringbuffer.allocFreeBlock(READ_BLOCK_SIZE);
		// if (NULL == block) {
		// 	LOG_ERROR << "alloc block fail!";
		// 
		// 	m_net->m_ringbuffer.statistic();
		// 	exit(0);
		// }

		// nread = ::recv(m_sockfd, block->begin(), block->m_length, NULL);
		// if (nread > 0) {
		// 	m_recvBuf.append(block->begin(), nread);

		// if (nread < block->m_length) {
		// 	m_net->m_ringbuffer.splitBlock(block->m_length + sizeof(RingBufferBlock), nread);
		// }
		// 
		// m_net->m_ringbuffer.skip(block->m_length + sizeof(RingBufferBlock));
		// block->bind(this);

		// LOG_WARN << "read task socket<" << m_sockfd << "> recv nread = " << nread;

		nread = ::recv(m_sockfd, global::g_recvBuf, MAX_PACKET_LEN, NULL);
		if (nread > 0) {
			m_recvBuf.append(global::g_recvBuf, nread);

			if (nread < MAX_PACKET_LEN) {
				break; // 相当于EWOULDBLOCK
			}
		}
		else if (0 == nread) {   // eof
			// LOG_WARN << "socket<" << m_sockfd << "> read 0, closed! buffer len = " << MAX_PACKET_LEN;
			this->close();
			return -1;
		}
		else {
			int err = socktool::geterrno();
			if(EINTR == err) {
				// LOG_WARN << "socket<" << m_sockfd << "> error = EINTR " << err;
				continue;
			}
			else if(EAGAIN == err || EWOULDBLOCK == err) {
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

	m_pNetReactor->onRecv(this, m_recvBuf, *m_head);
	return 0;
}

int Link::handleWrite()
{
	// LOG_INFO << "socket <" << m_sockfd << "> is writable";
	if (!isopen()) {
		return 0;
	}

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
			// LOG_WARN << "close socket<" << m_sockfd << ">";
			this->close();
			return -1;
		}
	}
	while (m_sendBuf.readableBytes() > 0);

	m_sendBuf.clear();
	return 0;
}

int Link::handleError()
{
	LOG_WARN << "socket<" << m_sockfd << "> error";
	this->close();
	return 0;
}

/*
int Link::handleReadTask()
{
	// LOG_WARN << "socket<" << m_sockfd << "> read task";

	if (!isopen()) {
		return 0;
	}

	Buffer &recvBuf = m_net->m_recvBuf;
	recvBuf.clear();

	int skip = 0;
	int max = recvBuf.writableBytes();

	// 先将历史数据填充进全局缓冲区，因为历史数据一般都构不成一个消息包，所以大小不大
	if (m_head != NULL) {
		int historyLen = m_head->getTotalLength();
		m_head->take(recvBuf, historyLen);
		m_head->skip(historyLen);

		m_head = NULL;

		skip = historyLen;
	}

	RingBufferBlock block;
	int nread = 0;
	do {
		nread = ::recv(m_sockfd, recvBuf.begin() + skip, max - skip, NULL);
		if (nread > 0) {
			recvBuf.hasWritten(nread);

			skip += nread;

			if (skip >= max) {
				// LOG_WARN << "recv too much skip =" << skip << ", nread = " << nread << ", max = " << max;
				m_pNetReactor->onRecv(this, recvBuf, block);

				if (!recvBuf.empty()) {
					int remain = recvBuf.readableBytes();
					memcpy(recvBuf.begin(), recvBuf.peek(), remain);
					recvBuf.clear();
					recvBuf.hasWritten(remain);

					skip = remain;
				}
				else {
					skip = 0;
				}

				continue;
			}

			// m_recvBuf.append(global::g_recvBuf, nread);

			// m_net->m_ringbuffer.add(global::g_recvBuf, nread, this);

			// LOG_WARN << "read task socket<" << m_sockfd << "> recv nread = " << nread;

			if (nread < max - skip) {
				break; // 相当于EWOULDBLOCK
			}
		}
		else if (0 == nread) {   //! eof
			// LOG_WARN << "socket<" << m_sockfd << "> read 0, closed";
			this->close();
			return -1;
		}
		else {
			int err = socktool::geterrno();
			if (err == EINTR) {
				continue;
			}
			else if (err == EWOULDBLOCK || err == EAGAIN) {
				// LOG_WARN << "read task socket<" << m_sockfd << "> EWOULDBLOCK || EAGAIN, err = " << err;
				break;
			}
			else {
				// LOG_WARN << "socket<" << m_sockfd << "> error = " << err;
				this->close();
				return -1;
			}
		}
	}
	while(true);

	m_pNetReactor->onRecv(this, recvBuf, block);
	m_net->m_ringbuffer.add(recvBuf.peek(), recvBuf.readableBytes(), this);
	return 0;
}
*/

int Link::trySend(Buffer &buffer)
{
	size_t nleft = buffer.readableBytes();
	int nwritten = 0;

	while(nleft > 0) {
		nwritten = ::send(m_sockfd, buffer.peek(), nleft, MSG_NOSIGNAL);
		if (nwritten > 0) {
			nleft  -= nwritten;
			buffer.skip(nwritten);
		}
		else if(SOCKET_ERROR == nwritten) {
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
