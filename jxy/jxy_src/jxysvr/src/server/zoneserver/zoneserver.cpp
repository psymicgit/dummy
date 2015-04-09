#include "sdframework.h"
#include "framework/zsapplication.h"
#include "common/server/utility.h"

#include "cjson.h"
#include "protocol/server/protocommondata.h"
#include "common/server/ngstring.h"


#include <string>
using namespace std;

#define CONFIG_FILE_NAME      _SDT("zs_config.xml")



INT32 main(INT32 argc, TCHAR** argv) 
{ 


    TCHAR szConfig[MAX_PATH] = {0};
    _SDTsnprintf(szConfig, MAX_PATH, _SDT("%s%s"), SDGetModulePath(),CONFIG_FILE_NAME);

    CZSApplication* pApp = (CZSApplication*)SDGetApp();
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



