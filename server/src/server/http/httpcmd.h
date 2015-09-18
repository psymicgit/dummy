///<------------------------------------------------------------------------------
//< @file:   server\http\httpcmd.h
//< @author: 洪坤安
//< @date:   2015年6月11日 21:56:12
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _httpcmd_h_
#define _httpcmd_h_

#include "httpmgr.h"

#include <curl/curl.h>
#include <tool/timetool.h>

#define HTTP_URL_SIGN	"Bef^3rwsvn!29jcx57ng"

class HttpCmd
{
	friend class HttpMgr;

public:
	HttpCmd();
	virtual ~HttpCmd() {};

	void release() { delete this; }

public:
	virtual bool init() = 0;

	void bind();

	// virtual HttpCmdType getHttpCmdType() = 0;

protected:
	virtual void onExecuted() = 0;

	// 对url进行签名，附加上&sign=?参数
	virtual string signUrl(const char *sign);

public:
	// 在当前线程内同步执行本命令，阻塞直到返回结果
	void syncExecute();

protected:
	CURL *m_curl; // URL的信息

	Timestamp m_lastAddTime; // 上次添加URL请求时间

	char m_errorBuf[256]; //错误信息

	string		m_url;
	string		m_fields;
	string		m_html; // 返回的html页面结果

	bool m_ok; // 是否成功执行本命令
};

#endif //_httpcmd_h_