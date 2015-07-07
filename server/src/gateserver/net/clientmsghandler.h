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
		registerMsg(eLoginReq, OnLoginReq);
		registerMsg(eAuthReq, OnAuthReq);
	}

private:
	static void OnAuthReq(Client* client, AuthReq *req, Timestamp receiveTime)
	{
		AuthAck ack;

		if(req->authkey().compare(std::string((const char*)client->m_authKey))) {
			ack.set_result(AUTH_OK);
		}
		else {
			ack.set_result(AUTH_FAIL);
		}

		client->send(eAuthAck, ack);
	}

	static void OnLoginReq(Client* client, LoginReq *req, Timestamp receiveTime)
	{
		static Tick tick("task_t test");

		// LOG_INFO << "OnLoginReq :" << msgtool::getMsgString(*req);
		static int loginCnt = 0;

		loginCnt++;

		if (loginCnt % 1000 == 0) {
			LOG_INFO << "loginCnt = " << loginCnt;
		}

		if (loginCnt == 10000) {
			double speed = tick.endTick() / loginCnt;
			double count = 1.0f / speed;
			LOG_WARN << "avg cost time = " << speed << ", exe count per second = " << count;
			// Server::instance->stop();
		}
	}
};


#endif //_clientmsgmgr_h_