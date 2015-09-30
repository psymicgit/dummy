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

// 处理游戏服发给db服的消息
class Game_DBMsgHandler : public MsgHandler<Link>
{
public:
	Game_DBMsgHandler(MsgDispatcher<Link> *msgDispacher)
		: MsgHandler<Link>(msgDispacher)
	{
		init();
	}

	void init()
	{
		registerMsg(ePostSql, OnPostSqlReq);
	}

private:
	static void OnPostSqlReq(Link* link, AuthReq *req, Timestamp receiveTime)
	{
	}

};


#endif //_clientmsgmgr_h_