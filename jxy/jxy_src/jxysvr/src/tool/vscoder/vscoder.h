#pragma once
#include "resource.h"

class CCoderApp : public CWinApp
{
public:
    CCoderApp();
public:
    virtual BOOL InitInstance();
    DECLARE_MESSAGE_MAP()
private:
    void RunOnConsole();
};


