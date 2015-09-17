#include "gateconfig.h"

#include <tool/filetool.h>

#include <rapidjson/document.h>
using namespace rapidjson;

bool GateConfig::load(const char* jsonConfig)
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
		// ¶ÁÈ¡ÄÚÍøÍøÂçÅäÖÃ
		const Value& wan = doc["wan"];
		const Value& wanListen = wan["listen"];

		m_wanListen.ip = wanListen["ip"].GetString();
		m_wanListen.port = wanListen["port"].GetInt();

		m_wanThreadNum = wan["threads"].GetInt();
	}

	{
		// ¶ÁÈ¡ÍâÍøÍøÂçÅäÖÃ
		const Value& lan = doc["lan"];
		const Value& lanListens = lan["listen"];
		const Value& lanConnects = lan["listen"];

		IpPort ipport;
		for (SizeType i = 0; i < lanListens.Size(); i++) {
			const Value &listen = lanListens[i];

			ipport.ip = listen["ip"].GetString();
			ipport.port = listen["port"].GetInt();

			m_lanListens.push_back(ipport);
		}

		for (SizeType i = 0; i < lanConnects.Size(); i++) {
			const Value &connect = lanConnects[i];

			ipport.ip = connect["ip"].GetString();
			ipport.port = connect["port"].GetInt();

			m_lanConnects.push_back(ipport);
		}

		m_lanThreadNum = lan["threads"].GetInt();
	}

	delete[] json;
	return true;
}
