

#include "forbidregistered.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <net/gm/rcclient.h>
#include <framework/zsapplication.h>



CForbidRegisteredCMD::CForbidRegisteredCMD()
{
    m_wErrCode = 0;
}


CForbidRegisteredCMD::~CForbidRegisteredCMD()
{

}

VOID CForbidRegisteredCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    tForbidRegistered* pstReq = (tForbidRegistered*)m_pUserData;
    if(NULL == pstReq)
        return;
    ForbidRegistered(poDBConn, pstReq);
}

VOID CForbidRegisteredCMD::OnExecuted()
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


BOOL CForbidRegisteredCMD::ForbidRegistered(SGDP::ISDDBConnection* poDBConn, tForbidRegistered* pstReq)
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
		sprintf(szSQL, "INSERT INTO forbidregistereddeviceid (deviceid) VALUES ('%s');", pstReq->aszDeviceID);
		DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
		nRet		=	poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
		if(nRet < 0)
		{
			SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
		}
	}

	if (0 != pstReq->dwUserID)
	{
		sprintf(szSQL, "INSERT INTO forbidregistereduserid (userid) VALUES (%u);", pstReq->dwUserID);
		DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
		nRet		=	poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
		if(nRet < 0)
		{
			SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
		}
	}

    return TRUE;
}
