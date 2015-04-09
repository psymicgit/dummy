
#include "framework/lsapplication.h"
#include "bindcheckvercode.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <net/cli/usermgr.h>
#include <sms/smsusermgr.h>



CBindCheckVerCodeCMD::CBindCheckVerCodeCMD()
{
    memset(&m_stAck, 0x00, sizeof(m_stAck));
}


CBindCheckVerCodeCMD::~CBindCheckVerCodeCMD()
{

}

VOID CBindCheckVerCodeCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    PKT_CLILS_BIND_CHECK_VER_CODE_REQ* pstReq = (PKT_CLILS_BIND_CHECK_VER_CODE_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;
    CheckVerCode(poDBConn, pstReq);
}

VOID CBindCheckVerCodeCMD::OnExecuted()
{
    ResponseMsg(CLILS_BIND_CHECK_VER_CODE_ACK, (CHAR*)(&m_stAck));
}


BOOL CBindCheckVerCodeCMD::CheckVerCode(SGDP::ISDDBConnection* poDBConn, PKT_CLILS_BIND_CHECK_VER_CODE_REQ* pstReq)
{
    m_stAck.wErrCode =  ERR_BIND_CHECK_VER_CODE::ID_SUCCESS;
    CHAR   abyMObile[MAX_MOBILE_LENGTH] = {0}; //用户手机号
    if(!CSmsUserMgr::Instance()->CkUser(pstReq->dwUserID, em_BIND, (CHAR*)pstReq->abyVerCode, abyMObile))
    {
        m_stAck.wErrCode =  ERR_BIND_CHECK_VER_CODE::ID_VER_CODE_FAILD;
        return FALSE;
    }
    SDStrncpy((CHAR*)m_stAck.abyMObile, abyMObile, MAX_MOBILE_LENGTH);
    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    sprintf(szSQL, "insert into mobile(UserID, Mobile, CreateTime, UpdateTime) values (%u, '%s', now(), now()) on duplicate key update mobile.Mobile = values(mobile.Mobile), mobile.UpdateTime = values(mobile.UpdateTime);", pstReq->dwUserID, (CHAR*)abyMObile);

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
