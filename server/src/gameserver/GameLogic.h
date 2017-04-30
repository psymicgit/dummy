//------------------------------------------------------------------------------
// 作者: 洪坤安
// 日期: 2017年4月29日
// 说明: 提供游戏服通用的接口
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