#ifndef _robotconfig_h_
#define _robotconfig_h_

struct IpPort {
	IpPort()
		: port(0)
	{}

	std::string ip;
	int port;
};

struct RobotConfig {
	RobotConfig ()
		: m_wanThreadNum(0)
		, m_robotNum(0)
		, m_robotsPerSecond(0)
	{}

	bool load(const char* configFile);

	int m_wanThreadNum;
	int m_robotNum;
	int m_robotsPerSecond;

	// 网关服务器地址
	IpPort m_gateserver;
};

#endif
