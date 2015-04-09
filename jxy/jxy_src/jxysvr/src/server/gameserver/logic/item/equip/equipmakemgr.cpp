///<------------------------------------------------------------------------------
//< @file:   item\equip\equipmakemgr.cpp
//< @author: hongkunan
//< @date:   2014Äê8ÔÂ25ÈÕ 17:15:43
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "equipmakemgr.h"
#include "framework/gsapi.h"
#include "commondef.h"
#include <sddb.h>
#include <db/dbmgr.h>
#include "logic/base/stringtool.h"

BOOL CEquipMakePropMgr::Init()
{
    if (!LoadEquipMakeProp()
        || !LoadSeriesProp())
    {
        return FALSE;
    }

    return TRUE;
}

BOOL CEquipMakePropMgr::LoadEquipMakeProp()
{
    DB_READ_BEGIN("equipmakeprop");

    m_mapMakeCfg.clear();

    while (pRecordSet->GetRecord()) {
        EquipMakeBookID wMakeBookID = SGDP::SDAtou(pRecordSet->GetFieldValueByName("MakeBookID"));
        SEquipMakeProp &stProp = m_mapMakeCfg[wMakeBookID];

        stProp.wMakeBookID = wMakeBookID;
        stProp.wSeriesID   = SGDP::SDAtou(pRecordSet->GetFieldValueByName("SeriesID"));
        stProp.wEquipID    = SGDP::SDAtou(pRecordSet->GetFieldValueByName("EquipID"));
        stProp.dwCostCoin  = SGDP::SDAtou(pRecordSet->GetFieldValueByName("CostCoin"));

        std::string strEquips       = pRecordSet->GetFieldValueByName("CostEquips");
        std::string strGoods        = pRecordSet->GetFieldValueByName("CostGoods");
        std::string strNums         = pRecordSet->GetFieldValueByName("Nums");

        std::vector<UINT32> vecEquipID;
        std::vector<UINT32> vecGoodsID;
        std::vector<UINT32> vecMaterialNum;

        StringTool::SDSplitStringToUInt(vecEquipID,     strEquips, ',');
        StringTool::SDSplitStringToUInt(vecGoodsID,     strGoods, ',');
        StringTool::SDSplitStringToUInt(vecMaterialNum, strNums, ',');

        if (vecEquipID.size() + vecGoodsID.size() != vecMaterialNum.size()) {
            SYS_CRITICAL(_SDT("[%s: %d]: CEquipMakePropMgr::LoadEquipMakeProp vecEquipID.size() + vecMaterialID.size() != vecMaterialNum.size(), make book id = %u, equip id = %u!"), MSG_MARK, wMakeBookID, stProp.wEquipID);
            return FALSE;
        }

        if (vecGoodsID.empty() && vecEquipID.empty()) {
            SYS_CRITICAL(_SDT("[%s: %d]: CEquipMakePropMgr::LoadEquipMakeProp vecMaterialID or vecEquipID is empty, make book id = %u, equip id = %u!"), MSG_MARK, wMakeBookID, stProp.wEquipID);
            return FALSE;
        }

        stProp.vecMaterial.clear();

        for (UINT16 i = 0; i < vecEquipID.size(); ++i){
            SMaterialProp stMaterial;
            stMaterial.byMaterialType = EIK_EQUIP;
            stMaterial.wMaterialID  = vecEquipID[i];
            stMaterial.wMaterialCnt = vecMaterialNum[i];

            stProp.vecMaterial.push_back(stMaterial);
        }

        for (UINT16 i = 0; i < vecGoodsID.size(); ++i){
            SMaterialProp stMaterial;
            stMaterial.byMaterialType = EIK_GOODS;
            stMaterial.wMaterialID  = vecGoodsID[i];
            stMaterial.wMaterialCnt = vecMaterialNum[vecEquipID.size() + i];

            stProp.vecMaterial.push_back(stMaterial);
        }
    }

    DB_READ_END;
    return TRUE;
}

BOOL CEquipMakePropMgr::LoadSeriesProp()
{
    DB_READ_BEGIN("equipseriesprop");
    m_mapSeries.clear();

    while (pRecordSet->GetRecord()) {
        SeriesID wSeriesID  = SGDP::SDAtou(pRecordSet->GetFieldValueByName("SeriesID"));

        SSeriesProp &stProp = m_mapSeries[wSeriesID];
        stProp.wSeriesID = wSeriesID;
        stProp.wLevel = SGDP::SDAtou(pRecordSet->GetFieldValueByName("Level"));
        stProp.m_strSeriesName = pRecordSet->GetFieldValueByName("Name");
    }

    for(EquipMakeMap::iterator itr = m_mapMakeCfg.begin(); itr != m_mapMakeCfg.end(); ++itr){
        EquipMakeBookID equipMakeBookID = itr->first;
        SEquipMakeProp &stProp = itr->second;

        SeriesMap::iterator seriesItr = m_mapSeries.find(stProp.wSeriesID);
        if (seriesItr == m_mapSeries.end()){
            SYS_CRITICAL(_SDT("[%s: %d]: CEquipMakePropMgr::LoadSeriesProp can not find series of equip make book[%u]!"), MSG_MARK, equipMakeBookID);
            return FALSE;            
        }

        SSeriesProp &stSeriesProp = seriesItr->second;
        stSeriesProp.m_equips.insert(equipMakeBookID);
    }

    DB_READ_END;
    return TRUE;
}

const SEquipMakeProp* CEquipMakePropMgr::GetMakeProp(EquipMakeBookID bookID)
{
    EquipMakeMap::iterator itr = m_mapMakeCfg.find(bookID);
    if (itr == m_mapMakeCfg.end()){
        return NULL;
    }

    return &itr->second;
}

SeriesID CEquipMakePropMgr::GetSeriesIDOfEquip(EquipID equipID)
{
    const SEquipMakeProp *pstProp = GetMakeProp(equipID);
    return pstProp ? pstProp->wSeriesID : 0;
}

UINT32 CEquipMakePropMgr::GetCostGoldOfMaterial(GoodsID goodsID)
{
    SGoodsProp *pstProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(goodsID);
    return pstProp ? pstProp->dwBuyGold : 0;
}

UINT32 CEquipMakePropMgr::GetCostCoinOfBook(GoodsID bookID)
{
    const SEquipMakeProp *pstProp = GetMakeProp(bookID);
    if (NULL == pstProp){
        return 0;
    }
    
    return pstProp ? pstProp->dwCostCoin : 0;
}

SSeriesProp* CEquipMakePropMgr::GetSeriesProp(SeriesID seriesID)
{
    SeriesMap::iterator itr = m_mapSeries.find(seriesID);
    if (itr == m_mapSeries.end()){
        return NULL;
    }

    return &itr->second;
}

BOOL CEquipMakeMgr::TakeEquipMakeBookInfo(CPlayer &oPlayer, GoodsID makeBookID, DT_EQUIP_MAKE_BOOK &stBookCli)
{
    memset(&stBookCli, 0, sizeof(stBookCli));

    const SEquipMakeProp* pstMakeProp = CEquipMakePropMgr::Instance()->GetMakeProp(makeBookID);
    if (NULL == pstMakeProp){
        return FALSE;
    }

    const SEquipProp* pstEquipProp =  CEquipPropMgr::Instance()->GetProp(pstMakeProp->wEquipID);
    if (NULL == pstEquipProp){
        return FALSE;
    }

    CGoods *poGoods = oPlayer.GetBag().GetGoodsItem(makeBookID);

    stBookCli.wBookCnt = poGoods ? poGoods->GetCurPileNum() : 0;
    stBookCli.wBookID     = makeBookID;
    stBookCli.byIsMakable = TRUE;
    stBookCli.byQuality   = pstEquipProp->byEquipColor;
    stBookCli.byEquipType = pstEquipProp->byPosKindID;
    stBookCli.dwCostCoin  = pstMakeProp->dwCostCoin;
    stBookCli.wEquipID    = pstMakeProp->wEquipID;
    stBookCli.wEquipDressLvl = pstEquipProp->wDressLevel;

    TakeMaterialListCli(oPlayer, pstMakeProp->vecMaterial, stBookCli.stMaterials);

    for(UINT16 idx = 0; idx < stBookCli.stMaterials.byMaterialCnt; idx++){
        DT_MATERIAL &stMaterial = stBookCli.stMaterials.astMaterialList[idx];
        if (stMaterial.wCurNum < stMaterial.wRequiredNum){
            stBookCli.byIsMakable = FALSE;

            if (EIK_GOODS == stMaterial.byMaterialType){
                stBookCli.dwBuyGold += CEquipMakePropMgr::Instance()->GetCostGoldOfMaterial(stMaterial.wMaterialID) * (stMaterial.wRequiredNum - stMaterial.wCurNum);
            }
        }
    }

    stBookCli.stAttr.dwHP = pstEquipProp->dwHP;
    stBookCli.stAttr.dwAttack = pstEquipProp->dwAttack;
    stBookCli.stAttr.dwDefend = pstEquipProp->dwDefend;
    return TRUE;
}

BOOL CEquipMakeMgr::TakeMaterialListCli(CPlayer &oPlayer, const MaterialVec &vecMaterial, DT_MATERIAL_LIST &stMaterialsCli)
{
    memset(&stMaterialsCli, 0, sizeof(stMaterialsCli));

    CBag &oBag = oPlayer.GetBag();
    CID2GoodsShmemMap& mapGoods  = oBag.GetGoodsMap();
    CID2EquipShmemMap& mapEquips = oBag.GetEquipMap();

    for(UINT16 idx = 0; idx < vecMaterial.size(); idx++){
        const SMaterialProp &stMaterialProp = vecMaterial[idx];
        DT_MATERIAL &stMaterial = stMaterialsCli.astMaterialList[stMaterialsCli.byMaterialCnt++];

        UINT16 wOwnCnt = 0;
        if (stMaterialProp.byMaterialType == EIK_EQUIP){
            for(CEquip *poEquip = mapEquips.GetFistData(); poEquip; poEquip = mapEquips.GetNextData()){
                if (poEquip->GetItemKindID() == stMaterialProp.wMaterialID && poEquip->GetLevel() < 1 && poEquip->GetMosaicGemNum() <= 0){
                    wOwnCnt += poEquip->GetCurPileNum();
                }
            }
        }else if(stMaterialProp.byMaterialType == EIK_GOODS){
            CGoods *poCostGoods = oBag.GetGoodsItem(stMaterialProp.wMaterialID);
            if (poCostGoods){
                wOwnCnt += poCostGoods->GetCurPileNum();
            }

            /*
            for(CGoods* poGoods = mapGoods.GetFistData(); poGoods; poGoods = mapGoods.GetNextData()){
                if (poGoods->GetItemKindID() == stMaterialProp.wMaterialID){
                    wOwnCnt += poGoods->GetCurPileNum();
                }
            }
            */
        }

        stMaterial.byMaterialType = stMaterialProp.byMaterialType;
        stMaterial.wMaterialID    = stMaterialProp.wMaterialID;
        stMaterial.wRequiredNum  = stMaterialProp.wMaterialCnt;
        stMaterial.wCurNum       = wOwnCnt;

        stMaterial.stDropInstanceID = oPlayer.GetInstance().GetMaxPassInstanceOfItem(stMaterialProp.wMaterialID);
        stMaterial.byGetFlag = (stMaterial.stDropInstanceID.wSceneIdx >  0 ? 0 : 1);
    }

    return TRUE;
}

BOOL CEquipMakeMgr::CostMaterialList(CPlayer &oPlayer, const DT_MATERIAL_LIST &stMaterials, CRecordMgr::EDecItemType eRecordType, BOOL bIsBuyMaterial /* = FALSE */)
{
    CBag &oBag = oPlayer.GetBag();

    typedef std::vector<CEquip*> CEquipVec;

    for (UINT16 i = 0; i < stMaterials.byMaterialCnt; i++){
        const DT_MATERIAL &stMaterial = stMaterials.astMaterialList[i];
        if (EIK_EQUIP == stMaterial.byMaterialType){
            CEquipVec vecEquip;

            CID2EquipShmemMap &oEquips = oBag.GetEquipMap();
            for(CEquip *poEquip = oEquips.GetFistData(); poEquip; poEquip = oEquips.GetNextData()){
                if (poEquip->GetItemKindID() == stMaterial.wMaterialID && poEquip->GetLevel() < 1 && poEquip->GetMosaicGemNum() <= 0){
                    vecEquip.push_back(poEquip);
                }
            }

            if (!bIsBuyMaterial && vecEquip.size() < stMaterial.wRequiredNum){
                return FALSE;
            }

            for(UINT16 wItemIdx = 0; wItemIdx < vecEquip.size() && wItemIdx < stMaterial.wRequiredNum; wItemIdx++){
                CEquip *poEquip = vecEquip[wItemIdx];
                oBag.DelItem(poEquip, TRUE, eRecordType);
            }
        }else if (EIK_GOODS == stMaterial.byMaterialType){
            CGoods *poCostGoods = oBag.GetGoodsItem(stMaterial.wMaterialID);
            if (NULL == poCostGoods){
                if (bIsBuyMaterial){
                    continue;
                }else{
                    return FALSE;
                }
            }

            if (poCostGoods->GetCurPileNum() < stMaterial.wRequiredNum){
                if (bIsBuyMaterial){
                    oBag.DelItem(poCostGoods, TRUE, eRecordType);
                }else{
                    return FALSE;
                }
            }else if(poCostGoods->GetCurPileNum() == stMaterial.wRequiredNum){
                oBag.DelItem(poCostGoods, TRUE, eRecordType);
            }else{
                poCostGoods->DecPileNum(stMaterial.wRequiredNum, eRecordType);
            }
        }
    }

    return TRUE;
}
