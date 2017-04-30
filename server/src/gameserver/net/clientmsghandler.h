///<------------------------------------------------------------------------------
//< @file:   gameserver\net\clientmsgmgr.h
//< @author: ������
//< @date:   2015��3��14�� 20:14:39
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _clientmsgmgr_h_
#define _clientmsgmgr_h_

#include <protocol.pb.h>
#include <client.pb.h>

#include "net/msghandler.h"
#include "tool/servertool.h"
#include "protocol/message.h"
#include <tool/ticktool.h>

class ClientMsgHandler : public MsgHandler<Client>
{
public:
	ClientMsgHandler(MsgDispatcher<Client> *msgDispacher)
		: MsgHandler<Client>(msgDispacher)
	{
		init();
	}

	void init()
	{
		registerMsg(ClientMsg_LoginRequest,	OnLoginReq);
		registerMsg(ClientMsg_AuthRequest,	OnAuthReq);
	}

private:
	static void OnAuthReq(Client* client, AuthReq *req, Timestamp receiveTime)
	{
	}

	static void OnLoginReq(Client* client, LoginReq *req, Timestamp receiveTime)
	{
	}
};


#endif //_clientmsgmgr_h_