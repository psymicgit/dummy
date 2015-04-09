
#include "usermgr.h"
#include <common/client/errdef.h>

#include <sstream>
#include <ostream>
using namespace std;


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

BOOL CUserMgr::CreateUser(UINT32 dwUserID, UINT8  byAuthType, UINT8  byAuthMethod, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNickName)
{
    CSDMutexLock lock(m_oMutex);
    CUser* poUser = _FindUser(pszDeviceID);
    if(poUser)
    {
        poUser->SetUserProp(dwUserID, byAuthType, byAuthMethod, pszUserName, pszPasswd, pszDeviceID, pszNickName);
        return TRUE;
    }
    poUser = m_oUserPool.Alloc();
    poUser->SetUserProp(dwUserID, byAuthType, byAuthMethod, pszUserName, pszPasswd, pszDeviceID, pszNickName);
    m_mapPlayerID2User[pszDeviceID] = poUser;

    return TRUE;
}

BOOL CUserMgr::CkUser(UINT32 &dwUserID, UINT8  byAuthType, UINT8  byAuthMethod, TCHAR* pszUserName, TCHAR* pszPasswd, TCHAR* pszDeviceID, TCHAR* pszNickName)
{
    CSDMutexLock lock(m_oMutex);
    CUser* poUser = _FindUser(pszDeviceID);
    if(NULL == poUser)
    {
        return FALSE;
    }
    return poUser->CkUser(dwUserID, byAuthType, byAuthMethod, pszUserName, pszPasswd, pszDeviceID, pszNickName);
}

void	 CUserMgr::ReleaseUser(UINT32 dwUserID)
{
    CSDMutexLock lock(m_oMutex);
    CSessionID2UserMapItr itr;
    for(itr = m_mapPlayerID2User.begin(); itr != m_mapPlayerID2User.end();)
    {
        CUser* poUser = itr->second;
        if(poUser->GetUserID() == dwUserID)
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



VOID CUserMgr::CkUserDisconnect()
{
    CSessionID2UserMapItr itr;
    CSDMutexLock lock(m_oMutex);
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

VOID CUserMgr::_ReleaseUser(CUser* poUser)
{
    if(NULL == poUser)
    {
        return;
    }

    CSessionID2UserMapItr itr;
    itr = m_mapPlayerID2User.find(poUser->GetDeviceID());
    if(itr != m_mapPlayerID2User.end())
    {
        m_mapPlayerID2User.erase(itr);
    }
    m_oUserPool.Free(poUser);
    poUser = NULL;
}

CUser* CUserMgr::_FindUser(string strkey)
{
    CSessionID2UserMapItr itr;
    itr = m_mapPlayerID2User.find(strkey);
    if(itr != m_mapPlayerID2User.end())
    {
        return itr->second;
    }
    return NULL;
}

