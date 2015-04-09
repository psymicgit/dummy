
#include "smsuser.h"
#include <sdutil.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <common/client/errdef.h>




CSmsUser::CSmsUser()
{
    m_bAuth = FALSE;
    m_dwUserID = 0;
    m_qwCreateTime	= SDTimeSecs();
    m_qwRecvMsgTime	= SDTimeSecs();
    memset(m_szUserName, 0x00, sizeof(m_szUserName));
    memset(m_szVerCode, 0x00, sizeof(m_szVerCode));
    memset(m_szMobile, 0x00, sizeof(m_szMobile));
}


CSmsUser::~CSmsUser()
{

}

BOOL		CSmsUser::SetUserProp(UINT32 dwUserID, TCHAR* pszUserName, TCHAR* pszMobile, TCHAR* pszVerCode)
{
    m_dwUserID =  dwUserID;
    m_qwRecvMsgTime = SDTimeSecs();
    memset(m_szUserName, 0x00, sizeof(m_szUserName));
    memset(m_szVerCode, 0x00, sizeof(m_szVerCode));
    memset(m_szMobile, 0x00, sizeof(m_szMobile));
    SDStrcpy(m_szUserName, pszUserName);
    SDStrcpy(m_szMobile, pszMobile);
    SDStrcpy(m_szVerCode, pszVerCode);
    return TRUE;
}

BOOL		CSmsUser::CkUser(TCHAR* pszVerCode, TCHAR *pszUserName)
{
    SDStrcpy(pszUserName, m_szUserName);
    if (0 == SDStrcmp(m_szVerCode, pszVerCode))
    {
        m_bAuth = TRUE;
        m_qwRecvMsgTime = SDTimeSecs();
        return TRUE;
    }
    else
    {
        m_bAuth = FALSE;
        return FALSE;
    }
}


BOOL CSmsUser::CkUserDisconnect()
{
    if ( SDTimeSecs() > m_qwCreateTime + MAX_MOBILE_DISCONNECT)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

BOOL	CSmsUser::CkUser(UINT16 &wCountdown)
{
    if(SDTimeSecs() - m_qwRecvMsgTime < MAX_MOBILE_TIME_OUT)
    {
        wCountdown = (UINT16)(MAX_MOBILE_TIME_OUT - (SDTimeSecs() - m_qwRecvMsgTime));
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}
