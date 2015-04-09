
#include "user.h"
#include <framework/zsapplication.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <common/client/errdef.h>

#define MAX_TIME_OUT	3600


CUser::CUser()
{
    m_dwUserID = 0;
    m_byAuthType = 0;
    m_byAuthMethod = 0;
    memset(m_szUserName, 0, sizeof(m_szUserName));
    memset(m_szNickName, 0, sizeof(m_szNickName));
    memset(m_szPasswd, 0x00, sizeof(m_szPasswd));
    memset(m_szDeviceID, 0, sizeof(m_szDeviceID));
    m_qwLastRecvMsgTime = SDTimeSecs();
}


CUser::~CUser()
{

}

VOID CUser::SetUserProp(UINT32 dwUserID, UINT8  byAuthType, UINT8  byAuthMethod, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNickName)
{
    m_dwUserID	= dwUserID;
    m_byAuthType = byAuthType;
    m_byAuthMethod = byAuthMethod;
    memset(m_szUserName, 0x00, sizeof(m_szUserName));
    memset(m_szPasswd, 0x00, sizeof(m_szPasswd));
    memset(m_szDeviceID, 0x00, sizeof(m_szDeviceID));
    memset(m_szNickName, 0x00, sizeof(m_szNickName));
    _SDTStrcpy(m_szUserName, pszUserName);
    _SDTStrcpy(m_szPasswd, pszPasswd);
    _SDTStrcpy(m_szDeviceID, pszDeviceID);
    _SDTStrcpy(m_szNickName, pszNickName);
    m_qwLastRecvMsgTime = SDTimeSecs();
}

void	CUser::SetPasswd(TCHAR* pszPasswd)
{
	memset(m_szPasswd, 0x00, sizeof(m_szPasswd));
	_SDTStrcpy(m_szPasswd, pszPasswd);
}

BOOL CUser::CkUser(UINT32 &dwUserID, UINT8  byAuthType, UINT8  byAuthMethod, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNickName)
{
    dwUserID = m_dwUserID;
    //一般登录认证与一般登录认证，比对设备号、用户名、、密码
    if (byAuthType == m_byAuthType &&
        byAuthMethod == m_byAuthMethod &&
        (_SDTStrcmp(pszUserName, m_szUserName) == 0) &&
        (_SDTStrcmp(pszPasswd, m_szPasswd) == 0) &&
        (_SDTStrcmp(pszDeviceID, m_szDeviceID) == 0) &&
        (_SDTStrcmp(pszNickName, m_szNickName) == 0)
       )
    {
        m_qwLastRecvMsgTime = SDTimeSecs();
        return TRUE;
    }
    return FALSE;
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
