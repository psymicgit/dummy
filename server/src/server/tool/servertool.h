///<------------------------------------------------------------------------------
//< @file:   server\tool\servertool.h
//< @author: 洪坤安
//< @date:   2015年3月10日 14:49:2
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _servertool_h_
#define _servertool_h_

#include "protocol/protocol.h"

// 服务器类型定义
enum ServerType {
	eNullServer		= 0,
	eGameServer		= 1, // 游戏服
	eGateServer		= 2, // 网关
	eServerTypeMax
};

const static std::string g_serverHandshakeAuthKey = "*={[(8b1a4e5f8a7d7d)]}*==";

namespace svrtool
{
	// 获取服务器名称
	string& getSvrName(ServerType svrType);
}

#endif //_servertool_h_