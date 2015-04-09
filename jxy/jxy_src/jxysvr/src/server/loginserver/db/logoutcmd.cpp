
#include "framework/lsapplication.h"
#include "logoutcmd.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>



CLogoutCMD::CLogoutCMD()
{
    memset(&m_stAck, 0x00, sizeof(m_stAck));
}


CLogoutCMD::~CLogoutCMD()
{

}

VOID CLogoutCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{

    m_bSuccess = FALSE;
    PKT_CLILS_LOGOUT_REQ* pstReq = (PKT_CLILS_LOGOUT_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;
    Logout(poDBConn, pstReq);
}

VOID CLogoutCMD::OnExecuted()
{
    ResponseMsg(CLILS_LOGOUT_ACK, (CHAR*)(&m_stAck));
}


BOOL CLogoutCMD::Logout(SGDP::ISDDBConnection* poDBConn, PKT_CLILS_LOGOUT_REQ* pstReq)
{
	//¼ì²é²ÎÊý
	if(HaveSqlToken(pstReq->aszDeviceID))
	{
		m_stAck.wErrCode =  ERR_LOGOUT::ID_INVALID_CHARACTER;
		return FALSE;
	}

    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    sprintf(szSQL, "call UnBindDeviceIDbyUserID(%u, '%s', %u, @ErrCode, @ErrMsg);", pstReq->byAuthType, pstReq->aszDeviceID, pstReq->dwUserID);

	DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);

	UINT32	dwErr	=	0;
	string	strError;
    INT32	nRet		=	poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
	if(nRet < 0)
    {
        SET_OTHER_ERR(m_stAck.wErrCode);
		SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
        return FALSE;
    }
    else
    {
        sprintf(szSQL, "select @ErrCode, @ErrMsg;");
    }

	DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);

	ISDDBRecordSet* pRecordSet = NULL;
    nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        UINT32 m_wErrCode = SDAtou(pRecordSet->GetFieldValueByName("@ErrCode"));
        SDStrcpy(szErrMsg, pRecordSet->GetFieldValueByName("@ErrMsg"));
		switch(m_wErrCode)
		{
		case 0:
			m_stAck.wErrCode = ERR_LOGOUT::ID_SUCCESS;
			break;
		case 1:
			m_stAck.wErrCode = ERR_LOGOUT::ID_UNBIND_ACCOUNT;
			break;
		default:
			SET_OTHER_ERR(m_stAck.wErrCode);
			break;
		}
    }
    else
    {
        SET_OTHER_ERR(m_stAck.wErrCode);
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}
