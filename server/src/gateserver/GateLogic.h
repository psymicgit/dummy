//------------------------------------------------------------------------------
// 作者: 洪坤安
// 日期: 2017年5月1日
// 说明: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

#include "gateserver.h"

class GateLogic : public Singleton<GateLogic>
{
public:
	template <typename T>
	static void RegisterClientMsg(int msgId, T& msg)
	{
		GateServer::Instance().m_clientDispatcher.registerMsg(msgId, msg);
	}

	template <typename T>
	static void RegisterGameMsg(int msgId, T& msg)
	{
		GateServer::Instance().m_gameDispatcher.registerMsg(msgId, msg);
	}

	template <typename T>
	static void SendToGame(int msgId, T& msg)
	{
		GateServer::Instance().SendToGameServer(msgId, msg);
	}

	static void SendToGameByClient(int clientId, int msgId, const char* msg, int msgSize);

	template <typename T>
	static void SendToGameByClient(int clientId, int msgId, T& msg)
	{
		int size = msg.ByteSize();

		std::string buff;
		buff.resize(size);
		msg.SerializeToArray((void*)buff.c_str(), size);

		SendToGameByClient(clientId, msgId, buff.c_str(), size);
	}
};