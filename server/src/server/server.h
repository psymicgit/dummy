///<------------------------------------------------------------------------------
//< @file:   server\server\server.h
//< @author: ������
//< @date:   2014��11��20�� 10:36:54
//< @brief:
//< Copyright (c) 2014 Tokit. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _server_h_
#define _server_h_

#include "protocol/protocol.h"

#include "net/net.h"
#include "net/msgdispatcher.h"

#include "basic/taskqueue.h"
#include "tool/servertool.h"

class Buffer;
class ConnectReq;
class ServerLink;

// ����������
class Server : public INetLogic
{
public:
	// ������ʵ��������ڵ���Instance()������˵Ч�ʸ�һ��
	static Server *instance;

	// <������id����Ӧ�ķ���������>map
	typedef tr1::unordered_map<int, ServerLink*> ServerLinkMap;

public:
	// ���յ��µķ���������
	virtual ServerLink* onAcceptServer(Link&, ServerType peerSvrType/*�Զ˷��������*/, int peerSvrId) = 0;

	// ��Զ˷������Ͽ�����
	virtual void onDisconnectServer(Link&, ServerType peerSvrType/*�Զ˷��������*/, int peerSvrId) = 0;

public:
	Server();

	// ��ʼ��������
	bool init();

	// ���ձ���������������Դ
	virtual bool uninit();

	// ������������
	virtual std::string name();

	// �ɹ�����������������������ԭʼtcp����
	virtual void onConnected(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	// �ɹ����յ�������������ԭʼtcp����
	virtual void onAccepted(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	// �������������Ͽ�����
	virtual void onDisconnect(Link*, const NetAddress& localAddr, const NetAddress& peerAddr);

	// ���յ�����������������������
	virtual void onRecv(Link*);

	// ��ȡ��������ѭ���ϵ��������
	virtual TaskQueue& getTaskQueue() { return m_taskQueue; }

	// ����ָ���������ѽ��յ�������
	virtual void HandleNetMsg(Link*);

	// ����������Ϣ
	void HandleServerMsg(Link*);

	// ����������
	void start();

	// �رշ����������ô˽ӿں󣬷��������ڴ����������������ݿ������߼�ҵ��֮��Źر�
	void stop();

	// ���շ�������Դ
	virtual void stopping();

	// ÿ�η�����ѭ����ִ�е�����
	virtual void run();

	// �Ƿ����˳�
	bool isquit() { return m_isquit; }

	// ��ȡ��������id
	int getServerId(ServerType, int zoneId);

	// �Ƿ����Ӧ��������������
	bool isSvrLinkExist(ServerType, int zoneId);

	// ע������������
	void registerServer(int svrId, ServerLink*);

	// ȡ��ע�������
	void unregisterServer(int svrId);

public:
	ServerType				m_svrType;		// �����������
	int						m_zoneId;		// ��Id
	Net						m_lan;			// �������ģ���������������������������ͨ��

protected:
	bool					m_isquit;		// �Ƿ����˳���־λ
	TaskQueue				m_taskQueue;	// ��ִ�е��������
	MsgDispatcher<Link>		m_dispatcher;	// ��Ϣ���ɷ���
	ServerLinkMap			m_svrLinkMap;	// �����������е�������������������
};

#endif //_server_h_