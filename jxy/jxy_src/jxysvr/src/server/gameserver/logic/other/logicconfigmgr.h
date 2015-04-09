
#ifndef _LOGICCONFIGMGR_H_
#define _LOGICCONFIGMGR_H_

#include <sdsingleton.h>
#include <common/server/utility.h>
#include <logic/base/basepropmgr.h>

class CLogicConfigMgr
{
public:
	static BOOL ReloadCfg(EConfigModule eModuleKind);
protected:
	
};


#endif // 
