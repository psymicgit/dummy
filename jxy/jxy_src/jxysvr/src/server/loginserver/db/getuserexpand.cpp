
#include "framework/lsapplication.h"
#include "getuserexpand.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <net/cli/usermgr.h>
#include "sms/basesmscmd.h"
#include "sms/smsmgr.h"



CGetUserExpandCMD::CGetUserExpandCMD()
{
    memset(&m_stAck, 0x00, sizeof(m_stAck));
}


CGetUserExpandCMD::~CGetUserExpandCMD()
{

}

VOID CGetUserExpandCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    PKT_CLILS_GET_USER_EXPAND_REQ* pstReq = (PKT_CLILS_GET_USER_EXPAND_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;
    GetUserExpand(poDBConn, pstReq);
}

VOID CGetUserExpandCMD::OnExecuted()
{
    ResponseMsg(CLILS_GET_USER_EXPAND_ACK, (CHAR*)(&m_stAck));
}


BOOL CGetUserExpandCMD::GetUserExpand(SGDP::ISDDBConnection* poDBConn, PKT_CLILS_GET_USER_EXPAND_REQ* pstReq)
{
    m_stAck.wErrCode =  ERR_GET_USER_EXPAND::ID_SUCCESS;
    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    sprintf(szSQL, "select * from mobile where UserID = %u", pstReq->dwUserID);

    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);


    ISDDBRecordSet* pRecordSet = NULL;
    INT32	nRet	  = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        UINT32 dwLen = pRecordSet->GetFieldLengthByName("Mobile");
        dwLen = dwLen >= MAX_EXPAND_DATA_LEN ? MAX_EXPAND_DATA_LEN - 1 : dwLen;
        m_stAck.stExpandFuncParaLst.astExpandFuncParaInfo[0].byExpandType = EXPAND_MOBILE;
        m_stAck.stExpandFuncParaLst.astExpandFuncParaInfo[0].byExpandLen = (UINT8)dwLen;
        SDStrncpy((CHAR*)m_stAck.stExpandFuncParaLst.astExpandFuncParaInfo[0].abyExpandData, pRecordSet->GetFieldValueByName("Mobile"), dwLen);
        m_stAck.stExpandFuncParaLst.byExpandNum++;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}
