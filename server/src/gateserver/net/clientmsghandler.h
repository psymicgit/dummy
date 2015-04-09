///<------------------------------------------------------------------------------
//< @file:   gateserver\net\clientmsgmgr.h
//< @author: 洪坤安
//< @date:   2015年3月14日 20:14:39
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _clientmsgmgr_h_
#define _clientmsgmgr_h_

#include <protocol.pb.h>
#include <client.pb.h>

#include "net/msghandler.h"
#include "tool/servertool.h"
#include "protocol/message.h"

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
		registerMsg(eLoginReq, OnLoginReq);
	}

private:
	static void OnLoginReq(Client* client, LoginReq *req, Timestamp receiveTime)
	{
		// LOG_INFO << "OnLoginReq :" << msgtool::getMsgString(*req);
		static int loginCnt = 0;

		loginCnt++;

		if (loginCnt % 10 == 0) {
			LOG_INFO << "loginCnt = " << loginCnt;
		}

		if (loginCnt == 100000) {
			Server::instance->stop();
		}
	}
};


#endif //_clientmsgmgr_h_