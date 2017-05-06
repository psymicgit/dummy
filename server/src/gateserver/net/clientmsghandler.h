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
#include <game_to_gate.pb.h>
#include <tool/ticktool.h>
#include <tool/strtool.h>
#include <tool/servertool.h>
#include <protocol/message.h>

// 处理客户端发给网关的各类消息
class ClientMsgHandler
{
public:
	static void OnAuthReq(GateClient& client, AuthReq& req, Timestamp receiveTime)
	{
		AuthAck ack;

		if(req.authkey() == client.m_authKey) {
			ack.set_result(AUTH_OK);
		} else {
			ack.set_result(AUTH_FAIL);
		}

		ack.set_encryptkey("123213");
		client.SendMsg(ServerMsg_AutyReply, ack);
	}

	static void OnLoginReq(GateClient& client, LoginReq& req, Timestamp receiveTime)
	{
		RouteLoginRequest routeLoginRequest;
		routeLoginRequest.set_client_id(client.m_clientId);
		routeLoginRequest.set_allocated_loginreq(&req);

		GateServer::Instance().SendToGameServer(GateToGame_RouteLoginRequest, routeLoginRequest);

		routeLoginRequest.release_loginreq();
	}

	static void OnPingTest(GateClient& client, PingPong& msg, Timestamp receiveTime)
	{
		static int g_pingCount = 0;
		static Tick tick("pingpong test", 100);

		++g_pingCount;
		++client.m_pingCount;

		if (g_pingCount % 100 == 0) {
			// LOG_INFO << msgtool::getMsgString(*p);
			LOG_INFO << "pingpong count = " << g_pingCount << ", PingPong size = " << msg.ByteSize();

			if (g_pingCount % 100 == 0) {
				tick.print(100, echotool::getmsg(" size = %d", msg.ByteSize()));
				tick.startTick();
			}
		}

		client.SendMsg(ServerMsg_PongReply, msg);
	}

	static void OnSpeedTest(GateClient& client, PingPong& msg, Timestamp receiveTime)
	{
		static int g_speedTestCount = 0;
		static Tick tick("speed test");

		++g_speedTestCount;
		++client.m_speedTestCount;

		if (g_speedTestCount % 100000 == 0) {
			tick.print(100000, echotool::getmsg(" size = %d", msg.ByteSize()));
			tick.startTick();

// 			double speed = tick.endTick() / g_speedTestCount;
// 			double count = 1.0f / speed;
// 			LOG_INFO << "speedtest count = " << g_speedTestCount << ", avg cost=" << speed << ", size = " << p->ByteSize() << ", exe count per second = " << count;

			if (g_speedTestCount == (5000 * 10000)) {
				Server::instance->stop();
			}
		}

// 		if (client.m_speedTestCount % 1000 == 0) {
// 			client.send(eSpeedTest, *p);
// 			// Server::instance->stop();
// 		}
	}

	static void OnLatencyTest(GateClient& client, PingPong& msg, Timestamp receiveTime)
	{
		static int g_latencyTestCount = 0;
		static Tick tick("latency test");

		++g_latencyTestCount;
		++client.m_latencyTestCount;

		if (g_latencyTestCount % 50000 == 0) {
			// LOG_INFO << msgtool::getMsgString(*p);
			LOG_INFO << "latencytest count = " << g_latencyTestCount << ", latencytest size = " << msg.ByteSize();

			if (g_latencyTestCount % 100000 == 0) {
				tick.print(100000, echotool::getmsg(" size = %d", msg.ByteSize()));
				tick.startTick();

				if (g_latencyTestCount % 1000000 == 0) {
					// client.send(eLatencyTest, *p);
					Server::instance->stop();
				}
			}
		}

// 		if (client.m_latencyTestCount % 1000 == 0) {
// 			double diff = ticktool::tickDiff(p->time());
// 			LOG_WARN << "avg latency = " << diff;
// 		}
	}
};


#endif //_clientmsgmgr_h_