
#include "croppropmgr.h"
#include "cropseedpropmgr.h"
#include "cropfieldpropmgr.h"

#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <common/client/errdef.h>
#include <logic/build/buildmgr.h>
#include <logic/player/playermgr.h>


IMPLEMENT_SINGLETON_PROPMGR(CCropPropMgr)


CCropPropMgr::CCropPropMgr()
{

}

CCropPropMgr::~CCropPropMgr()
{
    //CCropSeedPropMgr::DestroyInstance();
}

BOOL CCropPropMgr::Init()
{
    // 初始化CCropSeedPropMgr管理器
    if(FALSE == CCropSeedPropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CCropSeedPropMgr::CreateBuild failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CCropSeedPropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CCropSeedPropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    if (FALSE == CCropFieldPropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CCropFieldPropMgr::CreateBuild failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CCropFieldPropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CCropFieldPropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}


VOID CCropPropMgr::UnInit()
{
    //CCropSeedPropMgr::Instance()->UnInit();
    //CCropFieldPropMgr::Instance()->UnInit();
}