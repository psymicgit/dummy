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

void Link::open()
{
	socktool::setNonBlocking(m_sockfd);
	socktool::setKeepAlive(m_sockfd, true, 120);

	// socktool::setSendBufSize(m_sockfd, 256 * 1024);
	// socktool::setRecvBufSize(m_sockfd, 256 * 1024);

	if (!socktool::setTcpNoDelay(m_sockfd)) {
		LOG_ERROR << m_pNetReactor->name() << " " << getLocalAddr().toIpPort() << "<-->" << getPeerAddr().toIpPort() << " setTcpNoDelay failed = " << m_localAddr.toPort();
	}

	m_net->addFd(this);
}

void Link::close()
{
	// 检测是否重复close
	if (m_closed) {
		return;
	}

	// LOG_INFO << m_pNetReactor->name() << " Link::close, socket = " << m_sockfd;

	// 如果未发生错误，则先将未发送的数据发送完毕
	if (!m_error) {
		lock_guard_t<> lock(m_sendBufLock);

		// 若数据未发送完毕，则暂缓关闭，只停止接收数据，等待之前的发送操作执行完毕后再关闭
		if (!m_sendBuf.empty()) {
			if (m_isWaitingClose) {
				return;
			}

			m_isWaitingClose = true;
			m_net->disableRead(this);

			if (!m_isWaitingWrite) {
				LOG_ERROR << m_pNetReactor->name() << " m_sendBuf != empty(), left size = " << m_sendBuf.readableBytes() << ", socket = " << m_sockfd;
			}

			return;
		}
	}

	m_closed = true;

// 首先屏蔽本连接上的所有网络输出
	socktool::closeSocket(m_sockfd);

#ifdef WIN
	// 若是windows平台，则需要再从读写集上取消注册
	m_net->disableAll(this);
#endif

// 等业务层处理好关闭操作
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
	m_net->delFd(this);
}

void Link::onSend()
{
	if (!isopen()) {
		return;
	}

	// LOG_INFO << "Link::onSend, socket = " << m_sockfd;
	bool isSendBufEmpty = false;
	{
		lock_guard_t<> lock(m_sendBufLock);
		isSendBufEmpty = m_sendBuf.empty();
	}

	if (isSendBufEmpty) {
		// LOG_ERROR << m_pNetReactor->name() << " m_sendBuf.empty(), socket = " << m_sockfd;
		return;
	}

	// 1. 将发送缓冲区的数据全部取出
	Buffer buf;

	{
		lock_guard_t<> lock(m_sendBufLock);
		buf.swap(m_sendBuf);

		m_isWaitingWrite = false;
	}

	// 2. 尝试发送数据，若数据未能全部发送，则注册写事件
	int left = trySend(buf);
	if (left < 0) {
		// 若发送异常，则关闭连接
		// LOG_ERROR << m_pNetReactor->name() << " = socket<" << m_sockfd << "> trySend fail, ret = " << left;
		m_error = true;
		this ->close();
		return;
	} else if (left > 0) {
		// 若数据未能全部发送，则将残余数据重新拷贝到发送缓冲区的头部以保持正确的发送顺序
		// LOG_ERROR << m_pNetReactor->name() << " register write, socket = " << m_sockfd;
		{
			lock_guard_t<> lock(m_sendBufLock);
			if (!m_sendBuf.empty()) {
				buf.append(m_sendBuf.peek(), m_sendBuf.readableBytes());
				m_sendBuf.swap(buf);
			} else {
				m_sendBuf.swap(buf);
			}
		}

		// 注册写事件，以待当本连接可写时再尝试发送
		m_net->enableWrite(this);
		// LOG_WARN << "m_net->enableWrite <" << m_sockfd << ">";
	} else {
		// 检查本连接是否已被标记为<待关闭>，是的话，若本次数据已全部发送成功且在此期间没有新的数据等待发送，则执行close操作
		bool isWaitingClose = false;
		{
			lock_guard_t<> lock(m_sendBufLock);
			isWaitingClose = (m_isWaitingClose && m_sendBuf.empty());

			if (!m_sendBuf.empty() && !m_isWaitingWrite) {
				LOG_ERROR << "m_sendBuf.readableBytes() = " << m_sendBuf.readableBytes() << "&& m_isWaitingWrite = " << m_isWaitingWrite;
			}
		}

		if(isWaitingClose) {
			// LOG_ERROR << m_pNetReactor->name() << "isWaitingClose = true m_sendBuf.readableBytes() = " << m_sendBuf.readableBytes() << " && m_isWaitingWrite = " << m_isWaitingWrite;
			close();
		}
	}
}

void Link::sendBuffer()
{
	if (!isopen()) {
		return;
	}

	{
		lock_guard_t<> lock(m_sendBufLock);
		if (m_isWaitingWrite) {
			return;
		}

		if (m_sendBuf.empty()) {
			// LOG_ERROR << m_pNetReactor->name() << " m_sendBuf.empty(), socket = " << m_sockfd;
			return;
		}

		m_isWaitingWrite = true;
	}

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

	msg.SerializeToArray(global::g_sendBuf, size);

	{
		lock_guard_t<> lock(m_sendBufLock);
		m_sendBuf.append((const char*)&msgHead, sizeof(msgHead));
		m_sendBuf.append(global::g_sendBuf, size);
	}

	this->sendBuffer();
}

void Link::send(int msgId, const char *data, int len)
{
	if (!isopen()) {
		return;
	}

	NetMsgHead *msgHead = (NetMsgHead*)global::g_sendBuf;
	msgtool::buildNetHeader(msgHead, msgId, len);

	{
		lock_guard_t<> lock(m_sendBufLock);
		m_sendBuf.append(global::g_sendBuf, sizeof(msgHead));
		m_sendBuf.append(data, len);
	}

	this->sendBuffer();
}

int Link::handleRead()
{
	// LOG_WARN << "socket<" << m_sockfd << "> read task";

	if (!isopen()) {
		return 0;
	}

	// 循环接收数据直到无法再接收
	int nread = 0;
	do {
		nread = ::recv(m_sockfd, global::g_recvBuf, MAX_PACKET_LEN, NULL);
		if (nread > 0) {
			// 若成功接收到数据，则将本次接收到的数据拷贝到接收缓冲区末尾
			{
				lock_guard_t<> lock(m_recvBufLock);
				m_recvBuf.append(global::g_recvBuf, nread);
			}

			// 若本次接收数据长度并未超出最大接收长度，则说明目前本连接上的数据已全部接收完毕
			if (nread < MAX_PACKET_LEN) {
				break; // 相当于EWOULDBLOCK
			}
		} else if (0 == nread) { // eof
			// 若接收到0字节的数据，则说明已检测到对端关闭，此时直接关闭本连接，不再处理未发送的数据

			// LOG_WARN << "socket<" << m_sockfd << "> read 0, closed! buffer len = " << MAX_PACKET_LEN;
			this->close();
			return -1;
		} else {
			// 发生异常：EAGAIN及EWOULDBLOCK信号说明已接收完毕，EINTR信号应忽略，其余信号说明本连接发生错误
			int err = socktool::geterrno();
			if(EINTR == err) {
				// LOG_WARN << "socket<" << m_sockfd << "> error = EINTR " << err;
				continue;
			} else if(EAGAIN == err || EWOULDBLOCK == err) {
				// LOG_WARN << "read task socket<" << m_sockfd << "> EWOULDBLOCK || EAGAIN, err = " << err;
				break;
			} else {
				// LOG_SOCKET_ERR(m_sockfd, err) << m_pNetReactor->name() << " recv fail, err = " << err << ", history recv buf size = " << m_recvBuf.readableBytes();
				m_error = true;
				this->close();
				return -1;
			}
		}
	} while(true);

	//m_pNetReactor->getTaskQueue().put(boost::bind(&INetReactor::onRecv, m_pNetReactor, this, m_recvBuf));

	// 检测业务层是否已存在处理本连接接收数据的任务，若有则无需再次通知业务层，由之前的数据处理任务一并处理本次接收到的所有数据
	{
		lock_guard_t<> lock(m_recvBufLock);
		if (m_isWaitingRead) {
			return 0;
		}

		m_isWaitingRead = true;
	}

	// 由业务层进行数据接收处理
	m_pNetReactor->onRecv(this, m_recvBuf);
	return 0;
}

int Link::handleWrite()
{
	// LOG_INFO << m_pNetReactor->name() << " socket <" << m_sockfd << "> is writable";
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
	if (m_error || m_isWaitingClose || !isopen()) {
		return 0;
	}

	// int err = socktool::getSocketError(m_sockfd);
	// LOG_SOCKET_ERR(m_sockfd, err) << m_pNetReactor->name() << " socket<" << m_sockfd << "> error";

	m_error = true;
	this->close();
	return 0;
}

int Link::trySend(Buffer &buffer)
{
	size_t nleft = buffer.readableBytes();
	int nwritten = 0;

	// 循环发送数据直到无法再发送
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
				// 说明已无法再发送，中断发送
				return nleft;

			default:
				// LOG_SOCKET_ERR(m_sockfd, err) << m_pNetReactor->name() << "  send fail, err = " << err << ",nleft = " << nleft << ", nwritten = " << nwritten;
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
