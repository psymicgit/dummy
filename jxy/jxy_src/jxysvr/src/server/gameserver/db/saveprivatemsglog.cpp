#include "saveprivatemsglog.h"
#include <db/autosqlbuf.h>
#include "logic/record/recordmgr.h"
#include <net/gt/gtpktbuilder.h>
#include "common/server/dblock.h"
#include "common/server/ngstring.h"

CSavePrivateMsgLogCmd::CSavePrivateMsgLogCmd()
{
    m_bSuccess = FALSE;
}

CSavePrivateMsgLogCmd::~CSavePrivateMsgLogCmd()
{

}


VOID SDAPI CSavePrivateMsgLogCmd::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    if ( 0 == m_dbPrivateMsgList.size())
    {
        return;
    }

    UINT32		dwIdx = 0;
    CNGString	strSql;
    strSql = " insert into private_chat_log (sendPid, recvPid, msgid, msgContent, sendTime, isRead) values ";
    for(CPrivateMsgListItr itr = m_dbPrivateMsgList.begin(); itr != m_dbPrivateMsgList.end(); itr++)
    {
        CSDDateTime	oDateTime(itr->qwSendTime);
        tstring		strSendTime = SDTimeToString(oDateTime);

        //对于用用户直接传递进来的数值是不可信的，数据进行转化为可信数据
        CHAR			szDesc[2 * PRIVATE_MSG_CONTENT_LEN + 100] = {0};
        UINT32		dwRet = poDBConn->EscapeString(itr->aszMsgContent, strlen(itr->aszMsgContent), szDesc, sizeof(szDesc));
        if(dwRet <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: save private chat from sender %u to recver %u failed, escapestring chat [%s] failed!"), MSG_MARK, itr->dwSenderID, itr->dwRecverID, itr->aszMsgContent);
            continue;
        }

        if (0 != dwIdx)
        {
            strSql += ",";
        }
        strSql += "(";
        strSql += itr->dwSenderID;
        strSql += ",";
        strSql += itr->dwRecverID;
        strSql += ",";
        strSql += itr->dwMsgIdx;
        strSql += ",'";
        strSql += szDesc;
        strSql += "','";
        strSql += strSendTime;
        strSql += "',";
        strSql += itr->byIsRead;
        strSql += ")";
        dwIdx++;
    }

    if(0 == dwIdx)
    {
        return;
    }

    strSql += "on duplicate key update private_chat_log.isRead = values(private_chat_log.isRead);";
    INT32 nRet = poDBConn->ExecuteSql(strSql.c_str());
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: private_chat_log save error!sql:%s"), MSG_MARK, strSql.c_str());
    }

    return;
}




/**
* @brief 同步执行SQL命令相关的逻辑部分
* @return VOID
*/
VOID SDAPI CSavePrivateMsgLogCmd::OnExecuted()
{

}

//设置数据
VOID CSavePrivateMsgLogCmd::SetUserData(VOID* pUserData)
{
    m_dbPrivateMsgList = *((CPrivateMsgList*)pUserData);
}
