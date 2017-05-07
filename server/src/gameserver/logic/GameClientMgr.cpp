///<------------------------------------------------------------------------------
//< @file:   gameserver\logic\clientmgr.cpp
//< @author: 洪坤安
//< @date:   2015年6月10日 22:36:32
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "GameClientMgr.h"
#include "GameLogic.h"
#include "logic/scene/AoiModule.h"
#include <protocol/message.h>
#include <tool/randtool.h>

bool GameClientMgr::Init()
{
	GameLogic::RegisterClientMsg(ClientMsg_LoginRequest, OnLoginRequest);
	GameLogic::RegisterClientMsg(ClientMsg_AuthRequest, OnAuthRequest);
	GameLogic::RegisterClientMsg(ClientMsg_MoveRequest, OnMoveRequest);
	GameLogic::RegisterClientMsg(ClientMsg_ReadyRequest, OnReadyRequest);

	return true;
}

GameClient* GameClientMgr::FindClient(int clientId)
{
	ClientMap::iterator itr = m_clients.find(clientId);
	if (itr == m_clients.end()) {
		return NULL;
	}

	return itr->second;
}

GameClient* GameClientMgr::AddClient(int clientId)
{
	GameClient *oldClient = FindClient(clientId);
	if (oldClient)
	{
		return oldClient;
	}

	GameClient *client = new GameClient;
	client->Init();

	client->m_clientId = clientId;
	m_clients[clientId] = client;
	return client;
}

void GameClientMgr::OnAuthRequest(GameClient& client, AuthReq& req, Timestamp receiveTime)
{
}

void GameClientMgr::OnLoginRequest(GameClient& client, LoginReq& req, Timestamp receiveTime)
{
	AoiObject *aoiObj = new AoiObject();
	aoiObj->x = randtool::rand_float_between(-10, 10);
	aoiObj->y = randtool::rand_float_between(-5, 5);
	aoiObj->height = randtool::rand_float_between(-2, 40);
	aoiObj->objId = AoiModule::instance->AllocObjId();
	aoiObj->clientId = client.m_clientId;
	client.m_aoiObjId = aoiObj->objId;

	AoiModule::instance->Add(aoiObj);

	AddObjNotify notify;
	notify.set_obj_id(aoiObj->objId);
	notify.set_x(aoiObj->x);
	notify.set_y(aoiObj->y);
	notify.set_z(aoiObj->height);

	GameLogic::SendToClientByKen(aoiObj->objId, ServerMsg_AddObj, 0, notify);
}

void GameClientMgr::OnMoveRequest(GameClient& client, MoveRequest& req, Timestamp receiveTime)
{
	AoiObject* aoiObj = AoiModule::instance->FindObject(client.m_aoiObjId);
	if (nullptr == aoiObj)
	{
		return;
	}

	MoveNotify move;
	move.set_obj_id(aoiObj->objId);
	move.set_from_x(aoiObj->x);
	move.set_from_y(aoiObj->y);
	move.set_from_z(aoiObj->height);
	move.set_to_x(req.y());
	move.set_to_y(req.y());
	move.set_to_z(req.y());

	AoiModule::instance->Move(aoiObj, req.x(), req.y());

	GameLogic::SendToClientByKen(aoiObj->objId, ServerMsg_MoveNotify, 0, move);
}

void GameClientMgr::OnReadyRequest(GameClient& client, ReadyRequest& req, Timestamp receiveTime)
{

}
