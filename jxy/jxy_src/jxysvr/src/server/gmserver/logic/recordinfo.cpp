
#include "framework/gmapplication.h"
#include "recordinfo.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include "protocol/server/protogmgs.h"
#include "protocol/server/protomsgm.h"



CRecordInfoCMD::CRecordInfoCMD()
{

}


CRecordInfoCMD::~CRecordInfoCMD()
{

}

VOID CRecordInfoCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    if (ERI_ADD == m_dwTransID)
    {

        m_bSuccess = FALSE;
        PKT_GMGS_PARM_INFO_REQ* pstReq = (PKT_GMGS_PARM_INFO_REQ*)m_pUserData;
        if(NULL == pstReq)
            return;

        CHAR		szSQL[2048] = {0};
        sprintf(szSQL, "insert into gmparminfolog (AreaID, PlayerID, Action, Param1, Param2, Param3, State, SerialNumber, CreateTime) values (%u, %u, %u, %u, %llu, %llu, 1, %u, now())",
        pstReq->dwAreaID, pstReq->dwPlayerID, pstReq->byAction, pstReq->dwParam1, pstReq->qwParam2, pstReq->qwParam3, pstReq->dwSerialNumber);

        UINT32	dwErr = 0;
        string	strErr;
        INT32	nRet		=	poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strErr);
        if(nRet < 0)
        {
            SYS_CRITICAL(_SDT("Eexe SQL Faild: Err(%u:%s), SQL:%s"), dwErr, strErr.c_str(), szSQL);
        }
    }
	else if (ERI_UPDATE == m_dwTransID)
	{
		m_bSuccess = FALSE;
		PKT_GMGS_PARM_INFO_ACK* pstReq = (PKT_GMGS_PARM_INFO_ACK*)m_pUserData;
		if(NULL == pstReq)
			return;
		CHAR		szSQL[2048] = {0};
		sprintf(szSQL, "update gmparminfolog set State = %u where  SerialNumber = %u", pstReq->wErrCode, pstReq->dwSerialNumber);

		UINT32	dwErr = 0;
		string	strErr;
		INT32	nRet		=	poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strErr);
		if(nRet < 0)
		{
			SYS_CRITICAL(_SDT("Eexe SQL Faild: Err(%u:%s), SQL:%s"), dwErr, strErr.c_str(), szSQL);
		}
	}
}

VOID CRecordInfoCMD::OnExecuted()
{
    //ResponseMsg(CLIZS_REPORT_USER_DEVICE_INFO_ACK, (CHAR*)(&m_stAck));
}