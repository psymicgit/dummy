///<------------------------------------------------------------------------------
//< @file:   src\gateserver\main.cpp
//< @author: 洪坤安
//< @date:   2015年3月9日 16:9:38
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "gateserver.h"

int main(int argc, char **argv)
{
	GateServer &svr = GateServer::Instance();
	if (!svr.init()) {
		svr.uninit();
		return 0;
	}

	svr.start();
	return 0;
}