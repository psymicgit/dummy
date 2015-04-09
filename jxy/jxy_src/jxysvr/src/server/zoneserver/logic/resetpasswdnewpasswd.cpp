

#include "resetpasswdnewpasswd.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <net/gm/rcclient.h>
#include <framework/zsapplication.h>
#include <framework/usermgr.h>



CResetPasswdNewPassCMD::CResetPasswdNewPassCMD()
{

}


CResetPasswdNewPassCMD::~CResetPasswdNewPassCMD()
{

}

VOID CResetPasswdNewPassCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    PKT_CLIZS_RESET_PASSWD_NEWPASSWD_REQ* pstReq = (PKT_CLIZS_RESET_PASSWD_NEWPASSWD_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;
    NewPasswd(poDBConn, pstReq);
}

VOID CResetPasswdNewPassCMD::OnExecuted()
{
	ResponseMsg(CLIZS_RESET_PASSWD_NEWPASSWD_ACK, (CHAR*)(&m_stAck));
}


BOOL CResetPasswdNewPassCMD::NewPasswd(SGDP::ISDDBConnection* poDBConn, PKT_CLIZS_RESET_PASSWD_NEWPASSWD_REQ* pstReq)
{
	CHAR      abyNewPassWd[USERNAME_LEN] = {0}; //ÃÜÂë
	SDStrncpy(abyNewPassWd, (CHAR*)pstReq->abyNewPassWd, pstReq->byPwdLen);
    //¼ì²é²ÎÊý
    if(HaveSqlToken((TCHAR*)abyNewPassWd))
    {
		m_stAck.wErrCode = ERR_RESET_PASSWD_NEWPASSWD_CODE::ID_INVALID_CHARACTER;
        return FALSE;
    }

	UINT32 dwUserID = 0;

    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    sprintf(szSQL, "UPDATE account set UserPwd = SHA1('%s') where UserID = %u;", (TCHAR*)abyNewPassWd, dwUserID);
    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
    UINT32	dwErr	=	0;
    string	strError;
    INT32	nRet		=	0;
    ISDDBRecordSet* pRecordSet = NULL;
    nRet = poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
		m_stAck.wErrCode = ERR_COMMON::ID_OTHER_ERR;
    }
    else
    {
        m_stAck.wErrCode = ERR_RESET_PASSWD_NEWPASSWD_CODE::ID_SUCCESS;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}
