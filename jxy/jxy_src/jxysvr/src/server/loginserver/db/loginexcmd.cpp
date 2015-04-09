#include "framework/lsapplication.h"
#include "loginexcmd.h"
#include <sdutil.h>
#include <sdtime.h>
#include <logic/dbconfig.h>
#include <net/gt/gtpktbuilder.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <logic/basefunc.h>
#include <algorithm/sdstrencrypt.h>

#include <sstream>
#include <ostream>
using namespace std;



CLoginExCMD::CLoginExCMD()
{
    m_wErrCode = 0;
    memset(&m_stAck, 0, sizeof(m_stAck));
}


CLoginExCMD::~CLoginExCMD()
{
}

VOID CLoginExCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    //m_bSuccess = FALSE;

    //PKT_GTLS_LOGIN_EX_REQ* pstReq = (PKT_GTLS_LOGIN_EX_REQ*)m_pUserData;
    //if(NULL == pstReq)
    //    return;


    //if (0 == pstReq->byPwdLen)
    //{
    //    pstReq->abyUserPwd[0] = '\0';
    //}
    //_SDTStrcpy(m_stAck.aszUserName, pstReq->aszUserName);
    //_SDTStrcpy(m_stAck.aszDeviceID, pstReq->aszDeviceID);
    //_SDTStrcpy(m_stAck.aszNotifyID, pstReq->aszNotifyID);

    //m_stAck.dwPlayerID = pstReq->dwPlayerID;
    //m_stAck.wZoneID = pstReq->wZoneID;
    //m_stAck.wCareerID = pstReq->wCareerID;
    //if (pstReq->byPwdLen >= USERNAME_LEN)
    //{
    //    pstReq->abyUserPwd[USERNAME_LEN - 1] = '\0';
    //}
    //else
    //{
    //    pstReq->abyUserPwd[pstReq->byPwdLen] = '\0';
    //}


    //if(GetVersionNotice(pstReq->dwCliVersion, m_stAck.stVersionInfo, m_stAck.stNoticeInfo1, m_stAck.stNoticeInfo2))
    //{
    //    if(1 == m_stAck.stVersionInfo.byForceUpdateFlag)
    //    {
    //        m_stAck.wErrCode = ERR_LOGIN::ID_VERSION_NEED_UPDATE;
    //    }
    //    else
    //    {
    //        m_bSuccess = Auth(poDBConn,
    //                          pstReq->byAuthType,
    //                          pstReq->aszDeviceID,
    //                          pstReq->aszNotifyID,
    //                          pstReq->aszUserName,
    //                          (TCHAR*)(pstReq->abyUserPwd),
    //                          pstReq->wCareerID,
    //                          pstReq->dwPlayerID,
    //                          pstReq->wZoneID,
    //                          pstReq->byAuthMethod,
    //                          pstReq->aszIP);
    //    }
    //}
    //else
    //{
    //    SET_OTHER_ERR(m_stAck.wErrCode);
    //}
}

VOID CLoginExCMD::OnExecuted()
{
    ResponseMsg(GTLS_LOGIN_EX_ACK, (CHAR*)(&m_stAck));
}

BOOL CLoginExCMD::GetVersionNotice(UINT32 dwOldVersionID, DT_VERSION_DATA& stVersionInfo, DT_NOTICE_DATA& stNoticeInfo1, DT_NOTICE_DATA& stNoticeInfo2)
{
    SVersionNoticeInfo &stVersionNoticeInfo = ((CLSApplication*)SDGetApp())->GetDBMgr()->GetVersionNoticeInf();
    stVersionInfo = stVersionNoticeInfo.stVersionInfo;
    stVersionInfo.byForceUpdateFlag = (dwOldVersionID < stVersionNoticeInfo.dwMinUnupdateVersionID) ? 1 : 0;


    //若版本一致，不下发更新内容
    if(dwOldVersionID == stVersionInfo.dwVersionID)
    {
        stVersionInfo.aszUpdateContent[0] = 0;
        stVersionInfo.aszUpdateURL[0] = 0;
    }
    stNoticeInfo1 = stVersionNoticeInfo.stNoticeInfo1;
    stNoticeInfo2 = stVersionNoticeInfo.stNoticeInfo2;
    return TRUE;
}








BOOL CLoginExCMD::Auth(SGDP::ISDDBConnection* poDBConn,
                       UINT8  byAuthType,
                       TCHAR* pszDeviceID,
                       TCHAR* pszNotifyID,
                       TCHAR* pszUserName,
                       TCHAR* pszUserPwd,
                       UINT16 wCareerID,
                       UINT32 dwPlayerID,
                       UINT16 wZoneID,
                       UINT8  byAuthMethod,
                       TCHAR* pszIp)//wZoneID用于新增玩家
{
 //   m_stAck.byAuthType = byAuthType;
 //   //检查参数
 //   m_stAck.wErrCode = CBaseFunc::CkAuthParam(pszDeviceID, byAuthType, pszNotifyID, pszUserName, pszUserPwd, wCareerID, wZoneID);
 //   if(ERR_LOGIN::ID_SUCCESS != m_stAck.wErrCode)
 //   {
 //       return FALSE;
 //   }

 //   //加密过的密码可能包括有数据库非常的字条，要先掉才行。
 //   if (AUTH_TYPE_MAXNET != byAuthType)
 //   {
 //       pszUserPwd[0] = '\0';
 //   }

 //   if (HaveSqlToken(pszIp))
 //   {
 //       pszIp[0] = '\0';
 //   }

 //   CHAR szSQL[256] = {0};
 //   memset(&szSQL, 0, sizeof(szSQL));

 //   CHAR szErrMsg[128] = {0};
 //   sprintf(szSQL, "call sp_UserAuthEx(%d, '%s', '%s', '%s', %d, '', 0, 0);",
 //           byAuthType, pszDeviceID, pszUserName, pszUserPwd, byAuthMethod);

 //   DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);

 //   UINT32	dwUserID =	0;
 //   UINT32	dwErr	=	0;
 //   string	strError;
 //   ISDDBRecordSet* pRecordSet = NULL;
 //   INT32	nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, &dwErr, &strError);
 //   if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
 //   {
 //       dwUserID = SDAtou(pRecordSet->GetFieldValueByName("UserID"));
 //       m_wErrCode = SDAtou(pRecordSet->GetFieldValueByName("ErrCode"));
 //       SDStrcpy(szErrMsg, pRecordSet->GetFieldValueByName("ErrStr"));
 //       switch(m_wErrCode)
 //       {
 //       case 0:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_SUCCESS;
 //           break;
 //       case 1:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_PWD_ERR;
 //           break;
 //       case 2:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_INVALID_USER_TYPE;
 //           break;
 //       case 3:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_USERNAME_NOT_EXIT;
 //           break;
 //       case 4:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_FORBID_LOGIN;
 //           break;
 //       default:
 //           SYS_CRITICAL(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
 //           SET_OTHER_ERR(m_stAck.wErrCode);
 //           break;
 //       }
 //   }
 //   else
 //   {
 //       SET_OTHER_ERR(m_stAck.wErrCode);
 //       SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
 //   }
 //   if(NULL != pRecordSet)
 //   {
 //       pRecordSet->Release();
 //       pRecordSet = NULL;
 //   }

 //   if (m_stAck.wErrCode != ERR_LOGIN::ID_SUCCESS)
 //   {
 //       return FALSE;
 //   }

 //   sprintf(szSQL, "call sp_PlayerAuthEx(%d, '%u', '%s', '%s', '%s', '%s', %d, %d, %d, %d, '%s', @UserID, @PlayerID, @ErrCode, @ErrMsg, @ZoneName, @OtherDeviceLoginFlag);",
 //           byAuthType, dwUserID, pszDeviceID, pszNotifyID, pszUserName, pszUserPwd, wZoneID, wCareerID, dwPlayerID, byAuthMethod, pszIp);

 //   DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);

 //   nRet = poDBConn->ExecuteSql(szSQL, NULL, &dwErr, &strError);
 //   if(nRet < 0)
 //   {
 //       SET_OTHER_ERR(m_stAck.wErrCode);
 //       SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
 //       return FALSE;
 //   }
 //   else
 //   {
 //       sprintf(szSQL, "select @UserID, @PlayerID, @ErrCode, @ErrMsg, @ZoneName, @OtherDeviceLoginFlag;");
 //   }
 //   nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
 //   if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
 //   {
 //       m_stAck.dwUserID = SDAtou(pRecordSet->GetFieldValueByName("@UserID"));
 //       m_stAck.dwPlayerID = SDAtou(pRecordSet->GetFieldValueByName("@PlayerID"));
 //       m_wErrCode = SDAtou(pRecordSet->GetFieldValueByName("@ErrCode"));
 //       SDStrcpy(szErrMsg, pRecordSet->GetFieldValueByName("@ErrMsg"));
 //       SDStrcpy(m_stAck.aszZoneName, pRecordSet->GetFieldValueByName("@ZoneName"));
 //       m_stAck.byOtherDeviceLoginFlag = SDAtou(pRecordSet->GetFieldValueByName("@OtherDeviceLoginFlag"));


 //       switch(m_wErrCode)
 //       {
 //       case 0:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_SUCCESS;
 //           break;
 //       case 1:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_PWD_ERR;
 //           break;
 //       case 2:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_REACH_MAX_PLAYER_NUM;
 //           break;
 //       case 3:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_PLAYER_NOT_EXIT;
 //           break;
 //       case 4:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_PLAYER_NOT_BIND;
 //           break;
 //       case 5:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_FORBID_CREATE_NEW_USER;
 //           break;
 //       case 6:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_INVALID_USER_TYPE;
 //           break;
 //       case 7:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_PLAYER_NOT_EXIT;
 //           break;
 //       case 8:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_SYSTEM_MAINTAIN;
 //           break;
 //       case 9:
 //           m_stAck.wErrCode = ERR_LOGIN::ID_FORBID_REGISTERED;
 //           break;
 //       default:
 //           SET_OTHER_ERR(m_stAck.wErrCode);
 //           SYS_CRITICAL(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
 //           break;
 //       }
 //   }
 //   else
 //   {
 //       SET_OTHER_ERR(m_stAck.wErrCode);
 //       SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
 //   }

 //   if(NULL != pRecordSet)
 //   {
 //       pRecordSet->Release();
 //       pRecordSet = NULL;
 //   }

 //   //老用户
	////必须要跟数据库的列对齐，如果数据库结构修改，代码也要跟随修改
	//CSDDateTime dt(SDTimeSecs());
	//ostringstream ss;
	//ss << "(" << m_stAck.dwUserID << "," << m_stAck.dwPlayerID << ",'" << pszDeviceID << "','" << pszIp << "','" << dt.ToString() << "'," << 1 << ")";
	//((CLSApplication*)SDGetApp())->AddData(ss.str(), poDBConn);

    return TRUE;
}
