///<------------------------------------------------------------------------------
//< @file:   robot\http\handshakehttpcmd.h
//< @author: 洪坤安
//< @date:   2015年6月12日 11:51:36
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _handshakehttpcmd_h_
#define _handshakehttpcmd_h_

#include <http/httpcmd.h>

class Robot;

// 握手请求
class HandShakeHttpCmd : public HttpCmd
{
private:
	virtual bool init();

	virtual void onExecuted();

private:
	Robot *m_robot;
};

#endif //_handshakehttpcmd_h_