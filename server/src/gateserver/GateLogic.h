//------------------------------------------------------------------------------
// ����: ������
// ����: 2017��5��1��
// ˵��: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

#include "gateserver.h"

class GateLogic : public Singleton<GateLogic>
{
public:
	template <typename T>
	static void RegisterClientMsg(int msgId, T t)
	{
		GateServer::Instance().m_dispatcher.registerMsg(msgId, t);
	}
};