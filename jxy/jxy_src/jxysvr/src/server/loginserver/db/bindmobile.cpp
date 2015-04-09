
#include "framework/lsapplication.h"
#include "bindmobile.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <net/cli/usermgr.h>
#include "sms/basesmscmd.h"
#include "sms/smsmgr.h"
#include "sms/smsusermgr.h"



CBindMobileCMD::CBindMobileCMD()
{
    memset(&m_stAck, 0x00, sizeof(m_stAck));
}


CBindMobileCMD::~CBindMobileCMD()
{

}

VOID CBindMobileCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    PKT_CLILS_BIND_MOBILE_REQ* pstReq = (PKT_CLILS_BIND_MOBILE_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;
    BindMobile(poDBConn, pstReq);
}

VOID CBindMobileCMD::OnExecuted()
{
    if (FALSE == m_bSuccess)
    {
		ResponseMsg(CLILS_BIND_MOBILE_ACK, (CHAR*)(&m_stAck));
		return;
	}
    if ( m_stAck.wErrCode != ERR_BIND_MOBILE::ID_SUCCESS)
    {
        ResponseMsg(CLILS_BIND_MOBILE_ACK, (CHAR*)(&m_stAck));
        return;
    }
    CBaseSmsCMD*	poBaseSmsCMD = CSmsMgr::Instance()->CreateCmd(ESMS_BINDMOBILE);
    if (NULL == poBaseSmsCMD)
    {
        SET_OTHER_ERR(m_stAck.wErrCode);
        ResponseMsg(CLILS_BIND_MOBILE_ACK, (CHAR*)(&m_stAck));
        return;
    }
    poBaseSmsCMD->SetUserData(m_pUserData, sizeof(PKT_CLILS_BIND_MOBILE_REQ), m_dwTransID);
    CSmsMgr::Instance()->AddCommand(poBaseSmsCMD);
}


BOOL CBindMobileCMD::BindMobile(SGDP::ISDDBConnection* poDBConn, PKT_CLILS_BIND_MOBILE_REQ* pstReq)
{
    m_bSuccess = TRUE;
    m_stAck.wErrCode =  ERR_BIND_MOBILE::ID_SUCCESS;
    //检查参数
    if(HaveSqlToken((CHAR*)pstReq->abyMObile))
    {
        m_bSuccess = FALSE;
        m_stAck.wErrCode =  ERR_BIND_MOBILE::ID_MOBILECHF;
        return FALSE;
    }
    //检查参数
    if(!IsMobile((CHAR*)pstReq->abyMObile))
    {
        m_bSuccess = FALSE;
        m_stAck.wErrCode =  ERR_BIND_MOBILE::ID_MOBILECHF;
        return FALSE;
    }

    //检查参数
    if(CSmsUserMgr::Instance()->CkUser(pstReq->dwUserID, (CHAR*)pstReq->abyMObile, em_BIND, m_stAck.wCountdown))
    {
        m_bSuccess = FALSE;
        m_stAck.wErrCode =  ERR_BIND_MOBILE::ID_SUCCESS;
        return FALSE;
    }

    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    sprintf(szSQL, "select 1 from mobile where UserID = %u", pstReq->dwUserID);
    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
    ISDDBRecordSet* pRecordSet = NULL;
    INT32	nRet	  = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        m_stAck.wErrCode =  ERR_BIND_MOBILE::ID_HAVE_BIND_MOBILE;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

	if (ERR_BIND_MOBILE::ID_SUCCESS != m_stAck.wErrCode)
	{
		return FALSE;
	}

    sprintf(szSQL, "select 1 from mobile where Mobile = %s", pstReq->abyMObile);
    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
    nRet	  = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        m_stAck.wErrCode =  ERR_BIND_MOBILE::ID_MOBILE_HAVE_BIND;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}
