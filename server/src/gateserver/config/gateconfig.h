#ifndef _gateconfig_h_
#define _gateconfig_h_

struct IpPort {
	IpPort()
		: port(0)
	{}

	std::string ip;
	int port;
};

struct GateConfig {
	GateConfig()
		: m_wanThreadNum(0)
		, m_lanThreadNum(0)
		, m_enableEncrypt(false)
		, m_maxPlayers(0)
		, m_sleepMsEachLoop(0)
	{
		m_encryptKey[0] = '\0';
	}

	bool load(const char* configFile);

	IpPort m_wanListen;
	std::vector<IpPort> m_lanListens;
	std::vector<IpPort> m_lanConnects;

	int m_wanThreadNum;
	int m_lanThreadNum;

	bool m_enableEncrypt;
	char m_encryptKey[16];

	int m_maxPlayers; // 服务器容纳的最大玩家数量（超过的玩家将无法登录）
	uint16 m_sleepMsEachLoop; // 服务器每次主循环间的sleep时间，单位毫秒（用于防止跑满cpu）
};

#endif