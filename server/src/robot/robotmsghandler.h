///<------------------------------------------------------------------------------
//< @file:   src\robot\robotmsghandler.h
//< @author: ������
//< @date:   2015��3��18�� 0:48:0
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _robotmsghandler_h_
#define _robotmsghandler_h_

#include <protocol.pb.h>
#include <client.pb.h>

#include "protocol/message.h"
#include "net/msghandler.h"
#include "tool/servertool.h"

#include "robot.h"

// ����������յ��ĸ�����Ϣ
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
		registerMsg(ServerMsg_LoginReply,			OnLoginAck);
		registerMsg(ServerMsg_EncryptKeyNotify,		OnEncryptKeyNtf);
		registerMsg(ServerMsg_AutyReply,			OnAuthAck);

		// ����
		registerMsg(ServerMsg_PongReply,			OnPongTest);
		registerMsg(ServerMsg_SpeedTestReply,		OnSpeedTestDone);
		registerMsg(ServerMsg_LatencyTestReply,		OnLatencyTestDone);
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

		robot->m_encryptKey = ntf->publickey();
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

		for(int i = 0; i < 1; i++) {
			//robot->send(eLoginReq, *req);
		}

		robot->speedTest();
	}

	// �յ���¼�𸴣������ã�
	static void OnLoginAck(Robot* robot, LoginReq *req, Timestamp receiveTime)
	{
		robot->send(ClientMsg_LoginRequest, *req);
	}

	// �յ���¼�𸴣������ã�
	static void OnAuthAck(Robot* robot, AuthAck *ack, Timestamp receiveTime)
	{
		LOG_INFO << msgtool::getMsgDebugString(*ack);
	}

	static void OnPongTest(Robot* robot, PingPong *p, Timestamp receiveTime)
	{
		robot->m_pingpongCount++;

		if (robot->m_pingpongCount % 10) {
			robot->send(ClientMsg_PingRequest, *p);
		} else {
			robot->speedTest();
		}
	}

	static void OnSpeedTestDone(Robot* robot, PingPong *p, Timestamp receiveTime)
	{
		robot->latencyTest();
	}

	static void OnLatencyTestDone(Robot* robot, PingPong *p, Timestamp receiveTime)
	{
	}
};


#endif //_robotmsghandler_h_