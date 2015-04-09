#pragma once
#include "resource.h"		

class CCoderApp : public CWinApp
{
public:
	CCoderApp();
	public:
	virtual SDBOOL InitInstance();
	DECLARE_MESSAGE_MAP()
private:
    void RunOnConsole();
};


