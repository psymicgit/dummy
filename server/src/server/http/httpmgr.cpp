///<------------------------------------------------------------------------------
//< @file:   server\http\httpmgr.cpp
//< @author: 洪坤安
//< @date:   2015年6月11日 21:48:12
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "httpmgr.h"
#include "httpcmd.h"

bool HttpMgr::init()
{
	return true;
}

void HttpMgr::uninit()
{

}

void HttpMgr::run()
{
	// 用于缓存当前已执行完成的http请求
	HttpCmdList httpcmds = m_httpCmdList;

	for(HttpCmdList::iterator itr = httpcmds.begin(); itr != httpcmds.end(); ++itr) {
		HttpCmd *httpcmd = *itr;

		httpcmd->onExecuted();
		httpcmd->release();
	}
}

size_t HttpMgr::onRecvResponse(void *buffer, size_t size, size_t count, void *pointer)
{
	if( NULL == pointer ) {
		return 0;
	}

	//接收到数据
	HttpCmd *httpcmd = (HttpCmd *)(pointer);
	httpcmd->m_html.append((char *)buffer, size * count );
	return size * count;
}

bool HttpMgr::addCmd(HttpCmd*)
{
	return true;
}
