
#ifndef _CROPMGR_H_
#define _CROPMGR_H_

#include "crop.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdmutex.h>
#include <logic/base/basepropmgr.h>
#include <sdhashmap.h>

using namespace SGDP;


typedef HashMap<UINT32, CCrop*> CCropMap;
typedef CCropMap::iterator CCropMapItr;



class CCropPropMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CCropPropMgr)

	virtual EConfigModule	GetConfigModuleType() { return ECM_CROP ; }

	BOOL Init();

	VOID UnInit();
public:
    CCropPropMgr();
    ~CCropPropMgr();
};




#endif //#ifndef _ROLEMGR_H_

