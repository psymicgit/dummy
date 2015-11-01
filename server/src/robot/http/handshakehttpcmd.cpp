///<------------------------------------------------------------------------------
//< @file:   robot\http\handshakehttpcmd.cpp
//< @author: 洪坤安
//< @date:   2015年6月12日 12:22:47
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "handshakehttpcmd.h"
#include "robot.h"
#include <tool/echotool.h>
#include <tool/timetool.h>

bool HandShakeHttpCmd::init()
{
	m_url = "http://127.0.0.1/phpinfo.php";

	m_fields =
	    "time=" + std::string(echotool::getmsg("%u", timetool::now())) +
	    "&deviceid=" + m_robot->m_deviceid +
	    "&ip=" + m_robot->m_ip;

	m_fields = "";

	// m_url = signUrl(HTTP_URL_SIGN);
	return true;
}


void HandShakeHttpCmd::onExecuted()
{
	m_robot->m_publickey = "MAGSKMNA2jNNF";
	m_robot->auth();
}
