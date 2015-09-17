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
	{}

	bool load(const char* configFile);

	IpPort m_wanListen;
	std::vector<IpPort> m_lanListens;
	std::vector<IpPort> m_lanConnects;

	int m_wanThreadNum;
	int m_lanThreadNum;


};

#endif