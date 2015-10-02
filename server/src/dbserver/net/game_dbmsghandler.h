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
#include <game_db.pb.h>
#include <client.pb.h>

#include "net/msghandler.h"
#include "tool/servertool.h"
#include "protocol/message.h"
#include <tool/ticktool.h>

#include <db/dbfactory.h>

#include "db/dbmgr.h"
#include "db/sqldbcmd.h"

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
		registerMsg(ePostSql, onPostSqlReq);
	}

private:
	static void onPostSqlReq(Link* link, PostSql *req, Timestamp receiveTime)
	{
		int n = req->sqls_size();

		DBFactory *db = NULL;
		switch (req->dbtype()) {
		case GameDB:
			db = DBMgr::instance->m_gamedb;
			break;
		case LogDB:
			db = DBMgr::instance->m_logdb;
			break;
		}

		for(int i = 0; i < n; ++i) {
			const SqlMsg &sqlmsg = req->sqls(i);

			SqlDBCmd *sqlcmd = new SqlDBCmd;
			sqlcmd->m_sql = sqlmsg.sql();

			db->addDBCommand(sqlcmd);
		}
	}
};


#endif //_clientmsgmgr_h_