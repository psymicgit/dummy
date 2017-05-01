///<------------------------------------------------------------------------------
//< @file:   gameserver\logic\clientmgr.h
//< @author: 洪坤安
//< @date:   2015年6月10日 22:36:43
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _clientmgr_h_
#define _clientmgr_h_

#include "client.h"
#include <net/msgdispatcher.h>

// 客户端管理器，管理当前通过网关与本游戏服连接的所有玩家连接
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