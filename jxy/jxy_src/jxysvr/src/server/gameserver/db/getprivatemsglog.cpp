#include "getprivatemsglog.h"
#include <db/autosqlbuf.h>
#include "logic/record/recordmgr.h"
#include <net/gt/gtpktbuilder.h>
#include "common/server/dblock.h"
#include "logic/friendsys/privatemsgmgr.h"

#include <net/db/dbprocessor.h>

extern const int MAX_PRIVATE_MSG_CACHE_NUM = 50;


CGetPrivateMsgLogCmd::CGetPrivateMsgLogCmd()
{
    m_wErrCode = SUCCESS;
    m_bSuccess = FALSE;
}

CGetPrivateMsgLogCmd::~CGetPrivateMsgLogCmd()
{

}


VOID SDAPI CGetPrivateMsgLogCmd::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    char v_szSql[1024] = {0};
    sprintf(v_szSql, " select sendPid, recvPid, msgid, msgContent, sendTime, isRead from private_chat_log  where (sendPid= %u  and recvPid= %u) or (sendPid=%u  and recvPid=%u) order by sendTime desc limit %u",
            m_GET_PRIVATEMSGLOG.dwSenderID, m_GET_PRIVATEMSGLOG.dwRecverID, m_GET_PRIVATEMSGLOG.dwRecverID, m_GET_PRIVATEMSGLOG.dwSenderID, MAX_PRIVATE_MSG_CACHE_NUM);
    ISDDBRecordSet* pRecordSet = NULL;
    UINT32	dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(v_szSql, &pRecordSet, 0, &strError );
    if (nRet < 0)
    {
        m_wErrCode = FAIL;
        return;
    }

    if(SGDP::SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
            SPrivateMsg stPrivateMsg;
            stPrivateMsg.dwSenderID	= SGDP::SDAtou(pRecordSet->GetFieldValueByName("sendPid"));
            stPrivateMsg.dwRecverID	= SGDP::SDAtou(pRecordSet->GetFieldValueByName("recvPid"));
            stPrivateMsg.dwMsgIdx		= SGDP::SDAtou(pRecordSet->GetFieldValueByName("msgid"));
            UINT32 dwLen = pRecordSet->GetFieldLengthByName("msgContent");
            dwLen = dwLen > (PRIVATE_MSG_CONTENT_LEN - 1) ? (PRIVATE_MSG_CONTENT_LEN - 1) : dwLen;
            memcpy(stPrivateMsg.aszMsgContent, pRecordSet->GetFieldValueByName("msgContent"), dwLen);

            char szTemp[128] = {0};
            memset(szTemp, 0x00, sizeof(szTemp));
            dwLen = pRecordSet->GetFieldLengthByName("sendTime");
            dwLen = dwLen > sizeof(szTemp) ? sizeof(szTemp) : dwLen;
            memcpy(szTemp, pRecordSet->GetFieldValueByName("sendTime"), dwLen);
            SGDP::CSDDateTime oLoginTime;
            SGDP::SDTimeFromString(szTemp, oLoginTime);
            stPrivateMsg.qwSendTime = oLoginTime.GetTimeValue();
            stPrivateMsg.byIsRead = SDAtou(pRecordSet->GetFieldValueByName("isRead"));
            m_listPrivateMsg.qwLastTime = SDTimeSecs();
            m_listPrivateMsg.listPrivateMsg.push_front(stPrivateMsg);

            if(m_listPrivateMsg.dwMaxMsgIdx < stPrivateMsg.dwMsgIdx)
            {
                m_listPrivateMsg.dwMaxMsgIdx = stPrivateMsg.dwMsgIdx;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    return;
}

/**
* @brief 同步执行SQL命令相关的逻辑部分
* @return VOID
*/
VOID SDAPI CGetPrivateMsgLogCmd::OnExecuted()
{
    CDBProcessor::OnGetPrivateMsg(m_GET_PRIVATEMSGLOG, m_listPrivateMsg, m_wErrCode);
}

//设置数据
VOID CGetPrivateMsgLogCmd::SetUserData(VOID* pUserData)
{
    m_GET_PRIVATEMSGLOG = *((GET_PRIVATEMSGLOG*)pUserData);
}
