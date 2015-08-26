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

		// 测试
		registerMsg(ePing, OnPingTest);
		registerMsg(eSpeedTest, OnSpeedTest);
		registerMsg(eLatencyTest, OnLatencyTest);
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

		ack.set_encryptkey("123213");
		client->send(eAuthAck, ack);
	}

	static void OnLoginReq(Client* client, LoginReq *req, Timestamp receiveTime)
	{

	}

	static void OnPingTest(Client* client, PingPong *p, Timestamp receiveTime)
	{
		static int g_pingCount = 0;
		static Tick tick("pingpong test");

		++g_pingCount;
		++client->m_pingCount;

		if (g_pingCount % 100 == 0) {
			// LOG_INFO << msgtool::getMsgString(*p);
			LOG_INFO << "pingpong count = " << g_pingCount << ", PingPong size = " << p->ByteSize();

			if (g_pingCount % 100 == 0) {
				double speed = tick.endTick() / g_pingCount;
				double count = 1.0f / speed;
				LOG_WARN << "pingpong count = " << g_pingCount << ", avg cost time = " << speed << ", exe count per second = " << count;
			}
		}

		client->send(ePong, *p);
	}

	static void OnSpeedTest(Client* client, PingPong *p, Timestamp receiveTime)
	{
		static int g_speedTestCount = 0;
		static Tick tick("speed test");

		++g_speedTestCount;
		++client->m_speedTestCount;

		if (g_speedTestCount % 5000 == 0) {
			// LOG_INFO << msgtool::getMsgString(*p);
			LOG_INFO << "speedtest count = " << g_speedTestCount << ", speedtest size = " << p->ByteSize();

			if (g_speedTestCount % 10000 == 0) {
				double speed = tick.endTick() / g_speedTestCount;
				double count = 1.0f / speed;
				LOG_WARN << "speedtest count = " << g_speedTestCount << ", avg cost=" << speed << ", exe count per second = " << count;
			}
		}

		if (client->m_speedTestCount % 1000 == 0) {
			client->send(eSpeedTest, *p);
			// Server::instance->stop();
		}
	}

	static void OnLatencyTest(Client* client, PingPong *p, Timestamp receiveTime)
	{
		static int g_latencyTestCount = 0;
		static Tick tick("latency test");

		++g_latencyTestCount;
		++client->m_latencyTestCount;

		if (g_latencyTestCount % 5000 == 0) {
			// LOG_INFO << msgtool::getMsgString(*p);
			LOG_INFO << "latencytest count = " << g_latencyTestCount << ", latencytest size = " << p->ByteSize();

			if (g_latencyTestCount % 10000 == 0) {
				double speed = tick.endTick() / g_latencyTestCount;
				double count = 1.0f / speed;
				LOG_WARN << "latencytest count = " << g_latencyTestCount << ", avg cost time = " << speed << ", exe count per second = " << count;

				if (g_latencyTestCount % 10000 == 0) {
					// client->send(eLatencyTest, *p);
					Server::instance->stop();
				}
			}
		}

		if (client->m_latencyTestCount % 1000 == 0) {
			double diff = ticktool::tickDiff(p->time());
			LOG_WARN << "avg latency = " << diff;
		}
	}
};


#endif //_clientmsgmgr_h_