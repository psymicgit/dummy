//------------------------------------------------------------------------------
// 作者: 洪坤安
// 日期: 2017年4月29日
// 说明: 
// Copyright (c) game. All rights reserved.
//------------------------------------------------------------------------------

#include "GameLogic.h"
#include "logic/scene/AoiModule.h"

bool GameLogic::SendToClient(ObjectId objId, int msgId, int subId, const Message& msg)
{
	AoiObject* aoiObj = AoiModule::instance->FindObject(objId);
	if (nullptr == aoiObj)
	{
		return false;
	}

	GameClient* client = GameClientMgr::instance->FindClient(aoiObj->clientId);
	if (nullptr == client)
	{
		return false;
	}

	client->SendMsg(msgId, msg);
	return true;
}

bool GameLogic::SendToClientByKen(ObjectId centerId, int msgId, int subId, const Message& msg)
{
	std::vector<AoiObject*> all;
	AoiModule::instance->PickKen(centerId, all);

	for (int i = 0, n = all.size(); i < n; ++i)
	{
		AoiObject* obj = all[i];
		SendToClient(obj->objId, msgId, subId, msg);
	}

	return true;
}

bool GameLogic::SendToClientByWorld(int msgId, int subId, const Message& msg)
{
	const std::vector<AoiObject*>& all = AoiModule::instance->GetObjs();

	for (int i = 0, n = all.size(); i < n; ++i)
	{
		AoiObject* obj = all[i];
		SendToClient(obj->objId, msgId, subId, msg);
	}

	return true;
}
