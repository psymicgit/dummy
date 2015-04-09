#include "logic/specialeffects/specialeffectsmgr.h"
#include "logic/specialeffects/specialeffects.h"
#include "framework/gsapi.h"
#include "sddb.h"
#include <sdloggerimpl.h>
#include "db/dbmgr.h"
#include "logic/soul/soul.h"
IMPLEMENT_SINGLETON_PROPMGR(CSpecialEffectPropMgr)

BOOL CSpecialEffectPropMgr::Init()
{
	if ( !LoadPropFromDB())
	{
		return FALSE;
	}

	return TRUE;
}

VOID CSpecialEffectPropMgr::UnInit()
{
	m_mapSpecialEffect.clear();
}

BOOL CSpecialEffectPropMgr::LoadPropFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();

	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs("select * from specialeffectprop  order by EffectID, EffectSmallType", &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: specialeffectprop has no data!"), MSG_MARK);
		return FALSE;
	}

	while(pRes->GetRecord())
	{
		SSpecialEffectProp stProp;
		stProp.wEffectID = SDAtou(pRes->GetFieldValueByName("EffectID"));
		stProp.strEffectName = pRes->GetFieldValueByName("EffectName");
		stProp.byEffectBigType = SDAtou(pRes->GetFieldValueByName("EffectBigType"));
		stProp.byEffectSmallType = SDAtou(pRes->GetFieldValueByName("EffectSmallType"));
		stProp.byUnlockLevel = SDAtou(pRes->GetFieldValueByName("UnlockLevel"));
		stProp.wTriggerProb = SDAtou(pRes->GetFieldValueByName("TriggerProb"));
		stProp.wEffectPercent = SDAtou(pRes->GetFieldValueByName("EffectPercent"));
		stProp.wLimitNum = SDAtou(pRes->GetFieldValueByName("LimitNum"));

		m_mapSpecialEffect.insert(make_pair( stProp.wEffectID, stProp));
		m_mapSpecialEffectByType[CLogicFunc::Get1616Key(stProp.wEffectID, stProp.byEffectSmallType)] = stProp;
	}

	if(pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

const SSpecialEffectProp* CSpecialEffectPropMgr::GetSpecialEffectProp(UINT16 wEffectID, UINT8 bySmallType)
{
	CMapTypeSpecialEffectItr itr = m_mapSpecialEffectByType.find(CLogicFunc::Get1616Key(wEffectID, bySmallType));
	if ( itr == m_mapSpecialEffectByType.end())
	{
		return NULL;
	}

	return &itr->second;
}

UINT16 CSpecialEffectPropMgr::GetTriggerProb(UINT16 wEffectID, UINT8 bySmallType)
{
	const SSpecialEffectProp* poProp = GetSpecialEffectProp(wEffectID, bySmallType);
	if ( NULL == poProp)
	{
		return 0;
	}
	return poProp->wTriggerProb;
}

UINT16 CSpecialEffectPropMgr::GetEffectPercent(UINT16 wEffectID, UINT8 bySmallType)
{
	const SSpecialEffectProp* poProp = GetSpecialEffectProp(wEffectID, bySmallType);
	if ( NULL == poProp)
	{
		return 0;
	}
	return poProp->wEffectPercent;
}

UINT16 CSpecialEffectPropMgr::GetLimitNum(UINT16 wEffectID, UINT8 bySmallType)
{
	const SSpecialEffectProp* poProp = GetSpecialEffectProp(wEffectID, bySmallType);
	if ( NULL == poProp)
	{
		return 0;
	}
	return poProp->wLimitNum;
}

BOOL CSpecialEffectPropMgr::CreateSpecialEffect(CPlayer* poPlayer, CBattleAttrObj* poMySelf, UINT32 dwEffectID, list<CSpecialEffect*>& listSpecialEffect)
{
	listSpecialEffect.clear();
	DT_SPECIAL_EFFECT_DATA stSpecialData = {0};
	if( !GenerateSpecialEffectData(static_cast<UINT16>(dwEffectID), stSpecialData) )
	{
		return FALSE;
	}
	UINT8 byIdx = 0;
	for ( ; byIdx < stSpecialData.bySkillNum; byIdx++)
	{
		CSpecialEffect* poEffect = CreateSpecialEffect(poPlayer, poMySelf, stSpecialData, byIdx);
		if ( NULL == poEffect)
		{
			FreeSpecialEffect(listSpecialEffect);
		}
		listSpecialEffect.push_back(poEffect);
	}
	return TRUE;
}

CSpecialEffect* CSpecialEffectPropMgr::CreateSpecialEffect(CPlayer* poPlayer, CBattleAttrObj* poMySelf, DT_SPECIAL_EFFECT_DATA& stSpecialData, UINT8 bySelectIdx)
{
	CSpecialEffect* poEffect = NULL;
	switch (stSpecialData.abyEffectSmallType[bySelectIdx])
	{
	case EBSE_CONTINUE_ATTACK:
		poEffect = CContinueAttackEffectPoolMgr::Instance()->Create();
		break;
	case EBSE_SUCK_BLOOD:
		poEffect = CSuckBloodEffectPoolMgr::Instance()->Create();
		break;
	case EBSE_PHYSICAL_IMMUNITY:
		poEffect = CPhysicalImmunityEffectPoolMgr::Instance()->Create();
		break;
	case EBSE_MAGIC_IMMUNITY:
		poEffect = CMagicImmunityEffectPoolMgr::Instance()->Create();
		break;
	case EBSE_ADD_BLOOD:
		poEffect = CAddBloodEffectPoolMgr::Instance()->Create();
		break;
	case EBSE_REBOUND:
		poEffect = CReboundEffectPoolMgr::Instance()->Create();
		break;
	case EBSE_BEAT_BACK:
		poEffect = CBeatBackPoolMgr::Instance()->Create();
		break;
	case EBSE_DODGE:
		poEffect = CDodgeEffectPoolMgr::Instance()->Create();
		break;
	default:
		return NULL;
	}

	if( !poEffect->Init(poPlayer, poMySelf, stSpecialData, bySelectIdx))
	{
		FreeSpecialEffect(stSpecialData.abyEffectSmallType[bySelectIdx], poEffect);
		USR_INFO(_SDT("[%s:%u] SpecialEffect Init Failed, ID = %u "), MSG_MARK, stSpecialData.wEffectID);
		return NULL;
	}
	return poEffect;
}
VOID CSpecialEffectPropMgr::FreeSpecialEffect(list<CSpecialEffect*>& poEffect)
{
	for ( list<CSpecialEffect*>::iterator itr = poEffect.begin(); itr != poEffect.end(); itr++)
	{
		FreeSpecialEffect((*itr)->GetEffectSmallType(), *itr);
	}
}

VOID CSpecialEffectPropMgr::FreeSpecialEffect(UINT8 byEffectType, CSpecialEffect*  poEffect)
{
	if ( NULL == poEffect)
	{
		return ;
	}
	
	switch (byEffectType)
	{
	case EBSE_CONTINUE_ATTACK:
		CContinueAttackEffectPoolMgr::Instance()->Remove((CContinueAttackEffect*)poEffect);
		break;
	case EBSE_SUCK_BLOOD:
		CSuckBloodEffectPoolMgr::Instance()->Remove((CSuckBloodEffect*)poEffect);
		break;
	case EBSE_PHYSICAL_IMMUNITY:
		CPhysicalImmunityEffectPoolMgr::Instance()->Remove((CPhysicalImmunityEffect*)poEffect);
		break;
	case EBSE_MAGIC_IMMUNITY:
		CMagicImmunityEffectPoolMgr::Instance()->Remove((CMagicImmunityEffect*)poEffect);
		break;
	case EBSE_ADD_BLOOD:
		CAddBloodEffectPoolMgr::Instance()->Remove((CAddBloodEffect*)poEffect);
		break;
	case EBSE_REBOUND:
		CReboundEffectPoolMgr::Instance()->Remove((CReboundEffect*)poEffect);
		break;
	case EBSE_BEAT_BACK:
		CBeatBackPoolMgr::Instance()->Remove((CBeatBack*)poEffect);
		break;
	case EBSE_DODGE:
		CDodgeEffectPoolMgr::Instance()->Remove((CDodgeEffect*)poEffect);
		break;
	default:
		break;
	}
}
//
//BOOL CSpecialEffectPropMgr::GetEffectRandomMap(UINT8 byAttackType, UINT8 byStarLevel,const CRandomMap*& mapRandom)
//{
//	if ( EAT_ATTACK == byAttackType )
//	{
//		CLevelRandomItr itr = m_mapAttackRandom.find(byStarLevel);
//		if ( itr != m_mapAttackRandom.end())
//		{
//			mapRandom = &itr->second;
//			return TRUE;
//		}
//	}
//	else if ( EAT_DEFEND == byAttackType)
//	{
//		CLevelRandomItr itr = m_mapDefendRandom.find(byStarLevel);
//		if ( itr != m_mapDefendRandom.end())
//		{
//			mapRandom = &itr->second;
//			return TRUE;
//		}
//	}
//	return FALSE;
//}
//
BOOL CSpecialEffectPropMgr::GenerateSpecialEffectData(UINT16 wEffectID, DT_SPECIAL_EFFECT_DATA& stData)
{
	memset(&stData, 0, sizeof(DT_SPECIAL_EFFECT_DATA));
	if ( 0 == m_mapSpecialEffect.count(wEffectID))
	{
		return FALSE;
	}
	stData.wEffectID = wEffectID;
	stData.byIsActivity = ESM_NO;
	pair<CMapSpecialEffectItr, CMapSpecialEffectItr> pairItr = m_mapSpecialEffect.equal_range(wEffectID);
	UINT8 byIdx = 0;
	for ( CMapSpecialEffectItr itr = pairItr.first; itr != pairItr.second; itr++, byIdx++)
	{
		stData.abyEffectSmallType[byIdx] = itr->second.byEffectSmallType;
	}
	stData.bySkillNum = byIdx;
	return TRUE;
}

UINT8 CSpecialEffectPropMgr::GetUnlockLevel(UINT16 wEffectID)
{
	CMapSpecialEffectItr itr = m_mapSpecialEffect.find(wEffectID);
	if ( itr == m_mapSpecialEffect.end())
	{
		return 0;
	}
	return itr->second.byUnlockLevel;
}

VOID CSpecialEffectPropMgr::GetSpecialEffectDataCli(UINT16 wEffectID, UINT8 bySmallType, DT_SPECIAL_EFFECT_DESC& stCli)
{
	stCli.byEffectType = bySmallType;

	stCli.wLimitNumValue = CSpecialEffectPropMgr::Instance()->GetLimitNum(wEffectID, bySmallType);

	stCli.wTriggerProbValue = CSpecialEffectPropMgr::Instance()->GetTriggerProb(wEffectID, bySmallType);

	stCli.wEffectPercentValue = CSpecialEffectPropMgr::Instance()->GetEffectPercent(wEffectID, bySmallType);

}