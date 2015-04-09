#include "equipcompose.h"
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <logic/other/singleprammgr.h>
#include <logic/bag/bag.h>
#include <logic/item/itemcomposepropmgr.h>
#include <dll/sdframework/sdloggerimpl.h>

#include <logic/item/itemcomposepropmgr.h>
#include <logic/record/recordmgr.h>
#include <logic/other/equipcompose.h>
#include <logic/item/itemmgr.h>

IMPLEMENT_CLASS_POOL_MGR(CEquipCompse)
CEquipCompse::CEquipCompse()
{
    memset(&m_stDT_EQUIP_COMPOSE_DATA, 0 , sizeof(m_stDT_EQUIP_COMPOSE_DATA));
    m_mapDT_EQUIP_LEVEL_COMPOSE_DATA.clear();
}

CEquipCompse::~CEquipCompse()
{

}

BOOL CEquipCompse::Init(DT_EQUIP_COMPOSE_DATA& stDT_EQUIP_COMPOSE_DATA, CPlayer* poOwner)
{
    if(NULL == poOwner)
    {
        return FALSE;
    }

    memcpy(&m_stDT_EQUIP_COMPOSE_DATA, &stDT_EQUIP_COMPOSE_DATA, sizeof(DT_EQUIP_COMPOSE_DATA));
    m_poOwner = poOwner;
    for(UINT8 byIdx = 0; byIdx < stDT_EQUIP_COMPOSE_DATA.byEquipLevelNum; byIdx++)
    {
        DT_EQUIP_LEVEL_COMPOSE_DATA& stDT_EQUIP_LEVEL_COMPOSE_DATA = stDT_EQUIP_COMPOSE_DATA.astEquipLevelInfo[byIdx];
        m_mapDT_EQUIP_LEVEL_COMPOSE_DATA[stDT_EQUIP_LEVEL_COMPOSE_DATA.wEquipLevel] = &stDT_EQUIP_LEVEL_COMPOSE_DATA;
    }

    return TRUE;
}

DT_EQUIP_LEVEL_COMPOSE_DATA* CEquipCompse::GetDT_EQUIP_LEVEL_COMPOSE_DATA(UINT16 wEquipLevel)
{
    map<UINT16, DT_EQUIP_LEVEL_COMPOSE_DATA*>::iterator itr = m_mapDT_EQUIP_LEVEL_COMPOSE_DATA.find(wEquipLevel);
    if(itr != m_mapDT_EQUIP_LEVEL_COMPOSE_DATA.end())
    {
        return itr->second;
    }

    return NULL;
}

DT_EQUIP_LEVEL_COMPOSE_DATA* CEquipCompse::SetNewDT_EQUIP_LEVEL_COMPOSE_DATA(UINT16 wEquipLevel)
{
    DT_EQUIP_LEVEL_COMPOSE_DATA* pstDT_EQUIP_LEVEL_COMPOSE_DATA = GetDT_EQUIP_LEVEL_COMPOSE_DATA(wEquipLevel);
    if(pstDT_EQUIP_LEVEL_COMPOSE_DATA)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SetNewDT_EQUIP_LEVEL_COMPOSE_DATA equiplevel[%d] exist!"), MSG_MARK, wEquipLevel);
        pstDT_EQUIP_LEVEL_COMPOSE_DATA->byCurLowColor = 0;
        pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID = 0;
        return pstDT_EQUIP_LEVEL_COMPOSE_DATA;
    }
    if(m_stDT_EQUIP_COMPOSE_DATA.byEquipLevelNum >= MAX_EQUIPLEVEL_NUM)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: SetNewDT_EQUIP_LEVEL_COMPOSE_DATA too match!"), MSG_MARK);
        return NULL;
    }
    DT_EQUIP_LEVEL_COMPOSE_DATA& stDT_EQUIP_LEVEL_COMPOSE_DATA = m_stDT_EQUIP_COMPOSE_DATA.astEquipLevelInfo[m_stDT_EQUIP_COMPOSE_DATA.byEquipLevelNum++];
    stDT_EQUIP_LEVEL_COMPOSE_DATA.wEquipLevel = wEquipLevel;
    stDT_EQUIP_LEVEL_COMPOSE_DATA.byCurLowColor = 0;
    stDT_EQUIP_LEVEL_COMPOSE_DATA.wHitKindID = 0;
    m_mapDT_EQUIP_LEVEL_COMPOSE_DATA[wEquipLevel] = &stDT_EQUIP_LEVEL_COMPOSE_DATA;

    return &stDT_EQUIP_LEVEL_COMPOSE_DATA;
}


VOID CEquipCompse::GetDT_EQUIP_COMPOSE_LEVEL_DATA_CLI(SComposeEquipProp& stComposeEquipProp, DT_EQUIP_COMPOSE_LEVEL_DATA_CLI& stDT_EQUIP_COMPOSE_LEVEL_DATA_CLI)
{
    CColor2EquipPropVecMap* pmapColor2EquipPropVec = CItemComposePropMgr::Instance()->GetEquipLevelProp(stComposeEquipProp.wEquipLevel);
    if(NULL == pmapColor2EquipPropVec)
    {
        return;
    }
    stDT_EQUIP_COMPOSE_LEVEL_DATA_CLI.wEquipLevel = stComposeEquipProp.wEquipLevel;
    stDT_EQUIP_COMPOSE_LEVEL_DATA_CLI.byEquipNum = 0;
    DT_EQUIP_LEVEL_COMPOSE_DATA* pstDT_EQUIP_LEVEL_COMPOSE_DATA = GetDT_EQUIP_LEVEL_COMPOSE_DATA(stComposeEquipProp.wEquipLevel);
    UINT8 byCurLowColor = EC_BLUE;
    if((NULL == pstDT_EQUIP_LEVEL_COMPOSE_DATA) || (0 == pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID) || (EC_BLUE == pstDT_EQUIP_LEVEL_COMPOSE_DATA->byCurLowColor))
    {
        byCurLowColor = EC_BLUE;
    }
    else
    {
        byCurLowColor = pstDT_EQUIP_LEVEL_COMPOSE_DATA->byCurLowColor;
    }
    for(CColor2EquipPropVecMapItr itr = pmapColor2EquipPropVec->begin(); itr != pmapColor2EquipPropVec->end(); itr++)
    {
        //客户端自己过滤
        //if(itr->first < byCurLowColor)
        //{
        //	continue;
        //}
        CEquipPropVec& vecEquipProp = itr->second;
        for(CEquipPropVecItr itr = vecEquipProp.begin(); itr != vecEquipProp.end(); itr++)
        {
            DT_COMPOSE_EQUIP_DATA_CLI& stDT_COMPOSE_EQUIP_DATA_CLI = stDT_EQUIP_COMPOSE_LEVEL_DATA_CLI.astEquipInfo[stDT_EQUIP_COMPOSE_LEVEL_DATA_CLI.byEquipNum++];
            SEquipProp& stEquipProp = *itr;
            stDT_COMPOSE_EQUIP_DATA_CLI.wKindID = stEquipProp.wKindID;
            stDT_COMPOSE_EQUIP_DATA_CLI.byDressPos = stEquipProp.byPosKindID;
            stDT_COMPOSE_EQUIP_DATA_CLI.wStrengthenLevel = 1;
            stDT_COMPOSE_EQUIP_DATA_CLI.wDressLevel = stEquipProp.wDressLevel;
            stDT_COMPOSE_EQUIP_DATA_CLI.stBattleAttribute.dwHP = stEquipProp.dwHP;
            stDT_COMPOSE_EQUIP_DATA_CLI.stBattleAttribute.dwAttack = stEquipProp.dwAttack;
        }
    }
    stDT_EQUIP_COMPOSE_LEVEL_DATA_CLI.wEquipClipNum = m_poOwner->GetBag().GetGoodsPileNum(stComposeEquipProp.wClipItemID);
    stDT_EQUIP_COMPOSE_LEVEL_DATA_CLI.wComposeNeedNum = stComposeEquipProp.wNeedClipNum;
    stDT_EQUIP_COMPOSE_LEVEL_DATA_CLI.byCurLowColor = pstDT_EQUIP_LEVEL_COMPOSE_DATA == NULL ? 0 : pstDT_EQUIP_LEVEL_COMPOSE_DATA->byCurLowColor;
    stDT_EQUIP_COMPOSE_LEVEL_DATA_CLI.wHitKindID = pstDT_EQUIP_LEVEL_COMPOSE_DATA == NULL ? 0 : pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID;
    if((NULL != pstDT_EQUIP_LEVEL_COMPOSE_DATA) && (0 != pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID))
    {
        SEquipProp* pstEquipProp = CEquipPropMgr::Instance()->GetProp(pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID);
        if(pstEquipProp)
        {
            stDT_EQUIP_COMPOSE_LEVEL_DATA_CLI.dwColorComposeNeedGold = CItemComposePropMgr::Instance()->GetColorComposeNeedGold(stDT_EQUIP_COMPOSE_LEVEL_DATA_CLI.wEquipLevel, pstEquipProp->byEquipColor);
        }
    }
    stDT_EQUIP_COMPOSE_LEVEL_DATA_CLI.dwCommonComposeNeedGold = stComposeEquipProp.dwCommonComposeGold;
}


UINT16 CEquipCompse::GetComposeCountDown()
{
    if(0 == m_stDT_EQUIP_COMPOSE_DATA.wLastComposeEquipLevel)
    {
        return 0;
    }
    SComposeEquipProp* pstComposeEquipProp = CItemComposePropMgr::Instance()->GetComposeEquipProp(m_stDT_EQUIP_COMPOSE_DATA.wLastComposeEquipLevel);
    if(NULL == pstComposeEquipProp)
    {
        return 0;
    }
    UINT64 qwCurTime = SDTimeSecs();
    INT64 qnDif = qwCurTime - m_stDT_EQUIP_COMPOSE_DATA.qwLastComposeTime;
    INT64 qnCD = (INT64)(pstComposeEquipProp->wEquipComposeCD);
    return (UINT16)(qnDif > qnCD ? 0 : qnCD - qnDif);
}

UINT16 CEquipCompse::OpenEquipComposeTab(UINT8& byEquipLevelNum, DT_EQUIP_COMPOSE_LEVEL_DATA_CLI astEquipLevelInfo[MAX_EQUIPLEVEL_NUM], UINT16& wComposeCountDown)
{
    byEquipLevelNum = 0;
    UINT16 wLevel = m_poOwner->GetLevel();
    CComposeEquipPropMap& mapComposeEquipProp = CItemComposePropMgr::Instance()->GetComposeEquipProp();
    for(CComposeEquipPropMapItr itr = mapComposeEquipProp.begin() ; ((itr != mapComposeEquipProp.end()) & (byEquipLevelNum < MAX_EQUIPLEVEL_NUM)); itr++)
    {
        SComposeEquipProp& stProp = itr->second;
        if((m_poOwner->GetBuild(EBK_EQUIPCOMPOSE)) && (itr == mapComposeEquipProp.begin()) || wLevel >= stProp.wEquipLevel)
        {
            GetDT_EQUIP_COMPOSE_LEVEL_DATA_CLI(stProp, astEquipLevelInfo[byEquipLevelNum++]);
        }
    }
    wComposeCountDown = GetComposeCountDown();

    return ERR_OPEN_EQUIP_COMPOSE_TAB::ID_SUCCESS;
}


UINT16 CEquipCompse::EquipCompose(UINT16 wEquipLevel, UINT8 byCurColorFlag, DT_EQUIP_COMPOSE_LEVEL_DATA_CLI& stEquipLevelInfo, UINT16& wComposeCountDown)
{
    CColor2EquipPropVecMap* pmapColor2EquipPropVec = CItemComposePropMgr::Instance()->GetEquipLevelProp(wEquipLevel);
    if(NULL == pmapColor2EquipPropVec)
    {
        return ERR_EQUIP_COMPOSE::ID_EQUIP_LEVEL_INVALID;
    }
    SComposeEquipProp* pstComposeEquipProp = CItemComposePropMgr::Instance()->GetComposeEquipProp(wEquipLevel);
    if(NULL == pstComposeEquipProp)
    {
        return ERR_EQUIP_COMPOSE::ID_EQUIP_LEVEL_INVALID;
    }
    DT_EQUIP_LEVEL_COMPOSE_DATA* pstDT_EQUIP_LEVEL_COMPOSE_DATA = GetDT_EQUIP_LEVEL_COMPOSE_DATA(wEquipLevel);
    BOOL bReCompose = FALSE;
    //之前没有命中，则是第一次合成消化碎片，否则为重转消耗元宝
    if(NULL == pstDT_EQUIP_LEVEL_COMPOSE_DATA)
    {
        pstDT_EQUIP_LEVEL_COMPOSE_DATA = SetNewDT_EQUIP_LEVEL_COMPOSE_DATA(wEquipLevel);
        if(NULL == pstDT_EQUIP_LEVEL_COMPOSE_DATA)
        {
            RETURN_OTHER_ERR;
        }
        byCurColorFlag = 0;
    }
    else
    {
        if(0 == pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID)
        {
            byCurColorFlag = 0;
        }
        else
        {
            bReCompose = TRUE;
        }

        //设置记录颜色
        if(byCurColorFlag && pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID)
        {
            SEquipProp* pstEquipProp = CEquipPropMgr::Instance()->GetProp(pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID);
            if(NULL == pstEquipProp) //异常，重置
            {
                pstDT_EQUIP_LEVEL_COMPOSE_DATA->byCurLowColor = 0;
                pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID = 0;
                bReCompose = FALSE;
                RETURN_OTHER_ERR;
            }
            pstDT_EQUIP_LEVEL_COMPOSE_DATA->byCurLowColor = pstEquipProp->byEquipColor;
            if(pstDT_EQUIP_LEVEL_COMPOSE_DATA->byCurLowColor <= EC_BLUE) //蓝色不需选定颜色
            {
                byCurColorFlag = 0;
            }
        }
    }

    UINT32 dwCostGold = 0;
    if(bReCompose)
    {
        if(1 == byCurColorFlag)
        {
            dwCostGold = CItemComposePropMgr::Instance()->GetColorComposeNeedGold(wEquipLevel, pstDT_EQUIP_LEVEL_COMPOSE_DATA->byCurLowColor);
        }
        else
        {
            dwCostGold = CItemComposePropMgr::Instance()->GetColorComposeNeedGold(wEquipLevel, 0);
        }
        if(m_poOwner->GetDT_PLAYER_BASE_DATA().qwGold < dwCostGold)
        {
            return ERR_COMMON::ID_GOLD_NOT_ENOUGH;
        }
    }
    else
    {
        if(0 != GetComposeCountDown())
        {
            return ERR_EQUIP_COMPOSE::ID_IN_CD;
        }
        if(m_poOwner->GetBag().GetGoodsPileNum(pstComposeEquipProp->wClipItemID) < pstComposeEquipProp->wNeedClipNum)
        {
            return ERR_EQUIP_COMPOSE::ID_CLIP_NOT_ENOUGH;
        }
    }

    CRandomVec vecRandom;
    CEquipPropVec vecEquipProp;
    for(CColor2EquipPropVecMapItr itr = pmapColor2EquipPropVec->begin(); itr != pmapColor2EquipPropVec->end(); itr++)
    {
        if((1 == byCurColorFlag) && (itr->first < pstDT_EQUIP_LEVEL_COMPOSE_DATA->byCurLowColor))
        {
            continue;
        }
        UINT16 wRate = 0;
        switch(itr->first)
        {
        case EC_BLUE:
            wRate = pstComposeEquipProp->wBlueRate;
            break;
        case EC_PURPLE:
            wRate = pstComposeEquipProp->wPurpleRate;
            break;
        case EC_RED:
            wRate = pstComposeEquipProp->wRedRate;
            break;
        case EC_ORANGE:
            wRate = pstComposeEquipProp->wOrageRate;
            break;
        default:
            wRate = 1;
            break;
        }
        CEquipPropVec vecProp = itr->second;
        for(CEquipPropVecItr itrVec = vecProp.begin(); itrVec != vecProp.end(); itrVec++)
        {
            vecRandom.push_back(wRate);
            vecEquipProp.push_back(*itrVec);
        }
    }
    SEquipProp& stHitProp = vecEquipProp[GetRandomIdxMustHit(vecRandom)];
    pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID = stHitProp.wKindID;
    pstDT_EQUIP_LEVEL_COMPOSE_DATA->wEquipLevel = wEquipLevel;

    if(bReCompose)
    {
        m_poOwner->DecGold(dwCostGold, CRecordMgr::EDGT_COMPOSEEQUIP, wEquipLevel, byCurColorFlag, pstDT_EQUIP_LEVEL_COMPOSE_DATA->byCurLowColor);
    }
    else
    {
        CGoods* poGoods = m_poOwner->GetBag().GetGoodsItem(pstComposeEquipProp->wClipItemID);
        if(poGoods)
        {
            //UINT16 wCostNum = poGoods->GetCurPileNum() < pstComposeEquipProp->wNeedClipNum ? poGoods->GetCurPileNum() : pstComposeEquipProp->wNeedClipNum;
            //poGoods->GetDT_GOODS_DATA().wPileCount -= wCostNum;

            //CRecordMgr::Instance()->RecordInfo( GetID(), ERM_DECITEM, CRecordMgr::EDIT_EQUIPCOMPOSE, pstComposeEquipProp->wClipItemID, SDTimeToString(SDNow()), poGoods->GetCurPileNum(),
            //	GetLevel(), m_stDT_PLAYER_BASE_DATA.byVipLevel, wCostNum);

            //扣除操作
            poGoods->DecPileNum(pstComposeEquipProp->wNeedClipNum, CRecordMgr::EDIT_EQUIPCOMPOSE);

            if(0 == poGoods->GetCurPileNum())
            {
                m_poOwner->OnDeleteItem(poGoods);
            }
        }

        m_stDT_EQUIP_COMPOSE_DATA.qwLastComposeTime = SDTimeSecs();
        m_stDT_EQUIP_COMPOSE_DATA.wLastComposeEquipLevel = wEquipLevel;
    }
    GetDT_EQUIP_COMPOSE_LEVEL_DATA_CLI(*pstComposeEquipProp, stEquipLevelInfo);
    wComposeCountDown = GetComposeCountDown();

    return ERR_EQUIP_COMPOSE::ID_SUCCESS;
}

UINT16 CEquipCompse::RecvComposeEquip(UINT16 wEquipLevel, DT_EQUIP_COMPOSE_LEVEL_DATA_CLI& stEquipLevelInfo, DT_EQUIP_DATA_CLI& stRecvEquipInfo, DT_EQUIP_DATA_STRENGTHEN_CLI& stRecvEquipStrengthenInfo, UINT32& dwStrengthen10Coin)
{
    SComposeEquipProp* pstComposeEquipProp = CItemComposePropMgr::Instance()->GetComposeEquipProp(wEquipLevel);
    if(NULL == pstComposeEquipProp)
    {
        return ERR_RECV_COMPOSE_EQUIP::ID_EQUIP_LEVEL_INVALID;
    }
    DT_EQUIP_LEVEL_COMPOSE_DATA* pstDT_EQUIP_LEVEL_COMPOSE_DATA = GetDT_EQUIP_LEVEL_COMPOSE_DATA(wEquipLevel);
    if((NULL == pstDT_EQUIP_LEVEL_COMPOSE_DATA) || (0 == pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID))
    {
        return ERR_RECV_COMPOSE_EQUIP::ID_UNCOMPOSE;
    }
    if(0 == m_poOwner->GetBag().GetIdleSize())
    {
        return ERR_RECV_COMPOSE_EQUIP::ID_BAG_FULL;
    }
    ECreateItemRet eRet;
    CItem* poItem = CItemMgr::Instance()->CreateItemIntoBag(m_poOwner, pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID, 1, eRet, 0, CRecordMgr::EAIT_BOX, pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID);
    if(NULL == poItem)
    {
        RETURN_OTHER_ERR;
    }
    CEquip* poEquip = dynamic_cast<CEquip*>(poItem);
    if(NULL == poEquip)
    {
        RETURN_OTHER_ERR;
    }
    poEquip->GetDT_EQUIP_DATA_CLI(stRecvEquipInfo);
    poEquip->GetDT_EQUIP_DATA_STRENGTHEN_CLI(stRecvEquipStrengthenInfo);
    dwStrengthen10Coin = poEquip->GetUpLevelCoin(10);

    //重置状态
    pstDT_EQUIP_LEVEL_COMPOSE_DATA->wHitKindID = 0;
    pstDT_EQUIP_LEVEL_COMPOSE_DATA->byCurLowColor = 0;

	GetDT_EQUIP_COMPOSE_LEVEL_DATA_CLI(*pstComposeEquipProp, stEquipLevelInfo);

    return ERR_RECV_COMPOSE_EQUIP::ID_SUCCESS;
}

