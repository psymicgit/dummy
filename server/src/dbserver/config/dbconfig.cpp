#include "dbconfig.h"

#include <tool/filetool.h>

#include <rapidjson/document.h>
using namespace rapidjson;

namespace inner
{
	bool parsedb(const Value &db, DBAccount &accnt)
	{
		accnt.m_dbName = db["name"].GetString();
		accnt.m_dbIp = db["ip"].GetString();
		accnt.m_dbPort = db["port"].GetInt();
		accnt.m_unixSocket = db["socket"].GetString();
		accnt.m_loginName = db["usr"].GetString();
		accnt.m_loginPwd = db["pwd"].GetString();
		accnt.m_poolSize = db["conn-pool-size"].GetInt();

		return true;
	}
}


bool DBConfig::load(const char* jsonConfig)
{
	char *json = filetool::get_whole_file_buf(jsonConfig);
	if (NULL == json) {
		return false;
	}

	Document doc;

	if (doc.ParseInsitu(json).HasParseError()) {
		LOG_ERROR << "parse config<" << jsonConfig << "> failed, error code = " << doc.GetParseError() << ", error offset = " << doc.GetErrorOffset()
		          << ", err text = " << json[doc.GetErrorOffset()];

		delete[] json;
		return false;
	}

	if (!doc.IsObject()) {
		LOG_ERROR;
		return false;
	}

	{
		// 读取内网网络配置
		const Value& lan = doc["lan"];
		const Value& lanListens = lan["listen"];

		IpPort ipport;
		for (SizeType i = 0; i < lanListens.Size(); i++) {
			const Value &listen = lanListens[i];

			ipport.ip = listen["ip"].GetString();
			ipport.port = listen["port"].GetInt();

			m_lanListens.push_back(ipport);
		}

		m_lanThreadNum = lan["threads"].GetInt();
	}

	{
		// 读取游戏数据库连接配置
		const Value& gamedb = doc["gamedb"];
		inner::parsedb(gamedb, m_gamedbAccnt);
	}

	{
		// 读取日志库连接配置
		const Value& logdb = doc["logdb"];
		inner::parsedb(logdb, m_logdbAccnt);
	}

	delete[] json;
	return true;
}