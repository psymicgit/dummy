///<------------------------------------------------------------------------------
//< @file:   robot\http\authhttpcmd.cpp
//< @author: 洪坤安
//< @date:   2015年6月12日 19:31:46
//< @brief:
//< Copyright (c) 2015 服务器. All rights reserved.
///<------------------------------------------------------------------------------

#include "authhttpcmd.h"
#include "robot.h"

#include <tool/echotool.h>

bool AuthHttpCmd::init()
{
	m_url = "http://127.0.0.1/auth.php";

	std::string encryptPwd = "encrypted_" + m_robot->m_password + "_encrypted";

	m_fields =
	    "cliver="	 + std::string(echotool::getmsg("%u", m_robot->m_cliversion)) +
	    "&deviceid=" + m_robot->m_deviceid +
	    "&username=" + m_robot->m_username +
	    "&password=" + m_robot->m_password +
	    "&ip="		 + m_robot->m_ip;

	m_url = signUrl(HTTP_URL_SIGN);
	return true;
}

void AuthHttpCmd::onExecuted()
{
	m_html = "authkey:xxx";
}
