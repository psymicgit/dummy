#include "logic/item/jewelry/jewelrypropmgr.h"
#include "hero.h"
#include <sdutil.h>
#include <errdef.h>
#include <logic/player/player.h>
#include <sdloggerimpl.h>
#include <sdutil.h>

#include <logic/updateattr/updateattrpropmgr.h>
#include <logic/other/singleprammgr.h>
#include <logic/awaken/awakenpropmgr.h>
#include <logic/awaken/awakenpropmgr2.h>
#include <logic/other/xiangqianmgr.h>
#include <logic/record/recordmgr.h>
#include <logic/base/basedefine.h>
#include <logic/base/logicfunc.h>
#include <logic/science/sciencetree.h>
#include <logic/jingjie/jingjie.h>
#include <logic/hero/heromgr.h>
#include <logic/bag/bag.h>
#include <logic/task/taskmgr.h>
#include <logic/skill/skillmgr.h>
#include <net/cli/user.h>
#include <logic/hero/heropropmgr.h>
#include <logic/science/scienceupdatepropmgr.h>
#include <logic/jingjie/jingjiepropmgr.h>
#include <logic/talent/drugpropmgr.h>
#include <logic/skill/skillpropmgr.h>
#include <logic/gymnasium/gymnasiumpropmgr.h>
#include "logic/player/player.h"
#include "logic/awaken/awaken.h"
#include "logic/formation/formation.h"
#include "logic/formation/formationpropmgr.h"
#include "logic/battle/battlemgr.h"
#include "logic/lua/luamgr.h"
#include "logic/specialeffects/specialeffects.h"
#include "logic/specialeffects/specialeffectsmgr.h"
#include "logic/soul/soul.h"
#include "logic/soul/playersoul.h"
#include "logic/medal/playermedal.h"
#include "logic/specialeffects/specialeffectsmgr.h"
#include "logic/item/equip/openlightpropmgr.h"
#include "logic/fly/flypropmgr.h"
#include "logic/item/equip//equipmakemgr.h"
#include "logic/soul/soulpropmgr.h"
using namespace SGDP;



CHero::CHero()
{
    memset(&m_stDT_HERO_BASE_DATA, 0, sizeof(DT_HERO_BASE_DATA));
    memset(&m_stDT_HERO_UPGRADE_DATA, 0, sizeof(m_stDT_HERO_UPGRADE_DATA));
    memset(&m_stAbsorbDataLst, 0, sizeof(m_stAbsorbDataLst));
	memset(&m_HeroSoulData, 0, sizeof(m_HeroSoulData));

    m_dwDrugTalent = 0;
    m_dwSkillTalent = 0;
    m_poGodWeapon = NULL;
    memset(m_astStudy, 0, sizeof(m_astStudy));
    m_qwUpgradeExperience = 0;
    
    m_wJewelrySuitDressLvl = 0;
    m_wJewelrySuitLvl = 0;

    //所有的map都得clear，避免对象池重复使用问题
    m_mapEquip.Clear();
    m_mapEquipPos.Clear();
    m_mapLevel2Drug.Clear();
	m_bIsOpenFormation = FALSE;
    m_poPlayerGroup = NULL;
	m_mapJewelry.Clear();
	m_mapJewelryPos.Clear();
}


CHero::~CHero()
{

    //释放装备
    CEquip* poEquip = m_mapEquip.GetFistData();
    while(poEquip)
    {
        CItemMgr::Instance()->RemoveItem(poEquip);
        poEquip = m_mapEquip.GetNextData();
    }
    m_mapEquip.Clear();
    m_mapEquipPos.Clear();

	//释放饰品
	CJewelry* poJewelry = m_mapJewelry.GetFistData();
	while ( poJewelry)
	{
		CItemMgr::Instance()->RemoveItem(poJewelry);
		poJewelry = m_mapJewelry.GetNextData();
	}
	m_mapJewelry.Clear();
	m_mapJewelryPos.Clear();

    //释放丹药
    if(m_poOwner)
    {
        UINT32 dwPlayerID = m_poOwner->GetDT_PLAYER_BASE_DATA().dwID;
        UINT16 wHeroKindID = m_stDT_HERO_BASE_DATA.wKindID;
        CDrug* poDrug = m_mapLevel2Drug.GetFistData();
        while(poDrug)
        {
            CDrugMgr::Instance()->RemoveDrug(dwPlayerID, wHeroKindID, poDrug->GetDT_DRUG_DATA().byLevel);
            poDrug = m_mapLevel2Drug.GetNextData();
        }
    }

    //释放神器
    if (m_poGodWeapon)
    {
        CGodweaponMgr::Instance()->RemoveGodWeapon(m_poOwner->GetDT_PLAYER_BASE_DATA().dwID, m_stDT_HERO_BASE_DATA.wKindID);
        m_poGodWeapon = NULL;
    }
	m_bIsOpenFormation = FALSE;
	memset(&m_HeroSoulData, 0, sizeof(m_HeroSoulData));
}

UINT8  CHero::GetFormationIdx()
{
    //对于武将有两个属主，一个是玩家自己本，还有一个就是组队副本，但是对于组队副本只有在战斗的时候才存在。
    if (NULL != m_poPlayerGroup)
    {
        for (int i = 1; i < MAX_FORMATION_IDX_NUM + 1; i++)
        {
            if(m_poPlayerGroup->m_apoFormationObj[i] == this)
            {
                return i;
            }
        }
    }
    else if(NULL != m_poOwner)
    {
        for (int i = 1; i < MAX_FORMATION_IDX_NUM + 1; i++)
        {
            if(m_poOwner->m_apoFormationObj[i] == this)
            {
                return i;
            }
        }
    }
    return 0;
}


void CHero::SetPlayerGroup(CPlayerGroup *poPlayerGroup)
{
    m_poPlayerGroup = poPlayerGroup;
}

//初始化相关对象
BOOL CHero::Init(DT_HERO_DATA* pstDT_HERO_DATA, DT_JEWELRY_DATA_LST& stJewelry , const DT_HERO_SOUL_DATA& rSoulData, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }
    m_poOwner = poOwner;
	m_bIsOpenFormation = FALSE;
    if(NULL == CHeroPropMgr::Instance()->GetHeroProp(pstDT_HERO_DATA->stHeroBaseData.wKindID))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstHeroProp[KindID:%d] is NULL!"), MSG_MARK, pstDT_HERO_DATA->stHeroBaseData.wKindID);
        return FALSE;
    }
    memcpy(&m_stDT_HERO_BASE_DATA, &(pstDT_HERO_DATA->stHeroBaseData), sizeof(DT_HERO_BASE_DATA));

    if(NULL == CHeroPropMgr::Instance()->GetHeroUpgradeProp(pstDT_HERO_DATA->stHeroBaseData.wLevel))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstHeroUpgradeProp[level:%d] is NULL!"), MSG_MARK, pstDT_HERO_DATA->stHeroBaseData.wLevel);
        return FALSE;
    }

    //初始化装备
    if(FALSE == InitEquip(&(pstDT_HERO_DATA->stEquipDataInfo)))
    {
        USR_INFO(_SDT("[%s: %d]:  Hero[%d] InitEquip failed!!"), MSG_MARK, pstDT_HERO_DATA->stHeroBaseData.wKindID);
        return FALSE;
    }

    //初始化技能
    if(FALSE == InitSkill(pstDT_HERO_DATA->stSkillInfo))
    {
        USR_INFO(_SDT("[%s: %d]:  Hero[%d] InitSkill failed!!"), MSG_MARK, pstDT_HERO_DATA->stHeroBaseData.wKindID);
        return FALSE;
    }

    //初始化心得
    if(FALSE == InitStudy(pstDT_HERO_DATA->stStudyInfo.astStudyInfo))
    {
        USR_INFO(_SDT("[%s: %d]:  Hero[%d] InitStudy failed!!"), MSG_MARK, pstDT_HERO_DATA->stHeroBaseData.wKindID);
        return FALSE;
    }

    //初始化经验继承
    if(FALSE == InitAbsorb(&pstDT_HERO_DATA->stAbsorbInfo))
    {
        USR_INFO(_SDT("[%s: %d]:  Hero[%d] InitAbsorb failed!!"), MSG_MARK, pstDT_HERO_DATA->stHeroBaseData.wKindID);
        return FALSE;
    }

	if ( FALSE == InitJewelry(stJewelry))
	{
		USR_INFO(_SDT("[%s: %d]:  Hero[%d] InitJewelry failed!!"), MSG_MARK, pstDT_HERO_DATA->stHeroBaseData.wKindID);
		return FALSE;
	}

	//初始化器魂
	if(FALSE == InitGodweapon(&(pstDT_HERO_DATA->stGodweaponInfo)))
	{
		USR_INFO(_SDT("[%s: %d]:  Hero[%d] InitGodweapon failed!!"), MSG_MARK, pstDT_HERO_DATA->stHeroBaseData.wKindID);
		return FALSE;
	}
	/////////////////////////////过滤错误数据////////////////////////////
	DT_HERO_SOUL_DATA stHeroSoulData = {0};
	UINT16 wSoulNum = 0;
	for ( UINT16 wIdx = 0; wIdx < rSoulData.wSoulNum; wIdx++)
	{
		const SoulBaseCfg* poBaseCfg = CSoulPropMgr::Instance()->GetSoulBaseCfg(rSoulData.astSoulLstData[wIdx].wSoulID);
		if ( NULL != poBaseCfg)
		{
			stHeroSoulData.astSoulLstData[wSoulNum] = rSoulData.astSoulLstData[wIdx];
			stHeroSoulData.astSoulLstData[wSoulNum].bySoulStar = poBaseCfg->ucStar;
			stHeroSoulData.astSoulLstData[wSoulNum].bySoulType = poBaseCfg->ucSoulType;
			stHeroSoulData.astSoulPotencyLstData[wSoulNum] = rSoulData.astSoulPotencyLstData[wIdx];
			if ( 20 <= stHeroSoulData.astSoulLstData[wSoulNum].bySoulLevel)
			{
				SYS_CRITICAL("[%s:%d] Over 20: SoulID = %u, SoulLevel = %d, PlayerID = %u", MSG_MARK, rSoulData.astSoulLstData[wIdx].wSoulID, rSoulData.astSoulLstData[wIdx].bySoulLevel, m_poOwner->GetID());
			}
			wSoulNum++;
			
		}
		else
		{
			SYS_CRITICAL("[%s:%d] ID NOT EXIST: SoulID = %u, SoulLevel = %d, PlayerID = %u", MSG_MARK, rSoulData.astSoulLstData[wIdx].wSoulID, rSoulData.astSoulLstData[wIdx].bySoulLevel, m_poOwner->GetID());
		}
	}
	stHeroSoulData.wSoulNum = wSoulNum;
	/////////////////////////////过滤错误数据////////////////////////////
	m_HeroSoulData = stHeroSoulData;
	InitSoulData();

    if(1 == m_stDT_HERO_BASE_DATA.byCoach)
    {
        m_poOwner->SetCoachHero(this);
    }

    //赋值下级别升级经验
    SetUpgradeExperience();

    SaveClientUpgradeExp();

    ReCheckJewelrySuit();
    
    //赋值战斗属性
    InitBattleAttribute();

    return TRUE;

}

void CHero::SaveClientUpgradeExp()
{
    //赋值升级信息，给客户端用
    m_stDT_HERO_UPGRADE_DATA.wHeroKindID = m_stDT_HERO_BASE_DATA.wKindID;
    m_stDT_HERO_UPGRADE_DATA.wLevel = m_stDT_HERO_BASE_DATA.wLevel;
    m_stDT_HERO_UPGRADE_DATA.dwExperience = (UINT32)m_stDT_HERO_BASE_DATA.qwExperience;
    m_stDT_HERO_UPGRADE_DATA.dwUpgradeExperience = (UINT32)m_qwUpgradeExperience;
}

//添加经验,若可升级则升级
BOOL CHero::AddExperience(UINT32 dwValue)
{
    if (GetLevel() >= FLY_LEVEL && !IsFly()){
        if (IsCoachHero()){
            AddFlyExp(dwValue);
        }

        return TRUE;
    }

    BOOL bIsFly = IsFly();

    //满级后不给经验
    if(m_stDT_HERO_BASE_DATA.wLevel >= CSinglePramMgr::Instance()->GetMaxPlayerLevel())
    {
        return TRUE;
    }

	m_stDT_HERO_BASE_DATA.qwExperience += dwValue;
	m_stDT_HERO_UPGRADE_DATA.dwEncExperience += dwValue;

    UINT16 cLoopNum = CSinglePramMgr::Instance()->GetMaxPlayerLevel();//每次最多可直升级数，防止错误数据while死循环

    //升级算法优化（可直接找到对应经验的等级），todo...
    while((m_stDT_HERO_BASE_DATA.qwExperience >= m_qwUpgradeExperience) && (--cLoopNum > 0))
    {
        //升级
        Upgrade();

        //赋值升级标志
        m_stDT_HERO_UPGRADE_DATA.byUpgradeFlag = 1;
    }

    if (!bIsFly && GetLevel() >= FLY_LEVEL){
        SetLevel(FLY_LEVEL);
    }

    //赋值升级信息，给客户端用
    SaveClientUpgradeExp();

    //主角
    if (1 == m_stDT_HERO_BASE_DATA.byCoach )
    {
        //记录消费日志
        m_poOwner->CkToday();
        m_poOwner->GetDT_PLAYER_TODAY_CONSUME_DATA().qwAddExperience += dwValue;
        m_poOwner->GetDT_PLAYER_TODAY_CONSUME_DATA().qwAfterExperience = m_stDT_HERO_BASE_DATA.qwExperience;

        //增加缓存记录
        CPlayerBaseDataCacheMgr::Instance()->SetExperience(m_poOwner->GetID(), (UINT32)m_stDT_HERO_BASE_DATA.qwExperience);
    }

    return TRUE;
}

//添加经验,若可升级则升级
BOOL CHero::AddExperience(UINT32 dwValue, UINT16 wLevel)
{
    if (GetLevel() >= FLY_LEVEL && !IsFly()){
        if (IsCoachHero()){
            AddFlyExp(dwValue);
        }

        return TRUE;
    }

    BOOL bIsFly = IsFly();

    BOOL		isRet = TRUE;
    UINT16	cLoopNum = CSinglePramMgr::Instance()->GetMaxPlayerLevel();//每次最多可直升级数，防止错误数据while死循环
    UINT64	qwExperience = m_stDT_HERO_BASE_DATA.qwExperience;
    m_stDT_HERO_BASE_DATA.qwExperience += dwValue;

    //升级算法优化（可直接找到对应经验的等级），todo...
    while((m_stDT_HERO_BASE_DATA.qwExperience >= m_qwUpgradeExperience) && (--cLoopNum > 0))
    {
        //升级
        if (GetDT_HERO_BASE_DATA().wLevel < wLevel)
        {
            Upgrade();
        }
        else
        {
            m_stDT_HERO_BASE_DATA.qwExperience = m_qwUpgradeExperience - 1;
            if (qwExperience == m_stDT_HERO_BASE_DATA.qwExperience)
            {
                isRet = FALSE;
            }
            //InitBattleAttribute(); //重新初始化属性
            break;
        }
        //赋值升级标志
        m_stDT_HERO_UPGRADE_DATA.byUpgradeFlag = 1;
    }

    if (!bIsFly && GetLevel() >= FLY_LEVEL){
        SetLevel(FLY_LEVEL);
    }

    //赋值升级信息，给客户端用
    SaveClientUpgradeExp();
    //InitExperience();

    //主角
    if (1 == m_stDT_HERO_BASE_DATA.byCoach )
    {
        //记录消费日志
        m_poOwner->CkToday();
        m_poOwner->GetDT_PLAYER_TODAY_CONSUME_DATA().qwAddExperience += dwValue;
        m_poOwner->GetDT_PLAYER_TODAY_CONSUME_DATA().qwAfterExperience = m_stDT_HERO_BASE_DATA.qwExperience;

        //增加缓存记录
        CPlayerBaseDataCacheMgr::Instance()->SetExperience(m_poOwner->GetID(), (UINT32)m_stDT_HERO_BASE_DATA.qwExperience);
    }
    return isRet;
}

//获取升级信息并清除该记录
VOID CHero::GetUpgrageInfo(DT_HERO_UPGRADE_DATA& stDT_HERO_UPGRADE_DATA)
{
    stDT_HERO_UPGRADE_DATA.wHeroKindID = m_stDT_HERO_BASE_DATA.wKindID;
    stDT_HERO_UPGRADE_DATA.byUpgradeFlag = m_stDT_HERO_UPGRADE_DATA.byUpgradeFlag;
    stDT_HERO_UPGRADE_DATA.wLevel = m_stDT_HERO_UPGRADE_DATA.wLevel;
    stDT_HERO_UPGRADE_DATA.dwEncExperience = m_stDT_HERO_UPGRADE_DATA.dwEncExperience;
    //stDT_HERO_UPGRADE_DATA.dwExperience = m_stDT_HERO_UPGRADE_DATA.dwExperience;
    //stDT_HERO_UPGRADE_DATA.dwUpgradeExperience = m_stDT_HERO_UPGRADE_DATA.dwUpgradeExperience;
    //经验下发差值
    GetExperience(stDT_HERO_UPGRADE_DATA.dwExperience, stDT_HERO_UPGRADE_DATA.dwUpgradeExperience);

    if((1 == m_stDT_HERO_BASE_DATA.byCoach) && (1 == m_stDT_HERO_UPGRADE_DATA.byUpgradeFlag))
    {
        stDT_HERO_UPGRADE_DATA.wCanUseBoxGoodsID = m_poOwner->GetBoxGoodsPrompt();
    }

	m_stDT_HERO_UPGRADE_DATA.byUpgradeFlag = 0;
	m_stDT_HERO_UPGRADE_DATA.dwEncExperience = 0;
}

//获取升级信息并清除该记录
VOID CHero::GetUpgrageInfo2(DT_HERO_UPGRADE_DATA2& stDT_HERO_UPGRADE_DATA2)
{
    stDT_HERO_UPGRADE_DATA2.wHeroKindID = m_stDT_HERO_BASE_DATA.wKindID;
    stDT_HERO_UPGRADE_DATA2.byUpgradeFlag = m_stDT_HERO_UPGRADE_DATA.byUpgradeFlag;
    stDT_HERO_UPGRADE_DATA2.wCurLevel = m_stDT_HERO_UPGRADE_DATA.wLevel;
    stDT_HERO_UPGRADE_DATA2.dwEncExperience = m_stDT_HERO_UPGRADE_DATA.dwEncExperience;
    //stDT_HERO_UPGRADE_DATA2.dwExperience = m_stDT_HERO_UPGRADE_DATA.dwExperience;
    //stDT_HERO_UPGRADE_DATA2.dwUpgradeExperience = m_stDT_HERO_UPGRADE_DATA.dwUpgradeExperience;
    //经验下发差值
    GetExperience(stDT_HERO_UPGRADE_DATA2.dwExperience, stDT_HERO_UPGRADE_DATA2.dwUpgradeExperience);

    if((1 == m_stDT_HERO_BASE_DATA.byCoach) && (1 == m_stDT_HERO_UPGRADE_DATA.byUpgradeFlag))
    {
        stDT_HERO_UPGRADE_DATA2.wCanUseBoxGoodsID = m_poOwner->GetBoxGoodsPrompt();
    }

    m_stDT_HERO_UPGRADE_DATA.byUpgradeFlag = 0;
    m_stDT_HERO_UPGRADE_DATA.dwEncExperience = 0;
}

BOOL CHero::InitAbsorb(DT_ABSORB_DATA_LST *pstAbsorbDataLst)
{
    memcpy(&m_stAbsorbDataLst, pstAbsorbDataLst, sizeof(DT_ABSORB_DATA_LST));
    return TRUE;
}


BOOL CHero::InitEquip(DT_HERO_EQUIP_DATA_LST* pstDT_EQUIP_DATA_LST)
{
    CPlayerEquipShmemMap& mapPlayerEquip = m_poOwner->GetAllEquip();
    CItemKind2IdxMap& mapEquipKind2Idx = m_poOwner->GetEquipKind2IdxMap();
    UINT16 wEquipNum = pstDT_EQUIP_DATA_LST->wEquipNum;
    for(UINT16 wIdx = 0; wIdx < wEquipNum; wIdx++)
    {
        DT_EQUIP_DATA* pstDT_EQUIP_DATA = &(pstDT_EQUIP_DATA_LST->astEquipList[wIdx]);

        if(pstDT_EQUIP_DATA->wHeroKindID != m_stDT_HERO_BASE_DATA.wKindID)
        {
            //异常
            SYS_CRITICAL(_SDT("[%s: %d]: InitEquip failed! HeroKindID is not match [%d != %d]!"), MSG_MARK, pstDT_EQUIP_DATA->wHeroKindID, m_stDT_HERO_BASE_DATA.wKindID);
            continue;
        }

        SEquipProperty stEquipProperty;
        stEquipProperty.pstEquip = pstDT_EQUIP_DATA;
        stEquipProperty.wEnchantLevel = pstDT_EQUIP_DATA_LST->awEnchantLevel[wIdx];

        CEquip* poEquip = dynamic_cast<CEquip*>(CItemMgr::Instance()->AttachItem(m_poOwner, pstDT_EQUIP_DATA->wKindID, &stEquipProperty));
        if(NULL == poEquip)
        {
            return FALSE;
        }

        SEquipProp* poEquipProp = poEquip->GetEquipProp();
        if (NULL == poEquipProp)
        {
            return FALSE;
        }

        m_mapEquipPos.AddData(poEquipProp->byPosKindID, poEquip);
        poEquip->SetHero(this);
        m_mapEquip.AddData(poEquip->GetMemID(), poEquip);

        mapPlayerEquip.AddData(CLogicFunc::Get1616Key(poEquip->GetDT_EQUIP_DATA().wKindID, poEquip->GetDT_EQUIP_DATA().wIdx), poEquip);
    }

    return TRUE;
}

BOOL CHero::InitDrug(DT_DRUG_DATA_LST* pstDrugDataInfo)
{
    if(NULL == m_poOwner)
    {
        return FALSE;
    }
    UINT32	dwPlayerID		= m_poOwner->GetDT_PLAYER_BASE_DATA().dwID;
    UINT16	wHeroKindID		= m_stDT_HERO_BASE_DATA.wKindID;
    UINT8	byDrugLevelNum	= pstDrugDataInfo->byDrugLevelNum;
    for(UINT16 wIdx = 0; wIdx < byDrugLevelNum; wIdx++)
    {
        DT_DRUG_DATA* pstDT_DRUG_DATA = &(pstDrugDataInfo->astDrugList[wIdx]);

        CDrug* poDrug = CDrugMgr::Instance()->CreateDrug(dwPlayerID, wHeroKindID, pstDT_DRUG_DATA->byLevel);
        if(NULL == poDrug)
        {
            return FALSE;
        }
        if(!poDrug->Init(pstDT_DRUG_DATA, m_poOwner))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: poDrug->Init failed!"), MSG_MARK);
            CDrugMgr::Instance()->RemoveDrug(dwPlayerID, wHeroKindID, pstDT_DRUG_DATA->byLevel);
            return FALSE;
        }

        m_mapLevel2Drug.AddData(pstDT_DRUG_DATA->byLevel, poDrug);
    }

    return TRUE;
}


BOOL CHero::InitGodweapon(DT_GODWEAPON_DATA* pstDT_GODWEAPON_DATA)
{
    //没有神器
    if(0 == pstDT_GODWEAPON_DATA->byHaveDataFlag)
    {
        return TRUE;
    }

    CGodWeapon* poGodWeapon = CGodweaponMgr::Instance()->CreateGodWeapon(m_poOwner->GetDT_PLAYER_BASE_DATA().dwID, m_stDT_HERO_BASE_DATA.wKindID);
    if(NULL == poGodWeapon)
    {
        return FALSE;
    }
    if(!poGodWeapon->Init(pstDT_GODWEAPON_DATA, m_poOwner, this))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CGodWeapon init failed!"), MSG_MARK);
        CGodweaponMgr::Instance()->RemoveGodWeapon(m_poOwner->GetDT_PLAYER_BASE_DATA().dwID, m_stDT_HERO_BASE_DATA.wKindID);
        return FALSE;
    }
    m_poGodWeapon = poGodWeapon;
    SetOtherLife(m_poGodWeapon);

    return TRUE;
}

VOID CHero::SetFormationData(UINT8 byKindID , UINT32 dwValue )
{
	if ( m_bIsOpenFormation )//测试打开阵法战力数据
	{
		m_stFormation.byAttrKind = byKindID;
		m_stFormation.dwAttrValue = dwValue;
	}
}

VOID CHero::SetFormationData()
{
	if ( !m_bIsOpenFormation)
	{
		if ( IsFormationHero())
		{
			//阵法
			CFormation& oFormation = m_poOwner->GetFormation();
			const DT_NEW_FORMATION_DATA& stNewFormationData = oFormation.GetDT_FORMATION_DATA();
			if ( ESM_YES == stNewFormationData.byIsUseNewFormation)
			{
				const DT_NEW_FORMATION_INFO& stNewFormationInfo = stNewFormationData.stNewFormationLst.astFormationArray[stNewFormationData.byNewSelectIdx];
				const SFormationUpgradeProp* poFormationProp = CFormationPropMgr::Instance()->GetFormationUpgrade(stNewFormationInfo.byFormationID, stNewFormationInfo.byFormationLevel);
				if ( NULL != poFormationProp)
				{
					m_stFormation.byAttrKind = poFormationProp->byAttrKindID;
					m_stFormation.dwAttrValue = poFormationProp->dwAttrValue;
				}
			}
		}
		else
		{
			memset(&m_stFormation, 0, sizeof(DT_ATTR_INC_DATA));
		}
	}
}

VOID CHero::SetOpenFormationStatu(BOOL bStatu )
{
	//TRUE为打开阵法计算战力数据， FALSE为正常数据
	m_bIsOpenFormation = bStatu;
}

VOID CHero::SoulPotencyAttribute(UINT16 wSoulID, UINT8 byPotencyLevel, UINT8 byPotencySubLevel)
{
	DT_POTENCY_ATTR_LST_INFO stAttrInfo = {0};
	CSoulPropMgr::Instance()->GetSoulPotencyCountAttr(wSoulID, byPotencyLevel, byPotencySubLevel, stAttrInfo);
	for(UINT8 byIdx = 0; byIdx < stAttrInfo.byAttrNum; byIdx++)
	{
		AddBattleAttrBySoulPotency( stAttrInfo.astAttrInfo[byIdx].byPotencyID, stAttrInfo.astAttrInfo[byIdx].dwPotencyValue);
	}
}

VOID CHero::AddBattleAttrBySoulPotency(UINT8 byPotencyID, UINT32 dwValue)
{
	switch (byPotencyID)
	{
	case ESP_HP:
		m_stBattleAttribute.qwHP += dwValue;
		break;
	case ESP_ATTACK:
		m_stBattleAttribute.dwAttack += dwValue;
		break;
	case ESP_HITRATE:
		m_stHideBattleAttr.wHitRate += dwValue;
		break;
	case ESP_DODGERATE:
		m_stHideBattleAttr.wDodgeRate += dwValue;
		break;
	case ESP_CRITRATE:
		m_stHideBattleAttr.wCritRate += dwValue;
		break;
	case ESP_DECRITRATE:
		m_stHideBattleAttr.wDeCritRate += dwValue;
		break;
	case ESP_DEFEND:
		m_stBattleAttribute.dwDefend += dwValue;
		break;
	}
}

VOID CHero::InitBattleAttribute()
{
	ResetJewelryAttr();
    memset(&m_stBattleAttribute, 0, sizeof(m_stBattleAttribute));
    memset(&m_stHideBattleAttr, 0, sizeof(m_stHideBattleAttr));
    memset(&m_stExtBattleAttr, 0, sizeof(m_stExtBattleAttr));

    SHeroProp* pstHeroProp = GetHeroProp();
    if(NULL == pstHeroProp)
    {
        return;
    }
	SetFormationData();
    //攻击血量
    m_stBattleAttribute.dwAttack += pstHeroProp->dwAttack;
    m_stBattleAttribute.qwHP += pstHeroProp->dwHP;

    ////隐藏属性
    m_stHideBattleAttr.wHitRate = pstHeroProp->wHitRate;
    m_stHideBattleAttr.wDodgeRate = pstHeroProp->wDodgeRate;
    m_stHideBattleAttr.wCritRate = pstHeroProp->wCritRate;
    m_stHideBattleAttr.wDeCritRate = pstHeroProp->wDeCritRate;
    m_stHideBattleAttr.wAngryValue = pstHeroProp->wAngryValue;

    //计算科技升级属性添加的
    CScienceTree *poScienceTree = m_poOwner->GetScienceTree();
    if(NULL != poScienceTree)
    {
        UINT32 dwCost, dwAddValue = 0;
		UINT32 dwAddLevel = 0;
		
        CScienceUpdatePropMgr::Instance()->GetInfo( EBAK_ATTACK, AddScienceLevel(poScienceTree->GetDT_SCIENCE_TREE_DATA().wAttractLevel), dwCost, dwAddValue );
        m_stBattleAttribute.dwAttack += dwAddValue;

        CScienceUpdatePropMgr::Instance()->GetInfo( EBAK_HP,
                AddScienceLevel(poScienceTree->GetDT_SCIENCE_TREE_DATA().wHPLevel), dwCost, dwAddValue );
        m_stBattleAttribute.qwHP += dwAddValue;

		CScienceUpdatePropMgr::Instance()->GetInfo( EBAK_FIRSTATTACK,
				AddScienceLevel(poScienceTree->GetDT_SCIENCE_TREE_DATA().dwFirstAttack), dwCost, dwAddValue );
		m_stExtBattleAttr.dwFirstAttack += dwAddValue;

        CScienceUpdatePropMgr::Instance()->GetInfo( EBAK_HITRATE,
                AddScienceLevel(poScienceTree->GetDT_SCIENCE_TREE_DATA().wHitLevel), dwCost, dwAddValue );
        m_stHideBattleAttr.wHitRate += dwAddValue;

        CScienceUpdatePropMgr::Instance()->GetInfo( EBAK_DODGERATE,
                AddScienceLevel(poScienceTree->GetDT_SCIENCE_TREE_DATA().wDodgeLevel), dwCost, dwAddValue );
        m_stHideBattleAttr.wDodgeRate += dwAddValue;

        CScienceUpdatePropMgr::Instance()->GetInfo( EBAK_CRITRATE,
                AddScienceLevel(poScienceTree->GetDT_SCIENCE_TREE_DATA().wCritLevel), dwCost, dwAddValue );
        m_stHideBattleAttr.wCritRate += dwAddValue;

        CScienceUpdatePropMgr::Instance()->GetInfo( EBAK_DECRITRATE,
                AddScienceLevel(poScienceTree->GetDT_SCIENCE_TREE_DATA().wShieldLevel), dwCost, dwAddValue );
        m_stHideBattleAttr.wDeCritRate += dwAddValue;

        CScienceUpdatePropMgr::Instance()->GetInfo( EBAK_ATTACK2,
				AddScienceLevel (poScienceTree->GetDT_SCIENCE_TREE_DATA().wAttractLevel2), dwCost, dwAddValue );
        m_stBattleAttribute.dwAttack += dwAddValue;

        CScienceUpdatePropMgr::Instance()->GetInfo( EBAK_HP2,
				 AddScienceLevel( poScienceTree->GetDT_SCIENCE_TREE_DATA().wHPLevel2), dwCost, dwAddValue );
        m_stBattleAttribute.qwHP += dwAddValue;

		AddBattleAttr( m_stFormation.byAttrKind, m_stFormation.dwAttrValue);

    }
    //武堂技能
    CFactionPlayer* poFactionPlayer = m_poOwner->GetFactionPlayer();
    if (NULL != poFactionPlayer)
    {
        UINT32 dwAddAttrValue = 0;
        //攻击
        CGymnasiumPropMgr::Instance()->AddAttribute(EBAK_ATTACK, poFactionPlayer->GetGymnasiumAttrLevel(EBAK_ATTACK), dwAddAttrValue);
        AddBattleAttr(EBAK_ATTACK, dwAddAttrValue);
        //HP
        CGymnasiumPropMgr::Instance()->AddAttribute(EBAK_HP, poFactionPlayer->GetGymnasiumAttrLevel(EBAK_HP), dwAddAttrValue);
        AddBattleAttr(EBAK_HP, dwAddAttrValue);
        //破击
        CGymnasiumPropMgr::Instance()->AddAttribute(EBAK_HITRATE, poFactionPlayer->GetGymnasiumAttrLevel(EBAK_HITRATE), dwAddAttrValue);
        AddBattleAttr(EBAK_HITRATE, dwAddAttrValue);
        //格档
        CGymnasiumPropMgr::Instance()->AddAttribute(EBAK_DODGERATE, poFactionPlayer->GetGymnasiumAttrLevel(EBAK_DODGERATE), dwAddAttrValue);
        AddBattleAttr(EBAK_DODGERATE, dwAddAttrValue);
        //暴击
        CGymnasiumPropMgr::Instance()->AddAttribute(EBAK_CRITRATE, poFactionPlayer->GetGymnasiumAttrLevel(EBAK_CRITRATE), dwAddAttrValue);
        AddBattleAttr(EBAK_CRITRATE, dwAddAttrValue);
        //抗暴
        CGymnasiumPropMgr::Instance()->AddAttribute(EBAK_DECRITRATE, poFactionPlayer->GetGymnasiumAttrLevel(EBAK_DECRITRATE), dwAddAttrValue);
        AddBattleAttr(EBAK_DECRITRATE, dwAddAttrValue);
        //增伤
        CGymnasiumPropMgr::Instance()->AddAttribute(EBAK_INCHURTRATE, poFactionPlayer->GetGymnasiumAttrLevel(EBAK_INCHURTRATE), dwAddAttrValue);
        AddBattleAttr(EBAK_INCHURTRATE, dwAddAttrValue);
        //减伤
        CGymnasiumPropMgr::Instance()->AddAttribute(EBAK_DECHURTRATE, poFactionPlayer->GetGymnasiumAttrLevel(EBAK_DECHURTRATE), dwAddAttrValue);
        AddBattleAttr(EBAK_DECHURTRATE, dwAddAttrValue);
    }

	//饰品基本属性
	CJewelry* poJewelry = m_mapJewelry.GetFistData();
	while( poJewelry )
	{
		DT_BATTLE_ATTR_LST_INFO& stJewelryInfo = poJewelry->GetBaseAttribute();
		for ( UINT8 byIdx = 0; byIdx < stJewelryInfo.byAttrNum; byIdx++)
		{
			AddBattleAttr(stJewelryInfo.astAttrInfo[byIdx].byAttrKind, stJewelryInfo.astAttrInfo[byIdx].dwAttrValue);
		}

		poJewelry = m_mapJewelry.GetNextData();
	}

    //+器魂属性
    if(m_poGodWeapon)
    {
        const SAllAttrProp* pstGodWeaponAttr = m_poGodWeapon->GetCurGodWeaponAttr();
        if(pstGodWeaponAttr)
        {
            //不分版本，全按照新版逻辑
            DT_BATTLE_BASE_ATTRIBUTE	stBattleAttributeTmp = {0};
            CLogicFunc::AddAttr(pstGodWeaponAttr, stBattleAttributeTmp, m_stHideBattleAttr, m_stExtBattleAttr);
			m_poGodWeapon->AddBattleByJewelry();
			m_stBattleAttribute.dwDefend += static_cast<UINT32>(pstGodWeaponAttr->stBattleAttr.qwHP + m_poGodWeapon->GetExtDefend());
        }
    }

	CPlayerSoul* pPlayerSoul = m_poOwner->GetPlayerSoul();
	if (NULL != pPlayerSoul) 
	{
		if (m_HeroSoulData.wSoulNum > MAX_SOUL_EQUIP_NUM) {
			CSDLoggerImpl::SysCritical(_SDT("[%s: %d]: SoulNum %d"), MSG_MARK, m_HeroSoulData.wSoulNum);
		}

		// 将魂属性
		for (size_t n = 0; n < m_HeroSoulData.wSoulNum && n < MAX_SOUL_EQUIP_NUM ; ++n) 
		{
			for (size_t nAttr = 0; nAttr < m_HeroSoulData.astSoulLstData[n].byBaseNum && nAttr < MAX_SOUL_ATTR_NUM; ++ nAttr) 
			{
				DT_SOUL_ATTR_DATA& rSoulAttrData = m_HeroSoulData.astSoulLstData[n].astSoulAttr[nAttr];
				AddBattleAttr(rSoulAttrData.byKindID, rSoulAttrData.dwKindValue);
			}

			SoulPotencyAttribute(m_HeroSoulData.astSoulLstData[n].wSoulID, m_HeroSoulData.astSoulPotencyLstData[n].byPotencyLevel, m_HeroSoulData.astSoulPotencyLstData[n].byPotencySubLevel);
		}
	}

    //主角境界
    if(1 == m_stDT_HERO_BASE_DATA.byCoach)
    {
        CJingJie* poJingJie = m_poOwner->GetJingJie();
        if(poJingJie)
        {
            const SAllAttrProp* pstProp = poJingJie->GetAllIncAttr();
            if(pstProp)
            {
                CLogicFunc::AddAttr(pstProp, m_stBattleAttribute, m_stHideBattleAttr, m_stExtBattleAttr);
            }
        }
    }

    m_stBattleAttribute.qwHP += GetAttrUpdateAddition( EAST_HP, m_stDT_HERO_BASE_DATA.byCoach );

    m_stBattleAttribute.dwAttack += GetAttrUpdateAddition( EAST_ATTACK, m_stDT_HERO_BASE_DATA.byCoach );

    UINT8	byAwakenVersion = 0;
    CAwaken *poAwaken = m_poOwner->GetAwaken();
    if (poAwaken)
    {
        byAwakenVersion = poAwaken->GetAwakenVersion();
    }
    //+卦象
    if(2 == byAwakenVersion)
    {
        AddStudyBattleAttr2();
    }
    else
    {
        AddStudyBattleAttr();
    }


    //+武将拥有的扩展属性
    m_stBattleAttribute.qwHP += m_stDT_HERO_BASE_DATA.stBattleAttribute.dwHP;
    m_stBattleAttribute.dwAttack += m_stDT_HERO_BASE_DATA.stBattleAttribute.dwAttack;

    //+装备计算装备拥有属性
    DT_BATTLE_BASE_ATTRIBUTE stEquipAttr = BattleAttrCountByEquip();
    m_stBattleAttribute.qwHP += stEquipAttr.qwHP;
    m_stBattleAttribute.dwAttack += stEquipAttr.dwAttack;
    m_stBattleAttribute.dwDefend += stEquipAttr.dwDefend;
	
    //+资质(等级/资质/丹药/神器)拥有的属性
    DT_BATTLE_BASE_ATTRIBUTE stTalentAttr;
    stTalentAttr = BattleAttrCountByLevelTalent();
    m_stBattleAttribute.qwHP += stTalentAttr.qwHP;
    m_stBattleAttribute.dwAttack += stTalentAttr.dwAttack;
	m_stBattleAttribute.dwDefend += stTalentAttr.dwDefend;
    //个人先攻值=（生命+攻击*5）/10+科技先攻
    //m_stExtBattleAttr.dwFirstAttack += (m_stBattleAttribute.dwHP + m_stBattleAttribute.dwAttack * 5) / 10;

    //就算装备镶嵌获得的总的数值
    map<UINT32, UINT32> mapCurAttr;
    mapCurAttr[EBAK_HP] = static_cast<UINT32>(m_stBattleAttribute.qwHP);
    mapCurAttr[EBAK_ATTACK] = m_stBattleAttribute.dwAttack;
    mapCurAttr[EBAK_HITRATE] = m_stHideBattleAttr.wHitRate;
    mapCurAttr[EBAK_DODGERATE] = m_stHideBattleAttr.wDodgeRate;
    mapCurAttr[EBAK_CRITRATE] = m_stHideBattleAttr.wCritRate;
    mapCurAttr[EBAK_DECRITRATE] = m_stHideBattleAttr.wDeCritRate;
	mapCurAttr[EBAK_DEFEND] = m_stBattleAttribute.dwDefend;
	C3232Map mapMedalAttr;
	CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
	if ( NULL != poPlayerMedal)
	{
		mapMedalAttr = poPlayerMedal->GetBattleAttrFix();
	}
	
	JewelryCountBySelf(mapCurAttr);//饰品加成
	mapCurAttr[EBAK_HP] += (m_mapJewelryValue[EBAK_HP] + mapMedalAttr[EBAK_HP]);
	mapCurAttr[EBAK_ATTACK] += (m_mapJewelryValue[EBAK_ATTACK] + mapMedalAttr[EBAK_ATTACK]);
	mapCurAttr[EBAK_DEFEND] += (m_mapJewelryValue[EBAK_DEFEND] + mapMedalAttr[EBAK_DEFEND]);

	mapCurAttr[EBAK_HITRATE] += (m_mapJewelryValue[EBAK_HITRATE] + mapMedalAttr[EBAK_HITRATE]);
	mapCurAttr[EBAK_DODGERATE] += (m_mapJewelryValue[EBAK_DODGERATE] + mapMedalAttr[EBAK_DODGERATE]);
	mapCurAttr[EBAK_CRITRATE] += (m_mapJewelryValue[EBAK_CRITRATE] + mapMedalAttr[EBAK_CRITRATE]);
	mapCurAttr[EBAK_DECRITRATE] += (m_mapJewelryValue[EBAK_DECRITRATE] + mapMedalAttr[EBAK_DECRITRATE]);
    XiangqianAttrCount( mapCurAttr );
	mapMedalAttr.clear();
	if ( NULL != poPlayerMedal)
	{
		mapMedalAttr = poPlayerMedal->GetBattleAttrPercent();
	}

    m_stBattleAttribute.qwHP = (mapCurAttr[EBAK_HP] * (100 + mapMedalAttr[EBAK_HP]) / 100);
    m_stBattleAttribute.dwAttack = (mapCurAttr[EBAK_ATTACK] * (100 + mapMedalAttr[EBAK_ATTACK]) / 100);
	m_stBattleAttribute.dwDefend = (mapCurAttr[EBAK_DEFEND] * (100 + mapMedalAttr[EBAK_DEFEND]) / 100);

    m_stHideBattleAttr.wHitRate = (mapCurAttr[EBAK_HITRATE] * (100 + mapMedalAttr[EBAK_HITRATE]) / 100);
    m_stHideBattleAttr.wDodgeRate = (mapCurAttr[EBAK_DODGERATE] * (100 + mapMedalAttr[EBAK_DODGERATE]) / 100);
    m_stHideBattleAttr.wCritRate = (mapCurAttr[EBAK_CRITRATE] * (100 + mapMedalAttr[EBAK_CRITRATE]) / 100);
    m_stHideBattleAttr.wDeCritRate = (mapCurAttr[EBAK_DECRITRATE] * (100 + mapMedalAttr[EBAK_DECRITRATE]) / 100);

    //初始化客户端信息
    //InitCliData(stEquipAttr);

    //个人先攻值=个人战斗力/50+科技先攻
    //m_stExtBattleAttr.dwFirstAttack += (m_stBattleAttribute.dwHP + m_stBattleAttribute.dwAttack * 5) / 10;
    m_stExtBattleAttr.dwFirstAttack += GetPower() / 50;

}

UINT32 CHero::GetAttrUpdateAddition( UINT8 byAttr, BOOL bCoachHero )
{
    SUpdateData updateData;

    if( EAST_HP == byAttr )
    {
        if( !CUpdateAttrPropMgr::Instance()->GetUpdateAttrNeedInfo( bCoachHero,
                EAST_HP, GetHeroProp()->byCareerID, GetAttrLevel( byAttr ) + 1,
                GetTalent(), updateData, GetDT_HERO_BASE_DATA().wLevel ) )
        {
            SYS_CRITICAL( _SDT( "[%s : %d] calc player attr failed, ID:%d" ), MSG_MARK, m_stDT_HERO_BASE_DATA.wKindID );
            return 0;
        }

        return m_stDT_HERO_BASE_DATA.stUpdateAttrInfo.wAttrHP * updateData.dwPerCountAddValue;
    }
    else if( EAST_ATTACK == byAttr )
    {
        if( !CUpdateAttrPropMgr::Instance()->GetUpdateAttrNeedInfo( bCoachHero,
                EAST_ATTACK, GetHeroProp()->byCareerID, GetAttrLevel( byAttr ) + 1,
                GetTalent(), updateData, m_stDT_HERO_BASE_DATA.wLevel ) )
        {
            SYS_CRITICAL( _SDT( "[%s : %d] calc player attr failed,ID:%d" ), MSG_MARK, m_stDT_HERO_BASE_DATA.wKindID );
            return 0;
        }

        return m_stDT_HERO_BASE_DATA.stUpdateAttrInfo.wAttrAttack * updateData.dwPerCountAddValue;
    }
    else
    {
        SYS_CRITICAL( _SDT( "calc player attr, unknow attr,ID:%d" ), MSG_MARK, byAttr );
        return 0;
    }
}


UINT16 CHero::GetAttrLevel( UINT8 byAttrKind )
{
    if( byAttrKind == EAST_HP )
    {
        return GetDT_HERO_BASE_DATA().stUpdateAttrInfo.wAttrHP;
    }
    else if( byAttrKind == EAST_ATTACK )
    {
        return GetDT_HERO_BASE_DATA().stUpdateAttrInfo.wAttrAttack;
    }
    else
    {
        SYS_CRITICAL( _SDT("[%s: %d]unknow attr kind:%d"), MSG_MARK, byAttrKind );
        return 0;
    }
}



VOID CHero::AddStudyBattleAttr()
{
    for(UINT8 byIdx = 0 ; byIdx < MAX_HERO_STUDY_GRID_NUM; byIdx++)
    {
        DT_STUDY_DATA& stDT_STUDY_DATA = m_astStudy[byIdx];
        if(0 == stDT_STUDY_DATA.byStudyColorKind) //没数据
        {
            continue;
        }
        UINT32 dwValue = CAwakenPropMgr::Instance()->GetStudyAttrValue(stDT_STUDY_DATA.byStudyColorKind, stDT_STUDY_DATA.byStudyAttrKind1, stDT_STUDY_DATA.byStudyLevel);
        switch(stDT_STUDY_DATA.byStudyAttrKind1)
        {
        case EBAK_HP:
            m_stBattleAttribute.qwHP += dwValue;
            break;
        case EBAK_ATTACK:
            m_stBattleAttribute.dwAttack += dwValue;
            break;
        case EBAK_HITRATE:
            m_stHideBattleAttr.wHitRate += dwValue;
            break;
        case EBAK_DODGERATE:
            m_stHideBattleAttr.wDodgeRate += dwValue;
            break;
        case EBAK_CRITRATE:
            m_stHideBattleAttr.wCritRate += dwValue;
            break;
        case EBAK_DECRITRATE:
            m_stHideBattleAttr.wDeCritRate += dwValue;
            break;
        case EBAK_INCHURTRATE:
            m_stExtBattleAttr.wIncHurtRate += dwValue;
            break;
        case EBAK_DECHURTRATE:
            m_stExtBattleAttr.wDecHurtRate += dwValue;
            break;
        }
    }
}

BOOL CHero::CkHaveStudyAttrKind(UINT8 byStudyAttrKind)
{
    for(UINT8 byIdx = 0 ; byIdx < MAX_HERO_STUDY_GRID_NUM; byIdx++)
    {
        DT_STUDY_DATA& stDT_STUDY_DATA = m_astStudy[byIdx];
        if(0 == stDT_STUDY_DATA.byStudyColorKind) //没数据
        {
            continue;
        }
        if(stDT_STUDY_DATA.byStudyAttrKind1 == byStudyAttrKind)
        {
            return TRUE;
        }
    }

    return FALSE;
}


VOID CHero::AddStudyBattleAttr2()
{
    for(UINT8 byIdx = 0 ; byIdx < MAX_HERO_STUDY_GRID_NUM; byIdx++)
    {
        DT_STUDY_DATA& stDT_STUDY_DATA_X = m_astStudy[byIdx];
        if(0 == stDT_STUDY_DATA_X.byStudyColorKind) //没数据
        {
            continue;
        }

        //过虑掉重复数据
        BOOL bRepeat = FALSE;
        for(UINT8 byIdy = 0 ; byIdy < byIdx; byIdy++)
        {
            DT_STUDY_DATA& stDT_STUDY_DATA_Y = m_astStudy[byIdy];

            if (stDT_STUDY_DATA_Y.byStudyAttrKind1 == stDT_STUDY_DATA_X.byStudyAttrKind1 &&
                    stDT_STUDY_DATA_Y.byStudyAttrKind2 == stDT_STUDY_DATA_X.byStudyAttrKind2)
            {
                bRepeat = TRUE;
                break;
            }
        }

        if (bRepeat)
        {
            continue;
        }

        UINT32 dwValue1 = 0;
        UINT32 dwValue2 = 0;
        CAwakenPropMgr2::Instance()->GetStudyAttrValue(stDT_STUDY_DATA_X.byStudyColorKind, stDT_STUDY_DATA_X.byStudyAttrKind1, stDT_STUDY_DATA_X.byStudyAttrKind2, stDT_STUDY_DATA_X.byStudyLevel, dwValue1, dwValue2);
        AddBattleAttr(stDT_STUDY_DATA_X.byStudyAttrKind1, dwValue1);
		//增加饰品对卦象的影响（增伤，减伤除外）
		if ( stDT_STUDY_DATA_X.byStudyAttrKind1 != EBAK_INCHURTRATE && EBAK_DECHURTRATE != stDT_STUDY_DATA_X.byStudyAttrKind1)
		{
			AddBattleAttr(stDT_STUDY_DATA_X.byStudyAttrKind1, AddValueByValueType(dwValue1, CJewelryPropMgr::Instance()->GetValueType( EJEA_ALL_STUDY), CJewelryPropMgr::Instance()->GetValueType( EJEA_ALL_STUDY) ));
		}
        AddBattleAttr(stDT_STUDY_DATA_X.byStudyAttrKind2, dwValue2);
		if ( stDT_STUDY_DATA_X.byStudyAttrKind2 != EBAK_INCHURTRATE && EBAK_DECHURTRATE != stDT_STUDY_DATA_X.byStudyAttrKind2)
		{
			AddBattleAttr(stDT_STUDY_DATA_X.byStudyAttrKind1, AddValueByValueType(dwValue1, CJewelryPropMgr::Instance()->GetValueType( EJEA_ALL_STUDY), CJewelryPropMgr::Instance()->GetValueType( EJEA_ALL_STUDY)));
		}
    }
}


VOID CHero::AddBattleAttr(UINT8 byAttrKind, UINT32 dwValue, UINT8 byValueType)
{
    switch(byAttrKind)
    {
    case EBAK_HP:
        m_stBattleAttribute.qwHP += AddValueByValueType(m_stBattleAttribute.qwHP, dwValue, byValueType);
        break;
    case EBAK_ATTACK:
        m_stBattleAttribute.dwAttack += AddValueByValueType(m_stBattleAttribute.dwAttack, dwValue, byValueType);
        break;
    case EBAK_HITRATE:
        m_stHideBattleAttr.wHitRate += AddValueByValueType(m_stHideBattleAttr.wHitRate, dwValue, byValueType);;
        break;
    case EBAK_DODGERATE:
        m_stHideBattleAttr.wDodgeRate += AddValueByValueType(m_stHideBattleAttr.wDodgeRate, dwValue, byValueType);;
        break;
    case EBAK_CRITRATE:
        m_stHideBattleAttr.wCritRate += AddValueByValueType(m_stHideBattleAttr.wCritRate, dwValue, byValueType);;
        break;
    case EBAK_DECRITRATE:
        m_stHideBattleAttr.wDeCritRate += AddValueByValueType(m_stHideBattleAttr.wDeCritRate, dwValue, byValueType);;
        break;
    case EBAK_INCHURTRATE:
        m_stExtBattleAttr.wIncHurtRate += AddValueByValueType(m_stExtBattleAttr.wIncHurtRate, dwValue, byValueType);;
        break;
    case EBAK_DECHURTRATE:
        m_stExtBattleAttr.wDecHurtRate += AddValueByValueType(m_stExtBattleAttr.wDecHurtRate, dwValue, byValueType);;
        break;
	case EBAK_FIRSTATTACK:
		m_stExtBattleAttr.dwFirstAttack += AddValueByValueType(m_stExtBattleAttr.dwFirstAttack, dwValue, byValueType);;
		break;
	case EBAK_DEFEND:
		m_stBattleAttribute.dwDefend += AddValueByValueType(m_stBattleAttribute.dwDefend, dwValue, byValueType);;
		break;
    default:
        break;
    }
}

VOID CHero::DecBattleAttr(UINT8 byAttrKink, UINT32 dwValue)
{
	switch(byAttrKink)
	{
	case EBAK_HP:
		m_stBattleAttribute.qwHP -= dwValue;
		break;
	case EBAK_ATTACK:
		m_stBattleAttribute.dwAttack -= dwValue;
		break;
	case EBAK_HITRATE:
		m_stHideBattleAttr.wHitRate -= dwValue;
		break;
	case EBAK_DODGERATE:
		m_stHideBattleAttr.wDodgeRate -= dwValue;
		break;
	case EBAK_CRITRATE:
		m_stHideBattleAttr.wCritRate -= dwValue;
		break;
	case EBAK_DECRITRATE:
		m_stHideBattleAttr.wDeCritRate -= dwValue;
		break;
	case EBAK_INCHURTRATE:
		m_stExtBattleAttr.wIncHurtRate -= dwValue;
		break;
	case EBAK_DECHURTRATE:
		m_stExtBattleAttr.wDecHurtRate -= dwValue;
		break;
	case EBAK_FIRSTATTACK:
		m_stExtBattleAttr.dwFirstAttack -= dwValue;
	default:
		break;
	}
}

BOOL CHero::CkHaveStudyAttrKind2(UINT8 byStudyAttrKind1, UINT8 byStudyAttrKind2)
{
    for(UINT8 byIdx = 0 ; byIdx < MAX_HERO_STUDY_GRID_NUM; byIdx++)
    {
        DT_STUDY_DATA& stDT_STUDY_DATA = m_astStudy[byIdx];
        if(0 == stDT_STUDY_DATA.byStudyColorKind) //没数据
        {
            continue;
        }
        if(0 != byStudyAttrKind1)
        {
            if((stDT_STUDY_DATA.byStudyAttrKind1 == byStudyAttrKind1)
                    || (stDT_STUDY_DATA.byStudyAttrKind2 == byStudyAttrKind1))
            {
                return TRUE;
            }
        }
        if(0 != byStudyAttrKind2)
        {
            if((stDT_STUDY_DATA.byStudyAttrKind1 == byStudyAttrKind2)
                    || (stDT_STUDY_DATA.byStudyAttrKind2 == byStudyAttrKind2))
            {
                return TRUE;
            }
        }
    }

    return FALSE;
}

//获取属性卦象位置，没有返回FALSE
BOOL CHero::GetStudyAttrGrid(UINT8 byStudyAttrKind, UINT8& byGridIdx)
{
    for(UINT8 byIdx = 0 ; byIdx < MAX_HERO_STUDY_GRID_NUM; byIdx++)
    {
        DT_STUDY_DATA& stDT_STUDY_DATA = m_astStudy[byIdx];
        if(0 == stDT_STUDY_DATA.byStudyColorKind) //没数据
        {
            continue;
        }
        if((stDT_STUDY_DATA.byStudyAttrKind1 == byStudyAttrKind)
                || (stDT_STUDY_DATA.byStudyAttrKind2 == byStudyAttrKind) )
        {
            byGridIdx = byIdx;
            return TRUE;
        }
    }

    return FALSE;
}

void CHero::AddJewelrySuitAttr()
{
    if(!IsDressJewelrySuit()){
        return;
    }

    SJewelrySuitProp *pstJewelrySuitProp = CJewelryPropMgr::Instance()->GetJewelrySuitProp(m_wJewelrySuitDressLvl, m_wJewelrySuitLvl);

    // 套装的属性加成 = 第1个等级的属性 + 第2个等级的属性 + ... 当前等级的属性
    for(UINT16 wAttrIdx = 0; wAttrIdx < pstJewelrySuitProp->vecAttr.size(); wAttrIdx++){
        SJewelrySuitAttrProp &stAttr = pstJewelrySuitProp->vecAttr[wAttrIdx];
        if (stAttr.wJewelryStrengLvl > m_wJewelrySuitLvl){
            break;
        }

        if (CJewelry::IsOneHeroAttr(stAttr.eAttrType)){
            m_mapJewelryAttr[stAttr.eAttrType] += stAttr.dwAttrVal;
        }
    }
}

void CHero::GetAllHeroAddAttr(C3232Map &mapAttr)
{
    if(!IsDressJewelrySuit()){
        return;
    }

    SJewelrySuitProp *pstJewelrySuitProp = CJewelryPropMgr::Instance()->GetJewelrySuitProp(m_wJewelrySuitDressLvl, m_wJewelrySuitLvl);

    // 套装的属性加成 = 第1个等级的属性 + 第2个等级的属性 + ... 当前等级的属性
    for(UINT16 wAttrIdx = 0; wAttrIdx < pstJewelrySuitProp->vecAttr.size(); wAttrIdx++){
        SJewelrySuitAttrProp &stAttr = pstJewelrySuitProp->vecAttr[wAttrIdx];
        if (stAttr.wJewelryStrengLvl > m_wJewelrySuitLvl){
            break;
        }

        if (CJewelry::IsAllHeroAttr(stAttr.eAttrType)){
            mapAttr[stAttr.eAttrType] += stAttr.dwAttrVal;
        }
    }
}

BOOL CHero::ReCheckJewelrySuit()
{
    m_wJewelrySuitDressLvl = 0;
    m_wJewelrySuitLvl = 0;

    // 是否构成一套饰品套装（首先饰品套装要求：身上饰品数 = 4）
    BOOL bIsSuit = (m_mapJewelryPos.Size() == EEPT_FOOT);
    if (!bIsSuit){
        return FALSE;
    }

    CJewelry* poJewelry = m_mapJewelry.GetFistData();
    UINT16 wMinStrengLvl = poJewelry->GetStrengLevel();
    UINT16 wSuitDressLvl = poJewelry->GetDressLevel();

    // 先找出4个饰品的最低强化等级
    for(poJewelry = m_mapJewelry.GetNextData(); poJewelry; poJewelry = m_mapJewelry.GetNextData()){
        SJewelryProp *pJewelryProp = poJewelry->GetProp();
        if (NULL == pJewelryProp){
            return FALSE;
        }

        // 饰品套装要求橙色品质
        if (pJewelryProp->byColorType < EC_ORANGE){
            bIsSuit = FALSE;
            break;
        }

        // 饰品套装要求4个饰品的穿戴等级一样
        if(poJewelry->GetDressLevel() != wSuitDressLvl){
            bIsSuit = FALSE;
            break;
        }

        wMinStrengLvl = MIN(poJewelry->GetStrengLevel(), wMinStrengLvl);
    }

    if(!bIsSuit){
        return FALSE;
    }

    SJewelrySuitProp *pstJewelrySuitProp = CJewelryPropMgr::Instance()->GetJewelrySuitProp(wSuitDressLvl, wMinStrengLvl);
    if (NULL == pstJewelrySuitProp){
        return FALSE;
    }

    // 再根据4个饰品的强化等级计算出对应级别的套装等级
    UINT16 wSuitLvl = CJewelryPropMgr::Instance()->GetJewelrySuitLvl(*pstJewelrySuitProp, wMinStrengLvl);
    for(poJewelry = m_mapJewelry.GetFistData(); poJewelry; poJewelry = m_mapJewelry.GetNextData()){
        if (poJewelry->GetStrengLevel() == wMinStrengLvl){
            continue;
        }

        UINT16 wJewelrySuitLvl = CJewelryPropMgr::Instance()->GetJewelrySuitLvl(*pstJewelrySuitProp, poJewelry->GetStrengLevel());
        if (wJewelrySuitLvl != wSuitLvl){
            bIsSuit = FALSE;
            break;
        }
    }

    if(!bIsSuit){
        return FALSE;
    }

    m_wJewelrySuitDressLvl = wSuitDressLvl;
    m_wJewelrySuitLvl = wSuitLvl;
    return TRUE;
}

BOOL CHero::IsDressJewelrySuit()
{
    return m_wJewelrySuitDressLvl > 0;
}

DT_BATTLE_BASE_ATTRIBUTE CHero::BattleAttrCountByLevelTalent()
{
    DT_BATTLE_BASE_ATTRIBUTE stDT_BATTLE_ATTRIBUTE;
    memset(&stDT_BATTLE_ATTRIBUTE, 0, sizeof(stDT_BATTLE_ATTRIBUTE));

    UINT16 wTalent = GetTalent();

    SHeroProp* pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(m_stDT_HERO_BASE_DATA.wKindID);
    if(NULL == pstHeroProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstHeroProp[KindID:%d] is NULL!"), MSG_MARK, m_stDT_HERO_BASE_DATA.wKindID);
        return stDT_BATTLE_ATTRIBUTE;
    }

    stDT_BATTLE_ATTRIBUTE.dwAttack = UINT32( CUpdateAttrPropMgr::Instance()->GetAttrAddValue(
            wTalent, m_stDT_HERO_BASE_DATA.wLevel, EAST_ATTACK, pstHeroProp->byCareerID ) );
    stDT_BATTLE_ATTRIBUTE.qwHP = UINT32( CUpdateAttrPropMgr::Instance()->GetAttrAddValue(
                                            wTalent, m_stDT_HERO_BASE_DATA.wLevel, EAST_HP, pstHeroProp->byCareerID ) );
	stDT_BATTLE_ATTRIBUTE.dwDefend = UINT32( CUpdateAttrPropMgr::Instance()->GetAttrAddValue(
		wTalent, m_stDT_HERO_BASE_DATA.wLevel, EAST_DEFEND, pstHeroProp->byCareerID, FALSE ) );

    if (IsFly()){
        const SFlyProp &stFlyProp = CFlyPropMgr::Instance()->GetFlyProp();

        // 飞升提升属性
        stDT_BATTLE_ATTRIBUTE.dwAttack = (UINT32)(stDT_BATTLE_ATTRIBUTE.dwAttack * stFlyProp.fFlyAttrPromoteCoef);
        stDT_BATTLE_ATTRIBUTE.qwHP     = (UINT32)(stDT_BATTLE_ATTRIBUTE.qwHP * stFlyProp.fFlyAttrPromoteCoef);
        stDT_BATTLE_ATTRIBUTE.dwDefend = (UINT32)(stDT_BATTLE_ATTRIBUTE.dwDefend * stFlyProp.fFlyAttrPromoteCoef);
    }

    return stDT_BATTLE_ATTRIBUTE;
}

VOID CHero::XiangqianAttrCount( map<UINT32, UINT32> &mapCurAttr )
{
    CEquip* poEquip = m_mapEquip.GetFistData();

    map<UINT32, UINT32> mapPercentage;
    while(poEquip)
    {


        DT_EQUIP_DATA &equipData = poEquip->GetDT_EQUIP_DATA();

        SEquipProp *pEquipProp = poEquip->GetEquipProp();
        if( NULL == pEquipProp )
        {
            continue;
        }
        UINT32 dwNum = CXiangqianMgr::Instance()->GetUnlockNum( pEquipProp->wDressLevel );

        for( UINT32 dwIndex = 0; dwIndex < dwNum; ++dwIndex )
        {
            UINT16 wItemID = equipData.astXiangqianList[dwIndex].wItemID;
            if( wItemID == 0 )
            {
                continue;
            }

            UINT32 dwAddValue = 0;
            UINT32 dwAddPercentValue = 0;
            UINT32 dwType = 0;
            if( !CXiangqianMgr::Instance()->GetAddValue( wItemID, dwAddValue, dwAddPercentValue, dwType ) )
            {
                SYS_CRITICAL( _SDT( "[%s %d]can't get item, by ID:%d !" ), MSG_MARK, wItemID );
                continue;
            }

            if( dwAddValue )
            {
                mapCurAttr[dwType] = mapCurAttr[dwType] + dwAddValue ;
		
            }
            if( dwAddPercentValue )
            {
                mapPercentage[dwType] = mapPercentage[dwType] + dwAddPercentValue ;
            }
        }
        poEquip = m_mapEquip.GetNextData();
    }

    //统一计算百分比

    map<UINT32, UINT32>::iterator itr;
    for( itr = mapPercentage.begin(); itr != mapPercentage.end(); ++itr )
    {
        mapCurAttr[itr->first] = ( 100 + itr->second ) * mapCurAttr[itr->first] / 100;
    }
}

VOID CHero::XiangqianDecAttrCount( map<UINT32, UINT32> &mapCurAttr )
{
    CEquip* poEquip = m_mapEquip.GetFistData();

    map<UINT32, UINT32> mapAttrPercentage;
    while(poEquip)
    {
        DT_EQUIP_DATA &equipData = poEquip->GetDT_EQUIP_DATA();
        SEquipProp *pEquipProp = poEquip->GetEquipProp();
        if( NULL == pEquipProp )
        {
            continue;
        }
        UINT32 dwNum = CXiangqianMgr::Instance()->GetUnlockNum( pEquipProp->wDressLevel );

        map<UINT32, UINT32> mapAttrAddValue;
        for( UINT32 dwIndex = 0; dwIndex < dwNum; ++dwIndex )
        {
            UINT16 wItemID = equipData.astXiangqianList[dwIndex].wItemID;
            if( wItemID == 0 )
            {
                continue;
            }

            UINT32 dwAddValue = 0;
            UINT32 dwAddPercentValue = 0;
            UINT32 dwType = 0;
            if( !CXiangqianMgr::Instance()->GetAddValue( wItemID, dwAddValue, dwAddPercentValue, dwType ) )
            {
                SYS_CRITICAL( _SDT( "[%s %d]can't get item, by ID:%d !" ), MSG_MARK, wItemID );
                continue;
            }

            //计算增加的百分比和数值
            mapAttrPercentage[dwType] += dwAddPercentValue;
            mapAttrAddValue[dwType] += dwAddValue;
        }

        for( UINT32 dwIndex = 0; dwIndex < dwNum; ++dwIndex )
        {
            UINT16 wItemID = equipData.astXiangqianList[dwIndex].wItemID;
            if( wItemID == 0 )
            {
                continue;
            }

            UINT32 dwAddValue = 0;
            UINT32 dwAddPercentValue = 0;
            UINT32 dwType = 0;
            if( !CXiangqianMgr::Instance()->GetAddValue( wItemID, dwAddValue, dwAddPercentValue, dwType ) )
            {
                SYS_CRITICAL( _SDT( "[%s %d]can't get item, by ID:%d !" ), MSG_MARK, wItemID );
                continue;
            }

            //mapCurAttr[dwType]=mapCurAttr[dwType]-dwAddValue-mapCurAttr[dwType]*100/(dwAddPercentValue+100);

            //按照逆序，先扣除百分比，再扣除具体值
            mapCurAttr[dwType] -= mapAttrAddValue[dwType];
        }

        poEquip = m_mapEquip.GetNextData();
    }

    map<UINT32, UINT32>::iterator itr;
    for( itr = mapAttrPercentage.begin(); itr != mapAttrPercentage.end(); ++itr )
    {
        mapCurAttr[itr->first] = mapCurAttr[itr->first] * 100 / (100 + mapAttrPercentage[itr->first]);
    }
}

DT_BATTLE_BASE_ATTRIBUTE CHero::BattleAttrCountByEquip()
{
    DT_BATTLE_BASE_ATTRIBUTE stDT_BATTLE_ATTRIBUTE_ALL;
    memset(&stDT_BATTLE_ATTRIBUTE_ALL, 0 , sizeof(stDT_BATTLE_ATTRIBUTE_ALL));

    CEquip* poEquip = m_mapEquip.GetFistData();

    CEquipMaster *poEquipMaster = m_poOwner->GetEquipMaster();
    while(poEquip)
    {
        const DT_BATTLE_ATTRIBUTE2 &stDT_BATTLE_ATTRIBUTE = poEquip->GetBattleAttr();
        stDT_BATTLE_ATTRIBUTE_ALL.qwHP += stDT_BATTLE_ATTRIBUTE.dwHP;
        stDT_BATTLE_ATTRIBUTE_ALL.dwAttack += stDT_BATTLE_ATTRIBUTE.dwAttack;
        stDT_BATTLE_ATTRIBUTE_ALL.dwDefend += stDT_BATTLE_ATTRIBUTE.dwDefend;

        poEquip = m_mapEquip.GetNextData();
    }

    return stDT_BATTLE_ATTRIBUTE_ALL;
}

VOID CHero::Upgrade()
{
    if(m_stDT_HERO_BASE_DATA.wLevel > CSinglePramMgr::Instance()->GetMaxPlayerLevel())
        return;

    SHeroUpgradeProp * pstHeroUpgradeProp = CHeroPropMgr::Instance()->GetHeroUpgradeProp(m_stDT_HERO_BASE_DATA.wLevel + 1); //肯定不为NULL
    if (NULL == pstHeroUpgradeProp)
    {
        return;
    }
    m_stDT_HERO_BASE_DATA.wLevel++;

    InitBattleAttribute(); //重新初始化属性

    SetUpgradeExperience();

    //主将升级回调
    if(1 == m_stDT_HERO_BASE_DATA.byCoach)
    {
        m_poOwner->OnUpgrade(m_stDT_HERO_BASE_DATA.wLevel);

        m_poOwner->CkToday();
        m_poOwner->GetDT_PLAYER_TODAY_CONSUME_DATA().wAddLevel++;
        m_poOwner->GetDT_PLAYER_TODAY_CONSUME_DATA().wAfterLevel = m_stDT_HERO_BASE_DATA.wLevel;

		CPlayerMedal* poPlayerMedal = m_poOwner->GetPlayerMedal();
		if ( NULL != poPlayerMedal)
		{
			poPlayerMedal->CheckMedalBySmallType( EMST_PLAYER_LEVEL );
		}
    }


    //m_stDT_PLAYER_TODAY_CONSUME_DATA.wAddLevel += (GetLevel() - m_stDT_PLAYER_TODAY_CONSUME_DATA.byAfterVipLevel);
    //m_stDT_PLAYER_TODAY_CONSUME_DATA.wAfterLevel = GetLevel();

    //CTaskMgr::Instance()->OnHeroUpgrade(m_poOwner);
}

VOID CHero::SetLevel(UINT16 wLevel)//升级
{
    if(wLevel > CSinglePramMgr::Instance()->GetMaxPlayerLevel())
        return;

    //首先设置用户等级的经验
    SHeroUpgradeProp * pstHeroUpgradeProp = CHeroPropMgr::Instance()->GetHeroUpgradeProp(wLevel); //肯定不为NULL
    if (NULL == pstHeroUpgradeProp)
    {
        return;
    }
    m_stDT_HERO_BASE_DATA.wLevel = wLevel;
    m_stDT_HERO_BASE_DATA.qwExperience = pstHeroUpgradeProp->qwExperience;

    //保存玩家当前下一级的相关数据，用于重新计算数据使用
    //pHeroUpgradeProp = pstHeroUpgradeProp;
    //pstHeroUpgradeProp = CHeroPropMgr::Instance()->GetHeroUpgradeProp(wLevel + 1); //肯定不为NULL
    //if (NULL == pstHeroUpgradeProp)
    //{
    //	pstHeroUpgradeProp = pHeroUpgradeProp;
    //}

    SetUpgradeExperience();

    InitBattleAttribute(); //重新初始化属性

    //主将升级回调
    if(1 == m_stDT_HERO_BASE_DATA.byCoach)
    {
        m_poOwner->OnUpgrade(m_stDT_HERO_BASE_DATA.wLevel);
    }
}


VOID CHero::SetUpgradeExperience()
{
    //查找是否存在下一级的配置
    SHeroUpgradeProp* pstHeroUpgradeProp = CHeroPropMgr::Instance()->GetHeroUpgradeProp(m_stDT_HERO_BASE_DATA.wLevel + 1);
    if(NULL == pstHeroUpgradeProp)
    {
        //未配置下一等级，赋值UNDEFINE_UPGRADE_EXPERIENCE
        m_qwUpgradeExperience = UNDEFINE_UPGRADE_EXPERIENCE;
    }
    //存在，则更新属性
    else
    {
        m_qwUpgradeExperience = pstHeroUpgradeProp->qwExperience;
    }
    //m_stEncourageExp.dwUpgradeExperience = m_dwUpgradeExperience;
}

UINT16 CHero::OnDressEquip(UINT16 wDressItemKindID, UINT16 wDressItemIdx)
{
    if (NULL == m_poOwner)
    {
        RETURN_OTHER_ERR;
    }

    CEquip* poEquip = m_poOwner->GetEquip(wDressItemKindID, wDressItemIdx);
    if(NULL == poEquip)
    {
        return ERR_EQUIP::ID_EQUIP_NOT_EXIST_ERR;
    }

    SEquipProp* poEquipProp = poEquip->GetEquipProp();
    if (NULL == poEquipProp)
    {
        return ERR_EQUIP::ID_EQUIP_NOT_EXIST_ERR;
    }

    if(m_stDT_HERO_BASE_DATA.wLevel < poEquipProp->wDressLevel)
    {
        return ERR_EQUIP::ID_NOT_REACH_LEVEL_ERR;
    }

    if((NULL != poEquip->GetHero()) || (NULL != m_mapEquip.GetData(poEquip->GetMemID())))
    {
        return ERR_EQUIP::ID_SUCCESS;
    }

    UINT8 byEquipPos = poEquipProp->byPosKindID;


    //装备背包处理
    CBag& oBag = m_poOwner->GetBag();
    //该位置已有装备
    CEquip* poUndressEquip = NULL;
    poUndressEquip = m_mapEquipPos.GetData(byEquipPos);
    if(NULL != poUndressEquip)
    {
        if((NULL == poUndressEquip->GetHero()) || (NULL == m_mapEquip.GetData(poUndressEquip->GetMemID())))
        {
            return ERR_EQUIP::ID_NOT_EQUIP_ERR;
        }

        poUndressEquip->SetHero(NULL);
        m_mapEquipPos.DelData(byEquipPos);
        m_mapEquip.DelData(poUndressEquip->GetMemID());
    }

    //装备背包处理, 从背包中把装备删除
    if(!oBag.DelItem(poEquip, FALSE ))
    {
        //如果失败，应该再把装备放回背包中。
        if(poUndressEquip)
        {
            m_mapEquip.AddData(poUndressEquip->GetMemID(), poUndressEquip);
            m_mapEquipPos.AddData(byEquipPos, poUndressEquip);
            poUndressEquip->SetHero(this);
        }
        SYS_CRITICAL(_SDT("[%s: %d]: bag DelItem failed!"), MSG_MARK);
        return ERR_EQUIP::ID_BAG_FULL_ERR;
    }

    //装备背包处理, 把卸下的装备放背包中
    if (NULL != poUndressEquip)
    {
        if(!oBag.AddItem(poUndressEquip, FALSE))
        {
            m_mapEquip.AddData(poUndressEquip->GetMemID(), poUndressEquip);
            m_mapEquipPos.AddData(byEquipPos, poUndressEquip);
            poUndressEquip->SetHero(this);

            SYS_CRITICAL(_SDT("[%s: %d]: bag DelItem failed!"), MSG_MARK);
            return ERR_EQUIP::ID_BAG_FULL_ERR;
        }
    }

    m_mapEquip.AddData(poEquip->GetMemID(), poEquip);
    m_mapEquipPos.AddData(byEquipPos, poEquip);
    poEquip->SetHero(this);

    InitBattleAttribute();

    CTaskMgr::Instance()->OnDressEquip(m_poOwner, wDressItemKindID);
    return ERR_EQUIP::ID_SUCCESS;

}

UINT16 CHero::OnUndressEquip(UINT16 wUndressItemKindID, UINT16 wUndressItemIdx)
{
    if (NULL == m_poOwner)
    {
        RETURN_OTHER_ERR;
    }

    CEquip* poEquip = m_poOwner->GetEquip(wUndressItemKindID, wUndressItemIdx);
    if(NULL == poEquip)
    {
        return ERR_EQUIP::ID_EQUIP_NOT_EXIST_ERR;
    }

    if((NULL == poEquip->GetHero()) || (NULL == m_mapEquip.GetData(poEquip->GetMemID())))
    {
        return ERR_EQUIP::ID_NOT_EQUIP_ERR;
    }

    //装备背包处理
    CBag& oBag = m_poOwner->GetBag();
    //判断背包是否空闲
    if(0 == oBag.GetIdleSize())
    {
        return ERR_EQUIP::ID_BAG_FULL_ERR;
    }
    if(!oBag.AddItem(poEquip, FALSE))
    {
        RETURN_OTHER_ERR;
    }
    SEquipProp* poEquipProp = poEquip->GetEquipProp();
    if (NULL == poEquipProp)
    {
        RETURN_OTHER_ERR;
    }
    //卸下
    m_mapEquipPos.DelData(poEquipProp->byPosKindID);
    m_mapEquip.DelData(poEquip->GetMemID());
    poEquip->SetHero(NULL);
    InitBattleAttribute();

    //SetSaveState(ESS_WANT_SAVE);
    //poEquip->SetSaveState(ESS_WANT_SAVE);
    //oBag.SetSaveState(ESS_WANT_SAVE);

    return ERR_EQUIP::ID_SUCCESS;
}

UINT16 CHero:: OnReplaceEquip(UINT16 wDressItemKindID, UINT16 wDressItemIdx, UINT16 wUndressItemKindID, UINT16 wUndressItemIdx)
{
    if (NULL == m_poOwner)
    {
        RETURN_OTHER_ERR;
    }

    CEquip* poDressEquip = m_poOwner->GetEquip(wDressItemKindID, wDressItemIdx);
    if(NULL == poDressEquip)
    {
        return ERR_EQUIP::ID_EQUIP_NOT_EXIST_ERR;
    }

    SEquipProp* poEquipProp = poDressEquip->GetEquipProp();
    if (NULL == poEquipProp)
    {
        return ERR_EQUIP::ID_EQUIP_NOT_EXIST_ERR;
    }

    if(m_stDT_HERO_BASE_DATA.wLevel < poEquipProp->wDressLevel)
    {
        return ERR_EQUIP::ID_NOT_REACH_LEVEL_ERR;
    }

    if((NULL != poDressEquip->GetHero()) || (NULL != m_mapEquip.GetData(poDressEquip->GetMemID())))
    {
        return ERR_EQUIP::ID_HAVE_EQUIP_ERR;
    }

    CEquip* poUndressEquip = m_poOwner->GetEquip(wUndressItemKindID, wUndressItemIdx);
    if(NULL == poUndressEquip)
    {
        return ERR_EQUIP::ID_EQUIP_NOT_EXIST_ERR;
    }

    if((NULL == poUndressEquip->GetHero()) || (NULL == m_mapEquip.GetData(poUndressEquip->GetMemID())))
    {
        return ERR_EQUIP::ID_NOT_EQUIP_ERR;
    }

    SEquipProp* poDressEquipProp = poDressEquip->GetEquipProp();
    if (NULL == poDressEquipProp)
    {
        return ERR_EQUIP::ID_NOT_EQUIP_ERR;
    }

    SEquipProp* poUndressEquipProp = poUndressEquip->GetEquipProp();
    if (NULL == poUndressEquipProp)
    {
        return ERR_EQUIP::ID_NOT_EQUIP_ERR;
    }

    UINT8 byDressPosKindID = poDressEquip->GetEquipProp()->byPosKindID;
    if(poUndressEquipProp->byPosKindID != byDressPosKindID)
    {
        return ERR_EQUIP::ID_POS_NOT_MATCH_ERR;
    }

    //装备背包处理
    CBag& oBag = m_poOwner->GetBag();
    if(!oBag.DelItem(poDressEquip, FALSE))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: bag DelItem failed!"), MSG_MARK);
        RETURN_OTHER_ERR;
    }
    if(!oBag.AddItem(poUndressEquip, FALSE))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: bag AddItem failed!"), MSG_MARK);
        oBag.AddItem(poDressEquip, FALSE);
        RETURN_OTHER_ERR;
    }

    //替换
    m_mapEquip.AddData(poDressEquip->GetMemID(), poDressEquip);
    m_mapEquipPos.DelData(byDressPosKindID);
    m_mapEquipPos.AddData(byDressPosKindID, poDressEquip);
    poDressEquip->SetHero(this);
    m_mapEquip.DelData(poUndressEquip->GetMemID());
    poUndressEquip->SetHero(NULL);
    //重新计算战力
    InitBattleAttribute();

    //SetSaveState(ESS_WANT_SAVE);
    //poDressEquip->SetSaveState(ESS_WANT_SAVE);
    //poUndressEquip->SetSaveState(ESS_WANT_SAVE);
    //oBag.SetSaveState(ESS_WANT_SAVE);

    return ERR_EQUIP::ID_SUCCESS;
}

//获取武将的装备
VOID CHero::GetEquip(DT_HERO_EQUIP_DATA_LST_CLI& stEquipDataList)
{
    UINT16 wEquipNum = stEquipDataList.wEquipNum;
    CEquip* poEquip = m_mapEquip.GetFistData();
    while(poEquip)
    {
        DT_EQUIP_DATA_CLI* pstDT_EQUIP_DATA_CLI = &(stEquipDataList.astEquipList[wEquipNum]);
        poEquip->GetDT_EQUIP_DATA_CLI(*pstDT_EQUIP_DATA_CLI);
        wEquipNum++;
        poEquip = m_mapEquip.GetNextData();
    }
    stEquipDataList.wEquipNum = wEquipNum;
}

VOID CHero::GetEquip_v410(DT_HERO_EQUIP_DATA_LST_CLI_v410& stEquips)
{
    CEquip* poEquip = m_mapEquip.GetFistData();
    while(poEquip)
    {
        DT_EQUIP_DATA_CLI_v410 &stEquip = stEquips.astEquipList[stEquips.wEquipNum];
        poEquip->GetDT_EQUIP_DATA_CLI_v410(stEquip);
        stEquips.wEquipNum++;

        poEquip = m_mapEquip.GetNextData();
    }
}

VOID CHero::GetEquipStrengthenCompose(DT_EQUIP_STRENGTHEN_COMPOSE_DATA_LST_CLI& stEquipDataList)
{
    UINT16 wEquipNum = stEquipDataList.wEquipNum;
    CEquip* poEquip = m_mapEquip.GetFistData();
    while(poEquip)
    {
        DT_EQUIP_DATA_STRENGTHEN_CLI* pstDT_EQUIP_DATA_STRENGTHEN_CLI = &(stEquipDataList.astEquipStrengthenInfo[wEquipNum]);
        poEquip->GetDT_EQUIP_DATA_STRENGTHEN_CLI(*pstDT_EQUIP_DATA_STRENGTHEN_CLI);
        DT_EQUIP_DATA_CLI* pstDT_EQUIP_DATA_CLI = &(stEquipDataList.astEquipInfo[wEquipNum]);
        poEquip->GetDT_EQUIP_DATA_CLI(*pstDT_EQUIP_DATA_CLI);
        wEquipNum++;
        poEquip = m_mapEquip.GetNextData();
    }
    stEquipDataList.wEquipNum = wEquipNum;
}

VOID CHero::GetEquipsStrengthenEnchantInfo_v410(DT_EQUIP_STRENGTHEN_ENCHANT_DATA_LST_CLI& stEquipList)
{
    CEquip* poEquip = m_mapEquip.GetFistData();
    while(poEquip)
    {
        if (stEquipList.wEquipNum >= MAX_BAG_OPEN_NUM){
            break;
        }

        DT_EQUIP_DATA_CLI_v410* pstEquip = &(stEquipList.astEquipInfo[stEquipList.wEquipNum]);
        DT_EQUIP_DATA_STRENGTHEN_CLI* pstEquipStrengthen = &(stEquipList.astEquipStrengthenInfo[stEquipList.wEquipNum]);
        DT_EQUIP_ENCHANT_DATA_CLI* pstEquipEnchant = &(stEquipList.astEquipEnchantInfo[stEquipList.wEquipNum]);

        poEquip->GetDT_EQUIP_DATA_CLI_v410(*pstEquip);
        poEquip->GetDT_EQUIP_DATA_STRENGTHEN_CLI(*pstEquipStrengthen);
        poEquip->GetDT_EQUIP_DATA_ENCHANT_CLI(*pstEquipEnchant);
        
        stEquipList.wEquipNum++;
        poEquip = m_mapEquip.GetNextData();
    }

    if(m_poOwner->GetBuild(EBK_ENCHANT)){
        stEquipList.wEnchantEquipNum = stEquipList.wEquipNum;
    }
}

//获取装备的最大镶嵌宝石个数。
UINT8 CHero::GetMaxMosaicGemNum()
{
    UINT8	byMosaicGemNum	=	0;
    UINT8	byMaxMosaicGemNum =	0;
    CEquip*	poEquip = m_mapEquip.GetFistData();
    while(poEquip)
    {
        byMosaicGemNum = poEquip->GetMosaicGemNum();
        if (byMosaicGemNum > byMaxMosaicGemNum )
        {
            byMaxMosaicGemNum = byMosaicGemNum;
        }
        poEquip = m_mapEquip.GetNextData();
    }
    return byMaxMosaicGemNum;
}

VOID CHero::GetEnemyEquip(DT_ENEMY_EQUIP_DATA_LST& stEquipList)
{
    UINT16 wEquipNum = stEquipList.wEquipNum;
    CEquip* poEquip = m_mapEquip.GetFistData();
    while(poEquip)
    {
        DT_ENEMY_EQUIP_DATA& stDT_ENEMY_EQUIP_DATA = stEquipList.astEquipList[wEquipNum];
        poEquip->GetDT_ENEMY_EQUIP_DATA(stDT_ENEMY_EQUIP_DATA);
        wEquipNum++;
        poEquip = m_mapEquip.GetNextData();
    }
    stEquipList.wEquipNum = wEquipNum;
}

VOID CHero::GetExperienceInfo(DT_EXPERIENCE_DATA& stExperienceInfo)
{
    //经验下发差值
    GetExperience(stExperienceInfo.dwExperience, stExperienceInfo.dwUpgradeExperience);
}


VOID CHero::GetDT_HERO_BASE_DATA_EQUIP_CLI(DT_HERO_BASE_DATA_CLI& stDT_HERO_BASE_DATA_CLI, BOOL bGetEquipAttr)
{
    if(bGetEquipAttr)
    {
        GetDT_HERO_BASE_DATA_CLI(stDT_HERO_BASE_DATA_CLI);
    }
    else
    {
        map<UINT32, UINT32> mapCurAttr;
        mapCurAttr[EBAK_HP] = static_cast<UINT32>(m_stBattleAttribute.qwHP);
        mapCurAttr[EBAK_ATTACK] = m_stBattleAttribute.dwAttack;
        mapCurAttr[EBAK_HITRATE] = m_stHideBattleAttr.wHitRate;
        mapCurAttr[EBAK_DODGERATE] = m_stHideBattleAttr.wDodgeRate;
        mapCurAttr[EBAK_CRITRATE] = m_stHideBattleAttr.wCritRate;
        mapCurAttr[EBAK_DECRITRATE] = m_stHideBattleAttr.wDeCritRate;

        XiangqianDecAttrCount( mapCurAttr );

        GetDT_HERO_BASE_DATA_CLI(stDT_HERO_BASE_DATA_CLI);
        //减去装备计算装备拥有属性
        DT_BATTLE_BASE_ATTRIBUTE stEquipAttr = BattleAttrCountByEquip();

        stDT_HERO_BASE_DATA_CLI.stBattleAttribute.dwHP = mapCurAttr[EBAK_HP] - static_cast<UINT32>(stEquipAttr.qwHP);
        stDT_HERO_BASE_DATA_CLI.stBattleAttribute.dwAttack = mapCurAttr[EBAK_ATTACK] - stEquipAttr.dwAttack;

        stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute.wHitRate = mapCurAttr[EBAK_HITRATE];

        stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute.wDodgeRate = mapCurAttr[EBAK_DODGERATE];

        stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute.wCritRate = mapCurAttr[EBAK_CRITRATE];

        stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute.wDeCritRate = mapCurAttr[EBAK_DECRITRATE];
    }
    //stDT_HERO_BASE_DATA_CLI.dwUpgradeExperience = m_dwUpgradeExperience;
}


/*
VOID CHero::InitCliData(DT_BATTLE_ATTRIBUTE& stEquipAttr)
{
    m_stDT_HERO_BASE_DATA_CLI.wKindID = m_stDT_HERO_BASE_DATA.wKindID;
    m_stDT_HERO_BASE_DATA_CLI.byCoach = m_stDT_HERO_BASE_DATA.byCoach;
    m_stDT_HERO_BASE_DATA_CLI.byPos = m_stDT_HERO_BASE_DATA.byPos;
    m_stDT_HERO_BASE_DATA_CLI.byFormationIdx = m_stDT_HERO_BASE_DATA.byFormationIdx;
    m_stDT_HERO_BASE_DATA_CLI.wLevel = m_stDT_HERO_BASE_DATA.wLevel;
	m_stDT_HERO_BASE_DATA_CLI.byRecuitedFlag = m_stDT_HERO_BASE_DATA.byRecuitedFlag;
    m_stDT_HERO_BASE_DATA_CLI.wTalent = GetTalent(); //包括丹药的


    m_stDT_HERO_BASE_DATA_CLI.byCareerID = pstHeroProp->byCareerID;
    m_stDT_HERO_BASE_DATA_CLI.wSelectSkillID = m_stDT_HERO_BASE_DATA.wSelectSkillID;
    m_stDT_HERO_BASE_DATA_CLI.wSelectSkillLevel = m_stDT_HERO_BASE_DATA.wSelectSkillLevel;

//	m_stDT_HERO_BASE_DATA_CLI.byJobID = CHeroPropMgr::Instance()->GetHeroProp( m_stDT_HERO_BASE_DATA.wKindID )->byJobID;
    //经验下发差值
    m_stDT_HERO_BASE_DATA_CLI.dwExperience = m_stDT_HERO_BASE_DATA.dwExperience >= pstHeroUpgradeProp->dwExperience ?
            (m_stDT_HERO_BASE_DATA.dwExperience - pstHeroUpgradeProp->dwExperience) : 0;

    m_stDT_HERO_BASE_DATA_CLI.dwUpgradeExperience = m_dwUpgradeExperience >= pstHeroUpgradeProp->dwExperience ?
            (m_dwUpgradeExperience - pstHeroUpgradeProp->dwExperience) : 0xFFFFFFFF;

    m_stDT_HERO_BASE_DATA_CLI.stBattleAttribute = m_stBattleAttribute;

    //隐藏属性，主伙伴去掉初始值
    //if(1 == m_stDT_HERO_BASE_DATA.byCoach)
    //{
    //	DT_HIDE_BATTLE_ATTRIBUTE& stHideAttr = m_stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute;
    //	stHideAttr.wHitRate =  m_stHideBattleAttr.wHitRate - pstHeroProp->wHitRate;
    //	stHideAttr.wDodgeRate =  m_stHideBattleAttr.wDodgeRate - pstHeroProp->wDodgeRate;
    //	stHideAttr.wCritRate =  m_stHideBattleAttr.wCritRate - pstHeroProp->wCritRate;
    //	stHideAttr.wDeCritRate =  m_stHideBattleAttr.wDeCritRate - pstHeroProp->wDeCritRate;
    //	stHideAttr.wAngryValue =  m_stHideBattleAttr.wAngryValue - pstHeroProp->wAngryValue;
    //}
    //else
    //{
    //      m_stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute = m_stHideBattleAttr;
    //}
    m_stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute = m_stHideBattleAttr;

    if(m_poGodWeapon)
    {
        m_stDT_HERO_BASE_DATA_CLI.wGodweaponQuality = m_poGodWeapon->GetDT_GODWEAPON_DATA().wQuality;
        m_stDT_HERO_BASE_DATA_CLI.wGodweaponLevel = m_poGodWeapon->GetDT_GODWEAPON_DATA().wLevel;
    }
	else
	{
		m_stDT_HERO_BASE_DATA_CLI.wGodweaponQuality = 0;
		m_stDT_HERO_BASE_DATA_CLI.wGodweaponLevel = 0;
	}

    //战力
    ResetPower();

    //m_stDT_HERO_BASE_DATA_CLI_NoEquip = m_stDT_HERO_BASE_DATA_CLI;
    memcpy(&m_stDT_HERO_BASE_DATA_CLI_NoEquip, &m_stDT_HERO_BASE_DATA_CLI, sizeof(m_stDT_HERO_BASE_DATA_CLI_NoEquip));
    //减去装备计算装备拥有属性
    m_stDT_HERO_BASE_DATA_CLI_NoEquip.stBattleAttribute.dwHP = m_stBattleAttribute.dwHP - stEquipAttr.dwHP;
    m_stDT_HERO_BASE_DATA_CLI_NoEquip.stBattleAttribute.dwAttack = m_stBattleAttribute.dwAttack - stEquipAttr.dwAttack;

    m_stDT_HERO_EMBATLE_DATA.wKindID = m_stDT_HERO_BASE_DATA.wKindID;
    m_stDT_HERO_EMBATLE_DATA.byCareerID = pstHeroProp->byCareerID;
    m_stDT_HERO_EMBATLE_DATA.wLevel = m_stDT_HERO_BASE_DATA.wLevel;
    m_stDT_HERO_EMBATLE_DATA.wTalent = m_stDT_HERO_BASE_DATA_CLI.wTalent;
    m_stDT_HERO_EMBATLE_DATA.stBattleAttribute = m_stDT_HERO_BASE_DATA_CLI.stBattleAttribute;
    m_stDT_HERO_EMBATLE_DATA.dwPower = m_stDT_HERO_BASE_DATA_CLI.dwPower;
    m_stDT_HERO_EMBATLE_DATA.bySkillNum = 0;
	CSkill* poSKill = m_mapAllSkill.GetFistData();
    do
    {
		if(NULL == poSKill)
		{
			break;
		}
		poSKill->GetDT_SKILL_DATA_CLI(m_stDT_HERO_EMBATLE_DATA.astSkillInfo[m_stDT_HERO_EMBATLE_DATA.bySkillNum++]);
    }while(poSKill = m_mapAllSkill.GetNextData());
}
*/

VOID CHero::GetDT_HERO_BASE_DATA_CLI(DT_HERO_BASE_DATA_CLI& stDT_HERO_BASE_DATA_CLI)
{
    SHeroProp* pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(m_stDT_HERO_BASE_DATA.wKindID);
    if(NULL == pstHeroProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstHeroProp[KindID:%d] is NULL!"), MSG_MARK, m_stDT_HERO_BASE_DATA.wKindID);
        return;
    }

    stDT_HERO_BASE_DATA_CLI.wKindID = m_stDT_HERO_BASE_DATA.wKindID;
    stDT_HERO_BASE_DATA_CLI.byCoach = m_stDT_HERO_BASE_DATA.byCoach;
    stDT_HERO_BASE_DATA_CLI.byPos = m_stDT_HERO_BASE_DATA.byPos;
    stDT_HERO_BASE_DATA_CLI.byFormationIdx = m_stDT_HERO_BASE_DATA.byFormationIdx;
    stDT_HERO_BASE_DATA_CLI.wLevel = m_stDT_HERO_BASE_DATA.wLevel;
    stDT_HERO_BASE_DATA_CLI.byRecuitedFlag = m_stDT_HERO_BASE_DATA.byRecuitedFlag;
    stDT_HERO_BASE_DATA_CLI.wTalent = GetTalent(); //包括丹药的

    stDT_HERO_BASE_DATA_CLI.byCareerID = pstHeroProp->byCareerID;

    //	m_stDT_HERO_BASE_DATA_CLI.byJobID = CHeroPropMgr::Instance()->GetHeroProp( m_stDT_HERO_BASE_DATA.wKindID )->byJobID;
    //经验下发差值
    GetExperience(stDT_HERO_BASE_DATA_CLI.dwExperience, stDT_HERO_BASE_DATA_CLI.dwUpgradeExperience);

    stDT_HERO_BASE_DATA_CLI.stBattleAttribute = CBattleMgr::Instance()->NewToOldBattleAttribute(m_stBattleAttribute);

    //隐藏属性，主伙伴去掉初始值
    //if(1 == m_stDT_HERO_BASE_DATA.byCoach)
    //{
    //	DT_HIDE_BATTLE_ATTRIBUTE& stHideAttr = m_stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute;
    //	stHideAttr.wHitRate =  m_stHideBattleAttr.wHitRate - pstHeroProp->wHitRate;
    //	stHideAttr.wDodgeRate =  m_stHideBattleAttr.wDodgeRate - pstHeroProp->wDodgeRate;
    //	stHideAttr.wCritRate =  m_stHideBattleAttr.wCritRate - pstHeroProp->wCritRate;
    //	stHideAttr.wDeCritRate =  m_stHideBattleAttr.wDeCritRate - pstHeroProp->wDeCritRate;
    //	stHideAttr.wAngryValue =  m_stHideBattleAttr.wAngryValue - pstHeroProp->wAngryValue;
    //}
    //else
    //{
    //      m_stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute = m_stHideBattleAttr;
    //}
    stDT_HERO_BASE_DATA_CLI.stHideBattleAttribute = m_stHideBattleAttr;

    if(m_poGodWeapon)
    {
        stDT_HERO_BASE_DATA_CLI.wGodweaponQuality = m_poGodWeapon->GetDT_GODWEAPON_DATA().wQuality;
        stDT_HERO_BASE_DATA_CLI.wGodweaponLevel = m_poGodWeapon->GetDT_GODWEAPON_DATA().wLevel;
    }
    else
    {
        stDT_HERO_BASE_DATA_CLI.wGodweaponQuality = 0;
        stDT_HERO_BASE_DATA_CLI.wGodweaponLevel = 0;
    }

    stDT_HERO_BASE_DATA_CLI.dwPower = GetPower();

    //当前吸收次数
    stDT_HERO_BASE_DATA_CLI.byAbsorbNum = m_stAbsorbDataLst.byAbsorbNum;
    for (UINT32 i = 0; i < m_stAbsorbDataLst.byAbsorbNum; i++ )
    {
        stDT_HERO_BASE_DATA_CLI.awKindIDLst[i] = m_stAbsorbDataLst.awKindIDLst[i];
    }

    stDT_HERO_BASE_DATA_CLI.byMaxAbsorbNum = (UINT8)pstHeroProp->wMaxAbsorbNum;
    stDT_HERO_BASE_DATA_CLI.wAbsorbKindID = m_stAbsorbDataLst.wAbsorbKindID;
    //
    //主角有颜色
    if (m_poOwner->GetCoachHeroKindID() == GetHeroID())
    {
        UINT16 wLevel = 0;
        if (NULL != m_poOwner->GetJingJie())
        {
            wLevel = m_poOwner->GetJingJie()->GetDT_JINGJIE_DATA().stBaseData.wLevel;
        }
        stDT_HERO_BASE_DATA_CLI.byColor = (UINT8)CJingJiePropMgr::Instance()->GetJingJieColor(wLevel);
    }
    else
    {
        stDT_HERO_BASE_DATA_CLI.byColor = 0;
    }
}

VOID CHero::GetDT_HERO_EMBATLE_DATA(DT_HERO_EMBATLE_DATA& stDT_HERO_EMBATLE_DATA)
{
    SHeroProp* pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(m_stDT_HERO_BASE_DATA.wKindID);
    if(NULL == pstHeroProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstHeroProp[KindID:%d] is NULL!"), MSG_MARK, m_stDT_HERO_BASE_DATA.wKindID);
        return;
    }

    stDT_HERO_EMBATLE_DATA.wKindID = m_stDT_HERO_BASE_DATA.wKindID;
    stDT_HERO_EMBATLE_DATA.byCareerID = pstHeroProp->byCareerID;
    stDT_HERO_EMBATLE_DATA.wLevel = m_stDT_HERO_BASE_DATA.wLevel;
    stDT_HERO_EMBATLE_DATA.wTalent = GetTalent();
    stDT_HERO_EMBATLE_DATA.stBattleAttribute = CBattleMgr::Instance()->NewToOldBattleAttribute(m_stBattleAttribute);
    stDT_HERO_EMBATLE_DATA.dwPower = GetPower();
    GetHeroSkillCli(stDT_HERO_EMBATLE_DATA.stSkillInfo);
}

VOID CHero::GetDT_NEW_HERO_EMBATLE_DATA(DT_NEW_HERO_EMBATLE_DATA& stDT_HERO_EMBATLE_DATA)
{
    SHeroProp* pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(m_stDT_HERO_BASE_DATA.wKindID);
    if(NULL == pstHeroProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstHeroProp[KindID:%d] is NULL!"), MSG_MARK, m_stDT_HERO_BASE_DATA.wKindID);
        return;
    }

    stDT_HERO_EMBATLE_DATA.wKindID = m_stDT_HERO_BASE_DATA.wKindID;
    stDT_HERO_EMBATLE_DATA.byCareerID = pstHeroProp->byCareerID;
    stDT_HERO_EMBATLE_DATA.wLevel = m_stDT_HERO_BASE_DATA.wLevel;
    stDT_HERO_EMBATLE_DATA.wTalent = GetTalent();
    CFormation& oFormation = m_poOwner->GetFormation();
	//遍历阵型
    CMapNewFormationInfo& mapFormation = oFormation.GetUnLockFormationInfoMap();
    stDT_HERO_EMBATLE_DATA.byFormationNum = mapFormation.size();
	UINT8 byIndex = 0;
	SetOpenFormationStatu( TRUE );
	for ( CMapNewFormationInfoItr itr = mapFormation.begin(); itr != mapFormation.end(); itr++, byIndex++)
	{
		GetPowerByFormation(itr->second.byFormationID, itr->second.byFormationLevel, 
			stDT_HERO_EMBATLE_DATA.adwPower[byIndex], stDT_HERO_EMBATLE_DATA.astBattleAttribute[byIndex]);
	}
	SetOpenFormationStatu( FALSE );
	InitBattleAttribute();//重新计算
    GetHeroSkillCli(stDT_HERO_EMBATLE_DATA.stSkillInfo);
}

VOID CHero::GetExperience(UINT32& dwExperience, UINT32& dwUpgradeExperience)
{
    SHeroUpgradeProp* pstHeroUpgradeProp = CHeroPropMgr::Instance()->GetHeroUpgradeProp(m_stDT_HERO_BASE_DATA.wLevel); //肯定不为NULL
    if(NULL == pstHeroUpgradeProp)
    {
        dwExperience = (UINT32)m_stDT_HERO_BASE_DATA.qwExperience;
        dwUpgradeExperience = 0xFFFFFFFF;
        //SYS_CRITICAL(_SDT("[%s: %d]: pstHeroUpgradeProp[level:%d] is NULL!"), MSG_MARK, m_stDT_HERO_BASE_DATA.wLevel + 1);
        return;
    }

    //经验下发差值
    dwExperience = (UINT32)(m_stDT_HERO_BASE_DATA.qwExperience >= pstHeroUpgradeProp->qwExperience ?
                   (m_stDT_HERO_BASE_DATA.qwExperience - pstHeroUpgradeProp->qwExperience) : 0);

    dwExperience += GetFlyExp();

    dwUpgradeExperience = (UINT32)(m_qwUpgradeExperience >= pstHeroUpgradeProp->qwExperience ?
                          (m_qwUpgradeExperience - pstHeroUpgradeProp->qwExperience) : 0xFFFFFFFF);
}

UINT32 CHero::GetPower()
{
    //个人总战力=攻击气血影响战力+隐藏属性影响战力+神器影响战力+技能影响战力+新增伤害
    return GetHPAttackPower() + GetHidePower() + GetGodweaponPower() + GetSkillPower() + GetHurtPower();
}

UINT32 CHero::GetHurtPower()
{
	return (m_mapJewelryAttr[EJEA_NORMAL_ATTACK] * 5) + (m_mapJewelryAttr[EJEA_SKILL_ATTACK] * 5);
}

UINT32 CHero::GetHPAttackPower()
{
	//攻击气血影响战力=	攻击×（1+增伤比例）×5+生命/（1-减伤比例）+防御×3
    UINT32 dwTmp = 100 <= m_stExtBattleAttr.wDecHurtRate ? 1 : 100 - m_stExtBattleAttr.wDecHurtRate;
    return (UINT32)((UINT64)(m_stBattleAttribute.dwAttack) * (100 + m_stExtBattleAttr.wIncHurtRate) / 20 + ((UINT64)(m_stBattleAttribute.qwHP) * 100 / dwTmp) + m_stBattleAttribute.dwDefend * 3);
}

UINT32 CHero::GetHidePower()
{
    SHeroProp* pstProp = GetHeroProp();
    if(NULL == pstProp)
    {
        return 0;
    }

    return ((m_stHideBattleAttr.wHitRate + m_stHideBattleAttr.wDodgeRate + m_stHideBattleAttr.wCritRate + m_stHideBattleAttr.wDeCritRate) -
            (pstProp->wHitRate + pstProp->wDodgeRate + pstProp->wCritRate + pstProp->wDeCritRate)) * 50;
}


UINT32 CHero::GetGodweaponPower()
{
    //神器影响战力=神器灵力×5+ 神器耐久/2
    if(NULL == m_poGodWeapon)
    {
        return 0;
    }

    //神器战力修改  神力 * 2 + 防御 * 3
    return (m_poGodWeapon->GetDT_BATTLE_ATTRIBUTE().dwAttack ) * 2 ;
}

UINT32 CHero::GetSkillPower()
{
    //技能影响战力=（(攻击×（1+增伤比例）×2 + (攻击×（1+增伤比例）×技能攻击百分比+技能额外伤害)/技能类型系数)/3-攻击×（1+增伤比例） ）×5
    SSkillActionProp* pstProp = NULL;
    if(*m_ppoFormationSkill)
    {
        pstProp = (*m_ppoFormationSkill)->GetSkillActionProp();
    }
    if(NULL == pstProp)
    {
        return 0;
    }

    UINT32 dwSkillParam = 100;//技能类型系数
    switch((*m_ppoFormationSkill)->GetDT_SKILL_DATA().stAttackRangInfo.byAttackRangKind)
    {
    case EARK_FRONTONE: //1, //前排单体
        dwSkillParam = 100;
        break;
    case EARK_BACKONE: //= 2, //后排单体
        dwSkillParam = 90;
        break;
    case EARK_FRONTALL: //= 3, //前排全体
        dwSkillParam = 80;
        break;
    case EARK_BACKALL: //= 4, //后排全体
        dwSkillParam = 72;
        break;
    case EARK_ALL: // = 5, //所有
        dwSkillParam = 57;
        break;
    case EARK_BUFF: // = 6, //追踪buff
        dwSkillParam = 95;
        break;
    case EARK_MIXBLOOD: //= 7, //追踪最少血量
        dwSkillParam = 95;
        break;
    case EARK_RAMDOM_COMBAT_TWO_ENEMY: // = 8, //随机打击两个敌人
        dwSkillParam = 80;
        break;
    default:
        break;
    }

    UINT32 dwPower1 = UINT32(UINT64(m_stBattleAttribute.dwAttack) * (100 + m_stExtBattleAttr.wIncHurtRate) / 50);
    UINT32 dwPower2 = UINT32((UINT64(m_stBattleAttribute.dwAttack) * (100 + m_stExtBattleAttr.wIncHurtRate) * pstProp->wEffectHurtRate / 10000 + pstProp->dwExtHurtValue) * 100 / dwSkillParam);
    UINT32 dwPower3 = UINT32((UINT64)(m_stBattleAttribute.dwAttack) * (100 + m_stExtBattleAttr.wIncHurtRate) / 100);

    return ((dwPower1 + dwPower2) / 3 - dwPower3) * 5;
}


/*
VOID CHero::InitExperience()//重新初始化客户端的经验信息
{
    //经验下发差值
    m_stDT_HERO_BASE_DATA_CLI.dwExperience = m_stDT_HERO_BASE_DATA.dwExperience >= pstHeroUpgradeProp->dwExperience ?
            (m_stDT_HERO_BASE_DATA.dwExperience - pstHeroUpgradeProp->dwExperience) : 0;

    m_stDT_HERO_BASE_DATA_CLI.dwUpgradeExperience = m_dwUpgradeExperience >= pstHeroUpgradeProp->dwExperience ?
            (m_dwUpgradeExperience - pstHeroUpgradeProp->dwExperience) : 0xFFFFFFFF;

    //经验下发差值
    m_stDT_HERO_BASE_DATA_CLI_NoEquip.dwExperience = m_stDT_HERO_BASE_DATA_CLI.dwExperience;

    m_stDT_HERO_BASE_DATA_CLI_NoEquip.dwUpgradeExperience = m_stDT_HERO_BASE_DATA_CLI.dwUpgradeExperience;

}
*/

VOID CHero::GetHeroSkill(DT_SKILL_DATA_LST& stDT_SKILL_DATA_LST)
{
    memset(&stDT_SKILL_DATA_LST, 0, sizeof(DT_SKILL_DATA_LST));
    for(UINT8 byIdx = 1; byIdx < MAX_SKILL_SLOT_NUM + 1; byIdx++)
    {
        CSkill* poSkill = m_apoSlotSKill[byIdx];
        if(poSkill)
        {
            memcpy(&stDT_SKILL_DATA_LST.astSkillInfo[stDT_SKILL_DATA_LST.bySkillNum++], &(poSkill->GetDT_SKILL_DATA()), sizeof(DT_SKILL_DATA));
        }
    }
}

VOID CHero::GetHeroSkillCli(DT_SKILL_DATA_CLI& stDT_SKILL_DATA_CLI)
{
    memset(&stDT_SKILL_DATA_CLI, 0, sizeof(DT_SKILL_DATA_CLI));
    if(*m_ppoFormationSkill)
    {
        (*m_ppoFormationSkill)->GetDT_SKILL_DATA_CLI(stDT_SKILL_DATA_CLI);
    }
}

VOID CHero::GetHeroSkillLstCli(DT_SKILL_DATA_LST_CLI& stDT_SKILL_DATA_LST_CLI)
{
	memset(&stDT_SKILL_DATA_LST_CLI, 0, sizeof(DT_SKILL_DATA_LST));
	UINT8 bySkillNum = 0;
	for(UINT8 byIdx = 1; byIdx < MAX_SKILL_SLOT_NUM + 1; byIdx++)
	{
		CSkill* poSkill = m_apoSlotSKill[byIdx];
		if(poSkill)
		{
			poSkill->GetDT_SKILL_DATA_CLI(stDT_SKILL_DATA_LST_CLI.astSkillInfo[bySkillNum]);
			bySkillNum++;
		}
	}

	stDT_SKILL_DATA_LST_CLI.bySkillNum = bySkillNum;
}

UINT16 CHero::OnUpdateAttr( UINT8 byAttrKind, UINT32 dwPerCountAddValue )
{
    UINT16 wCurCount = 0;
    if( EAST_HP == byAttrKind )
    {
        wCurCount = ++m_stDT_HERO_BASE_DATA.stUpdateAttrInfo.wAttrHP;
    }
    else if( EAST_ATTACK == byAttrKind )
    {
        wCurCount = ++m_stDT_HERO_BASE_DATA.stUpdateAttrInfo.wAttrAttack;
    }

    InitBattleAttribute();

    return wCurCount;
}

/**
* @brief
* 武将最后用药时间
* @return
*/
VOID CHero::ClrUseDrugCD()
{
    m_stDT_HERO_BASE_DATA.byLastUseDrugLevel = 0;
    m_stDT_HERO_BASE_DATA.qwLastUseDrugTime = 0;
}


VOID CHero::GetDrug(DT_DRUG_DATA_LST& stDT_DRUG_DATA_LST)
{
    UINT8 byDrugLevelNum = 0;
    CDrug* poDrug = m_mapLevel2Drug.GetFistData();
    while(poDrug)
    {
        DT_DRUG_DATA& stDT_DRUG_DATA = stDT_DRUG_DATA_LST.astDrugList[byDrugLevelNum++];
        memcpy(&stDT_DRUG_DATA, &(poDrug->GetDT_DRUG_DATA()), sizeof(DT_DRUG_DATA));
        poDrug = m_mapLevel2Drug.GetNextData();
    }
    stDT_DRUG_DATA_LST.byDrugLevelNum = byDrugLevelNum;
}

//穿戴上由旧装备制造的新装备，清除旧装备记录
VOID CHero::DressMakeEquip(CEquip* poNewEquip)
{
    if(NULL == poNewEquip)
    {
        return;
    }

    SEquipProp* poEquipProp = poNewEquip->GetEquipProp();
    if (NULL == poEquipProp)
    {
        return;
    }

    //记录新装备
    m_mapEquip.AddData(poNewEquip->GetMemID(), poNewEquip);
    m_mapEquipPos.AddData(poEquipProp->byPosKindID, poNewEquip);
    poNewEquip->SetHero(this);

    InitBattleAttribute();

    //SetSaveState(ESS_WANT_SAVE);
    //poNewEquip->SetObjDataSaveType(EODST_INSERT);
    //poNewEquip->SetSaveState(ESS_WANT_SAVE);
}

//清除装备回调
VOID CHero::OnDeleteEquip(CEquip* poEquip)
{
    SEquipProp* poEquipProp = poEquip->GetEquipProp();
    if (NULL == poEquipProp)
    {
        return;
    }

    //清除旧装备记录
    m_mapEquip.DelData(poEquip->GetMemID());//ID不会相同，则可以直接清除
    UINT8 byPos = poEquipProp->byPosKindID;
    if(m_mapEquipPos.GetData(byPos) == poEquip)
    {
        m_mapEquipPos.DelData(byPos);
    }
}

CDrug * CHero::GetDrug(UINT8 byDrugLevel)
{
    return m_mapLevel2Drug.GetData(byDrugLevel);
}




UINT16 CHero::GetDrugTalent()
{
    m_dwDrugTalent = 0;
    CDrug* poDrug = m_mapLevel2Drug.GetFistData();
    while(poDrug)
    {
        m_dwDrugTalent += poDrug->GetTalent();
        poDrug = m_mapLevel2Drug.GetNextData();
    }

    return m_dwDrugTalent;
}


//使用丹药
UINT16 CHero::UseDrug(SDrugProp* pstDrugProp)
{
    UINT8 byDrugLevel	= pstDrugProp->byLevel;
    UINT32 dwPlayerID	= m_poOwner->GetDT_PLAYER_BASE_DATA().dwID;
    UINT16 wHeroKindID	= m_stDT_HERO_BASE_DATA.wKindID;
    CDrug* poDrug = m_mapLevel2Drug.GetData(byDrugLevel);

    //该级别丹药未使用过
    if(NULL == poDrug)
    {
        //新增
        poDrug = CDrugMgr::Instance()->CreateDrug(dwPlayerID, wHeroKindID, byDrugLevel);
        if(NULL == poDrug)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CreateDrug failed!"), MSG_MARK);
            RETURN_OTHER_ERR;
        }

        DT_DRUG_DATA stDT_DRUG_DATA;
        stDT_DRUG_DATA.byLevel = byDrugLevel;
        stDT_DRUG_DATA.byNum = 1;
        stDT_DRUG_DATA.wHeroKindID = wHeroKindID;
        if(FALSE == poDrug->Init(&stDT_DRUG_DATA, m_poOwner))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: drug init failed!"), MSG_MARK);
            CDrugMgr::Instance()->RemoveDrug(dwPlayerID, wHeroKindID, byDrugLevel);
            RETURN_OTHER_ERR;
        }
        m_mapLevel2Drug.AddData(byDrugLevel, poDrug);

        //重新初始化属性
        poDrug->OnUse();
        InitBattleAttribute();

        //poDrug->SetObjDataSaveType(EODST_INSERT);
        //poDrug->SetSaveState(ESS_WANT_SAVE);
    }
    //该级别丹药有使用过
    else
    {
        DT_DRUG_DATA& stDT_DRUG_DATA = poDrug->GetDT_DRUG_DATA();
        if(poDrug->CkFull())
        {
            return ERR_USE_DRUG::ID_SUCCESS;
        }

        //记录使用个数
        stDT_DRUG_DATA.byNum++;

        //重新初始化属性
        poDrug->OnUse();
        InitBattleAttribute();

        //poDrug->SetSaveState(ESS_WANT_SAVE);
    }

    //SetSaveState(ESS_WANT_SAVE);	//丹药有hero管理，丹药需保存则hero需保存
    return ERR_CLR_USE_DRUG_CD::ID_SUCCESS;
}


//获取资质
VOID CHero::GetAttrForAfterUseDrug(DT_BATTLE_ATTRIBUTE& stHeroBattleAttribute, UINT16& wHeroTalent)
{
    SHeroProp* pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(m_stDT_HERO_BASE_DATA.wKindID);
    if(NULL == pstHeroProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstHeroProp[KindID:%d] is NULL!"), MSG_MARK, m_stDT_HERO_BASE_DATA.wKindID);
        return;
    }

    memcpy(&stHeroBattleAttribute, &m_stBattleAttribute, sizeof(stHeroBattleAttribute));
    wHeroTalent = m_stDT_HERO_BASE_DATA.wTalent + pstHeroProp->wTalent + m_dwDrugTalent;
}

//强化回调
VOID CHero::OnStrengthen()
{
    //赋值战斗属性
    InitBattleAttribute();
}

//db清除器魂回调
BOOL CHero::FreeGodWeapon()
{

    //释放神器
    if (m_poGodWeapon)
    {
        CGodweaponMgr::Instance()->RemoveGodWeapon(m_poOwner->GetDT_PLAYER_BASE_DATA().dwID, m_stDT_HERO_BASE_DATA.wKindID);
        m_poGodWeapon = NULL;
    }
    //重算武将的战斗属性
    InitBattleAttribute();

    return TRUE;
}

//获得神器战斗信息
VOID CHero::GetDT_GODWEAPON_BATTLE_DATA(DT_GODWEAPON_BATTLE_DATA& stGodweaponInfo)
{
    if (m_poGodWeapon)
    {
        m_poGodWeapon->GetDT_GODWEAPON_BATTLE_DATA(m_stDT_HERO_BASE_DATA.wKindID, stGodweaponInfo);
    }
    else
    {
        memset(&stGodweaponInfo, 0, sizeof(stGodweaponInfo));
    }
}


CGodWeapon* CHero::NewGodweapon(DT_GODWEAPON_DATA* pstDT_GODWEAPON_DATA)
{
    if(InitGodweapon(pstDT_GODWEAPON_DATA))
    {
        //重算武将的战斗属性
        InitBattleAttribute();

        return m_poGodWeapon;
    }

    return NULL;
}



DT_STUDY_DATA* CHero::GetStudy(UINT8 byPos)
{
    if(byPos >= MAX_HERO_STUDY_GRID_NUM)
    {
        return NULL;
    }

    return &m_astStudy[byPos];
}

VOID CHero::OnUpgradeStudy()
{
    InitBattleAttribute();
}

VOID CHero::OnDressStudy()
{
    InitBattleAttribute();
}


//获取武将所有资质的总和,不加神器的，神器的已经加到m_stExtBattleAttr.wCoachTalent
UINT16 CHero::GetTalent()
{
    SHeroProp* pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(m_stDT_HERO_BASE_DATA.wKindID);
    if(NULL == pstHeroProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstHeroProp[KindID:%d] is NULL!"), MSG_MARK, m_stDT_HERO_BASE_DATA.wKindID);
    }

    if( pstHeroProp )
    {
        return m_stDT_HERO_BASE_DATA.wTalent + pstHeroProp->wTalent +  GetDrugTalent() + m_stExtBattleAttr.wCoachTalent;
    }
    else
    {
        return m_stDT_HERO_BASE_DATA.wTalent +  GetDrugTalent() + m_stExtBattleAttr.wCoachTalent;
    }

}

UINT8 CHero::GetGodweaponAddTalent()
{
    if( !m_poGodWeapon )
    {
        return 0;
    }

    UINT8 retTalent = 0;
    SHeroGodweaponAllEncAttrProp *pallProp = CGodweaponPropMgr::Instance()->GetGodweaponAllEncAttr( m_stDT_HERO_BASE_DATA.wKindID );
    if (NULL == pallProp)
    {
        return 0;
    }

    for( INT32 nIndex = 0; nIndex < m_poGodWeapon->GetDT_GODWEAPON_DATA().wQuality - 1 && nIndex < pallProp->byEncAttrNum; ++nIndex )
    {
        if( pallProp->astQualityUpgradeEncAttr[nIndex].byAttrKind != EBAK_COACHTALENT )
        {
            continue;
        }
        retTalent += pallProp->astQualityUpgradeEncAttr[nIndex].dwAttrValue;
    }
    return  retTalent;
}


//招募回调
VOID CHero::OnRecuit()
{
    m_stDT_HERO_BASE_DATA.byPos = EHP_CARRY;
    m_stDT_HERO_BASE_DATA.byRecuitedFlag = 1;//已经招募过

    /*
    m_stDT_HERO_BASE_DATA_CLI.byPos = m_stDT_HERO_BASE_DATA.byPos;
    m_stDT_HERO_BASE_DATA_CLI_NoEquip.byPos = m_stDT_HERO_BASE_DATA.byPos;
    m_stDT_HERO_BASE_DATA_CLI.byRecuitedFlag = m_stDT_HERO_BASE_DATA.byRecuitedFlag;
    m_stDT_HERO_BASE_DATA_CLI_NoEquip.byRecuitedFlag = m_stDT_HERO_BASE_DATA.byRecuitedFlag;
    */
}

//离队回调
VOID CHero::OnDisband()
{
    m_stDT_HERO_BASE_DATA.byPos = EHP_RECRUIT;
    m_stDT_HERO_BASE_DATA.byFormationIdx = 0;

    /*
    m_stDT_HERO_BASE_DATA_CLI.byPos = m_stDT_HERO_BASE_DATA.byPos;
    m_stDT_HERO_BASE_DATA_CLI_NoEquip.byPos = m_stDT_HERO_BASE_DATA.byPos;
    m_stDT_HERO_BASE_DATA_CLI.byFormationIdx = m_stDT_HERO_BASE_DATA.byFormationIdx;
    m_stDT_HERO_BASE_DATA_CLI_NoEquip.byFormationIdx = m_stDT_HERO_BASE_DATA.byFormationIdx;
    */
}

//拜访回调
VOID CHero::OnVisit(UINT8 byVisitNum)
{
    m_stDT_HERO_BASE_DATA.byVisitNum += byVisitNum;
}

//上阵回调
VOID CHero::OnUp(UINT8 byFormationIdx)
{
    m_stDT_HERO_BASE_DATA.byPos = EHP_BATTLE;
    m_stDT_HERO_BASE_DATA.byFormationIdx = byFormationIdx;

    /*
    m_stDT_HERO_BASE_DATA_CLI.byPos = m_stDT_HERO_BASE_DATA.byPos;
    m_stDT_HERO_BASE_DATA_CLI_NoEquip.byPos = m_stDT_HERO_BASE_DATA.byPos;
    m_stDT_HERO_BASE_DATA_CLI.byFormationIdx = m_stDT_HERO_BASE_DATA.byFormationIdx;
    m_stDT_HERO_BASE_DATA_CLI_NoEquip.byFormationIdx = m_stDT_HERO_BASE_DATA.byFormationIdx;
    */
}

//下阵回调
VOID CHero::OnDown()
{
    m_stDT_HERO_BASE_DATA.byPos = EHP_CARRY;
    m_stDT_HERO_BASE_DATA.byFormationIdx = 0;

    /*
    m_stDT_HERO_BASE_DATA_CLI.byPos = m_stDT_HERO_BASE_DATA.byPos;
    m_stDT_HERO_BASE_DATA_CLI_NoEquip.byPos = m_stDT_HERO_BASE_DATA.byPos;
    m_stDT_HERO_BASE_DATA_CLI.byFormationIdx = m_stDT_HERO_BASE_DATA.byFormationIdx;
    m_stDT_HERO_BASE_DATA_CLI_NoEquip.byFormationIdx = m_stDT_HERO_BASE_DATA.byFormationIdx;
    */
}


//初始化
BOOL CHero::InitSkill(DT_SKILL_DATA_LST& stDT_SKILL_DATA_LST)
{
    UINT8 bySkillNum = stDT_SKILL_DATA_LST.bySkillNum;
    if(0 == m_stDT_HERO_BASE_DATA.byCoach) //非主角，使用默认配置的技能
    {
        bySkillNum = 0;
    }
    BOOL bHaveSkill = FALSE;
    for(UINT8 byIdx = 1; ((byIdx <= bySkillNum) && (byIdx <= MAX_SKILL_SLOT_NUM)); byIdx++)
    {
        CSkill* poSKill = CSkillMgr::Instance()->CreateSkill();
        if(NULL == poSKill)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CreateSkill failed!"), MSG_MARK);
            continue;
        }
        if(FALSE == poSKill->Init(stDT_SKILL_DATA_LST.astSkillInfo[byIdx - 1], m_poOwner))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: Skill init failed!"), MSG_MARK);
            CSkillMgr::Instance()->RemoveSkill(poSKill);
            continue;
        }
        m_apoSlotSKill[byIdx] = poSKill;
        bHaveSkill = TRUE;
    }
    //没有技能则则添加初始默认技能
    if((0 == bySkillNum) || (!bHaveSkill))
    {
        SHeroProp* pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(m_stDT_HERO_BASE_DATA.wKindID);
        if(NULL == pstHeroProp)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: pstHeroProp[KindID:%d] is NULL!"), MSG_MARK, m_stDT_HERO_BASE_DATA.wKindID);
            return FALSE;
        }
        SSkillActionProp* pstSkillActionProp = CSkillPropMgr::Instance()->GetSkillActionProp(pstHeroProp->wSkillActionID, pstHeroProp->wSkillActionLevel);
        if(NULL == pstSkillActionProp)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: pstSkillActionProp[%d.%d] is NULL!"), MSG_MARK, pstHeroProp->wSkillActionID, pstHeroProp->wSkillActionLevel);
            return FALSE;
        }

        DT_SKILL_DATA stInitSKill;
        memset(&stInitSKill, 0, sizeof(stInitSKill));
        //stInitSKill.wID = pstHeroProp->wSkillID;
        //stInitSKill.wLevel = pstHeroProp->wSkillLevel;
        stInitSKill.wActionID = pstHeroProp->wSkillActionID;
        stInitSKill.wActionLevel = pstHeroProp->wSkillActionLevel;
        stInitSKill.wEffectID = pstHeroProp->wSkillEffectID;
        stInitSKill.stAttackRangInfo.byAttackRangKind = pstSkillActionProp->byUnlockAttackRangKind;
        stInitSKill.stAttackRangInfo.byTraceBuffKind = pstSkillActionProp->byUnlockTraceBuffKind;
        if(EARK_INVALID == stInitSKill.stAttackRangInfo.byAttackRangKind)
        {
            stInitSKill.stAttackRangInfo.byAttackRangKind = EARK_FRONTONE;
        }

        CSkill* poSKill = CSkillMgr::Instance()->CreateSkill();
        if(NULL == poSKill)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CreateSkill failed!"), MSG_MARK);
            return FALSE;
        }
        if(FALSE == poSKill->Init(stInitSKill, m_poOwner))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: Skill init failed!"), MSG_MARK);
            CSkillMgr::Instance()->RemoveSkill(poSKill);
            return FALSE;
        }
        m_apoSlotSKill[1] = poSKill;//先使用第1个技能槽的技能，后续扩展再修改
    }

    m_ppoFormationSkill = &m_apoSlotSKill[1];//先使用第1个技能槽的技能，后续扩展再修改

    return TRUE;
}

void CHero::ResetSkill()
{
    for (UINT16 i = 0; i < MAX_SKILL_SLOT_NUM+1; i++){
        CSkill *poSkill = m_apoSlotSKill[i];
        if (poSkill){
            CSkillMgr::Instance()->RemoveSkill(poSkill);
            m_apoSlotSKill[i] = NULL;
        }
    }
        
    DT_SKILL_DATA_LST stEmptySkill;
    memset(&stEmptySkill, 0, sizeof(stEmptySkill));

    InitSkill(stEmptySkill);
}

void CHero::ResetEquipOwner()
{
    UINT16 wHeroID = GetHeroID();
    for(CEquip* poEquip = m_mapEquip.GetFistData(); poEquip; poEquip = m_mapEquip.GetNextData()){
        poEquip->GetDT_EQUIP_DATA().wHeroKindID = wHeroID;
    }

    for(CJewelry* poJewelry = m_mapJewelry.GetFistData(); poJewelry; poJewelry = m_mapJewelry.GetNextData()){
        poJewelry->GetDT_JEWELRY_DATA().wHeroKindID = wHeroID;
    }

    for(CDrug* poDrug = m_mapLevel2Drug.GetFistData(); poDrug; poDrug = m_mapLevel2Drug.GetNextData()){
        poDrug->GetDT_DRUG_DATA().wHeroKindID = wHeroID;
    }

    m_stDT_HERO_UPGRADE_DATA.wHeroKindID = wHeroID;
}

BOOL CHero::InitStudy(DT_STUDY_DATA astStudyInfo[MAX_HERO_STUDY_GRID_NUM])
{
    memcpy(m_astStudy, astStudyInfo, sizeof(m_astStudy));

    return TRUE;
}


SHeroProp* CHero::GetHeroProp()
{
    SHeroProp* pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(m_stDT_HERO_BASE_DATA.wKindID);
    if(NULL == pstHeroProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstHeroProp[KindID:%d] is NULL!"), MSG_MARK, m_stDT_HERO_BASE_DATA.wKindID);
    }

    return pstHeroProp;
}


SHeroUpgradeProp* CHero::GetHeroUpgradeProp()
{
    return CHeroPropMgr::Instance()->GetHeroUpgradeProp(m_stDT_HERO_BASE_DATA.wLevel + 1);
}


//境界进阶回调
VOID CHero::OnJingjieQualityUpgrade()
{
    //赋值战斗属性
    InitBattleAttribute();
}

UINT8 CHero::GetMaxStudyLevel() //心得等级
{
    UINT8 byMaxStudyLevel = 0;
    for (UINT8 byidx = 0; byidx < MAX_HERO_STUDY_GRID_NUM; byidx++)
    {
        if (0 == m_astStudy[byidx].byStudyColorKind)
        {
            continue;
        }
        if (m_astStudy[byidx].byStudyLevel > byMaxStudyLevel)
        {
            byMaxStudyLevel = m_astStudy[byidx].byStudyLevel;
        }
    }
    return byMaxStudyLevel;
}

UINT16 CHero::UpgradeSkillAction(UINT16 wActionID, UINT16 wActionLevelCli)
{
    CSkill* poSkill = *m_ppoFormationSkill; //目前就1个技能
    //for(UINT8 byIdx = 1; byIdx <= MAX_SKILL_SLOT_NUM; byIdx++)
    //{
    //	poSkill = m_apoSlotSKill[byIdx];
    //	if(poSkill->GetDT_SKILL_DATA().wActionID == wActionID)
    //	{
    //		break;
    //	}
    //}
    if(NULL == poSkill)
    {
        return ERR_UPGRADE_SKILL_ACTION_LEVEL::ID_ACTION_NOT_EXIST;
    }
	return poSkill->Upgrade(wActionLevelCli);
}


void CHero::AddTransHeroID(UINT16 KindID)
{
    if (m_stAbsorbDataLst.byAbsorbNum >= MAX_ABSORB_NUM)
    {
        return;
    }
    m_stAbsorbDataLst.awKindIDLst[m_stAbsorbDataLst.byAbsorbNum++] = KindID;
    return;
}

BOOL	CHero::GetTransHeroID(UINT16 KindID)
{
    for (UINT8 byAbsorbNum = 0; byAbsorbNum < m_stAbsorbDataLst.byAbsorbNum; byAbsorbNum++)
    {
        if (m_stAbsorbDataLst.awKindIDLst[byAbsorbNum] == KindID)
        {
            return TRUE;
        }
    }
    return FALSE;
}


UINT16 CHero::Change2NewCareer(UINT8 byCareerID)
{
    UINT16 wOldHeroID = m_stDT_HERO_BASE_DATA.wKindID;
    if(byCareerID == wOldHeroID)
    {
        return ERR_CHANGE_CAREER_INFO::ID_SUCCESS;
    }
    m_stDT_HERO_BASE_DATA.wKindID = byCareerID;

    //修改穿戴装备的HeroID
    CEquip* poEquip = m_mapEquip.GetFistData();
    while(poEquip)
    {
        poEquip->GetDT_EQUIP_DATA().wHeroKindID = byCareerID;
        poEquip = m_mapEquip.GetNextData();
    }

    //修改神器
    if(m_poGodWeapon)
    {
        DT_GODWEAPON_DATA stDT_GODWEAPON_DATA = m_poGodWeapon->GetDT_GODWEAPON_DATA();
        stDT_GODWEAPON_DATA.wHeroKindID = byCareerID;
        if(!m_poGodWeapon->Init(&stDT_GODWEAPON_DATA, m_poOwner, this))
        {
            RETURN_OTHER_ERR;
        }
    }

    //修改技能
    DT_SKILL_DATA_LST stOldDT_SKILL_DATA_LST  = {0};
    GetHeroSkill(stOldDT_SKILL_DATA_LST);
    DT_SKILL_DATA_LST stNewDT_SKILL_DATA_LST = stOldDT_SKILL_DATA_LST;
    //目前只有1个技能
    stNewDT_SKILL_DATA_LST.bySkillNum = 1;
    stNewDT_SKILL_DATA_LST.astSkillInfo[0].wActionID = 0;
    SHeroProp* pstHeroProp = CHeroPropMgr::Instance()->GetHeroProp(byCareerID);
    if(NULL == pstHeroProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstHeroProp[KindID:%d] is NULL!"), MSG_MARK, byCareerID);
        RETURN_OTHER_ERR;
    }
    SSkillActionProp* pstSkillActionProp = CSkillPropMgr::Instance()->GetSkillActionProp(pstHeroProp->wSkillActionID, stNewDT_SKILL_DATA_LST.astSkillInfo[0].wActionLevel);
    if(NULL == pstSkillActionProp)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: pstSkillActionProp[%d.%d] is NULL!"), MSG_MARK, pstHeroProp->wSkillActionID, stNewDT_SKILL_DATA_LST.astSkillInfo[0].wActionLevel);
        RETURN_OTHER_ERR;
    }
    stNewDT_SKILL_DATA_LST.astSkillInfo[0].wActionID = pstHeroProp->wSkillActionID;
    //清除之前的技能,目前就一个技能
    if(*m_ppoFormationSkill)
    {
        CSkillMgr::Instance()->RemoveSkill(*m_ppoFormationSkill);
        m_apoSlotSKill[1] = NULL;
    }
    if(FALSE == InitSkill(stNewDT_SKILL_DATA_LST))
    {
        USR_INFO(_SDT("[%s: %d]:  Hero[%d] InitSkill failed!!"), MSG_MARK, byCareerID);
        InitSkill(stOldDT_SKILL_DATA_LST);//恢复之前的技能
        RETURN_OTHER_ERR;
    }

    m_stDT_HERO_UPGRADE_DATA.wHeroKindID = m_stDT_HERO_BASE_DATA.wKindID;

    //重新赋值战斗属性
    InitBattleAttribute();

    return ERR_CHANGE_CAREER_INFO::ID_SUCCESS;
}

BOOL CHero::IsFormationHero()
{
    CFormationHeroShmemMap& mapHero = m_poOwner->GetFormationHeroMap();//所有上阵英雄
    CHero* poHero = mapHero.GetFistData();
    while (poHero)
    {
        if ( poHero == this)
        {
            return TRUE;
        }
        poHero = mapHero.GetNextData();
    }
    return FALSE;
}

VOID CHero::GetPowerByFormation(UINT8 byFormationID, UINT8 byFormationLevel, UINT32& dwPower, DT_BATTLE_ATTRIBUTE& stAttrBute)
{
	
    const SFormationUpgradeProp* poProp = CFormationPropMgr::Instance()->GetFormationUpgrade(byFormationID, byFormationLevel);
    if ( NULL == poProp)
    {
        return ;
    }
	//设置打开阵法的战力计算
	SetFormationData( poProp->byAttrKindID, poProp->dwAttrValue);
	InitBattleAttribute();

    dwPower = GetPower();
	stAttrBute = CBattleMgr::Instance()->NewToOldBattleAttribute(m_stBattleAttribute);
}

BOOL CHero::InitJewelry(DT_JEWELRY_DATA_LST& pstDT_JEWELRY_DATA_LST)
{
	CPlayerJewelryShmemMap& mapPlayerJewery = m_poOwner->GetAllJewelry();
	CItemKind2IdxMap& mapJewelryKind2Idx = m_poOwner->GetEquipKind2IdxMap();
	UINT16 wJewelryNum = pstDT_JEWELRY_DATA_LST.wJewelryNum;
	for(UINT16 wIdx = 0; wIdx < wJewelryNum; wIdx++)
	{
		DT_JEWELRY_DATA& stJewelryData = pstDT_JEWELRY_DATA_LST.astJewelryInfo[wIdx];
		//代码原因会造成饰品丢失，注释
		//if(stJewelryData.wHeroKindID != m_stDT_HERO_BASE_DATA.wKindID)
		//{
		//	//异常
		//	SYS_CRITICAL(_SDT("[%s: %d]: InitJewelry failed! HeroKindID is not match [%d != %d]!"), MSG_MARK, stJewelryData.wJewelryID, m_stDT_HERO_BASE_DATA.wKindID);
		//	continue;
		//}

		//赋值最大序号
		UINT16& wMaxJewelryIdx = mapJewelryKind2Idx[stJewelryData.wHeroKindID];
		if(stJewelryData.wIdx > wMaxJewelryIdx)
		{
			wMaxJewelryIdx = stJewelryData.wIdx;
		}

		CJewelry* poJewery = dynamic_cast<CJewelry*>(CItemMgr::Instance()->AttachItem(m_poOwner, stJewelryData.wJewelryID, &stJewelryData));
		if(NULL == poJewery)
		{
			return FALSE;
		}

		SJewelryProp* poJeweryProp = poJewery->GetProp();
		if (NULL == poJeweryProp)
		{
			return FALSE;
		}

		m_mapJewelryPos.AddData(poJeweryProp->byPosKindID, poJewery);
		poJewery->SetHero(this);
		m_mapJewelry.AddData(poJewery->GetMemID(), poJewery);

		mapPlayerJewery.AddData(CLogicFunc::Get1616Key(poJewery->GetDT_JEWELRY_DATA().wJewelryID, poJewery->GetDT_JEWELRY_DATA().wIdx), poJewery);
	}

	return TRUE;
}
VOID CHero::GetJeweryData(DT_JEWELRY_DATA_LST& stJewelryDataList)
{
	stJewelryDataList.wJewelryNum = m_mapJewelryPos.Size();
	CJewelry* poJewelry = m_mapJewelryPos.GetFistData();
	UINT8 byJewelryNum = 0;
	while ( poJewelry )
	{
		DT_JEWELRY_DATA& stData = poJewelry->GetDT_JEWELRY_DATA();
		stData.wHeroKindID = m_stDT_HERO_BASE_DATA.wKindID;
		stJewelryDataList.astJewelryInfo[byJewelryNum] = stData;
		byJewelryNum++;
		poJewelry = m_mapJewelryPos.GetNextData();
	}
}

VOID CHero::GetJeweryCli(DT_HERO_JEWELRY_DATA_LST_CLI& stJewelryDataList)
{
	stJewelryDataList.wJewelryNum = m_mapJewelryPos.Size();
	CJewelry* poJewelry = m_mapJewelryPos.GetFistData();
	UINT8 byJewelryNum = 0;
	while ( poJewelry )
	{
		poJewelry->GetJewelryDataCli(stJewelryDataList.astJewelryList[byJewelryNum]);
		byJewelryNum++;
		poJewelry = m_mapJewelryPos.GetNextData();
	}
}

VOID CHero::GetJewerySuitCli(DT_JEWELRY_SUIT_CLI& stJewelrySuitCli)
{
    stJewelrySuitCli.bySuitAttrNum = 0;
    if(!IsDressJewelrySuit()){
        return;
    }

    SJewelrySuitProp *pstJewelrySuitProp = CJewelryPropMgr::Instance()->GetJewelrySuitProp(m_wJewelrySuitDressLvl, m_wJewelrySuitLvl);
    stJewelrySuitCli = pstJewelrySuitProp->stSuitCli;
    stJewelrySuitCli.wSuitLvl = m_wJewelrySuitLvl;
}

UINT16 CHero::DressJewelry(PKT_CLIGS_DRESS_JEWELRY_REQ& stReq, PKT_CLIGS_DRESS_JEWELRY_ACK& stAck)
{
	//检测背包玩家是否拥有该饰品
	CJewelry* poJewelry = m_poOwner->GetJewelry(stReq.wDressJewelryID, stReq.wDressItemIdx);
	if ( NULL == poJewelry)
	{
		return ERR_DRESS_JEWELRY::ID_NOT_HAVE_JEWELRY;
	}
	UINT16 wDressLevel = poJewelry->GetDT_JEWELRY_DATA().wJewelryLevel;
	UINT16 wHeroLevel = m_stDT_HERO_BASE_DATA.wLevel;
	if ( wHeroLevel < wDressLevel )
	{
		return ERR_DRESS_JEWELRY::ID_LEVEL_NOT_ENOUGH;
	}

	poJewelry->DressHero(this);


	return ERR_DRESS_JEWELRY::ID_SUCCESS;
}

UINT16 CHero::GetOffJewelry(PKT_CLIGS_DRESS_JEWELRY_REQ& stReq, PKT_CLIGS_DRESS_JEWELRY_ACK& stAck)
{
	if( 0 == m_poOwner->GetBag().GetIdleSize())
	{
		return ERR_DRESS_JEWELRY::ID_BAG_IS_FULL;
	}
	
	//检测背包玩家是否拥有该饰品
	CJewelry* poJewelry = m_poOwner->GetJewelry(stReq.wUndressJewelryID, stReq.wUndressItemIdx);
	if ( NULL == poJewelry)
	{
		return ERR_DRESS_JEWELRY::ID_NOT_HAVE_JEWELRY;
	}

	if( !poJewelry->UnDressBag())
	{
		return ERR_DRESS_JEWELRY::ID_BAG_IS_FULL;
	}

	return ERR_DRESS_JEWELRY::ID_SUCCESS;
}

UINT16 CHero::ReplaceJewelry(PKT_CLIGS_DRESS_JEWELRY_REQ& stReq, PKT_CLIGS_DRESS_JEWELRY_ACK& stAck)
{

	//检测背包玩家是否拥有该饰品
	CJewelry* poDress = m_poOwner->GetJewelry(stReq.wDressJewelryID, stReq.wDressItemIdx);
	if ( NULL == poDress)
	{
		return ERR_DRESS_JEWELRY::ID_NOT_HAVE_JEWELRY;
	}

	CJewelry* poUnDress = m_poOwner->GetJewelry(stReq.wUndressJewelryID, stReq.wUndressItemIdx);
	if ( NULL == poUnDress)
	{
		return ERR_DRESS_JEWELRY::ID_NOT_HAVE_JEWELRY;
	}
	SJewelryProp* poDressProp = poDress->GetProp();
	if( NULL == poDressProp)
	{
		return ERR_DRESS_JEWELRY::ID_NOT_HAVE_JEWELRY;
	}
	SJewelryProp* poUnDressProp = poUnDress->GetProp();
	if ( NULL == poUnDressProp)
	{
		return ERR_DRESS_JEWELRY::ID_NOT_HAVE_JEWELRY;
	}
	//位置不一
	if ( poDressProp->byPosKindID != poUnDressProp->byPosKindID)
	{
		return ERR_DRESS_JEWELRY::ID_NOT_HAVE_JEWELRY;
	}

	UINT16 wDressLevel = poDress->GetDT_JEWELRY_DATA().wJewelryLevel;
	UINT16 wHeroLevel = m_stDT_HERO_BASE_DATA.wLevel;
	if ( wHeroLevel < wDressLevel )
	{
		return ERR_DRESS_JEWELRY::ID_LEVEL_NOT_ENOUGH;
	}

	if( (this == poDress->GetHero()) && (NULL == poUnDress->GetHero()))
	{
		return ERR_DRESS_JEWELRY::ID_SUCCESS;
	}


	if( !poDress->DressHero(this))
	{
		return ERR_DRESS_JEWELRY::ID_BAG_IS_FULL;
	}

	return ERR_DRESS_JEWELRY::ID_SUCCESS;
}

UINT16 CHero::OnDressJewelryAck(PKT_CLIGS_DRESS_JEWELRY_REQ& stReq, PKT_CLIGS_DRESS_JEWELRY_ACK& stAck)
{
	UINT16 wRet = ERR_DRESS_JEWELRY::ID_SUCCESS;
	if ( EEDT_DRESS == stReq.byDressType )
	{
		wRet = DressJewelry(stReq, stAck);
	}
	else if ( EEDT_UNDRESS == stReq.byDressType)
	{
		wRet = GetOffJewelry(stReq, stAck);
	}
	else if (EEDT_REPLACE == stReq.byDressType)
	{
		wRet = ReplaceJewelry(stReq, stAck);
	}

    ReCheckJewelrySuit();
    GetJewerySuitCli(stAck.stJewelrySuitInfo);

	stAck.stPowerInfo.dwOldPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	m_poOwner->ResetBattleAttr(TRUE);
	stAck.stPowerInfo.dwCurPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;
	stAck.dwPlayerHP = (UINT32)m_poOwner->GetMaxDT_BATTLE_ATTRIBUTE().qwHP;


	stAck.byDressType = stReq.byDressType;
	stAck.wDressItemIdx = stReq.wDressItemIdx;
	stAck.wDressJewelryID = stReq.wDressJewelryID;
	stAck.wHeroKindID = stReq.wHeroKindID;
	stAck.wUndressItemIdx = stReq.wUndressItemIdx;
	stAck.wUndressJewelryID = stReq.wUndressJewelryID;
	m_poOwner->GetHeroPanelLstCli(stAck.stHeroPanelInfo);
	return wRet;
}

BOOL CHero::DressJewelry(CJewelry* poJewelry)
{
	if ( NULL == poJewelry)
	{
		return FALSE;
	}

	m_mapJewelry.AddData(poJewelry->GetMemID(), poJewelry);
	m_mapJewelryPos.AddData(poJewelry->GetProp()->byPosKindID, poJewelry);
	poJewelry->SetHero(this);
	return TRUE;
}

BOOL CHero::UnDressJewelry(CJewelry* poJewelry)
{
	if ( NULL == poJewelry)
	{
		return FALSE;
	}
	SJewelryProp* poProp = poJewelry->GetProp();
	if ( NULL == poProp)
	{
		return FALSE;
	}

	m_mapJewelry.DelData(poJewelry->GetMemID());
	m_mapJewelryPos.DelData(poProp->byPosKindID);
	poJewelry->SetHero(NULL);
	return TRUE;
}

VOID CHero::ResetJewelryAttr()
{
	m_mapJewelryAttr.clear();
	m_mapJewelryValue.clear();

	CJewelry* poJewelry = m_mapJewelry.GetFistData();
	while ( poJewelry )
	{
		UINT8 byExtNum = poJewelry->GetDT_JEWELRY_DATA().stExtAttrLst.byExtAttrNum;
		for ( UINT8 byIdx = 0; byIdx < byExtNum; byIdx++)
		{
			DT_BATTLE_ATTR_EXT_INFO& stInfo = poJewelry->GetDT_JEWELRY_DATA().stExtAttrLst.astExtAttrInfo[byIdx];
            if (CJewelry::IsOneHeroAttr((JewelryExtAttr)stInfo.byKindID)){
                m_mapJewelryAttr[stInfo.byKindID] += stInfo.dwKindValue;
            }
		}

		poJewelry = m_mapJewelry.GetNextData();
	}

    AddJewelrySuitAttr();

	//全局属性
	C3232Map& mapJewelryAttr = m_poOwner->GetJewelryAttr();
	m_mapJewelryAttr[EJEA_NORMAL_ATTACK] += mapJewelryAttr[EJEA_ALL_NORMAL_ATTACK];
	m_mapJewelryAttr[EJEA_SKILL_ATTACK] += mapJewelryAttr[EJEA_ALL_SKILL_ATTACK];
	m_mapJewelryAttr[EJEA_WEAPON_ATTACK] += mapJewelryAttr[EJEA_ALL_WEAPON_ATTACK];
	m_mapJewelryAttr[EJEA_WEAPON_DEFEND] += mapJewelryAttr[EJEA_ALL_WEAPON_DEFEND];
	m_mapJewelryAttr[EJEA_ALL_SCIENCE_LEVEL] += mapJewelryAttr[EJEA_ALL_SCIENCE_LEVEL];
}

VOID CHero::JewelryCountBySelf(map<UINT32, UINT32>& mapAttr)
{
	m_mapJewelryValue.clear();

	C3232MapItr itr = m_mapJewelryAttr.begin();
	for ( ; itr != m_mapJewelryAttr.end(); itr++)
	{
		switch(itr->first)
		{
		case EJEA_ARMS://武器
			AddEquipAttr(EEPT_HAND, itr->second, itr->first);
			break;
		case EJEA_CLOTHING://衣服
			AddEquipAttr(EEPT_BODY, itr->second, itr->first);
			break;
		case EJEA_HELM://头盔
			AddEquipAttr(EEPT_HEAD, itr->second, itr->first);
			break;
		case EJEA_SHOE://鞋子
			AddEquipAttr(EEPT_FOOT, itr->second, itr->first);
			break;
		case EJEA_ALL_EQUIP://所有装备
			AddEquipAttr(0, itr->second, itr->first);
			break;
		case EJEA_FIRST_STUDY://第一卦象
			AddStudyAttr(1, itr->second, itr->first);
			break;
		case EJEA_SECOND_STUDY://第二卦象
			AddStudyAttr(2, itr->second, itr->first);
			break;
		case EJEA_THIRD_STUDY://第三卦象
			AddStudyAttr(3, itr->second, itr->first);
			break;
		case EJEA_FOURTH_STUDY://第四卦象
			AddStudyAttr(4, itr->second, itr->first);
			break;
		case EJEA_ALL_STUDY://所有卦象
			AddStudyAttr(0, itr->second, itr->first);
			break;
		case EJEA_HP://生命
			m_mapJewelryValue[EBAK_HP] += AddValueByValueType(mapAttr[EBAK_HP], itr->second, CJewelryPropMgr::Instance()->GetValueType(itr->first));
			break;
		case EJEA_ATTACK://攻击
			m_mapJewelryValue[EBAK_ATTACK] += AddValueByValueType(mapAttr[EBAK_ATTACK], itr->second, CJewelryPropMgr::Instance()->GetValueType(itr->first));
			break;
		case EJEA_DEFENG://防守+XXXX
			m_mapJewelryValue[EBAK_DEFEND] += AddValueByValueType(mapAttr[EBAK_DEFEND], itr->second, CJewelryPropMgr::Instance()->GetValueType(itr->first));
			break;
		case EJEA_HITRATE://破击率+XXXX
			m_mapJewelryValue[EBAK_HITRATE] += AddValueByValueType(mapAttr[EBAK_HITRATE], itr->second, CJewelryPropMgr::Instance()->GetValueType(itr->first));
			break;
		case EJEA_DODGERATE://格挡率+XXXX
			m_mapJewelryValue[EBAK_DODGERATE] += AddValueByValueType(mapAttr[EBAK_DODGERATE], itr->second, CJewelryPropMgr::Instance()->GetValueType(itr->first));
			break;
		case EJEA_CRITRATE://暴击率+XXXX
			m_mapJewelryValue[EBAK_CRITRATE] += AddValueByValueType(mapAttr[EBAK_CRITRATE], itr->second, CJewelryPropMgr::Instance()->GetValueType(itr->first));
			break;
		case EJEA_DECRITRATE://抗暴率+XXXX
			m_mapJewelryValue[EBAK_DECRITRATE] += AddValueByValueType(mapAttr[EBAK_DECRITRATE], itr->second, CJewelryPropMgr::Instance()->GetValueType(itr->first));
			break;
		default:
			break;
		}
	}
}

VOID CHero::AddEquipAttr(UINT8 byPos,  UINT32 dwValue, UINT8 byJewelryExtID)
{
	CEquip* poEquip = m_mapEquip.GetFistData();
	CEquipMaster *poEquipMaster = m_poOwner->GetEquipMaster();
	while(poEquip)
	{
		if ((byPos == poEquip->GetEquipProp()->byPosKindID) || (0 == byPos) )
		{
			const DT_BATTLE_ATTRIBUTE2 stDT_BATTLE_ATTRIBUTE = poEquip->GetBattleAttr();

			m_mapJewelryValue[EBAK_HP] += AddValueByValueType(stDT_BATTLE_ATTRIBUTE.dwHP, dwValue, CJewelryPropMgr::Instance()->GetValueType(byJewelryExtID));						//(( 100 + dwValue ) * stDT_BATTLE_ATTRIBUTE.dwHP / 100);
            m_mapJewelryValue[EBAK_ATTACK] += AddValueByValueType(stDT_BATTLE_ATTRIBUTE.dwAttack, dwValue, CJewelryPropMgr::Instance()->GetValueType(byJewelryExtID));
			m_mapJewelryValue[EBAK_DEFEND] += AddValueByValueType(stDT_BATTLE_ATTRIBUTE.dwDefend, dwValue, CJewelryPropMgr::Instance()->GetValueType(byJewelryExtID));

			poEquip->AddJewelryAttribute(EBAK_HP, AddValueByValueType(stDT_BATTLE_ATTRIBUTE.dwHP, dwValue, CJewelryPropMgr::Instance()->GetValueType(byJewelryExtID)));
            poEquip->AddJewelryAttribute(EBAK_ATTACK, AddValueByValueType(stDT_BATTLE_ATTRIBUTE.dwAttack, dwValue, CJewelryPropMgr::Instance()->GetValueType(byJewelryExtID)));
			poEquip->AddJewelryAttribute(EBAK_DEFEND, AddValueByValueType(stDT_BATTLE_ATTRIBUTE.dwDefend, dwValue, CJewelryPropMgr::Instance()->GetValueType(byJewelryExtID)));
		}

		poEquip = m_mapEquip.GetNextData();
	}
}

VOID CHero::AddStudyAttr(UINT8 byPos,  UINT32 dwValue, UINT8 byJewelryExtID)
{
	for(UINT8 byIdx = 0 ; byIdx < MAX_HERO_STUDY_GRID_NUM; byIdx++)
	{
		if ( (byPos == byIdx + 1) || 0 == byPos )
		{
			DT_STUDY_DATA& stDT_STUDY_DATA_X = m_astStudy[byIdx];
			if(0 == stDT_STUDY_DATA_X.byStudyColorKind) //没数据
			{
				continue;
			}

			//过虑掉重复数据
			BOOL bRepeat = FALSE;
			for(UINT8 byIdy = 0 ; byIdy < byIdx; byIdy++)
			{
				DT_STUDY_DATA& stDT_STUDY_DATA_Y = m_astStudy[byIdy];

				if (stDT_STUDY_DATA_Y.byStudyAttrKind1 == stDT_STUDY_DATA_X.byStudyAttrKind1 &&
					stDT_STUDY_DATA_Y.byStudyAttrKind2 == stDT_STUDY_DATA_X.byStudyAttrKind2)
				{
					bRepeat = TRUE;
					break;
				}
			}

			if (bRepeat)
			{
				continue;
			}

			UINT32 dwValue1 = 0;
			UINT32 dwValue2 = 0;
			CAwakenPropMgr2::Instance()->GetStudyAttrValue(stDT_STUDY_DATA_X.byStudyColorKind, stDT_STUDY_DATA_X.byStudyAttrKind1, stDT_STUDY_DATA_X.byStudyAttrKind2, stDT_STUDY_DATA_X.byStudyLevel, dwValue1, dwValue2);
			//增加饰品对卦象的影响（增伤，减伤除外）
			if ( stDT_STUDY_DATA_X.byStudyAttrKind1 != EBAK_INCHURTRATE && EBAK_DECHURTRATE != stDT_STUDY_DATA_X.byStudyAttrKind1)
			{
				m_mapJewelryValue[stDT_STUDY_DATA_X.byStudyAttrKind1] += AddValueByValueType(dwValue1, dwValue, CJewelryPropMgr::Instance()->GetValueType(byJewelryExtID));	
			}
			//增加饰品对卦象的影响（增伤，减伤除外）
			if ( stDT_STUDY_DATA_X.byStudyAttrKind2 != EBAK_INCHURTRATE && EBAK_DECHURTRATE != stDT_STUDY_DATA_X.byStudyAttrKind2)
			{
				m_mapJewelryValue[stDT_STUDY_DATA_X.byStudyAttrKind2] += AddValueByValueType(dwValue2, dwValue, CJewelryPropMgr::Instance()->GetValueType(byJewelryExtID));	
			}
		}
		
	}
}

UINT32 CHero::GetJewelryAttr(UINT32 dwKey)
{
	return m_mapJewelryAttr[dwKey];
}

UINT32 CHero::AddScienceLevel(UINT32 dwLevel)
{
	if(0 == dwLevel)
	{
		return dwLevel;
	}
	else
	{
		return dwLevel += m_mapJewelryAttr[EJEA_ALL_SCIENCE_LEVEL];
	}
}

BOOL CHero::GetJewelryStrengthInfo(DT_HERO_JEWELRY_STRENGTH_INFO& stIfno)
{
	stIfno.wHeroID = m_stDT_HERO_BASE_DATA.wKindID;


	for ( UINT8 byIdx = EJDP_NECKLACE; byIdx <= EJDP_Jade ; byIdx++)
	{
		CJewelry* poJewelry = m_mapJewelryPos.GetData(byIdx);
		if ( NULL != poJewelry)
		{
			poJewelry->GetJewelryStrength(stIfno.astJewelryList[byIdx - 1], stIfno.astStrengthJewelryList[byIdx - 1]);
			if ( !poJewelry->GetStrengItemInfo( stIfno.astStringth[byIdx - 1]))
			{
				stIfno.abyMaxMark[byIdx - 1] = ESM_YES;
			}
		}
	}

	return TRUE;
}

CJewelry* CHero::GetJewelryByPos(UINT8 byPosID)
{
	return m_mapJewelryPos.GetData(byPosID);
}

VOID CHero::GetHeroPanelInfo(DT_HERO_PANEL_CLI& stHeroCli)
{
	stHeroCli.dwPower = GetPower();
	stHeroCli.stBattleAttribute = CBattleMgr::Instance()->ConvertAttribute2ToBase(GetDT_BATTLE_ATTRIBUTE());
	stHeroCli.stHideBattleAttribute = GetDT_HIDE_BATTLE_ATTRIBUTE();
	stHeroCli.wKindID = m_stDT_HERO_BASE_DATA.wKindID;
	stHeroCli.byPos = m_stDT_HERO_BASE_DATA.byPos;
	stHeroCli.wLevel = m_stDT_HERO_BASE_DATA.wLevel;
	stHeroCli.wTalent = GetTalent(); //包括丹药的
	//经验下发差值
	GetExperience(stHeroCli.dwExperience, stHeroCli.dwUpgradeExperience);
}

VOID CHero::OnDeleteJewelry(CJewelry* poJewelry)
{	
	if (NULL == poJewelry)
	{
		return;
	}

	//清除旧装备记录
	m_mapJewelry.DelData(poJewelry->GetMemID());//ID不会相同，则可以直接清除
	UINT8 byPos = EJDP_NECKLACE;
	SJewelryProp* poJewelryProp = poJewelry->GetProp();
	if ( NULL != poJewelryProp)
	{
		byPos = poJewelryProp->byPosKindID;
	}
	if(m_mapJewelryPos.GetData(byPos) == poJewelry)
	{
		m_mapJewelryPos.DelData(byPos);
	}
}

CSoul* CHero::GetEquipSoul(UINT16 usSoulID, UINT32 unIndex) {
	UINT64 ullKey = CLogicFunc::Get3216Key(UINT32(usSoulID), UINT16(unIndex));
	map<UINT64, CSoul>::iterator itor = m_mpSoul.find(ullKey);
	if (m_mpSoul.end() != itor) {
		return &itor->second;
	}

	return NULL;
}

BOOL CHero::IsSoulAllEquip() {
	for (size_t n = 0; n < MAX_SOUL_EQUIP_NUM; ++n) {
		if (0 == m_HeroSoulData.astSoulLstData[n].wSoulID) {
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CHero::IsHaveSameSoulType(CSoul* pSoul) {
	if (NULL == pSoul) {
		return FALSE;
	}

	for (size_t n = 0; n < MAX_SOUL_EQUIP_NUM; ++n) {
		if (pSoul->GetDT_SOUL_DATA().bySoulType == m_HeroSoulData.astSoulLstData[n].bySoulType) {
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CHero::EquipSoul(CSoul* pSoul) {
	if (NULL == pSoul || MAX_SOUL_EQUIP_NUM == m_HeroSoulData.wSoulNum) {
		return FALSE;
	}

	if (IsSoulAllEquip()) {
		return FALSE;
	}

	DT_SOUL_DATA& rSouData = pSoul->GetDT_SOUL_DATA();
	for (size_t n = 0; n < MAX_SOUL_EQUIP_NUM; ++n) {
		if (0 == m_HeroSoulData.astSoulLstData[n].wSoulID) {
			m_HeroSoulData.astSoulLstData[n] = rSouData;
			m_HeroSoulData.astSoulPotencyLstData[n] = pSoul->GetDT_SOUL_POTENCY_DATA();
			++ m_HeroSoulData.wSoulNum;

			CSoul& soul = m_mpSoul[CLogicFunc::Get3216Key(rSouData.wSoulID, rSouData.dwSoulIdx)];
			soul.Init(m_poOwner, m_HeroSoulData.astSoulLstData[n], m_HeroSoulData.astSoulPotencyLstData[n], this);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CHero::UnDressSoul(CSoul* pSoul) {
	if (NULL == pSoul || 0 >= m_HeroSoulData.wSoulNum) {
		return FALSE;
	}

	DT_SOUL_DATA rSoulData = pSoul->GetDT_SOUL_DATA();
	for (size_t n = 0; n < MAX_SOUL_EQUIP_NUM; ++n) {
		if (m_HeroSoulData.astSoulLstData[n].wSoulID == rSoulData.wSoulID 
			&& m_HeroSoulData.astSoulLstData[n].dwSoulIdx == rSoulData.dwSoulIdx )
		{
			for (size_t nIdx = n; nIdx < MAX_SOUL_EQUIP_NUM; ++nIdx) {
				if (nIdx != (MAX_SOUL_EQUIP_NUM - 1)) {
					memcpy(&m_HeroSoulData.astSoulLstData[nIdx], &m_HeroSoulData.astSoulLstData[nIdx + 1], sizeof(DT_SOUL_DATA));
					memcpy(&m_HeroSoulData.astSoulPotencyLstData[nIdx], &m_HeroSoulData.astSoulPotencyLstData[nIdx + 1], sizeof(DT_SOUL_POTENCY_DATA));
				} else {
					memset(&m_HeroSoulData.astSoulLstData[nIdx], 0, sizeof(DT_SOUL_DATA));
					memset(&m_HeroSoulData.astSoulPotencyLstData[nIdx], 0, sizeof(DT_SOUL_POTENCY_DATA));
				}
			}

			-- m_HeroSoulData.wSoulNum;
			UINT64 ullKey = CLogicFunc::Get3216Key(UINT32(rSoulData.wSoulID), UINT16(rSoulData.dwSoulIdx));
			map<UINT64, CSoul>::iterator itor = m_mpSoul.find(ullKey);
			if (m_mpSoul.end() != itor) {
				m_mpSoul.erase(itor);
			}
			return TRUE;
		}
	}
	return FALSE;
}

VOID CHero::UpdateSoulData(CSoul* pSoul) {
	if (NULL == pSoul || 0 >= m_HeroSoulData.wSoulNum) {
		return;
	}

	const DT_SOUL_DATA& rSoulData = pSoul->GetDT_SOUL_DATA();
	for (size_t n = 0; n < MAX_SOUL_EQUIP_NUM; ++n) {
		if (m_HeroSoulData.astSoulLstData[n].wSoulID == rSoulData.wSoulID
			&& m_HeroSoulData.astSoulLstData[n].dwSoulIdx == rSoulData.dwSoulIdx)
		{
			m_HeroSoulData.astSoulLstData[n] = rSoulData;
			m_HeroSoulData.astSoulPotencyLstData[n] = pSoul->GetDT_SOUL_POTENCY_DATA();
			return;
		}
	}
}

const DT_HERO_SOUL_DATA& CHero::GetHeroSoulData() {
	//数值错乱修正
	m_HeroSoulData.wSoulNum = m_HeroSoulData.wSoulNum > MAX_SOUL_EQUIP_NUM ? MAX_SOUL_EQUIP_NUM : m_HeroSoulData.wSoulNum;
	return m_HeroSoulData;
}

VOID CHero::InitSoulData() {
	CPlayerSoul* pPlayerSoul = m_poOwner->GetPlayerSoul();
	if (NULL == pPlayerSoul) {
		return;
	}
	m_mpSoul.clear();
	for (size_t n = 0; n < m_HeroSoulData.wSoulNum && n < MAX_SOUL_EQUIP_NUM; ++n) {
		DT_SOUL_DATA& rSoulData = m_HeroSoulData.astSoulLstData[n];
		if (0 != rSoulData.wSoulID) {
			rSoulData.dwSoulIdx = pPlayerSoul->GenerateSoulIdx(rSoulData.wSoulID);
			CSoul& soul = m_mpSoul[CLogicFunc::Get3216Key(rSoulData.wSoulID, rSoulData.dwSoulIdx)];
			soul.Init(m_poOwner, rSoulData, m_HeroSoulData.astSoulPotencyLstData[n], this);
			//修正
			rSoulData = soul.GetDT_SOUL_DATA();
		}
	}
}

VOID CHero::ResetSpecialEffects()
{
	m_bPhysicalImmunity = FALSE;
	m_bMagicImmunity = FALSE;
	m_bContinueAttack = NULL;
	m_bBeatBack = NULL;
	m_mapSpecialEffect.clear();

	for ( UINT16 wIdx = 0; wIdx < m_HeroSoulData.wSoulNum && wIdx < MAX_SOUL_EQUIP_NUM; wIdx++)
	{
		DT_SPECIAL_EFFECT_DATA& stSpecialEffectData = m_HeroSoulData.astSoulLstData[wIdx].stSpecialEffect;
		const SoulBaseCfg* poBaseProp = CSoulPropMgr::Instance()->GetSoulBaseCfg( m_HeroSoulData.astSoulLstData[wIdx].wSoulID );
		UINT16 wExtLimitNum = 0;
		UINT16 wExtEffectPercent = 0;
		UINT16 wExtTrigerProb = 0;
		if ( NULL != poBaseProp)
		{
			C3232Map AttrMap;
			if( CSoulPropMgr::Instance()->GetSoulPotencyCountAttr( m_HeroSoulData.astSoulLstData[wIdx].wSoulID, 
				m_HeroSoulData.astSoulPotencyLstData[wIdx].byPotencyLevel, 
				m_HeroSoulData.astSoulPotencyLstData[wIdx].byPotencySubLevel, AttrMap) )
			{
				wExtLimitNum = AttrMap[ESP_SKILL_ROUND];
				wExtEffectPercent = AttrMap[ESP_SKILL_EFFECT];
				wExtTrigerProb = AttrMap[ESP_SKILL_PROBABILITY];
			}
		}
		for ( UINT8 byIdx = 0; byIdx < stSpecialEffectData.bySkillNum && byIdx < MAX_SKILL_SOUL_NUM; byIdx++)
		{
			CSpecialEffect* poSpecialEffect = CSpecialEffectPropMgr::Instance()->CreateSpecialEffect( m_poOwner, this, stSpecialEffectData, byIdx);
			if ( NULL != poSpecialEffect)
			{
				poSpecialEffect->SetExtLimit(wExtLimitNum);
				poSpecialEffect->SetExtEffectPercent(wExtEffectPercent);
				poSpecialEffect->SetExtTrigerProb(wExtTrigerProb);
				m_mapSpecialEffect[CLogicFunc::Get1616Key(stSpecialEffectData.wEffectID, stSpecialEffectData.abyEffectSmallType[byIdx])] = poSpecialEffect;
			}
		}
	}
	for ( CMapID2SpecialEffectItr itr = m_mapSpecialEffect.begin(); itr != m_mapSpecialEffect.end(); itr++)
	{
		itr->second->ResetData();
	}
}

BOOL CHero::IsShowFly()
{
    if (NULL == m_poOwner->GetBuild(EBK_FLY)){
        return FALSE;
    }

    if (IsFly()){
        return FALSE;
    }

    // 伙伴如果低于115级则不显示飞升按钮
    if (GetLevel() < 115){
        return FALSE;
    }

    return TRUE;
}

BOOL CHero::IsFly()
{
    return m_stDT_HERO_BASE_DATA.wLevel > FLY_LEVEL && m_poOwner->GetBuild(EBK_FLY);
}

BOOL CHero::ResetFly()
{
    SetLevel(FLY_LEVEL);
    return TRUE;
}

UINT16 CHero::Fly(PKT_CLIGS_FLY_ACK &stAck)
{
    if (NULL == m_poOwner->GetBuild(EBK_FLY)){
        return ERR_FLY::ID_IS_UNOPEN;
    }

    // 是否已飞升
    if (IsFly()){
        return ERR_FLY::ID_SUCCESS;
    }

    SHeroProp *pstHeroProp = GetHeroProp();
    if (NULL == pstHeroProp){
        return ERR_FLY::ID_IS_UNOPEN;
    }

    const SFlyProp &stFlyProp = CFlyPropMgr::Instance()->GetFlyProp();
    if (!IsCoachHero()){
        // 必须等主角飞升后伙伴才能飞升
        CHero *poCoachHero = m_poOwner->GetCoachHero();
        if (!poCoachHero->IsFly()){
            return ERR_FLY::ID_COACH_NOT_FLY;
        }
    }

    // 检测主角等级和境界等级
    if (IsCoachHero()){
        if(NULL == m_poOwner->GetJingJie()){
            return ERR_FLY::ID_JINGJIE_LEVEL_ERR;
        }

        if(m_poOwner->GetJingJie()->GetJingJieLevel() < FLY_REQUIRE_JINGJIE_LVL){
            return ERR_FLY::ID_JINGJIE_LEVEL_ERR;
        }
    }

    if (GetLevel() < FLY_LEVEL){
        return ERR_FLY::ID_LEVEL_ERR;
    }

    // 检测飞升材料是否足够
    DT_MATERIAL_LIST stFlyCostMaterials;
    CEquipMakeMgr::TakeMaterialListCli(*m_poOwner, pstHeroProp->vecFlyMaterial, stFlyCostMaterials);

    for (UINT16 i = 0; i < stFlyCostMaterials.byMaterialCnt; i++){
        DT_MATERIAL &stMaterial = stFlyCostMaterials.astMaterialList[i];
        if (stMaterial.wCurNum < stMaterial.wRequiredNum){
            return ERR_FLY::ID_MATERIAL_NOT_ENOUGH;
        }
    }

    BOOL bCostSucc = CEquipMakeMgr::CostMaterialList(*m_poOwner, stFlyCostMaterials, CRecordMgr::EDIT_FLY);
    if (!bCostSucc){
        return ERR_FLY::ID_MATERIAL_NOT_ENOUGH;
    }

    m_poOwner->InitBattleAttr(TRUE);
    stAck.stPowerChange.dwOldPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;

    // 开始飞升：等级强制设置为121级
    SetLevel(FLY_LEVEL + 1);

    // 将之前积攒的飞升经验加上来
    AddExperience(m_stDT_HERO_BASE_DATA.dwFlyExp);

    m_poOwner->InitBattleAttr(TRUE);
    stAck.stPowerChange.dwCurPower = m_poOwner->GetDT_PLAYER_BASE_DATA().dwPower;

    stAck.wHeroID = GetHeroID();
    stAck.wHeroLvl = GetLevel();
    m_poOwner->GetCoachHero()->GetExperienceInfo(stAck.stCoachHeroExpInfo);

    CRecordMgr::Instance()->RecordInfo(m_poOwner->GetPlayerID(), ERM_FLY, CRecordMgr::EF_FLY, GetHeroID(), 0, 
        GetLevel(), m_poOwner->GetDT_PLAYER_BASE_DATA().byVipLevel, stAck.stPowerChange.dwOldPower, stAck.stPowerChange.dwCurPower, m_stDT_HERO_BASE_DATA.dwFlyExp, 0);

    // 清空飞升经验
    m_stDT_HERO_BASE_DATA.dwFlyExp = 0;
    return ERR_FLY::ID_SUCCESS;
}
