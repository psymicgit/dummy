///<------------------------------------------------------------------------------
//< @file:   server\tool\servertool.cpp
//< @author: 洪坤安
//< @date:   2015年3月10日 14:50:47
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "servertool.h"

namespace svrtool
{
	static string g_svrName[eServerTypeMax] = {
		"UnknownServer",
		"GameServer",
		"GateServer"
	};

	string& getSvrName(ServerType svrType)
	{
		if (svrType < 0 || svrType >= eServerTypeMax) {
			svrType = eNullServer;
		}

		return g_svrName[svrType];
	}
}
