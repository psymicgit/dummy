
#include "smsusermgr.h"
#include <common/client/errdef.h>

#include <sstream>
#include <ostream>
using namespace std;


static UINT64 Get3232Key(UINT32 dwValue1,UINT32 dwValue2)
{
	UINT64 qwKey = dwValue1;
	qwKey = qwKey <<32;
	qwKey += dwValue2;

	return qwKey;
}

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

BOOL CSmsUserMgr::CreateUser(UINT32 dwUserID, TCHAR* pszMobile, em_OptType emOpt, TCHAR* pszVerCode)
{
    CSmsUser * poUser = NULL;
    CSDMutexLock lock(m_oMutex);
    CSmsID2UserMapItr itr = m_mapSmsID2User.find(Get3232Key(dwUserID, emOpt));
    if(itr != m_mapSmsID2User.end())
    {
        poUser = itr->second;
        return poUser->SetUserProp(dwUserID, pszMobile, pszVerCode);
    }
    poUser = m_oUserPool.Alloc();
    poUser->SetUserProp(dwUserID, pszMobile, pszVerCode);
    m_mapSmsID2User[Get3232Key(dwUserID, emOpt)] = poUser;
    return TRUE;
}

BOOL	CSmsUserMgr::CkUser(UINT32 dwUserID, TCHAR* pszMobile, em_OptType emOpt, UINT16 &wCountdown)
{
    CSDMutexLock lock(m_oMutex);
    CSmsID2UserMapItr itr = m_mapSmsID2User.find(Get3232Key(dwUserID, emOpt));
    if(itr == m_mapSmsID2User.end())
    {
        return FALSE;
    }
    CSmsUser *poUser = itr->second;
    return poUser->CkUser(pszMobile, wCountdown);
}

BOOL	CSmsUserMgr::CkUser(UINT32 dwUserID, em_OptType emOpt, TCHAR* pszVerCode, TCHAR *pszMobile)
{
    CSDMutexLock lock(m_oMutex);
    CSmsID2UserMapItr itr = m_mapSmsID2User.find(Get3232Key(dwUserID, emOpt));
    if(itr == m_mapSmsID2User.end())
    {
        return FALSE;
    }

    CSmsUser *poUser = itr->second;
    SDStrcpy(pszMobile, poUser->GetMobile());
    if (0 == SDStrcmp(poUser->GetVerCode(), pszVerCode))
    {
        poUser->GetRecvMsgTime() = SDTimeSecs() - MAX_MOBILE_TIME_OUT;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    return FALSE;
}

BOOL	CSmsUserMgr::GetMobile(UINT32 dwUserID, em_OptType emOpt, TCHAR* pszMobile)
{
    CSDMutexLock lock(m_oMutex);
    CSmsID2UserMapItr itr = m_mapSmsID2User.find(Get3232Key(dwUserID, emOpt));
    if(itr == m_mapSmsID2User.end())
    {
        return FALSE;
    }
    CSmsUser *poUser = itr->second;
    SDStrcpy(pszMobile, poUser->GetMobile());
    return TRUE;
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



