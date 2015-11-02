///<------------------------------------------------------------------------------
//< @file:   gameconfig.h
//< @author: 洪坤安
//< @date:   2015年11月2日
//< @brief:	 游戏服务器配置
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _gameconfig_h_
#define _gameconfig_h_

struct IpPort {
	IpPort()
		: port(0)
	{}

	std::string ip;
	int			port;
	std::string peerName;
};

class GameConfig : public Singleton<GameConfig>
{
public:
	GameConfig()
		: m_lanThreadNum(0)
		, m_saveToDBInterval(0)
		, m_sleep(0)
	{}

	bool load(const char* configFile);

	std::vector<IpPort> m_lanConnects;		// 内网主动连接列表
	int					m_lanThreadNum;		// 内网线程数

	int					m_saveToDBInterval;	// 数据保存到数据库的时间间隔，单位毫秒
	int					m_sleep;			// 每次游戏主循环中sleep的时间，单位毫秒
};

#endif // _gameconfig_h_