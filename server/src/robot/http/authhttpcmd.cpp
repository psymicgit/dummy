///<------------------------------------------------------------------------------
//< @file:   robot\http\authhttpcmd.cpp
//< @author: ������
//< @date:   2015��6��12�� 19:31:46
//< @brief:
//< Copyright (c) 2015 ������. All rights reserved.
///<------------------------------------------------------------------------------

#include "authhttpcmd.h"
#include "robot.h"

#include <tool/echotool.h>

bool AuthHttpCmd::init()
{
	m_url = "http://127.0.0.1/auth.php";

	std::string encryptPwd = "encrypted_" + m_robot->m_password + "_encrypted";

	m_fields =
	    "cliver="	 + std::string(echotool::getmsg("%u", m_robot->m_clientVersion)) +
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
