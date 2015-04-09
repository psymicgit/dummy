#include <sdutil.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <logic/base/logicfunc.h>
#include <logic/other/singleprammgr.h>
#include <framework/gsapi.h>
#include "openlightpropmgr.h"
#include <logic/item/equip/equippropmgr.h>
#include <logic/record/recordmgr.h>
#include <logic/item/equip/strengthenpropmgr.h>
#include <logic/item/equip/equip.h>
#include <logic/bag/bag.h>
#include <logic/hero/heromgr.h>
#include <logic/task/taskmgr.h>
#include <logic/player/player.h>
#include <errdef.h>

#include <db/dbmgr.h>

using namespace SGDP;

//const int MAX_OPENLIGHTPROP_ROW = 28;

IMPLEMENT_SINGLETON_PROPMGR(COpenLightPropMgr);

COpenLightPropMgr::COpenLightPropMgr()
{

}

COpenLightPropMgr::~COpenLightPropMgr()
{
}

BOOL COpenLightPropMgr::Init()
{
    if(!LoadOpenLightPropFromDB())
    {
        SYS_CRITICAL(_SDT("[%s: %d]: LoadOpenLightPropFromDB failed!"), MSG_MARK);
        return FALSE;
    }

    return TRUE;
}

VOID COpenLightPropMgr::UnInit()
{
    m_mapOpenLightPropMap.clear();
    m_setRealEquipSet.clear();
}


BOOL COpenLightPropMgr::LoadOpenLightPropFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;

    UINT32 dwErrID = 0;
    string strErr;

    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("equipopenlightprop", " order by KindID ").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    UINT8 byRowNum = 0;

    while(pRes->GetRecord()) // 当有行结果时
    {
        SOpenLightProp stProp = {0};

        stProp.wKindID          = SGDP::SDAtou(pRes->GetFieldValueByName("KindID"));
        stProp.byIsCanOpenLight = SGDP::SDAtou(pRes->GetFieldValueByName("IsCanOpenLight"));
        stProp.wNeedGoodsKindID = SGDP::SDAtou(pRes->GetFieldValueByName("NeedGoodsKindID"));
        stProp.wNeedGoodsCnt    = SGDP::SDAtou(pRes->GetFieldValueByName("NeedGoodsCnt"));
        stProp.bySuccessRate    = SGDP::SDAtou(pRes->GetFieldValueByName("SuccessRate"));
        stProp.dwFullRateGold   = SGDP::SDAtou(pRes->GetFieldValueByName("FullRateGold"));
        stProp.wNewKindID       = SGDP::SDAtou(pRes->GetFieldValueByName("NewKindID"));

        //stProp.vecSuccessRate.push_back(stProp.bySuccessRate);

        m_mapOpenLightPropMap[stProp.wKindID] = stProp;

        if (0 != stProp.wNewKindID)
        {
            m_setRealEquipSet.insert(stProp.wNewKindID);
        }

        byRowNum++;
    }

    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    /*
    if(byRowNum != MAX_OPENLIGHTPROP_ROW)
    {
    	SYS_CRITICAL(_SDT("[%s: %d]: OpenLightprop row[%d] not match[%d] !"), MSG_MARK, byRowNum, MAX_OPENLIGHTPROP_ROW);
    	return FALSE;
    }
    */

    return TRUE;
}


// 获取所有装备的可开光标识串
void COpenLightPropMgr::GetOpenLightFlagInfo(PKT_CLIGS_OPEN_SMITHY_ACK& stAck)
{
    memset(&stAck.abyEquipOpenLightFlagInfo, 0, sizeof(stAck.abyEquipOpenLightFlagInfo));
    for(size_t i = 0; i < stAck.wEquipNum; ++i)
    {
        stAck.abyEquipOpenLightFlagInfo[i] = GetOpenLightFlagOfEquip(stAck.stEquipList.astEquipInfo[i]); // 已开光的真装备
    }
}

void COpenLightPropMgr::GetOpenLightFlagInfo_v410(PKT_CLIGS_OPEN_SMITHY_ACK_v410& stAck)
{
    memset(stAck.stEquipList.abyEquipOpenLightFlagInfo, 0, sizeof(stAck.stEquipList.abyEquipOpenLightFlagInfo));
    for(size_t i = 0; i < stAck.stEquipList.wEquipNum && i < MAX_BAG_OPEN_NUM; ++i)
    {
        stAck.stEquipList.abyEquipOpenLightFlagInfo[i] = GetOpenLightFlagOfEquip(stAck.stEquipList.astEquipInfo[i].stEquipData); 
    }
}

UINT8 COpenLightPropMgr::GetOpenLightFlagOfEquip(DT_EQUIP_DATA_CLI &stEquip)
{
    CRealEquipSetItr equipItr = m_setRealEquipSet.find(stEquip.wKindID);

    if (equipItr != m_setRealEquipSet.end())
    {
        return 2; // 已开光的真装备
    }

    COpenLightPropMapItr propItr = m_mapOpenLightPropMap.find(stEquip.wKindID);
    if(propItr == m_mapOpenLightPropMap.end())
    {
        return 0;
    }

    SOpenLightProp& stProp = propItr->second;
    return stProp.byIsCanOpenLight;
}

// 获取某个装备的开光配置信息
UINT16 COpenLightPropMgr::GetOpenLightPropInfo(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx,
        DT_EQUIP_DATA_CLI& stAfterOpenLightInfo, UINT16& wOpenLightGoodsID,
        UINT16& wNeedOpenLightGoodsNum, UINT16& wOpenLightGoodsNum,
        UINT8& byOpenLightRate, UINT32& dwOpenLightGold,
        DT_BATTLE_ATTRIBUTE& stOldEquipInitBatlAttr, DT_BATTLE_ATTRIBUTE& stNewEquipInitBatlAttr,
        DT_BATTLE_ATTRIBUTE& stOldEquipCurrAttr)

{
    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    CEquip* poEquip = poPlayer->GetEquip(wEquipKindID, wIdx);
    if(NULL == poEquip)
    {
        return ERR_GET_OPENLIGHT_INFO::ID_EQUIP_NOT_EXIST; // 装备不存在
    }

    SEquipProp* pstOldEquipProp = CEquipPropMgr::Instance()->GetProp(wEquipKindID);
    if(NULL == pstOldEquipProp)
    {
        RETURN_OTHER_ERR;
    }

    // 旧装备0级战斗属性(to cli)
    stOldEquipInitBatlAttr.dwHP     = pstOldEquipProp->dwHP;
    stOldEquipInitBatlAttr.dwAttack = pstOldEquipProp->dwAttack;
    // 旧装备当前战斗属性(to cli)
    stOldEquipCurrAttr = CEquip::BattleAttCountByLevel(poEquip->GetDT_EQUIP_DATA().wLevel, poEquip->GetEnchantLevel(), pstOldEquipProp);

    // 检查装备是否已开光
    CRealEquipSetItr iter = m_setRealEquipSet.find(wEquipKindID);
    if (iter != m_setRealEquipSet.end())
    {
        return ERR_GET_OPENLIGHT_INFO::ID_EQUIP_HAS_OPENLIGHT; // 装备已开光
    }

    COpenLightPropMapItr itr = m_mapOpenLightPropMap.find(wEquipKindID);
    if(itr == m_mapOpenLightPropMap.end())
    {
        // 开光配置错误
        return ERR_GET_OPENLIGHT_INFO::ID_EQUIP_CAN_NOT_OPENLIGHT; // 装备不可开光
    }

    SOpenLightProp& stProp = itr->second;

    if (0 == stProp.byIsCanOpenLight)
    {
        return ERR_GET_OPENLIGHT_INFO::ID_EQUIP_CAN_NOT_OPENLIGHT; // 装备不可开光
    }

    if (pstOldEquipProp->bIsLegendEquip){
        return ERR_GET_OPENLIGHT_INFO::ID_EQUIP_CAN_NOT_OPENLIGHT; // 外传装备不可进阶
    }

    SEquipProp* pstNewEquipProp = CEquipPropMgr::Instance()->GetProp(stProp.wNewKindID);
    if(NULL == pstNewEquipProp){
        return ERR_GET_OPENLIGHT_INFO::ID_EQUIP_CAN_NOT_OPENLIGHT; // 装备不可开光
    }


    stAfterOpenLightInfo.wKindID					= pstNewEquipProp->wKindID;
    stAfterOpenLightInfo.wIdx						= poPlayer->CkNextEquipIdx(stProp.wNewKindID);
    stAfterOpenLightInfo.byDressPos					= pstNewEquipProp->byPosKindID;
    stAfterOpenLightInfo.wLevel                     = poEquip->GetDT_EQUIP_DATA().wLevel;
    stAfterOpenLightInfo.wDressLevel                = pstNewEquipProp->wDressLevel;

    // 获取等级战斗属性
    stAfterOpenLightInfo.stBattleAttribute = CEquip::BattleAttCountByLevel(stAfterOpenLightInfo.wLevel, 0, pstNewEquipProp);

    // 获取器魂信息
    CEquip::GetRecoverScience(stAfterOpenLightInfo.wKindID, stAfterOpenLightInfo.dwMinScience, stAfterOpenLightInfo.dwMaxScience);

    // 获取原装备的镶嵌信息
    DT_EQUIP_DATA_CLI stDT_EQUIP_DATA_CLI = {0};
    poEquip->GetDT_EQUIP_DATA_CLI(stDT_EQUIP_DATA_CLI);

    //
    stAfterOpenLightInfo.byXiangqianNum = stDT_EQUIP_DATA_CLI.byXiangqianNum;
    memcpy( stAfterOpenLightInfo.astXiangqianList, stDT_EQUIP_DATA_CLI.astXiangqianList, sizeof(stDT_EQUIP_DATA_CLI.astXiangqianList));

    // 返回客户端的信息
    wOpenLightGoodsID      = stProp.wNeedGoodsKindID;
    wNeedOpenLightGoodsNum = stProp.wNeedGoodsCnt;
    byOpenLightRate        = stProp.bySuccessRate;
    dwOpenLightGold        = stProp.dwFullRateGold;

    // 玩家当前这个类型开光石的数量
    CGoods* poOpenLightGoods = poPlayer->GetBag().GetGoodsItem(stProp.wNeedGoodsKindID);
    if (NULL == poOpenLightGoods)
    {
        wOpenLightGoodsNum = 0;
    }
    else
    {
        wOpenLightGoodsNum = poOpenLightGoods->GetCurPileNum();
    }

    // 新备0级战斗属性
    stNewEquipInitBatlAttr.dwHP     = pstNewEquipProp->dwHP;
    stNewEquipInitBatlAttr.dwAttack = pstNewEquipProp->dwAttack;
    poPlayer->AddGuideRecord(EGF_OPENLIGHT);
    return ERR_GET_OPENLIGHT_INFO::ID_SUCCESS;
}

//装备开光
UINT16 COpenLightPropMgr::OpenLight(CPlayer* poPlayer, UINT16 wEquipKindID, UINT16 wIdx, UINT8 byGoldFlag,
                                    UINT16& wNewEquipKindID, UINT16& wNewIdx, UINT8& byNewOpenLightFlag,
                                    UINT16& wOpenLightGoodsID, UINT16& wOpenLightGoodsNum)
{

    if(NULL == poPlayer)
    {
        RETURN_OTHER_ERR;
    }

    CEquip* poEquip = poPlayer->GetEquip(wEquipKindID, wIdx);
    if(NULL == poEquip)
    {
        return ERR_OPENLIGHT_INFO::ID_EQUIP_NOT_EXIST;//return //装备不存在
    }

    CRealEquipSetItr iter = m_setRealEquipSet.find(wEquipKindID);

    if (iter != m_setRealEquipSet.end())
    {
        return ERR_OPENLIGHT_INFO::ID_EQUIP_HAS_OPENLIGHT; // 装备已开光
    }

    COpenLightPropMapItr itr = m_mapOpenLightPropMap.find(wEquipKindID);
    if(itr == m_mapOpenLightPropMap.end())
    {
        RETURN_OTHER_ERR;
    }

    SOpenLightProp& stProp = itr->second;

    if (0 == stProp.byIsCanOpenLight)
    {
        return ERR_OPENLIGHT_INFO::ID_EQUIP_CAN_NOT_OPENLIGHT; // 装备不可开光
    }


    UINT16 wNeedGoodsKindID = stProp.wNeedGoodsKindID;  // 开光需要的开光石类型
    UINT16 wNeedGoodsCnt    = stProp.wNeedGoodsCnt;     // 开光需要的开光石数量
    UINT32 dwFullRateGold   = stProp.dwFullRateGold;

    //判断开光石是否足够
    CGoods* poOpenLightGoods = poPlayer->GetBag().GetGoodsItem(stProp.wNeedGoodsKindID);

    if((NULL == poOpenLightGoods) || (poOpenLightGoods->GetCurPileNum() < stProp.wNeedGoodsCnt))
    {
        return ERR_OPENLIGHT_INFO::ID_OPENLIGHT_STONE_NOT_ENOUGH; // 开光石不足
    }

    // 没有用元宝并且没有命中
    if((0 == byGoldFlag) && (RandomReseed(100) > stProp.bySuccessRate))
    {
        // 扣除开光石
        //UINT16 wPileCount = poOpenLightGoods->GetDT_GOODS_DATA().wPileCount;
        //poOpenLightGoods->GetDT_GOODS_DATA().wPileCount = (wPileCount >= wNeedGoodsCnt) ? wPileCount - wNeedGoodsCnt : 0;

		//扣除操作
		poOpenLightGoods->DecPileNum(wNeedGoodsCnt, CRecordMgr::EDIT_OPENLIGHT);

        // 当前的开光石(to cli)
        wOpenLightGoodsID  = wNeedGoodsKindID;
        // 当前的开光石数量(to cli)
        wOpenLightGoodsNum = poOpenLightGoods->GetCurPileNum();

        if(0 == poOpenLightGoods->GetCurPileNum())
        {
            poPlayer->OnDeleteItem(poOpenLightGoods);
        }

        return ERR_OPENLIGHT_INFO::ID_OPENLIGHT_RATE_NOT_HIT; // 开光概率没有命中
    }

    // 使用元宝,检查元宝是否够用
    if (byGoldFlag)
    {
        if (poPlayer->GetDT_PLAYER_BASE_DATA().qwGold < stProp.dwFullRateGold)
        {
            return ERR_OPENLIGHT_INFO::ID_GOLD_NOT_ENOUGH; // 元宝不足
        }
    }


    SEquipProp* pstNewEquipProp = CEquipPropMgr::Instance()->GetProp(stProp.wNewKindID);
    if(NULL == pstNewEquipProp)
    {
        RETURN_OTHER_ERR;
    }

	SEquipProp* poEquipProp =poEquip->GetEquipProp();
	if (NULL == poEquipProp)
	{
		RETURN_OTHER_ERR;
	}
    //
    if(poEquipProp->byPosKindID != pstNewEquipProp->byPosKindID)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: new byPosKindID != old byPosKindID!"), MSG_MARK);
        //SDStrncpy(aszErrInfo, SDGBK2UTF8(ERR_MAKE_ITEM::STR_MAKER_CONFIG_ERR).c_str(), MAX_COMMON_TXT_NUM-1);
        RETURN_OTHER_ERR;
    }

    //判断旧装备是否穿戴
    UINT16 wHeroKindID = poEquip->GetDT_EQUIP_DATA().wHeroKindID;
    BOOL   bDress      = FALSE;  // 需穿戴且可穿戴标志
    CHero* poHero      = NULL;

    if(0 != wHeroKindID)
    {
        poHero = poPlayer->GetHero(wHeroKindID);
        if(NULL == poHero)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: poHero is NULL!, poHero = %u"), MSG_MARK, wHeroKindID);
            RETURN_OTHER_ERR;
        }

        //判断旧装备穿戴的武将是否能穿戴新装备
        if(poHero->GetDT_HERO_BASE_DATA().wLevel >= pstNewEquipProp->wDressLevel)
        {
            //需穿戴且可穿戴
            bDress = TRUE;
        }
    }


    //生成新装备
    ECreateItemRet eRet;
    CEquip* poNewEquip = CItemMgr::Instance()->CreateEquip(poPlayer, pstNewEquipProp->wKindID, 1, eRet, poEquip->GetDT_EQUIP_DATA().wLevel);
    if(NULL == poNewEquip)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CreateEquip failed! KindID[%d],eRet[%d]"), MSG_MARK, pstNewEquipProp->wKindID, eRet);
        RETURN_OTHER_ERR;
    }

    // 获取镶嵌列表
    DT_EQUIP_DATA_CLI stDT_EQUIP_DATA_CLI = {0};
    poEquip->GetDT_EQUIP_DATA_CLI(stDT_EQUIP_DATA_CLI);


    //先清除旧装备，才能放入背包，防止原先背包满无法插入问题
    poPlayer->OnDeleteEquip(poEquip);

    //需要穿戴，给武将穿戴装备,不需要穿戴，设置背包信息，放背包
    if(bDress)
    {
        //穿戴新装备
        poHero->DressMakeEquip(poNewEquip);
    }
    poPlayer->OnAddEquip(poNewEquip, bDress);


    // 转移镶嵌列表
    for(size_t i = 0; i < MAX_XIANGQIAN_NUM; ++i)
    {
        poNewEquip->GetDT_EQUIP_DATA().astXiangqianList[i].wItemID = stDT_EQUIP_DATA_CLI.astXiangqianList[i].wKindID;
    }

    if(bDress)
    {
        // 重新计算战力
        poPlayer->ResetBattleAttr(TRUE);
    }

    // 扣元宝
    if (byGoldFlag)
    {
        poPlayer->DecGold(stProp.dwFullRateGold, CRecordMgr::EDGT_EQUIP_OPENLIGHT, stProp.wNewKindID, stProp.wNeedGoodsKindID, stProp.wNeedGoodsCnt);
    }

    // 扣开光石
    UINT16 wPileCount = poOpenLightGoods->GetDT_GOODS_DATA().wPileCount;

	//扣除操作
	poOpenLightGoods->DecPileNum(wNeedGoodsCnt, CRecordMgr::EDIT_OPENLIGHT);


    // 当前开光用的开光石的类型(to cli)
    wOpenLightGoodsID  = wNeedGoodsKindID;
    // 当前的开光石数量(to cli)
    wOpenLightGoodsNum = poOpenLightGoods->GetCurPileNum();

    //日志
    //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_OPENLIGHT, poOpenLightGoods->GetItemKindID(), SDTimeToString(SDNow()), poOpenLightGoods->GetDT_GOODS_DATA().wPileCount,
    //                                    poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, wNeedGoodsKindID);

    if(0 == poOpenLightGoods->GetCurPileNum())
    {
        poPlayer->OnDeleteItem(poOpenLightGoods);
    }



    // 进阶后的装备信息(to cli)
    wNewEquipKindID = poNewEquip->GetDT_EQUIP_DATA().wKindID;
    wNewIdx = poNewEquip->GetDT_EQUIP_DATA().wIdx;
    byNewOpenLightFlag = 2;
    CTaskMgr::Instance()->OnOpenLight(poPlayer);
    return ERR_OPENLIGHT_INFO::ID_SUCCESS;
}




