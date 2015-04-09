#include "player.h"
#include "common/client/errdef.h"
#include "logic/other/singleprammgr.h"
#include "logic/record/recordmgr.h"
#include "logic/updateattr/updateattrpropmgr.h"
#include "logic/hero/heropropmgr.h"
#include <logic/task/taskmgr.h>
#include "logic/hero/hero.h"

INT32 CPlayer::GetUpdateAttrInfo( UINT16 wHeroKindID, UINT8 byAttrKind, DT_UPDATE_ATTR_DATA &data )
{
    CHero *pHero = m_mapKind2Hero.GetData( wHeroKindID );

    if( NULL == pHero )
    {
        return ERR_OPEN_UPDATE_ATTR_PANNEL::ID_HERO_CANNOT_FIND;
    }

    UINT16 wCurAttrLevel = pHero->GetAttrLevel( byAttrKind );


    SUpdateData updateData;
    if( !CUpdateAttrPropMgr::Instance()->GetUpdateAttrNeedInfo(
                pHero->GetDT_HERO_BASE_DATA().byCoach,
                byAttrKind, pHero->GetHeroProp()->byCareerID, wCurAttrLevel + 1,
                pHero->GetTalent(), updateData, pHero->GetDT_HERO_BASE_DATA().wLevel ) )
    {
        return ERR_OPEN_UPDATE_ATTR_PANNEL::ID_ATTR_PARAM_ERROR;
    }

    data = GetUpdateAttrData( pHero, wCurAttrLevel, updateData, byAttrKind );
    return ERR_OPEN_UPDATE_ATTR_PANNEL::ID_SUCCESS;
}



UINT32 CPlayer::GetHeroAttrValue( UINT32 dwPerCountAddValue, UINT16 wCount, UINT8 byAttr, CHero *pHero )
{
    if( EAST_HP == byAttr )
    {
        return static_cast<UINT32>(dwPerCountAddValue * wCount + pHero->GetDT_BATTLE_ATTRIBUTE().qwHP);
    }
    else if( EAST_ATTACK == byAttr )
    {
        return dwPerCountAddValue * wCount + pHero->GetDT_BATTLE_ATTRIBUTE().dwAttack;
    }
    else
    {
        SYS_CRITICAL( _SDT("[%s: %d]unknow attr kind:%d"), MSG_MARK, byAttr );
        return 0;
    }
}


INT32 CPlayer::UpdateAttr( UINT16 wHeroKindID, UINT8 byAttrKind, UINT8 byUpdateKind, UINT16 wAttrUpdateCnt,
                           DT_UPDATE_ATTR_DATA &oUpdateAttrData, DT_UPDATED_ATTR_BATTLE_DATA &oBattleData)
{
    memset( &oUpdateAttrData, 0 , sizeof( DT_UPDATE_ATTR_DATA ) );
    memset( &oBattleData, 0 , sizeof( DT_UPDATED_ATTR_BATTLE_DATA ) );

    CHero *pHero = m_mapKind2Hero.GetData( wHeroKindID );
    if( NULL == pHero )
    {
        return ERR_UPDATE_ATTR::ID_HERO_CANNOT_FIND;
    }

    SUpdateData updateData;
    if( !CUpdateAttrPropMgr::Instance()->GetUpdateAttrNeedInfo(
                pHero->GetDT_HERO_BASE_DATA().byCoach,
                byAttrKind, pHero->GetHeroProp()->byCareerID, pHero->GetAttrLevel( byAttrKind ) + 1,
                pHero->GetTalent(), updateData, pHero->GetDT_HERO_BASE_DATA().wLevel ) )
    {
        return ERR_UPDATE_ATTR::ID_ATTR_PARAM_ERROR;
    }
    if(CKGuide( EGF_UPDATEATTR))
    {
        updateData.stCost.dwCostCoinValue = 0; //引导消耗0
    }

    //先判断是否客户端提交的等级与现在相等
    UINT16 wActualCnt = 0;
    wActualCnt = pHero->GetAttrLevel( byAttrKind );

    //获得所需消耗物品
    CGoods *poGoods = NULL;
    UINT16 wItemKind = 0;
    if( EUABT_ITEM == byUpdateKind )
    {
        poGoods = GetUpdateItem( byAttrKind, wItemKind );
        if( NULL == poGoods )
        {
            //DBG_INFO( _SDT("[%s: %d]无法获得poGoods,属性:%d"), MSG_MARK, byAttrKind );
            return ERR_UPDATE_ATTR::ID_CANNOT_FIND_UPDATE_ITEM_ERROR;
        }
    }

    oUpdateAttrData.byAttrKind = byAttrKind;
    if( wActualCnt != wAttrUpdateCnt )
    {
        //等级不匹配，那么返回当前的数值即可，不进行升级
        oUpdateAttrData = GetUpdateAttrData( pHero, wActualCnt, updateData, byAttrKind );

        oBattleData = GetUpdateAttrBattleInfo( pHero );
        //重复升级，也返回成功
        return ERR_UPDATE_ATTR::ID_SUCCESS;
    }

    //判断是否到达限定等级
    int nItemNum = 0;
    UINT16 nTotalCnt = CUpdateAttrPropMgr::Instance()->GetTotalCount( pHero->GetDT_HERO_BASE_DATA().wLevel );
    if( EAST_HP == byAttrKind )
    {
        if( nTotalCnt <= pHero->GetDT_HERO_BASE_DATA().stUpdateAttrInfo.wAttrHP )
        {
            return ERR_UPDATE_ATTR::ID_ATTR_UPDATE_FULL;
        }
    }
    else if( EAST_ATTACK == byAttrKind )
    {
        if( nTotalCnt <= pHero->GetDT_HERO_BASE_DATA().stUpdateAttrInfo.wAttrAttack )
        {
            return ERR_UPDATE_ATTR::ID_ATTR_UPDATE_FULL;
        }
    }
    else
    {
        return ERR_UPDATE_ATTR::ID_ATTR_CANNOT_FIND;
    }
    if( EUABT_COIN == byUpdateKind )
    {
        //消耗铜钱
        if( GetDT_PLAYER_BASE_DATA().qwCoin < updateData.stCost.dwCostCoinValue )
        {
            return ERR_COMMON::ID_COIN_NOT_ENOUGH;
        }

        //GetDT_PLAYER_BASE_DATA().qwCoin-=updateData.stCost.dwCostCoinValue;
        DecCoin(updateData.stCost.dwCostCoinValue, CRecordMgr::EDCT_UPDATEATTR, byAttrKind, wActualCnt + 1 );
    }
    else
    {
        if( poGoods->GetCurPileNum() < updateData.stCost.wCostItemValue )
        {
            return ERR_UPDATE_ATTR::ID_NOT_ENOUGH_ITEM;
        }

        //poGoods->GetDT_GOODS_DATA().wPileCount-=updateData.stCost.wCostItemValue;
        //日志
        //CRecordMgr::Instance()->RecordInfo( GetID(), ERM_DECITEM, CRecordMgr::EDIT_MAKEGOODS, poGoods->GetItemKindID(), SDTimeToString(SDNow()), poGoods->GetCurPileNum(),
        //	GetLevel(), GetDT_PLAYER_BASE_DATA().byVipLevel, updateData.stCost.wCostItemValue);

        //扣除操作
        poGoods->DecPileNum(updateData.stCost.wCostItemValue, CRecordMgr::EDIT_MAKEGOODS);


        if( poGoods->GetDT_GOODS_DATA().wPileCount == 0 )
        {
            GetBag().DelItem( poGoods, TRUE );
        }

    }

    //属性升级，并返回升级后的结果

    UINT16 wCurCount = 0;
    wCurCount = pHero->OnUpdateAttr( byAttrKind, updateData.dwPerCountAddValue );

    SUpdateData newLevelUpdateData;
    if( !CUpdateAttrPropMgr::Instance()->GetUpdateAttrNeedInfo(
                pHero->GetDT_HERO_BASE_DATA().byCoach,
                byAttrKind, pHero->GetHeroProp()->byCareerID, wCurCount + 1,
                pHero->GetTalent(), newLevelUpdateData, pHero->GetDT_HERO_BASE_DATA().wLevel ) )
    {
        return ERR_UPDATE_ATTR::ID_ATTR_PARAM_ERROR;
    }

    oUpdateAttrData = GetUpdateAttrData( pHero, wCurCount, newLevelUpdateData, byAttrKind );

    oBattleData = GetUpdateAttrBattleInfo( pHero );

    if (EAST_HP == byAttrKind)
    {
        CTaskMgr::Instance()->OnStrengthLife(this);
    }
    else if (EAST_ATTACK == byAttrKind)
    {
        CTaskMgr::Instance()->OnStrengthAttack(this);
    }

    AddGuideRecord( EGF_UPDATEATTR );
    return ERR_UPDATE_ATTR::ID_SUCCESS;
}

DT_UPDATE_ATTR_DATA CPlayer::GetUpdateAttrData( CHero *pHero, UINT16 wCount, const SUpdateData &inputUpdateData, UINT8 byAttrKind )
{
    //获得升级的属性
    DT_UPDATE_ATTR_DATA outUpdateAttrData = {0};
    outUpdateAttrData.dwCostCoin = inputUpdateData.stCost.dwCostCoinValue;
    outUpdateAttrData.dwCostItemNum = inputUpdateData.stCost.wCostItemValue;
    outUpdateAttrData.dwCurLevelValue = GetHeroAttrValue( inputUpdateData.dwPerCountAddValue, 0, byAttrKind, pHero );
    outUpdateAttrData.byAttrKind = byAttrKind;
    UINT16 wItemKind = 0;
    CGoods* poGoods = GetUpdateItem( byAttrKind, wItemKind );
    if( NULL == poGoods )
    {
        //DBG_INFO( _SDT("[%s: %d]无法获得poGoods,属性:%d"), MSG_MARK, byAttrKind );
        outUpdateAttrData.dwItemID = wItemKind;
    }
    else
    {
        outUpdateAttrData.dwItemID = poGoods->GetItemKindID();
        outUpdateAttrData.dwItemNum = poGoods->GetCurPileNum();
    }
    outUpdateAttrData.dwNextLevelValue = GetHeroAttrValue( inputUpdateData.dwPerCountAddValue, 1, byAttrKind, pHero );

    outUpdateAttrData.qwCurCoinNum = GetDT_PLAYER_BASE_DATA().qwCoin;
    outUpdateAttrData.wCurCount = wCount;
    outUpdateAttrData.wTotalCount = CUpdateAttrPropMgr::Instance()->GetTotalCount( pHero->GetDT_HERO_BASE_DATA().wLevel );

    return outUpdateAttrData;
}

DT_UPDATED_ATTR_BATTLE_DATA CPlayer::GetUpdateAttrBattleInfo( CHero *pHero )
{
    DT_UPDATED_ATTR_BATTLE_DATA oBattleData;
    oBattleData.dwAttackValue = pHero->GetDT_BATTLE_ATTRIBUTE().dwAttack;
    oBattleData.dwHPValue = static_cast<UINT32>(pHero->GetDT_BATTLE_ATTRIBUTE().qwHP);

    oBattleData.dwOldTotalPower = GetDT_PLAYER_BASE_DATA().dwPower;
    oBattleData.dwSelfOldPower = pHero->GetPower();
    ResetBattleAttr(TRUE);
    oBattleData.dwTotalPower = GetDT_PLAYER_BASE_DATA().dwPower;
    oBattleData.dwSelfPower = pHero->GetPower();
    return oBattleData;
}

CGoods *CPlayer::GetUpdateItem( UINT8 byAttr, UINT16 &wItemID )
{
    CItem *pItem = NULL;
    //if( EAST_HP==byAttr )
    //{
    //	pItem=GetBag().GetGoodsItem( CSinglePramMgr::Instance()->GetStrengthenHeroHPItemID() );
    //	wItemID=CSinglePramMgr::Instance()->GetStrengthenHeroHPItemID();
    //}
    //else
    //{
    //	pItem=GetBag().GetGoodsItem( CSinglePramMgr::Instance()->GetStrengthenHeroAttackItemID() );
    //	wItemID=CSinglePramMgr::Instance()->GetStrengthenHeroAttackItemID();
    //}

    if( NULL == pItem )
    {
        return NULL;
    }

    if( pItem->GetItemKind() != EIK_GOODS )
    {
        return NULL;
    }

    return dynamic_cast<CGoods *>( pItem );
}

BOOL CPlayer::AddAttrUpdateCnt( CHero *pHero, UINT8 byAttrKind, UINT32 dwPerCountAddValue, UINT16 &wCntResult )
{
    if( EAST_HP == byAttrKind )
    {
        wCntResult = pHero->GetDT_HERO_BASE_DATA().stUpdateAttrInfo.wAttrHP++;
        pHero->GetDT_HERO_BASE_DATA().stBattleAttribute.dwHP += dwPerCountAddValue;
    }
    else if( EAST_ATTACK == byAttrKind )
    {
        wCntResult = pHero->GetDT_HERO_BASE_DATA().stUpdateAttrInfo.wAttrAttack++;
        pHero->GetDT_HERO_BASE_DATA().stBattleAttribute.dwAttack += dwPerCountAddValue;
    }
    else
    {
        SYS_CRITICAL( _SDT("[%s: %d]:unknow attr kind:%d"), MSG_MARK, byAttrKind );
        return FALSE;
    }

    return TRUE;
}