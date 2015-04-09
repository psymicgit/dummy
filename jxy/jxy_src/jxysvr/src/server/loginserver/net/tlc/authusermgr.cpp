#include "authusermgr.h"
#include "common/server/auth.h"

IMPLEMENT_SINGLETON(CAuthUserMgr)
CAuthUserMgr::CAuthUserMgr(): m_oUserPool(MAX_USER)
{

}

CAuthUserMgr::~CAuthUserMgr()
{

}


BOOL CAuthUserMgr::Init()
{
    return TRUE;
}

VOID CAuthUserMgr::ReqAuth(UINT32 dwTransID, PKT_GTLS_LOGIN_REQ* pstReq1, PKT_GTLS_LOGIN_EX_REQ* pstReq2)
{
	if((NULL == pstReq1) && (NULL == pstReq2))
	{
		return;
	}
	CAuthUser* poUser = CreateUser(dwTransID, pstReq1, pstReq2);
	if(NULL == poUser)
	{
		return ;
	}
	poUser->ReqAuth();
}


VOID CAuthUserMgr::OnAuthAck(UINT32 dwTransID, UINT16 wErrCode)
{
	CAuthUser* poUser = FindUser(dwTransID);
	if(NULL == poUser)
	{
		return ;
	}
	poUser->OnAuthAck(wErrCode);
	ReleaseUser(poUser);
	poUser = NULL;
}



CAuthUser* CAuthUserMgr::CreateUser(UINT32 dwTransID, PKT_GTLS_LOGIN_REQ* pstReq1, PKT_GTLS_LOGIN_EX_REQ* pstReq2)
{
    CSDMutexLock lock(m_oMutex);
	CAuthUser* poUser = FindUser(dwTransID);
	if(poUser)
	{
		return poUser;
	}
	poUser = m_oUserPool.Alloc();	
    poUser->SetUserProp(dwTransID, pstReq1, pstReq2);
    m_mapID2User[dwTransID] = poUser;

    return poUser;
}


VOID CAuthUserMgr::ReleaseUser(CAuthUser* poUser)
{
    if(NULL == poUser)
    {
        return;
    }
	m_mapID2User.erase(poUser->GetTransID());
    m_oUserPool.Free(poUser);
    poUser = NULL;
}



CAuthUser* CAuthUserMgr::FindUser(UINT32 dwTransID)
{
	CSessionID2AuthUserMapItr itr = m_mapID2User.find(dwTransID);
	if(itr != m_mapID2User.end())
	{
		return itr->second;
	}
	return NULL;
}

VOID CAuthUserMgr::Run()
{
	//每3秒检测一次
	if(SDTimeSecs() > 3)
	{
		CkUserTimeOut();
	}
	
}

VOID CAuthUserMgr::CkUserTimeOut()
{
	m_qwLastCkTime = SDTimeSecs();
	for(CSessionID2AuthUserMapItr itr = m_mapID2User.begin(); itr != m_mapID2User.end(); )
	{
		CAuthUser* poUser = itr->second;
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




