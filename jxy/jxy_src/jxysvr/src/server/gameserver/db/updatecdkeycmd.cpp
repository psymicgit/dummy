#include "updatecdkeycmd.h"
#include <db/autosqlbuf.h>
#include "logic/record/recordmgr.h"
#include <net/gt/gtpktbuilder.h>
#include "common/server/dblock.h"
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <logic/player/playermgr.h>
#include <sdloggerimpl.h>
#include <utility.h>
#include "logic/activityencourage/activityencourage.h"

CUpdateCdKeyCmd::CUpdateCdKeyCmd()
{
    m_bSuccess = FALSE;
}

CUpdateCdKeyCmd::~CUpdateCdKeyCmd()
{

}


VOID SDAPI CUpdateCdKeyCmd::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    CUpdateCdKey( poDBConn );
}

/**
* @brief 同步执行SQL命令相关的逻辑部分
* @return VOID
*/
VOID SDAPI CUpdateCdKeyCmd::OnExecuted()
{
    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(m_dwTransID);
    if(NULL == poPlayer)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: player[%u] is not found!"), MSG_MARK, m_dwTransID);
        return;
    }

    UINT16 wMsgID = CGTPktBuilder::Instance()->OnRecvActivityEncourageAck(poPlayer, m_stData.qwActivityID, m_stData.aszSeriesNo, m_wErrCode);
    if(INVALID_MSGID == wMsgID)
    {
        return;
    }
    poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
}

//设置数据
VOID CUpdateCdKeyCmd::SetUserData(VOID* pUserData)
{
    memcpy(&m_stData, pUserData, sizeof(m_stData));
}

BOOL CUpdateCdKeyCmd::CUpdateCdKey(SGDP::ISDDBConnection* poDBConn)
{
    m_wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_CD_KEY_NOT_EXIT;

    //检查参数
    if(HaveSqlToken(m_stData.aszSeriesNo))
    {
        return FALSE;
    }

    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    sprintf(szSQL, "call UpdateCdKey(%llu, '%s', %u, @ErrCode, @ErrMsg);", m_stData.qwActivityID, m_stData.aszSeriesNo, m_dwTransID);

    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);

    UINT32	dwErr	=	0;
    string	strError;
    INT32	nRet		=	poDBConn->ExecuteSql(szSQL, NULL,  NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
        SET_OTHER_ERR(m_wErrCode);
        return FALSE;
    }
    else
    {
        sprintf(szSQL, "select @ErrCode, @ErrMsg;");
    }

    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);

    ISDDBRecordSet* pRecordSet = NULL;
    nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        UINT32 wErrCode = SDAtou(pRecordSet->GetFieldValueByName("@ErrCode"));
        SDStrcpy(szErrMsg, pRecordSet->GetFieldValueByName("@ErrMsg"));
        switch(wErrCode)
        {
        case 0:
            m_wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_SUCCESS;
            break;
        case 1:
            m_wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_CD_KEY_NOT_EXIT;
            break;
        case 2:
            m_wErrCode = ERR_RECV_ACTIVITY_ENCOURRAGE::ID_CD_KEY_IS_USERD;
            break;
        default:
            SET_OTHER_ERR(m_wErrCode);
            break;
        }
    }
    else
    {
        SET_OTHER_ERR(m_wErrCode);
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}