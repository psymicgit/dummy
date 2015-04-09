#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/other/singleprammgr.h>
#include <framework/gsapi.h>
#include "awakenpropmgr2.h"

#include <db/dbmgr.h>

using namespace SGDP;


IMPLEMENT_SINGLETON_PROPMGR(CAwakenPropMgr2);

CAwakenPropMgr2::CAwakenPropMgr2()
{
	memset(m_awStudyUnlockLevelInfo, 0, sizeof(m_awStudyUnlockLevelInfo));
	memset(m_adwAwakenCoin, 0, sizeof(m_adwAwakenCoin));
}

CAwakenPropMgr2::~CAwakenPropMgr2()
{
}

BOOL CAwakenPropMgr2::Init()
{
	if(!LoadBaseFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadBaseFromDB failed!"), MSG_MARK); 
		return FALSE;
	}

	if(!LoadAwakenLevelFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadAwakenLevelFromDB failed!"), MSG_MARK); 
		return FALSE;
	}

	if(!LoadUnlockHeroStudyGridFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadUnlockHeroStudyGridFromDB failed!"), MSG_MARK); 
		return FALSE;
	}

	if(!LoadLevel2UnlockAwakenStudyFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadLevel2UnlockAwakenStudyFromDB failed!"), MSG_MARK); 
		return FALSE;
	}

	if(!LoadAwakenStudyAttrFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadAwakenStudyAttrFromDB failed!"), MSG_MARK); 
		return FALSE;
	}

	if(!LoadAwakenClipToStudyFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadAwakenClipToStudyFromDB failed!"), MSG_MARK); 
		return FALSE;
	}
	
    if (!LoadAwakenResolvePropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadAwakenResolvePropFromDB failed!"), MSG_MARK); 
        return FALSE;
    }
	return TRUE;
}

VOID CAwakenPropMgr2::UnInit()
{
	m_mapPlayerAwakenLevelProp.clear();

	m_mapLevel2AwakenStudyGenUnlockPropMap.clear();

	m_mapAwakenStudyAttrProp.clear();

	memset(m_awStudyUnlockLevelInfo, 0, sizeof(m_awStudyUnlockLevelInfo));

	memset(m_adwAwakenCoin, 0, sizeof(m_adwAwakenCoin));
	

	m_mapLevel2OpenNumProp.clear();

	m_mapAwakenClipToStudyProp.clear();
	m_vecAwakenClipToStudyProp.clear();

    m_mapResolveProp.clear();

}


// 从数据源获取数据
BOOL CAwakenPropMgr2::LoadBaseFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("awakenbaseprop2").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
	if(pRes->GetRecord())// 当有行结果时
	{       
        memset(&m_stAwakenBaseProp, 0, sizeof(SAwakenBaseProp2));    		
		m_stAwakenBaseProp.dwGoldAwaken = SGDP::SDAtou(pRes->GetFieldValueByName("GoldAwaken")); 
		m_stAwakenBaseProp.dwGenStudyCoin = SGDP::SDAtou(pRes->GetFieldValueByName("GenStudyCoin"));					
		if(0 == m_stAwakenBaseProp.dwGenStudyCoin)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: GenStudyCoin can not 0!"), MSG_MARK);      
			return FALSE;
		}
		m_stAwakenBaseProp.dwGenStudyClip = SGDP::SDAtou(pRes->GetFieldValueByName("GenStudyClip"));				
		if(0 == m_stAwakenBaseProp.dwGenStudyClip)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: GenStudyExp can not 0!"), MSG_MARK);      
			return FALSE;
		}
		m_stAwakenBaseProp.byMaxStudyLevel = SGDP::SDAtou(pRes->GetFieldValueByName("MaxStudyLevel"));       
		if(0 == m_stAwakenBaseProp.byMaxStudyLevel)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: MaxStudyLevel can not 0!"), MSG_MARK);      
			return FALSE;
		}

		m_stAwakenBaseProp.byCallStudyColorKind = SGDP::SDAtou(pRes->GetFieldValueByName("CallStudyColorKind")); 
		m_stAwakenBaseProp.byCallStudyAttrKind1 = SGDP::SDAtou(pRes->GetFieldValueByName("CallStudyAttrKind1")); 
		m_stAwakenBaseProp.byCallStudyAttrKind2 = SGDP::SDAtou(pRes->GetFieldValueByName("CallStudyAttrKind2")); 
		m_stAwakenBaseProp.byCallStudyLevel = SGDP::SDAtou(pRes->GetFieldValueByName("CallStudyLevel")); 

		UINT32 dwItemID = SGDP::SDAtou(pRes->GetFieldValueByName("ResolveItemID1")); 
		if ( 0 != dwItemID )
		{
			m_setResolveItemID.insert(dwItemID);
		}

		dwItemID = SGDP::SDAtou(pRes->GetFieldValueByName("ResolveItemID2")); 
		if ( 0 != dwItemID )
		{
			m_setResolveItemID.insert(dwItemID);
		}

		dwItemID = SGDP::SDAtou(pRes->GetFieldValueByName("ResolveItemID3")); 
		if ( 0 != dwItemID )
		{
			m_setResolveItemID.insert(dwItemID);
		}
		
	}
	else
	{
		SYS_CRITICAL(_SDT("[%s: %d]: awakenbaseprop have no data!"), MSG_MARK); 
		return FALSE;
	}

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}

BOOL CAwakenPropMgr2::LoadAwakenLevelFromDB()
{	
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("awakenlevelprop2", " order by StartPlayerLevel, AwakenLevel").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}
	UINT8 byRowNum = 0;
	SAwakenLevelProp2* pstProp = NULL;
	UINT16 wLastStartPlayerLevel = 0;
	UINT8 byNumPerLevel = 0;
	UINT8 byNumLevel = 0;
	while(pRes->GetRecord())// 当有行结果时
	{
		UINT16 wStartPlayerLevel = SGDP::SDAtou(pRes->GetFieldValueByName("StartPlayerLevel"));
		if(0 == wStartPlayerLevel)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: StartPlayerLevel can not 0!"), MSG_MARK); 
			return FALSE;
		}
		UINT8 byAwakenLevel = SGDP::SDAtou(pRes->GetFieldValueByName("AwakenLevel"));
		if(byAwakenLevel == GOLD_RUDING_PROP)
		{
			pstProp = &m_mapPlayerAwakenLevelProp[wStartPlayerLevel].m_stRudingGoldAwakenLevelProp;
		}
		else if(byAwakenLevel == GOLD_CHEWU_PROP)
		{
			pstProp = &m_mapPlayerAwakenLevelProp[wStartPlayerLevel].m_stChewuGoldAwakenLevelProp;
		}
		else if((byAwakenLevel <= MAX_AWAKEN_LEVEL) && (0 != byAwakenLevel))
		{
			pstProp = &m_mapPlayerAwakenLevelProp[wStartPlayerLevel].m_astAwakenLevelProp[byAwakenLevel];
		}
		else
		{
			SYS_CRITICAL(_SDT("[%s: %d]: awakenlevelprop invalid StartPlayerLevel[%d], AwakenLevel[%d]!"), MSG_MARK, wStartPlayerLevel, byAwakenLevel); 
			return FALSE;			
		}

		pstProp->wStartPlayerLevel = wStartPlayerLevel;
		pstProp->byAwakenLevel = byAwakenLevel;
		pstProp->dwNeedCoin = SGDP::SDAtou(pRes->GetFieldValueByName("NeedCoin"));		
		pstProp->byNextLevelRate = SGDP::SDAtou(pRes->GetFieldValueByName("NextLevelRate"));
		pstProp->byCoinRate = SGDP::SDAtou(pRes->GetFieldValueByName("CoinRate"));
		pstProp->byClipRate = SGDP::SDAtou(pRes->GetFieldValueByName("ClipRate"));		
		pstProp->byBuleStudyRate = SGDP::SDAtou(pRes->GetFieldValueByName("BuleStudyRate"));
		pstProp->byPurpleStudyRate = SGDP::SDAtou(pRes->GetFieldValueByName("PurpleStudyRate"));
		pstProp->byRedStudyRate = SGDP::SDAtou(pRes->GetFieldValueByName("RedStudyRate"));
		pstProp->byOrangeStudyRate = SGDP::SDAtou(pRes->GetFieldValueByName("OrangeStudyRate"));
		pstProp->vecRandom.push_back(pstProp->byCoinRate);
		pstProp->vecRandom.push_back(pstProp->byClipRate);
		pstProp->vecRandom.push_back(pstProp->byBuleStudyRate);
		pstProp->vecRandom.push_back(pstProp->byPurpleStudyRate);
		pstProp->vecRandom.push_back(pstProp->byRedStudyRate);
		pstProp->vecRandom.push_back(pstProp->byOrangeStudyRate);

		if((byAwakenLevel < MAX_AWAKEN_NUM) && (byAwakenLevel > 0))
		{
			m_adwAwakenCoin[byAwakenLevel-1] = pstProp->dwNeedCoin;
		}
		//读完一个级别的配置
		if((wStartPlayerLevel != wLastStartPlayerLevel) && (0 != wLastStartPlayerLevel))
		{
			if(byNumPerLevel != MAX_AWAKEN_LEVEL+2)
			{
				SYS_CRITICAL(_SDT("[%s: %d]: awakenlevelprop playerlevel[%u] row not match[%u] !"), MSG_MARK, wLastStartPlayerLevel, MAX_AWAKEN_LEVEL+2); 
				return FALSE;
			}
			if(wStartPlayerLevel > wLastStartPlayerLevel)
			{
				for(UINT16 wIdx = wLastStartPlayerLevel+1; wIdx < wStartPlayerLevel; wIdx++)
				{
					m_mapPlayerAwakenLevelProp[wIdx] = m_mapPlayerAwakenLevelProp[wLastStartPlayerLevel];
				}
			}
			byNumPerLevel = 0;
			byNumLevel++;
		}
	
		wLastStartPlayerLevel = wStartPlayerLevel;

		byRowNum++;
		byNumPerLevel++;
	}
	UINT16 wMaxPlayerLevel = CSinglePramMgr::Instance()->GetMaxPlayerLevel();
	if(wLastStartPlayerLevel < wMaxPlayerLevel)
	{
		for(UINT16 wIdx = wLastStartPlayerLevel+1; wIdx <= wMaxPlayerLevel; wIdx++)
		{
			m_mapPlayerAwakenLevelProp[wIdx] = m_mapPlayerAwakenLevelProp[wLastStartPlayerLevel];
		}
		byNumLevel++;
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	if(byRowNum != (MAX_AWAKEN_LEVEL+2) * byNumLevel)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: awakenlevelprop row[%d] not match[%d] !"), MSG_MARK, byRowNum, (MAX_AWAKEN_LEVEL+2) * byNumLevel); 
		return FALSE;
	}

	return TRUE;
}


BOOL CAwakenPropMgr2::LoadUnlockHeroStudyGridFromDB()
{	
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("awakenunlockherostudygridprop2", " order by UnlockGridNum").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}

	m_mapOpenNum2LevelProp.clear();
	BOOL bHaveData = FALSE;
	// 打印查询的表结果
	UINT16 wLastLevel = 0;
	while(pRes->GetRecord())// 当有行结果时
	{
		UINT16 wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("NeedHeroLevel"));
		if(wLevel <= wLastLevel)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: wLevel <= wLastLevel !"), MSG_MARK); 
			return FALSE;
		}
		UINT8 byUnlockGridNum = SGDP::SDAtou(pRes->GetFieldValueByName("UnlockGridNum"));
		if((byUnlockGridNum > MAX_HERO_STUDY_GRID_NUM) || (0 == byUnlockGridNum))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: UnlockGridNum[%d] invalid !"), MSG_MARK, byUnlockGridNum); 
			return FALSE;
		}
		//填充之间的数据
		for(UINT16 wIdx = wLastLevel + 1; wIdx < wLevel; wIdx++)
		{
			m_mapLevel2OpenNumProp[wIdx] = byUnlockGridNum;
		}
		m_mapLevel2OpenNumProp[wLevel] = byUnlockGridNum;
		m_awStudyUnlockLevelInfo[byUnlockGridNum-1] = wLevel;

		m_mapOpenNum2LevelProp[byUnlockGridNum] = wLevel;

		wLastLevel = wLevel;		

		bHaveData = TRUE;
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	if(!bHaveData)	
	{
		SYS_CRITICAL(_SDT("[%s: %d]: awakenunlockherostudygridprop have no data!"), MSG_MARK); 
		return FALSE;
	}

	m_wMaxOpenNumHeroLevel = wLastLevel;

	return TRUE;
}



BOOL CAwakenPropMgr2::LoadLevel2UnlockAwakenStudyFromDB()
{	
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("awakenstudygenunlockprop2", " order by PlayerLevel, StudyColorKind, StudyAttrKind1, StudyAttrKind2").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		if(0 != dwErrID)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: awakenstudygenunlockprop query failed, Err:%s!"), MSG_MARK, strErr.c_str());     
			return FALSE;
		}
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	
	UINT16 wLastLevel = 0; //从0开始才对，否则等级1检查不到
	while(pRes->GetRecord())// 当有行结果时
	{
		SAwakenStudyGenUnlockProp2 stProp;
		stProp.wPlayerLevel = SGDP::SDAtou(pRes->GetFieldValueByName("PlayerLevel"));

		//如果配置中出现了不连贯的等级配置
		if(stProp.wPlayerLevel > wLastLevel + 1)
		{
			//等级1未配置，报错
			if (wLastLevel == 0)
			{
				SYS_CRITICAL(_SDT("[%s: %d]: awakenstudygenunlockprop player level[%d] miss!"), MSG_MARK, wLastLevel+1);
				return FALSE;
			}
			
			//配置中两个不连续的等级之间的配置需要填充
			SPlayerAwakenStudyGenUnlockProp2& stLastLevelProp = m_mapLevel2AwakenStudyGenUnlockPropMap[wLastLevel];
			for(UINT16 wLevelIdx = wLastLevel + 1; wLevelIdx < stProp.wPlayerLevel; wLevelIdx ++)
			{
				m_mapLevel2AwakenStudyGenUnlockPropMap[wLevelIdx] = stLastLevelProp;
			}
		}

		stProp.byStudyColorKind = SGDP::SDAtou(pRes->GetFieldValueByName("StudyColorKind"));
		stProp.byStudyAttrKind1 = SGDP::SDAtou(pRes->GetFieldValueByName("StudyAttrKind1"));
		stProp.byStudyAttrKind2 = SGDP::SDAtou(pRes->GetFieldValueByName("StudyAttrKind2"));
		stProp.byGenRate = SGDP::SDAtou(pRes->GetFieldValueByName("GenRate"));
		
		SPlayerAwakenStudyGenUnlockProp2& stPlayerAwakenStudyGenUnlockProp = m_mapLevel2AwakenStudyGenUnlockPropMap[stProp.wPlayerLevel];
		stPlayerAwakenStudyGenUnlockProp.wPlayerLevel = stProp.wPlayerLevel;
		stPlayerAwakenStudyGenUnlockProp.avecAwakenStudyGenUnlockProp[stProp.byStudyColorKind].push_back(stProp);
		stPlayerAwakenStudyGenUnlockProp.avecRandom[stProp.byStudyColorKind].push_back(stProp.byGenRate);

		wLastLevel = stProp.wPlayerLevel;
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	UINT16 wMaxPlayerLevel = CSinglePramMgr::Instance()->GetMaxPlayerLevel();

	//配置中的最后等级和玩家的最大等级之间的这部分数据需要填充
	if(wLastLevel < wMaxPlayerLevel)
	{
		SPlayerAwakenStudyGenUnlockProp2 stLastLevelProp = m_mapLevel2AwakenStudyGenUnlockPropMap[wLastLevel];
		for(UINT16 wLevelIdx = wLastLevel + 1; wLevelIdx <= wMaxPlayerLevel; wLevelIdx ++)
		{
			m_mapLevel2AwakenStudyGenUnlockPropMap[wLevelIdx] = stLastLevelProp;
		}
	}


	return TRUE;
}


BOOL CAwakenPropMgr2::LoadAwakenStudyAttrFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("awakenstudyattrprop2", " order by StudyColorKind, StudyAttrKind1, StudyAttrKind2, StudyLevel").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}		
	while(pRes->GetRecord())// 当有行结果时
	{
		SAwakenStudyAttrProp2 stProp;
		stProp.byStudyColorKind = SGDP::SDAtou(pRes->GetFieldValueByName("StudyColorKind"));
		stProp.byStudyAttrKind1 = SGDP::SDAtou(pRes->GetFieldValueByName("StudyAttrKind1"));
		stProp.byStudyAttrKind2 = SGDP::SDAtou(pRes->GetFieldValueByName("StudyAttrKind2"));
		stProp.byStudyLevel = SGDP::SDAtou(pRes->GetFieldValueByName("StudyLevel"));
		stProp.dwAttrValue1 = SGDP::SDAtou(pRes->GetFieldValueByName("AttrValue1"));
		stProp.dwAttrValue2 = SGDP::SDAtou(pRes->GetFieldValueByName("AttrValue2"));
		stProp.dwHaveExp = SGDP::SDAtou(pRes->GetFieldValueByName("HaveExp"));
		
		//有效性检查,todo
		m_mapAwakenStudyAttrProp[CLogicFunc::Get8888Key(stProp.byStudyColorKind, stProp.byStudyAttrKind1, stProp.byStudyAttrKind2, stProp.byStudyLevel)] = stProp;
		if ( EC_ORANGE == stProp.byStudyColorKind)
		{
			CAwakenStudyAttrPropMapItr2 itr = m_mapResolveOrangeProp.find(stProp.byStudyAttrKind1);
			if ( itr == m_mapResolveOrangeProp.end())
			{
				m_mapResolveOrangeProp[stProp.byStudyAttrKind1] = stProp;
			}
		}
	}

	if(m_mapAwakenStudyAttrProp.end() == m_mapAwakenStudyAttrProp.find(CLogicFunc::Get8888Key(m_stAwakenBaseProp.byCallStudyColorKind, m_stAwakenBaseProp.byCallStudyAttrKind1, m_stAwakenBaseProp.byCallStudyAttrKind2, m_stAwakenBaseProp.byCallStudyLevel)))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CallStudy prop not cfg!"), MSG_MARK);      
		return FALSE;
	}

	

	return TRUE;
}

BOOL CAwakenPropMgr2::LoadAwakenClipToStudyFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("awakencliptostudyprop2", " order by NeedPlayerLevel, StudyColorKind, StudyAttrKind1, StudyAttrKind2, StudyLevel").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}		
	while(pRes->GetRecord())// 当有行结果时
	{
		SAwakenClipToStudyProp2 stProp;
		stProp.byStudyColorKind = SGDP::SDAtou(pRes->GetFieldValueByName("StudyColorKind"));
		stProp.byStudyAttrKind1 = SGDP::SDAtou(pRes->GetFieldValueByName("StudyAttrKind1"));
		stProp.byStudyAttrKind2 = SGDP::SDAtou(pRes->GetFieldValueByName("StudyAttrKind2"));
		stProp.byStudyLevel = SGDP::SDAtou(pRes->GetFieldValueByName("StudyLevel"));
		stProp.dwNeedCoin = SGDP::SDAtou(pRes->GetFieldValueByName("NeedCoin"));
		stProp.dwNeedClip = SGDP::SDAtou(pRes->GetFieldValueByName("NeedClip"));
		stProp.wNeedPlayerLevel = SGDP::SDAtou(pRes->GetFieldValueByName("NeedPlayerLevel"));
		if(m_mapAwakenStudyAttrProp.end() == m_mapAwakenStudyAttrProp.find(CLogicFunc::Get8888Key(stProp.byStudyColorKind, stProp.byStudyAttrKind1, stProp.byStudyAttrKind2, stProp.byStudyLevel)))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: LoadAwakenClipToStudyFromDB not found study[%d.%d.%d.%d]!"), MSG_MARK, stProp.byStudyColorKind, stProp.byStudyAttrKind1, stProp.byStudyAttrKind2, stProp.byStudyLevel);      
			return FALSE;
		}
		m_mapAwakenClipToStudyProp[CLogicFunc::Get8888Key(stProp.byStudyColorKind, stProp.byStudyAttrKind1, stProp.byStudyAttrKind2, stProp.byStudyLevel)] = stProp;
		m_vecAwakenClipToStudyProp.push_back(stProp);
		
		
	}


	return TRUE;
}

UINT32 CAwakenPropMgr2::GetAwakenCoin(UINT16 wPlayerLevel, UINT8 byGoldFlag, UINT8 byAwakenLevel)
{
	CPlayerAwakenLevelPropMapItr2 itr = m_mapPlayerAwakenLevelProp.lower_bound(wPlayerLevel);
	if(itr == m_mapPlayerAwakenLevelProp.end())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: playerlevel[%u] awaken prop not cfg!"), MSG_MARK, wPlayerLevel);   
		return 0xFFFFFFFF;
	}

	if((0 == byAwakenLevel) || (byAwakenLevel > MAX_AWAKEN_LEVEL)) 
	{ 
		return 0xFFFFFFFF; 
	}
	if((1 == byGoldFlag) && (EALK_RUDING == byAwakenLevel))
	{
		return itr->second.m_stRudingGoldAwakenLevelProp.dwNeedCoin;
	}
	else if((1 == byGoldFlag) && (EALK_CHEWU == byAwakenLevel))
	{
		return itr->second.m_stChewuGoldAwakenLevelProp.dwNeedCoin;
	}
	return itr->second.m_astAwakenLevelProp[byAwakenLevel].dwNeedCoin;
}
	

UINT8 CAwakenPropMgr2::AwakenStudy(UINT16 wPlayerLevel, BOOL bOpenExpStudy, UINT8 byAwakenLevel, UINT8 byGoldFlag, UINT8& byStudyColorKind, UINT8& byStudyAttrKind1, UINT8& byStudyAttrKind2, 
	UINT32& dwGenCoin, UINT32& dwGenClip, UINT8& byZTSUseTime)
{
	UINT8 byNextAwakenLevel = EALK_KESHUI;
	byStudyColorKind = 0;
	byStudyAttrKind1 = 0;
	byStudyAttrKind2 = 0;
	dwGenCoin = 0;
	dwGenClip = 0;
	if(((0 == byAwakenLevel) || (byAwakenLevel > MAX_AWAKEN_LEVEL) || (0 == wPlayerLevel)
		|| (wPlayerLevel > CSinglePramMgr::Instance()->GetMaxPlayerLevel()))) 
	{
		return byNextAwakenLevel;
	}
	CPlayerAwakenLevelPropMapItr2 itr = m_mapPlayerAwakenLevelProp.lower_bound(wPlayerLevel);
	if(!bOpenExpStudy)
	{
		//未开放兑换功能则取前一个配置
		if((itr != m_mapPlayerAwakenLevelProp.end()) && (itr != m_mapPlayerAwakenLevelProp.begin()))
		{
			itr--;
		}		
	}
	if(itr == m_mapPlayerAwakenLevelProp.end())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: playerlevel[%u] awaken prop not cfg!"), MSG_MARK, wPlayerLevel);   
		return byNextAwakenLevel;
	}
	SAwakenLevelProp2* pstAwakenLevelProp = NULL;
	if((1 == byGoldFlag) && (EALK_RUDING == byAwakenLevel))
	{
		pstAwakenLevelProp = &itr->second.m_stRudingGoldAwakenLevelProp;
	}
	else if((1 == byGoldFlag) && (EALK_CHEWU == byAwakenLevel))
	{
		pstAwakenLevelProp = &itr->second.m_stChewuGoldAwakenLevelProp;
	}
	else
	{
		pstAwakenLevelProp = &itr->second.m_astAwakenLevelProp[byAwakenLevel];
	}

	//计算悟道下一级的概率
	if(RandomReseed(100) < pstAwakenLevelProp->byNextLevelRate)
	{
		byNextAwakenLevel = byAwakenLevel+1;
		if(byNextAwakenLevel > EALK_CHEWU)
		{
			byNextAwakenLevel = EALK_KESHUI;
		}
	}
	else
	{
		byNextAwakenLevel = EALK_KESHUI;
	}
	
	//计算生成的心得颜色
	INT32 nHit = GetRandomIdxMustHit(pstAwakenLevelProp->vecRandom);	
	if(0 == nHit)
	{
		dwGenCoin = m_stAwakenBaseProp.dwGenStudyCoin;
		return byNextAwakenLevel;
	}
	else if(1 == nHit)
	{
		dwGenClip = m_stAwakenBaseProp.dwGenStudyClip;
		return byNextAwakenLevel;
	}
	else if(nHit > 5)
	{
		dwGenCoin = m_stAwakenBaseProp.dwGenStudyCoin;
		return byNextAwakenLevel;
	}
	else
	{
		byStudyColorKind = nHit - 1;
	}

	//伪概率出黄卦
	if( EALK_CHEWU == byAwakenLevel)
	{
		if ( EC_ORANGE == byStudyColorKind)
		{
			byZTSUseTime = 0;
		}
		else
		{
			if ( CSinglePramMgr::Instance()->GetSureOutOrangeAwaken() <= byZTSUseTime )
			{
				byStudyColorKind = EC_ORANGE;
				byZTSUseTime = 0;
			}
			else
			{
				byZTSUseTime += 1;
			}
		}
		
	}
	
	//计算生成的心得类型
	SPlayerAwakenStudyGenUnlockProp2& stColor = m_mapLevel2AwakenStudyGenUnlockPropMap[wPlayerLevel];
	CRandomVec* poRandomVec = NULL;
	poRandomVec = &(stColor.avecRandom[byStudyColorKind]);
	if(0 == poRandomVec->size()) //没有解锁该颜色，则取蓝色
	{
		byStudyColorKind = EC_BLUE;
		poRandomVec = &(stColor.avecRandom[byStudyColorKind]);		
	}
	if(0 == poRandomVec->size())
	{
		byStudyColorKind = 0;
		return byNextAwakenLevel;
	}	
	nHit = GetRandomIdxMustHit(*poRandomVec);	
	byStudyAttrKind1 = stColor.avecAwakenStudyGenUnlockProp[byStudyColorKind][nHit].byStudyAttrKind1;
	byStudyAttrKind2 = stColor.avecAwakenStudyGenUnlockProp[byStudyColorKind][nHit].byStudyAttrKind2;

	return byNextAwakenLevel;
}


UINT32 CAwakenPropMgr2::GetStudyExp(UINT8 byStudyColorKind, UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2, UINT8 byStudyLevel)
{
	CAwakenStudyAttrPropMapItr2 itr = m_mapAwakenStudyAttrProp.find(CLogicFunc::Get8888Key(byStudyColorKind, byStudyAttrKind1, byStudyAttrKind2, byStudyLevel));
	if(itr != m_mapAwakenStudyAttrProp.end())
	{
		return itr->second.dwHaveExp;
	}

	return 0;
}

UINT32 CAwakenPropMgr2::GetStudyUpgradeNeedExp(UINT8 byStudyColorKind, UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2, UINT8 byStudyCurLevel)
{
	UINT32 dwCurLevelExp = 0;
	UINT32 dwNextLevelExp = 0;
	CAwakenStudyAttrPropMapItr2 itrCur = m_mapAwakenStudyAttrProp.find(CLogicFunc::Get8888Key(byStudyColorKind, byStudyAttrKind1, byStudyAttrKind2, byStudyCurLevel));
	if(itrCur == m_mapAwakenStudyAttrProp.end())
	{
		return 0xFFFFFFFF;
	}
	dwCurLevelExp = itrCur->second.dwHaveExp;
	CAwakenStudyAttrPropMapItr2 itrNext = m_mapAwakenStudyAttrProp.find(CLogicFunc::Get8888Key(byStudyColorKind, byStudyAttrKind1, byStudyAttrKind2, byStudyCurLevel+1));
	if(itrNext == m_mapAwakenStudyAttrProp.end())
	{
		return 0xFFFFFFFF;
	}
	dwNextLevelExp = itrNext->second.dwHaveExp;
	
	UINT32 dwCostExp = dwNextLevelExp < dwCurLevelExp ? 0 : dwNextLevelExp - dwCurLevelExp;
	return dwCostExp;
}

BOOL CAwakenPropMgr2::CkUnlockHeroGrid(UINT16 wHeroLevel, UINT8 byGridIdx)
{
	C1616MapItr itr = m_mapOpenNum2LevelProp.find(byGridIdx+1);
	if(itr != m_mapOpenNum2LevelProp.end())
	{
		if(itr->second <= wHeroLevel)
		{
			return TRUE;
		}
	}

	return FALSE;
}


VOID CAwakenPropMgr2::GetStudyAttrValue(UINT8 byStudyColorKind, UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2, UINT8 byStudyLevel, UINT32& dwAttrValue1, UINT32& dwAttrValue2)
{
	dwAttrValue1 = 0;
	dwAttrValue2 = 0;
	CAwakenStudyAttrPropMapItr2 itr = m_mapAwakenStudyAttrProp.find(CLogicFunc::Get8888Key(byStudyColorKind, byStudyAttrKind1, byStudyAttrKind2, byStudyLevel));
	if(itr != m_mapAwakenStudyAttrProp.end())
	{
		dwAttrValue1 =  itr->second.dwAttrValue1;
		dwAttrValue2 =  itr->second.dwAttrValue2;
	}
}

SAwakenClipToStudyProp2* CAwakenPropMgr2::GetClipToStudyProp(UINT8 byStudyColorKind, UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2, UINT8 byStudyLevel)
{
	CAwakenClipToStudyPropMapItr2 itr = m_mapAwakenClipToStudyProp.find(CLogicFunc::Get8888Key(byStudyColorKind, byStudyAttrKind1, byStudyAttrKind2, byStudyLevel));
	if(itr != m_mapAwakenClipToStudyProp.end())
	{
		return &itr->second;
	}

	return NULL;
}

UINT8 CAwakenPropMgr2::GetRandomCliByItemID(UINT16 wItemID)
{
	SGoodsProp* poGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wItemID);
	if ( NULL == poGoodsProp)
	{
		return 0;
	}

	CSetResolveItemIDItr itr = m_setResolveItemID.find(wItemID);
	if ( itr == m_setResolveItemID.end())
	{
		return 0;
	}

	return Random(poGoodsProp->dwParam1, poGoodsProp->dwParam2);
}

BOOL CAwakenPropMgr2::LoadAwakenResolvePropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("awakenresolveprop").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
        return FALSE;
    }	
    while(pRes->GetRecord())// 当有行结果时
    {
        SAwakenResolveProp stProp;
        stProp.byStudyColorKind = SGDP::SDAtou(pRes->GetFieldValueByName("AwakenColorKind"));
        stProp.byIfResolve = SGDP::SDAtou(pRes->GetFieldValueByName("IfResolve"));
        stProp.dwCommonCoin = SGDP::SDAtou(pRes->GetFieldValueByName("AwakenCommonCoin"));
        stProp.dwSpecialGold = SGDP::SDAtou(pRes->GetFieldValueByName("AwakenSpecialGold"));
        stProp.wCommonNum= SGDP::SDAtou(pRes->GetFieldValueByName("AwakenCommonNum"));
        stProp.wSpecialNum = SGDP::SDAtou(pRes->GetFieldValueByName("AwakenSpecialNum"));

        m_mapResolveProp[stProp.byStudyColorKind] = stProp;

    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

SAwakenResolveProp* CAwakenPropMgr2::GetAwakenResolvePropByID( UINT8 byStudyColorKind )
{
    CAwakenResolvePropMapItr itr = m_mapResolveProp.find(byStudyColorKind);
    if (itr == m_mapResolveProp.end())
    {
        return NULL;
    }
    return &itr->second;
}

UINT8 CAwakenPropMgr2::GetAwakenIfResolveByID( UINT8 byStudyColorKind )
{
    CAwakenResolvePropMapItr itr = m_mapResolveProp.find(byStudyColorKind);
    if (itr == m_mapResolveProp.end())
    {
        return 0;
    }
    return itr->second.byIfResolve;
}
