///<------------------------------------------------------------------------------
//< @file:   gameconfig.cpp
//< @author: 洪坤安
//< @date:   2015年11月2日
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "gameconfig.h"

#include <tool/filetool.h>

#include <rapidjson/document.h>
using namespace rapidjson;

bool GameConfig::load(const char* jsonConfig)
{
	char *json = filetool::open(jsonConfig);
	if (NULL == json) {
		LOG_ERROR << "not found config file <" << jsonConfig << ">";
		return false;
	}

	rapidjson::Document doc;

	if (doc.ParseInsitu(json).HasParseError()) {
		LOG_ERROR << "parse config<" << jsonConfig << "> failed, error code = " << doc.GetParseError()
		          << ", error offset = " << doc.GetErrorOffset()
		          << ", err text = " << json[doc.GetErrorOffset()];

		delete[] json;
		return false;
	}

	if (!doc.IsObject()) {
		LOG_ERROR;
		return false;
	}

	{
		// 读取外网网络配置
		const rapidjson::Value& lan				= doc["lan"];
		const rapidjson::Value& lanConnects	= lan["connect"];

		IpPort ipport;

		for (SizeType i = 0; i < lanConnects.Size(); i++) {
			const Value &connect = lanConnects[i];

			ipport.ip		= connect["ip"].GetString();
			ipport.port		= connect["port"].GetInt();
			ipport.peerName	= connect["name"].GetString();

			m_lanConnects.push_back(ipport);
		}

		m_lanThreadNum	= lan["threads"].GetInt();
	}

	{
		// 读取服务器配置参数
		m_sleep			= doc["sleep-ms-each-loop"].GetInt();
	}


	delete[] json;
	return true;
}