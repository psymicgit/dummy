#include "sdframework.h"
#include "framework/tlcapplication.h"
#include "common/server/utility.h"

#if (defined(WIN32) || defined(WIN64))
#include "common/server/debuginfo/breakpad.h"
#endif

#define CONFIG_FILE_NAME      _SDT("tlc_config.xml")


INT32 main(INT32 argc, TCHAR** argv)
{
  #if (defined(WIN32) || defined(WIN64))
  	CBreakPad::Init();
  #endif

	SetLocateCharSet();
    TCHAR szConfig[MAX_PATH] = {0};
    _SDTsnprintf(szConfig, MAX_PATH, _SDT("%s%s"), (TCHAR*)(SDGetModulePath()),CONFIG_FILE_NAME); 

    CTLCApplication* pApp = (CTLCApplication*)SDGetApp();
    pApp->OnSetCmdLine(argc,argv); 

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



