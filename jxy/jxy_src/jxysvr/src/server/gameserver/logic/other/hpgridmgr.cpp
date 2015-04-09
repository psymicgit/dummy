#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <logic/other/singleprammgr.h>
#include <sdloggerimpl.h>
#include "hpgridmgr.h"
#include <framework/gsapi.h>
#include <db/dbmgr.h>

IMPLEMENT_SINGLETON_PROPMGR(CHPGridMgr);

CHPGridMgr::CHPGridMgr()
{

}

CHPGridMgr::~CHPGridMgr()
{
}

BOOL CHPGridMgr::Init()
{
	m_byMaxGridNum = CSinglePramMgr::Instance()->GetMaxHPGridNum();
	m_wMaxPlayerLevel = CSinglePramMgr::Instance()->GetMaxPlayerLevel();
    return LoadFromDB();
}

VOID CHPGridMgr::UnInit()
{
	m_mapLevel2PerGridHP.clear();	

}


// 从数据源获取数据
BOOL CHPGridMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("hpgridprop", " order by Level").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
    UINT16 wLastLevel = 0;
    while(pRes->GetRecord())// 当有行结果时
    {
        UINT16 wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
		if(wLevel != ++wLastLevel)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: level no continuation, level[%d]!"), MSG_MARK, wLevel);
			return FALSE;
		}
        UINT32 dwHPPerGrid = SGDP::SDAtou(pRes->GetFieldValueByName("HPPerGrid"));
		UINT32 dwGodweaponHPperGrid = SGDP::SDAtou(pRes->GetFieldValueByName("GodweaponHPperGrid"));
		m_mapGodweaponQuality2PerGridHP[wLevel] = dwGodweaponHPperGrid;
		m_mapLevel2PerGridHP[wLevel] = dwHPPerGrid;

		wLastLevel = wLevel;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

	if(m_mapLevel2PerGridHP.size() != m_wMaxPlayerLevel)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: row not match[%d]!"), MSG_MARK, m_wMaxPlayerLevel);
		return FALSE;
	}
    return TRUE;
}

UINT8 CHPGridMgr::GetHPGridNum(UINT16 wMaxLevel, UINT32 dwHPPerGrid, UINT32 dwHP)
{
	if(0 == wMaxLevel)
	{
		return 1;
	}
	else if(wMaxLevel > m_wMaxPlayerLevel)
	{
		return m_byMaxGridNum;
	}

	UINT8 byGridNum = UINT8(dwHP / dwHPPerGrid);
	UINT32 dwOddHP = dwHP % dwHPPerGrid;
	if(0 != dwOddHP)
	{
		byGridNum++;
	}

	if(0 == byGridNum)
	{
		byGridNum++;
	}
	else if(byGridNum > m_byMaxGridNum)
	{
		return m_byMaxGridNum;
	}

	return byGridNum;
}

VOID CHPGridMgr::SetHPGridNum(DT_BATTLE_OBJ_GROUP_DATA3& stMyselfInfo, DT_BATTLE_OBJ_GROUP_DATA3& stEnemyInfo, map<UINT16, UINT16>& mapBossMonsterID)
{
	UINT16 wMaxLevel = 0;
	UINT32 dwMaxHP = 0;
	for(UINT8 byIdx = 0; byIdx < stMyselfInfo.byObjNum; byIdx++)
	{
		DT_BATTLE_OBJ_DATA2& stObjData = stMyselfInfo.astBattleObjInfo[byIdx];
		if(stObjData.wLevel > wMaxLevel)
		{
			wMaxLevel = stObjData.wLevel;
		}
		if(stObjData.stBattleAttr.dwHP > dwMaxHP)
		{
			dwMaxHP = stObjData.stBattleAttr.dwHP;
		}
	}

	for(UINT8 byIdx = 0; byIdx < stEnemyInfo.byObjNum; byIdx++)
	{
		DT_BATTLE_OBJ_DATA2& stObjData = stEnemyInfo.astBattleObjInfo[byIdx];
		if(mapBossMonsterID.find(stObjData.wKindID) != mapBossMonsterID.end())//只选择普通怪最大等级与血量
		{
			continue;
		}
		if(stObjData.wLevel > wMaxLevel)
		{
			wMaxLevel = stObjData.wLevel;
		}
		if(stObjData.stBattleAttr.dwHP > dwMaxHP)
		{
			dwMaxHP = stObjData.stBattleAttr.dwHP;
		}
	}
	wMaxLevel = wMaxLevel > m_wMaxPlayerLevel ? m_wMaxPlayerLevel : wMaxLevel;//不能超过人物最高等级
	UINT32 dwHPPerGrid = 0;
	try
	{
		dwHPPerGrid = m_mapLevel2PerGridHP[wMaxLevel];
		if(dwMaxHP / dwHPPerGrid > m_byMaxGridNum)
		{
			dwHPPerGrid = dwMaxHP / m_byMaxGridNum;
		}
	}
	catch ( ... )
	{
		dwHPPerGrid = 0;
	}

	

	for(UINT8 byIdx = 0; byIdx < stMyselfInfo.byObjNum; byIdx++)
	{
		DT_BATTLE_OBJ_DATA2& stObjData = stMyselfInfo.astBattleObjInfo[byIdx];
		stObjData.byHPGridNum = GetHPGridNum(wMaxLevel, dwHPPerGrid, stObjData.stBattleAttr.dwHP);
	}

	for(UINT8 byIdx = 0; byIdx < stEnemyInfo.byObjNum; byIdx++)
	{
		DT_BATTLE_OBJ_DATA2& stObjData = stEnemyInfo.astBattleObjInfo[byIdx];
		stObjData.byHPGridNum = GetHPGridNum(wMaxLevel, dwHPPerGrid, stObjData.stBattleAttr.dwHP);
	}

}


VOID CHPGridMgr::SetGodweaponHPGridNum(DT_BATTLE_OBJ_GROUP_DATA2& stMyselfInfo, DT_BATTLE_OBJ_GROUP_DATA2& stEnemyInfo, map<UINT16, UINT16>& mapBossMonsterID)
{
	UINT16 wMaxLevel = 1;
	UINT32 dwMaxHP = 1;
	for(UINT8 byIdx = 0; byIdx < stMyselfInfo.byObjNum; byIdx++)
	{
		DT_GODWEAPON_BATTLE_DATA& stGodweaponInfo = stMyselfInfo.astBattleObjInfo[byIdx].stGodweaponInfo;
		if(0 == stGodweaponInfo.wQuality)
		{
			continue;
		}
		if(stGodweaponInfo.wQuality > wMaxLevel)
		{
			wMaxLevel = stGodweaponInfo.wQuality;
		}
		if(stGodweaponInfo.stBattleAttr.dwHP > dwMaxHP)
		{
			dwMaxHP = stGodweaponInfo.stBattleAttr.dwHP;
		}
	}

	for(UINT8 byIdx = 0; byIdx < stEnemyInfo.byObjNum; byIdx++)
	{
		DT_GODWEAPON_BATTLE_DATA& stGodweaponInfo = stMyselfInfo.astBattleObjInfo[byIdx].stGodweaponInfo;
		if(0 == stGodweaponInfo.wQuality)
		{
			continue;
		}
		if(stGodweaponInfo.wQuality > wMaxLevel)
		{
			wMaxLevel = stGodweaponInfo.wQuality;
		}
		if(stGodweaponInfo.stBattleAttr.dwHP > dwMaxHP)
		{
			dwMaxHP = stGodweaponInfo.stBattleAttr.dwHP;
		}
	}
	wMaxLevel = wMaxLevel > m_wMaxPlayerLevel ? m_wMaxPlayerLevel : wMaxLevel;

	UINT32 dwHPPerGrid = m_mapGodweaponQuality2PerGridHP[wMaxLevel];
	dwMaxHP = dwMaxHP == 0 ? 1 : dwMaxHP;
	dwHPPerGrid = dwHPPerGrid == 0 ? dwMaxHP : dwHPPerGrid;
	if(dwMaxHP / dwHPPerGrid > m_byMaxGridNum)
	{
		dwHPPerGrid = dwMaxHP / m_byMaxGridNum;
	}

	for(UINT8 byIdx = 0; byIdx < stMyselfInfo.byObjNum; byIdx++)
	{
		DT_GODWEAPON_BATTLE_DATA& stGodweaponInfo = stMyselfInfo.astBattleObjInfo[byIdx].stGodweaponInfo;
		if(0 == stGodweaponInfo.wQuality)
		{
			stGodweaponInfo.byHPGridNum = 1;
		}
		else
		{
			stGodweaponInfo.byHPGridNum = GetHPGridNum(wMaxLevel, dwHPPerGrid, stGodweaponInfo.stBattleAttr.dwHP);
		}		
	}

	for(UINT8 byIdx = 0; byIdx < stEnemyInfo.byObjNum; byIdx++)
	{
		DT_GODWEAPON_BATTLE_DATA& stGodweaponInfo = stEnemyInfo.astBattleObjInfo[byIdx].stGodweaponInfo;
		if(0 == stGodweaponInfo.wQuality)
		{
			stGodweaponInfo.byHPGridNum = 1;
		}
		else
		{
			stGodweaponInfo.byHPGridNum = GetHPGridNum(wMaxLevel, dwHPPerGrid, stGodweaponInfo.stBattleAttr.dwHP);
		}		
	}
}

