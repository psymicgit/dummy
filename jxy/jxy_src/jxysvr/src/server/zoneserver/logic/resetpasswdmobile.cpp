

#include "resetpasswdmobile.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <net/gm/rcclient.h>
#include <framework/zsapplication.h>
#include <framework/usermgr.h>


CResetPasswdMobileCMD::CResetPasswdMobileCMD()
{
	m_dwUserID = 0;
	memset(m_aszUserName, 0x00, sizeof(m_aszUserName));
}


CResetPasswdMobileCMD::~CResetPasswdMobileCMD()
{

}

VOID CResetPasswdMobileCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    PKT_CLIZS_RESET_PASSWD_MOBILE_REQ* pstReq = (PKT_CLIZS_RESET_PASSWD_MOBILE_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;
    CheckMobile(poDBConn, pstReq);
}

VOID CResetPasswdMobileCMD::OnExecuted()
{
	if (FALSE == m_bSuccess)
	{
		ResponseMsg(CLIZS_RESET_PASSWD_MOBILE_ACK, (CHAR*)(&m_stAck));
		return;
	}
	if (ERR_RESET_PASSWD_MOBILE::ID_SUCCESS != m_stAck.wErrCode)
	{	
		ResponseMsg(CLIZS_RESET_PASSWD_MOBILE_ACK, (CHAR*)(&m_stAck));
		return;
	}
	//CBaseSmsCMD*	poBaseSmsCMD = CSmsMgr::Instance()->CreateCmd(ESMS_RESETPASSWD);
	//if (NULL == poBaseSmsCMD)
	//{
	//	SET_OTHER_ERR(m_stAck.wErrCode);
	//	ResponseMsg(CLIZS_RESET_PASSWD_MOBILE_ACK, (CHAR*)(&m_stAck));
	//	return;
	//}
	//poBaseSmsCMD->SetUserData(m_pUserData, sizeof(PKT_CLIZS_RESET_PASSWD_MOBILE_REQ), m_dwTransID, m_dwUserID, m_aszUserName);
	//CSmsMgr::Instance()->AddCommand(poBaseSmsCMD);
}


BOOL CResetPasswdMobileCMD::CheckMobile(SGDP::ISDDBConnection* poDBConn, PKT_CLIZS_RESET_PASSWD_MOBILE_REQ* pstReq)
{
	m_bSuccess = TRUE;
    //检查参数
    if(HaveSqlToken((CHAR*)pstReq->abyMObile))
    {
		m_bSuccess = FALSE;
		m_stAck.wErrCode = ERR_RESET_PASSWD_MOBILE::ID_MOBILECHF;
        return FALSE;
    }

	//检查参数
	if(!IsMobile((CHAR*)pstReq->abyMObile))
	{
		m_bSuccess = FALSE;
		m_stAck.wErrCode = ERR_RESET_PASSWD_MOBILE::ID_MOBILECHF;
		return FALSE;
	}

	//if (CSmsUserMgr::Instance()->CkUser((CHAR*)pstReq->abyMObile, m_stAck.abyToken, m_stAck.wCountdown))
	//{
	//	m_bSuccess = FALSE;
	//	m_stAck.wErrCode = ERR_RESET_PASSWD_MOBILE::ID_SUCCESS;
	//}

    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    sprintf(szSQL, "select mobile.UserID, account.UserName from mobile LEFT JOIN account on mobile.UserID = account.UserID where mobile.Mobile = '%s'", (CHAR*)pstReq->abyMObile);

    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);

    UINT32	dwErr	=	0;
    string	strError;
    INT32	nRet		=	0;
    ISDDBRecordSet* pRecordSet = NULL;
    nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
		m_dwUserID = SDAtou(pRecordSet->GetFieldValueByName("UserID"));
		UINT32 dwLen = pRecordSet->GetFieldLengthByName("UserName");
		dwLen = dwLen >= USERNAME_LEN ? USERNAME_LEN - 1 :dwLen;
		SDStrncpy(m_aszUserName, pRecordSet->GetFieldValueByName("UserName"), dwLen);
    }
    else
    {
        m_stAck.wErrCode = ERR_RESET_PASSWD_MOBILE::ID_HAVE_NOT_BIND;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}
