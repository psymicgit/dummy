
// parser.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include <string>

// CparserApp:
// See parser.cpp for the implementation of this class
//

class CparserApp : public CWinApp
{
public:
	CparserApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
    void RunOnConsole();

	DECLARE_MESSAGE_MAP()
};

extern CparserApp theApp;