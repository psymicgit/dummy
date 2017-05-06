//------------------------------------------------------------------------------
// 作者: 洪坤安
// 日期: 2017年4月29日
// 说明: 提供游戏服通用的接口
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

#include "gameserver.h"

typedef int64 ObjectId;

class GameLogic : public Singleton<GameLogic>
{
public:
	template <typename T>
	static void RegisterClientMsg(int msgId, T t)
	{
		GameServer::Instance().m_clientDispatcher.registerMsg(msgId, t);
	}

	template <typename T>
	static void RegisterGateMsg(int msgId, T t)
	{
		GameServer::Instance().m_gateDispatcher.registerMsg(msgId, t);
	}

	// 发消息给客户端
	static bool SendToClient(ObjectId, int msgId, int subId, const Message& msg);

	// 发消息给可见到该对象的所有玩家
	static bool SendToClientByKen(ObjectId centerId, int msgId, int subId, const Message& msg);

	// 发消息给所有玩家
	static bool SendToClientByWorld(int msgId, int subId, const Message& msg);
};