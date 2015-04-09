// PacketRobot.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CPacketRobotApp:
// See PacketRobot.cpp for the implementation of this class
//

class CPacketRobotApp : public CWinApp
{
public:
	CPacketRobotApp();

// Overrides
	public:
	virtual SDBOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CPacketRobotApp theApp;

