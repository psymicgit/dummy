///<------------------------------------------------------------------------------
//< @file:   gameserver\logic\clientmgr.h
//< @author: ������
//< @date:   2015��6��10�� 22:36:43
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _clientmgr_h_
#define _clientmgr_h_

#include "client.h"
#include <net/msgdispatcher.h>

// �ͻ��˹�����������ǰͨ�������뱾��Ϸ�����ӵ������������
class ClientMgr : public Singleton<ClientMgr>
{
private:
	typedef std::tr1::unordered_map<uint32, Client*> ClientMap;

public:
	ClientMgr();

	Client* FindClient(uint32 playerId);

	void handleMsg(Link* link);

private:
	ClientMap m_clients;

	MsgDispatcher<Client> m_dispatcher;
};


#endif //_clientmgr_h_