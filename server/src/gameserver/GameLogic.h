//------------------------------------------------------------------------------
// ����: ������
// ����: 2017��4��29��
// ˵��: �ṩ��Ϸ��ͨ�õĽӿ�
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

	// ����Ϣ���ͻ���
	static bool SendToClient(ObjectId, int msgId, int subId, const Message& msg);
};