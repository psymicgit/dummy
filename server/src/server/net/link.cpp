///<------------------------------------------------------------------------------
//< @file:   server\net\link.cpp
//< @author: 洪坤安
//< @date:   2015年1月14日 23:7:5
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
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

	// 投递到网络线程执行close命令
	m_net->getTaskQueue()->put(boost::bind(&Link::closing, this));
}

void Link::closing()
{
	// 检测是否重复close
	if (m_closed) {
		LOG_ERROR << m_logic->name() << " double closed, canceled, m_sendBuf left size = " << evbuffer_get_length(&m_sendBuf);
		return;
	}

	// 如果未发生错误，则先将未发送的数据发送完毕
	if (!m_error && !m_isPeerClosed) {
		lock_guard_t<> lock(m_sendBufLock);

		// 若数据未发送完毕，则暂缓关闭，只停止接收数据，等待之前的发送操作执行完毕后再关闭
		if (evbuffer_get_length(&m_sendBuf) >  0) {
			if (m_isWaitingClose) {
				// LOG_ERROR << m_logic->name() << " is waiting close";
				return;
			}

			m_isWaitingClose = true;
			// shutdown(m_sockfd, SHUT_RD);

			// 禁止读取数据
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

	// 首先屏蔽本连接上的所有网络输出
	socktool::closeSocket(m_sockfd);

#ifdef WIN
	// 若是windows平台，则需要再从读写集上取消注册
	m_net->disableAll(this);
#endif

	// 等业务层处理好关闭操作
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

	// 1. 将发送缓冲区的数据全部取出
	evbuffer sendSwapBuf;
	evbuffer *buf = &sendSwapBuf;

	{
		lock_guard_t<> lock(m_sendBufLock);
		evbuffer_add_buffer(buf, &m_sendBuf);
		m_isWaitingWrite = false;
	}

	// 2. 尝试发送数据，若数据未能全部发送，则注册写事件
	int left = trySend(buf);
	if (left < 0) {
		// 发送异常: 则关闭连接
		// LOG_ERROR << m_logic->name() << " = socket<" << m_sockfd << "> trySend fail, ret = " << left;
		m_error = true;
		this ->close();
		return;
	} else if (left > 0) {
		// 数据未能全部发送: 则将残余数据重新拷贝到发送缓冲区的头部以保持正确的发送顺序
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

		// 注册写事件，以待当本连接可写时再尝试发送
		m_net->enableWrite(this);
#ifdef WIN
		// LOG_INFO << m_logic->name() << " register write, m_sendBuf.readableBytes() = " << m_sendBuf.readableBytes();
#endif
	} else {
		// 本次数据已发送成功: 检查本连接是否已<待关闭>，是的话，若本次数据已全部发送成功且在此期间没有新的数据等待发送，则执行close操作
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
	// m_recvBuf数据取出到readto
	lock_guard_t<> lock(m_recvBufLock);
	evbuffer_add_buffer(readto, &m_recvBuf);
	m_isWaitingRead = false;
}

void Link::endRead(evbuffer *remain)
{
	// remain数据移动到m_recvBuf前面
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

	// 记录本次接收数据的总长度
	int totalRecvLen = 0;

	// 循环接收数据直到无法再接收
	int nread = 0;
	do {
		//nread = evbuffer_read(buf, m_sockfd, -1);
		nread = ::recv(m_sockfd, m_net->g_recvBuf, MAX_PACKET_LEN, 0);
		if (nread > 0) {
			// 成功接收到数据：将本次接收到的数据拷贝到接收缓冲区末尾
			{
				lock_guard_t<> lock(m_recvBufLock);
				evbuffer_add(&m_recvBuf, m_net->g_recvBuf, nread);
			}

			totalRecvLen += nread;
		} else if (0 == nread) { // eof
			// 接收到0字节的数据: 说明已检测到对端关闭，此时直接关闭本连接，不再处理未发送的数据
			// LOG_WARN << m_logic->name() << " read 0, closed! buffer len = " << m_recvBuf.readableBytes() << ", g_closecnt = " << g_closecnt;
			m_isPeerClosed = true;
			this->close();
			break;
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

	// 检测业务层是否已存在处理本连接接收数据的任务，若有则无需再次通知业务层，由之前的数据处理任务一并处理本次接收到的所有数据
	{
		lock_guard_t<> lock(m_recvBufLock);
		if (m_isWaitingRead) {
			return;
		}

		m_isWaitingRead = true;
	}

	// 由业务层进行数据接收处理
	m_logic->onRecv(this);
}

void Link::handleWrite()
{
	// LOG_INFO << m_logic->name() << " socket <" << m_sockfd << "> is writable";
	if (!isopen()) {
		return;
	}

#ifdef WIN
	// windows下select模型属于LT，需要屏蔽可写事件
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

	// 循环发送数据直到无法再发送
	while(nleft > 0) {
		nwritten = evbuffer_write(buffer, m_sockfd);
		if (nwritten > 0) {
			nleft  -= nwritten;
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
