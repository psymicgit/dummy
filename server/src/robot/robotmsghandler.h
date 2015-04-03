///<------------------------------------------------------------------------------
//< @file:   src\robot\robotmsghandler.h
//< @author: 洪坤安
//< @date:   2015年3月18日 0:48:0
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _robotmsghandler_h_
#define _robotmsghandler_h_

#include <protocol.pb.h>
#include <client.pb.h>

#include "net/msghandler.h"
#include "tool/servertool.h"
#include "protocol/message.h"

#include "robot.h"

class RobotMsgHandler : public MsgHandler<Robot>
{
public:
	RobotMsgHandler(MsgDispatcher<Robot> *msgDispacher)
		: MsgHandler<Robot>(msgDispacher)
	{
		init();
	}

	void init()
	{
		registerMsg(eEncryptKeyNtf, OnEncryptKeyNtf);
	}

private:
	static void OnEncryptKeyNtf(Robot* robot, EncryptKeyNtf *ntf, Timestamp receiveTime)
	{
		static int count = 0;

		count++;

		if (count % 100 == 0) {
			LOG_INFO << "OnEncryptKeyNtf count = " << count;
		}

		// LOG_INFO << "OnEncryptKeyNtf :" << msgtool::getMsgString(*ntf);

		memcpy(robot->m_encryptKey, ntf->encryptkey().data(), sizeof(robot->m_encryptKey));
		robot->m_isEncrypt = true;

		LoginReq req;
		req.set_clientversion(100);
		req.set_deviceid("1273ab23c3390fe840a9e0");
		req.set_notifyid("notifyid-00134678");
		req.set_username("psy_micgit");
		req.set_zoneid(1);
		req.set_userid(100000001);
		req.set_playerid(14560034);
		req.set_job("sword");
		req.set_logintype(LoginReq_LoginType_NewRole);
		req.set_authtype(1);
		req.set_authkey("2ab456b6b2b1b6b1bb2b");

		robot->send(eLoginReq, req);
	}
};


#endif //_robotmsghandler_h_