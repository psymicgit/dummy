
#include "buffmgr.h"
#include <sdstring.h>

UINT32 CBuffMgr::m_dwBuffIDGen = 0;

IMPLEMENT_SINGLETON(CBuffMgr)

CBuffMgr::CBuffMgr()
{
	
}

CBuffMgr::~CBuffMgr()
{
	for(CID2BuffMapItr itr = m_mapID2Buff.begin(); itr != m_mapID2Buff.end(); itr++)
	{
		CBuff* poBuff = itr->second;
		m_oBuffPool.Free(poBuff);
		poBuff = NULL;

	}
	m_mapID2Buff.clear();

}

BOOL CBuffMgr::Init()
{
    //¶ÁÈ¡ÅäÖÃÎÄ¼þ

	return TRUE;

}


VOID CBuffMgr::UnInit()
{
	
}

CBuff* CBuffMgr::CreateBuff()
{

	CBuff* poBuff = m_oBuffPool.Alloc();
	if(NULL == poBuff)
	{
		return NULL;
	}
	UINT32 dwBuffID = ++m_dwBuffIDGen;
	poBuff->SetMemID(dwBuffID);
	m_mapID2Buff[dwBuffID] = poBuff;

	return poBuff;
}


VOID CBuffMgr::RemoveBuff(UINT32 dwBuffID)
{
	CID2BuffMapItr itr = m_mapID2Buff.find(dwBuffID);
	if (m_mapID2Buff.end() != itr)
	{
		m_oBuffPool.Free(itr->second);
		m_mapID2Buff.erase(itr);
	}
}

