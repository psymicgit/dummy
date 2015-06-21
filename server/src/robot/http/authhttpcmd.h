///<------------------------------------------------------------------------------
//< @file:   robot\http\authhttpcmd.h
//< @author: 洪坤安
//< @date:   2015年6月12日 19:31:37
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _authhttpcmd_h_
#define _authhttpcmd_h_

#include <http/httpcmd.h>
class Robot;

class AuthHttpCmd : public HttpCmd
{
private:
	virtual bool init();

	virtual void onExecuted();

private:
	Robot *m_robot;
};

#endif //_authhttpcmd_h_