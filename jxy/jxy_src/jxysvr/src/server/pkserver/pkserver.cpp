///<------------------------------------------------------------------------------
//< @file:   pkserver.cpp
//< @author: 洪坤安
//< @date:   2014年1月2日 15:53:30
//< @brief:  
//< Copyright (c) 2014 黑火. All rights reserved.
///<------------------------------------------------------------------------------

#include "sdframework.h"
#include "framework/gsapplication.h"
#include "common/server/utility.h"
#include <json/json.h>

#include "common/server/ngstring.h"



#if (defined(WIN32) || defined(WIN64))
    #include "common/server/debuginfo/breakpad.h"
#endif

#define CONFIG_FILE_NAME      _SDT("gs_config.xml")

#ifdef offsetof
    #define evutil_offsetof(type, field) offsetof(type, field)
#else
    #define evutil_offsetof(type, field) ((off_t)(&((type *)0)->field))
#endif


//测试
void test();

INT32 main(INT32 argc, TCHAR** argv)
{
#if (defined(WIN32) || defined(WIN64))
    CBreakPad::Init();
#endif

    SetLocateCharSet();
    TCHAR szConfig[MAX_PATH] = {0};
    _SDTsnprintf(szConfig, MAX_PATH, _SDT("%s%s"), (TCHAR*)(SDGetModulePath()), CONFIG_FILE_NAME);

    CPKApplication* pPKApp = (CPKApplication*)SDGetApp();
    pPKApp->OnSetCmdLine(argc, argv);

    if (FALSE == pPKApp->Init(szConfig))
    {
        pPKApp->UnInit();
        return -1;
    }

    while(FALSE == pPKApp->IsTerminated())
    {
        pPKApp->Run();
    };
    pPKApp->UnInit();

#if (defined(WIN32) || defined(WIN64))
    CBreakPad::UnInit();
#endif

    return 0;
}
