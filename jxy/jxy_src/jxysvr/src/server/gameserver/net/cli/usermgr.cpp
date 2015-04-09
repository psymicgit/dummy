
#include "usermgr.h"

#include <dll/sdframework/sdloggerimpl.h>


IMPLEMENT_SINGLETON(CUserMgr)
CUserMgr::CUserMgr(): m_oUserPool(MAX_USER)
{
    m_itrCurDealUser = m_mapSessionID2User.end();
    m_dwConnectedNum = 0;
}

CUserMgr::~CUserMgr()
{
    CUser* poUser = NULL;
    for (CSessionID2UserMapItr itr = m_mapSessionID2User.begin() ;
            itr != m_mapSessionID2User.end();)
    {
        poUser = itr->second;
        m_oUserPool.Free(poUser);
        poUser = NULL;

    }
    m_mapSessionID2User.clear() ;

}


BOOL CUserMgr::Init()
{
    return TRUE;
}

CUser* CUserMgr::CreateUser(UINT32 dwUserID, UINT32 dwPlayerID, UINT16 wCareerID, TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, 
    UINT16 wZoneID, UINT16 wOriZoneID, UINT32 dwLoginIP, UINT32 dwCliVer, UINT8 byAuthType,UINT8 byDaichong)
{
    CUser* poUser = FindUser(dwPlayerID);
    if(poUser)
    {
        return poUser;
    }
    if(0 == dwPlayerID)
    {
        SYS_CRITICAL(_SDT("[%s:%d]: PlayerID is 0 , DeviceID[%s]!"), MSG_MARK, pszDeviceID);
        return NULL;
    }

    poUser = m_oUserPool.Alloc();
    poUser->SetUserProp(dwUserID, dwPlayerID, pszUserName, pszDeviceID, pszNotifyID, wCareerID, wZoneID, wOriZoneID, dwLoginIP, dwCliVer, byAuthType, byDaichong);
    m_mapSessionID2User[dwPlayerID] = poUser;
    m_itrCurDealUser = m_mapSessionID2User.begin();

    return poUser;
}


VOID CUserMgr::ReleaseUser(CUser* poUser)
{
    if(NULL == poUser)
    {
        return;
    }
    CSessionID2UserMapItr itrSessionID = m_mapSessionID2User.find(poUser->GetPlayerID());
    if(itrSessionID != m_mapSessionID2User.end())
    {
        if(m_itrCurDealUser == itrSessionID)
        {
            m_mapSessionID2User.erase(itrSessionID);
        }
        else
        {
            m_mapSessionID2User.erase(itrSessionID);
        }
        m_itrCurDealUser = m_mapSessionID2User.begin();
    }

    m_oUserPool.Free(poUser);
    poUser = NULL;
}

/*
CUser* CUserMgr::FindUser(TCHAR* pszUserName, TCHAR* pszDeviceID)
{
	CNameDevice2UserMapItr itrStr = m_mapNameDevice2User.find(GetKey(pszUserName, pszDeviceID));
	if(itrStr != m_mapNameDevice2User.end())
	{
		return itrStr->second;
	}

	return NULL;
}
*/

CUser* CUserMgr::FindUser(UINT32 dwPlayerID)
{
    CSessionID2UserMapItr itr = m_mapSessionID2User.find(dwPlayerID);
    if(itr != m_mapSessionID2User.end())
    {
        return itr->second;
    }

    return NULL;
}


VOID CUserMgr::CkUserDisconnect()
{
    UINT16 wIdx = 0;
    if(m_itrCurDealUser == m_mapSessionID2User.end())
    {
        m_itrCurDealUser = m_mapSessionID2User.begin();
    }
    for(; ((m_itrCurDealUser != m_mapSessionID2User.end()) && (wIdx < 10)); m_itrCurDealUser++, wIdx++)
    {
        if (NULL == m_itrCurDealUser->second)
        {
            continue;
        }
        m_itrCurDealUser->second->CkUserDisconnect();
    }
}

