#ifndef _gameconfig_h_
#define _gameconfig_h_

struct IpPort {
	IpPort()
		: port(0)
	{}

	std::string ip;
	int port;
	std::string peerName;
};

class GameConfig : public Singleton<GameConfig>
{
public:
	GameConfig()
		: m_lanThreadNum(0)
		, m_saveToDBInterval(0)
	{}

	bool load(const char* configFile);

	std::vector<IpPort> m_lanConnects;
	int m_lanThreadNum;

	int m_saveToDBInterval;
};

#endif