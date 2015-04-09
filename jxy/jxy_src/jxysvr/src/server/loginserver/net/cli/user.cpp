
#include "user.h"
#include <framework/lsapplication.h>
#include <net/gt/gtpktbuilder.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <common/client/errdef.h>

#define MAX_TIME_OUT	3600


CUser::CUser()
{
    m_dwUserID = 0;
    m_dwPlayerID = 0;
    m_byAuthType = 0;
    m_byAuthMethod = 0;
    memset(m_szUserName, 0, sizeof(m_szUserName));
    memset(m_szPasswd, 0x00, sizeof(m_szPasswd));
    memset(m_szDeviceID, 0, sizeof(m_szDeviceID));
    memset(m_szNotifyID, 0, sizeof(m_szNotifyID));
    memset(m_aszZoneName, 0, sizeof(m_aszZoneName));

    m_qwLastRecvMsgTime = SDTimeSecs();
}


CUser::~CUser()
{

}

VOID CUser::SetUserProp(UINT32 dwUserID, UINT8  byAuthType, UINT8  byAuthMethod, UINT32 dwPlayerID, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNotifyID, TCHAR *pszZoneName, UINT16 wCareerID, UINT16 wZoneID, UINT32 dwCliVer)
{
    m_dwUserID	= dwUserID;
    m_dwPlayerID	= dwPlayerID;
    m_wCareerID	= wCareerID;
    m_wZoneID	= wZoneID;
    m_dwCliVer	= dwCliVer;
    m_byAuthType = byAuthType;
    m_byAuthMethod = byAuthMethod;
    memset(m_szUserName, 0x00, sizeof(m_szUserName));
    memset(m_szPasswd, 0x00, sizeof(m_szPasswd));
    memset(m_szDeviceID, 0x00, sizeof(m_szDeviceID));
    memset(m_szNotifyID, 0x00, sizeof(m_szNotifyID));
    _SDTStrcpy(m_szUserName, pszUserName);
    _SDTStrcpy(m_szPasswd, pszPasswd);
    _SDTStrcpy(m_szDeviceID, pszDeviceID);
    _SDTStrcpy(m_szNotifyID, pszNotifyID);
    _SDTStrcpy(m_aszZoneName, pszZoneName);

    m_qwLastRecvMsgTime = SDTimeSecs();
}

UINT16 CUser::CkUser(UINT32 &dwUserID, TCHAR *pszZoneName, UINT8  byAuthType, UINT8  byAuthMethod, UINT32 dwPlayerID, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT16 wCareerID, UINT16 wZoneID, UINT32 dwCliVer)
{
    dwUserID = m_dwUserID;
    _SDTStrcpy(pszZoneName, m_aszZoneName);
    if(0 == m_byAuthMethod && 0 == byAuthMethod)
    {
        //一般登录认证与一般登录认证，比对设备号、用户名、、密码
        if (dwPlayerID == m_dwPlayerID &&
                wCareerID == m_wCareerID &&
                wZoneID == m_wZoneID &&
                dwCliVer == m_dwCliVer &&
                byAuthType == m_byAuthType &&
                (_SDTStrcmp(pszUserName, m_szUserName) == 0) &&
                (_SDTStrcmp(pszPasswd, m_szPasswd) == 0) &&
                (_SDTStrcmp(pszDeviceID, m_szDeviceID) == 0) &&
                (_SDTStrcmp(pszNotifyID, m_szNotifyID) == 0)
           )
        {
            m_qwLastRecvMsgTime = SDTimeSecs();
            return ERR_LOGIN::ID_SUCCESS;
        }
        else if (dwPlayerID == m_dwPlayerID &&
                 (_SDTStrcmp(pszPasswd, m_szPasswd) != 0))
        {
            m_qwLastRecvMsgTime = SDTimeSecs();
            return ERR_LOGIN::ID_PWD_ERR;
        }
    }
    else if (0 == m_byAuthMethod && 1 == byAuthMethod)
    {
        //一般登录认证与快速登录认证，比对设备号、用户名
        if (dwPlayerID == m_dwPlayerID &&
                wCareerID == m_wCareerID &&
                wZoneID == m_wZoneID &&
                dwCliVer == m_dwCliVer &&
                byAuthType == m_byAuthType &&
                (_SDTStrcmp(pszUserName, m_szUserName) == 0) &&
                (_SDTStrcmp(pszDeviceID, m_szDeviceID) == 0) &&
                (_SDTStrcmp(pszNotifyID, m_szNotifyID) == 0)
           )
        {
            m_qwLastRecvMsgTime = SDTimeSecs();
            return ERR_LOGIN::ID_SUCCESS;
        }
        else if (dwPlayerID == m_dwPlayerID &&
                 (_SDTStrcmp(pszPasswd, m_szPasswd) != 0))
        {
            m_qwLastRecvMsgTime = SDTimeSecs();
            return ERR_LOGIN::ID_PWD_ERR;
        }
    }
    else if (1 == m_byAuthMethod && 1 == byAuthMethod)
    {
        //快速登录认证与快速登录认证， 比对设备号、及用户名、不验证密码
        if (dwPlayerID == m_dwPlayerID &&
                wCareerID == m_wCareerID &&
                wZoneID == m_wZoneID &&
                dwCliVer == m_dwCliVer &&
                byAuthType == m_byAuthType &&
                (_SDTStrcmp(pszUserName, m_szUserName) == 0) &&
                (_SDTStrcmp(pszDeviceID, m_szDeviceID) == 0) &&
                (_SDTStrcmp(pszNotifyID, m_szNotifyID) == 0)
           )
        {
            m_qwLastRecvMsgTime = SDTimeSecs();
            return ERR_LOGIN::ID_SUCCESS;
        }
    }
    return ERR_COMMON::ID_OTHER_ERR;
}

BOOL CUser::CkUserDisconnect()
{
    if ( SDTimeSecs() > m_qwLastRecvMsgTime + MAX_TIME_OUT)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

VOID	 CUser::ChangePwd(TCHAR* pszPasswd)
{
    memset(m_szPasswd, 0x00, sizeof(m_szPasswd));
    _SDTStrcpy(m_szPasswd, pszPasswd);
}

