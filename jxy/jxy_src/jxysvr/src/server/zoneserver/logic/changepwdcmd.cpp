

#include "changepwdcmd.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <net/gm/rcclient.h>
#include <framework/zsapplication.h>
#include <framework/usermgr.h>



CChangePwdCMD::CChangePwdCMD()
{
    m_dwUserID = 0;
    m_wErrCode = 0;
}


CChangePwdCMD::~CChangePwdCMD()
{

}

VOID CChangePwdCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    tChangePwd* pstReq = (tChangePwd*)m_pUserData;
    if(NULL == pstReq)
        return;
    ChangePwd(poDBConn, pstReq);
}

VOID CChangePwdCMD::OnExecuted()
{
    CRCClient* poClinet = ((CZSApplication *)SDGetApp() )->GetRCClientMgr()->FindCliSession(m_dwTransID);
    if (NULL == poClinet)
    {
        return;
    }

    CHAR szRet[128] = {0};
    if(ERR_CHANGE_PWD::ID_SUCCESS == m_wErrCode)
    {
        sprintf(szRet, "{\"status\":\"success\"}");

        //修改密码成功，修改缓存中的数据
        tChangePwd* pstReq = (tChangePwd*)m_pUserData;
        if(NULL == pstReq)
            return;

        //直接删除内存数据，强制用户认证
        CUserMgr::Instance()->ReleaseUser(m_dwUserID);
    }
    else if (ERR_CHANGE_PWD::ID_INVALID_CHARACTER == m_wErrCode)
    {
        sprintf(szRet, "{\"status\":\"PlayerID Error!\"}");
    }
    else
    {
        sprintf(szRet, "{\"status\":\"Other Error!\"}");
    }
    poClinet->Rspone(szRet);
}


BOOL CChangePwdCMD::ChangePwd(SGDP::ISDDBConnection* poDBConn, tChangePwd* pstReq)
{
    TCHAR szUserPwd[USERNAME_LEN] = {0};
    SDStrncpy(szUserPwd, (TCHAR*)pstReq->szUserPwd, USERNAME_LEN);

    //检查参数
    if(HaveSqlToken(pstReq->szUserPwd))
    {
        m_wErrCode =  2;
        return FALSE;
    }

    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    sprintf(szSQL, "call sp_ChangePwdbyPlayerIDGM(%u, '%s', %u);", pstReq->dwPlayerID, szUserPwd, pstReq->byDevice);

    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);

    UINT32	dwErr	=	0;
    string	strError;
    INT32	nRet		=	0;
    ISDDBRecordSet* pRecordSet = NULL;
    nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        m_dwUserID = SDAtou(pRecordSet->GetFieldValueByName("UserID"));
        UINT32 wErrCode = SDAtou(pRecordSet->GetFieldValueByName("ErrCode"));
        SDStrncpy(szErrMsg, pRecordSet->GetFieldValueByName("ErrStr"), 128);
        switch(wErrCode)
        {
        case 0:
            m_wErrCode = 0;
            break;
        case 1:
            m_wErrCode = 1;
            break;
        default:
            m_wErrCode = ERR_COMMON::ID_OTHER_ERR;
            break;
        }
    }
    else
    {
        m_wErrCode = ERR_COMMON::ID_OTHER_ERR;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}
