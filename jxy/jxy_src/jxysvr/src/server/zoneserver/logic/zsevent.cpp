
#include "zsevent.h"
#include "framework/zsclient.h"
#include "framework/zsapplication.h"
#include <sdutil.h>
#include <logic/dbmgr.h>
#include "framework/usermgr.h"

CZSEvent::CZSEvent()
{

}

CZSEvent::~CZSEvent()
{

}

UINT32 CZSEvent::OnEvent()
{
    switch(m_byType)
    {
    case CHK_SESSIONON_TIMER:
        CheckSession();
        break;
    case CHK_SYNC_DB_TIMER:
        SyncDB();
        break;
    case CHK_USER_TIMER:
        CkUser();
        break;
    case CHK_SAVE_DB_TIMER:
        SaveDB();
        break;
    case CHK_MOBILE_TIMER:
        CkMobile();
        break;
    default:
        break;
    }

    return 0;
}

INT32  CZSEvent::OnDestroy()
{
    return 0;
}

VOID CZSEvent::CheckSession()
{
    ((CZSApplication*)SDGetApp())->GetClientMgr()->ChkSessionOnTimer();
}

VOID CZSEvent::SyncDB()
{
    //CBaseDBCMD* poCMD = ((CZSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_SYNC_ZONENOTICE);
    //if(NULL == poCMD)
    //{
    //    return;
    //}
    //((CZSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

VOID CZSEvent::CkUser()
{
    CUserMgr::Instance()->CkUserDisconnect();
}

VOID	CZSEvent::SaveDB()
{
    //定时保存日志
    ((CZSApplication*)SDGetApp())->SaveData();
}

VOID	CZSEvent::CkMobile()
{

}

