///<------------------------------------------------------------------------------
//< @file:   server\net\link.h
//< @author: ������
//< @date:   2015��1��14�� 23:3:23
//< @brief:	 Link = tcp����
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _link_h_
#define _link_h_

#include "netdefine.h"
#include "netaddress.h"
#include "basic/buffer.h"
#include "basic/evbuffer-internal.h"
#include "protocol/protocol.h"

class NetAddress;
class INetLogic;
struct RingBufferBlock;

struct evbuffer;

namespace google {namespace protobuf {class Message;}}

// �������ӣ�Link = tcp���ӣ��൱��connection
class Link : public IFd
{
public:
	explicit Link(int sockfd, NetAddress &localAddr, NetAddress &peerAddr, NetModel *pNet, INetLogic *logic)
		: m_localAddr(localAddr)
		, m_peerAddr(peerAddr)
		, m_logic(logic)
		, m_isAutoReconnect(false)
		, m_closed(false)
		, m_error(false)
		, m_sockfd(sockfd)
		, m_net(pNet)
		, m_isWaitingWrite(false)
		, m_isWaitingRead(false)
		, m_isWaitingClose(false)
		, m_isPeerClosed(false)
	{
	}

	~Link();

public:
	virtual socket_t socket() const {return m_sockfd;}

	virtual void handleRead();
	virtual void handleWrite();
	virtual void handleError();
	virtual void close();
	virtual void erase();

private:
	void closing();

public:
	// ���ñ����Ӳ�������ע�ᵽ����
	void open();

	// ��������
	void send(const char *data, int len);

	// �����ı�
	void send(const char *text);

	// ����δ���ܵ���Ϣ��
	void send(int msgId, const Message &msg);

	// ���;������ܻ��������Ϣ��
	void send(int msgId, const char *data, int len);

	// ҵ��㿪ʼ��ȡ���ݣ������ջ������ڵ�����ȫ��ȡ���浽readto���ÿ�
	void beginRead(evbuffer *readto);

	// ҵ����ȡ������ϣ���ҵ���δ��������������´�����ջ�������ͷ��
	void endRead(evbuffer *remain);

	// �������Ƿ��
	inline bool isopen() const { return !m_closed; }

	// ��ȡ�����ӵı��ص�ַ
	NetAddress getLocalAddr() const;

	// ��ȡ�����ӵĶԶ˵�ַ
	NetAddress getPeerAddr() const;

private:
	// ���ͱ�������δ�����������
	void sendBuffer();

private:
	// �����ִ��,������һ���Է������ݣ�������δ���͵����ݳ���
	int trySend(evbuffer*);

	// ҵ���ִ�У��������ӵĹر�
	void onLogicClose();

	// �����ִ�У���ʼ��������
	void onSend();

public:
	const NetAddress m_localAddr;
	const NetAddress m_peerAddr;

	INetLogic*	m_logic;			// ����������Ӧ���߼�ʵ��

	bool		m_isAutoReconnect;	// �Զ�������־λ����ʾ�����ӶϿ����Ƿ���Ҫ�Զ�����

public:

	bool		m_closed;			// �Ƿ��ѹرձ�־λ����ֹ�ظ�close

	bool		m_error;			// �����־λ����ʾ�������Ƿ���������������Ҫ�ر����ӵĴ���

	bool		m_isWaitingClose;	// �Ƿ����ڵȴ��ر���

	socket_t	m_sockfd;			// �׽���

	NetModel*	m_net;				// ��������

	evbuffer	m_sendBuf;			// ���ͻ�����

	evbuffer	m_recvBuf;			// ���ջ�����

	mutex_t		m_sendBufLock;		// ���ͻ�������

	mutex_t		m_recvBufLock;		// ���ջ�������

	bool		m_isWaitingWrite;	// �Ƿ����ڵȴ�����㷢������

	bool		m_isWaitingRead;	// �Ƿ����ڵȴ�ҵ����ȡ����

	bool		m_isPeerClosed;		// �Զ��Ƿ��ѹر�
};

#endif //_link_h_