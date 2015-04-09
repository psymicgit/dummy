
#include "smsuser.h"
#include <sdutil.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <common/client/errdef.h>




CSmsUser::CSmsUser()
{
    m_dwUserID = 0;
    m_qwCreateTime = SDTimeSecs();
    m_qwRecvMsgTime = SDTimeSecs();
    memset(m_szMobile, 0x00, sizeof(m_szMobile));
    memset(m_szVerCode, 0x00, sizeof(m_szVerCode));
}


CSmsUser::~CSmsUser()
{

}

BOOL	CSmsUser::SetUserProp(UINT32 dwUserID, TCHAR* pszMobile, TCHAR* pszVerCode)
{
    m_dwUserID =  dwUserID;
    m_qwRecvMsgTime = SDTimeSecs();
    memset(m_szMobile, 0x00, sizeof(m_szMobile));
    memset(m_szVerCode, 0x00, sizeof(m_szVerCode));
    SDStrcpy(m_szMobile, pszMobile);
    SDStrcpy(m_szVerCode, pszVerCode);
    return TRUE;
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

BOOL	CSmsUser::CkUser(TCHAR *pszMobile, UINT16 &wCountdown)
{
    if(0 != SDStrcmp(pszMobile, m_szMobile))
    {
        wCountdown = MAX_MOBILE_TIME_OUT;
        return FALSE;
    }

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
