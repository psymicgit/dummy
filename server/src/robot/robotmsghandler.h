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

#include <protocol/message.h>
#include <tool/servertool.h>

#include "robot.h"

// 处理机器人收到的各类消息
class RobotMsgHandler
{
public:
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

		robot->Login();
		robot->MoveTest();
		robot->speedTest();
	}

	// 收到登录答复（测试用）
	static void OnLoginAck(Robot* robot, LoginReq *req, Timestamp receiveTime)
	{
		robot->send(ClientMsg_LoginRequest, *req);
	}

	// 收到登录答复（测试用）
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