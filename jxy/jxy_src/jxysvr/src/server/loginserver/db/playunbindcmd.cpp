#include "framework/lsapplication.h"
#include "playunbindcmd.h"
#include <sdutil.h>
#include <sdtime.h>
#include <logic/dbconfig.h>
#include <net/gt/gtpktbuilder.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <logic/basefunc.h>
#include <algorithm/sdstrencrypt.h>
//#include "common/third/ppclient.h"



CPlayUnBindCMD::CPlayUnBindCMD()
{
    m_wErrCode = 0;
    memset(&m_stAck, 0, sizeof(m_stAck));
}


CPlayUnBindCMD::~CPlayUnBindCMD()
{

}

VOID CPlayUnBindCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;

    PKT_CLILS_UNBIND_REQ* pstReq = (PKT_CLILS_UNBIND_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;

    if (FALSE == UnBind(poDBConn, pstReq->byAuthType, pstReq->dwPlayerID, pstReq->aszUserName))
    {
        m_bSuccess = FALSE;
    }
    else
    {
        m_bSuccess = TRUE;
    }
}

VOID CPlayUnBindCMD::OnExecuted()
{
    m_stAck.wErrCode = m_wErrCode;
    ResponseMsg(CLILS_UNBIND_ACK, (CHAR*)(&m_stAck));
}

BOOL CPlayUnBindCMD::UnBind(SGDP::ISDDBConnection* poDBConn, UINT8  byAuthType, UINT32 dwPlayerID, TCHAR* pszUserName)//wZoneID用于新增玩家
{
    CHAR szSQL[256] = {0};
    memset(&szSQL, 0, sizeof(szSQL));

    CHAR szErrMsg[128] = {0};
    sprintf(szSQL, "call UnUserBind(%d, %u, '%s', @ErrCode, @ErrMsg);'", byAuthType, dwPlayerID, pszUserName);

    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);

    UINT32	dwErr	=	0;
    string	strError;
    INT32 nRet = poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        SET_OTHER_ERR(m_wErrCode);
        SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
        return FALSE;
    }
    else
    {
        sprintf(szSQL, "select @ErrCode, @ErrMsg;");
    }

    ISDDBRecordSet* pRecordSet = NULL;
    nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        m_wErrCode = SDAtou(pRecordSet->GetFieldValueByName("@ErrCode"));
        SDStrcpy(szErrMsg, pRecordSet->GetFieldValueByName("@ErrMsg"));
        switch(m_wErrCode)
        {
        case 0:
        {
            m_wErrCode = ERR_UNBIND::ID_SUCCESS;
        }
        break;
        case 1:
        {
            m_wErrCode = ERR_UNBIND::ID_PLAYERID_NOT_EXIT;
        }
        break;
        case 2:
        {
            m_wErrCode = ERR_UNBIND::ID_AUTHTYPE_NOT_EXIT;
        }
        break;
        case 3:
        {
            m_wErrCode = ERR_UNBIND::ID_PLAYERID_NOT_BIND;
        }
        break;
        default:
        {
            SET_OTHER_ERR(m_wErrCode);
        }
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
