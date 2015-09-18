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
		, m_robotsPerSecond(0)
		, m_robotsInterval(0)
		, m_speedTestPacketSize(0)
		, m_speedTestPacketNum(0)
	{}

	bool load(const char* configFile);

	int m_wanThreadNum;
	int m_robotsPerSecond;
	int m_robotsInterval;
	int m_speedTestPacketSize;
	int m_speedTestPacketNum;

	// 网关服务器地址
	IpPort m_gateserver;
};

#endif
