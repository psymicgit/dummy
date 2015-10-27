#include "gameconfig.h"

#include <tool/filetool.h>

#include <rapidjson/document.h>
using namespace rapidjson;

bool GameConfig::load(const char* jsonConfig)
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
		// ¶ÁÈ¡ÍâÍøÍøÂçÅäÖÃ
		const Value& lan			= doc["lan"];
		const Value& lanConnects	= lan["connect"];

		IpPort ipport;

		for (SizeType i = 0; i < lanConnects.Size(); i++) {
			const Value &connect = lanConnects[i];

			ipport.ip		= connect["ip"].GetString();
			ipport.port		= connect["port"].GetInt();
			ipport.peerName	= connect["name"].GetString();

			m_lanConnects.push_back(ipport);
		}

		m_lanThreadNum = lan["threads"].GetInt();
	}

	delete[] json;
	return true;
}
