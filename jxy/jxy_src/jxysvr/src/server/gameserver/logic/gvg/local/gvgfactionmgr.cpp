#include "gvgfactionmgr.h"
#include "db/dbmgr.h"
#include "framework/gsapi.h"
#include "gvgbuild.h"

IMPLEMENT_SINGLETON(CGvGFactionMgr)

BOOL CGvGFactionMgr::Init()
{
	if ( !LoadFromDB())
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CGvGFactionMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}

	CHAR szSql[1024] = {0};
	sprintf(szSql, "select * from gvgfaction order by FactionID ;");


	SGDP::ISDDBRecordSet* pRecordSet = NULL;
	string strError;
	INT32 nRet = pDBSession->ExecuteSqlRs(szSql, &pRecordSet, 0, &strError );
	if(SDDB_HAS_RECORDSET == nRet)
	{
		while (pRecordSet->GetRecord())
		{
			DT_GVG_FACTION stGvGFaction = {0};
			stGvGFaction.stBaseData.dwFactionID = SDAtou(pRecordSet->GetFieldValueByName("FactionID"));
			stGvGFaction.wZoneID = SDAtou(pRecordSet->GetFieldValueByName("ZoneID"));
			stGvGFaction.stBaseData.byIconID = SDAtou(pRecordSet->GetFieldValueByName("IconID"));
			SDStrncpy(stGvGFaction.stBaseData.aszFactionName, pRecordSet->GetFieldValueByName("FactionDispName"), MAX_FACTION_NUM - 1);
			SDStrncpy(stGvGFaction.stBaseData.aszHeadName, pRecordSet->GetFieldValueByName("HeadDispName"), USERNAME_LEN - 1);
			stGvGFaction.byJoinCnt = SDAtou(pRecordSet->GetFieldValueByName("JoinPlayerNum"));
			stGvGFaction.qwSumPower = SDAtou64(pRecordSet->GetFieldValueByName("SumPower"));
			stGvGFaction.wScore = SDAtou(pRecordSet->GetFieldValueByName("SumGameIntegral"));
			stGvGFaction.dwGambleCnt = SDAtou(pRecordSet->GetFieldValueByName("GamblePlayerNum"));

			m_mapGvGFaction[stGvGFaction.stBaseData.dwFactionID] = stGvGFaction;
		}
	}

	if ( pRecordSet)
	{
		pRecordSet->Release();
		pRecordSet = NULL;
	}
	return TRUE;

}

BOOL CGvGFactionMgr::GetFactionGambleInfo(UINT32 dwFactionID, DT_FACTION_GAMBLE_INFO& stInfo)
{
	CMapGvGFactionItr itr = m_mapGvGFaction.find(dwFactionID);
	if ( itr != m_mapGvGFaction.end() )
	{
		SDStrncpy(stInfo.aszFactionName, itr->second.stBaseData.aszFactionName, MAX_FACTION_NUM - 1);
		SDStrncpy(stInfo.aszHeadName, itr->second.stBaseData.aszHeadName, USERNAME_LEN - 1);
		stInfo.byIconID = itr->second.stBaseData.byIconID;
		stInfo.dwFactionID = itr->second.stBaseData.dwFactionID;
		stInfo.dwSumIntegral = itr->second.wScore;
		stInfo.dwSupportNum = itr->second.dwGambleCnt;
		stInfo.wZoneID = itr->second.wZoneID;
		stInfo.qwSumPower = itr->second.qwSumPower;
		stInfo.wJoinNum = itr->second.byJoinCnt;
		return TRUE;
	}
	return FALSE;
}

BOOL CGvGFactionMgr::GetFactionInfo(UINT32 dwFactionID, DT_GVG_FACTION& stFaction)
{
    memset(&stFaction, 0, sizeof(stFaction));

	CMapGvGFactionItr itr = m_mapGvGFaction.find(dwFactionID);
	if ( itr == m_mapGvGFaction.end() )
	{
		return FALSE;
	}
	stFaction = itr->second;
	return TRUE;
}

DT_GVG_FACTION* CGvGFactionMgr::FindFactionInfo(UINT32 dwFactionID)
{
    CMapGvGFactionItr itr = m_mapGvGFaction.find(dwFactionID);
    if (itr == m_mapGvGFaction.end())
    {
        return NULL;
    }

    DT_GVG_FACTION* pstFaction = &itr->second;
    return pstFaction;
}

void CGvGFactionMgr::AddGvGFaction(DT_GVG_FACTION& stFaction)
{
    if (0 == stFaction.stBaseData.dwFactionID)
    {
        return;
    }

    m_mapGvGFaction[stFaction.stBaseData.dwFactionID] = stFaction;
}