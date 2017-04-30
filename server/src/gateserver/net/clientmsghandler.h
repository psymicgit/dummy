///<------------------------------------------------------------------------------
//< @file:   gateserver\net\clientmsgmgr.h
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
#include <tool/strtool.h>

// ����ͻ��˷������صĸ�����Ϣ
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
		registerMsg(ClientMsg_LoginRequest,			OnLoginReq);
		registerMsg(ClientMsg_AuthRequest,			OnAuthReq);

		// ����
		registerMsg(ClientMsg_PingRequest,			OnPingTest);
		registerMsg(ClientMsg_SpeedTestRequest,		OnSpeedTest);
		registerMsg(ClientMsg_LatencyTestRequest,	OnLatencyTest);
	}

private:
	static void OnAuthReq(Client* client, AuthReq *req, Timestamp receiveTime)
	{
		AuthAck ack;

		if(req->authkey().compare(std::string((const char*)client->m_authKey))) {
			ack.set_result(AUTH_OK);
		} else {
			ack.set_result(AUTH_FAIL);
		}

		ack.set_encryptkey("123213");
		client->send(ServerMsg_AutyReply, ack);
	}

	static void OnLoginReq(Client* client, LoginReq *req, Timestamp receiveTime)
	{

	}

	static void OnPingTest(Client* client, PingPong *p, Timestamp receiveTime)
	{
		static int g_pingCount = 0;
		static Tick tick("pingpong test", 100);

		++g_pingCount;
		++client->m_pingCount;

		if (g_pingCount % 100 == 0) {
			// LOG_INFO << msgtool::getMsgString(*p);
			LOG_INFO << "pingpong count = " << g_pingCount << ", PingPong size = " << p->ByteSize();

			if (g_pingCount % 100 == 0) {
				tick.print(100, echotool::getmsg(" size = %d", p->ByteSize()));
				tick.startTick();
			}
		}

		client->send(ServerMsg_PongReply, *p);
	}

	static void OnSpeedTest(Client* client, PingPong *p, Timestamp receiveTime)
	{
		static int g_speedTestCount = 0;
		static Tick tick("speed test");

		++g_speedTestCount;
		++client->m_speedTestCount;

		if (g_speedTestCount % 100000 == 0) {
			tick.print(100000, echotool::getmsg(" size = %d", p->ByteSize()));
			tick.startTick();

// 			double speed = tick.endTick() / g_speedTestCount;
// 			double count = 1.0f / speed;
// 			LOG_INFO << "speedtest count = " << g_speedTestCount << ", avg cost=" << speed << ", size = " << p->ByteSize() << ", exe count per second = " << count;

			if (g_speedTestCount == (5000 * 10000)) {
				Server::instance->stop();
			}
		}

// 		if (client->m_speedTestCount % 1000 == 0) {
// 			client->send(eSpeedTest, *p);
// 			// Server::instance->stop();
// 		}
	}

	static void OnLatencyTest(Client* client, PingPong *p, Timestamp receiveTime)
	{
		static int g_latencyTestCount = 0;
		static Tick tick("latency test");

		++g_latencyTestCount;
		++client->m_latencyTestCount;

		if (g_latencyTestCount % 50000 == 0) {
			// LOG_INFO << msgtool::getMsgString(*p);
			LOG_INFO << "latencytest count = " << g_latencyTestCount << ", latencytest size = " << p->ByteSize();

			if (g_latencyTestCount % 100000 == 0) {
				tick.print(100000, echotool::getmsg(" size = %d", p->ByteSize()));
				tick.startTick();

				if (g_latencyTestCount % 1000000 == 0) {
					// client->send(eLatencyTest, *p);
					Server::instance->stop();
				}
			}
		}

// 		if (client->m_latencyTestCount % 1000 == 0) {
// 			double diff = ticktool::tickDiff(p->time());
// 			LOG_WARN << "avg latency = " << diff;
// 		}
	}
};


#endif //_clientmsgmgr_h_