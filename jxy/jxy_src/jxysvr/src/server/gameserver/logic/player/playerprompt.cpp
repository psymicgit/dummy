#include "player.h"
#include <logic/race/racepropmgr.h>
#include <logic/moneytree/moneytreepropbasemgr.h>
#include <logic/item/equip/strengthenpropmgr.h>
#include "logic/item/equip/equipmakemgr.h"
#include "logic/crop/cropseedpropmgr.h"
#include "logic/crop/cropfieldpropmgr.h"
#include "logic/other/singleprammgr.h"
#include <logic/vip/vippropmgr.h>
#include <logic/activity/eatbreadpropmgr.h>
#include <logic/lua/luamgr.h>
#include <logic/science/scienceupdatepropmgr.h>
#include <logic/hero/heropropmgr.h>
#include <logic/jingjie/jingjiepropmgr.h>
#include <logic/plunder/plunder.h>
#include <logic/build/build.h>
#include <logic/activity/eatbread.h>
#include <logic/activity/bossbattle.h>
#include <logic/crop/crop.h>
#include <logic/activity/courage.h>

#include <protocligs.h>
#include <logic/other/worship.h>
#include <logic/other/rdchallenge.h>
#include "logic/build/build.h"
#include "logic/hero/hero.h"
#include "logic/race/race.h"
#include "logic/yabiao/yabiaoinfo.h"
#include "logic/jingjie/jingjie.h"
#include "logic/activity/luaactivity.h"
#include "logic/moneytree/moneytree.h"
#include "logic/crop/crop.h"
#include "logic/science/sciencetree.h"
#include "logic/activity/luaactivity.h"
#include "logic/activityencourage/activityencourage.h"
#include "logic/moneytree/moneytree.h"
#include "logic/climbtower/climbtower.h"
#include <logic/event/huntingevent.h>
#include <logic/faction/factionauthorityprop.h>
#include <logic/faction/factionprop.h>
#include "logic/formation/formation.h"
#include "logic/formation/formationmgr.h"

VOID CPlayer::GetPrompt(DT_FUNC_PROMPT_DATA& stPromptInfo)
{
    memset(&stPromptInfo, 0, sizeof(stPromptInfo));
    GetMaintownPrompt1(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]);
    GetMaintownPrompt2(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]);
    GetSmithyPrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]);
    GetRecruitPrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]);
    GetPVPPrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]);
    GetGodweaponPrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]);
    GetMoneyTreePrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]);
    GetCropPrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]);
    GetSciencePrompt( stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++] );
    GetPlunderPrompt( stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++] );
    GetEmprisePrompt( stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++] );
    GetActivityCenterPrompt( stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++] );
    GetJingJiePrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]);
    GetYabiaoPrompt( stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++] );
    GetActivityEncouragePrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]);
    GetFactionPrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]);
	GetJewelryPrompt(stPromptInfo.astBuildPromptInfo[stPromptInfo.byBuildNum++]);
}


//获取主城提示
VOID CPlayer::GetMaintownPrompt1(DT_FUNC_COMMON_PROMPT& stMaintownPrompt)
{
    memset(&stMaintownPrompt, 0, sizeof(stMaintownPrompt));
    stMaintownPrompt.byBuildKindID = EBK_MAINTOWN;
    CBuild* poBuild = GetBuild(EBK_MAINTOWN);
    if(NULL == poBuild)
    {
        return;
    }
    DT_BUILD_BASE_DATA&  stDT_BUILD_BASE_DATA = poBuild->GetDT_BUILD_BASE_DATA();
    //今天未经收获
    if(!IsToday(stDT_BUILD_BASE_DATA.qwValue1))
    {
        stMaintownPrompt.byFuncFlag = 1;
    }
}


//获取主城提示
VOID CPlayer::GetMaintownPrompt2(DT_FUNC_COMMON_PROMPT& stMaintownPrompt)
{
    memset(&stMaintownPrompt, 0, sizeof(stMaintownPrompt));
    stMaintownPrompt.byBuildKindID = EBK_LEVY;
    CBuild* poBuild = poBuild = GetBuild(EBK_LEVY);
    if(NULL == poBuild)
    {
        return;
    }

    DT_BUILD_BASE_DATA&  stDT_BUILD_BASE_DATAEX = poBuild->GetDT_BUILD_BASE_DATA();
    //今天未征收
    if(!IsToday(stDT_BUILD_BASE_DATAEX.qwValue2))
    {
        stDT_BUILD_BASE_DATAEX.byValue3 = 0; //重新赋值征收次数
    }
    UINT8 byLevyNum = (m_pstMaintownLevyProp->byLevyNumPerDay > stDT_BUILD_BASE_DATAEX.byValue3) ? m_pstMaintownLevyProp->byLevyNumPerDay - stDT_BUILD_BASE_DATAEX.byValue3 : 0;
    if(byLevyNum > 0)
    {
        stMaintownPrompt.byFuncFlag = 1;
        stMaintownPrompt.dwFuncCountDown = GetLevyCountDown();
    }
}

//获取铁匠铺提示
VOID CPlayer::GetSmithyPrompt(DT_FUNC_COMMON_PROMPT& stSmithyPrompt)
{
    if (GetCliVer() >= VERSION_FLY){
        GetEquipMakePrompt(stSmithyPrompt);
        if (stSmithyPrompt.byFuncFlag > 0){
            stSmithyPrompt.byBuildKindID = EBK_SMITHY;
            stSmithyPrompt.byFuncFlag = ESPID_EQUIP_MAKE;
            return;
        }

        GetEnchantPrompt(stSmithyPrompt);
        if (stSmithyPrompt.byFuncFlag > 0){
            stSmithyPrompt.byBuildKindID = EBK_SMITHY;
            stSmithyPrompt.byFuncFlag = ESPID_ENCHANT;
            return;
        }

        GetStrengthenPrompt(stSmithyPrompt);
        if (stSmithyPrompt.byFuncFlag > 0){
            stSmithyPrompt.byBuildKindID = EBK_SMITHY;
            stSmithyPrompt.byFuncFlag = ESPID_STRENGTHEN;
            return;
        }
    }else{
        GetStrengthenPrompt(stSmithyPrompt);
    }
}

VOID CPlayer::GetStrengthenPrompt(DT_FUNC_COMMON_PROMPT& stStrengthenPrompt)
{
    memset(&stStrengthenPrompt, 0, sizeof(stStrengthenPrompt));
    stStrengthenPrompt.byBuildKindID = EBK_SMITHY;
    UINT16 wPlayerLevel = GetLevel();
    CHero* poHero = m_mapFormationHero.GetFistData();
    CC_LOOP_DO
        if((NULL == poHero) || (0 != stStrengthenPrompt.byFuncFlag))
        {
            break;
        }
        CEquipMap& mapEquip = poHero->GetEquipMap();
        CEquip* poEquip = mapEquip.GetFistData();
        while(poEquip)
        {
            if(poEquip->GetDT_EQUIP_DATA().wLevel < wPlayerLevel)
            {
                stStrengthenPrompt.byFuncFlag = 1;
                return;
                break;
            }
            poEquip = mapEquip.GetNextData();
        }
        CC_LOOP_WHILE(poHero =  m_mapFormationHero.GetNextData())
}

VOID CPlayer::GetEnchantPrompt(DT_FUNC_COMMON_PROMPT& stEnchantPrompt)
{
    memset(&stEnchantPrompt, 0, sizeof(stEnchantPrompt));
    stEnchantPrompt.byBuildKindID = EBK_ENCHANT;

    if (NULL == GetBuild(EBK_ENCHANT)){
        return;
    }

    // 检测是否有装备可被附魔
    UINT16 wPlayerLevel = GetLevel();
    for(CHero* poHero = m_mapFormationHero.GetFistData(); poHero; poHero = m_mapFormationHero.GetNextData()){
        CEquipMap& mapEquip = poHero->GetEquipMap();

        for(CEquip* poEquip = mapEquip.GetFistData(); poEquip; poEquip = mapEquip.GetNextData()){
            if(poEquip->CanBeEnchant()){
                stEnchantPrompt.byFuncFlag = 1;
                return;
            }
        }
    }
}

VOID CPlayer::GetEquipMakePrompt(DT_FUNC_COMMON_PROMPT& stEquipMakePrompt)
{
    memset(&stEquipMakePrompt, 0, sizeof(stEquipMakePrompt));
    stEquipMakePrompt.byBuildKindID = EBK_EQUIP_MAKE;

    CBag &oBag = GetBag();

    CID2GoodsShmemMap& mapGoods  = oBag.GetGoodsMap();
    CID2EquipShmemMap& mapEquips = oBag.GetEquipMap();

    typedef std::vector<CGoods*> GoodsVec;

    // 搜寻制作卷，检测是否可被制作
    GoodsVec vecBook;
    for(CGoods* poGoods = mapGoods.GetFistData(); poGoods; poGoods = mapGoods.GetNextData()){
        if (EGMK_EQUIP_MAKE_BOOK == poGoods->GetGoodsMainKindID()){
            vecBook.push_back(poGoods);
        }
    }

    for(UINT16 i = 0; i < vecBook.size(); i++){
        CGoods* poBook = vecBook[i];
        const SEquipMakeProp* pstProp = CEquipMakePropMgr::Instance()->GetMakeProp(poBook->GetItemKindID());
        if (NULL == pstProp){
            continue;
        }

        DT_EQUIP_MAKE_BOOK stBookCli;
        CEquipMakeMgr::TakeEquipMakeBookInfo(*this, poBook->GetItemKindID(), stBookCli);

        if (stBookCli.byIsMakable){
            stEquipMakePrompt.byFuncFlag = 1;
            return;
        }
    }
}

//获取招募馆提示信息
VOID CPlayer::GetRecruitPrompt(DT_FUNC_COMMON_PROMPT& stRecruitPrompt)
{
    memset(&stRecruitPrompt, 0, sizeof(stRecruitPrompt));
    stRecruitPrompt.byBuildKindID = EBK_RECRUIT;
    CBuild* poBuild = GetBuild(EBK_RECRUIT);
    if(NULL == poBuild){
        return;
    }
    
    DT_BUILD_BASE_DATA &stBuild = poBuild->GetDT_BUILD_BASE_DATA();
    if (stBuild.qwValue1 > 0){
        stRecruitPrompt.byFuncFlag = RPID_UPGRADE; // 可进阶
        return;
    }
    
    //对于招募馆，则表示是否存在新伙伴可招募
    if(stBuild.byValue3 > 0){
        stRecruitPrompt.byFuncFlag = RPID_RECRUIT; // 可招募
    }
}


//比武提示信息
VOID CPlayer::GetPVPPrompt(DT_FUNC_COMMON_PROMPT& stPvpPrompt)
{
    memset(&stPvpPrompt, 0, sizeof(&stPvpPrompt));
    if(NULL != GetBuild(EBK_RACEBUILD))
    {
        if(NULL != m_poRace && m_poRace->GetPrompt(stPvpPrompt))
        {
            if(EPP_RECVENC != stPvpPrompt.byFuncFlag)
            {
                DT_FUNC_COMMON_PROMPT stTmp = {0};
                if(NULL != m_poWorship && m_poWorship->GetPrompt(stTmp))
                {
                    memcpy(&stPvpPrompt, &stTmp, sizeof(DT_FUNC_COMMON_PROMPT));
                    return;
                }
            }
            return;
        }
    }

    if(NULL != GetBuild(EBK_RDCHALLENGE) && NULL != m_poRdChallenge)
    {
        if(m_poRdChallenge->GetPrompt(stPvpPrompt))
        {
            return;
        }
        return;
    }
}

//神器提示信息
VOID CPlayer::GetGodweaponPrompt(DT_FUNC_COMMON_PROMPT& stGodweaponPrompt)
{
    memset(&stGodweaponPrompt, 0, sizeof(stGodweaponPrompt));
    stGodweaponPrompt.byBuildKindID = EBK_GODWEAPON;
    if(NULL == GetBuild(EBK_GODWEAPON))
    {
        return;
    }
    CHero* poHero = NULL;
    CGodWeapon *pGodWeapon = NULL;

    //todo优化
    poHero = m_mapFormationHero.GetFistData();
    CC_LOOP_DO
    CC_LOOP_BREAK(NULL == poHero)
    pGodWeapon = poHero->GetGodweapon();
    if(NULL == pGodWeapon)
    {
        stGodweaponPrompt.byFuncFlag = 1;//可激活
        return;
    }
    CC_LOOP_WHILE(poHero = m_mapFormationHero.GetNextData())


    poHero = m_mapFormationHero.GetFistData();
    CC_LOOP_DO
    CC_LOOP_BREAK(NULL == poHero)
    pGodWeapon = poHero->GetGodweapon();
    CC_LOOP_CONTINUE(NULL == pGodWeapon)
    DT_GODWEAPON_DATA& stDT_GODWEAPON_DATA = pGodWeapon->GetDT_GODWEAPON_DATA();
    UINT64 qwCostScience = CGodweaponPropMgr::Instance()->GetNextUpgradeScience(stDT_GODWEAPON_DATA.wHeroKindID, stDT_GODWEAPON_DATA.wQuality, stDT_GODWEAPON_DATA.wLevel);
    if(m_stDT_PLAYER_BASE_DATA.qwScience >= qwCostScience)
    {
        stGodweaponPrompt.byFuncFlag = 2; //可升级
    }
    CC_LOOP_WHILE(poHero = m_mapFormationHero.GetNextData())

}

//招财树提示信息
VOID CPlayer::GetMoneyTreePrompt(DT_FUNC_COMMON_PROMPT& stMoneyTreePrompt)
{
    memset(&stMoneyTreePrompt, 0, sizeof(stMoneyTreePrompt));
    stMoneyTreePrompt.byBuildKindID = EBK_MONEYTREE;
    if(NULL == m_poMoneyTree)
    {
        return;
    }
    DT_MONEY_TREE_DATA& stDT_MONEY_TREE_DATA = m_poMoneyTree->GetDT_MONEY_TREE_DATA();
    //今天
    if(IsToday(stDT_MONEY_TREE_DATA.qwRockTime))
    {
        UINT16 wTotalRockNum = CVipPropMgr::Instance()->GetIncNum(EVINF_MONEYTREE, m_stDT_PLAYER_BASE_DATA.byVipLevel);
        if(stDT_MONEY_TREE_DATA.wRockNum < wTotalRockNum)
        {
            stMoneyTreePrompt.byFuncFlag = 1;
        }
    }
    else
    {
        stMoneyTreePrompt.byFuncFlag = 1;
    }
}

VOID CPlayer::GetCropPrompt(DT_FUNC_COMMON_PROMPT& stDT_CROP_PROMPT) //种植提示信息
{
    memset(&stDT_CROP_PROMPT, 0, sizeof(stDT_CROP_PROMPT));
    stDT_CROP_PROMPT.byBuildKindID = EBK_CROP;
    if(NULL == m_poCrop)
        return;

    //检测是否可收获
    //////////////////////////////////////////////////////////////////////////
    DT_CROP_DATA &stDT_CROP_DATA = m_poCrop->GetDT_CROP_DATA();
    SCropProp * pCropProp = NULL;
    stDT_CROP_PROMPT.dwFuncCountDown = 0xFFFFFFFF;
    for (int i = 0; i < MAX_CROP_NUM; i++)
    {
        if (stDT_CROP_DATA.astCropInfoList[i].byOpen && stDT_CROP_DATA.astCropInfoList[i].wCropKindID > 0)
        {
            pCropProp = CCropSeedPropMgr::Instance()->GetCropProp(m_poCrop->GetDT_CROP_DATA().astCropInfoList[i].wCropKindID);
            if (NULL == pCropProp)
            {
                continue;
            }

            //计算经过的时间
            UINT64 qwDiffSecond =	SDTimeSecs() - stDT_CROP_DATA.astCropInfoList[i].qwLastCropTime;
            qwDiffSecond = pCropProp->wObtainCD > qwDiffSecond ? (pCropProp->wObtainCD - qwDiffSecond) : 0;
            if(0 == qwDiffSecond)
            {
                stDT_CROP_PROMPT.byFuncFlag = 1; //可收获
                stDT_CROP_PROMPT.dwFuncCountDown = 0;
                return;
            }

            if(qwDiffSecond < stDT_CROP_PROMPT.dwFuncCountDown)
            {
                stDT_CROP_PROMPT.byFuncFlag = 1; //可收获
                stDT_CROP_PROMPT.dwFuncCountDown = (UINT32)qwDiffSecond;
            }
        }
    }

    //检测地块是否可种植
    //////////////////////////////////////////////////////////////////////////
    CCropFieldPropMapItr itr;
    for (int i = 0; i < MAX_CROP_NUM; i++)
    {
        if (stDT_CROP_DATA.astCropInfoList[i].byOpen && stDT_CROP_DATA.astCropInfoList[i].wCropKindID == 0)
        {
            stDT_CROP_PROMPT.byFuncFlag = 2; //可种植
            return;
        }
    }
}

VOID CPlayer::GetSciencePrompt(DT_FUNC_COMMON_PROMPT& stDT_SCIENCE_PROMPT)
{
    memset(&stDT_SCIENCE_PROMPT, 0, sizeof(stDT_SCIENCE_PROMPT));
    UINT64 qwStory = m_stDT_PLAYER_BASE_DATA.qwStory;
    stDT_SCIENCE_PROMPT.byBuildKindID = EBK_SCIENCE;
    UINT16 wLevel = GetLevel();
    BOOL bCanUpdate = TRUE;

    if (NULL == m_poScienceTree)
    {
        return;
    }

    //DBG_INFO( "科技提示信息开始获得" );
    if( IsScienceCanGet( m_poScienceTree->GetDT_SCIENCE_TREE_DATA().wAttractLevel + 1, EBAK_ATTACK ) )
    {
        stDT_SCIENCE_PROMPT.byFuncFlag = 1; //可升级
        return;
    }

    if( IsScienceCanGet( m_poScienceTree->GetDT_SCIENCE_TREE_DATA().wHPLevel + 1, EBAK_HP ) )
    {
        stDT_SCIENCE_PROMPT.byFuncFlag = 1; //可升级
        return;
    }

    if( IsScienceCanGet( m_poScienceTree->GetDT_SCIENCE_TREE_DATA().wCritLevel + 1, EBAK_CRITRATE ) )
    {
        stDT_SCIENCE_PROMPT.byFuncFlag = 1; //可升级
        return;
    }

    if( IsScienceCanGet( m_poScienceTree->GetDT_SCIENCE_TREE_DATA().wDodgeLevel + 1, EBAK_DODGERATE ) )
    {
        stDT_SCIENCE_PROMPT.byFuncFlag = 1; //可升级
        return;
    }

    if( IsScienceCanGet( m_poScienceTree->GetDT_SCIENCE_TREE_DATA().wHitLevel + 1, EBAK_HITRATE ) )
    {
        stDT_SCIENCE_PROMPT.byFuncFlag = 1; //可升级
        return;
    }

    if( IsScienceCanGet( m_poScienceTree->GetDT_SCIENCE_TREE_DATA().wShieldLevel + 1, EBAK_DECRITRATE ) )
    {
        stDT_SCIENCE_PROMPT.byFuncFlag = 1; //可升级
        return;
    }

    if( IsScienceCanGet( m_poScienceTree->GetDT_SCIENCE_TREE_DATA().dwFirstAttack + 1, EBAK_FIRSTATTACK ) )
    {
        stDT_SCIENCE_PROMPT.byFuncFlag = 1; //可升级
        return;
    }

    if( IsScienceCanGet( m_poScienceTree->GetDT_SCIENCE_TREE_DATA().wAttractLevel2 + 1, EBAK_ATTACK2 ) )
    {
        stDT_SCIENCE_PROMPT.byFuncFlag = 1; //可升级
        return;
    }

    if( IsScienceCanGet( m_poScienceTree->GetDT_SCIENCE_TREE_DATA().wHPLevel2 + 1, EBAK_HP2 ) )
    {
        stDT_SCIENCE_PROMPT.byFuncFlag = 1; //可升级
        return;
    }
	if ( CFormationMgr::Instance()->CheckConvertFormation(this))
	{
		CFormation& oFormation = GetFormation();
		DT_NEW_FORMATION_DATA& stFormationData = oFormation.GetDT_FORMATION_DATA();
		for ( UINT8 byIndex = 0; byIndex < stFormationData.stNewFormationLst.byFormationNum; byIndex++)
		{
			DT_FORMATION_DATA_LEVEL stLevel = {0};
			oFormation.GetFormationLevel(stLevel, stFormationData.stNewFormationLst.astFormationArray[byIndex].byFormationID, stFormationData.stNewFormationLst.astFormationArray[byIndex].byFormationLevel);
			if ( 0 == stLevel.byIsUpgrade)
			{
				stDT_SCIENCE_PROMPT.byFuncFlag = 1; //可升级
				return ;
			}
		}
	}
	
}

//掠夺提示信息
VOID CPlayer::GetPlunderPrompt(DT_FUNC_COMMON_PROMPT& stPlunderPrompt)
{
    memset(&stPlunderPrompt, 0, sizeof(stPlunderPrompt));
    stPlunderPrompt.byBuildKindID = EBK_PLUNDER;
    if(m_poPlunder)
    {
        if((m_poPlunder->GetCanPlunderNum() > 0) && (m_poPlunder->HaveIdleGrid()))
        {
            stPlunderPrompt.byFuncFlag = 1;
        }
    }
}

BOOL CPlayer::IsScienceCanGet( UINT16 wAppointScienceLevel, UINT8 byScienceKind )
{
    return CScienceUpdatePropMgr::Instance()->IsCanUpdateAttr(
               m_stDT_PLAYER_BASE_DATA.qwStory, wAppointScienceLevel, GetLevel(), byScienceKind );
}


BOOL CPlayer::HaveBetterHero()
{
    //伙伴数目不是很多，可使用遍历方式计算是否有更强的伙伴可招募
    UINT16 wMinCarryHeroTalent = 0xFFFF;
    CHero* poHero = m_mapKind2HeroAllCarry.GetFistData();
    while(poHero)
    {
        UINT16 wTalent = poHero->GetHeroProp()->wTalent;
        if(wTalent < wMinCarryHeroTalent)
        {
            wMinCarryHeroTalent = wTalent;
        }
        poHero = m_mapKind2HeroAllCarry.GetNextData();
    }
    poHero = m_mapKind2Hero.GetFistData();
    while(poHero)
    {
        UINT16 wTalent = poHero->GetHeroProp()->wTalent;
        if((EHP_RECRUIT == poHero->GetDT_HERO_BASE_DATA().byPos) && (wTalent > wMinCarryHeroTalent))
        {
            return TRUE;
        }
        poHero = m_mapKind2Hero.GetNextData();
    }

    return FALSE;
}


VOID CPlayer::GetNewEquipPrompt(DT_NEW_EQUIP_PROMPT_DATA& stNewEquipPrompt)
{
    memset(&stNewEquipPrompt, 0, sizeof(stNewEquipPrompt));
    UINT16 wPlayerLevel = GetLevel();
    UINT16 wMinHeroLevel = 0xFFFF;
    UINT16 wMinHeroID = 0;
    UINT16 wMinDressEquipLevel = 0xFFFF;
    //新手或总共才3个伙伴，遍历背包装备
    if((wPlayerLevel <= CSinglePramMgr::Instance()->GetJuniorLevel()) || (m_mapKind2Hero.Size() <= 3))
    {
        CID2ItemShmemMap& mapItem = m_oBag.GetItemMap();
        CItem* poItem = mapItem.GetFistData();
        CC_LOOP_DO
        CC_LOOP_BREAK(NULL == poItem)
        CC_LOOP_CONTINUE(EIK_EQUIP != poItem->GetItemKind())
        CkCanPromote(dynamic_cast<CEquip*>(poItem), stNewEquipPrompt, TRUE);
        CC_LOOP_WHILE(poItem = mapItem.GetNextData())
    }
    else
    {
        UINT8 bySize = m_vecNewEquip.size();
        for(UINT8 byIdx = 0; byIdx < bySize; byIdx++)
        {
            CkCanPromote(m_vecNewEquip[byIdx], stNewEquipPrompt, FALSE);
        }
    }

    m_vecNewEquip.clear();
}

BOOL CPlayer::CkCanPromote(CEquip* poEquip, DT_NEW_EQUIP_PROMPT_DATA& stNewEquipPrompt, BOOL bJunior)
{
    if(NULL == poEquip)
    {
        return FALSE;
    }

    //主将优先
    if(CkCanPromote(m_poCoachHero, poEquip, stNewEquipPrompt))
    {
        stNewEquipPrompt.wHeroID = m_poCoachHero->GetDT_HERO_BASE_DATA().wKindID;
        return TRUE;
    }

    //新手，遍历所有携带武将
    if(bJunior)
    {
        CHero* poHero = m_mapKind2HeroAllCarry.GetFistData();
        CC_LOOP_DO
        CC_LOOP_BREAK(NULL == poHero)
        CC_LOOP_CONTINUE(poHero == m_poCoachHero)
        if(CkCanPromote(poHero, poEquip, stNewEquipPrompt))
        {
            return TRUE;
        }
        CC_LOOP_WHILE(poHero = m_mapKind2HeroAllCarry.GetNextData());
    }
    else
    {
        CHero* poHero = m_mapFormationHero.GetFistData();
        CC_LOOP_DO
        CC_LOOP_BREAK(NULL == poHero)
        CC_LOOP_CONTINUE(poHero == m_poCoachHero)
        if(CkCanPromote(poHero, poEquip, stNewEquipPrompt))
        {
            return TRUE;
        }
        CC_LOOP_WHILE(poHero = m_mapFormationHero.GetNextData())
    }


    return FALSE;
}

BOOL CPlayer::CkCanPromote(CHero* poHero, CEquip* poEquip, DT_NEW_EQUIP_PROMPT_DATA& stNewEquipPrompt)
{
    SEquipProp* pstEquipProp = poEquip->GetEquipProp();
    if (NULL == pstEquipProp)
    {
        return FALSE;
    }
    UINT16 wEquipID = pstEquipProp->wKindID;
    UINT8 byPosKindID = pstEquipProp->byPosKindID;
    UINT16 wEquipDressLevel = pstEquipProp->wDressLevel;
    BOOL bPromote = FALSE;
    UINT16 wHeroLevel = poHero->GetDT_HERO_BASE_DATA().wLevel;
    CEquip* poHaveEquip = poHero->GetEquipByPos(byPosKindID);
    if(NULL == poHaveEquip)
    {
        if(wHeroLevel >= wEquipDressLevel)
        {
            bPromote = TRUE;
        }
        else
        {
            bPromote = FALSE;
        }
    }
    else
    {
        SEquipProp* poHaveEquipProp = poHaveEquip->GetEquipProp();
        if (NULL == poHaveEquipProp)
        {
            return FALSE;
        }

        if((poHaveEquipProp->wDressLevel < wEquipDressLevel) && (wHeroLevel >= wEquipDressLevel))
        {
            bPromote = TRUE;
        }
        else
        {
            bPromote = FALSE;
        }
    }

    if(bPromote)
    {
        if(0 == stNewEquipPrompt.wHeroID)
        {
            stNewEquipPrompt.wHeroID = poHero->GetDT_HERO_BASE_DATA().wKindID;
        }
        if(stNewEquipPrompt.byEquipNum < EQUIP_POS_NUM)
        {
            stNewEquipPrompt.awEquipIDInfo[stNewEquipPrompt.byEquipNum++] = wEquipID;
        }
    }

    return bPromote;
}

VOID CPlayer::GetEmprisePrompt(DT_FUNC_COMMON_PROMPT& stPlunderPrompt)
{

    memset(&stPlunderPrompt, 0, sizeof(stPlunderPrompt));
    stPlunderPrompt.byBuildKindID = EBK_ADVENTURE;

    //精英副本相关的提示，是否可冒险

    //从第一个开始查找记录，返回记录
    //精英副本每个城镇只有1个副本
    UINT8 byTownIdx = 1;
    UINT8 byInstandeIdx = 1;
    if(NULL == m_poInstance)
        return;

    CC_LOOP_DO
    CC_LOOP_BREAK(!GetBuild(EBK_ELITEINSTANCE))
    while( (m_poInstance->CkEliteUnlock(byTownIdx, 1)) && (byTownIdx <= MAX_TOWN_PER_SCENE_NUM))
    {
        while( (byInstandeIdx < MAX_INSTANCE_PER_TOWN_NUM) && (m_poInstance->CkEliteUnlock(byTownIdx, byInstandeIdx) ) )
        {
            CInstanceRecord* poInstanceRecord = m_poInstance->GetSingleInstaceRecord(EIT_ELITE, byTownIdx, byInstandeIdx);

            byInstandeIdx++;
            if(NULL == poInstanceRecord)
            {
                //已解锁，没有记录，未打，可挑战
                stPlunderPrompt.byFuncFlag = 1;
                return;
            }
            //今天没打过
            if(!IsToday(poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime))
            {
                stPlunderPrompt.byFuncFlag = 1;
                return;
            }

        }

        byTownIdx++;
    }
    CC_LOOP_WHILE(0)



    CC_LOOP_DO
    CC_LOOP_BREAK(!GetBuild(EBK_CLIMBTOWER))
    CC_LOOP_BREAK(m_poClimbTower)
    //获取城镇相关属性信息
    STownInstanceProp * pstTownInstanceProp = NULL;
    pstTownInstanceProp = CInstancePropMgr::Instance()->GetSceneInstance(EIT_TOWER, 1);
    CC_LOOP_BREAK (NULL == pstTownInstanceProp)
    UINT16 wLevel = GetLevel();
    UINT8 byInstanceNum = pstTownInstanceProp->m_mapInstance.size();
    byInstanceNum = byInstanceNum > MAX_INSTANCE_CLIMB_TOWER_NUM ? MAX_INSTANCE_CLIMB_TOWER_NUM : byInstanceNum;
    for (UINT8 byInstanceIdx = 1; byInstanceIdx <= byInstanceNum; byInstanceIdx++)
    {
        //查看城镇的副本是否有打通关记录。
        CInstanceRecord* poInstanceRecord = NULL;
        poInstanceRecord = m_poInstance->GetSingleInstaceRecord(EIT_TOWER, 1, byInstanceIdx);
        SInstanceProp* pstProp = CInstancePropMgr::Instance()->GetInstanceProp(EIT_TOWER, 1, byInstanceIdx);
        if(NULL == pstProp)
        {
            continue;
        }
        //没有记录，可打
        if((NULL == poInstanceRecord) && (pstProp->wEnterLevel <= wLevel))
        {
            stPlunderPrompt.byFuncFlag = 2;
            return;
        }
        else if((poInstanceRecord) && (!IsToday(poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime)))
        {
            stPlunderPrompt.byFuncFlag = 2;
            return;
        }
    }

    UINT16	wCanResetClimbTownNum;
    UINT16	wCanBuyResetClimbTownNum;
    m_poClimbTower->GetClimbTowerBaseInfo(wCanResetClimbTownNum, wCanBuyResetClimbTownNum);
    if (wCanResetClimbTownNum > 0 )
    {
        stPlunderPrompt.byFuncFlag = 2;
        return;
    }
    CC_LOOP_WHILE(0)


    CC_LOOP_DO
    CC_LOOP_BREAK(NULL == GetBuild(EBK_YABIAO))
    CC_LOOP_BREAK(NULL == m_poYabiaoInfo)

    //押镖相关的提示，是否可冒险
    if( m_poYabiaoInfo->GetRemainJiebiaoCnt() > 0 )
    {
        stPlunderPrompt.byFuncFlag = 3;
        return;
    }

    if( m_poYabiaoInfo->GetRemainYabiaoCnt() > 0 )
    {
        stPlunderPrompt.byFuncFlag = 3;
        return ;
    }
    CC_LOOP_WHILE(0)
}
VOID CPlayer::GetActivityCenterPrompt(DT_FUNC_COMMON_PROMPT& stPrompt)
{
    memset(&stPrompt, 0, sizeof(stPrompt));
    stPrompt.byBuildKindID = EBK_ACTIVITY;

    //优先吃包子
    //有活动就返回
    //都不闪时取倒计时最少的
    DT_FUNC_COMMON_PROMPT stPromptTmp = {0};
    if((m_poEatBread) && (m_poEatBread->GetPrompt(stPromptTmp)))
    {
        memcpy(&stPrompt, &stPromptTmp, sizeof(DT_FUNC_COMMON_PROMPT));
        return;
    }

    //优先GVE
    if ((m_poCourage) && ((this->GetCliVer()>=310)) && (m_poCourage->GetPrompt(stPromptTmp)))
    {
        memcpy(&stPrompt, &stPromptTmp, sizeof(DT_FUNC_COMMON_PROMPT));
        return;
    }

    //都没有的时候取倒计时最少的
    if(m_poBossBattle)
    {
        if(m_poBossBattle->GetPrompt(stPrompt))
        {
            return;
        }
        if((stPromptTmp.dwFuncCountDown < stPrompt.dwFuncCountDown) && (0 != stPromptTmp.byBuildKindID))
        {
            memcpy(&stPrompt, &stPromptTmp, sizeof(DT_FUNC_COMMON_PROMPT));
            return;
        }
    }
    else
    {
        memcpy(&stPrompt, &stPromptTmp, sizeof(DT_FUNC_COMMON_PROMPT));
    }

}

VOID CPlayer::GetRacePrompt(DT_FUNC_COMMON_PROMPT& stPrompt) //比武提示信息
{
    memset(&stPrompt, 0, sizeof(&stPrompt));
    if(!m_poRace)
    {
        return;
    }

    if(m_poRace->GetPrompt(stPrompt))
    {
        stPrompt.byBuildKindID = EBK_RACEBUILD;
    }
    //领取奖励优先
    if (EPP_RECVENC == stPrompt.byFuncFlag)
    {
        return;
    }

    if(m_poWorship && m_poWorship->CKCanWorship(EWT_RACE, TRUE))
    {
        stPrompt.byFuncFlag = EPP_WORSHIP;
        stPrompt.byBuildKindID = EBK_RACEBUILD;
        return;
    }
    if(m_poRace->GetPrompt(stPrompt))
    {
        stPrompt.byBuildKindID = EBK_RACEBUILD;
    }
}

VOID CPlayer::GetRdChallengePrompt(DT_FUNC_COMMON_PROMPT& stPrompt)//比武提示信息
{
    memset(&stPrompt, 0, sizeof(&stPrompt));
    if(m_poWorship && m_poWorship->CKCanWorship(EWT_RDC, TRUE))
    {
        stPrompt.byFuncFlag = EPP_LIGHTING;
        stPrompt.byBuildKindID = EBK_RDCHALLENGE;
        return;
    }

    if(NULL != GetBuild(EBK_RDCHALLENGE) && NULL != m_poRdChallenge)
    {
        m_poRdChallenge->GetPrompt(stPrompt);
        stPrompt.byBuildKindID = EBK_RDCHALLENGE;
    }
}

VOID CPlayer::GetElitePrompt(DT_FUNC_COMMON_PROMPT& stPrompt) //精英提示信息
{
    memset(&stPrompt, 0, sizeof(stPrompt));
    if(NULL == m_poInstance)
        return;

    //精英副本相关的提示，是否可冒险

    //从第一个开始查找记录，返回记录
    //精英副本每个城镇只有1个副本
    UINT8 byTownIdx = 1;
    UINT8 byInstandeIdx = 1;

    if(GetBuild(EBK_ELITEINSTANCE))
    {
        stPrompt.byBuildKindID = EBK_ELITEINSTANCE;
        while( (m_poInstance->CkEliteUnlock(byTownIdx, 1)) && (byTownIdx <= MAX_TOWN_PER_SCENE_NUM))
        {
            while( (byInstandeIdx < MAX_INSTANCE_PER_TOWN_NUM) && (m_poInstance->CkEliteUnlock(byTownIdx, byInstandeIdx) ) )
            {
                CInstanceRecord* poInstanceRecord = m_poInstance->GetSingleInstaceRecord(EIT_ELITE, byTownIdx, byInstandeIdx);

                byInstandeIdx++;
                if(NULL == poInstanceRecord)
                {
                    //已解锁，没有记录，未打，可挑战
                    stPrompt.byFuncFlag = 1;
                    return;
                }
                //今天没打过
                if(!IsToday(poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime))
                {
                    stPrompt.byFuncFlag = 1;
                    return;
                }

            }

            byTownIdx++;
        }
    }
}

VOID CPlayer::GetSoulInstancePrompt(DT_FUNC_COMMON_PROMPT& stPrompt)
{
	memset(&stPrompt, 0, sizeof(DT_FUNC_COMMON_PROMPT));
	stPrompt.byBuildKindID = EBK_SOUL;
	stPrompt.byFuncFlag = 1;
}

VOID CPlayer::GetClimbTowerPrompt(DT_FUNC_COMMON_PROMPT& stPrompt)//爬塔提示信息
{
    memset(&stPrompt, 0, sizeof(stPrompt));
    if(NULL == m_poInstance)
        return;
    if(GetBuild(EBK_CLIMBTOWER) && NULL != m_poClimbTower)
    {
        stPrompt.byBuildKindID = EBK_CLIMBTOWER;
        //获取城镇相关属性信息
        STownInstanceProp * pstTownInstanceProp = NULL;
        pstTownInstanceProp = CInstancePropMgr::Instance()->GetSceneInstance(EIT_TOWER, 1);
        if (NULL == pstTownInstanceProp)
        {
            return;
        }
        UINT16 wLevel = GetLevel();
        UINT8 byInstanceNum = pstTownInstanceProp->m_mapInstance.size();
        byInstanceNum = byInstanceNum > MAX_INSTANCE_CLIMB_TOWER_NUM ? MAX_INSTANCE_CLIMB_TOWER_NUM : byInstanceNum;
        for (UINT8 byInstanceIdx = 1; byInstanceIdx <= byInstanceNum; byInstanceIdx++)
        {
            //查看城镇的副本是否有打通关记录。
            CInstanceRecord* poInstanceRecord = NULL;
            poInstanceRecord = m_poInstance->GetSingleInstaceRecord(EIT_TOWER, 1, byInstanceIdx);
            SInstanceProp* pstProp = CInstancePropMgr::Instance()->GetInstanceProp(EIT_TOWER, 1, byInstanceIdx);
            if(NULL == pstProp)
            {
                continue;
            }
            //没有记录，可打
            if((NULL == poInstanceRecord) && (pstProp->wEnterLevel <= wLevel))
            {
                stPrompt.byFuncFlag = 1;
                return;
            }
            else if((poInstanceRecord) && (!IsToday(poInstanceRecord->GetDT_INSTANCE_DATA().qwLastPassTime)))
            {
                stPrompt.byFuncFlag = 1;
                return;
            }
        }

        UINT16	wCanResetClimbTownNum;
        UINT16	wCanBuyResetClimbTownNum;
        m_poClimbTower->GetClimbTowerBaseInfo(wCanResetClimbTownNum, wCanBuyResetClimbTownNum);
        if (wCanResetClimbTownNum > 0 )
        {
            stPrompt.byFuncFlag = 1;
            return;
        }
    }
}

VOID CPlayer::GetYabiaoPrompt(DT_FUNC_COMMON_PROMPT& stYabiaoPrompt)
{
    memset(&stYabiaoPrompt, 0, sizeof(stYabiaoPrompt));
    stYabiaoPrompt.byBuildKindID = EBK_YABIAO;
    if (NULL == m_poYabiaoInfo)
    {
        return;
    }


    if( m_poYabiaoInfo->GetYabiaoFlag() == CYabiaoInfo::ms_nYabiaoProcessFlagRunning )
    {
        stYabiaoPrompt.byFuncFlag = ms_nPromotYabiaoRunning;
        stYabiaoPrompt.dwFuncCountDown = m_poYabiaoInfo->GetCurBiaoInfo().wCostTime - m_poYabiaoInfo->GetYabiaoPassedTime();
    }
    else if( m_poYabiaoInfo->GetYabiaoFlag() == CYabiaoInfo::ms_nYabiaoProcessFlagSuccess )
    {
        stYabiaoPrompt.byFuncFlag = ms_nPromotYabiaoSucceed;
    }
    else if( m_poYabiaoInfo->GetRemainYabiaoCnt() != 0 )
    {
        stYabiaoPrompt.byFuncFlag = ms_nPromotYabiaoCanYabiao;

    }
    else if( m_poYabiaoInfo->GetRemainJiebiaoCnt() != 0 )
    {
        stYabiaoPrompt.byFuncFlag = ms_nPromotYabiaoCanJiebiao;
    }
    else
    {
        stYabiaoPrompt.byFuncFlag = 0;
    }
}

VOID CPlayer::GetActivityEncouragePrompt(DT_FUNC_COMMON_PROMPT& stPrompt)//活动领取奖励提示信息
{
    memset(&stPrompt, 0, sizeof(stPrompt));
    if(!GetBuild(EBK_ACTIVITY_ENCOURAGE) || NULL == m_poActivityEncourage)
    {
        return;
    }

    stPrompt.byBuildKindID = EBK_ACTIVITY_ENCOURAGE;
    stPrompt.byFuncFlag  = m_poActivityEncourage->GetCanActivityEncourageNum();
}

//获取可使用宝箱ID
UINT16 CPlayer::GetBoxGoodsPrompt()
{
    CID2ItemShmemMap& mapItem = m_oBag.GetItemMap();
    CItem* poItem = mapItem.GetFistData();
    do
    {
        if(NULL == poItem)
        {
            break;
        }

        if(EIK_GOODS != poItem->GetItemKind())
        {
            continue;
        }
        CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
        if(NULL == poGoods)
        {
            continue;
        }
        if((EGMK_BOX != poGoods->GetGoodsMainKindID()) && (EGMK_RATEBOX != poGoods->GetGoodsMainKindID()))
        {
            continue;
        }
        UINT16 wGoodsID = poGoods->GetItemKindID();
        if(CLuamgr::Instance()->CkBoxGoodsPrompt(wGoodsID))
        {
            SGoodsProp* pstProp = poGoods->GetGoodsProp();
            if((pstProp) && (GetLevel() >= pstProp->wUseLevel))
            {
                return wGoodsID;
            }
        }

    }
    while (poItem = mapItem.GetNextData());

    return 0;
}

VOID CPlayer::GetJingJiePrompt(DT_FUNC_COMMON_PROMPT& stJingJiePrompt)
{
    memset(&stJingJiePrompt, 0, sizeof(stJingJiePrompt));
    stJingJiePrompt.byBuildKindID = EBK_JINGJIE;

    if(NULL == m_poJingJie)
    {
        return;
    }
    DT_JINGJIE_BASE_DATA& stBaseData = m_poJingJie->GetDT_JINGJIE_DATA().stBaseData;
    UINT16 wMaxQualityLevel = CJingJiePropMgr::Instance()->GetMaxQuality();
    if(stBaseData.wLevel >= wMaxQualityLevel)
    {
        return;
    }
    UINT16 wSubLevelNum = CJingJiePropMgr::Instance()->GetSubLevelNum();
    UINT32 dwCostGas = 0;
    if(wSubLevelNum == stBaseData.wSubLevel)
    {
        dwCostGas = CJingJiePropMgr::Instance()->GetJingJieSubLevelUpgradeGas( GetCoachCareerID(), stBaseData.wLevel + 1, 1);
    }
    else
    {
        dwCostGas = CJingJiePropMgr::Instance()->GetJingJieSubLevelUpgradeGas( GetCoachCareerID(), stBaseData.wLevel, stBaseData.wSubLevel + 1);
    }
    if(m_stDT_PLAYER_BASE_DATA.qwJingJie >= dwCostGas)
    {
        stJingJiePrompt.byFuncFlag = 1;
        return;
    }
}

VOID CPlayer::GetCoinResourcePrompt(DT_RESOURCES_PROMPT_DATA &stRsPromptData)
{
    memset(&stRsPromptData, 0, sizeof(stRsPromptData));
    GetMainCoinResourcePrompt(stRsPromptData.astRsPromptInfo[stRsPromptData.byRsNum++]);
    GetCropCoinResourcePrompt(stRsPromptData.astRsPromptInfo[stRsPromptData.byRsNum++]);
    GetMoneyTreeCoinResourcePrompt(stRsPromptData.astRsPromptInfo[stRsPromptData.byRsNum++]);
    GetSellCoinResourcePrompt(stRsPromptData.astRsPromptInfo[stRsPromptData.byRsNum++]);
    GetMonsterCoinResourcePrompt(stRsPromptData.astRsPromptInfo[stRsPromptData.byRsNum++]);
    GetBuddhistSutrasCoinResourcePrompt(stRsPromptData.astRsPromptInfo[stRsPromptData.byRsNum++]);
}

VOID CPlayer::GetStoryResourcePrompt(DT_RESOURCES_PROMPT_DATA &stRsPromptData)
{
    memset(&stRsPromptData, 0, sizeof(stRsPromptData));
    GetCropStoryResourcePrompt(stRsPromptData.astRsPromptInfo[stRsPromptData.byRsNum++]);
}

VOID CPlayer::GetJingJieResourcePrompt(DT_RESOURCES_PROMPT_DATA &stRsPromptData)
{
    memset(&stRsPromptData, 0, sizeof(stRsPromptData));
    GetFirstBossJingJieResourcePrompt(stRsPromptData.astRsPromptInfo[stRsPromptData.byRsNum++]);
    GetBuddhistSutrasJingJieResourcePrompt(stRsPromptData.astRsPromptInfo[stRsPromptData.byRsNum++]);
    GetRaceJingJieResourcePrompt(stRsPromptData.astRsPromptInfo[stRsPromptData.byRsNum++]);
    GetCropJingJieResourcePrompt(stRsPromptData.astRsPromptInfo[stRsPromptData.byRsNum++]);
}

VOID CPlayer::GetScienceResourcePrompt(DT_RESOURCES_PROMPT_DATA &stRsPromptData)
{
    memset(&stRsPromptData, 0, sizeof(stRsPromptData));
    GetMeltScienceResourcePrompt(stRsPromptData.astRsPromptInfo[stRsPromptData.byRsNum++]);
    GetCropScienceResourcePrompt(stRsPromptData.astRsPromptInfo[stRsPromptData.byRsNum++]);
}

//////////////////////////////////////////////////////////////////////////
VOID CPlayer::GetMainCoinResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt)
{
    memset(&stRsPrompt, 0, sizeof(stRsPrompt));
    stRsPrompt.byKindID = ERS_COIN_MAIN;
    CBuild* poBuild = poBuild = GetBuild(EBK_LEVY);
    if(NULL == poBuild)
    {
        return;
    }

    DT_BUILD_BASE_DATA&  stDT_BUILD_BASE_DATAEX = poBuild->GetDT_BUILD_BASE_DATA();
    //今天未征收
    if(!IsToday(stDT_BUILD_BASE_DATAEX.qwValue2))
    {
        stDT_BUILD_BASE_DATAEX.byValue3 = 0; //重新赋值征收次数
    }
    UINT8 byLevyNum = (m_pstMaintownLevyProp->byLevyNumPerDay > stDT_BUILD_BASE_DATAEX.byValue3) ? m_pstMaintownLevyProp->byLevyNumPerDay - stDT_BUILD_BASE_DATAEX.byValue3 : 0;
    if(byLevyNum > 0)
    {
        stRsPrompt.byFlag = 1;
    }
}

VOID CPlayer::GetCropCoinResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt)
{
    memset(&stRsPrompt, 0, sizeof(stRsPrompt));
    stRsPrompt.byKindID = ERS_COIN_CROP;
    if(NULL == m_poCrop)
        return;

    //检测是否可收获
    //////////////////////////////////////////////////////////////////////////
    SCropProp *		pCropProp = NULL;
    DT_CROP_DATA &	stDT_CROP_DATA = m_poCrop->GetDT_CROP_DATA();
    for (int i = 0; i < MAX_CROP_NUM; i++)
    {
        if (stDT_CROP_DATA.astCropInfoList[i].byOpen && stDT_CROP_DATA.astCropInfoList[i].wCropKindID > 0)
        {
            pCropProp = CCropSeedPropMgr::Instance()->GetCropProp(m_poCrop->GetDT_CROP_DATA().astCropInfoList[i].wCropKindID);
            if (NULL == pCropProp)
            {
                continue;
            }
            if (ECPK_COIN != pCropProp->byType)
            {
                continue;
            }

            //计算经过的时间
            UINT64 qwDiffSecond =	SDTimeSecs() - stDT_CROP_DATA.astCropInfoList[i].qwLastCropTime;
            qwDiffSecond = pCropProp->wObtainCD > qwDiffSecond ? (pCropProp->wObtainCD - qwDiffSecond) : 0;
            if(0 == qwDiffSecond)
            {
                stRsPrompt.byFlag = 1; //可收获
                return;
            }
        }
    }

    //检测种子是否可种植
    CCropPropMapItr iter;
    CCropPropMap & mapCropProp = CCropSeedPropMgr::Instance()->GetCropPropMap();
    for (iter = mapCropProp.begin(); iter != mapCropProp.end(); iter++)
    {
        SCropProp &stCropProp = iter->second;
        CGoods * poGood = m_oBag.GetGoodsItem(stCropProp.wCropKindID);
        if (NULL == poGood)
        {
            continue;
        }
        if (poGood->GetCurPileNum() <= 0)
        {
            continue;
        }

        if (ECPK_COIN != stCropProp.byType)
        {
            continue;
        }
        stRsPrompt.byFlag = 1;
        break;
    }
}

VOID CPlayer::GetMoneyTreeCoinResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt)
{
    memset(&stRsPrompt, 0, sizeof(stRsPrompt));
    stRsPrompt.byKindID = ERS_COIN_MONEYTREE;
    if(NULL == m_poMoneyTree)
    {
        return;
    }
    DT_MONEY_TREE_DATA& stDT_MONEY_TREE_DATA = m_poMoneyTree->GetDT_MONEY_TREE_DATA();
    //今天
    if(IsToday(stDT_MONEY_TREE_DATA.qwRockTime))
    {
        UINT16 wTotalRockNum = CVipPropMgr::Instance()->GetIncNum(EVINF_MONEYTREE, m_stDT_PLAYER_BASE_DATA.byVipLevel);
        if(stDT_MONEY_TREE_DATA.wRockNum < wTotalRockNum)
        {
            stRsPrompt.byFlag = 1;
        }
    }
    else
    {
        stRsPrompt.byFlag = 1;
    }
}

VOID CPlayer::GetSellCoinResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt)
{
    memset(&stRsPrompt, 0, sizeof(stRsPrompt));
    stRsPrompt.byKindID = ERS_COIN_SELL;
    if(m_oBag.HaveEquip())
    {
        stRsPrompt.byFlag = 1;
    }
}

VOID CPlayer::GetMonsterCoinResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt)
{
    memset(&stRsPrompt, 0, sizeof(stRsPrompt));
    stRsPrompt.byKindID = ERS_COIN_MONSTER;
    stRsPrompt.byFlag = 1;
}

VOID CPlayer::GetBuddhistSutrasCoinResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt)
{
    memset(&stRsPrompt, 0, sizeof(stRsPrompt));
    stRsPrompt.byKindID = ERS_COIN_BUDDHISTSUTRAS;
    stRsPrompt.byFlag = 1;
}

//////////////////////////////////////////////////////////////////////////
VOID CPlayer::GetCropStoryResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt)
{
    memset(&stRsPrompt, 0, sizeof(stRsPrompt));
    stRsPrompt.byKindID = ERS_STORY_CROP;
    if(NULL == m_poCrop)
        return;

    //检测是否可收获
    //////////////////////////////////////////////////////////////////////////
    SCropProp *		pCropProp = NULL;
    DT_CROP_DATA &	stDT_CROP_DATA = m_poCrop->GetDT_CROP_DATA();
    for (int i = 0; i < MAX_CROP_NUM; i++)
    {
        if (stDT_CROP_DATA.astCropInfoList[i].byOpen && stDT_CROP_DATA.astCropInfoList[i].wCropKindID > 0)
        {
            pCropProp = CCropSeedPropMgr::Instance()->GetCropProp(m_poCrop->GetDT_CROP_DATA().astCropInfoList[i].wCropKindID);
            if (NULL == pCropProp)
            {
                continue;
            }
            if (ECPK_STORY != pCropProp->byType)
            {
                continue;
            }

            //计算经过的时间
            UINT64 qwDiffSecond =	SDTimeSecs() - stDT_CROP_DATA.astCropInfoList[i].qwLastCropTime;
            qwDiffSecond = pCropProp->wObtainCD > qwDiffSecond ? (pCropProp->wObtainCD - qwDiffSecond) : 0;
            if(0 == qwDiffSecond)
            {
                stRsPrompt.byFlag = 1; //可收获
                return;
            }
        }
    }

    //检测种子是否可种植
    CCropPropMapItr iter;
    CCropPropMap & mapCropProp = CCropSeedPropMgr::Instance()->GetCropPropMap();
    for (iter = mapCropProp.begin(); iter != mapCropProp.end(); iter++)
    {
        SCropProp &stCropProp = iter->second;
        CGoods * poGood = m_oBag.GetGoodsItem(stCropProp.wCropKindID);
        if (NULL == poGood)
        {
            continue;
        }
        if (poGood->GetCurPileNum() <= 0)
        {
            continue;
        }

        if (ECPK_STORY != stCropProp.byType)
        {
            continue;
        }
        stRsPrompt.byFlag = 1;
        break;
    }
}

//////////////////////////////////////////////////////////////////////////
VOID CPlayer::GetMeltScienceResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt)
{
    memset(&stRsPrompt, 0, sizeof(stRsPrompt));
    stRsPrompt.byKindID = ERS_SCIENCE_MELT;
    if(m_oBag.HaveEquip())
    {
        stRsPrompt.byFlag = 1;
    }
}
VOID CPlayer::GetCropScienceResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt)
{
    memset(&stRsPrompt, 0, sizeof(stRsPrompt));
    stRsPrompt.byKindID = ERS_SCIENCE_CROP;
    if(NULL == m_poCrop)
        return;

    //检测是否可收获
    //////////////////////////////////////////////////////////////////////////
    SCropProp *		pCropProp = NULL;
    DT_CROP_DATA &	stDT_CROP_DATA = m_poCrop->GetDT_CROP_DATA();
    for (int i = 0; i < MAX_CROP_NUM; i++)
    {
        if (stDT_CROP_DATA.astCropInfoList[i].byOpen && stDT_CROP_DATA.astCropInfoList[i].wCropKindID > 0)
        {
            pCropProp = CCropSeedPropMgr::Instance()->GetCropProp(m_poCrop->GetDT_CROP_DATA().astCropInfoList[i].wCropKindID);
            if (NULL == pCropProp)
            {
                continue;
            }
            if (ECPK_SCIENCE != pCropProp->byType)
            {
                continue;
            }

            //计算经过的时间
            UINT64 qwDiffSecond =	SDTimeSecs() - stDT_CROP_DATA.astCropInfoList[i].qwLastCropTime;
            qwDiffSecond = pCropProp->wObtainCD > qwDiffSecond ? (pCropProp->wObtainCD - qwDiffSecond) : 0;
            if(0 == qwDiffSecond)
            {
                stRsPrompt.byFlag = 1; //可收获
                return;
            }
        }
    }

    //检测种子是否可种植
    CCropPropMapItr iter;
    CCropPropMap & mapCropProp = CCropSeedPropMgr::Instance()->GetCropPropMap();
    for (iter = mapCropProp.begin(); iter != mapCropProp.end(); iter++)
    {
        SCropProp &stCropProp = iter->second;
        CGoods * poGood = m_oBag.GetGoodsItem(stCropProp.wCropKindID);
        if (NULL == poGood)
        {
            continue;
        }
        if (poGood->GetCurPileNum() <= 0)
        {
            continue;
        }

        if (ECPK_SCIENCE != stCropProp.byType)
        {
            continue;
        }
        stRsPrompt.byFlag = 1;
        break;
    }
}

//////////////////////////////////////////////////////////////////////////
VOID CPlayer::GetFirstBossJingJieResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt)
{

}
VOID CPlayer::GetBuddhistSutrasJingJieResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt)
{
    memset(&stRsPrompt, 0, sizeof(&stRsPrompt));
    stRsPrompt.byKindID = ERS_JINGJIE_XTQJ;
    stRsPrompt.byFlag = 1;//可领取奖励
}
VOID CPlayer::GetRaceJingJieResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt)
{
    memset(&stRsPrompt, 0, sizeof(&stRsPrompt));
    stRsPrompt.byKindID = ERS_JINGJIE_RACE;
    if(NULL == m_poRace)
    {
        return;
    }
    DT_RACE_BASE_DATA& stDT_RACE_BASE_DATA = m_poRace->GetDT_RACE_BASE_DATA();
    if(!IsToday(stDT_RACE_BASE_DATA.qwRecvEncourageTime))
    {
        stRsPrompt.byFlag = 1;//可领取奖励
        return;
    }

    SRaceBaseProp& stProp = CRacePropMgr::Instance()->GetRaceProp();
    //判断挑战次数
    //今天
    if(IsToday(stDT_RACE_BASE_DATA.qwLastChallTime))
    {
        UINT16 wMaxChallengeNumPerDay = CVipPropMgr::Instance()->GetIncNum(EVINF_RACEBUYNUM, GetDT_PLAYER_BASE_DATA().byVipLevel);
        if(stDT_RACE_BASE_DATA.byChallTimes < wMaxChallengeNumPerDay)
        {
            stRsPrompt.byFlag = 2;//可挑战
        }
    }
    else
    {
        stRsPrompt.byFlag = 2;//可挑战
    }
}
VOID CPlayer::GetCropJingJieResourcePrompt(DT_RESOURCES_PROMPT &stRsPrompt)
{
    memset(&stRsPrompt, 0, sizeof(stRsPrompt));
    stRsPrompt.byKindID = ERS_JINGJIE_CROP;
    if(NULL == m_poCrop)
        return;

    //检测是否可收获
    //////////////////////////////////////////////////////////////////////////
    SCropProp *		pCropProp = NULL;
    DT_CROP_DATA &	stDT_CROP_DATA = m_poCrop->GetDT_CROP_DATA();
    for (int i = 0; i < MAX_CROP_NUM; i++)
    {
        if (stDT_CROP_DATA.astCropInfoList[i].byOpen && stDT_CROP_DATA.astCropInfoList[i].wCropKindID > 0)
        {
            pCropProp = CCropSeedPropMgr::Instance()->GetCropProp(m_poCrop->GetDT_CROP_DATA().astCropInfoList[i].wCropKindID);
            if (NULL == pCropProp)
            {
                continue;
            }
            if (ECPK_JINGJIE != pCropProp->byType)
            {
                continue;
            }

            //计算经过的时间
            UINT64 qwDiffSecond =	SDTimeSecs() - stDT_CROP_DATA.astCropInfoList[i].qwLastCropTime;
            qwDiffSecond = pCropProp->wObtainCD > qwDiffSecond ? (pCropProp->wObtainCD - qwDiffSecond) : 0;
            if(0 == qwDiffSecond)
            {
                stRsPrompt.byFlag = 1; //可收获
                return;
            }
        }
    }

    //检测种子是否可种植
    CCropPropMapItr iter;
    CCropPropMap & mapCropProp = CCropSeedPropMgr::Instance()->GetCropPropMap();
    for (iter = mapCropProp.begin(); iter != mapCropProp.end(); iter++)
    {
        SCropProp &stCropProp = iter->second;
        CGoods * poGood = m_oBag.GetGoodsItem(stCropProp.wCropKindID);
        if (NULL == poGood)
        {
            continue;
        }
        if (poGood->GetCurPileNum() <= 0)
        {
            continue;
        }

        if (ECPK_JINGJIE != stCropProp.byType)
        {
            continue;
        }
        stRsPrompt.byFlag = 1;
        break;
    }
}

VOID CPlayer::GetHotSpringPrompt(DT_ACTIVI_SIDE_PROMPT_DATA_LST& stHotSpringPrompt)
{
    if ( NULL == m_poFactionPlayer)
    {
        return ;
    }
    CFaction* poFaction = GetFaction();
    if ( NULL == poFaction)
    {
        return ;
    }
    if ( 0 == poFaction->GetDT_FACTION_BASE_DATA().dwHotSpringLevel)
    {
        return ;
    }
    m_poFactionPlayer->GetHotSpring().CheckByEveryTime();
    UINT8 byIgnore = m_poFactionPlayer->GetDT_HOT_SPRING_DATA().byIsIgnorePrompt;
    //温泉
    UINT8 byOpenFlag = poFaction->GetDT_FACTION_COMMON_DATA().stHotSpringData.byGiveWineOpenState;
    if (ESM_YES == byOpenFlag/* && 0 == byIgnore*/)
    {
        stHotSpringPrompt.astActiviPrompt[stHotSpringPrompt.byActiviNum].byActiviID = EASPID_HotSpring;
        stHotSpringPrompt.astActiviPrompt[stHotSpringPrompt.byActiviNum].dwActiviCountDown = 0;
        stHotSpringPrompt.byActiviNum++;
    }


}

VOID CPlayer::GetShadePrompt(DT_ACTIVI_SIDE_PROMPT_DATA_LST& stShadePrompt)
{
    if ( NULL == m_poFactionPlayer)
    {
        return ;
    }
    CFaction* poFaction = GetFaction();
    if ( NULL == poFaction)
    {
        return ;
    }

    if ( 0 == poFaction->GetDT_FACTION_BASE_DATA().dwShadeLevel)
    {
        return ;
    }
    m_poFactionPlayer->GetShade().EachUpdataShade();

    UINT8 byIgnore = m_poFactionPlayer->GetDT_SHADE_DATA().stHuntingInfo.byIsIgnorePrompt;
    BOOL bOpenFlag = CShadePropMgr::Instance()->GetOpenFlag();

    //if ( 0 == byIgnore) //0不忽略 ， 1忽略 //已取消
    //{
		if ( bOpenFlag )
		{
			stShadePrompt.astActiviPrompt[stShadePrompt.byActiviNum].byActiviID = EASPID_Hunting;
			stShadePrompt.astActiviPrompt[stShadePrompt.byActiviNum].dwActiviCountDown = 0;
			stShadePrompt.byActiviNum++;

		}
		else
		{
			stShadePrompt.astActiviPrompt[stShadePrompt.byActiviNum].byActiviID = EASPID_Hunting;
			CHuntingEvent oEvent;
			oEvent.SetParam(TRUE);
			stShadePrompt.astActiviPrompt[stShadePrompt.byActiviNum].dwActiviCountDown = oEvent.GetInterval();
			stShadePrompt.byActiviNum++;
		}
   // }
}

UINT16 CPlayer::SetIgnorePrompt(UINT8 byActiviID, PKT_CLIGS_PROMPT_IGNORE_ACK& stAck)
{
    switch (byActiviID)
    {
    case EASPID_HotSpring:
    {
        if ( NULL == m_poFactionPlayer)
        {
            return ERR_PROMPT_IGNORE::ID_FACTION_NOT_OPEN;
        }

        m_poFactionPlayer->GetDT_HOT_SPRING_DATA().byIsIgnorePrompt = ESM_YES;
        stAck.byActiviID = EASPID_HotSpring;
    }
    break;
    case EASPID_Hunting:
    {
        if ( NULL == m_poFactionPlayer)
        {
            return ERR_PROMPT_IGNORE::ID_FACTION_NOT_OPEN;
        }
        m_poFactionPlayer->GetDT_SHADE_DATA().stHuntingInfo.byIsIgnorePrompt = ESM_YES;
        stAck.byActiviID = EASPID_Hunting;
    }
    break;
    }
    return ERR_PROMPT_IGNORE::ID_SUCCESS;
}

VOID  CPlayer::GetSideActiviPrompt(DT_ACTIVI_SIDE_PROMPT_DATA_LST& stPromptInfo)
{
    memset(&stPromptInfo, 0, sizeof(stPromptInfo));
    CFaction* poFaction = GetFaction();
    if (NULL != poFaction)
    {
		if ( ESM_NO != CSinglePramMgr::Instance()->GetHotSpringFlag())
		{
			if (0 != poFaction->GetDT_FACTION_BASE_DATA().dwHotSpringLevel)
			{
				GetHotSpringPrompt(stPromptInfo);
			}
		}
        
        if ( 0 != poFaction->GetDT_FACTION_BASE_DATA().dwShadeLevel)
        {
            GetShadePrompt(stPromptInfo);
        }

    }
}

VOID CPlayer::GetFactionPrompt(DT_FUNC_COMMON_PROMPT& stFactionPrompt)
{
    memset(&stFactionPrompt, 0, sizeof(stFactionPrompt));
    stFactionPrompt.byBuildKindID = EBK_FACTION;
    if ( NULL == m_poFactionPlayer)
    {
        return ;
    }
    CFaction* poFaction = GetFaction();
    if ( NULL == poFaction)
    {
		stFactionPrompt.byFuncFlag = EFPID_CanApply;

		if( ESM_YES == m_poFactionPlayer->GetDT_PLAYER_MARK_DATA().stLeaveFactionMark.byLeaveMark)
		{
			UINT64 qwCurTime = SGDP::SDTimeSecs();
			if ( qwCurTime < CLuamgr::Instance()->GetJoinFactionInterval() + m_poFactionPlayer->GetDT_PLAYER_MARK_DATA().stLeaveFactionMark.qwLeaveTime )
			{
				stFactionPrompt.dwFuncCountDown = static_cast<UINT32>(m_poFactionPlayer->GetDT_PLAYER_MARK_DATA().stLeaveFactionMark.qwLeaveTime + CLuamgr::Instance()->GetJoinFactionInterval() - qwCurTime );
			}
			else
			{
				stFactionPrompt.dwFuncCountDown = 0;
			}
		}
		else
		{
			stFactionPrompt.dwFuncCountDown = 0;
		}
		return ;
    }

    UINT8 byHotSpringOpenFlag = poFaction->GetDT_FACTION_COMMON_DATA().stHotSpringData.byGiveWineOpenState;
    BOOL bOpenFlag = CShadePropMgr::Instance()->GetOpenFlag();
    DT_FACTION_BASE_DATA& stBaseData = poFaction->GetDT_FACTION_BASE_DATA();
    if (stBaseData.dwHotSpringLevel != 0)
    {
        if ( 1 == byHotSpringOpenFlag)
        {
            stFactionPrompt.byFuncFlag = EFPID_Activi;
            return ;
        }
    }
    if ( stBaseData.dwShadeLevel != 0)
    {
        if ( 1 == CShadePropMgr::Instance()->GetOpenFlag())
        {
            stFactionPrompt.byFuncFlag = EFPID_Activi;
            return ;
        }
    }

    DT_FACTION_PLAYER_CLI stCli = {0};
    poFaction->GetDT_FACTION_PLAYER_CLI_SELF(stCli, m_dwPlayerID);
    BOOL bRet = CFactorAuthorityPropMgr::Instance()->CkJobAuthority(static_cast<em_Faction_Job>(stCli.byJobType), EFA_Audit);
    if ( bRet )
    {
        UINT32 dwApplyNum = poFaction->GetApplyFactionNum();
        if (dwApplyNum != 0)
        {
			UINT16 wCurPlayerNum = 0;
			UINT16 wMaxPlayerNum = 0;
			poFaction->GetFactionPlayerNum(wCurPlayerNum, wMaxPlayerNum);
			if ( wCurPlayerNum != wMaxPlayerNum)
			{
				stFactionPrompt.byFuncFlag = EFPID_HaveApply;
				stFactionPrompt.dwFuncCountDown = 0;
				return ;
			}
        }
    }

    UINT32 dwFactionLevel = poFaction->GetDT_FACTION_BASE_DATA().dwFactionLevel;
    DT_FACTION_PLAYER_DATA* stData = poFaction->GetDT_Faction_Player_Data(m_dwPlayerID);
    if ( NULL == stData)
    {
        return;
    }
	DT_PLAYER_DONATE_INFO& stDonateInfo = m_poFactionPlayer->GetFactionDonate();
    if (!IsToday(stDonateInfo.qwLastCoinDoorsTributeTime))
    {
        stDonateInfo.wTodaysCoinDoorsTributeTimes = 0;
        stDonateInfo.byTodaysCoinDoorsTributeEncourage = 0;//奖励清零
        stDonateInfo.qwLastCoinDoorsTributeTime = SGDP::SDTimeSecs();
    }

    if (!IsToday(stDonateInfo.qwLastGoldDoorsTributeTime))
    {
        stDonateInfo.wTodaysGoldDoorsTributeTimes = 0;
        stDonateInfo.qwLastGoldDoorsTributeTime = SGDP::SDTimeSecs();
    }

    UINT16 wMaxTime = CFactorPropMgr::Instance()->GetCoinMaxTimeEveryDay(dwFactionLevel);
    if ( stDonateInfo.wTodaysCoinDoorsTributeTimes < wMaxTime )
    {
        stFactionPrompt.byFuncFlag = EFPID_Donate;
        stFactionPrompt.dwFuncCountDown = 0;
        return ;
    }

    if ( wMaxTime <= stDonateInfo.wTodaysCoinDoorsTributeTimes && ESM_YES == stDonateInfo.byTodaysCoinDoorsTributeEncourage )
    {
        stFactionPrompt.byFuncFlag = EFPID_Get;
        stFactionPrompt.dwFuncCountDown = 0;
        return ;
    }


}
VOID CPlayer::GetJewelryPrompt(DT_FUNC_COMMON_PROMPT& stJewelryPrompt)
{
	CHero* poHero = m_mapFormationHero.GetFistData();
	while(poHero)
	{
		CJewelryMap& mapJewelry = poHero->GetJewelryMap();
		CJewelry* poJewelry = mapJewelry.GetFistData();
		while(poJewelry)
		{
			if( 0 != poJewelry->GetExtStrengLeftNum() )
			{
				stJewelryPrompt.byBuildKindID = EBK_JEWELRY;
				stJewelryPrompt.byFuncFlag = EJP_CAN_EXT_UPGRADE;	//可提升附加属性
				return ;
			}
			poJewelry = mapJewelry.GetNextData();
		}
		poHero = m_mapFormationHero.GetNextData();
	}



	poHero = m_mapFormationHero.GetFistData();
	while(poHero)
	{
		CJewelryMap& mapJewelry = poHero->GetJewelryMap();
		CJewelry* poJewelry = mapJewelry.GetFistData();
		while(poJewelry)
		{
			if( poJewelry->IsCanUpgrade())
			{
				stJewelryPrompt.byBuildKindID = EBK_JEWELRY;
				stJewelryPrompt.byFuncFlag = EJP_CAN_BASE_UPGRADE;	//可强化
				return ;
			}
			poJewelry = mapJewelry.GetNextData();
		}
		poHero = m_mapFormationHero.GetNextData();
	}
}
