

#include "setaccounttype.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <net/gm/rcclient.h>
#include <framework/zsapplication.h>



CSetAccountTypeCMD::CSetAccountTypeCMD()
{
    m_wErrCode = 0;
}


CSetAccountTypeCMD::~CSetAccountTypeCMD()
{

}

VOID CSetAccountTypeCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    tSetAccountType* pstReq = (tSetAccountType*)m_pUserData;
    if(NULL == pstReq)
        return;
    SetAccountType(poDBConn, pstReq);
}

VOID CSetAccountTypeCMD::OnExecuted()
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


BOOL CSetAccountTypeCMD::SetAccountType(SGDP::ISDDBConnection* poDBConn, tSetAccountType* pstReq)
{
    UINT32	dwErr	=	0;
    string	strError;
    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    INT32	nRet = 0;
    sprintf(szSQL, "UPDATE user set UserType = %u where UserID in (select UserID from userzone where PlayerID = %u)", pstReq->byType, pstReq->dwPlayerID);
    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
    nRet		=	poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
    }
    return TRUE;
}
