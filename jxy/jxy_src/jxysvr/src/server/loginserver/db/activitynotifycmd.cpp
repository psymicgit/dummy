
#include "framework/lsapplication.h"
#include "activitynotifycmd.h"
#include <sdutil.h>
#include <logic/dbconfig.h>
#include "net/gt/gtpktbuilder.h"
#include "common/client/errdef.h"


CActivityNotifyCMD::CActivityNotifyCMD()
{

}


CActivityNotifyCMD::~CActivityNotifyCMD()
{

}

VOID CActivityNotifyCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    PKT_GSLS_ACTIVITY_NOTIFY_NTF* pstNtf = (PKT_GSLS_ACTIVITY_NOTIFY_NTF*)m_pUserData;
    if(NULL == pstNtf)
        return;

    if (FALSE == ActivityNotify(poDBConn, pstNtf->dwPlayerID, pstNtf->byActivityType, pstNtf->byNotify))
    {
        m_bSuccess = FALSE;
    }
    else
    {
        m_bSuccess = TRUE;
    }
}

VOID CActivityNotifyCMD::OnExecuted()
{

}



BOOL CActivityNotifyCMD::ActivityNotify(SGDP::ISDDBConnection* poDBConn,
                                        UINT32		dwPlayerID,
                                        UINT8		byActivityType,
                                        UINT8		byNotify)
{
    CHAR szSQL[1024] = {0};
    //更新用户信息
    sprintf(szSQL, "insert into activitynotify (PlayerID, ActivityType, Notify, UpdateTime ) values (%u, %u, %u, now()) on duplicate key \
				   update activitynotify.Notify = values(activitynotify.Notify), activitynotify.UpdateTime = values(activitynotify.UpdateTime);",
            dwPlayerID, byActivityType, byNotify);
    UINT32 dwErr;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        //异常
        SYS_CRITICAL(_SDT("[%s: %d]: ExecuteSqlRs[%s] failed[%s]!"), MSG_MARK, szSQL, strError.c_str());
        return FALSE;
    }

    return TRUE;
}


