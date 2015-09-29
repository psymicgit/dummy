///<------------------------------------------------------------------------------
//< @file:   src\dbserver\main.cpp
//< @author: 洪坤安
//< @date:   2015年9月29日 14:14:4
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "dbserver.h"

int main(int argc, char **argv)
{
	DBServer &svr = DBServer::Instance();
	if (!svr.init("../dbconfig.json")) {
		svr.uninit();
		return 0;
	}

	svr.start();
	return 0;
}