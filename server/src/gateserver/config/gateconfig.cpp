#include "gateconfig.h"

#include <tool/filetool.h>

#include <rapidjson/document.h>
using namespace rapidjson;

bool GateConfig::load(const char* jsonConfig)
{
	char *json = filetool::open(jsonConfig);
	if (NULL == json) {
		return false;
	}

	rapidjson::Document doc;

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
		// ¶ÁÈ¡ÍâÍøÍøÂçÅäÖÃ
		const rapidjson::Value& wan		= doc["wan"];
		const rapidjson::Value& wanListen = wan["listen"];

		m_wanListen.ip		= wanListen["ip"].GetString();
		m_wanListen.port	= wanListen["port"].GetInt();
		m_wanThreadNum		= wan["threads"].GetInt();
	}

	{
		// ¶ÁÈ¡ÄÚÍøÍøÂçÅäÖÃ
		const rapidjson::Value& lan				= doc["lan"];
		const rapidjson::Value& lanListens		= lan["listen"];
		const rapidjson::Value& lanConnects	= lan["listen"];

		IpPort ipport;

		// ¶ÁÈ¡listenÅäÖÃ
		for (SizeType i = 0; i < lanListens.Size(); i++) {
			const Value &listen = lanListens[i];

			ipport.ip		= listen["ip"].GetString();
			ipport.port		= listen["port"].GetInt();

			m_lanListens.push_back(ipport);
		}

		// ¶ÁÈ¡connectÅäÖÃ
		for (SizeType i = 0; i < lanConnects.Size(); i++) {
			const Value &connect = lanConnects[i];

			ipport.ip		= connect["ip"].GetString();
			ipport.port		= connect["port"].GetInt();

			m_lanConnects.push_back(ipport);
		}

		m_lanThreadNum = lan["threads"].GetInt();
	}

	{
		// ¶ÁÈ¡·þÎñÆ÷ÅäÖÃ²ÎÊý
		m_maxPlayers	= doc["max-players"].GetInt();
		m_sleep			= doc["sleep"].GetInt();
	}

	delete[] json;
	return true;
}
