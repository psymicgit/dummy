#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdstring.h>
#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include "itemcomposepropmgr.h"
#include <logic/item/goods/goods.h>
#include <logic/item/equip/equippropmgr.h>
#include <logic/item/goods/goodspropmgr.h>
#include <common/client/commondef.h>
#include <logic/player/player.h>
#include <framework/gsapi.h>
#include <logic/item/equip/equippropmgr.h>
#include <logic/item/itemmgr.h>
#include <logic/bag/bag.h>

#include <logic/instance/instancepropmgr.h>

#include <db/dbmgr.h>

using namespace SGDP;

#define SQL_READ_ITEMCOMPOSE_STRING "select * from itemcompose order by KindID"


IMPLEMENT_SINGLETON_PROPMGR(CItemComposePropMgr);

CItemComposePropMgr::CItemComposePropMgr()
{
    m_byMaxGemLevel = 0;
    memset(m_astDT_GEM_MAKER_DATA, 0, sizeof(m_astDT_GEM_MAKER_DATA));
}

CItemComposePropMgr::~CItemComposePropMgr()
{
    for(CItemComposePropMapItr itr = m_mapItemComposeProp.begin(); itr != m_mapItemComposeProp.end(); itr++)
    {
        SDDelete itr->second;
        itr->second = NULL;
    }
}

BOOL CItemComposePropMgr::Init()
{
    if(!LoadFromDBEx())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadGemMakerOpenPropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadGemMakerOpenPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    InitBoxComposeItem();

    if(!InitGemMakerInfo())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: InitGemMakerInfo failed!"), MSG_MARK);
        return FALSE;
    }

    if(!LoadComposeEquipPropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadComposeEquipPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    InitEquipCompose();

    return TRUE;
}

VOID CItemComposePropMgr::UnInit()
{
    m_mapItemComposeProp.clear();
    m_mapGemMakerOpenProp.clear();


	m_byMaxGemLevel = 0;
	memset(m_astDT_GEM_MAKER_DATA, 0, sizeof(m_astDT_GEM_MAKER_DATA));
	m_mapComposeEquipProp.clear();
	m_mapLevel2Color2EquipPropVec.clear();
}

// 从数据源获取数据
//BOOL CItemComposePropMgr::LoadFromDB()
//{
//    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
//    if(NULL == pDBSession)
//    {
//        return FALSE;
//    }
//    SGDP::ISDDBRecordSet* pRes = NULL;
//    UINT32 dwErrID = 0;
//    string strErr;
//    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("itemcompose", " order by KindID").c_str(), &pRes, &dwErrID,  &strErr);
//    if(SDDB_HAS_RECORDSET != nRet)
//    {
//        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
//        return FALSE;
//    }
//    SItemComposeProp* pstItemComposeProp = NULL;
//    UINT16 wLastKindID = 0;
//    while(pRes->GetRecord())// 当有行结果时
//    {
//
//        UINT16 wKindID = SGDP::SDAtou(pRes->GetFieldValueByName("KindID"));
//        if(0 == wKindID)
//        {
//            SYS_CRITICAL(_SDT("[%s: %d]: KindID[0] invalid!"), MSG_MARK);
//            continue;
//        }
//        if(wLastKindID != wKindID)
//        {
//            if(0 == wLastKindID)
//            {
//                pstItemComposeProp = SDNew SItemComposeProp;
//                pstItemComposeProp->wKindID = wKindID;
//            }
//            else
//            {
//                m_mapItemComposeProp[pstItemComposeProp->wKindID] = pstItemComposeProp;
//                pstItemComposeProp = SDNew SItemComposeProp;
//                pstItemComposeProp->wKindID = wKindID;
//            }
//
//        }
//
//        UINT16 wSubKindID = SGDP::SDAtou(pRes->GetFieldValueByName("SubKindID"));
//        //若是制作卷，初始化材料信息
//        SGoodsProp* pstMakerProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(pstItemComposeProp->wKindID);
//        if(pstMakerProp)
//        {
//            SGoodsProp* pstMaterialProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(wSubKindID);
//            if((pstMaterialProp) && (pstMaterialProp->byComposeItemNum < MAX_COMPOSE_ITEM_NUM))
//            {
//                pstMaterialProp->awComposeItem[pstMaterialProp->byComposeItemNum++] = pstItemComposeProp->wKindID;
//            }
//        }
//
//        SItemRate stItemRate;
//        stItemRate.wItemNum = SGDP::SDAtou(pRes->GetFieldValueByName("Num"));
//        stItemRate.wRate = SGDP::SDAtou(pRes->GetFieldValueByName("Rate")) * 100; //万分比
//        pstItemComposeProp->mapComposeItem[wSubKindID] = stItemRate;
//
//        wLastKindID = wKindID;
//    }
//
//    if(NULL != pstItemComposeProp)
//    {
//        m_mapItemComposeProp[pstItemComposeProp->wKindID] = pstItemComposeProp;
//    }
//
//    if(NULL != pRes)
//    {
//        pRes->Release();
//        pRes = NULL;
//    }
//    return TRUE;
//}


// 从数据源获取数据
BOOL CItemComposePropMgr::LoadFromDBEx()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("itemcompose", " order by KindID").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    SItemComposeProp* pstItemComposeProp = NULL;
    //UINT16 wLastKindID = 0;
    while(pRes->GetRecord())// 当有行结果时
    {
        UINT16 wKindID = SGDP::SDAtou(pRes->GetFieldValueByName("KindID"));
        if(0 == wKindID)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: KindID[0] invalid!"), MSG_MARK);
            continue;
        }

        CItemComposePropMapItr itr = m_mapItemComposeProp.find(wKindID);
        if (itr == m_mapItemComposeProp.end())
        {
            pstItemComposeProp = SDNew SItemComposeProp;
            pstItemComposeProp->wKindID = wKindID;
            m_mapItemComposeProp[wKindID] = pstItemComposeProp;
        }
        else
        {
            pstItemComposeProp = itr->second;
        }

        UINT16 wSubKindID = SGDP::SDAtou(pRes->GetFieldValueByName("SubKindID"));
        //初始化物品材料信息
        SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(pstItemComposeProp->wKindID);
        if((pstGoodsProp) && (pstGoodsProp->byComposeItemNum < MAX_COMPOSE_ITEM_NUM))
        {
            pstGoodsProp->awComposeItem[pstGoodsProp->byComposeItemNum++] = wSubKindID;
        }

        //初始化装备材料信息
        SEquipProp* pstEquipProp = CEquipPropMgr::Instance()->GetProp(pstItemComposeProp->wKindID);
        if((pstEquipProp) && (pstEquipProp->byComposeItemNum < MAX_COMPOSE_ITEM_NUM))
        {
            pstEquipProp->awComposeItem[pstEquipProp->byComposeItemNum++] = wSubKindID;
        }

        SItemRate stItemRate;
        stItemRate.wItemNum = SGDP::SDAtou(pRes->GetFieldValueByName("Num"));
        stItemRate.wRate = SGDP::SDAtou(pRes->GetFieldValueByName("Rate")) * 100;  //万分比
        pstItemComposeProp->mapComposeItem[wSubKindID] = stItemRate;
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}

BOOL CItemComposePropMgr::LoadGemMakerOpenPropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("gemmakeropenprop", " order by GemLevel asc;").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    m_byMaxGemLevel = 0;
    while(pRes->GetRecord())
    {
        UINT8 byGemLevel = SGDP::SDAtou(pRes->GetFieldValueByName("GemLevel"));
        if(byGemLevel != m_byMaxGemLevel + 1)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: GemLevel[%d] miss!"), MSG_MARK, m_byMaxGemLevel + 1);
            return FALSE;
        }
        UINT16 wOpenNeedTowerNum = SGDP::SDAtou(pRes->GetFieldValueByName("OpenNeedTowerNum"));
        m_mapGemMakerOpenProp[byGemLevel] = wOpenNeedTowerNum;
        m_byMaxGemLevel = byGemLevel;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}


BOOL CItemComposePropMgr::LoadComposeEquipPropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("composeequipprop", " order by EquipLevel;").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    UINT16 wLastLevel = 0;
    while(pRes->GetRecord())
    {
        SComposeEquipProp stProp;
        memset(&stProp, 0, sizeof(stProp));
        stProp.wEquipLevel = SGDP::SDAtou(pRes->GetFieldValueByName("EquipLevel"));
        stProp.wClipItemID = SGDP::SDAtou(pRes->GetFieldValueByName("ClipItemID"));
        if(!CGoodsPropMgr::Instance()->GetPropByPrimaryKey(stProp.wClipItemID))
        {
            SYS_CRITICAL(_SDT("[%s: %d]: composeequipprop ClipItemID[%d] not cfg!"), MSG_MARK, stProp.wClipItemID);
            return FALSE;
        }
        stProp.wNeedClipNum = SGDP::SDAtou(pRes->GetFieldValueByName("NeedClipNum"));
        if(0 == stProp.wNeedClipNum)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: composeequipprop NeedClipNum is 0!"), MSG_MARK);
            return FALSE;
        }
        stProp.dwCommonComposeGold = SGDP::SDAtou(pRes->GetFieldValueByName("CommonComposeGold"));
        stProp.dwPurpleComposeGold = SGDP::SDAtou(pRes->GetFieldValueByName("PurpleComposeGold"));
        stProp.dwRedComposeGold = SGDP::SDAtou(pRes->GetFieldValueByName("RedComposeGold"));
        stProp.dwOrangeComposeGold = SGDP::SDAtou(pRes->GetFieldValueByName("OrangeComposeGold"));
        stProp.wBlueRate = SGDP::SDAtou(pRes->GetFieldValueByName("BlueRate"));
        stProp.wPurpleRate = SGDP::SDAtou(pRes->GetFieldValueByName("PurpleRate"));
        stProp.wRedRate = SGDP::SDAtou(pRes->GetFieldValueByName("RedRate"));
        stProp.wOrageRate = SGDP::SDAtou(pRes->GetFieldValueByName("OrageRate"));
        stProp.wEquipComposeCD = SGDP::SDAtou(pRes->GetFieldValueByName("EquipComposeCD"));

        m_mapComposeEquipProp[stProp.wEquipLevel] = stProp;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }

    return TRUE;
}

//初始化礼包属性的组合道具
VOID CItemComposePropMgr::InitBoxComposeItem()
{
    for(CItemComposePropMapItr itr = m_mapItemComposeProp.begin(); itr != m_mapItemComposeProp.end(); itr++)
    {
        SItemComposeProp* pstComposeProp = itr->second;
        if(NULL == pstComposeProp)
        {
            continue;
        }
        SGoodsProp* pstGoodsProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey(pstComposeProp->wKindID);
        if(NULL == pstGoodsProp)
        {
            continue;
        }
        if((EGMK_RATEBOX != pstGoodsProp->byMainKindID) && (EGMK_BOX != pstGoodsProp->byMainKindID))
        {
            continue;
        }

        CItemRateMap& mapComposeItem = pstComposeProp->mapComposeItem;
        for(CItemRateMapItr itrCompose = mapComposeItem.begin(); ((itrCompose != mapComposeItem.end()) && (pstGoodsProp->byComposeItemNum < MAX_COMPOSE_ITEM_NUM)); itrCompose++)
        {
            pstGoodsProp->awComposeItem[pstGoodsProp->byComposeItemNum++] = itrCompose->first;
        }
    }
}

SItemComposeProp* CItemComposePropMgr::GetProp(UINT16 wItemKindID)
{
    CItemComposePropMapItr itr = m_mapItemComposeProp.find(wItemKindID);
    if( itr != m_mapItemComposeProp.end())
    {
        return itr->second;
    }

    return NULL;
}


BOOL CItemComposePropMgr::GetDT_NEED_ITEM_DATA(CPlayer* poPlayer, DT_NEED_ITEM_DATA &stDT_NEED_ITEM_DATA, UINT16 wItemKindID, UINT16 wSubKindID)
{
    if (NULL == poPlayer)
    {
        return FALSE;
    }

    CGoods *				poGoods	=	NULL;
    CItemComposePropMapItr itr = m_mapItemComposeProp.find(wItemKindID);
    if( itr != m_mapItemComposeProp.end())
    {

        CItemRateMapItr iter = itr->second->mapComposeItem.find(wSubKindID);
        if (iter != itr->second->mapComposeItem.end())
        {
            stDT_NEED_ITEM_DATA.wItemKindID	 = wSubKindID;
            stDT_NEED_ITEM_DATA.byNum	 = (UINT8)iter->second.wItemNum;
            poGoods = poPlayer->GetBag().GetGoodsItem(wSubKindID);
            if (poGoods)
            {
                stDT_NEED_ITEM_DATA.wCurNum = poGoods->GetCurPileNum();
            }
            else
            {
                stDT_NEED_ITEM_DATA.wCurNum = 0;
            }

            DT_INSTANCE_ID* pstInstanceID = CInstancePropMgr::Instance()->GetBurstItemInstancdID(wSubKindID);
            //副本不一定有爆出该物品
            if(NULL != pstInstanceID)
            {
                stDT_NEED_ITEM_DATA.stBurstInstance.wSceneIdx = pstInstanceID->wSceneIdx;
                stDT_NEED_ITEM_DATA.stBurstInstance.byTownIdx = pstInstanceID->byTownIdx;
                stDT_NEED_ITEM_DATA.stBurstInstance.byInstanceIdx = pstInstanceID->byInstanceIdx;
            }
            else
            {
                stDT_NEED_ITEM_DATA.stBurstInstance.wSceneIdx = 0;
                stDT_NEED_ITEM_DATA.stBurstInstance.byTownIdx = 0;
                stDT_NEED_ITEM_DATA.stBurstInstance.byInstanceIdx = 0;
            }
        }
    }
    return TRUE;
}

UINT16 CItemComposePropMgr::GetGemMakeOpenNeedTowerNum(UINT16 wGemLevel)
{
    C1616MapItr itr = m_mapGemMakerOpenProp.find(wGemLevel);
    if(itr != m_mapGemMakerOpenProp.end())
    {
        return itr->second;
    }

    return 0xFFFF;
}


BOOL CItemComposePropMgr::InitGemMakerInfo()
{
    for(UINT16 wIdx = 0; wIdx < MAX_GEM_LEVEL_EX && wIdx < m_byMaxGemLevel; wIdx++)
    {
        UINT16 wGemLevel = wIdx + 1;;
        DT_GEM_MAKER_DATA& stDT_GEM_MAKER_DATA = m_astDT_GEM_MAKER_DATA[wIdx];
        memset(&stDT_GEM_MAKER_DATA, 0, sizeof(stDT_GEM_MAKER_DATA));
        stDT_GEM_MAKER_DATA.wGemLevel = wGemLevel;
        SGoodsProp* pstGemMakerProp = CGoodsPropMgr::Instance()->GetGemMakerProp(wGemLevel);
        if(NULL == pstGemMakerProp)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: Gem Level[%d] maker is NULL!"), MSG_MARK, wGemLevel);
            return FALSE;
        }
        SGoodsProp* pstGemProp = CGoodsPropMgr::Instance()->GetPropByPrimaryKey((UINT16)(pstGemMakerProp->dwValue));
        if(NULL == pstGemProp)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: Gem [%d] is NULL!"), MSG_MARK, pstGemMakerProp->dwValue);
            return FALSE;
        }
        stDT_GEM_MAKER_DATA.wMakerKindID = pstGemMakerProp->wKindID;
        stDT_GEM_MAKER_DATA.wGemKindID = pstGemProp->wKindID;
        stDT_GEM_MAKER_DATA.dwNeedBlueGas = pstGemMakerProp->dwParam1;
        stDT_GEM_MAKER_DATA.byOpenNeedTowerNum = (UINT8)(GetGemMakeOpenNeedTowerNum(wGemLevel));
        //宝箱，返回的是宝箱的物品
        if((EGMK_INNERBOX == pstGemProp->byMainKindID) || (EGMK_RATEBOX == pstGemProp->byMainKindID) || (EGMK_BOX == pstGemProp->byMainKindID))
        {
            SItemComposeProp* pstItemComposeProp = CItemComposePropMgr::Instance()->GetProp(pstGemProp->wKindID);
            if(NULL == pstItemComposeProp)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: pstItemComposeProp is NULL, ID[%d]!"), MSG_MARK, pstGemProp->wKindID);
                return FALSE;
            }

            CItemRateMap& mapComposeItem = pstItemComposeProp->mapComposeItem;
            if(0 == mapComposeItem.size())
            {
                SYS_CRITICAL(_SDT("[%s: %d]: mapComposeItem[%d] is 0!"), MSG_MARK, pstGemProp->wKindID);
                return FALSE;
            }
            for(CItemRateMapItr itr = mapComposeItem.begin(); itr != mapComposeItem.end(); itr++)
            {
                CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(itr->first, itr->second.wItemNum, stDT_GEM_MAKER_DATA.stGemInfo.astItemList[stDT_GEM_MAKER_DATA.stGemInfo.byItemNum++]);
            }
        }
        else
        {
            CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(pstGemProp->wKindID, 1, stDT_GEM_MAKER_DATA.stGemInfo.astItemList[stDT_GEM_MAKER_DATA.stGemInfo.byItemNum++]);
        }
    }

    return TRUE;
}


VOID CItemComposePropMgr::InitEquipCompose()
{
    CEquipPropMap& mapEquipProp = CEquipPropMgr::Instance()->GetEquipProp();
    for(CEquipPropMapItr itr = mapEquipProp.begin(); itr != mapEquipProp.end(); itr++)
    {
        SEquipProp& stEquipProp = itr->second;
        m_mapLevel2Color2EquipPropVec[stEquipProp.wDressLevel][stEquipProp.byEquipColor].push_back(stEquipProp);
    }
}

UINT32 CItemComposePropMgr::GetColorComposeNeedGold(UINT16 wEquipLevel, UINT8 byCurLowColor)
{
    CComposeEquipPropMapItr itr = m_mapComposeEquipProp.find(wEquipLevel);
    if(itr == m_mapComposeEquipProp.end())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: equiplevel[%d] compose prop is null!"), MSG_MARK, wEquipLevel);
        return 0xFFFFFFFF;
    }
    SComposeEquipProp& stProp = itr->second;
    switch(byCurLowColor)
    {
    case 0:
    case EC_BLUE:
        return stProp.dwCommonComposeGold;
        break;
    case EC_PURPLE:
        return stProp.dwPurpleComposeGold;
        break;
    case EC_RED:
        return stProp.dwRedComposeGold;
        break;
    case EC_ORANGE:
        return stProp.dwOrangeComposeGold;
        break;
    default:
        return stProp.dwOrangeComposeGold;
        break;
    }

    return 0xFFFFFFFF;
}


SComposeEquipProp* CItemComposePropMgr::GetComposeEquipProp(UINT16 wEquipLevel)
{
    CComposeEquipPropMapItr itr = m_mapComposeEquipProp.find(wEquipLevel);
    if(itr != m_mapComposeEquipProp.end())
    {
        return &itr->second;
    }

    return NULL;
}

