//------------------------------------------------------------------------------
// ����: ������
// ����: 2017��4��29��
// ˵��: �ṩ��Ϸ��ͨ�õĽӿ�
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#pragma once

typedef int64 ObjectId;

class GameLogic : public Singleton<GameLogic>
{
public:
	//template <typename MessageType>
	//bool ListenMessage(int msgId, int subId, );

	bool Send(ObjectId, int msgId, int subId, const Message& msg);
};