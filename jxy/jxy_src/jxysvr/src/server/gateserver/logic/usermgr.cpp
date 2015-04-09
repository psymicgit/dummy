
#include "usermgr.h"
#include <sdloggerimpl.h>
#include <sdutil.h>
using namespace SGDP;


//UINT32 CUserMgr::m_dwUserSessionIDGen = 0;


CUserMgr::CUserMgr():m_oUserPool(MAX_USER)
{
}

CUserMgr::~CUserMgr()
{

}


CUser* CUserMgr::CreateUser(TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT16 wZoneID, UINT16 wOriZoneID, UINT32 dwUserID, UINT32 dwPlayerID, UINT16 wCareerID, UINT8 byAuthType, CGTClient* poGTClient, UINT8 byDaiChong)
{
	if(0 == dwUserID)
	{
		return NULL;
	}

	CSessionID2UserMapItr itr = m_mapSessionID2User.find(dwPlayerID);
	CUser* poUser = NULL;
	//用户登录过，释放之前的连接，重置新连接
	if(itr != m_mapSessionID2User.end())
	{		
		poUser = itr->second;
		poUser->SetUserProp(pszUserName, pszDeviceID, pszNotifyID, wZoneID, wOriZoneID, dwUserID, dwPlayerID, wCareerID, byAuthType, poGTClient, byDaiChong);
		return poUser;
	}

	poUser = m_oUserPool.Alloc();
	poUser->SetUserProp(pszUserName, pszDeviceID, pszNotifyID, wZoneID, wOriZoneID, dwUserID, dwPlayerID, wCareerID, byAuthType, poGTClient, byDaiChong);
	m_mapSessionID2User[poUser->GetUserSessionID()] = poUser;
	m_mapPlayerID2User[dwPlayerID] = poUser;

	return poUser;
}


VOID CUserMgr::ReleaseUser(CUser* poUser)
{
	if(NULL == poUser)
	{
		return;
	}
	CSessionID2UserMapItr itrSessionID = m_mapSessionID2User.find(poUser->GetUserSessionID());
	if(itrSessionID != m_mapSessionID2User.end())
	{
		m_mapSessionID2User.erase(itrSessionID);
	}
	m_mapPlayerID2User.erase(poUser->GetPlayerID());

	m_oUserPool.Free(poUser);
	poUser = NULL;
}


CUser* CUserMgr::FindUser(UINT32 dwUserSessionID)
{
	CSessionID2UserMapItr itrSessionID = m_mapSessionID2User.find(dwUserSessionID);
	if(itrSessionID != m_mapSessionID2User.end())
	{
		return itrSessionID->second;
	}

	return NULL;
}



CUser* CUserMgr::FindUserByPlayerID(UINT32 dwPlayerID)
{
	CPlayerID2UserMapItr itr = m_mapPlayerID2User.find(dwPlayerID);
	if(itr != m_mapPlayerID2User.end())
	{
		return itr->second;
	}

	return NULL;
}

VOID CUserMgr::AddKickOutPlayer(UINT32 dwPlayerID)
{
	m_mapPlayerID2Time[dwPlayerID] = SDTimeSecs();
}

BOOL CUserMgr::BeKickOut(UINT32 dwPlayerID)
{
	CPlayerID2TimeMapItr itr = m_mapPlayerID2Time.find(dwPlayerID);
	if(itr == m_mapPlayerID2Time.end())
	{
		return FALSE;
	}

	if(SDTimeSecs() - itr->second >= 3600)
	{
		m_mapPlayerID2Time.erase(dwPlayerID);
		return FALSE;
	}

	return TRUE;
}
