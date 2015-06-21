///<------------------------------------------------------------------------------
//< @file:   robot\http\robothttpmgr.h
//< @author: 洪坤安
//< @date:   2015年6月11日 22:50:37
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _robothttpmgr_h_
#define _robothttpmgr_h_

#include <http/httpmgr.h>

class RobotHttpMgr : public Singleton<RobotHttpMgr>, public HttpMgr
{

};

#endif //_robothttpmgr_h_