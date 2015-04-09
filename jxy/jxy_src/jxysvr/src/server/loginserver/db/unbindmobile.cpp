
#include "framework/lsapplication.h"
#include "unbindmobile.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <net/cli/usermgr.h>
#include "sms/basesmscmd.h"
#include "sms/smsmgr.h"
#include "sms/smsusermgr.h"



CUnBindMobileCMD::CUnBindMobileCMD()
{
	memset(m_szMobile, 0x00, sizeof(m_szMobile));
    memset(&m_stAck, 0x00, sizeof(m_stAck));
}


CUnBindMobileCMD::~CUnBindMobileCMD()
{

}

VOID CUnBindMobileCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    PKT_CLILS_UNBIND_MOBILE_REQ* pstReq = (PKT_CLILS_UNBIND_MOBILE_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;
    UnBindMobile(poDBConn, pstReq);
}

VOID CUnBindMobileCMD::OnExecuted()
{
	if (FALSE == m_bSuccess)
	{
		ResponseMsg(CLILS_UNBIND_MOBILE_ACK, (CHAR*)(&m_stAck));
		return;
	}
	if ( m_stAck.wErrCode != ERR_UNBIND_MOBILE::ID_SUCCESS)
	{
		ResponseMsg(CLILS_UNBIND_MOBILE_ACK, (CHAR*)(&m_stAck));
		return;
	}
	CBaseSmsCMD*	poBaseSmsCMD = CSmsMgr::Instance()->CreateCmd(ESMS_UNBINDMOBILE);
	if (NULL == poBaseSmsCMD)
	{
		SET_OTHER_ERR(m_stAck.wErrCode);
		ResponseMsg(CLILS_UNBIND_MOBILE_ACK, (CHAR*)(&m_stAck));
		return;
	}
	poBaseSmsCMD->SetUserData(m_pUserData, sizeof(PKT_CLILS_UNBIND_MOBILE_REQ), m_dwTransID, m_szMobile);
	CSmsMgr::Instance()->AddCommand(poBaseSmsCMD);
}


BOOL CUnBindMobileCMD::UnBindMobile(SGDP::ISDDBConnection* poDBConn, PKT_CLILS_UNBIND_MOBILE_REQ* pstReq)
{
	m_bSuccess = TRUE;
    m_stAck.wErrCode =  ERR_UNBIND_MOBILE::ID_SUCCESS;
    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    sprintf(szSQL, "select * from mobile where UserID = %u", pstReq->dwUserID);
    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
    ISDDBRecordSet* pRecordSet = NULL;
    INT32	nRet	  = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
		INT32 nLen = pRecordSet->GetFieldLengthByName("Mobile");
		nLen = nLen >= MAX_MOBILE_LENGTH ? MAX_MOBILE_LENGTH - 1 : nLen;
		SDStrncpy(m_szMobile, pRecordSet->GetFieldValueByName("Mobile"), nLen);
    }
	else
	{
		m_bSuccess = FALSE;
		m_stAck.wErrCode =  ERR_UNBIND_MOBILE::ID_HAVE_UNBIND_MOBILE;
	}
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

	if (ERR_UNBIND_MOBILE::ID_SUCCESS != m_stAck.wErrCode)
	{
		return FALSE;
	}

	//¼ì²é²ÎÊý
	if(CSmsUserMgr::Instance()->CkUser(pstReq->dwUserID, m_szMobile, em_UNBIND, m_stAck.wCountdown))
	{
		m_bSuccess = FALSE;
		m_stAck.wErrCode =  ERR_BIND_MOBILE::ID_SUCCESS;
		return TRUE;
	}


    return TRUE;
}
