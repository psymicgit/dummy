#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <logic/skill/skillpropmgr.h>
#include <sdloggerimpl.h>
#include <framework/gsapi.h>
#include "monsterpropmgr.h"

#include <logic/base/logicfunc.h>

#include <db/dbmgr.h>

#define SQL_READ_MONSTER_STRING "select * from monsterprop"


IMPLEMENT_SINGLETON_PROPMGR(CMonsterPropMgr);

CMonsterPropMgr::CMonsterPropMgr()
{
}

CMonsterPropMgr::~CMonsterPropMgr()
{
}

BOOL CMonsterPropMgr::Init()
{
	if(!LoadFromDB())
	{
		return FALSE;
	}

    return TRUE;
}

VOID CMonsterPropMgr::UnInit()
{
	m_mapKind2MonsterProp.clear();
	m_mapKindLvl2MonsterProp.clear();
	m_mapKind2ShadeMonsterProp.clear();
}



// 从数据源获取数据
BOOL CMonsterPropMgr::LoadFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("monsterprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SMonsterProp stMonsterProp;
		memset(&stMonsterProp, 0, sizeof(SMonsterProp));
		stMonsterProp.wKindID = SGDP::SDAtou(pRes->GetFieldValueByName("KindID"));	
		_SDTStrncpy(stMonsterProp.szName, _SDTUTF82T(pRes->GetFieldValueByName("Name")), NAME_LEN_MAX);
		stMonsterProp.byType = SGDP::SDAtou(pRes->GetFieldValueByName("Type"));
		stMonsterProp.byFuncType = SGDP::SDAtou(pRes->GetFieldValueByName("FuncType"));
		stMonsterProp.byCareerID = SGDP::SDAtou(pRes->GetFieldValueByName("CareerID"));	
		stMonsterProp.wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
		stMonsterProp.qwHP = SGDP::SDAtou64(pRes->GetFieldValueByName("HP"));
		stMonsterProp.dwAttack = SGDP::SDAtou(pRes->GetFieldValueByName("Attack"));
		stMonsterProp.dwDefend = SGDP::SDAtou(pRes->GetFieldValueByName("Defend"));
		stMonsterProp.wHitRate = SGDP::SDAtou(pRes->GetFieldValueByName("HitRate"));
		stMonsterProp.wDodgeRate = SGDP::SDAtou(pRes->GetFieldValueByName("DodgeRate"));
		stMonsterProp.wCritRate = SGDP::SDAtou(pRes->GetFieldValueByName("CritRate"));
		stMonsterProp.wDeCritRate = SGDP::SDAtou(pRes->GetFieldValueByName("DeCritRate"));
		stMonsterProp.wAngryValue = SGDP::SDAtou(pRes->GetFieldValueByName("AngryValue"));
		stMonsterProp.wSkillActionID = SGDP::SDAtou(pRes->GetFieldValueByName("SkillActionID"));	
		if(0 != stMonsterProp.wSkillActionID)
		{
			stMonsterProp.wSkillActionLevel = SGDP::SDAtou(pRes->GetFieldValueByName("SkillActionLevel"));	
			if(!CSkillPropMgr::Instance()->GetSkillActionProp(stMonsterProp.wSkillActionID, stMonsterProp.wSkillActionLevel))
			{
				SYS_CRITICAL(_SDT("[%s: %d]: Monster[%d] skill action[%d.%d] not exist!"), MSG_MARK, stMonsterProp.wKindID, stMonsterProp.wSkillActionID, stMonsterProp.wSkillActionLevel);    
				return FALSE;
			}
			stMonsterProp.wSkillEffectID = SGDP::SDAtou(pRes->GetFieldValueByName("SkillEffectID"));				
			if((0 != stMonsterProp.wSkillEffectID) && (!CSkillPropMgr::Instance()->GetSkillEffectProp(stMonsterProp.wSkillEffectID)))
			{
				SYS_CRITICAL(_SDT("[%s: %d]: Monster[%d] skill effect[%da] not exist!"), MSG_MARK, stMonsterProp.wKindID, stMonsterProp.wSkillEffectID);    
				return FALSE;
			}	
		}
		
		stMonsterProp.wGodweaponQuality = SGDP::SDAtou(pRes->GetFieldValueByName("GodweaponQuality"));	
		stMonsterProp.wGodweaponLevel = SGDP::SDAtou(pRes->GetFieldValueByName("GodweaponLevel"));	
		stMonsterProp.dwGodweaponHP = SGDP::SDAtou(pRes->GetFieldValueByName("GodweaponHP"));	
		stMonsterProp.dwGodweaponAttack = SGDP::SDAtou(pRes->GetFieldValueByName("GodweaponAttack"));	
		stMonsterProp.wFakeID = SGDP::SDAtou(pRes->GetFieldValueByName("FakeID"));	
		stMonsterProp.wFakeID = stMonsterProp.wFakeID == 0 ? stMonsterProp.wKindID : stMonsterProp.wFakeID;

		if (EMFT_HUNTING == stMonsterProp.byFuncType)
		{
			m_mapKind2ShadeMonsterProp[stMonsterProp.wKindID] = stMonsterProp;
		}
		else
		{
			m_mapKind2MonsterProp[stMonsterProp.wKindID] = stMonsterProp;
			m_mapKindLvl2MonsterProp[CLogicFunc::Get1616Key(stMonsterProp.wKindID, stMonsterProp.wLevel)] = stMonsterProp;
		}

	}
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}

SMonsterProp* CMonsterPropMgr::GetShadeMonsterProp(UINT16 wKindID)
{
	CMonsterPropMapItr itr = m_mapKind2ShadeMonsterProp.find(wKindID);
	if(itr != m_mapKind2ShadeMonsterProp.end())
	{
		return &itr->second;
	}

	return NULL;
}

SMonsterProp* CMonsterPropMgr::GetMonsterProp(UINT16 wKindID)
{
	CMonsterPropMapItr itr = m_mapKind2MonsterProp.find(wKindID);
	if(itr != m_mapKind2MonsterProp.end())
	{
		return &itr->second;
	}

	return NULL;
}

SMonsterProp* CMonsterPropMgr::GetMonsterProp(UINT16 wKindID, UINT16 wLevel)
{
	CMonsterPropMapItr itr = m_mapKindLvl2MonsterProp.find(CLogicFunc::Get1616Key(wKindID, wLevel));
	if(itr != m_mapKindLvl2MonsterProp.end())
	{
		return &itr->second;
	}

	return NULL;
}