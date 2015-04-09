#include "usermgr.h"


IMPLEMENT_SINGLETON(CUserMgr)
CUserMgr::CUserMgr(): m_oUserPool(MAX_USER)
{

}

CUserMgr::~CUserMgr()
{

}


BOOL CUserMgr::Init()
{
    return TRUE;
}

UINT64 Get3232Key(UINT32 dwValue1,UINT32 dwValue2)
{
	UINT64 qwKey = dwValue1;
	qwKey = qwKey <<32;
	qwKey += dwValue2;

	return qwKey;
}



CUser* CUserMgr::CreateUser(UINT16 wZoneID, UINT32 dwTransID, PKT_LSTLC_AUTH_REQ& stReq)
{
    CSDMutexLock lock(m_oMutex);
    CUser* poUser = FindUser(wZoneID, dwTransID);
    if(poUser)
    {        
        return poUser;
    }
    poUser = m_oUserPool.Alloc();
    poUser->SetUserProp(wZoneID, dwTransID, stReq);
    m_mapID2User[Get3232Key(wZoneID, dwTransID)] = poUser;

    return poUser;
}


VOID CUserMgr::ReleaseUser(CUser* poUser)
{
    if(NULL == poUser)
    {
        return;
    }
	m_mapID2User.erase(poUser->GetTransID());
    m_oUserPool.Free(poUser);
    poUser = NULL;
}



CUser* CUserMgr::FindUser(UINT16 wZoneID, UINT32 dwTransID)
{
	CSessionID2UserMapItr itr;
	itr = m_mapID2User.find(Get3232Key(wZoneID, dwTransID));
	if(itr != m_mapID2User.end())
	{
		return itr->second;
	}
	return NULL;
}


CAuthClient* SDAPI CUserMgr::FindClient(UINT16 wZoneID, UINT32 dwTransID)
{
	CSessionID2UserMapItr itr = m_mapID2User.find(Get3232Key(wZoneID, dwTransID));
	if(itr != m_mapID2User.end())
	{
		return itr->second;
	}
	return NULL;
}


VOID CUserMgr::Run()
{
	//每3秒检测一次
	if(SDTimeSecs() > 3)
	{
		CkUserTimeOut();
	}
	
}

VOID CUserMgr::CkUserTimeOut()
{
	m_qwLastCkTime = SDTimeSecs();
	for(CSessionID2UserMapItr itr = m_mapID2User.begin(); itr != m_mapID2User.end(); )
	{
		CUser* poUser = itr->second;
		if((poUser) && (SDTimeSecs() - poUser->GetLastRecvMsgTime() > 120)) //2分钟没返回则清除
		{
			m_mapID2User.erase(itr++);
			m_oUserPool.Free(poUser);
		}
		else
		{
			itr++;
		}
	}
}

