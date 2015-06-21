///<------------------------------------------------------------------------------
//< @file:   server\http\httpcmd.cpp
//< @author: 洪坤安
//< @date:   2015年6月11日 21:56:20
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "httpcmd.h"

void HttpCmd::syncExecute()
{

}

string HttpCmd::signUrl(const char *sign)
{
	std::string md5PlainText = m_url + sign; //  md5原串

	// 1. 进行md5消息摘要

	// 2. 取md5的前16位转为十六进制串
	CHAR hex[32] = {0};

	CHAR hexDigest[256] = {0};
	for (UINT8 byIdx = 0; byIdx < 16; byIdx++) {
		sprintf(hex, "%02x", (UINT8)hexDigest[byIdx]);
		strcat(hexDigest, hex);
	}

	// 3. 在原url后附加_sign字段
	std::string signText = m_url + "?_sign=" + hexDigest;
	return signText;
}
