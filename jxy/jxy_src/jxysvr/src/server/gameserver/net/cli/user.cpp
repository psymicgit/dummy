
#include "user.h"

#include <db/autosqlbuf.h>
#include <net/gt/gtpktbuilder.h>
#include <sdutil.h>
#include <errdef.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <net/db/dbpktbuilder.h>
#include <logic/player/playermgr.h>
#include <framework/gsapi.h>
#include <logic/player/player.h>
#include <sdpipechannel.h>

CUser::CUser()
{
    m_enUserStat = CLIS_LOGIN_INVALID;
    m_wZoneID = 0;
    m_wOriZoneID = 0;

    m_dwPlayerID = 0;
    m_dwUserID = 0;
    m_dwLoginIP = 0;
    m_dwCliVer = 0;
    m_byAuthType = 0;
    memset(m_szUserName, 0, sizeof(m_szUserName));
    memset(m_szDeviceID, 0, sizeof(m_szDeviceID));
    memset(m_szNotifyID, 0, sizeof(m_szNotifyID));

    m_qwLastRecvMsgTime = SDTimeSecs();
}

CUser::~CUser()
{

}


BOOL CUser::Send(const CHAR* pData, UINT32 dwLen)
{
    CSDPipeChannel* poPipeChannel = gsapi::GetGTPipeChannel(m_wZoneID);

    if (poPipeChannel)
    {
        SDPipeMsgHeader* pHeader = (SDPipeMsgHeader*)pData ;
        pHeader->dwTransID    = SDHtonl(m_dwPlayerID);
        return poPipeChannel->Send(pData, dwLen);
    }

    return FALSE;
}

BOOL CUser::SendMsg(const CHAR* pData, UINT16 wMsgID)
{
    CSDPipeChannel* poPipeChannel = gsapi::GetGTPipeChannel(m_wZoneID);

    if (poPipeChannel)
    {
        return poPipeChannel->SendMsg(m_dwPlayerID, wMsgID, pData);
    }

    return FALSE;
}

EUserStat CUser::GetUserStat()
{
    return m_enUserStat;
}

VOID CUser::SetUserStat(EUserStat enCliSessionStat)
{
    m_enUserStat = enCliSessionStat;
}

VOID CUser::SetUserProp(UINT32 dwUserID, UINT32 dwPlayerID, TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT16 wCareerID, UINT16 wZoneID, UINT16 wOriZoneID, UINT32 dwLoginIP, UINT32 dwCliVer, UINT8 byAuthType, UINT8 byDaiChong)
{
    m_dwUserID	= dwUserID;
    m_dwPlayerID	= dwPlayerID;
    m_wCareerID	= wCareerID;
    m_wZoneID	= wZoneID;
    m_wOriZoneID = wOriZoneID;
    m_dwLoginIP	= dwLoginIP;
    m_dwCliVer	= dwCliVer;
    m_byAuthType	= byAuthType;
    _SDTStrcpy(m_szUserName, pszUserName);
    _SDTStrcpy(m_szDeviceID, pszDeviceID);
    _SDTStrcpy(m_szNotifyID, pszNotifyID);
    m_byDaiChong = byDaiChong;
}

CPlayer*    CUser::GetPlayer()
{
    return CPlayerMgr::Instance()->FindPlayer(m_dwPlayerID);
}

VOID CUser::CkUserLogin()
{
    UINT64 qwCurSecs = SDTimeSecs();
    if((qwCurSecs - m_qwLastRecvMsgTime) > 60)
    {
        CPlayer* poPlayer = GetPlayer();
        if((poPlayer) && (!poPlayer->CkLogin()))
        {
            poPlayer->OnLogin(TRUE);
        }
    }
    m_qwLastRecvMsgTime = SDTimeSecs();
}

VOID CUser::CkUserDisconnect()
{
    UINT64 qwCurSecs = SDTimeSecs();
    if((qwCurSecs - m_qwLastRecvMsgTime) >  600)
    {
        CPlayer* poPlayer = GetPlayer();
        if((poPlayer) && (poPlayer->CkLogin()))
        {
            poPlayer->OnLogin(FALSE);
        }
    }
}

