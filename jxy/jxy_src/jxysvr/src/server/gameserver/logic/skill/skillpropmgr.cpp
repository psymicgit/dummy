#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/player/player.h>
#include <framework/gsapi.h>
#include "skillpropmgr.h"
#include "skillaffecthandler.h"

#include <db/dbmgr.h>

using namespace SGDP;


IMPLEMENT_SINGLETON_PROPMGR(CSkillPropMgr);

CSkillPropMgr::CSkillPropMgr()
{
    memset(&m_stSkillBaseProp, 0, sizeof(m_stSkillBaseProp));
}

CSkillPropMgr::~CSkillPropMgr()
{
}

BOOL CSkillPropMgr::Init()
{
    if(!LoadSkillBaseFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadSkillBaseFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadSkillEffectRecommendPropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadSkillEffectRecommendPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadSkillEffectPropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadSkillLevelPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadSkillActionPropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadSkillLevelPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

	if ( !LoadPetSkillFromDB())
	{
		SYS_CRITICAL(_SDT("[%s: %d]: LoadPetSkillFromDB failed!"), MSG_MARK);
		return FALSE;
	}

    return TRUE;
}

VOID CSkillPropMgr::UnInit()
{
    //m_mapIDLevel2SkillProp.clear();
    m_mapIDLevel2SkillActionProp.clear();
    m_mapIDLevel2SkillEffectProp.clear();
    m_mapMaxSkillActionLevel.clear();
    m_mapSkillEffectRecommendProp.clear();
    m_mapRang2ActionProp.clear();
    m_mapEffect2ActionProp.clear();
    m_mapActionID2AllRang.clear();
    m_mapActionID2AllEffect.clear();
	m_mapPetSkillProp.clear();
	memset(&m_stSkillBaseProp, 0, sizeof(m_stSkillBaseProp));
}

BOOL CSkillPropMgr::LoadPetSkillFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("skillpetprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
		return FALSE;
	}

	// 打印查询的表结果
	UINT16 wLastID = 0;
	UINT16 wLastLevel = 0;
	UINT16 wMaxLevel = 0;
	while(pRes->GetRecord())// 当有行结果时
	{
		UINT8 bySkillType = SDAtou(pRes->GetFieldValueByName("SkillType"));
		string strXmlValue = pRes->GetFieldValueByName("SkillValue");
		if ( EPST_FIX == bySkillType )
		{
			if ( !ParsePetSkill(strXmlValue))
			{
				SYS_CRITICAL(_SDT("[%s: %d]: Parse Pet Skill failed!"), MSG_MARK);
				return FALSE;
			}
		}else if ( EPST_RANDOM == bySkillType)
		{
			if (!ParseRandomPetSkill(strXmlValue) )
			{
				SYS_CRITICAL(_SDT("[%s: %d]: Parse Pet Skill failed!"), MSG_MARK);
				return FALSE;
			}
		}
		
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}

BOOL CSkillPropMgr::ParseRandomPetSkill(const string& strXMLValue)
{
	SGDP::CSDXMLFile xmlFile;

	if(FALSE == xmlFile.LoadData(strXMLValue.c_str(), strXMLValue.length()))
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

	CSDXMLNode skillXmlProp = rootNode["Skill"];
	if ( skillXmlProp == NULL )
	{
		SYS_CRITICAL(_SDT("[%s: %d]: xml obj err!"), MSG_MARK);
		return FALSE;
	}
	UINT16 wSkillID = skillXmlProp("ID").AsInteger();
	CSDXMLNode levelXmlProp = skillXmlProp["Level"];
	while(levelXmlProp != NULL)
	{
		SRandomPetSkillProp stProp;
		stProp.stMainProp.wSkillID = wSkillID;
		stProp.stMainProp.wSkillLevel = levelXmlProp("Value").AsInteger();
		stProp.stMainProp.strSkillName = levelXmlProp("Name").AsString();
		stProp.stMainProp.strSkillDesc = levelXmlProp("Desc").AsString();
		stProp.stMainProp.wFinalHurtValue = levelXmlProp("IncreaRate").AsInteger();
		stProp.stMainProp.byMaxTargetNum = levelXmlProp("MaxTarget").AsInteger();
		CSDXMLNode skillXmlProp = levelXmlProp["Skills"];
		if ( skillXmlProp == NULL)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: Parse Pet SKill failed!"), MSG_MARK);
			return FALSE;
		}
		skillXmlProp = skillXmlProp["Skill"];
		while ( skillXmlProp != NULL)
		{
			UINT16 wRandomSkillID = skillXmlProp("ID").AsInteger();
			UINT16 wIncreaRate = skillXmlProp("IncreaRate").AsInteger();
			CMapPetSkillEffectPropItr itr = m_mapPetSkillProp.find(CLogicFunc::Get1616Key(wRandomSkillID, 1));
			if ( itr != m_mapPetSkillProp.end())
			{
				SPetSkillEffectProp stEffectProp = itr->second;
				stEffectProp.wFinalHurtValue += wIncreaRate;
				stProp.vecRandomProp.push_back(stEffectProp);
			}
			skillXmlProp = skillXmlProp.Sibling("Skill");
		}

		levelXmlProp = levelXmlProp.Sibling("Level");
		m_mapRandomPetSkillProp[CLogicFunc::Get1616Key(wSkillID, stProp.stMainProp.wSkillLevel)] = stProp;
	}
	return TRUE;
}

BOOL CSkillPropMgr::ParsePetSkill(const string& strXMLValue)
{
	SGDP::CSDXMLFile xmlFile;

	if(FALSE == xmlFile.LoadData(strXMLValue.c_str(), strXMLValue.length()))
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

	CSDXMLNode skillXmlProp = rootNode["Skill"];
	if ( skillXmlProp == NULL )
	{
		SYS_CRITICAL(_SDT("[%s: %d]: xml obj err!"), MSG_MARK);
		return FALSE;
	}
	UINT16 wSkillID = skillXmlProp("ID").AsInteger();
	string strBeginRound = skillXmlProp("BeginRound").AsString();
	CSDXMLNode levelXmlProp = skillXmlProp["Level"];
	while(levelXmlProp != NULL)
	{
		SPetSkillEffectProp stProp;
		stProp.wSkillID = wSkillID;
		stProp.strSkillBeginRound = strBeginRound;
		stProp.wSkillLevel = levelXmlProp("Value").AsInteger();
		stProp.strSkillName = levelXmlProp("Name").AsString();
		stProp.strSkillDesc = levelXmlProp("Desc").AsString();
		//解析伤害方式
		CSDXMLNode childXmlProp = levelXmlProp["Hurt"];
		if ( childXmlProp == NULL)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: Pet Skill Hurt failed!"), MSG_MARK);
			return FALSE;
		}
		stProp.dwExtHurtValue = childXmlProp("ExtValue").AsInteger();
		stProp.wHurtRate = childXmlProp("Rate").AsInteger();
		stProp.byAttackRangType = childXmlProp("Range").AsInteger();

		childXmlProp = levelXmlProp["AttackType"];
		if ( childXmlProp == NULL)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: Pet Skill AttackType failed!"), MSG_MARK);
			return FALSE;
		}
		stProp.byOnceEffectID = childXmlProp("ID").AsInteger();
		stProp.wOnceEffectParam = childXmlProp("Param").AsInteger();
		stProp.wOnceEffectRate = childXmlProp("Rate").AsInteger();
		stProp.byMaxTargetNum = childXmlProp("MaxTarget").AsInteger();
		childXmlProp = levelXmlProp["Buffs"];
		if ( childXmlProp != NULL)
		{
			childXmlProp = childXmlProp["Buff"];
			while(childXmlProp != NULL)
			{
				SSkillOnceBuffProp stBuffProp = {0};
				stBuffProp.byBuffKindID = childXmlProp("KindID").AsInteger();
				stBuffProp.byBuffRate = childXmlProp("Rate").AsInteger();
				stBuffProp.byBuffRound = childXmlProp("RoundNum").AsInteger();
				stBuffProp.wBuffParam = childXmlProp("Param").AsInteger();
				if ( 0 != stBuffProp.byBuffKindID)
				{
					stProp.vecBuffProp.push_back(stBuffProp);
				}

				childXmlProp = childXmlProp.Sibling("Buff");
			}
		}
		

		m_mapPetSkillProp[CLogicFunc::Get1616Key(stProp.wSkillID, stProp.wSkillLevel)] = stProp;
		//Next
		levelXmlProp = levelXmlProp.Sibling("Level");
	}

	return TRUE;
}

BOOL CSkillPropMgr::LoadSkillBaseFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("skillbaseprop").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    // 打印查询的表结果
    UINT16 wLastID = 0;
    UINT16 wLastLevel = 0;
    UINT16 wMaxLevel = 0;
    if(pRes->GetRecord())// 当有行结果时
    {
        //m_stSkillBaseProp.wMaxGenNumPerDay = SGDP::SDAtou(pRes->GetFieldValueByName("MaxGenNumPerDay"));
        m_stSkillBaseProp.dwGenGasCoin = SGDP::SDAtou(pRes->GetFieldValueByName("GenGasCoin"));
        m_stSkillBaseProp.dwUpFullGasBallLevelGold = SGDP::SDAtou(pRes->GetFieldValueByName("UpFullGasBallLevelGold"));
        m_stSkillBaseProp.wAddExpPerUpgrade = SGDP::SDAtou(pRes->GetFieldValueByName("AddExpPerUpgrade"));
        m_stSkillBaseProp.wCostGasPerUpgrade = SGDP::SDAtou(pRes->GetFieldValueByName("CostGasPerUpgrade"));
    }
    else
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}


BOOL CSkillPropMgr::LoadSkillEffectRecommendPropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("skilleffectrecommendprop", " order by EffectID").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    // 打印查询的表结果
    UINT16 wLastID = 0;
    UINT16 wLastLevel = 0;
    UINT16 wMaxLevel = 0;
    while(pRes->GetRecord())// 当有行结果时
    {
        SSkillEffectRecommendProp stProp;
        memset(&stProp, 0, sizeof(stProp));
        stProp.wEffectID = SGDP::SDAtou(pRes->GetFieldValueByName("EffectID"));
        stProp.wStartRecommendJingJieLevel = SGDP::SDAtou(pRes->GetFieldValueByName("StartRecommendJingJieLevel"));
        stProp.wRecommendHeroID1 = SGDP::SDAtou(pRes->GetFieldValueByName("RecommendHeroID1"));
        stProp.wRecommendHeroEffectID1 = SGDP::SDAtou(pRes->GetFieldValueByName("RecommendHeroEffectID1"));
        stProp.byRecommendHeroRangKind1 = SGDP::SDAtou(pRes->GetFieldValueByName("RecommendHeroRangKind1"));
        stProp.byRecommendHeroRangBuff1 = SGDP::SDAtou(pRes->GetFieldValueByName("RecommendHeroRangBuff1"));
        stProp.wRecommendHeroID2 = SGDP::SDAtou(pRes->GetFieldValueByName("RecommendHeroID2"));
        stProp.wRecommendHeroEffectID2 = SGDP::SDAtou(pRes->GetFieldValueByName("RecommendHeroEffectID2"));
        stProp.byRecommendHeroRangKind2 = SGDP::SDAtou(pRes->GetFieldValueByName("RecommendHeroRangKind2"));
        stProp.byRecommendHeroRangBuff2 = SGDP::SDAtou(pRes->GetFieldValueByName("RecommendHeroRangBuff2"));

        m_mapSkillEffectRecommendProp[stProp.wEffectID][stProp.wStartRecommendJingJieLevel] = stProp;

    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}

BOOL CSkillPropMgr::LoadSkillActionPropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("skillactionprop", " order by ID, Level asc").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    // 打印查询的表结果
    UINT16 wLastID = 0;
    UINT16 wLastLevel = 0;
    UINT16 wMaxLevel = 0;
    vector<UINT16> vecCurUnlockSkillEffectID; //当前所有解锁的效果ID
    vector<DT_ATTACK_RANG_DATA> vecCurUnlockAttackRang; //当前所有解锁的攻击范围
    while(pRes->GetRecord())// 当有行结果时
    {
        SSkillActionProp stSkillActionProp;
        stSkillActionProp.wID = SGDP::SDAtou(pRes->GetFieldValueByName("ID"));
        if(0 == stSkillActionProp.wID)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: skill action id is 0!"), MSG_MARK);
            return FALSE;
        }
        if(stSkillActionProp.wID != wLastID)
        {
            stSkillActionProp.vecCurUnlockSkillEffectID = vecCurUnlockSkillEffectID;
            stSkillActionProp.vecCurUnlockAttackRang = vecCurUnlockAttackRang;
            if(0 != wMaxLevel)
            {
                m_mapMaxSkillActionLevel[wLastID] = wMaxLevel;
            }

            wLastID = stSkillActionProp.wID;
            wLastLevel = 0;

        }
        stSkillActionProp.wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
        if(stSkillActionProp.wLevel != wLastLevel + 1)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: skill action[%d] Level miss[%d]!"), MSG_MARK, stSkillActionProp.wID, wLastLevel + 1);
            return FALSE;
        }

        stSkillActionProp.dwExp = SGDP::SDAtou(pRes->GetFieldValueByName("Exp"));
        stSkillActionProp.wUnlockSkillEffectID = SGDP::SDAtou(pRes->GetFieldValueByName("UnlockSkillEffectID"));
        if(0 != stSkillActionProp.wUnlockSkillEffectID)
        {
            if(!CSkillPropMgr::Instance()->GetSkillEffectProp(stSkillActionProp.wUnlockSkillEffectID))
            {
                SYS_CRITICAL(_SDT("[%s: %d]: skill action[%d] UnlockSkillEffectID[%d] is invalid!"), MSG_MARK, stSkillActionProp.wID, stSkillActionProp.wUnlockSkillEffectID);
                return FALSE;
            }
        }
        stSkillActionProp.wDspUnlockSkillEffectIDPlayerLevel = SGDP::SDAtou(pRes->GetFieldValueByName("DspUnlockSkillEffectIDPlayerLevel"));

        stSkillActionProp.wEffectHurtRate = SGDP::SDAtou(pRes->GetFieldValueByName("EffectHurtRate"));
        stSkillActionProp.dwExtHurtValue = SGDP::SDAtou(pRes->GetFieldValueByName("ExtHurtValue"));

        stSkillActionProp.byUnlockAttackRangKind = SGDP::SDAtou(pRes->GetFieldValueByName("UnlockAttackRangKind"));
        stSkillActionProp.byUnlockTraceBuffKind = SGDP::SDAtou(pRes->GetFieldValueByName("UnlockTraceBuffKind"));
        if(EARK_BUFF != stSkillActionProp.byUnlockAttackRangKind)
        {
            stSkillActionProp.byUnlockTraceBuffKind = 0;
        }
        else if((EARK_BUFF == stSkillActionProp.byUnlockAttackRangKind) && (0 == stSkillActionProp.byUnlockTraceBuffKind))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: skill action[%d] level[%d] UnlockAttackRangKind is EARK_BUFF[%d], UnlockTraceBuffKind cann't 0!"), MSG_MARK, stSkillActionProp.wID, stSkillActionProp.wLevel, EARK_BUFF);
            return FALSE;
        }
        if(0 != stSkillActionProp.byUnlockAttackRangKind)
        {
            DT_ATTACK_RANG_DATA stDT_ATTACK_RANG_DATA;
            stDT_ATTACK_RANG_DATA.byAttackRangKind = stSkillActionProp.byUnlockAttackRangKind;
            stDT_ATTACK_RANG_DATA.byTraceBuffKind = stSkillActionProp.byUnlockTraceBuffKind;
            stSkillActionProp.vecCurUnlockAttackRang.push_back(stDT_ATTACK_RANG_DATA);
            m_mapActionID2AllRang[stSkillActionProp.wID].push_back(stDT_ATTACK_RANG_DATA);
        }
        stSkillActionProp.wAttackRangHurtRate = SGDP::SDAtou(pRes->GetFieldValueByName("AttackRangHurtRate"));
        if(0 == stSkillActionProp.wAttackRangHurtRate)
        {
            stSkillActionProp.wAttackRangHurtRate = 100;
        }
        stSkillActionProp.wDspUnlockAttackRangPlayerLevel = SGDP::SDAtou(pRes->GetFieldValueByName("DspUnlockAttackRangPlayerLevel"));

        wLastID = stSkillActionProp.wID;
        wLastLevel = stSkillActionProp.wLevel;
        wMaxLevel = wLastLevel;

        vecCurUnlockSkillEffectID = stSkillActionProp.vecCurUnlockSkillEffectID;
        vecCurUnlockAttackRang = stSkillActionProp.vecCurUnlockAttackRang;

        m_mapIDLevel2SkillActionProp[CLogicFunc::Get1616Key(stSkillActionProp.wID, stSkillActionProp.wLevel)] = stSkillActionProp;
        if(0 != stSkillActionProp.byUnlockAttackRangKind)
        {
            m_mapRang2ActionProp[CLogicFunc::Get1688Key(stSkillActionProp.wID, stSkillActionProp.byUnlockAttackRangKind, stSkillActionProp.byUnlockTraceBuffKind)] = stSkillActionProp;
        }
        if(0 != stSkillActionProp.wUnlockSkillEffectID)
        {
            m_mapEffect2ActionProp[CLogicFunc::Get1616Key(stSkillActionProp.wID, stSkillActionProp.wUnlockSkillEffectID)] = stSkillActionProp;
            stSkillActionProp.vecCurUnlockSkillEffectID.push_back(stSkillActionProp.wUnlockSkillEffectID);
            m_mapActionID2AllEffect[stSkillActionProp.wID].push_back(stSkillActionProp.wUnlockSkillEffectID);
        }

    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}


BOOL CSkillPropMgr::LoadSkillEffectPropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("skilleffectprop", " order by ID asc").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    while(pRes->GetRecord())// 当有行结果时
    {
        SSkillEffectProp stSkillEffectProp;
        memset(&stSkillEffectProp, 0, sizeof(SSkillEffectProp));
        stSkillEffectProp.wID = SGDP::SDAtou(pRes->GetFieldValueByName("ID"));
        if(0 == stSkillEffectProp.wID)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: skill effect id is 0!"), MSG_MARK);
            return FALSE;
        }

        stSkillEffectProp.byOnceEffectKindID = SGDP::SDAtou(pRes->GetFieldValueByName("OnceEffectKindID"));
        stSkillEffectProp.wOnceEffectParam = SGDP::SDAtou(pRes->GetFieldValueByName("OnceEffectParam"));

        stSkillEffectProp.astSkillBuffrop[0].byBuffKindID = SGDP::SDAtou(pRes->GetFieldValueByName("BuffKindID1"));
        stSkillEffectProp.astSkillBuffrop[0].byBuffRound = SGDP::SDAtou(pRes->GetFieldValueByName("BuffRound1"));
        stSkillEffectProp.astSkillBuffrop[0].wBuffParam = SGDP::SDAtou(pRes->GetFieldValueByName("BuffParam1"));
        stSkillEffectProp.astSkillBuffrop[0].byBuffRate = SGDP::SDAtou(pRes->GetFieldValueByName("BuffRate1"));

        stSkillEffectProp.astSkillBuffrop[1].byBuffKindID = SGDP::SDAtou(pRes->GetFieldValueByName("BuffKindID2"));
        stSkillEffectProp.astSkillBuffrop[1].byBuffRound = SGDP::SDAtou(pRes->GetFieldValueByName("BuffRound2"));
        stSkillEffectProp.astSkillBuffrop[1].wBuffParam = SGDP::SDAtou(pRes->GetFieldValueByName("BuffParam2"));
        stSkillEffectProp.astSkillBuffrop[1].byBuffRate = SGDP::SDAtou(pRes->GetFieldValueByName("BuffRate2"));

        stSkillEffectProp.astSkillBuffrop[2].byBuffKindID = SGDP::SDAtou(pRes->GetFieldValueByName("BuffKindID3"));
        stSkillEffectProp.astSkillBuffrop[2].byBuffRound = SGDP::SDAtou(pRes->GetFieldValueByName("BuffRound3"));
        stSkillEffectProp.astSkillBuffrop[2].wBuffParam = SGDP::SDAtou(pRes->GetFieldValueByName("BuffParam3"));
        stSkillEffectProp.astSkillBuffrop[2].byBuffRate = SGDP::SDAtou(pRes->GetFieldValueByName("BuffRate3"));

        m_mapIDLevel2SkillEffectProp[stSkillEffectProp.wID] = stSkillEffectProp;

    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}

//SSkillProp* CSkillPropMgr::GetSkillProp(UINT16 wSkillID, UINT16 wSkillLevel)
//{
//	CIDLevel2SkillPropMapItr itr = m_mapIDLevel2SkillProp.find(CLogicFunc::Get1616Key(wSkillID, wSkillLevel));
//	if(itr != m_mapIDLevel2SkillProp.end())
//	{
//		return &itr->second;
//	}
//
//	return NULL;
//}

SSkillActionProp* CSkillPropMgr::GetSkillActionProp(UINT16 wID, UINT16 wLevel)
{
    CID2SkillActionPropMapItr itr = m_mapIDLevel2SkillActionProp.find(CLogicFunc::Get1616Key(wID, wLevel));
    if(itr != m_mapIDLevel2SkillActionProp.end())
    {
        return &itr->second;
    }

    return NULL;
}

SSkillEffectProp* CSkillPropMgr::GetSkillEffectProp(UINT16 wID)
{
    CID2SkillEffectPropMapItr itr = m_mapIDLevel2SkillEffectProp.find(wID);
    if(itr != m_mapIDLevel2SkillEffectProp.end())
    {
        return &itr->second;
    }

    return NULL;
}

UINT16 CSkillPropMgr::GetMaxSkillActionLevel(UINT16 wID)
{
    CMaxSkillActionLevelMapItr itr = m_mapMaxSkillActionLevel.find(wID);
    if(itr != m_mapMaxSkillActionLevel.end())
    {
        return itr->second;
    }

    return 0;
}

SSkillActionProp* CSkillPropMgr::GetSkillActionInfoCli(UINT16 wActionID, UINT16 wLevel, DT_SKILL_ACTION_DATA_CLI& stActionData)
{
    SSkillActionProp* pstSkillActionProp = CSkillPropMgr::Instance()->GetSkillActionProp(wActionID, wLevel);
    if(NULL == pstSkillActionProp)
    {
        return NULL;
    }
    stActionData.wActionID = wActionID;
    stActionData.wActionLevel = wLevel;
    stActionData.wEffectHurtRate =  pstSkillActionProp->wEffectHurtRate;
    stActionData.dwExtHurtValue = pstSkillActionProp->dwExtHurtValue;

    return pstSkillActionProp;
}

SSkillEffectProp* CSkillPropMgr::GetSkillEffectInfoCli(UINT16 wEffectID, DT_SKILL_EFFECT_DATA_CLI& stEffectData)
{
    SSkillEffectProp* pstSkillEffectProp = CSkillPropMgr::Instance()->GetSkillEffectProp(wEffectID);
    if(NULL == pstSkillEffectProp)
    {
        return NULL;
    }
    stEffectData.wEffectID = wEffectID;
    stEffectData.byOnceEffectKindID = pstSkillEffectProp->byOnceEffectKindID;
    stEffectData.wOnceEffectParam = pstSkillEffectProp->wOnceEffectParam;
    stEffectData.byBuffNum = 0;
    memset(&stEffectData.astBuffInfo, 0, sizeof(stEffectData.astBuffInfo));
    for(UINT8 byBuffIdx = 0; byBuffIdx < MAX_SKILL_BUFF_NUM; byBuffIdx++)
    {
        SSkillOnceBuffProp& stSkillBuffProp = pstSkillEffectProp->astSkillBuffrop[byBuffIdx];
        if(0 != stSkillBuffProp.byBuffKindID)
        {
            DT_SKILL_BUFF_DATA& stBuffInfo = stEffectData.astBuffInfo[stEffectData.byBuffNum++];
            stBuffInfo.byBuffKindID = stSkillBuffProp.byBuffKindID;
            stBuffInfo.byBuffRound = stSkillBuffProp.byBuffRound;
            stBuffInfo.wEffectParam = stSkillBuffProp.wBuffParam;
        }
    }

    return pstSkillEffectProp;
}

SSkillActionProp* CSkillPropMgr::GetSkillAttackRangInfoCli(UINT16 wActionID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind, DT_ATTACK_RANG_DATA_CLI& stRangInfo)
{
    memset(&stRangInfo, 0, sizeof(stRangInfo));
    stRangInfo.stRangInfo.byAttackRangKind = byAttackRangKind;
    stRangInfo.stRangInfo.byTraceBuffKind = byTraceBuffKind;
    SSkillActionProp* pstAttackRangActionProp = CSkillPropMgr::Instance()->GetAttackRangActionProp(wActionID, byAttackRangKind, byTraceBuffKind);
    if(pstAttackRangActionProp)
    {
        stRangInfo.wUnlockLevel = pstAttackRangActionProp->wLevel;
        stRangInfo.wAttackRangHurtRate = pstAttackRangActionProp->wAttackRangHurtRate;
		//兼容客户端计算错误
		stRangInfo.wAttackRangHurtRate = 100;
    }

    return pstAttackRangActionProp;
}

SSkillActionProp* CSkillPropMgr::GetAttackRangActionProp(UINT16 wActionID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind)
{
    CRang2ActionPropMapItr itr = m_mapRang2ActionProp.find(CLogicFunc::Get1688Key(wActionID, byAttackRangKind, byTraceBuffKind));
    if(itr != m_mapRang2ActionProp.end())
    {
        return &itr->second;
    }

    return NULL;
}

UINT16 CSkillPropMgr::GetAttackRangHurtRate(UINT16 wActionID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind)
{
    CRang2ActionPropMapItr itr = m_mapRang2ActionProp.find(CLogicFunc::Get1688Key(wActionID, byAttackRangKind, byTraceBuffKind));
    if(itr != m_mapRang2ActionProp.end())
    {
        return itr->second.wAttackRangHurtRate;
    }

    return 100;
}

UINT16 CSkillPropMgr::GetAttackRangUnlockLevel(UINT16 wActionID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind)
{
    CRang2ActionPropMapItr itr = m_mapRang2ActionProp.find(CLogicFunc::Get1688Key(wActionID, byAttackRangKind, byTraceBuffKind));
    if(itr != m_mapRang2ActionProp.end())
    {
        return itr->second.wLevel;
    }

    return 0xFFFF;
}

UINT16 CSkillPropMgr::GetAttackRangDspLevel(UINT16 wActionID, UINT8 byAttackRangKind, UINT8 byTraceBuffKind)
{
    CRang2ActionPropMapItr itr = m_mapRang2ActionProp.find(CLogicFunc::Get1688Key(wActionID, byAttackRangKind, byTraceBuffKind));
    if(itr != m_mapRang2ActionProp.end())
    {
        return itr->second.wDspUnlockAttackRangPlayerLevel;
    }

    return 0xFFFF;
}




UINT16 CSkillPropMgr::GetEffectUnlockLevel(UINT16 wActionID, UINT16 wEffectID)
{
    CEffect2ActionPropMapItr itr = m_mapEffect2ActionProp.find(CLogicFunc::Get1616Key(wActionID, wEffectID));
    if(itr != m_mapEffect2ActionProp.end())
    {
        return itr->second.wLevel;
    }

    return 0xFFFF;
}


UINT16 CSkillPropMgr::GetEffectDspLevel(UINT16 wActionID, UINT16 wEffectID)
{
    CEffect2ActionPropMapItr itr = m_mapEffect2ActionProp.find(CLogicFunc::Get1616Key(wActionID, wEffectID));
    if(itr != m_mapEffect2ActionProp.end())
    {
        return itr->second.wDspUnlockSkillEffectIDPlayerLevel;
    }

    return 0xFFFF;
}

VOID CSkillPropMgr::GenGas(UINT8 byPurpleFlag, DT_GAS_BALL_DATA& stDT_GAS_BALL_DATA)
{
    memset(&stDT_GAS_BALL_DATA, 0, sizeof(stDT_GAS_BALL_DATA));
}

VOID CSkillPropMgr::UpGasBallLevel(UINT8 byFullLevelFlag, UINT8& byLevel)
{

}


UINT32 CSkillPropMgr::GetGas(UINT8 byGasType, UINT8 byLevel)
{
    return 0;
}


UINT32 CSkillPropMgr::GetGuidePurpleGas()
{
    return 0;
}

UINT32 CSkillPropMgr::GetPetSkillTrigerNum(UINT16 wSkillID, UINT16 wPetLevel)
{
	UINT8 byMaxTargetNum = wPetLevel / 100 + 1;
	
	SPetSkillEffectProp* poSkillProp = GetPetSkillProp(wSkillID, 1);
	if ( NULL == poSkillProp )
	{
		SRandomPetSkillProp* poRandomProp = GetPetRandomSkillProp(wSkillID, 1);
		if ( NULL != poRandomProp)
		{
			return poRandomProp->stMainProp.byMaxTargetNum < byMaxTargetNum ? poRandomProp->stMainProp.byMaxTargetNum : byMaxTargetNum;
		}
	}
	else
	{
		return poSkillProp->byMaxTargetNum < byMaxTargetNum ? poSkillProp->byMaxTargetNum : byMaxTargetNum;
	}
	return 1;
}

UINT32 CSkillPropMgr::GetCommonUpGasCoin(UINT16 wUpNum)
{
	return 0;
}

SSkillEffectRecommendProp* CSkillPropMgr::GetSkillEffectRecommendCli(UINT16 wEffectID, UINT16 wCurJingJieLevel, DT_EFFECT_RECOMMEND_CLI astRecommendInfo[2])
{
    memset(astRecommendInfo, 0, sizeof(DT_EFFECT_RECOMMEND_CLI) * 2);
    CEffect2RecommendPropMapMapItr itr = m_mapSkillEffectRecommendProp.find(wEffectID);
    if(itr == m_mapSkillEffectRecommendProp.end())
    {
        return NULL;
    }
    CJingJieLevel2RecommendPropMap& mapSkillEffectRecommendProp = itr->second;
    CJingJieLevel2RecommendPropMapItr itrProp =  mapSkillEffectRecommendProp.lower_bound(wCurJingJieLevel);
    if(itrProp != mapSkillEffectRecommendProp.end())
    {
        if((itrProp->second.wStartRecommendJingJieLevel > wCurJingJieLevel) && (itrProp != mapSkillEffectRecommendProp.begin()))
        {
            itrProp--;
        }
        else if(itrProp->second.wStartRecommendJingJieLevel == wCurJingJieLevel)
        {
            //donothing
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return NULL;
    }
    SSkillEffectRecommendProp* stProp = &itrProp->second;
    DT_EFFECT_RECOMMEND_CLI& stDT_EFFECT_RECOMMEND_CLI1 = astRecommendInfo[0];
    stDT_EFFECT_RECOMMEND_CLI1.wHeroID = stProp->wRecommendHeroID1;
    stDT_EFFECT_RECOMMEND_CLI1.wRecommendEffectKindID = stProp->wRecommendHeroEffectID1;
    stDT_EFFECT_RECOMMEND_CLI1.byRecommendHeroRangKind = stProp->byRecommendHeroRangKind1;
    stDT_EFFECT_RECOMMEND_CLI1.byRecommendHeroRangBuff = stProp->byRecommendHeroRangBuff1;
    DT_EFFECT_RECOMMEND_CLI& stDT_EFFECT_RECOMMEND_CLI2 = astRecommendInfo[1];
    stDT_EFFECT_RECOMMEND_CLI2.wHeroID = stProp->wRecommendHeroID2;
    stDT_EFFECT_RECOMMEND_CLI2.wRecommendEffectKindID = stProp->wRecommendHeroEffectID2;
    stDT_EFFECT_RECOMMEND_CLI2.byRecommendHeroRangKind = stProp->byRecommendHeroRangKind2;
    stDT_EFFECT_RECOMMEND_CLI2.byRecommendHeroRangBuff = stProp->byRecommendHeroRangBuff2;
    return stProp;
}


vector<DT_ATTACK_RANG_DATA>& CSkillPropMgr::GetActionAllRang(UINT16 wActionID)
{
    CActionID2AllRangMapItr itr = m_mapActionID2AllRang.find(wActionID);
    if(itr != m_mapActionID2AllRang.end())
    {
        return itr->second;
    }

    static vector<DT_ATTACK_RANG_DATA> vecEmpty;

    return vecEmpty;
}

vector<UINT16>& CSkillPropMgr::GetActionAllEffect(UINT16 wActionID)
{
    CActionID2AllEffectMapItr itr = m_mapActionID2AllEffect.find(wActionID);
    if(itr != m_mapActionID2AllEffect.end())
    {
        return itr->second;
    }

    static vector<UINT16> vecEmpty;

    return vecEmpty;
}

SPetSkillEffectProp* CSkillPropMgr::GetPetSkillProp(UINT16 wSkillID, UINT16 wLevel)
{
	CMapPetSkillEffectPropItr itr = m_mapPetSkillProp.find(CLogicFunc::Get1616Key(wSkillID, wLevel));
	if ( itr == m_mapPetSkillProp.end())
	{
		return NULL;
	}
	return &itr->second;
}

SRandomPetSkillProp* CSkillPropMgr::GetPetRandomSkillProp(UINT16 wSkillID, UINT16 wLevel)
{
	CMapVecRandomSkillPropItr itr = m_mapRandomPetSkillProp.find(CLogicFunc::Get1616Key(wSkillID, wLevel));
	if ( itr == m_mapRandomPetSkillProp.end())
	{
		return NULL;
	}
	return &itr->second;
}