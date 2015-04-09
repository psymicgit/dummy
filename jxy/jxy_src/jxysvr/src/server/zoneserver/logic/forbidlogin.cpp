

#include "forbidlogin.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <net/gm/rcclient.h>
#include <framework/zsapplication.h>



CForbidLoginCMD::CForbidLoginCMD()
{
    m_wErrCode = 0;
}


CForbidLoginCMD::~CForbidLoginCMD()
{

}

VOID CForbidLoginCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    tForbidLogin* pstReq = (tForbidLogin*)m_pUserData;
    if(NULL == pstReq)
        return;
    ForbidLogin(poDBConn, pstReq);
}

VOID CForbidLoginCMD::OnExecuted()
{
    CRCClient* poClinet = ((CZSApplication *)SDGetApp() )->GetRCClientMgr()->FindCliSession(m_dwTransID);
    if (NULL == poClinet)
    {
        return;
    }

    CHAR szRet[128] = {0};
    if(0 == m_wErrCode)
    {
        sprintf(szRet, "{\"status\":\"success\"}");
    }
    else
    {
        sprintf(szRet, "{\"status\":\"error_%u\"}", m_wErrCode);
    }
    poClinet->Rspone(szRet);
}


BOOL CForbidLoginCMD::ForbidLogin(SGDP::ISDDBConnection* poDBConn, tForbidLogin* pstReq)
{
    //¼ì²é²ÎÊý
    if(HaveSqlToken(pstReq->aszDeviceID))
    {
        m_wErrCode =  2;
        return FALSE;
    }

    UINT32	dwErr	=	0;
    string	strError;
    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    INT32	nRet = 0;

    if ('\0' != pstReq->aszDeviceID[0])
    {
        sprintf(szSQL, "INSERT INTO forbidlogindeviceid (DeviceID, CreateTime) VALUES (%s, now()) on duplicate key update forbidlogindeviceid.CreateTime = now();", pstReq->aszDeviceID);
        DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
        nRet		=	poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
        if(nRet < 0)
        {
            SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
        }
    }

    if (0 != pstReq->dwUserID)
    {
        sprintf(szSQL, "INSERT INTO forbidloginuserid (UserID, CreateTime) VALUES (%u, now()) on duplicate key update forbidloginuserid.CreateTime = now();", pstReq->dwUserID);
        DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
        nRet		=	poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
        if(nRet < 0)
        {
            SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
        }
    }

    if (0 != pstReq->dwPlayerID)
    {
        sprintf(szSQL, "INSERT INTO forbidloginplayerid (PlayerID, CreateTime) VALUES (%u, now()) on duplicate key update forbidloginplayerid.CreateTime = now();", pstReq->dwPlayerID);
        DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
        nRet		=	poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
        if(nRet < 0)
        {
            SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
        }
    }

    return TRUE;
}
