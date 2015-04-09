#include "dbsessionmgr.h"
#include "sdframework/sdpipechannel.h"

	/*
	用于db执行回复消息时查找session的管理，防止回复使用session时该session已经释放
	*/

	CDbSessionMgr::~CDbSessionMgr()
	{

	}

	CDbSessionMgr::CDbSessionMgr()
	{
		m_oIDSessionPool.Init(MAX_SESSION_NUM);	
	}

	UINT32 CDbSessionMgr::AddSession(CSDPipeChannel* poSession)
	{
		UINT32 dwSessionId = m_oIDSessionPool.Add(poSession);
		if((UINT32)(-1) == dwSessionId)
		{
			//log
			return dwSessionId;
		}

		return dwSessionId;
	}

	VOID CDbSessionMgr::DelSession(UINT32 dwSessionId)
	{
		if((UINT32)(-1) != dwSessionId)
		{
			m_oIDSessionPool.Del(dwSessionId);	
		}
	}

	CSDPipeChannel* CDbSessionMgr::FindSession(UINT32 dwSessionId)
	{	
		CSDPipeChannel* poSession = NULL;
		if(m_oIDSessionPool.Find(dwSessionId,poSession))
		{
			return poSession;
		}
		//log
		return NULL;
	}


