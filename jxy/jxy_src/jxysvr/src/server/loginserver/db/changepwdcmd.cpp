
#include "framework/lsapplication.h"
#include "changepwdcmd.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <net/cli/usermgr.h>



CChangePwdCMD::CChangePwdCMD()
{
    memset(&m_stAck, 0x00, sizeof(m_stAck));
}


CChangePwdCMD::~CChangePwdCMD()
{

}

VOID CChangePwdCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    PKT_GTLS_CHANGE_PWD_REQ* pstReq = (PKT_GTLS_CHANGE_PWD_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;
    ChangePwd(poDBConn, pstReq);
}

VOID CChangePwdCMD::OnExecuted()
{
    ResponseMsg(CLILS_CHANGE_PWD_ACK, (CHAR*)(&m_stAck));
}


BOOL CChangePwdCMD::ChangePwd(SGDP::ISDDBConnection* poDBConn, PKT_GTLS_CHANGE_PWD_REQ* pstReq)
{
    TCHAR szUserPwd[USERNAME_LEN] = {0};
    TCHAR szNewUserPwd[USERNAME_LEN] = {0};
    SDStrncpy(szUserPwd, (TCHAR*)pstReq->abyUserPwd, pstReq->byPwdLen);
    SDStrncpy(szNewUserPwd, (TCHAR*)pstReq->abyNewUserPwd, pstReq->byNewPwdLen);

    //检查参数
    if(HaveSqlToken(szUserPwd) || HaveSqlToken(szNewUserPwd))
    {
        m_stAck.wErrCode =  ERR_CHANGE_PWD::ID_INVALID_CHARACTER;
        return FALSE;
    }

    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    sprintf(szSQL, "call sp_ChangePwdbyUserIDEx('%s', '%s', %u, '%s', @ErrCode, @ErrMsg);", szUserPwd, szNewUserPwd, pstReq->dwUserID, pstReq->aszDeviceID);

    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);

    UINT32	dwErr	=	0;
    string	strError;
    INT32	nRet		=	poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stAck.wErrCode = ERR_COMMON::ID_OTHER_ERR;
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
			//更改内存中的密码
			CUserMgr::Instance()->ChangePwd(pstReq->dwUserID, szNewUserPwd);
            m_stAck.wErrCode = ERR_CHANGE_PWD::ID_SUCCESS;
            break;
        case 1:
            m_stAck.wErrCode = ERR_CHANGE_PWD::ID_USER_AUTH_TYPE_ERR;
            break;
        case 2:
            m_stAck.wErrCode = ERR_CHANGE_PWD::ID_USER_PWD_ERR;
            break;
        default:
            m_stAck.wErrCode = ERR_COMMON::ID_OTHER_ERR;
            break;
        }
    }
    else
    {
        m_stAck.wErrCode = ERR_COMMON::ID_OTHER_ERR;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}
