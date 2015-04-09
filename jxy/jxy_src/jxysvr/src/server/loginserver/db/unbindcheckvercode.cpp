
#include "framework/lsapplication.h"
#include "unbindcheckvercode.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <net/cli/usermgr.h>
#include <sms/smsusermgr.h>



CUnBindCheckVerCodeCMD::CUnBindCheckVerCodeCMD()
{
    memset(&m_stAck, 0x00, sizeof(m_stAck));
}


CUnBindCheckVerCodeCMD::~CUnBindCheckVerCodeCMD()
{

}

VOID CUnBindCheckVerCodeCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    PKT_CLILS_UNBIND_CHECK_VER_CODE_REQ* pstReq = (PKT_CLILS_UNBIND_CHECK_VER_CODE_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;
    CheckVerCode(poDBConn, pstReq);
}

VOID CUnBindCheckVerCodeCMD::OnExecuted()
{
    ResponseMsg(CLILS_UNBIND_CHECK_VER_CODE_ACK, (CHAR*)(&m_stAck));
}


BOOL CUnBindCheckVerCodeCMD::CheckVerCode(SGDP::ISDDBConnection* poDBConn, PKT_CLILS_UNBIND_CHECK_VER_CODE_REQ* pstReq)
{
	m_stAck.wErrCode =  ERR_UNBIND_CHECK_VER_CODE::ID_SUCCESS;
    CHAR   abyMObile[MAX_MOBILE_LENGTH] = {0}; //用户手机号
    if(!CSmsUserMgr::Instance()->CkUser(pstReq->dwUserID, em_UNBIND, (CHAR*)pstReq->abyVerCode, abyMObile))
    {
		m_stAck.wErrCode =  ERR_UNBIND_CHECK_VER_CODE::ID_VER_CODE_FAILD;
        return FALSE;
    }

    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    sprintf(szSQL, "delete from  mobile where userid = %u", pstReq->dwUserID);

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
    return TRUE;
}
