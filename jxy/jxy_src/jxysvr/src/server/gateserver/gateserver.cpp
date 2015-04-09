#include "sdframework.h"
#include "framework/gtapplication.h"
#include "common/server/utility.h"

#define CONFIG_FILE_NAME      _SDT("gt_config.xml")


INT32 main(INT32 argc, TCHAR** argv) 
{ 
	SetLocateCharSet();

	CProtoCliGT::Instance()->Init();

    TCHAR szConfig[MAX_PATH] = {0};
    _SDTsnprintf(szConfig, MAX_PATH, _SDT("%s%s"), SDGetModulePath(),CONFIG_FILE_NAME);

    CGTApplication* pApp = (CGTApplication*)SDGetApp();
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

    return 0; 
}



