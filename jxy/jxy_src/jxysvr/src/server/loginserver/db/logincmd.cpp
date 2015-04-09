#include "framework/lsapplication.h"
#include "logincmd.h"
#include <sdutil.h>
#include <sdtime.h>
#include <logic/dbconfig.h>
#include <net/gt/gtpktbuilder.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <logic/basefunc.h>
#include <algorithm/sdstrencrypt.h>
#include "net/cli/usermgr.h"
//#include <common/third/ppclient.h>

#include <sstream>
#include <ostream>
using namespace std;



CLoginCMD::CLoginCMD()
{
    m_wErrCode = 0;
    memset(&m_stAck, 0, sizeof(m_stAck));
}


CLoginCMD::~CLoginCMD()
{

}

VOID CLoginCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    m_bSuccess = FALSE;

    PKT_GTLS_LOGIN_REQ* pstReq = (PKT_GTLS_LOGIN_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;

    //对于重新创建新角色的用户。先检测一下用户是否是重复的请求。检测设备号以及流水号,是否是重复请求数据等
    if (0 == pstReq->dwPlayerID)
    {
        char v_szKey[256] = {0};
        sprintf(v_szKey, "%s_%u_%u_%u", pstReq->aszDeviceID, pstReq->wZoneID, pstReq->wCareerID, pstReq->dwSerialNumber);

        string strKey = v_szKey;
        DBG_INFO(_SDT("[%s: %d]: CK SerialNumber:%u Key:%s"), MSG_MARK, pstReq->dwSerialNumber, strKey.c_str());
        if(((CLSApplication*)SDGetApp())->GetLoginAck(strKey, m_stAck))
        {
            return;
        }
    }

    if (0 == pstReq->byPwdLen)
    {
        pstReq->abyUserPwd[0] = '\0';
    }

    _SDTStrcpy(m_stAck.aszUserName, pstReq->aszUserName);
    _SDTStrcpy(m_stAck.aszDeviceID, pstReq->aszDeviceID);
    _SDTStrcpy(m_stAck.aszNotifyID, pstReq->aszNotifyID);

    m_stAck.dwPlayerID = pstReq->dwPlayerID;
    m_stAck.wZoneID = pstReq->wZoneID;
    m_stAck.wCareerID = pstReq->wCareerID;
    if (pstReq->byPwdLen >= USERNAME_LEN)
    {
        pstReq->abyUserPwd[USERNAME_LEN - 1] = '\0';
    }
    else
    {
        pstReq->abyUserPwd[pstReq->byPwdLen] = '\0';
    }

    if(!GetVersionNotice(pstReq->dwCliVersion, m_stAck.stVersionInfo, m_stAck.stNoticeInfo1, m_stAck.stNoticeInfo2))
    {
        SET_OTHER_ERR(m_stAck.wErrCode);
        return;
    }

    //对于要求强制更新的用户，直接返回更新版本错误码
    if(1 == m_stAck.stVersionInfo.byForceUpdateFlag)
    {
        m_stAck.wErrCode = ERR_LOGIN::ID_VERSION_NEED_UPDATE;
    }
    else
    {
        m_bSuccess = Auth(poDBConn,
                          pstReq->byAuthType,
                          pstReq->aszDeviceID,
                          pstReq->aszNotifyID,
                          pstReq->aszUserName,
                          (TCHAR*)(pstReq->abyUserPwd),
                          pstReq->wCareerID,
                          pstReq->dwPlayerID,
                          pstReq->wZoneID,
                          pstReq->byAuthMethod,
                          pstReq->aszIP,
                          pstReq->dwSerialNumber,
                          pstReq->dwCliVersion);
    }
}


VOID CLoginCMD::OnExecuted()
{
    ResponseMsg(GTLS_LOGIN_ACK, (CHAR*)(&m_stAck));
}


BOOL CLoginCMD::GetVersionNotice(UINT32 dwCliVersion, DT_VERSION_DATA& stVersionInfo, DT_NOTICE_DATA& stNoticeInfo1, DT_NOTICE_DATA& stNoticeInfo2)
{
    SVersionNoticeInfo &stVersionNoticeInfo = ((CLSApplication*)SDGetApp())->GetDBMgr()->GetVersionNoticeInf();
    stVersionInfo = stVersionNoticeInfo.stVersionInfo;
    stVersionInfo.byForceUpdateFlag = (dwCliVersion < stVersionNoticeInfo.dwMinUnupdateVersionID) ? 1 : 0;


    //若版本一致，不下发更新内容
    if(dwCliVersion == stVersionInfo.dwVersionID)
    {
        stVersionInfo.aszUpdateContent[0] = 0;
        stVersionInfo.aszUpdateURL[0] = 0;
    }
    stNoticeInfo1 = stVersionNoticeInfo.stNoticeInfo1;
    stNoticeInfo2 = stVersionNoticeInfo.stNoticeInfo2;
    return TRUE;
}





BOOL CLoginCMD::Auth(SGDP::ISDDBConnection* poDBConn,
                     UINT8  byAuthType,
                     TCHAR* pszDeviceID,
                     TCHAR* pszNotifyID,
                     TCHAR* pszUserName,
                     TCHAR* pszUserPwd,
                     UINT16 wCareerID,
                     UINT32 dwPlayerID,
                     UINT16 wZoneID,
                     UINT8  byAuthMethod,
                     TCHAR* pszIp,
                     UINT32 dwSerialNumber,
                     UINT32 dwCliVersion)//wZoneID用于新增玩家
{
    CHAR		szSQL[1024] = {0};
    CHAR		szErrStr[128] = {0};
    UINT32	dwUserID =	0;
    UINT32	dwErr	=	0;
    string	strError;
    INT32	nRet = 0;
    ISDDBRecordSet* pRecordSet = NULL;

    m_stAck.byAuthType = byAuthType;
    //加密过的密码可能包括有数据库非常的字条，要先掉才行。
    if (AUTH_TYPE_MAXNET != byAuthType)
    {
        pszUserPwd[0] = '\0';
    }
    if (HaveSqlToken(pszIp))
    {
        pszIp[0] = '\0';
    }

    //检测区ID是否正确
    CSDServerID	oServerID(((CLSApplication*)SDGetApp())->GetLocalID());
    if(wZoneID != oServerID.GetAreaID())
    {
        m_stAck.wErrCode = ERR_LOGIN::ID_INVALID_ZONEID;
        return FALSE;
    }

    //检查参数
    m_stAck.wErrCode = CBaseFunc::CkAuthParam(pszDeviceID, byAuthType, pszNotifyID, pszUserName, pszUserPwd, wCareerID, wZoneID);
    if(ERR_LOGIN::ID_SUCCESS != m_stAck.wErrCode)
    {
        return FALSE;
    }

    //过虑设备号登录
    if (((CLSApplication*)SDGetApp())->GetDBMgr()->IsFindForbidLoginbyDeviceIDMap(pszDeviceID))
    {
        m_stAck.wErrCode = ERR_LOGIN::ID_FORBID_LOGIN;
        return FALSE;
    }

    //过虑玩家ID登录
    if (((CLSApplication*)SDGetApp())->GetDBMgr()->IsFindForbidLoginbyPlayerIDMap(dwPlayerID))
    {
        m_stAck.wErrCode = ERR_LOGIN::ID_FORBID_LOGIN;
        return FALSE;
    }

    CC_LOOP_DO
    //用于用户的数据检测登录，不再查找数据库
    UINT16	wErrCode = CUserMgr::Instance()->CkUser(m_stAck.dwUserID, m_stAck.aszZoneName, byAuthType, byAuthMethod, dwPlayerID, wCareerID, pszUserName, pszUserPwd, pszDeviceID, pszNotifyID, wZoneID, dwCliVersion);
    if (ERR_LOGIN::ID_SUCCESS == wErrCode)
    {
        //过虑用户ID过虑
        if (((CLSApplication*)SDGetApp())->GetDBMgr()->IsFindForbidLoginbyUserIDMap(m_stAck.dwUserID))
        {
            m_stAck.wErrCode = ERR_LOGIN::ID_FORBID_LOGIN;
        }
        break;
    }
    else if (ERR_LOGIN::ID_PWD_ERR == wErrCode)
    {
        //对于密码出错的，直接返回密码出错。
        m_stAck.wErrCode = ERR_LOGIN::ID_PWD_ERR;
        break;
    }

    //对于其它错误的，重新认证数据
    memset(&szSQL, 0, sizeof(szSQL));
    memset(&szErrStr, 0, sizeof(szErrStr));

    sprintf(szSQL, "call sp_UserAuthEx(%d, '%s', '%s', '%s', %d, '', 0, 0);", byAuthType, pszDeviceID, pszUserName, pszUserPwd, byAuthMethod);
    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);

    nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, &dwErr, &strError);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        dwUserID = SDAtou(pRecordSet->GetFieldValueByName("UserID"));
        m_wErrCode = SDAtou(pRecordSet->GetFieldValueByName("ErrCode"));
        SDStrcpy(szErrStr, pRecordSet->GetFieldValueByName("ErrStr"));

        UINT32	dwLen = pRecordSet->GetFieldLengthByName("UserName");
        dwLen = dwLen >= USERNAME_LEN ? USERNAME_LEN - 1 : dwLen;
        if(dwLen > 0)
        {
            memset(m_stAck.aszUserName, 0x00, sizeof(m_stAck.aszUserName));
            SDStrncpy(m_stAck.aszUserName, pRecordSet->GetFieldValueByName("UserName"), dwLen);
        }

        switch(m_wErrCode)
        {
        case 0:
            m_stAck.wErrCode = ERR_LOGIN::ID_SUCCESS;
            break;
        case 1:
            m_stAck.wErrCode = ERR_LOGIN::ID_PWD_ERR;
            break;
        case 2:
            m_stAck.wErrCode = ERR_LOGIN::ID_INVALID_USER_TYPE;
            break;
        case 3:
            m_stAck.wErrCode = ERR_LOGIN::ID_USERNAME_NOT_EXIT;
            break;
        case 4:
            m_stAck.wErrCode = ERR_LOGIN::ID_FORBID_LOGIN;
            break;
        default:
            SET_OTHER_ERR(m_stAck.wErrCode);
            SYS_CRITICAL(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
            break;
        }
    }
    else
    {
        SET_OTHER_ERR(m_stAck.wErrCode);
        SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    if (m_stAck.wErrCode != ERR_LOGIN::ID_SUCCESS)
    {
        break;
    }

    //过虑用户ID过虑
    if (((CLSApplication*)SDGetApp())->GetDBMgr()->IsFindForbidLoginbyUserIDMap(dwUserID))
    {
        m_stAck.wErrCode = ERR_LOGIN::ID_FORBID_LOGIN;
        break;
    }

    //过虑用户，当PlayerID为0，则要求新创建
    if (0 == dwPlayerID)
    {
        //通过设备号过虑用户注册
        if (((CLSApplication*)SDGetApp())->GetDBMgr()->IsForbidRegisteredbyDeviceIDMap(pszDeviceID))
        {
            m_stAck.wErrCode = ERR_LOGIN::ID_FORBID_REGISTERED;
            break;
        }

        //通过用户ID过虑用户注册
        if (((CLSApplication*)SDGetApp())->GetDBMgr()->IsFindForbidRegisteredbyUserID(dwUserID))
        {
            m_stAck.wErrCode = ERR_LOGIN::ID_FORBID_REGISTERED;
            break;
        }
    }

    memset(&szSQL, 0, sizeof(szSQL));
    memset(&szErrStr, 0, sizeof(szErrStr));
    sprintf(szSQL, "call sp_PlayerAuthEx(%d, '%u', '%s', '%s', '%s', '%s', %d, %d, %d, %d, '%s');",
            byAuthType, dwUserID, pszDeviceID, pszNotifyID, pszUserName, pszUserPwd, wZoneID, wCareerID, dwPlayerID, byAuthMethod, pszIp);

    DBG_INFO(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);

    nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, &dwErr, &strError);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        m_stAck.dwUserID = SDAtou(pRecordSet->GetFieldValueByName("UserID"));
        m_stAck.dwPlayerID = SDAtou(pRecordSet->GetFieldValueByName("PlayerID"));
        m_wErrCode = SDAtou(pRecordSet->GetFieldValueByName("ErrCode"));
        SDStrcpy(szErrStr, pRecordSet->GetFieldValueByName("ErrStr"));

        SDStrcpy(m_stAck.aszZoneName, pRecordSet->GetFieldValueByName("ZoneName"));
        m_stAck.byOtherDeviceLoginFlag = SDAtou(pRecordSet->GetFieldValueByName("OtherDeviceLoginFlag"));


        switch(m_wErrCode)
        {
        case 0:
            m_stAck.wErrCode = ERR_LOGIN::ID_SUCCESS;
            break;
        case 1:
            m_stAck.wErrCode = ERR_LOGIN::ID_PWD_ERR;
            break;
        case 2:
            m_stAck.wErrCode = ERR_LOGIN::ID_REACH_MAX_PLAYER_NUM;
            break;
        case 3:
            m_stAck.wErrCode = ERR_LOGIN::ID_PLAYER_NOT_EXIT;
            break;
        case 4:
            m_stAck.wErrCode = ERR_LOGIN::ID_PLAYER_NOT_BIND;
            break;
        case 5:
            m_stAck.wErrCode = ERR_LOGIN::ID_FORBID_CREATE_NEW_USER;
            break;
        case 6:
            m_stAck.wErrCode = ERR_LOGIN::ID_INVALID_USER_TYPE;
            break;
        case 7:
            m_stAck.wErrCode = ERR_LOGIN::ID_PLAYER_NOT_EXIT;
            break;
        case 8:
            m_stAck.wErrCode = ERR_LOGIN::ID_SYSTEM_MAINTAIN;
            break;
        case 9:
            m_stAck.wErrCode = ERR_LOGIN::ID_FORBID_REGISTERED;
            break;
        default:
            SET_OTHER_ERR(m_stAck.wErrCode);
            SYS_CRITICAL(_SDT("[%s: %d]: SQL:%s"), MSG_MARK, szSQL);
            break;
        }
    }
    else
    {
        SET_OTHER_ERR(m_stAck.wErrCode);
        SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
    }

    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    //对于操作数据库的检测都要在内存中保存一份。
    if (ERR_LOGIN::ID_SUCCESS == m_stAck.wErrCode)
    {
        CUserMgr::Instance()->CreateUser(m_stAck.dwUserID, byAuthType, byAuthMethod, m_stAck.dwPlayerID, wCareerID, pszUserName, pszUserPwd, pszDeviceID, pszNotifyID, m_stAck.aszZoneName, wZoneID, dwCliVersion);
    }
    CC_LOOP_WHILE(0);

    //对于重新创建新角色的用户。保存用户发送数据
    if (ERR_LOGIN::ID_SUCCESS == m_stAck.wErrCode && 0 == dwPlayerID)
    {
        char v_szKey[256] = {0};
        sprintf(v_szKey, "%s_%u_%u_%u", pszDeviceID, wZoneID, wCareerID, dwSerialNumber);
        string strKey = v_szKey;

        DBG_INFO(_SDT("[%s: %d]: Set SerialNumber:%u Key:%s"), MSG_MARK, dwSerialNumber, strKey.c_str());
        ((CLSApplication*)SDGetApp())->AddLoginAck(strKey, m_stAck);
    }

    //必须要跟数据库的列对齐，如果数据库结构修改，代码也要跟随修改
    CSDDateTime dt(SDTimeSecs());
    char v_szData[1024] = {0};
    sprintf(v_szData, "(%u, %u, '%s', '%s', '%s', %u)", m_stAck.dwUserID, m_stAck.dwPlayerID, pszDeviceID, pszIp, dt.ToString().c_str(), 1);
    ((CLSApplication*)SDGetApp())->AddData(v_szData);

    return TRUE;
}
