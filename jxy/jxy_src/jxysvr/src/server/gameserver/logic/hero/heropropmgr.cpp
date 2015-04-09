#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdutil.h>
#include <sdstring.h>
#include <logic/skill/skillmgr.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/skill/skillpropmgr.h>
#include <framework/gsapi.h>
#include "heropropmgr.h"
#include "logic/other/singleprammgr.h"
#include "logic/base/stringtool.h"
#include <protocligs.h>

#include <db/dbmgr.h>

using namespace SGDP;

#define SQL_READ_HERO_STRING "select * from heroprop"
#define SQL_READ_HEROUPGRADE_STRING "select * from heroupgradeprop"


IMPLEMENT_SINGLETON_PROPMGR(CHeroPropMgr);

CHeroPropMgr::CHeroPropMgr()
{
}

CHeroPropMgr::~CHeroPropMgr()
{
}

BOOL CHeroPropMgr::Init()
{
    return LoadFromDB();
}

VOID CHeroPropMgr::UnInit()
{
    m_mapHeroProp.clear();
    m_mapHeroUpgradeProp.clear();
}

BOOL CHeroPropMgr::LoadFromDB()
{
    if(!LoadHeroPropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadHeroPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadHeroUpgradePropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadHeroUpgradePropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}


// 从数据源获取数据
BOOL CHeroPropMgr::LoadHeroUpgradePropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("heroupgradeprop").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    UINT16 wMaxPlayerLevel = 0;

    // 打印查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        SHeroUpgradeProp stHeroUpgradeProp;
        memset(&stHeroUpgradeProp, 0, sizeof(SHeroUpgradeProp));
        stHeroUpgradeProp.wLevel = SGDP::SDAtou(pRes->GetFieldValueByName("Level"));
        stHeroUpgradeProp.qwExperience = SGDP::SDAtou(pRes->GetFieldValueByName("Experience"));
        m_mapHeroUpgradeProp[stHeroUpgradeProp.wLevel] = stHeroUpgradeProp;

        if (wMaxPlayerLevel < stHeroUpgradeProp.wLevel)
        {
            wMaxPlayerLevel = stHeroUpgradeProp.wLevel;
        }
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    //检测当前等级是否相等，不相等，则配置有问题
    if (CSinglePramMgr::Instance()->GetMaxPlayerLevel() != wMaxPlayerLevel)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: hero upgrade Level error!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

// 从数据源获取数据
BOOL CHeroPropMgr::LoadHeroPropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("heroprop").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    // 打印查询的表结果
    while(pRes->GetRecord())// 当有行结果时
    {
        SHeroProp stHeroProp;
        stHeroProp.wKindID = SGDP::SDAtou(pRes->GetFieldValueByName("KindID"));
        _SDTStrncpy(stHeroProp.szName, _SDTUTF82T(pRes->GetFieldValueByName("Name")), NAME_LEN_MAX);
        stHeroProp.byCareerID = SGDP::SDAtou(pRes->GetFieldValueByName("CareerID"));
        if((stHeroProp.byCareerID == 0) || (stHeroProp.byCareerID > 4))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CareerID[%d] is invalid!"), MSG_MARK, stHeroProp.byCareerID);
            return FALSE;
        }
        stHeroProp.wTalent = SGDP::SDAtou(pRes->GetFieldValueByName("Talent"));
        stHeroProp.wHitRate = SGDP::SDAtou(pRes->GetFieldValueByName("HitRate"));
        stHeroProp.wDodgeRate = SGDP::SDAtou(pRes->GetFieldValueByName("DodgeRate"));
        stHeroProp.wCritRate = SGDP::SDAtou(pRes->GetFieldValueByName("CritRate"));
        stHeroProp.wDeCritRate = SGDP::SDAtou(pRes->GetFieldValueByName("DeCritRate"));
        stHeroProp.wAngryValue = SGDP::SDAtou(pRes->GetFieldValueByName("AngryValue"));
        stHeroProp.dwVisitCoin = SGDP::SDAtou(pRes->GetFieldValueByName("VisitCoin"));
        stHeroProp.byVisitNum = SGDP::SDAtou(pRes->GetFieldValueByName("VisitNum"));
        stHeroProp.wVisitGold = SGDP::SDAtou(pRes->GetFieldValueByName("VisitGold"));
        stHeroProp.dwAttack = SGDP::SDAtou(pRes->GetFieldValueByName("Attack"));
        stHeroProp.dwHP = SGDP::SDAtou(pRes->GetFieldValueByName("HP"));

        stHeroProp.wProbability = SGDP::SDAtou(pRes->GetFieldValueByName("Probability"));
        stHeroProp.wGoldLeopardProbability = SGDP::SDAtou(pRes->GetFieldValueByName("GoldLeopardProbability"));
        stHeroProp.wCoinLeopardProbability = SGDP::SDAtou(pRes->GetFieldValueByName("CoinLeopardProbability"));
        stHeroProp.wSkillActionID = SGDP::SDAtou(pRes->GetFieldValueByName("SkillActionID"));
        stHeroProp.wSkillActionLevel = SGDP::SDAtou(pRes->GetFieldValueByName("SkillActionLevel"));
        if(!CSkillPropMgr::Instance()->GetSkillActionProp(stHeroProp.wSkillActionID, stHeroProp.wSkillActionLevel))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: Hero[%d] skill action[%d.%d] not exist!"), MSG_MARK, stHeroProp.wKindID, stHeroProp.wSkillActionID, stHeroProp.wSkillActionLevel);
            return FALSE;
        }
        stHeroProp.wSkillEffectID = SGDP::SDAtou(pRes->GetFieldValueByName("SkillEffectID"));
        if((0 != stHeroProp.wSkillEffectID) && (!CSkillPropMgr::Instance()->GetSkillEffectProp(stHeroProp.wSkillEffectID)))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: Hero[%d] skill effect[%d] not exist!"), MSG_MARK, stHeroProp.wKindID, stHeroProp.wSkillEffectID);
            return FALSE;
        }
        stHeroProp.wRecuitLevel = SGDP::SDAtou(pRes->GetFieldValueByName("RecuitLevel"));
        stHeroProp.wRequireJingjieLevel = SGDP::SDAtou(pRes->GetFieldValueByName("RequireJingjieLevel"));

        stHeroProp.wMaxAbsorbNum = SGDP::SDAtou(pRes->GetFieldValueByName("AbsorbNum"));
        stHeroProp.byRecommend = SGDP::SDAtou(pRes->GetFieldValueByName("Recommend"));

        stHeroProp.bIsLegendHero    = (SGDP::SDAtou(pRes->GetFieldValueByName("IsLegendHero")) > 0 ? TRUE : FALSE);
        stHeroProp.wUpgradeFromHeroID = SGDP::SDAtou(pRes->GetFieldValueByName("UpgradeFromHeroID"));
        stHeroProp.wUpgradeToHeroID = 0;

        std::string strCostMaterials    = pRes->GetFieldValueByName("CostMaterialList");
        std::string strCostMaterialsNum = pRes->GetFieldValueByName("CostMaterialsNum");
        std::string strFlyMaterials    = pRes->GetFieldValueByName("FlyMaterialList");
        std::string strFlyMaterialsNum = pRes->GetFieldValueByName("FlyMaterialsNum");
        
        std::vector<UINT32> vecCostMaterial;
        std::vector<UINT32> vecCostMaterialNum;
        std::vector<UINT32> vecFlyMaterial;
        std::vector<UINT32> vecFlyMaterialNum;

        StringTool::SDSplitStringToUInt(vecCostMaterial,    strCostMaterials, ',');
        StringTool::SDSplitStringToUInt(vecCostMaterialNum, strCostMaterialsNum, ',');
        StringTool::SDSplitStringToUInt(vecFlyMaterial,     strFlyMaterials, ',');
        StringTool::SDSplitStringToUInt(vecFlyMaterialNum,  strFlyMaterialsNum, ',');

        if (vecCostMaterial.size() != vecCostMaterialNum.size()){
            SYS_CRITICAL(_SDT("[%s: %d]: CHeroPropMgr::LoadHeroPropFromDB Hero[%d] vecCostMaterial.size()[%u] != vecCostMaterialNum.size()[%u]!"), MSG_MARK, stHeroProp.wKindID, vecCostMaterial.size(), vecCostMaterialNum.size());
            return FALSE;
        }

        if (vecFlyMaterial.size() != vecFlyMaterialNum.size()){
            SYS_CRITICAL(_SDT("[%s: %d]: CHeroPropMgr::LoadHeroPropFromDB Hero[%d] vecFlyMaterial.size()[%u] != vecFlyMaterialNum.size()[%u]!"), MSG_MARK, stHeroProp.wKindID, vecFlyMaterial.size(), vecFlyMaterialNum.size());
            return FALSE;
        }
        
        for(UINT16 i = 0; i < vecCostMaterial.size(); i++){
            SMaterialProp stMaterialProp;
            stMaterialProp.byMaterialType = EIK_GOODS;
            stMaterialProp.wMaterialID = vecCostMaterial[i];
            stMaterialProp.wMaterialCnt = vecCostMaterialNum[i];
            stHeroProp.vecCostMaterial.push_back(stMaterialProp);
        }

        for(UINT16 i = 0; i < vecFlyMaterial.size(); i++){
            SMaterialProp stMaterialProp;
            stMaterialProp.byMaterialType = EIK_GOODS;
            stMaterialProp.wMaterialID = vecFlyMaterial[i];
            stMaterialProp.wMaterialCnt = vecFlyMaterialNum[i];
            stHeroProp.vecFlyMaterial.push_back(stMaterialProp);
        }

        if (stHeroProp.bIsLegendHero && !stHeroProp.wUpgradeFromHeroID){
            if(stHeroProp.vecCostMaterial.size() != 5){
                SYS_CRITICAL(_SDT("[%s: %d]: CHeroPropMgr::LoadHeroPropFromDB recruit material of Hero[%u] size != 5, aborted!"), MSG_MARK, stHeroProp.wKindID);
                return FALSE;
            }
        }

        // 检测是否有配置飞升材料（资质等于0的是剧情人物，不用配置飞升材料）
        if (stHeroProp.wTalent > 0){
            if(stHeroProp.vecFlyMaterial.size() != 4){
                SYS_CRITICAL(_SDT("[%s: %d]: CHeroPropMgr::LoadHeroPropFromDB recruit material of Hero[%u] size != 5, aborted!"), MSG_MARK, stHeroProp.wKindID);
                return FALSE;
            }
        }

        m_mapHeroProp[stHeroProp.wKindID] = stHeroProp;
    }

    if(pRes){
        pRes->Release();
    }

    // 建立起 伙伴->进阶伙伴 的 关联
    for(CHeroPropPriMap::iterator itr = m_mapHeroProp.begin(); itr != m_mapHeroProp.end(); ++itr){
        SHeroProp &stHeroProp = itr->second;

        if (stHeroProp.wUpgradeFromHeroID > 0){
            SHeroProp *pstUpgradeFromHeroProp = GetHeroProp(stHeroProp.wUpgradeFromHeroID);
            if (NULL == pstUpgradeFromHeroProp){
                SYS_CRITICAL(_SDT("[%s: %d]: CHeroPropMgr::LoadHeroPropFromDB Hero[%d] upgrade from of hero[%u] err: could not found born hero!"), MSG_MARK, stHeroProp.wKindID, stHeroProp.wUpgradeFromHeroID);
                return FALSE;
            }

            pstUpgradeFromHeroProp->wUpgradeToHeroID = stHeroProp.wKindID;

            if(stHeroProp.vecCostMaterial.size() != 5){
                SYS_CRITICAL(_SDT("[%s: %d]: CHeroPropMgr::LoadHeroPropFromDB upgrade material of Hero[%u] size != 5, aborted!"), MSG_MARK, stHeroProp.wKindID);
                return FALSE;
            }
        }       
    }

    return TRUE;
}

SHeroProp* CHeroPropMgr::GetHeroProp(UINT16 wHeroKindID)
{
    CHeroPropPriMapItr itr = m_mapHeroProp.find(wHeroKindID);
    if(itr != m_mapHeroProp.end())
    {
        return &itr->second;;
    }

    return NULL;
}

SHeroUpgradeProp* CHeroPropMgr::GetHeroUpgradeProp(UINT16 wLevel)
{
    CHeroUpgradePropPriMapItr itr = m_mapHeroUpgradeProp.find(wLevel);
    if(itr != m_mapHeroUpgradeProp.end())
    {
        return &itr->second;;
    }

    return NULL;
}

UINT16 CHeroPropMgr::GetLevel(UINT64 qwExperience)
{
    CHeroUpgradePropPriMapItr itr;
    UINT16 wMaxPlayerLevel = CSinglePramMgr::Instance()->GetMaxPlayerLevel();
    for (UINT16 wLevel = 1; wLevel <= wMaxPlayerLevel; wLevel++)
    {
        itr = m_mapHeroUpgradeProp.find(wLevel);
        if (itr->second.qwExperience > qwExperience)
        {
            return itr->second.wLevel - 1;
        }
    }
    return wMaxPlayerLevel;
}


VOID CHeroPropMgr::GetDT_HERO_BASE_DATA_CLI(DT_HERO_BASE_DATA_CLI& stDT_HERO_BASE_DATA_CLI, DT_SKILL_DATA_CLI &stSkillInfo, UINT16 wHeroKindID)
{
    SHeroProp * pstHeroProp = NULL;
    memset(&stDT_HERO_BASE_DATA_CLI, 0x00, sizeof(DT_HERO_BASE_DATA_CLI));
    CHeroPropPriMapItr itr = m_mapHeroProp.find(wHeroKindID);
    if(itr == m_mapHeroProp.end())
    {
        return;
    }
    pstHeroProp = &itr->second;
    stDT_HERO_BASE_DATA_CLI.wKindID = itr->second.wKindID;
    stDT_HERO_BASE_DATA_CLI.byCareerID = itr->second.byCareerID;
    stDT_HERO_BASE_DATA_CLI.wTalent = itr->second.wTalent;
    stDT_HERO_BASE_DATA_CLI.stBattleAttribute.dwHP =  itr->second.dwHP;
    stDT_HERO_BASE_DATA_CLI.stBattleAttribute.dwAttack =  itr->second.dwAttack;

    stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute.wHitRate =  itr->second.wHitRate;
    stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute.wDodgeRate =  itr->second.wDodgeRate;
    stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute.wCritRate =  itr->second.wCritRate;
    stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute.wDeCritRate =  itr->second.wDeCritRate;
    stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute.wAngryValue =  itr->second.wAngryValue;

    SSkillActionProp * pstSkillActionProp = CSkillPropMgr::Instance()->GetSkillActionInfoCli(pstHeroProp->wSkillActionID, pstHeroProp->wSkillActionLevel, stSkillInfo.stActionInfo);
    if(NULL == pstSkillActionProp)
    {
        return;
    }

    DT_SKILL_ACTION_DATA_CLI& stActionInfo = stSkillInfo.stActionInfo;
    SSkillEffectProp* pstSkillEffectProp = CSkillPropMgr::Instance()->GetSkillEffectInfoCli(pstHeroProp->wSkillEffectID, stSkillInfo.stEffectInfo);
    if(pstSkillEffectProp)
    {
        stSkillInfo.stEffectInfo.wUnlockLevel = CSkillPropMgr::Instance()->GetEffectUnlockLevel(pstHeroProp->wSkillActionID, pstHeroProp->wSkillEffectID);
    }

    UINT8  byUnlockAttackRangKind = pstSkillActionProp->byUnlockAttackRangKind;
    if(EARK_INVALID == byUnlockAttackRangKind)
    {
        byUnlockAttackRangKind = EARK_FRONTONE;
    }
    //CSkillPropMgr::Instance()->GetSkillAttackRangInfoCli(pstHeroProp->wSkillActionID, byUnlockAttackRangKind, pstSkillActionProp->byUnlockTraceBuffKind, stSkillInfo.stRangInfo);
    SSkillActionProp *poSkillActionProp = NULL;
    //兼容客户端计算错误
    poSkillActionProp = CSkillPropMgr::Instance()->GetSkillAttackRangInfoCli(pstHeroProp->wSkillActionID, byUnlockAttackRangKind, pstSkillActionProp->byUnlockTraceBuffKind, stSkillInfo.stRangInfo);
    if (NULL != poSkillActionProp)
    {
        stSkillInfo.stActionInfo.wEffectHurtRate = stSkillInfo.stActionInfo.wEffectHurtRate * poSkillActionProp->wAttackRangHurtRate / 100;
        stSkillInfo.stActionInfo.dwExtHurtValue = stSkillInfo.stActionInfo.dwExtHurtValue * poSkillActionProp->wAttackRangHurtRate / 100;
    }
}

