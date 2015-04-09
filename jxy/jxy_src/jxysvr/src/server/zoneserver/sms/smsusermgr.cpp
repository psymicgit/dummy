
#include "smsusermgr.h"
#include <common/client/errdef.h>

#include <sstream>
#include <ostream>
using namespace std;


IMPLEMENT_SINGLETON(CSmsUserMgr)
CSmsUserMgr::CSmsUserMgr(): m_oUserPool(MAX_USER)
{

}

CSmsUserMgr::~CSmsUserMgr()
{
    CSmsUser* poUser = NULL;
    CSmsID2UserMapItr itr;
    CSDMutexLock lock(m_oMutex);
    for (itr = m_mapSmsID2User.begin() ; itr != m_mapSmsID2User.end();)
    {
        poUser = itr->second;
        m_oUserPool.Free(poUser);
        poUser = NULL;
    }
    m_mapSmsID2User.clear() ;
}


BOOL CSmsUserMgr::Init()
{
    return TRUE;
}

BOOL CSmsUserMgr::CreateUser(string strKey, UINT32 dwUserID, TCHAR* pszUserName, TCHAR* pszMobile, TCHAR* pszVerCode)
{
    CSmsUser * poUser = NULL;
    CSDMutexLock lock(m_oMutex);
    CSmsID2UserMapItr itr = m_mapSmsID2User.find(strKey);
    if(itr != m_mapSmsID2User.end())
    {
        poUser = itr->second;
        return poUser->SetUserProp(dwUserID, pszUserName, pszMobile, pszVerCode);
    }
    poUser = m_oUserPool.Alloc();
    poUser->SetUserProp(dwUserID, pszUserName, pszMobile, pszVerCode);
    m_mapSmsID2User[strKey] = poUser;
    return TRUE;
}

BOOL	CSmsUserMgr::CkUser(string strKey, TCHAR* pszVerCode, TCHAR *pszUserName)
{
    CSDMutexLock lock(m_oMutex);
    CSmsID2UserMapItr itr = m_mapSmsID2User.find(strKey);
    if(itr == m_mapSmsID2User.end())
    {
        return FALSE;
    }
    CSmsUser *poUser = itr->second;
    return poUser->CkUser(pszVerCode, pszUserName);
}

BOOL	CSmsUserMgr::CkUser(TCHAR *pszMobile, UINT8 abyToken[MAX_RESET_PASSWD_TOKEN_NUM], UINT16 &wCountdown)
{
    CSDMutexLock lock(m_oMutex);
    CSmsID2UserMapItr itr = m_mapSmsID2User.begin();
    for (itr; itr != m_mapSmsID2User.end(); itr++)
    {
        CSmsUser *poUser = itr->second;
        if (0 == SDStrcmp(poUser->GetMobile(), pszMobile))
        {
            SDStrcpy((CHAR*)abyToken, itr->first.c_str());
            return poUser->CkUser(wCountdown);
        }
    }
    return FALSE;
}

BOOL	CSmsUserMgr::CkAuth(string strKey, UINT32 &dwUserID)
{
    CSDMutexLock lock(m_oMutex);
    CSmsID2UserMapItr itr = m_mapSmsID2User.find(strKey);
    if(itr == m_mapSmsID2User.end())
    {
        return FALSE;
    }
    CSmsUser *poUser = itr->second;
    dwUserID = poUser->GetUserID();
    return poUser->GetAuth();
}


VOID CSmsUserMgr::CkUserDisconnect()
{
    CSmsID2UserMapItr itr;
    CSDMutexLock lock(m_oMutex);
    for(itr = m_mapSmsID2User.begin(); itr != m_mapSmsID2User.end();)
    {
        CSmsUser* poUser = itr->second;
        if(poUser->CkUserDisconnect())
        {
            m_oUserPool.Free(poUser);
            m_mapSmsID2User.erase(itr++);
        }
        else
        {
            itr++;
        }
    }
}



