
#include "usermgr.h"
#include <common/client/errdef.h>


IMPLEMENT_SINGLETON(CUserMgr)
CUserMgr::CUserMgr(): m_oUserPool(MAX_USER)
{

}

CUserMgr::~CUserMgr()
{
    CUser* poUser = NULL;
    CSessionID2UserMapItr itr;
    CSDMutexLock lock(m_oMutex);
    for (itr = m_mapPlayerID2User.begin() ; itr != m_mapPlayerID2User.end();)
    {
        poUser = itr->second;
        m_oUserPool.Free(poUser);
        poUser = NULL;
    }
    m_mapPlayerID2User.clear() ;
}


BOOL CUserMgr::Init()
{
    return TRUE;
}

BOOL CUserMgr::CreateUser(UINT32 dwUserID, UINT8  byAuthType, UINT8  byAuthMethod, UINT32 dwPlayerID, UINT16 wCareerID, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNotifyID, TCHAR *pszZoneName, UINT16 wZoneID, UINT32 dwCliVer)
{
    CSDMutexLock lock(m_oMutex);
    CUser* poUser = FindUser(dwPlayerID);
    if(poUser)
    {
        poUser->SetUserProp(dwUserID, dwPlayerID, byAuthType, byAuthMethod, pszUserName, pszPasswd, pszDeviceID, pszNotifyID, pszZoneName, wCareerID, wZoneID, dwCliVer);
        return TRUE;
    }
    poUser = m_oUserPool.Alloc();
    poUser->SetUserProp(dwUserID, dwPlayerID, byAuthType, byAuthMethod, pszUserName, pszPasswd, pszDeviceID, pszNotifyID, pszZoneName, wCareerID, wZoneID, dwCliVer);
    m_mapPlayerID2User[dwPlayerID] = poUser;

    return TRUE;
}

UINT16 CUserMgr::CkUser(UINT32 &dwUserID, TCHAR *pszZoneName, UINT8  byAuthType, UINT8  byAuthMethod, UINT32 dwPlayerID, UINT16 wCareerID, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT16 wZoneID, UINT32 dwCliVer)
{
    CSDMutexLock lock(m_oMutex);
    CUser* poUser = FindUser(dwPlayerID);
    if(NULL == poUser)
    {
        return ERR_COMMON::ID_OTHER_ERR;
    }
    return poUser->CkUser(dwUserID, pszZoneName, dwPlayerID, byAuthType, byAuthMethod, pszUserName, pszPasswd, pszDeviceID, pszNotifyID, wCareerID, wZoneID, dwCliVer);
}

VOID CUserMgr::CkUserDisconnect()
{
    CSDMutexLock lock(m_oMutex);
    CSessionID2UserMapItr itr;
    for(itr = m_mapPlayerID2User.begin(); itr != m_mapPlayerID2User.end();)
    {
        CUser* poUser = itr->second;
        if(poUser->CkUserDisconnect())
        {
            m_oUserPool.Free(poUser);
            m_mapPlayerID2User.erase(itr++);
        }
        else
        {
            itr++;
        }
    }
}

VOID	 CUserMgr::ChangePwd(UINT32 dwUserID, TCHAR* pszPasswd)
{
    CSDMutexLock lock(m_oMutex);
    CSessionID2UserMapItr itr;
    for(itr = m_mapPlayerID2User.begin(); itr != m_mapPlayerID2User.end(); itr++)
    {
        CUser* poUser = itr->second;
        if (poUser->GetUserID() == dwUserID)
        {
            poUser->ChangePwd(pszPasswd);
        }
    }
}


VOID CUserMgr::ReleaseUser(CUser* poUser)
{
    if(NULL == poUser)
    {
        return;
    }

    CSessionID2UserMapItr itr;
    itr = m_mapPlayerID2User.find(poUser->GetPlayerID());
    if(itr != m_mapPlayerID2User.end())
    {
        m_mapPlayerID2User.erase(itr);
    }
    m_oUserPool.Free(poUser);
    poUser = NULL;
}


CUser* CUserMgr::FindUser(UINT32 dwPlayerID)
{
    CSessionID2UserMapItr itr;
    itr = m_mapPlayerID2User.find(dwPlayerID);
    if(itr != m_mapPlayerID2User.end())
    {
        return itr->second;
    }
    return NULL;
}

