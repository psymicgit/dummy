///<------------------------------------------------------------------------------
//< @file:   src\gameserver\main.cpp
//< @author: ������
//< @date:   2015��3��2�� 21:14:4
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#include "gameserver.h"
#include "scene/AoiModule.h"
#include "logic/ComponentModule.h"
#include "basic/Var.h"

bool Init()
{
	DeclareModule(AoiModule);
	DeclareModule(GameClientMgr);

	if (!ComponentModule::instance->Init())
	{
		return false;
	}

	//AoiModule::instance->test();

	VarList::Test();
	return true;
}

int main(int argc, char **argv)
{
	GameServer &server = GameServer::Instance();
	if (!server.init("../gameconfig.json")) {
		server.uninit();
		return 0;
	}

	if (!Init())
	{
		return 0;
	}

	server.start();
	return 0;
}