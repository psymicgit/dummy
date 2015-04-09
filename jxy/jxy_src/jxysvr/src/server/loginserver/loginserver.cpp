#include "sdframework/sdframework.h"
#include "framework/lsapplication.h"
#include "common/server/utility.h"


#define CONFIG_FILE_NAME      _SDT("ls_config.xml")

INT32 main(INT32 argc, TCHAR** argv) 
{ 
	SetLocateCharSet();

	TCHAR szConfig[MAX_PATH] = {0};
	_SDTsnprintf(szConfig, MAX_PATH, _SDT("%s%s"), SDGetModulePath(),CONFIG_FILE_NAME); 

	CLSApplication* pApp = (CLSApplication*)SDGetApp();
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


