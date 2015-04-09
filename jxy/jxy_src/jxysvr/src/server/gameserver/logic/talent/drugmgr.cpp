#include "drugmgr.h"
#include "drugpropmgr.h"
#include "drugupgradepropmgr.h"
#include "experiencedrugpropmgr.h"

#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <common/client/errdef.h>
#include <logic/build/buildmgr.h>
#include <logic/player/playermgr.h>
#include <logic/item/itemmgr.h>
#include <logic/other/singleprammgr.h>
#include <logic/record/recordmgr.h>
#include <logic/base/basedefine.h>
#include <logic/base/logicfunc.h>
#include <logic/bag/bag.h>
#include <logic/hero/hero.h>
#include <logic/task/taskmgr.h>
#include <logic/vip/vippropmgr.h>
#include <logic/player/player.h>
#include "logic/other/fewparam.h"
IMPLEMENT_SINGLETON(CDrugMgr)


CDrugMgr::CDrugMgr()
{

}

CDrugMgr::~CDrugMgr()
{
    CDrugPropMgr::DestroyInstance();
}

BOOL CDrugMgr::Init()
{
    // 初始化CDrugPropMgr管理器
    if(FALSE == CDrugPropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CDrugPropMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CDrugPropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CDrugPropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    if (FALSE == CDrugUpradePropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CDrugUpradePropMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }
    if(FALSE == CDrugUpradePropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CDrugUpradePropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    if (FALSE == CExperienceDrugPropMgr::CreateInstance())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CExperienceDrugPropMgr::CreateInstance failed!"), MSG_MARK);
        return FALSE;
    }

    if(FALSE == CExperienceDrugPropMgr::Instance()->Init())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CExperienceDrugPropMgr Init failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}


VOID CDrugMgr::UnInit()
{
    CDrugPropMgr::Instance()->UnInit();
    CDrugUpradePropMgr::Instance()->UnInit();
    CExperienceDrugPropMgr::Instance()->UnInit();
}


CDrug* CDrugMgr::CreateDrug(UINT32 dwPlayerID, UINT16 wHeroKindID, UINT8 byDrugLevel)
{

    CDrug*poDrug = m_oDrugShmemObjMgr.Alloc();
    if(NULL == poDrug)
    {
        return NULL;
    }
    UINT64 qwKey = CLogicFunc::GetObjKey(dwPlayerID, wHeroKindID, byDrugLevel);
    m_mapDrug[qwKey] = poDrug;

    return poDrug;
}

VOID CDrugMgr::RemoveDrug(UINT32 dwPlayerID, UINT16 wHeroKindID, UINT8 byDrugLevel)
{
    UINT64 qwKey = CLogicFunc::GetObjKey(dwPlayerID, wHeroKindID, byDrugLevel);
    CDrugMapItr itr = m_mapDrug.find(qwKey);
    if (m_mapDrug.end() != itr)
    {
        CDrug* poDrug = itr->second;
        m_oDrugShmemObjMgr.Free(poDrug);
        m_mapDrug.erase(itr);
    }

}


CDrug* CDrugMgr::FindDrug(UINT32 dwPlayerID, UINT16 wHeroKindID, UINT8 byDrugLevel)
{
    UINT64 qwKey = CLogicFunc::GetObjKey(dwPlayerID, wHeroKindID, byDrugLevel);
    CDrugMapItr itr = m_mapDrug.find(qwKey);
    if (m_mapDrug.end() != itr)
    {
        return itr->second;
    }

    return NULL;
}


UINT16 CDrugMgr::UseDrug(CPlayer* poPlayer, UINT16 wHeroKindID, UINT16 wDrugLevel)
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    //根据丹药级别获取丹药对应的物品ID
    SDrugProp*	pstDrugProp	= NULL;
    CItem*		poItem		= NULL;
    pstDrugProp = CDrugPropMgr::Instance()->GetDrugProp(static_cast<UINT8>(wDrugLevel));
    if (NULL == pstDrugProp)
    {
        return ERR_USE_DRUG::ID_DRUGNOTEXIST;
    }

    //获取背包中的物品属性
    poItem = poPlayer->GetBag().GetGoodsItem(pstDrugProp->wDrugItemKindID);
    if(NULL == poItem)
    {
        return ERR_USE_DRUG::ID_BAGDRUGNOTEXIST;
    }

    CGoods* poGoods = dynamic_cast<CGoods*>(poItem);
    if(NULL == poGoods)
    {
        return ERR_USE_DRUG::ID_BAGDRUGNOTEXIST;
    }

    if(EGMK_DRUG != poGoods->GetGoodsMainKindID())
    {
        return ERR_USE_DRUG::ID_BAGDRUGNOTEXIST;
    }

    //检测等级丹药剩余个数
    if(0 == poGoods->GetCurPileNum())
    {
        //异常。。。
        SYS_CRITICAL(_SDT("[%s: %d]: pile num is 0, but exist!"), MSG_MARK);
        //清除
        poPlayer->OnDeleteItem(poGoods);
        return ERR_USE_DRUG::ID_BAGDRUGNOTEXIST;
    }

    CHero* poHero = poPlayer->GetHero(wHeroKindID);
    if(NULL == poHero)
    {
        return ERR_USE_DRUG::ID_HERONOTEXIST;;
    }

    UINT16 wErrCode = poHero->UseDrug(pstDrugProp);
    //使用成功，叠加数--
    if(ERR_USE_DRUG::ID_SUCCESS == wErrCode)
    {
        //重置最后服务时间及丹药等级
        poHero->GetDT_HERO_BASE_DATA().byLastUseDrugLevel = static_cast<UINT8>(wDrugLevel);
        poHero->GetDT_HERO_BASE_DATA().qwLastUseDrugTime = SDTimeSecs();

        //      poGoods->GetDT_GOODS_DATA().wPileCount--;

        //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_EATDRUG, poGoods->GetItemKindID(), SDTimeToString(SDNow()), poGoods->GetCurPileNum(),
        //	poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, 1);

        //扣除操作
        poGoods->DecPileNum(1, CRecordMgr::EDIT_EATDRUG);

        if(0 == poGoods->GetCurPileNum())
        {
            //清除叠加数为0的丹药物品
            poPlayer->OnDeleteItem(poItem);
        }
        //玩家属性重置
        poPlayer->ResetBattleAttr();
    }
    poPlayer->AddGuideRecord( EGF_DRUG);
//	CTaskMgr::Instance()->OnUseDrug(poPlayer, wHeroKindID);

    return wErrCode;
}


UINT16 CDrugMgr::UseExperienceDrug(CPlayer* poPlayer, UINT16 wHeroKindID, UINT16 wExperienceDrugID, UINT16 wGoldExperienceCount)
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

// 	if( wExperienceDrugID==CDrugMgr::ms_nGoldExperienceID && wGoldExperienceCount!=poPlayer->GetRemainGoldExperienceNum() )
// 	{
// 		return ERR_USER_ECPERIENCE_DRUG::ID_SUCCESS;
// 	}

    CHero* poHero = poPlayer->GetHero(wHeroKindID);
    if(NULL == poHero)
    {
        return ERR_USER_ECPERIENCE_DRUG::ID_HERO_NOT_EXIT;
    }

    CHero* poCoachHero = poPlayer->GetCoachHero();
    if(NULL == poCoachHero)
    {
        return ERR_USER_ECPERIENCE_DRUG::ID_HERO_NOT_EXIT;
    }

    //检测武将类型是否为主武将
    if (poCoachHero == poHero || 1 == poHero->GetDT_HERO_BASE_DATA().byCoach)
    {
        return ERR_USER_ECPERIENCE_DRUG::ID_HERO_IS_COACH;;
    }


    if( wExperienceDrugID != CDrugMgr::ms_nGoldExperienceID )
    {
        //获取丹对应的物品ID
        SExperienceDrugProp*	pstExperienceDrugProp	= NULL;
        pstExperienceDrugProp = CExperienceDrugPropMgr::Instance()->GetExperienceDrugProp(wExperienceDrugID);
        if (NULL == pstExperienceDrugProp)
        {
            return ERR_USER_ECPERIENCE_DRUG::ID_ECPERIENCE_NOT_EXIT;
        }

        //获取背包中的物品属性
        CGoods*		poGoods		= NULL;
        poGoods = poPlayer->GetBag().GetGoodsItem(pstExperienceDrugProp->wExperienceDrugItemKindID);
        if(NULL == poGoods)
        {
            return ERR_USER_ECPERIENCE_DRUG::ID_BAG_ECPERIENCE_NOT_EXIT;
        }

        SGoodsProp *poGoodsProp = poGoods->GetGoodsProp();
        if(NULL == poGoodsProp)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: pile num is 0, but exist!"), MSG_MARK);//异常。。。
            return ERR_USER_ECPERIENCE_DRUG::ID_BAG_ECPERIENCE_NOT_EXIT;
        }

        if(EGMK_EXPERIENCE != poGoods->GetGoodsMainKindID())
        {
            return ERR_USER_ECPERIENCE_DRUG::ID_GOOD_TYPE_ERR;
        }

        //检测等级丹药剩余个数
        if(0 == poGoods->GetCurPileNum())
        {
            SYS_CRITICAL(_SDT("[%s: %d]: pile num is 0, but exist!"), MSG_MARK);//异常。。。
            poPlayer->OnDeleteItem(poGoods);//清除
            return ERR_USER_ECPERIENCE_DRUG::ID_BAG_ECPERIENCE_NOT_EXIT;
        }

        if (FLY_LEVEL == poHero->GetLevel()){
            return ERR_USER_ECPERIENCE_DRUG::ID_NEED_FLY;
        }

        //服用经验丹
        if (!poHero->AddExperience(poGoodsProp->dwValue, poCoachHero->GetDT_HERO_BASE_DATA().wLevel))
        {
            return ERR_USER_ECPERIENCE_DRUG::ID_HERO_ENOUGH_LEAVER;
        }

		if ( 3 == pstExperienceDrugProp->byLevel )
		{
			poPlayer->GetFewParam().AddExperienceDrug();
		}
        //扣除操作
        poGoods->DecPileNum(1, CRecordMgr::EDIT_EATDRUG);

        if(0 == poGoods->GetCurPileNum())
        {
            poPlayer->OnDeleteItem(poGoods);		//清除叠加数为0的丹药物品
        }
        poPlayer->ResetBattleAttr();//玩家属性重置
        CTaskMgr::Instance()->OnUseExprienceDrug(poPlayer);
        return ERR_USER_ECPERIENCE_DRUG::ID_SUCCESS;
    }
    else
    {

        poPlayer->GetVipExtData().Check4ResetExperience();

        if( poPlayer->GetVipExtData().GetRemainGoldExperienceNum() == 0 )
        {
            return ERR_USER_ECPERIENCE_DRUG::ID_NOT_REMAIN_GOLD_EXPERIENCE_CNT;
        }


        UINT16 wCostGold = CSinglePramMgr::Instance()->GetGoldExperienceCostGold();

        if( poPlayer->GetDT_PLAYER_BASE_DATA().qwGold < wCostGold )
        {
            return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
        }

        if(poHero->GetDT_HERO_BASE_DATA().wLevel >= poCoachHero->GetDT_HERO_BASE_DATA().wLevel)
        {
            return ERR_USER_ECPERIENCE_DRUG::ID_HERO_ENOUGH_LEAVER;
        }

        if (FLY_LEVEL == poHero->GetLevel()){
            return ERR_USER_ECPERIENCE_DRUG::ID_NEED_FLY;
        }

        if ( !poHero->AddExperience( CSinglePramMgr::Instance()->GetGoldExperienceAddValue(), poCoachHero->GetDT_HERO_BASE_DATA().wLevel  ) )
        {
            return ERR_USER_ECPERIENCE_DRUG::ID_HERO_ENOUGH_LEAVER;
        }

        poPlayer->DecGold( wCostGold, CRecordMgr::EDGT_GOLDEXPERIENCE );

        poPlayer->GetVipExtData().GetDT_VIP_EXT_DATA().wEatGoldExperienceCnt++;
        CTaskMgr::Instance()->OnUseExprienceDrug(poPlayer);
        poPlayer->ResetBattleAttr();//玩家属性重置

        poPlayer->GetVipExtData().GetDT_VIP_EXT_DATA().qwLastEatTime = SDTimeSecs();
        return ERR_USER_ECPERIENCE_DRUG::ID_SUCCESS;
    }

}

UINT16	CDrugMgr::GetDT_EXPERIENCE_DRUG_DATA(CPlayer* poPlayer, UINT16 wExperienceDrugID, DT_EXPERIENCE_DRUG_DATA & stExperienceDrugData)
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    DT_ITEM_DATA_CLI2 stDT_ITEM_DATA_CLI2;
    memset(&stDT_ITEM_DATA_CLI2, 0x00, sizeof(stDT_ITEM_DATA_CLI2));
    CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(wExperienceDrugID, 1, stDT_ITEM_DATA_CLI2);

    stExperienceDrugData.wBagNum = 0;
    stExperienceDrugData.wExperienceDrugID = wExperienceDrugID;
    stExperienceDrugData.dwExperience = stDT_ITEM_DATA_CLI2.stItemInfo.stGoods.dwValue;

    SExperienceDrugProp * pstExperienceDrugProp = NULL;
    pstExperienceDrugProp = CExperienceDrugPropMgr::Instance()->GetExperienceDrugProp(wExperienceDrugID);
    if (NULL == pstExperienceDrugProp)
    {
        return ERR_OPEN_ECPERIENCE_DRUG::ID_SUCCESS;
    }

    CGoods*	poGoods		= NULL;
    poGoods = poPlayer->GetBag().GetGoodsItem(pstExperienceDrugProp->wExperienceDrugItemKindID);
    if (NULL == poGoods)
    {
        return ERR_OPEN_ECPERIENCE_DRUG::ID_SUCCESS;
    }

    stExperienceDrugData.wBagNum = poGoods->GetCurPileNum();
    return ERR_OPEN_ECPERIENCE_DRUG::ID_SUCCESS;
}

UINT16	CDrugMgr::GetDT_EXPERIENCE_DRUG_DATA(CPlayer* poPlayer, DT_EXPERIENCE_DRUG_DATA astExperienceDrugDataList[3])
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    int		nNum = 0;
    CGoods*	poGoods		= NULL;
    CExperienceDrugPropMapItr itr;
    CExperienceDrugPropMap &mapExperienceDrugProp = CExperienceDrugPropMgr::Instance()->GetExperienceDrugPropMap();
    for (itr = mapExperienceDrugProp.begin(), nNum = 0; itr != mapExperienceDrugProp.end() && nNum < 3; itr++, nNum++)
    {
        SExperienceDrugProp & stExperienceDrugProp = itr->second;
        astExperienceDrugDataList[nNum].wBagNum = 0;
        astExperienceDrugDataList[nNum].wExperienceDrugID = stExperienceDrugProp.wExperienceDrugItemKindID;
        poGoods = poPlayer->GetBag().GetGoodsItem(stExperienceDrugProp.wExperienceDrugItemKindID);
        if(NULL == poGoods)
        {
            astExperienceDrugDataList[nNum].wBagNum = 0;
        }
        else
        {
            astExperienceDrugDataList[nNum].wBagNum = poGoods->GetCurPileNum();
        }

        DT_ITEM_DATA_CLI2 stDT_ITEM_DATA_CLI2;
        memset(&stDT_ITEM_DATA_CLI2, 0x00, sizeof(stDT_ITEM_DATA_CLI2));
        CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(stExperienceDrugProp.wExperienceDrugItemKindID, 1, stDT_ITEM_DATA_CLI2);
        astExperienceDrugDataList[nNum].dwExperience = stDT_ITEM_DATA_CLI2.stItemInfo.stGoods.dwValue;
    }
    return ERR_OPEN_ECPERIENCE_DRUG::ID_SUCCESS;
}

UINT16  CDrugMgr::GetDT_GOLD_EXPERIENCE_DRUG_DATA( CPlayer *poPlayer, DT_GOLD_EXPERIENCE_DRUG_DATA &data )
{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    data.wExperienceDrugID = ms_nGoldExperienceID;

    data.dwCostGold = CSinglePramMgr::Instance()->GetGoldExperienceCostGold();
    data.wMaxNum = CVipPropMgr::Instance()->GetIncNum( EVINF_HEROEATJINGYANDAN, poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel );
    data.wRemainNum = poPlayer->GetVipExtData().GetRemainGoldExperienceNum();
    data.dwExperience = CSinglePramMgr::Instance()->GetGoldExperienceAddValue();
    data.wOpenVipLevel = CVipPropMgr::Instance()->GetLowVipIncLevel( EVINF_HEROEATJINGYANDAN );
    data.wNextVipLevelMaxNum = CVipPropMgr::Instance()->GetIncNum( EVINF_HEROEATJINGYANDAN, poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel );
    return ERR_OPEN_ECPERIENCE_DRUG::ID_SUCCESS;
}