///<------------------------------------------------------------------------------
//< @file:   gateconfig.h
//< @author: 洪坤安
//< @date:   2015年11月2日
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gateconfig_h_
#define _gateconfig_h_

struct IpPort {
	IpPort()
		: port(0)
	{}

	std::string ip;
	int			port;
};

struct GateConfig {
	GateConfig()
		: m_wanThreadNum(0)
		, m_lanThreadNum(0)
		, m_enableEncrypt(false)
		, m_maxPlayers(0)
		, m_sleep(0)
	{
		m_encryptKey[0] = '\0';
	}

	bool load(const char* configFile);

	IpPort				m_wanListen;		// 外网监听监听
	std::vector<IpPort> m_lanListens;		// 内网监听列表
	std::vector<IpPort> m_lanConnects;		// 内网主动连接列表

	int					m_wanThreadNum;		// 外网线程数
	int					m_lanThreadNum;		// 内网线程数

	bool				m_enableEncrypt;	// 是否开启加密
	char				m_encryptKey[16];	// 加密key

	int					m_maxPlayers;		// 服务器容纳的最大玩家数量（超过的玩家将无法登录）
	uint16				m_sleep;			// 服务器每次主循环间的sleep时间，单位毫秒（用于防止跑满cpu）
};

#endif // _gateconfig_h_