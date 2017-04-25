///<------------------------------------------------------------------------------
//< @file:   src\gameserver\main.cpp
//< @author: ������
//< @date:   2015��3��2�� 21:14:4
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#include "gameserver.h"
#include <tool/actool.h>

int main(int argc, char **argv)
{
	actool::test();

	GameServer &svr = GameServer::Instance();
	if (!svr.init("../gameconfig.json")) {
		svr.uninit();
		return 0;
	}

	svr.start();

	return 0;
}