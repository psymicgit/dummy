///<------------------------------------------------------------------------------
//< @file:   src\gameserver\main.cpp
//< @author: 洪坤安
//< @date:   2015年3月2日 21:14:4
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "gameserver.h"

int main(int argc, char **argv)
{
	GameServer &svr = GameServer::Instance();
	if (!svr.init()) {
		svr.uninit();
		return 0;
	}

	svr.start();
	return 0;
}