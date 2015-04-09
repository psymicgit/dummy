#include "framework/lsapplication.h"
#include "playbindcmd.h"
#include <sdutil.h>
#include <sdtime.h>
#include <logic/dbconfig.h>
#include <net/gt/gtpktbuilder.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <logic/basefunc.h>
#include <algorithm/sdstrencrypt.h>
//#include <common/third/ppclient.h>



CPlayBindCMD::CPlayBindCMD()
{
    m_wErrCode = 0;

    memset(&m_stAck, 0, sizeof(m_stAck));
}


CPlayBindCMD::~CPlayBindCMD()
{
}

VOID CPlayBindCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;

    PKT_CLILS_BIND_REQ* pstReq = (PKT_CLILS_BIND_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;

    if (FALSE == Bind(poDBConn, pstReq->byAuthType, pstReq->dwPlayerID, pstReq->aszUserName, pstReq->aszNickName))
    {
        m_bSuccess = FALSE;
    }
    else
    {
        m_bSuccess = TRUE;
    }
}

VOID CPlayBindCMD::OnExecuted()
{
    m_stAck.wErrCode = m_wErrCode;
    ResponseMsg(CLILS_BIND_ACK, (CHAR*)(&m_stAck));
}

BOOL CPlayBindCMD::Bind(SGDP::ISDDBConnection* poDBConn, UINT8  byAuthType, UINT32 dwPlayerID, TCHAR* pszUserName, TCHAR* pszNickName)//wZoneID用于新增玩家
{
  //  if (AUTH_TYPE_PP == byAuthType )
  //  {
		//UINT64		qwUserID = 0;
		//char			szNickName[64] = {0};
  //      try
  //      {
  //          CPPClient	oPPClient;
  //          oPPClient.SetPPServer(PP_IP, PP_PORT);
  //          oPPClient.Do(pszTokenKey, szNickName, qwUserID);
  //          m_stAck.wErrCode = ERR_BIND::ID_SUCCESS;
  //      }
  //      catch (ECPP ecpp)
  //      {
  //          ecpp.GetErrorText();
  //          m_stAck.wErrCode = ERR_BIND::ID_THIRD_LOGIN_ERROR;
  //          return TRUE;
  //      }
  //      sprintf(pszUserName, "%llu", qwUserID);
  //      sprintf(pszNickName, "%s", szNickName);
  //  }

    CHAR szSQL[256] = {0};
    memset(&szSQL, 0, sizeof(szSQL));

    CHAR szErrMsg[128] = {0};
    sprintf(szSQL, "call UserBind(%d, %u, '%s','%s', @ErrCode, @ErrMsg);", byAuthType, dwPlayerID, pszUserName, pszNickName);

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
            m_wErrCode = ERR_BIND::ID_SUCCESS;
            break;
        case 1:
            m_wErrCode = ERR_BIND::ID_INVALID_PARM;
            break;
        case 2:
            m_wErrCode = ERR_BIND::ID_PLAYERID_NOT_EXIT;
            break;
        case 3:
            m_wErrCode = ERR_BIND::ID_USERID_IS_BINDED;
            break;
        case 4:
            m_wErrCode = ERR_BIND::ID_ACCOUNT_IS_BINDED;
            break;
        case 5:
            m_wErrCode = ERR_BIND::ID_AUTHTYPE_NOT_EXIT;
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
