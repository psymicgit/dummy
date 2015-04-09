
#include "yabiaobattlelog.h"
#include "dll/sdframework/sdloggerimpl.h"
IMPLEMENT_CLASS_POOL_MGR(CYabiaoBattleLog)
CYabiaoBattleLog::CYabiaoBattleLog()
{

}

CYabiaoBattleLog::~CYabiaoBattleLog()
{

}

VOID CYabiaoBattleLog::InitAttach()
{

}


BOOL CYabiaoBattleLog::Init(DT_YABIAO_BATTLELOG_DATA stDT_PAOSHANG_BATTLELOG_DATA, CPlayer* poOwner)
{
	if(NULL == poOwner)
	{
		return FALSE;
	}
	memcpy(&m_stDT_PAOSHANG_BATTLELOG_DATA, &stDT_PAOSHANG_BATTLELOG_DATA, sizeof(m_stDT_PAOSHANG_BATTLELOG_DATA));

	m_poOwner = poOwner;

	return TRUE;
}

DT_YABIAO_BATTLELOG_DATA &CYabiaoBattleLog::GetDT_PAOSHANG_BATTLELOG_DATA()
{ 
	return m_stDT_PAOSHANG_BATTLELOG_DATA; 
}