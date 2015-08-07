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
		static int pingCount = 0;
		static Tick tick("pingpong test");

		if (++pingCount % 100 == 0) {
			// LOG_INFO << msgtool::getMsgString(*p);
			LOG_INFO << "pingpong count = " << pingCount << ", PingPong size = " << p->ByteSize();

			if (pingCount % 100 == 0) {
				double speed = tick.endTick() / pingCount;
				double count = 1.0f / speed;
				LOG_WARN << "pingpong count = " << pingCount << ", avg cost time = " << speed << ", exe count per second = " << count;
			}
		}

		client->send(ePong, *p);
	}

	static void OnSpeedTest(Client* client, PingPong *p, Timestamp receiveTime)
	{
		static int speedTestCount = 0;
		static Tick tick("speed test");

		if (++speedTestCount % 100 == 0) {
			// LOG_INFO << msgtool::getMsgString(*p);
			LOG_INFO << "speedtest count = " << speedTestCount << ", speedtest size = " << p->ByteSize();

			if (speedTestCount % 10000 == 0) {
				double speed = tick.endTick() / speedTestCount;
				double count = 1.0f / speed;
				LOG_WARN << "speedtest count = " << speedTestCount << ", avg cost time = " << speed << ", exe count per second = " << count;
			}

			if (speedTestCount % 100000 == 0) {
				client->send(eSpeedTest, *p);
				// Server::instance->stop();
			}
		}

	}

	static void OnLatencyTest(Client* client, PingPong *p, Timestamp receiveTime)
	{
		static int latencyTestCount = 0;
		static Tick tick("latency test");

		if (++latencyTestCount % 1000 == 0) {
			// LOG_INFO << msgtool::getMsgString(*p);
			LOG_INFO << "latencytest count = " << latencyTestCount << ", latencytest size = " << p->ByteSize();

			if (latencyTestCount % 10000 == 0) {
				double speed = tick.endTick() / latencyTestCount;
				double count = 1.0f / speed;
				LOG_WARN << "latencytest count = " << latencyTestCount << ", avg cost time = " << speed << ", exe count per second = " << count;

				if (latencyTestCount % 100000 == 0) {
					client->send(eLatencyTest, *p);
					Server::instance->stop();
				}
			}
		}

	}
};


#endif //_clientmsgmgr_h_