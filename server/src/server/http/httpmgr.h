///<------------------------------------------------------------------------------
//< @file:   server\http\httpmgr.h
//< @author: 洪坤安
//< @date:   2015年6月11日 21:47:59
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _httpmgr_h_
#define _httpmgr_h_

#include <curl/curl.h>

class HttpCmd;

// 发送到用户中心的http请求类型
enum HttpCmdType {
	EHCT_APP_PAY = 1,
	EHCT_LEVEL = 2,
};

class HttpMgr
{
	typedef std::list<HttpCmd*> HttpCmdList;

public:
	bool init();

	void uninit();

	void run();

	bool addCmd(HttpCmd*);

private:
	// 一旦返回http结果时将调用此函数
	static size_t onRecvHtml(void *buffer, size_t size, size_t count, void *pointer);

private:
	HttpCmdList m_httpCmdList;

	CURL *m_currentCurl;
};

#endif //_httpmgr_h_