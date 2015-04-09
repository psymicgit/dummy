
#include "sdframework.h"
#include "framework/gsapplication.h"
#include "common/server/utility.h"
#include <json/json.h>
#include "common/server/ngstring.h"
//#include "common/server/ngsql.h"



#if (defined(WIN32) || defined(WIN64))
#include "common/server/debuginfo/breakpad.h"
#endif

#define CONFIG_FILE_NAME      _SDT("gs_config.xml")

//²âÊÔ
#ifdef offsetof
#define evutil_offsetof(type, field) offsetof(type, field)
#else
#define evutil_offsetof(type, field) ((off_t)(&((type *)0)->field))
#endif



INT32 main(INT32 argc, TCHAR** argv)
{
	//UINT32 dwSize = sizeof(DT_PLAYER_PET_DATA);
#if (defined(WIN32) || defined(WIN64))
    CBreakPad::Init();
#endif
    SetLocateCharSet();
    TCHAR szConfig[MAX_PATH] = {0};
    _SDTsnprintf(szConfig, MAX_PATH, _SDT("%s%s"), (TCHAR*)(SDGetModulePath()), CONFIG_FILE_NAME);

    CGSApplication* pApp = (CGSApplication*)SDGetApp();
    pApp->OnSetCmdLine(argc, argv);

    if (FALSE == pApp->Init(szConfig))
    {
        pApp->UnInit();
        return -1;
    }

    while(FALSE == pApp->IsTerminated())
    {
        pApp->Run();
    };
    pApp->UnInit();

#if (defined(WIN32) || defined(WIN64))
    CBreakPad::UnInit();
#endif

    return 0;
}

