#include "luaactivity.h"
#include <logic/lua/luamgr.h>
#include "dll/sdframework/sdloggerimpl.h"
IMPLEMENT_CLASS_POOL_MGR(CLuaActivity)

CLuaActivity::CLuaActivity()
{

}

CLuaActivity::~CLuaActivity()
{

} 


BOOL CLuaActivity::Init(const DT_LUAACTIVITY_RECORD_LIST_DATA& stDT_LUAACTIVITY_RECORD_LIST_DATA, CPlayer* poOwner)
{
	for(UINT8 byIdx = 0; byIdx < stDT_LUAACTIVITY_RECORD_LIST_DATA.byRecordNum; byIdx++)
	{
		m_mapActivityEndTimeRecord[stDT_LUAACTIVITY_RECORD_LIST_DATA.astHaveFinishActivityInfo[byIdx].dwID] = stDT_LUAACTIVITY_RECORD_LIST_DATA.astHaveFinishActivityInfo[byIdx];
	}

	return TRUE;
}


VOID CLuaActivity::GetDT_ACTIVITY_RECORD_DATA(DT_LUAACTIVITY_RECORD_LIST_DATA& stDT_LUAACTIVITY_RECORD_LIST_DATA)
{
	stDT_LUAACTIVITY_RECORD_LIST_DATA.byRecordNum = 0;
	for(CDT_LUAACTIVITY_RECORD_DATAMapItr itr = m_mapActivityEndTimeRecord.begin(); ((itr != m_mapActivityEndTimeRecord.end()) && (stDT_LUAACTIVITY_RECORD_LIST_DATA.byRecordNum <= MAX_LUAACTIVITY_RECORD_NUM)); itr++)
	{
		memcpy(&(stDT_LUAACTIVITY_RECORD_LIST_DATA.astHaveFinishActivityInfo[stDT_LUAACTIVITY_RECORD_LIST_DATA.byRecordNum++]), &itr->first, sizeof(DT_LUAACTIVITY_RECORD_DATA));
	}	
}

UINT16 CLuaActivity::CkActivityHaveDoNum(UINT32 dwId)
{
	CDT_LUAACTIVITY_RECORD_DATAMapItr itr = m_mapActivityEndTimeRecord.find(dwId);
	if(itr != m_mapActivityEndTimeRecord.end())
	{
		return itr->second.wFinishNum;
	}

	return 0;
}

VOID CLuaActivity::RecordDo(UINT32 dwId)
{
	CDT_LUAACTIVITY_RECORD_DATAMapItr itr = m_mapActivityEndTimeRecord.find(dwId);
	if(itr != m_mapActivityEndTimeRecord.end())
	{
		itr->second.wFinishNum++;
		itr->second.qwFinishTime = SDTimeSecs();
	}
	else
	{
		if(m_mapActivityEndTimeRecord.size() >= MAX_LUAACTIVITY_RECORD_NUM)
		{
			for(itr = m_mapActivityEndTimeRecord.begin(); itr != m_mapActivityEndTimeRecord.end(); itr++)
			{
				if(!CLuamgr::Instance()->CkHaveActivity(itr->first))
				{
					m_mapActivityEndTimeRecord.erase(itr);
					break;
				}
			}
		}

		DT_LUAACTIVITY_RECORD_DATA stDT_LUAACTIVITY_RECORD_DATA = {0};
		stDT_LUAACTIVITY_RECORD_DATA.dwID = dwId;
		stDT_LUAACTIVITY_RECORD_DATA.wFinishNum = 1;
		stDT_LUAACTIVITY_RECORD_DATA.qwFinishTime = SDTimeSecs();
		m_mapActivityEndTimeRecord[dwId] = stDT_LUAACTIVITY_RECORD_DATA;
	}

}

BOOL CLuaActivity::ActivityEncourage(UINT32 dwId, EBurstKind eEncKind, UINT32 dwValue1, UINT32 dwValue2)
{
	return TRUE;
}

//UINT16 CLuaActivity::CkActivityHaveDoNum(UINT32 dwId)
//{
//	return m_poLuaActivity->CkActivityHaveDoNum(dwId);
//}
//
//BOOL CLuaActivity::ActivityEncourage(UINT32 dwId, EBurstKind eEncKind, UINT32 dwValue1, UINT32 dwValue2)
//{
//	return m_poLuaActivity->ActivityEncourage(dwId, eEncKind, dwValue1, dwValue2);
//}
