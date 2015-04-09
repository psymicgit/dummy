#include "instancepropmgr.h"
#include <sdfile.h>
#include <sddebug.h>
#include <common/server//utility.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/monster/monstermgr.h>
#include <logic/item/goods/goodspropmgr.h>
#include <logic/item/itemcomposepropmgr.h>
#include <logic/hero/heropropmgr.h>
#include <logic/monster/monsterpropmgr.h>
#include <logic/other/singleprammgr.h>
#include <framework/gsapi.h>
#include "logic/base/stringtool.h"
#include <db/dbmgr.h>

#include <sdxmlfile.h>

using namespace SGDP;

//用于全局保存精英副本信息
static CUnlockEliteInstanceIDMap		s_mapUnlockEliteInstanceID;


IMPLEMENT_SINGLETON_PROPMGR(CInstancePropMgr);

CInstancePropMgr::CInstancePropMgr()
{
    memset(&m_stInstanceIDTmp, 0, sizeof(m_stInstanceIDTmp));
    memset(&m_stLastEliteInstanceID, 0, sizeof(m_stLastEliteInstanceID));
    m_byLastCarryNum = 1;//默认1个，主将就1个了
    m_pstLastInstanceProp = NULL;
    m_byCurHeroIndex = 0;
	m_byRoundNum = 0;
	m_byPercentageHp = 0;
}

CInstancePropMgr::~CInstancePropMgr()
{

}

BOOL CInstancePropMgr::Init()
{
	s_mapUnlockEliteInstanceID.clear();
	CMonsterMgr::Instance()->RemoveAllMonster();//为了Reload方法及能重新构造monster

    if(!LoadFromXML())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CInstancePropMgr::Init failed!"), MSG_MARK);
        return FALSE;
    }
	if ( !LoadOutBound() 
		|| !LoadNeiDanInstance())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CInstancePropMgr::Init failed!"), MSG_MARK);
		return FALSE;
	}
	
    return TRUE;
}

VOID CInstancePropMgr::UnInit()
{
    for(CIndex2SceneTownPropMapItr itrScence = m_mapIndex2SceneTownProp.begin(); itrScence != m_mapIndex2SceneTownProp.end(); itrScence++)
    {
        SDDelete itrScence->second;
        itrScence->second = NULL;
    }
    m_mapIndex2SceneTownProp.clear();

    m_mapUnlockBuildKind2InstanceID.clear();
    m_mapInstanceID2HeroUnlockProp.clear();
    m_mapBurstItemKind2InstanceID.clear();
    m_mapUnlockSKillID2InstanceID.clear();

    memset(&m_stInstanceIDTmp, 0, sizeof(m_stInstanceIDTmp));
    memset(&m_stLastEliteInstanceID, 0, sizeof(m_stLastEliteInstanceID));
    m_byLastCarryNum = 1;//默认1个，主将就1个了
    m_pstLastInstanceProp = NULL;
    m_byCurHeroIndex = 0;

}


BOOL CInstancePropMgr::LoadFromXML()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("instancefile", " order by ScenceIdx, TownIdx").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    UINT16			wLastSceneIdx = 0;
    SSceneTownProp*	pstSceneTownProp = NULL;
    while(pRes->GetRecord())
    {
        UINT16 wSceneIdx = SGDP::SDAtou(pRes->GetFieldValueByName("ScenceIdx"));
        if(0 == wSceneIdx)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: ScenceIdx 0 invalid!"), MSG_MARK);
            return FALSE;
        }
        if(wSceneIdx != wLastSceneIdx)
        {
            pstSceneTownProp = SDNew SSceneTownProp;
            pstSceneTownProp->wSceneIndex = wSceneIdx;
            if(pstSceneTownProp)
            {
                m_mapIndex2SceneTownProp[pstSceneTownProp->wSceneIndex] = pstSceneTownProp;
            }
        }
        UINT8 byTownIdx = SGDP::SDAtou(pRes->GetFieldValueByName("TownIdx"));
        if(0 == byTownIdx)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: TownIdx 0 invalid!"), MSG_MARK, byTownIdx);
            return FALSE;
        }
        STownInstanceProp* pstTownInstanceProp = SDNew STownInstanceProp;
		
        pstTownInstanceProp->byTownIndex = byTownIdx;
        m_stInstanceIDTmp.wSceneIdx = pstSceneTownProp->wSceneIndex;
        m_stInstanceIDTmp.byTownIdx = pstTownInstanceProp->byTownIndex;
        pstTownInstanceProp->wOnhookLevel = SGDP::SDAtou(pRes->GetFieldValueByName("OnhookLevel"));
        if(!LoadFromData(pstTownInstanceProp, pRes->GetFieldValueByName("InstanceData"), pRes->GetFieldLengthByName("InstanceData")))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: LoadFromXML failed, ScenceIdx[%d] TownIdx[%d]!"), MSG_MARK, wSceneIdx, byTownIdx);
            SDDelete pstTownInstanceProp;
            pstTownInstanceProp = NULL;
            return FALSE;
        }
        pstSceneTownProp->m_mapTownInstance[pstTownInstanceProp->byTownIndex] = pstTownInstanceProp;

        wLastSceneIdx = pstSceneTownProp->wSceneIndex;
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;

}
BOOL CInstancePropMgr::ReadBuild(SInstanceProp* pstInstanceProp, const string& strJson){

	if(strJson == "" || "0" == strJson)
	{
		return TRUE;//不一定有建筑解锁配置
	}

	vector<UINT32> vecBuildID;
	StringTool::SDSplitStringToUInt(vecBuildID, strJson, ',');

	for (vector<UINT32>::iterator itr = vecBuildID.begin(); itr != vecBuildID.end(); itr++)
	{
		UINT8 byUnlockBuildKindID = *itr;

		if(0 == byUnlockBuildKindID)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: ReadBuild  UnlockBuildKindID invalid!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
				m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
			return FALSE;
		}
		pstInstanceProp->mapUnlockBuildKind[byUnlockBuildKindID] = byUnlockBuildKindID;

		if(m_mapUnlockBuildKind2InstanceID.find(byUnlockBuildKindID) != m_mapUnlockBuildKind2InstanceID.end())
		{
			SYS_CRITICAL(_SDT("[%s: %d]: build[%d] is reunlock!"), MSG_MARK, byUnlockBuildKindID);
			return FALSE;
		}
		m_mapUnlockBuildKind2InstanceID[byUnlockBuildKindID] = m_stInstanceIDTmp;

		pstInstanceProp->stInstanceOpenFunc.setOpenBuild.insert(byUnlockBuildKindID);
	}	

	return TRUE;
}
	//读取怪物
BOOL CInstancePropMgr::ReadMonster(SInstanceProp* pstInstanceProp, const string& strJson)
{
	if(strJson == "")
	{
		SYS_CRITICAL(_SDT("[%s: %d]: Not Monster Prop, InstanceIdx = %u"), MSG_MARK, pstInstanceProp->byIndex);
		return FALSE;
	}

	vector<UINT32> vecMonterID;
	StringTool::SDSplitStringToUInt(vecMonterID, strJson, ',');
	
	map<UINT8, UINT8> mapFormation;
	SWholeMonsterProp* pstWholeMonsterProp = SDNew SWholeMonsterProp();
	UINT8 byFormationIdx = 1;
	for (vector<UINT32>::iterator itr = vecMonterID.begin(); itr != vecMonterID.end() && byFormationIdx <= MAX_FORMATION_IDX_NUM; itr++, byFormationIdx++)
	{
		if ( 0 == *itr)
		{
			continue;
		}
		SSingleMonsterProp stProp;
		stProp.wMonsterID = *itr;
		stProp.byFormationIdx = byFormationIdx;

		if((mapFormation.find(stProp.byFormationIdx) != mapFormation.end()) || (stProp.byFormationIdx > MAX_FORMATION_IDX_NUM) || (0 == stProp.byFormationIdx))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: ReadMonster failed!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
				m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
			SDDelete pstWholeMonsterProp;
			return FALSE;
		}
		mapFormation[stProp.byFormationIdx] = stProp.byFormationIdx;

		pstWholeMonsterProp->vecMonster.push_back(stProp);
		
	}

	if(0 == pstWholeMonsterProp->vecMonster.size())
	{
		SDDelete pstWholeMonsterProp;
		pstWholeMonsterProp = NULL;
	}
	else
	{
		pstInstanceProp->vecWholeMonsterProp.push_back(pstWholeMonsterProp);
	}


	UINT8 bySize = pstInstanceProp->vecWholeMonsterProp.size();
	if(0 == bySize)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: No monster!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
			m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
		return FALSE;
	}
	SWholeMonsterProp* poMainMonster = pstInstanceProp->vecWholeMonsterProp[bySize - 1];
	if(NULL == poMainMonster)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: poMainMonster is NULL!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
			m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
		return FALSE;
	}
	bySize = poMainMonster->vecMonster.size();
	if(0 == bySize)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: poMainMonster prop invalid!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
			m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
		return FALSE;
	}
	pstInstanceProp->wMainMonsterKindID = poMainMonster->vecMonster[bySize - 1].wMonsterID;

	return TRUE;
}

BOOL CInstancePropMgr::ReadHero(SInstanceProp* pstInstanceProp, const string& strJson)
{
	//没有配置武将解锁信息时能插入记录
	if(strJson == "")
	{
		pstInstanceProp->stHeroUnlockProp.byCarryNum = m_byLastCarryNum;
		for (int i = 0; i < MAX_UNLOCK_HERO_NUM; i++)
		{
			pstInstanceProp->stHeroUnlockProp.wUnlockHeroKindID[i] = 0;
		}
		//方便在玩家每个副本时都能查找到武将解锁信息
		m_mapInstanceID2HeroUnlockProp[CLogicFunc::GetInstanceIdxKey(m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx)] = pstInstanceProp->stHeroUnlockProp;
		return TRUE;//不一定有武将解锁配置
	}

	pstInstanceProp->stHeroUnlockProp.byCarryNum = m_byLastCarryNum;

	vector<UINT32> vecHeroID;
	StringTool::SDSplitStringToUInt(vecHeroID, strJson, ',');
	UINT8 byHeroNum = 0;
	for ( vector<UINT32>::iterator itr = vecHeroID.begin(); itr != vecHeroID.end(); itr++)
	{
		UINT16 wUnlockHeroKindID = *itr;

		if(0 != wUnlockHeroKindID)
		{
			vector<UINT16>::iterator result = find(pstInstanceProp->stInstanceOpenFunc.vecOpenHero.begin(), pstInstanceProp->stInstanceOpenFunc.vecOpenHero.end(), wUnlockHeroKindID); //查找3
			if ( result == pstInstanceProp->stInstanceOpenFunc.vecOpenHero.end()) //没找到
			{
				pstInstanceProp->stInstanceOpenFunc.vecOpenHero.push_back(wUnlockHeroKindID);
			}
			else
			{
				SYS_CRITICAL(_SDT("[%s: %d]:KindID:%d"), MSG_MARK, wUnlockHeroKindID);
				return FALSE;
			}

			//处理武将出场次序
			SHeroProp* pHeroProp = CHeroPropMgr::Instance()->GetHeroProp(wUnlockHeroKindID);

			if( pHeroProp )
			{
				pHeroProp->byIndex = ++m_byCurHeroIndex;
			}
			else
			{
				SYS_CRITICAL(_SDT("[%s: %d]: pHeroProp is NULL! KindID:%d"), MSG_MARK, wUnlockHeroKindID );
			}
			pstInstanceProp->stHeroUnlockProp.wUnlockHeroKindID[byHeroNum] = wUnlockHeroKindID;
			byHeroNum++;
		}
		
	}

	//副本武将解锁信息,方便在玩家每个副本时都能查找到武将解锁信息
	m_mapInstanceID2HeroUnlockProp[CLogicFunc::GetInstanceIdxKey(m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx)] = pstInstanceProp->stHeroUnlockProp;
	return TRUE;
}

BOOL CInstancePropMgr::ReadEncourage(SInstanceProp* poInstanceProp,const string& strItemID, const string& strItemNum, const string& strItemRate, const string& strFirstFlag, const string& strDisPlayItem)
{
	if ( strItemID == "" || "" == strItemNum || "" == strItemRate || "" == strDisPlayItem)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: Parse Encourage ID Num Rate Zero"), MSG_MARK );
		return FALSE;
	}
	
	vector<UINT32> vecItemID;
	vector<UINT32> vecItemNum;
	vector<UINT32> vecItemRate;
	set<UINT16> setItemFirstFlag;
	StringTool::SDSplitStringToUInt(vecItemID, strItemID, ',');
	StringTool::SDSplitStringToUInt(vecItemNum, strItemNum, ',');
	StringTool::SDSplitStringToUInt(vecItemRate, strItemRate, ',');
	StringTool::SDSplitStringToUInt(poInstanceProp->vecDisplayItem, strDisPlayItem, ',');
	if ( "" != strFirstFlag)
	{
		StringTool::SDSplitStringToSet(setItemFirstFlag, strFirstFlag, ',');
	}

    for(UINT16 i = 0; i < poInstanceProp->vecDisplayItem.size(); i++){
        UINT16 wItemKindID = poInstanceProp->vecDisplayItem[i];
        m_mapDropItemID2InstanceList[wItemKindID].push_back(m_stInstanceIDTmp);
    }
	
	UINT32 dwSize = vecItemID.size();
	if ( dwSize != vecItemNum.size() || dwSize != vecItemRate.size())
	{
		return FALSE;
	}

	for ( UINT8 byIdx = 0; byIdx < dwSize; byIdx++)
	{
		UINT16 wItemID = vecItemID[byIdx];
		UINT16 wItemNum = vecItemNum[byIdx];
		UINT16 wRate = vecItemRate[byIdx];
		UINT8 byFirstFlag = 0;
		C16SetItr itr = setItemFirstFlag.find(wItemID);
		if ( itr != setItemFirstFlag.end())
		{
			byFirstFlag = 1;
		}
		
		UINT16 wFirstItemKindID = 0;
		UINT16 wFirstItemPileNum = 0;
		
		
		if((0 == wItemID) || (0 == wItemNum))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: Item Config failed"), MSG_MARK);
			return FALSE;
		}
		if(1 == byFirstFlag)
		{
			wFirstItemKindID = wItemID;
			wFirstItemPileNum = wItemNum;
		}
		
		
		
		SItemRate stItemRate;
		stItemRate.wItemNum = wItemNum;
		stItemRate.wRate = wRate * 100; //万分比
		SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wItemID);
		
		//内部宝箱全部拆开
		if((pstGoodsProp) && (EGMK_INNERBOX == pstGoodsProp->byMainKindID))
		{
			SItemComposeProp* pstItemComposeProp = CItemComposePropMgr::Instance()->GetProp(wItemID);
			if(NULL == pstItemComposeProp)
			{
				SYS_CRITICAL(_SDT("[%s: %d]: pstItemComposeProp is NULL,wKindID[%d]!"), MSG_MARK, wItemID);
				return FALSE;
			}
		
			CItemRateMap& mapComposeItem = pstItemComposeProp->mapComposeItem;
			if(0 == mapComposeItem.size())
			{
				SYS_CRITICAL(_SDT("[%s: %d]: mapComposeItem[%d] is 0!"), MSG_MARK, wItemID);
				return FALSE;
			}
			SItemGroupProp& stItemGroupProp = poInstanceProp->stPassBoxProp.mapItemGroup[byIdx + 1];
			for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end();  itr++)
			{
				SItemRate stRate = itr->second;
				stRate.wRate = stRate.wRate * wRate / 100;
				stRate.wRate = stRate.wRate == 0 ? 1 : stRate.wRate;
				stItemGroupProp.mapItem[itr->first] = stRate;
		
				m_mapBurstItemKind2InstanceID[itr->first] = m_stInstanceIDTmp;//只需记录最后产出该道具的，可直接覆盖
			}
			stItemGroupProp.wFirstItemKindID = wFirstItemKindID;
			stItemGroupProp.wFirstItemPileNum = wFirstItemPileNum;
		}
		else
		{
			SItemGroupProp& stItemGroupProp = poInstanceProp->stPassBoxProp.mapItemGroup[byIdx + 1];
			stItemGroupProp.mapItem[wItemID] = stItemRate;
			stItemGroupProp.wFirstItemKindID = wFirstItemKindID;
			stItemGroupProp.wFirstItemPileNum = wFirstItemPileNum;
		}
		
		if(poInstanceProp->stPassBoxProp.stDT_BURST_ITEM_DATA.byBurstItemNum < MAX_ENCOURAGE_ITEM_KIND_NUM)
		{
			poInstanceProp->stPassBoxProp.stDT_BURST_ITEM_DATA.awBurstItemList[poInstanceProp->stPassBoxProp.stDT_BURST_ITEM_DATA.byBurstItemNum++] = wItemID;
		}
		m_mapBurstItemKind2InstanceID[wItemID] = m_stInstanceIDTmp;//只需记录最后产出该道具的，可直接覆盖
	}

	return TRUE;
}
BOOL CInstancePropMgr::LoadNeiDanInstance()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("petneidaninstanceprop", " order by ScenceIdx, TownIdx, InstanceIdx").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
		return FALSE;
	}
	UINT16			wLastSceneIdx = 0;
	SSceneTownProp*	pstSceneTownProp = NULL;
	while(pRes->GetRecord())
	{
		UINT16 wScenceIdx = SDAtou(pRes->GetFieldValueByName("ScenceIdx"));
		SSceneTownProp* poSceneTown = NULL;
		CIndex2SceneTownPropMapItr sceneItr = m_mapIndex2SceneTownProp.find(wScenceIdx);
		if ( sceneItr == m_mapIndex2SceneTownProp.end() )
		{
			SSceneTownProp* poSceneTownTmp = SDNew SSceneTownProp;
			poSceneTownTmp->wSceneIndex = wScenceIdx;
			poSceneTownTmp->m_mapTownInstance.clear();
			m_mapIndex2SceneTownProp[wScenceIdx] = poSceneTownTmp;
			poSceneTown = poSceneTownTmp;
		}
		else
		{
			poSceneTown = sceneItr->second;
		}
		UINT8  byTownIdx = SDAtou(pRes->GetFieldValueByName("TownIdx"));
		CIndex2TownInstancePropMapItr TownItr = poSceneTown->m_mapTownInstance.find(byTownIdx);
		STownInstanceProp* poTownProp = NULL;
		if ( TownItr == poSceneTown->m_mapTownInstance.end())
		{
			STownInstanceProp* poTownPropTmp = SDNew STownInstanceProp;
			poTownPropTmp->byTownIndex = byTownIdx;
			poTownPropTmp->m_mapInstance.clear();
			poTownProp = poTownPropTmp;
			poSceneTown->m_mapTownInstance[byTownIdx] = poTownProp;
		}
		else
		{
			poTownProp = TownItr->second;
		}
		UINT8  byInstanceIdx = SDAtou(pRes->GetFieldValueByName("InstanceIdx"));
		SInstanceProp* poInstanceProp = SDNew SInstanceProp;
		poInstanceProp->byIndex = byInstanceIdx;
		m_stInstanceIDTmp.wSceneIdx = wScenceIdx;
		m_stInstanceIDTmp.byTownIdx = byTownIdx;
		m_stInstanceIDTmp.byInstanceIdx = byInstanceIdx;
		m_byMaxNeiDanInstanceIdx = m_byMaxNeiDanInstanceIdx < byInstanceIdx ? byInstanceIdx : m_byMaxNeiDanInstanceIdx;

		poInstanceProp->byPercentageHp = 80;
		poInstanceProp->byRoundNum = 4;
		poInstanceProp->strDispName = pRes->GetFieldValueByName("InstanceName");
		poInstanceProp->wEnterLevel = SDAtou(pRes->GetFieldValueByName("PlayerLevelReq"));
		poInstanceProp->strItemDesc = pRes->GetFieldValueByName("OutItemDesc");
		string strMonterID = pRes->GetFieldValueByName("MonterProp");
		if( !ReadMonster(poInstanceProp, strMonterID))
		{
			return FALSE;
		}

		string strItemID = pRes->GetFieldValueByName("ItemOutputID");
		string strItemNum = pRes->GetFieldValueByName("ItemOutputNum");
		string strItemRate = pRes->GetFieldValueByName("ItemOutputRate");
		string strFirstFlag = pRes->GetFieldValueByName("ItemFirstFlag");

		if ( !ReadEncourage(poInstanceProp, strItemID, strItemNum, strItemRate, strFirstFlag, strItemID) )
		{
			return FALSE;
		}


		if(FALSE == InitBoxBurstItem(poInstanceProp))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: InitBurstItem failed!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
				m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
			SDDelete poInstanceProp;
			poInstanceProp = NULL;
			return FALSE;
		}
		if(FALSE == InitBurstExp(poInstanceProp))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: InitBurstExp failed!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
				m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
			SDDelete poInstanceProp;
			poInstanceProp = NULL;
			return FALSE;
		}

		m_pstLastInstanceProp = poInstanceProp;
		poTownProp->m_mapInstance[poInstanceProp->byIndex] = poInstanceProp;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

BOOL CInstancePropMgr::LoadOutBound()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("outboundinstanceprop", " order by ScenceIdx, TownIdx, InstanceIdx").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
		return FALSE;
	}
	UINT16			wLastSceneIdx = 0;
	SSceneTownProp*	pstSceneTownProp = NULL;
	while(pRes->GetRecord())
	{
		UINT16 wScenceIdx = SDAtou(pRes->GetFieldValueByName("ScenceIdx"));
		SSceneTownProp* poSceneTown = NULL;
		CIndex2SceneTownPropMapItr sceneItr = m_mapIndex2SceneTownProp.find(wScenceIdx);
		if ( sceneItr == m_mapIndex2SceneTownProp.end() )
		{
			SSceneTownProp* poSceneTownTmp = SDNew SSceneTownProp;
			poSceneTownTmp->wSceneIndex = wScenceIdx;
			poSceneTownTmp->m_mapTownInstance.clear();
			m_mapIndex2SceneTownProp[wScenceIdx] = poSceneTownTmp;
			poSceneTown = poSceneTownTmp;
		}
		else
		{
			poSceneTown = sceneItr->second;
		}
		UINT8  byTownIdx = SDAtou(pRes->GetFieldValueByName("TownIdx"));
		CIndex2TownInstancePropMapItr TownItr = poSceneTown->m_mapTownInstance.find(byTownIdx);
		STownInstanceProp* poTownProp = NULL;
		if ( TownItr == poSceneTown->m_mapTownInstance.end())
		{
			STownInstanceProp* poTownPropTmp = SDNew STownInstanceProp;
			poTownPropTmp->byTownIndex = byTownIdx;
			poTownPropTmp->m_mapInstance.clear();
			poTownProp = poTownPropTmp;
			poSceneTown->m_mapTownInstance[byTownIdx] = poTownProp;
		}
		else
		{
			poTownProp = TownItr->second;
		}
		UINT8  byInstanceIdx = SDAtou(pRes->GetFieldValueByName("InstanceIdx"));
		SInstanceProp* poInstanceProp = SDNew SInstanceProp;
		poInstanceProp->byIndex = byInstanceIdx;
		m_stInstanceIDTmp.wSceneIdx = wScenceIdx;
		m_stInstanceIDTmp.byTownIdx = byTownIdx;
		m_stInstanceIDTmp.byInstanceIdx = byInstanceIdx;

		poInstanceProp->bIsMajor = SDAtou(pRes->GetFieldValueByName("Major"));
		poInstanceProp->byBattleNum = SDAtou(pRes->GetFieldValueByName("BattleNum"));
		poInstanceProp->byPercentageHp = 80;
		poInstanceProp->byRoundNum = 4;
		poInstanceProp->wEnterLevel = SDAtou(pRes->GetFieldValueByName("EnterLevel"));
		poInstanceProp->stParentInstance.wSceneIdx = SDAtou(pRes->GetFieldValueByName("ParentScenceIdx"));
		poInstanceProp->stParentInstance.byTownIdx = SDAtou(pRes->GetFieldValueByName("ParentTownIdx"));
		poInstanceProp->stParentInstance.byInstanceIdx = SDAtou(pRes->GetFieldValueByName("ParentInstanceIdx"));
		poInstanceProp->strInstanceDesc = pRes->GetFieldValueByName("InstanceDesc");
		poInstanceProp->strItemDesc = pRes->GetFieldValueByName("ItemOutPutDesc");
		poInstanceProp->strHeroDesc = pRes->GetFieldValueByName("HeroOutPutDesc");
		poInstanceProp->strLeveReqDesc = pRes->GetFieldValueByName("LevelReqDesc");
		poInstanceProp->strParentReqDesc = pRes->GetFieldValueByName("ParentReqDesc");
		string strMonterID = pRes->GetFieldValueByName("MonterProp");
		if( !ReadMonster(poInstanceProp, strMonterID))
		{
			return FALSE;
		}

		string strItemID = pRes->GetFieldValueByName("ItemOutputID");
		string strItemNum = pRes->GetFieldValueByName("ItemOutputNum");
		string strItemRate = pRes->GetFieldValueByName("ItemOutputRate");
		string strFirstFlag = pRes->GetFieldValueByName("ItemFirstFlag");
		string strDisPlay = pRes->GetFieldValueByName("DisPlayItemID");
		if ( !ReadEncourage(poInstanceProp, strItemID, strItemNum, strItemRate, strFirstFlag, strDisPlay) )
		{
			return FALSE;
		}

		string strHeroID = pRes->GetFieldValueByName("HeroUnlockProp");
		if( !ReadHero(poInstanceProp, strHeroID))
		{
			return FALSE;
		}
		string strBuildID = pRes->GetFieldValueByName("BuildUnlockProp");
		if ( !ReadBuild(poInstanceProp, strBuildID))
		{
			return FALSE;
		}

		if(FALSE == InitBoxBurstItem(poInstanceProp))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: InitBurstItem failed!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
				m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
			SDDelete poInstanceProp;
			poInstanceProp = NULL;
			return FALSE;
		}
		if(FALSE == InitBurstExp(poInstanceProp))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: InitBurstExp failed!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
				m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
			SDDelete poInstanceProp;
			poInstanceProp = NULL;
			return FALSE;
		}

		if( FALSE == InitMaxPower( poInstanceProp ) )
		{
			SYS_CRITICAL(_SDT("[%s: %d]: SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
				m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
			SDDelete poInstanceProp;
			poInstanceProp = NULL;
			return FALSE;
		}

		m_pstLastInstanceProp = poInstanceProp;
		poTownProp->m_mapInstance[poInstanceProp->byIndex] = poInstanceProp;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}
BOOL CInstancePropMgr::LoadFromData(STownInstanceProp* pstTownInstanceProp, const CHAR* pszData, const UINT32 dwDataLen)
{
    SGDP::CSDXMLFile xmlFile;
    std::string strValue;


    if(FALSE == xmlFile.LoadData(pszData, dwDataLen))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: xml file load failed!"), MSG_MARK);
        return FALSE;
    }
    CSDXMLNode rootNode = xmlFile.GetRootNode();
    if(rootNode == NULL)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: xml obj err!"), MSG_MARK);
        return FALSE;
    }

    UINT16	wLastEnterLevel		=	0;
    UINT8	byForwardIndex	=	0;
    CSDXMLNode xmlNodePropInstance = rootNode["Instance"];
    while(xmlNodePropInstance != NULL)
    {
        SInstanceProp* pstInstanceProp = SDNew SInstanceProp;
        pstInstanceProp->byIndex = xmlNodePropInstance("Index").AsInteger();
        m_stInstanceIDTmp.byInstanceIdx = pstInstanceProp->byIndex;//
        pstInstanceProp->strName = xmlNodePropInstance("Name").AsString();
        pstInstanceProp->strDispName = xmlNodePropInstance("DispName").AsString();

        if (byForwardIndex + 1 != pstInstanceProp->byIndex)
        {
            SYS_CRITICAL(_SDT("[%s: %d]:Instances cfg invalid! (%d-%d)"), MSG_MARK, byForwardIndex, pstInstanceProp->byIndex);
            return FALSE;
        }
        byForwardIndex++;


        if (pstInstanceProp->strName.empty())
        {
            char szTemp[256] = {0};
            sprintf(szTemp, "%u", pstInstanceProp->byIndex);
            pstInstanceProp->strName = szTemp;
        }

        if (pstInstanceProp->strDispName.empty())
        {
            char szTemp[256] = {0};
            sprintf(szTemp, "%u", pstInstanceProp->byIndex);
            pstInstanceProp->strDispName = szTemp;
        }

        if((pstInstanceProp->byIndex == 0) || (pstInstanceProp->strName.empty()) || (pstInstanceProp->strDispName.empty()))
        {
            SYS_CRITICAL(_SDT("[%s: %d]:Instances cfg invalid!"), MSG_MARK);
            SDDelete pstInstanceProp;
            pstInstanceProp = NULL;
            return FALSE;
        }
        pstInstanceProp->wEnterLevel = xmlNodePropInstance("EnterLevel").AsInteger();

        if (wLastEnterLevel < pstInstanceProp->wEnterLevel)
        {
            wLastEnterLevel = pstInstanceProp->wEnterLevel;
        }
        else
        {
            pstInstanceProp->wEnterLevel = wLastEnterLevel;
        }

        //战斗回合数
        pstInstanceProp->byRoundNum = xmlNodePropInstance("RoundNum").AsInteger();
		if (0 == pstInstanceProp->byRoundNum)
		{
			pstInstanceProp->byRoundNum = m_byRoundNum;
		}
		else
		{
			m_byRoundNum = pstInstanceProp->byRoundNum;
		}

        //血量百分比
        pstInstanceProp->byPercentageHp = xmlNodePropInstance("PercentageHp").AsInteger();
		if (0 == pstInstanceProp->byPercentageHp)
		{
			pstInstanceProp->byPercentageHp = m_byPercentageHp;
		}
		else
		{
			m_byPercentageHp = pstInstanceProp->byPercentageHp;
		}
        //奖励
        CSDXMLNode xmlNodeProp = xmlNodePropInstance["Encourage"];
        if(!ReadEncourage(pstInstanceProp, xmlNodeProp))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: ReadEncourage failed!"), MSG_MARK);
            SDDelete pstInstanceProp;
            pstInstanceProp = NULL;
            return FALSE;
        }

		xmlNodeProp = xmlNodePropInstance["OutBound"];
		if ( !ReadOutBound(pstInstanceProp, xmlNodeProp))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: ReadOutBound failed!"), MSG_MARK);
			SDDelete pstInstanceProp;
			pstInstanceProp = NULL;
			return FALSE;
		}
		
        //怪物
        xmlNodeProp = xmlNodePropInstance["Monsters"];
        if(!ReadMonster(pstInstanceProp, xmlNodeProp))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: ReadMonster failed!"), MSG_MARK);
            SDDelete pstInstanceProp;
            pstInstanceProp = NULL;
            return FALSE;
        }

        //只有普通副本才有这些解锁配置
        if(EIT_COMMON == m_stInstanceIDTmp.wSceneIdx)
        {
            xmlNodeProp = xmlNodePropInstance["Build"];
            if(!ReadBuild(pstInstanceProp, xmlNodeProp))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: ReadBuild failed!"), MSG_MARK);
                SDDelete pstInstanceProp;
                pstInstanceProp = NULL;
                return FALSE;
            }

            xmlNodeProp = xmlNodePropInstance["Hero"];
            if(!ReadHero(pstInstanceProp, xmlNodeProp))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: ReadHero failed!"), MSG_MARK);
                SDDelete pstInstanceProp;
                pstInstanceProp = NULL;
                return FALSE;
            }

            xmlNodeProp = xmlNodePropInstance["UnlockEliteInstance"];
            if(!ReadEliteInstance(pstInstanceProp, xmlNodeProp))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: ReadEliteInstance failed!"), MSG_MARK);
                SDDelete pstInstanceProp;
                pstInstanceProp = NULL;
                return FALSE;
            }

            xmlNodeProp = xmlNodePropInstance["Skill"];
            if(!ReadUnlockSkill(pstInstanceProp, xmlNodeProp))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: ReadUnlockSkill failed!"), MSG_MARK);
                SDDelete pstInstanceProp;
                pstInstanceProp = NULL;
                return FALSE;
            }

        } //end if(EIT_COMMON == m_stInstanceIDTmp.wSceneIdx)
		if ( (m_stInstanceIDTmp.byTownIdx == 95) && (m_stInstanceIDTmp.byInstanceIdx == 3))
		{
			pstInstanceProp = pstInstanceProp;
		}

        if(FALSE == InitBoxBurstItem(pstInstanceProp))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: InitBurstItem failed!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
                         m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
            SDDelete pstInstanceProp;
            pstInstanceProp = NULL;
            return FALSE;
        }
        if(FALSE == InitBurstExp(pstInstanceProp))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: InitBurstExp failed!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
                         m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
            SDDelete pstInstanceProp;
            pstInstanceProp = NULL;
            return FALSE;
        }

        if( FALSE == InitMaxPower( pstInstanceProp ) )
        {
            SYS_CRITICAL(_SDT("[%s: %d]: SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
                         m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
            SDDelete pstInstanceProp;
            pstInstanceProp = NULL;
            return FALSE;
        }


        pstTownInstanceProp->m_mapInstance[pstInstanceProp->byIndex] = pstInstanceProp;
        xmlNodePropInstance = xmlNodePropInstance.Sibling("Instance");
        m_pstLastInstanceProp = pstInstanceProp;

        //DBG_INFO(_SDT("[%s: %d]: CInstancePropMgr::LoadFromXML InstanceID[%d.%d.%d]\n"), MSG_MARK, m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
    }

    return TRUE;
}



//读取单个属性值
UINT32 CInstancePropMgr::ReadOneValue(EBurstKind eBurstKind, CSDXMLNode& xmlNode)
{
    if(xmlNode == NULL)
    {
        return 0;
    }

    CHAR* pszReadItemName = NULL;
    switch(eBurstKind)
    {
    case EBKIND_COIN:
        pszReadItemName = "Coin";
        break;
    case EBKIND_GOLD:
        pszReadItemName = "Gold";
        break;
    case EBKIND_EXPERIENCE:
        pszReadItemName = "Experience";
        break;
    case EBKIND_PHYSTRENGTH:
        pszReadItemName = "PhyStrength";
        break;
    case EBKIND_SCIENCE:
        pszReadItemName = "Science";
        break;
    case EBKIND_STORY:
        pszReadItemName = "Story";
        break;
    default:
        return FALSE;
    }

    CSDXMLNode xmlNodeProp = xmlNode[pszReadItemName];
    if(xmlNodeProp != NULL)
    {
        return xmlNodeProp("Value").AsInteger();
    }

    return 0;
}

BOOL CInstancePropMgr::ReadEncourageItemGroup(SPassBoxProp& stPassBoxProp, UINT8 byGroupIdx, CSDXMLNode& xmlNode)
{
    if(xmlNode == NULL)
    {
        return FALSE;
    }

    CSDXMLNode xmlNodeProp = xmlNode["Prop"];
    UINT16	wFirstItemKindID = 0;
    UINT16	wFirstItemPileNum = 0;
    while(xmlNodeProp != NULL)
    {
        UINT16 wKindID = xmlNodeProp("KindID").AsInteger();
        UINT16 wNum = xmlNodeProp("Num").AsInteger();
        UINT16 wRate = xmlNodeProp("Rate").AsInteger();
        wRate = wRate > 100 ? 100 : wRate;
        UINT8 byFirstFalg = xmlNodeProp("FirstFalg").AsInteger();
        if((0 == wKindID) || (0 == wNum))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CInstancePropMgr::ReadEncourageItem failed ((0 == wKindID) || (0 == wNum) || (0 == wRate)), InstanceID[%d.%d.%d]\n"),
                         MSG_MARK, m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
            return FALSE;
        }
        if(1 == byFirstFalg)
        {
            wFirstItemKindID = wKindID;
            wFirstItemPileNum = wNum;
        }

        SItemRate stItemRate;
        stItemRate.wItemNum = wNum;
        stItemRate.wRate = wRate * 100; //万分比
        SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wKindID);

        //内部宝箱全部拆开
        if((pstGoodsProp) && (EGMK_INNERBOX == pstGoodsProp->byMainKindID))
        {
            SItemComposeProp* pstItemComposeProp = CItemComposePropMgr::Instance()->GetProp(wKindID);
            if(NULL == pstItemComposeProp)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: pstItemComposeProp is NULL,wKindID[%d]!"), MSG_MARK, wKindID);
                return FALSE;
            }

            CItemRateMap& mapComposeItem = pstItemComposeProp->mapComposeItem;
            if(0 == mapComposeItem.size())
            {
                SYS_CRITICAL(_SDT("[%s: %d]: mapComposeItem[%d] is 0!"), MSG_MARK, wKindID);
                return FALSE;
            }
            SItemGroupProp& stItemGroupProp = stPassBoxProp.mapItemGroup[byGroupIdx];
            for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end();  itr++)
            {
                UINT16 wItemKindID = itr->first;

                SItemRate stRate = itr->second;
                stRate.wRate = stRate.wRate * wRate / 100;
                stRate.wRate = stRate.wRate == 0 ? 1 : stRate.wRate;
                stItemGroupProp.mapItem[wItemKindID] = stRate;

                m_mapBurstItemKind2InstanceID[wItemKindID] = m_stInstanceIDTmp;//只需记录最后产出该道具的，可直接覆盖
                // m_mapDropItemID2InstanceList[wItemKindID].push_back(m_stInstanceIDTmp); // 存入副本列表
            }
            stItemGroupProp.wFirstItemKindID = wFirstItemKindID;
            stItemGroupProp.wFirstItemPileNum = wFirstItemPileNum;
        }
        else
        {
            SItemGroupProp& stItemGroupProp = stPassBoxProp.mapItemGroup[byGroupIdx];
            stItemGroupProp.mapItem[wKindID] = stItemRate;
            stItemGroupProp.wFirstItemKindID = wFirstItemKindID;
            stItemGroupProp.wFirstItemPileNum = wFirstItemPileNum;
        }

        //铜币元宝不下发产出
        //if((NULL == pstGoodsProp) || ((EGMK_COIN != pstGoodsProp->byMainKindID) && (EGMK_GOLD != pstGoodsProp->byMainKindID)))
        //铜币元宝又要下发了。。。
        if(stPassBoxProp.stDT_BURST_ITEM_DATA.byBurstItemNum < MAX_ENCOURAGE_ITEM_KIND_NUM)
        {
            stPassBoxProp.stDT_BURST_ITEM_DATA.awBurstItemList[stPassBoxProp.stDT_BURST_ITEM_DATA.byBurstItemNum++] = wKindID;
        }
        m_mapBurstItemKind2InstanceID[wKindID] = m_stInstanceIDTmp;//只需记录最后产出该道具的，可直接覆盖
        xmlNodeProp = xmlNodeProp.Sibling("Prop");
    }

    return TRUE;
}

BOOL CInstancePropMgr::ReadEncourageItem(SPassBoxProp& stPassBoxProp, CSDXMLNode& xmlNode)
{
    if(xmlNode == NULL)
    {
        return TRUE;
    }

    CHAR* pszReadItemName = "Item";
    CSDXMLNode xmlItemNodeProp = xmlNode[pszReadItemName];
    if(xmlItemNodeProp == NULL)
    {
        return TRUE;//没有配置爆出物品
    }

    CSDXMLNode xmlNodeProp = xmlItemNodeProp["Group"];
    UINT8 byGroupIdx = 1;//组序号从1开始
    while(xmlNodeProp != NULL)
    {
        if(FALSE == ReadEncourageItemGroup(stPassBoxProp, byGroupIdx++, xmlNodeProp))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CInstancePropMgr::ReadEncourageItemGroup[%d] failed, InstanceID[%d.%d.%d]\n"),
                         MSG_MARK, byGroupIdx, m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
            return FALSE;
        }
        xmlNodeProp = xmlNodeProp.Sibling("Group");
    }

    return TRUE;
}

//读取奖励
BOOL CInstancePropMgr::ReadEncourage(SInstanceProp* pstInstanceProp, CSDXMLNode& xmlNode)
{
    if(xmlNode == NULL)
    {
        return FALSE;
    }
    pstInstanceProp->stPassEncourageProp.dwCoin = ReadOneValue(EBKIND_COIN, xmlNode);
    pstInstanceProp->stPassEncourageProp.dwStory = ReadOneValue(EBKIND_STORY, xmlNode);
    pstInstanceProp->stPassEncourageProp.dwExperience = ReadOneValue(EBKIND_EXPERIENCE, xmlNode);

    if(FALSE == ReadEncourageItem(pstInstanceProp->stPassBoxProp, xmlNode))
    {
        return FALSE;
    }

    return TRUE;
}


//读取怪物
BOOL CInstancePropMgr::ReadMonster(SInstanceProp* pstInstanceProp, CSDXMLNode& xmlNode)
{
    if(xmlNode == NULL)
    {
        return FALSE;
    }
    CHAR* pszReadItemName = "Monster";
    CSDXMLNode xmlNodeProp = xmlNode[pszReadItemName];
    while(xmlNodeProp != NULL)
    {
        map<UINT8, UINT8> mapFormation;
        SWholeMonsterProp* pstWholeMonsterProp = SDNew SWholeMonsterProp();
        CSDXMLNode xmlKindNodeProp = xmlNodeProp["KindID"];
        while(xmlKindNodeProp != NULL)
        {
            SSingleMonsterProp stProp;
            stProp.wMonsterID = xmlKindNodeProp("Value").AsInteger();
            if(0 == stProp.wMonsterID)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: ReadMonster failed!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
                             m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
                SDDelete pstWholeMonsterProp;
                return FALSE;
            }
            stProp.byFormationIdx = xmlKindNodeProp("FormationIdx").AsInteger(1);
            if((mapFormation.find(stProp.byFormationIdx) != mapFormation.end()) || (stProp.byFormationIdx > MAX_FORMATION_IDX_NUM) || (0 == stProp.byFormationIdx))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: ReadMonster failed!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
                             m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
                SDDelete pstWholeMonsterProp;
                return FALSE;
            }
            mapFormation[stProp.byFormationIdx] = stProp.byFormationIdx;

            pstWholeMonsterProp->vecMonster.push_back(stProp);

            xmlKindNodeProp = xmlKindNodeProp.Sibling("KindID");
        }

        if(0 == pstWholeMonsterProp->vecMonster.size())
        {
            SDDelete pstWholeMonsterProp;
            pstWholeMonsterProp = NULL;
        }
        else
        {
            pstInstanceProp->vecWholeMonsterProp.push_back(pstWholeMonsterProp);
        }

        xmlNodeProp = xmlNodeProp.Sibling(pszReadItemName);
    }
    UINT8 bySize = pstInstanceProp->vecWholeMonsterProp.size();
    if(0 == bySize)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: No monster!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
                     m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
        return FALSE;
    }
    SWholeMonsterProp* poMainMonster = pstInstanceProp->vecWholeMonsterProp[bySize - 1];
    if(NULL == poMainMonster)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: poMainMonster is NULL!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
                     m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
        return FALSE;
    }
    bySize = poMainMonster->vecMonster.size();
    if(0 == bySize)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: poMainMonster prop invalid!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
                     m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
        return FALSE;
    }
    pstInstanceProp->wMainMonsterKindID = poMainMonster->vecMonster[bySize - 1].wMonsterID;

    return TRUE;
}

BOOL CInstancePropMgr::ReadOutBound(SInstanceProp* pstInsanceProp, CSDXMLNode& xmlNode)
{
	if ( NULL == pstInsanceProp)
	{
		return FALSE;
	}
	//外传才会有该配置
	if ( xmlNode == NULL)
	{
		return TRUE;
	}

	pstInsanceProp->strInstanceDesc = xmlNode("Desc").AsString();
	pstInsanceProp->byBattleNum = xmlNode("BattleNum").AsInteger();
	CSDXMLNode xmlNodeProp = xmlNode["UnlockLevel"];
	if ( xmlNodeProp == NULL)
	{
		return FALSE;
	}
	pstInsanceProp->strLeveReqDesc = xmlNodeProp("Desc").AsString();
	xmlNodeProp = xmlNode["ParentInstance"];
	if ( xmlNodeProp != NULL)
	{
		pstInsanceProp->stParentInstance.wSceneIdx = xmlNodeProp("ScenceIdx").AsInteger();
		pstInsanceProp->stParentInstance.byTownIdx = xmlNodeProp("TownIdx").AsInteger();
		pstInsanceProp->stParentInstance.byInstanceIdx = xmlNodeProp("InstanceIdx").AsInteger();
		pstInsanceProp->strParentReqDesc = xmlNodeProp("Desc").AsString();
	}
	xmlNodeProp = xmlNode["Major"];
	if ( xmlNodeProp != NULL)
	{
		pstInsanceProp->bIsMajor = xmlNodeProp("Value").AsBoolean();
	}
	xmlNodeProp = xmlNode["ItemOutPut"];
	if ( xmlNodeProp != NULL)
	{
		pstInsanceProp->strItemDesc = xmlNodeProp("Desc").AsString();
	}
	xmlNodeProp = xmlNode["HeroOutPut"];
	if ( xmlNodeProp != NULL)
	{
		pstInsanceProp->strHeroDesc = xmlNodeProp("Desc").AsString();
	}
	
	return TRUE;
}

//读取建筑
BOOL CInstancePropMgr::ReadBuild(SInstanceProp* pstInstanceProp, CSDXMLNode& xmlNode)
{
    //先赋值之前副本解锁到的建筑
    if(NULL != m_pstLastInstanceProp)
    {
        pstInstanceProp->stInstanceOpenFunc.setOpenBuild = m_pstLastInstanceProp->stInstanceOpenFunc.setOpenBuild;
    }

    if(xmlNode == NULL)
    {
        return TRUE;//不一定有建筑解锁配置
    }

    CSDXMLNode xmlNodeProp = xmlNode["Prop"];
    while(xmlNodeProp != NULL)
    {
        UINT8 byUnlockBuildKindID = xmlNodeProp("UnlockBuildKindID").AsInteger(0);
        if(0 == byUnlockBuildKindID)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: ReadBuild  UnlockBuildKindID invalid!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
                         m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
            return FALSE;
        }
        pstInstanceProp->mapUnlockBuildKind[byUnlockBuildKindID] = byUnlockBuildKindID;

        if(m_mapUnlockBuildKind2InstanceID.find(byUnlockBuildKindID) != m_mapUnlockBuildKind2InstanceID.end())
        {
            SYS_CRITICAL(_SDT("[%s: %d]: build[%d] is reunlock!"), MSG_MARK, byUnlockBuildKindID);
            return FALSE;
        }
        m_mapUnlockBuildKind2InstanceID[byUnlockBuildKindID] = m_stInstanceIDTmp;

        pstInstanceProp->stInstanceOpenFunc.setOpenBuild.insert(byUnlockBuildKindID);

        xmlNodeProp = xmlNodeProp.Sibling("Prop");
    }

    return TRUE;
}

//武将
BOOL CInstancePropMgr::ReadHero(SInstanceProp* pstInstanceProp, CSDXMLNode& xmlNode)
{
    //先赋值之前副本解锁到的武将
    if(m_pstLastInstanceProp)
    {
        pstInstanceProp->stInstanceOpenFunc.vecOpenHero = m_pstLastInstanceProp->stInstanceOpenFunc.vecOpenHero;
        //pstInstanceProp->stInstanceOpenFunc.mapOpenHero = m_pstLastInstanceProp->stInstanceOpenFunc.mapOpenHero;
    }


    //没有配置武将解锁信息时能插入记录
    if(xmlNode == NULL)
    {
        pstInstanceProp->stHeroUnlockProp.byCarryNum = m_byLastCarryNum;
        for (int i = 0; i < MAX_UNLOCK_HERO_NUM; i++)
        {
            pstInstanceProp->stHeroUnlockProp.wUnlockHeroKindID[i] = 0;
        }
        //方便在玩家每个副本时都能查找到武将解锁信息
        m_mapInstanceID2HeroUnlockProp[CLogicFunc::GetInstanceIdxKey(m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx)] = pstInstanceProp->stHeroUnlockProp;
        return TRUE;//不一定有武将解锁配置
    }

    //CarryNum不一定有
    pstInstanceProp->stHeroUnlockProp.byCarryNum = xmlNode("CarryNum").AsInteger();
    if(0 == pstInstanceProp->stHeroUnlockProp.byCarryNum)
    {
        pstInstanceProp->stHeroUnlockProp.byCarryNum = m_byLastCarryNum;
    }

    if(pstInstanceProp->stHeroUnlockProp.byCarryNum < m_byLastCarryNum)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: ReadHero failed(byCarryNum < m_byLastCarryNum)!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
                     m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
        return FALSE;
    }
    m_byLastCarryNum = pstInstanceProp->stHeroUnlockProp.byCarryNum;//用于每读取1个副本，在没有配置武将解锁信息时能插入记录


    int nIndex = 0;
    CSDXMLNode xmlNodeProp = xmlNode["Prop"];
    while(xmlNodeProp != NULL && nIndex < MAX_UNLOCK_HERO_NUM)
    {
        UINT16 wUnlockHeroKindID = xmlNodeProp("UnlockHeroKindID").AsInteger();


        if(0 != wUnlockHeroKindID)
        {
            vector<UINT16>::iterator result = find(pstInstanceProp->stInstanceOpenFunc.vecOpenHero.begin(), pstInstanceProp->stInstanceOpenFunc.vecOpenHero.end(), wUnlockHeroKindID); //查找3
            if ( result == pstInstanceProp->stInstanceOpenFunc.vecOpenHero.end()) //没找到
            {
                pstInstanceProp->stInstanceOpenFunc.vecOpenHero.push_back(wUnlockHeroKindID);
            }
            else
            {
                SYS_CRITICAL(_SDT("[%s: %d]:KindID:%d"), MSG_MARK, wUnlockHeroKindID);
                return FALSE;
            }

            //处理武将出场次序
            SHeroProp* pHeroProp = CHeroPropMgr::Instance()->GetHeroProp(wUnlockHeroKindID);

            if( pHeroProp )
            {
                pHeroProp->byIndex = ++m_byCurHeroIndex;
            }
            else
            {
                SYS_CRITICAL(_SDT("[%s: %d]: pHeroProp is NULL! KindID:%d"), MSG_MARK, wUnlockHeroKindID );
            }

            pstInstanceProp->stHeroUnlockProp.wUnlockHeroKindID[nIndex] = wUnlockHeroKindID;
            nIndex++;
        }
        xmlNodeProp = xmlNodeProp.Sibling("Prop");
    }
    //副本武将解锁信息,方便在玩家每个副本时都能查找到武将解锁信息
    m_mapInstanceID2HeroUnlockProp[CLogicFunc::GetInstanceIdxKey(m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx)] = pstInstanceProp->stHeroUnlockProp;


    return TRUE;
}

//读取解锁精英副本
BOOL CInstancePropMgr::ReadEliteInstance(SInstanceProp* pstInstanceProp, CSDXMLNode& xmlNode)
{
    DT_INSTANCE_ID& stUnlockEliteInstanceID = pstInstanceProp->stUnlockEliteInstanceID;
    DT_INSTANCE_ID& stUnlock2EliteInstanceID = pstInstanceProp->stUnlock2EliteInstanceID;



    //没有配置解锁信息时能插入记录
    if(xmlNode == NULL)
    {
        memset(&stUnlockEliteInstanceID, 0, sizeof(stUnlockEliteInstanceID));
        stUnlock2EliteInstanceID = m_stLastEliteInstanceID;
		pstInstanceProp->mapUnlockEliteInstanceID = s_mapUnlockEliteInstanceID;
        return TRUE;
    }

    stUnlockEliteInstanceID.wSceneIdx = EIT_ELITE;
    stUnlockEliteInstanceID.byTownIdx = xmlNode("TownIdx").AsInteger();
    stUnlockEliteInstanceID.byInstanceIdx = xmlNode("InstanceIdx").AsInteger();
    

    UINT16	wKey = CLogicFunc::Get88Key(stUnlockEliteInstanceID.byTownIdx, stUnlockEliteInstanceID.byInstanceIdx);

	//检测是否存在，如果存在，则之前已解锁
    if(s_mapUnlockEliteInstanceID.find(wKey) != s_mapUnlockEliteInstanceID.end())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: ReadEliteInstance failed!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d], Repeat"), MSG_MARK,
                     m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
        return FALSE;
    }


    if((stUnlockEliteInstanceID.byTownIdx < m_stLastEliteInstanceID.byTownIdx) ||
            ((stUnlockEliteInstanceID.byTownIdx == m_stLastEliteInstanceID.byTownIdx) &&
             (stUnlockEliteInstanceID.byInstanceIdx <= m_stLastEliteInstanceID.byInstanceIdx)))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: ReadEliteInstance failed!SceneIdx[%d],TownIdx[%d],InstanceIdx[%d]"), MSG_MARK,
                     m_stInstanceIDTmp.wSceneIdx, m_stInstanceIDTmp.byTownIdx, m_stInstanceIDTmp.byInstanceIdx);
        return FALSE;
    }
	//更新
	s_mapUnlockEliteInstanceID[wKey] = stUnlockEliteInstanceID;
	pstInstanceProp->mapUnlockEliteInstanceID = s_mapUnlockEliteInstanceID;


    stUnlock2EliteInstanceID = stUnlockEliteInstanceID;
    m_stLastEliteInstanceID = stUnlockEliteInstanceID;

    return TRUE;
}

BOOL CInstancePropMgr::ReadUnlockSkill(SInstanceProp* pstInstanceProp, CSDXMLNode& xmlNode)
{
    vector<UINT16>& vecUnlockSkill = pstInstanceProp->stInstanceOpenFunc.vecUnlockSkill;
    //先赋值之前副本解锁到的技能
    if(NULL != m_pstLastInstanceProp)
    {
        vector<UINT16>& vecLastUnlockSkill = m_pstLastInstanceProp->stInstanceOpenFunc.vecUnlockSkill;
        UINT8 bySize = vecLastUnlockSkill.size();
        for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
        {
            vecUnlockSkill.push_back(vecLastUnlockSkill[byIdx]);
        }
    }

    if(xmlNode == NULL)
    {
        pstInstanceProp->wUnlockSKillID = 0;
        return TRUE;
    }

    pstInstanceProp->wUnlockSKillID = xmlNode("Value").AsInteger();
    if(m_mapUnlockSKillID2InstanceID.find(pstInstanceProp->wUnlockSKillID) != m_mapUnlockSKillID2InstanceID.end())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: skillid[%d] is reunlock!"), MSG_MARK, pstInstanceProp->wUnlockSKillID);
        return FALSE;
    }
    m_mapUnlockSKillID2InstanceID[pstInstanceProp->wUnlockSKillID] = m_stInstanceIDTmp;
    vecUnlockSkill.push_back(pstInstanceProp->wUnlockSKillID);

    return TRUE;
}

//统计副本解锁记录
VOID CInstancePropMgr::ReadInstanceUnlock()
{
	CIndex2SceneTownPropMapItr sceneItr = m_mapIndex2SceneTownProp.find(EIT_OUT_BOUND);
	if ( sceneItr == m_mapIndex2SceneTownProp.end())
	{
		return ;
	}
	for ( CIndex2TownInstancePropMapItr townItr = sceneItr->second->m_mapTownInstance.begin(); townItr != sceneItr->second->m_mapTownInstance.end(); townItr++)
	{
		for ( CIndex2InstancePropMapItr instanceItr = townItr->second->m_mapInstance.begin(); instanceItr != townItr->second->m_mapInstance.end(); instanceItr++)
		{
			SInstanceProp* poProp = GetInstanceProp(instanceItr->second->stParentInstance.wSceneIdx, instanceItr->second->stParentInstance.byTownIdx, instanceItr->second->stParentInstance.byInstanceIdx);
			DT_INSTANCE_ID stInstanceID = {0};
			stInstanceID.wSceneIdx = sceneItr->second->wSceneIndex;
			stInstanceID.byTownIdx = townItr->second->byTownIndex;
			stInstanceID.byInstanceIdx = instanceItr->second->byIndex;
			poProp->vecUnlockInstance.push_back(stInstanceID);
		}
	}
	
}


SInstanceProp::~SInstanceProp()
{
    for(vector<SWholeMonsterProp*>::iterator itrMonsterProp = vecWholeMonsterProp.begin(); itrMonsterProp != vecWholeMonsterProp.end(); itrMonsterProp++)
    {
        SDDelete *itrMonsterProp;
        *itrMonsterProp = NULL;
    }
    vecWholeMonsterProp.clear();

}

STownInstanceProp::~STownInstanceProp()
{
    for(CIndex2InstancePropMapItr itr = m_mapInstance.begin(); itr != m_mapInstance.end(); itr++)
    {
        SDDelete itr->second;
        itr->second = NULL;
    }
    m_mapInstance.clear();
}

SSceneTownProp::~SSceneTownProp()
{
    for(CIndex2TownInstancePropMapItr itr = m_mapTownInstance.begin(); itr != m_mapTownInstance.end(); itr++)
    {
        SDDelete itr->second;
        itr->second = NULL;
    }
    m_mapTownInstance.clear();
}

SInstanceProp* CInstancePropMgr::GetInstanceProp(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx)
{
    CIndex2SceneTownPropMapItr itrScene = m_mapIndex2SceneTownProp.find(wSceneIdx);
    if(itrScene == m_mapIndex2SceneTownProp.end())
    {
        return NULL;
    }
    SSceneTownProp* pstSceneTownProp = itrScene->second;
    if(NULL == pstSceneTownProp)
    {
        return NULL;
    }

    CIndex2TownInstancePropMapItr itrTown = pstSceneTownProp->m_mapTownInstance.find(byTownIdx);
    if(itrTown == pstSceneTownProp->m_mapTownInstance.end())
    {
        return NULL;
    }
    STownInstanceProp* pstTownInstanceProp = itrTown->second;
    if(NULL == pstTownInstanceProp)
    {
        return NULL;
    }

    CIndex2InstancePropMapItr itrInstance = pstTownInstanceProp->m_mapInstance.find(byInstanceIdx);
    if(itrInstance == pstTownInstanceProp->m_mapInstance.end())
    {
        return NULL;
    }

    return  itrInstance->second;
}


VOID CInstancePropMgr::GetNextInstance(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, SNextInstance& stNextInstance)
{
    //假设没有下一个副本，默认下一个还是自己
    stNextInstance.bHaveNext = TRUE;
    stNextInstance.eNextInstanceType = ENIT_NEXT_INSTANCE;
    stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx = wSceneIdx;
    stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx = byTownIdx;
    stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx = byInstanceIdx;

    //新建用户
    if(0 == byTownIdx)
    {
        stNextInstance.eNextInstanceType = ENIT_NEXT_TOWN;
        stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx = wSceneIdx;
        stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx = 1;
        stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx = 1;
        //stNextInstance.stDT_INSTANCE_DATA.byScore = 0;
        //stNextInstance.stDT_INSTANCE_DATA.wPassTimes = 0;
        stNextInstance.pstInstanceProp = GetInstanceProp(wSceneIdx, 1, 1);
        if(stNextInstance.pstInstanceProp)
        {
            stNextInstance.bHaveNext = TRUE;
        }
        else
        {
            stNextInstance.bHaveNext = FALSE;
        }

        return;
    }

    CIndex2SceneTownPropMapItr itrScene = m_mapIndex2SceneTownProp.find(wSceneIdx);
    if(itrScene == m_mapIndex2SceneTownProp.end())
    {
        stNextInstance.bHaveNext = FALSE;
        return;
    }
    SSceneTownProp* pstSceneTownProp = itrScene->second;
    if(NULL == pstSceneTownProp)
    {
        stNextInstance.bHaveNext = FALSE;
        return;
    }

    CIndex2TownInstancePropMapItr itrTown = pstSceneTownProp->m_mapTownInstance.find(byTownIdx);
    if(itrTown == pstSceneTownProp->m_mapTownInstance.end())
    {
        stNextInstance.bHaveNext = FALSE;
        return;
    }
    STownInstanceProp* pstTownInstanceProp = itrTown->second;
    if(NULL == pstTownInstanceProp)
    {
        stNextInstance.bHaveNext = FALSE;
        return;
    }

    CIndex2InstancePropMapItr itrInstance = pstTownInstanceProp->m_mapInstance.find(byInstanceIdx + 1);
    //下一个副本没有，查找下一个城镇
    if(itrInstance == pstTownInstanceProp->m_mapInstance.end())
    {
        CIndex2TownInstancePropMapItr itrTown2 = pstSceneTownProp->m_mapTownInstance.find(byTownIdx + 1);
        //下一个城镇没有，已经没有副本，返回
        if(itrTown2 == pstSceneTownProp->m_mapTownInstance.end())
        {
            stNextInstance.bHaveNext = FALSE;
            return;
        }
        //存在下一个城镇
        else
        {
            STownInstanceProp* pstTownInstanceProp2 = itrTown2->second;
            CIndex2InstancePropMapItr itrInstance2 = pstTownInstanceProp2->m_mapInstance.find(1);
            //下一个副本没有，则结束，没有下一个副本信息
            if(itrInstance2 == pstTownInstanceProp2->m_mapInstance.end())
            {
                stNextInstance.bHaveNext = FALSE;
                return;
            }
            //存在下一个副本
            else
            {
                SInstanceProp* pstInstanceProp = itrInstance2->second;
                stNextInstance.bHaveNext = TRUE;
                stNextInstance.pstInstanceProp = pstInstanceProp;
                stNextInstance.eNextInstanceType = ENIT_NEXT_TOWN;
                stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx = pstInstanceProp->byIndex;
                //stNextInstance.stDT_INSTANCE_DATA.wPassTimes = 0;
                //stNextInstance.stDT_INSTANCE_DATA.byScore = 0;
                stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx = byTownIdx + 1;
                stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx = wSceneIdx;
            }
        }

    }
    //存在下一个副本
    else
    {
        SInstanceProp* pstInstanceProp = itrInstance->second;
        stNextInstance.bHaveNext = TRUE;
        stNextInstance.pstInstanceProp = pstInstanceProp;
        stNextInstance.eNextInstanceType = ENIT_NEXT_INSTANCE;
        stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byInstanceIdx = pstInstanceProp->byIndex;
        //stNextInstance.stDT_INSTANCE_DATA.wPassTimes = 0;
        //stNextInstance.stDT_INSTANCE_DATA.byScore = 0;
        stNextInstance.stDT_INSTANCE_DATA.stInstanceID.byTownIdx = byTownIdx;
        stNextInstance.stDT_INSTANCE_DATA.stInstanceID.wSceneIdx = wSceneIdx;
    }
}


SHeroUnlockProp* CInstancePropMgr::GetUnlockHero(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx)
{
    CInstanceID2HeroUnlockPropMapItr itr = m_mapInstanceID2HeroUnlockProp.find(CLogicFunc::GetInstanceIdxKey(wSceneIdx, byTownIdx, byInstanceIdx));
    if(itr != m_mapInstanceID2HeroUnlockProp.end())
    {
        return &itr->second;
    }

    return NULL;
}

//初始化配置的可能爆出的物品,用于返回给客户端，不需每次都重新计算
BOOL CInstancePropMgr::InitBoxBurstItem(SInstanceProp* pstInstanceProp)
{
    if(NULL == pstInstanceProp)
    {
        return FALSE;
    }

    CItemGroupMap& mapItemGroup = pstInstanceProp->stPassBoxProp.mapItemGroup;
    for(CItemGroupMapItr itrGroup = mapItemGroup.begin(); itrGroup != mapItemGroup.end(); itrGroup++)
    {
        SItemGroupProp& stItemGroupProp = itrGroup->second;
        CItemRateMap& mapItem = stItemGroupProp.mapItem;
        DT_ITEM_DATA_LIST_CLI& stBoxItem = stItemGroupProp.stBoxItem;
        UINT8 byIdx = 1;//从1开始
        //stDT_BURST_ITEM_DATA的顺序要按照mapItem的顺序，避免随机命中后stDT_BURST_ITEM_DATA需修改
        for(CItemRateMapItr itr = mapItem.begin(); itr != mapItem.end(); itr++, byIdx++)
        {
            UINT16 wKindID = itr->first;
            if(stBoxItem.byItemNum < MAX_ENCOURAGE_ITEM_KIND_NUM)
            {
                //给客户端只返回MAX_ENCOURAGE_ITEM_KIND_NUM个
                DT_ITEM_DATA_CLI& stDT_ITEM_DATA_CLI = stBoxItem.astItemList[stBoxItem.byItemNum++];
                stDT_ITEM_DATA_CLI.wKindID = wKindID;
                SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wKindID);
                if(pstGoodsProp)
                {
                    stDT_ITEM_DATA_CLI.byGoodsMainKindID = pstGoodsProp->byMainKindID;
                }
                else
                {
                    stDT_ITEM_DATA_CLI.byGoodsMainKindID = 0;
                }
                stDT_ITEM_DATA_CLI.wPileNum = itr->second.wItemNum;
            }

            if(wKindID == stItemGroupProp.wFirstItemKindID)
            {
                stItemGroupProp.byFirstItemHitIdx = byIdx;
            }
        }
    }


    return TRUE;
}


//初始化配置的可能爆出的经验,用于返回给客户端，不需每次都重新计算
BOOL CInstancePropMgr::InitBurstExp(SInstanceProp* pstInstanceProp)
{
    if(NULL == pstInstanceProp)
    {
        return FALSE;
    }
    pstInstanceProp->dwAllExperience = 0;
    vector<SWholeMonsterProp*>& vecWholeMonsterProp = pstInstanceProp->vecWholeMonsterProp;
    UINT8 bySize = vecWholeMonsterProp.size();
    for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
    {
        SWholeMonsterProp* pstProp = vecWholeMonsterProp[byIdx];
        if(NULL == pstProp)
        {
            continue;
        }
        if(0 == pstProp->vecMonster.size())
        {
            continue;
        }

        //奖励只给主怪的
        SMonsterProp* pstMonsterProp = CMonsterPropMgr::Instance()->GetMonsterProp(pstProp->vecMonster[0].wMonsterID);
        if(NULL == pstMonsterProp)
        {
			SYS_CRITICAL(_SDT("[%s: %d]: MonsterID %u not find "), MSG_MARK, pstProp->vecMonster[0].wMonsterID);
            return FALSE;
        }
        SMonsterUpgradeProp* pstMonsterUpgradeProp = CMonsterUpgradePropMgr::Instance()->GetMonsterUpgradeProp(pstMonsterProp->wLevel, pstMonsterProp->byType);
        if(NULL == pstMonsterUpgradeProp)
        {
			SYS_CRITICAL(_SDT("[%s: %d]: MonsterID %u not find "), MSG_MARK, pstProp->vecMonster[0].wMonsterID);
            return FALSE;
        }
        pstInstanceProp->dwAllExperience += pstMonsterUpgradeProp->dwEncourageExp;

		pstInstanceProp->stOutBoundEncourage.dwCoin = pstMonsterUpgradeProp->dwEncourageCoin;
		pstInstanceProp->stOutBoundEncourage.dwStory = pstMonsterUpgradeProp->dwEncourageStory;
		pstInstanceProp->stOutBoundEncourage.dwExperience = pstInstanceProp->dwAllExperience;
    }

    pstInstanceProp->dwAllExperience  += pstInstanceProp->stPassEncourageProp.dwExperience;

    return TRUE;
}

BOOL CInstancePropMgr::InitMaxPower(SInstanceProp* pstInstanceProp)
{
    if(NULL == pstInstanceProp)
    {
        return FALSE;
    }

    pstInstanceProp->dwMaxMonsterPower = 0;

    for( std::size_t nWholeMonsterIndex = 0; nWholeMonsterIndex < pstInstanceProp->vecWholeMonsterProp.size(); ++nWholeMonsterIndex )
    {
        pstInstanceProp->vecWholeMonsterProp[nWholeMonsterIndex];
        SWholeMonsterProp *pMonsterProp = pstInstanceProp->vecWholeMonsterProp[ nWholeMonsterIndex ];
        if( NULL == pMonsterProp )
        {
            continue;
        }

        UINT32 dwMaxPower = 0;
        for( std::size_t nIndex = 0; nIndex < pMonsterProp->vecMonster.size(); ++nIndex )
        {
            SMonsterProp *pProp = CMonsterPropMgr::Instance()->GetMonsterProp( pMonsterProp->vecMonster[nIndex].wMonsterID );
            if( NULL == pProp )
            {
                continue;
            }

            dwMaxPower += static_cast<UINT32>(pProp->qwHP + pProp->dwAttack * 5);
        }

        if( dwMaxPower > pstInstanceProp->dwMaxMonsterPower )
        {
            pstInstanceProp->dwMaxMonsterPower = dwMaxPower;
        }
    }

    return TRUE;
}


DT_INSTANCE_ID* CInstancePropMgr::GetBurstItemInstancdID(UINT16 wItemKindID)
{
    CBurstItemKind2InstanceIDMapItr itr = m_mapBurstItemKind2InstanceID.find(wItemKindID);
    if(itr != m_mapBurstItemKind2InstanceID.end())
    {
        return &itr->second;
    }

    return NULL;
}

InstanceVec* CInstancePropMgr::GetDropInstanceListOfItem(UINT16 wItemKindID)
{
    BurstItemID2InstancesMap::iterator itr = m_mapDropItemID2InstanceList.find(wItemKindID);
    if (itr == m_mapDropItemID2InstanceList.end()){
        return NULL;
    }

    InstanceVec &vecInstance = itr->second;
    return &vecInstance;
}

vector<UINT16>& CInstancePropMgr::GetUnlockSkill(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx)
{
    SInstanceProp* pstInstanceProp = GetInstanceProp(wSceneIdx, byTownIdx, byInstanceIdx);
    if(pstInstanceProp)
    {
        return pstInstanceProp->stInstanceOpenFunc.vecUnlockSkill;
    }

    static vector<UINT16> vecEmptySkill;

    return vecEmptySkill;
}


DT_INSTANCE_ID* CInstancePropMgr::GetUnlockSkillInstancdID(UINT16 wSkillID)
{
    CUnlockSKillID2InstanceIDMapItr itr = m_mapUnlockSKillID2InstanceID.find(wSkillID);
    if(itr != m_mapUnlockSKillID2InstanceID.end())
    {
        return &itr->second;
    }

    return NULL;
}

UINT8 CInstancePropMgr::GetMaxOutBoundTownIdx()
{
	CIndex2SceneTownPropMapItr itr = m_mapIndex2SceneTownProp.find(EIT_OUT_BOUND);
	if ( itr == m_mapIndex2SceneTownProp.end())
	{
		return 0;
	}
	CIndex2TownInstancePropMap::reverse_iterator townItr = itr->second->m_mapTownInstance.rbegin();
	if ( townItr == itr->second->m_mapTownInstance.rend())
	{
		return 0;
	}
	return townItr->second->byTownIndex;

}

UINT8 CInstancePropMgr::GetMaxOutBoundInstanceIdx(UINT8 byTownIdx)
{
	CIndex2SceneTownPropMapItr itr = m_mapIndex2SceneTownProp.find(EIT_OUT_BOUND);
	if ( itr == m_mapIndex2SceneTownProp.end())
	{
		return 0;
	}
	CIndex2TownInstancePropMapItr townItr = itr->second->m_mapTownInstance.find(byTownIdx);
	if ( townItr == itr->second->m_mapTownInstance.end())
	{
		return 0;
	}
	CIndex2InstancePropMap::reverse_iterator instanceIdx = townItr->second->m_mapInstance.rbegin();
	if ( instanceIdx == townItr->second->m_mapInstance.rend())
	{
		return 0;
	}
	return instanceIdx->second->byIndex;
}

STownInstanceProp* CInstancePropMgr::GetSceneInstance(UINT16 wSceneIdx, UINT8 byTownIdx)
{
    SSceneTownProp * pstSceneTownProp = GetSceneTownProp( wSceneIdx );
    if ( NULL != pstSceneTownProp)
    {
        CIndex2TownInstancePropMapItr itr = pstSceneTownProp->m_mapTownInstance.find(byTownIdx);
        if (pstSceneTownProp->m_mapTownInstance.end() != itr)
        {
            return itr->second;
        }
    }
    return NULL;
}

SSceneTownProp* CInstancePropMgr::GetSceneTownProp(UINT16 wSceneIdx)
{
	SSceneTownProp * pstSceneTownProp = NULL;
	CIndex2SceneTownPropMapItr itr = m_mapIndex2SceneTownProp.find(wSceneIdx);
	if (m_mapIndex2SceneTownProp.end() != itr)
	{
		return itr->second;
		
	}
	return NULL;
}

VOID CInstancePropMgr::GetNeiDanAllInstance(CPlayer* poPlayer, DT_NAIDAN_INSTANCE_LST_DATA& stInstanceLst)
{
	SSceneTownProp* poSceneProp = GetSceneTownProp(EIT_NEIDAN);
	if ( NULL == poSceneProp || NULL == poPlayer)
	{
		return ;
	}
	CInstance& oInstance = poPlayer->GetInstance();
	UINT8 byInstanceNum = 0;
	for( CIndex2TownInstancePropMapItr itr = poSceneProp->m_mapTownInstance.begin(); itr != poSceneProp->m_mapTownInstance.end(); itr++, byInstanceNum++)
	{
		DT_NAIDAN_INSTANCE_DATA& stInstance = stInstanceLst.astInstanceInfo[byInstanceNum];
		SInstanceProp* poProp = itr->second->m_mapInstance[1];
		if ( NULL != poProp)
		{
			stInstance.wPlayerLevelReq = poProp->wEnterLevel;
			SDStrncpy(stInstance.aszInstanceName, poProp->strDispName.c_str(), NORMAL_MSG_LEN - 1);
			SDStrncpy(stInstance.aszItemDesc, poProp->strItemDesc.c_str(), NORMAL_MSG_LEN - 1);
			stInstance.stInstanceID.wSceneIdx = EIT_NEIDAN;
			stInstance.stInstanceID.byTownIdx = itr->second->byTownIndex;
			stInstance.stInstanceID.byInstanceIdx = 1;
			
			
			stInstance.byState = oInstance.GetInstanceStateByNeiDan(itr->second->byTownIndex);
		}
	}
	stInstanceLst.byInstanceNum = byInstanceNum;
}