#include <common/server/utility.h>
#include <logic/base/dbconfig.h>
#include <sdutil.h>
#include <sdstring.h>
#include <sdloggerimpl.h>
#include <sdutil.h>
#include <framework/gsapi.h>
#include <logic/record/recordmgr.h>
#include "itemexchangemgr.h"
#include "logic/other/errmsgmgr.h"
#include "logic/item/itemmgr.h"
#include "logic/bag/bag.h"
#include "logic/player/player.h"

#include <db/dbmgr.h>

using namespace SGDP;

IMPLEMENT_SINGLETON_PROPMGR(CItemExchangeMgr);

CItemExchangeMgr::CItemExchangeMgr()
{
}

CItemExchangeMgr::~CItemExchangeMgr()
{
}

BOOL CItemExchangeMgr::Init()
{
    return LoadFromDB();
}

VOID CItemExchangeMgr::UnInit()
{
    m_mapItemExchangeProp.clear();
}



// 从数据源获取数据
BOOL CItemExchangeMgr::LoadFromDB()
{
    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }
    SGDP::ISDDBRecordSet* pRes = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("itemexchangeprop").c_str(), &pRes, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }
    // 打印查询的表结果
    while(NULL != pRes && pRes->GetRecord())// 当有行结果时
    {
        SItemExchangeProp stProp;
        memset(&stProp, 0, sizeof(stProp));

        stProp.wExchangeIdx = SGDP::SDAtou(pRes->GetFieldValueByName("ExchangeIdx"));
        SDStrncpy(stProp.aszTitle, pRes->GetFieldValueByName("Title"), MAX_COMMON_TXT_NUM - 1);
        SDStrncpy(stProp.aszDesc, pRes->GetFieldValueByName("Desc"), MAX_COMMON_TXT_NUM - 1);
        string strTime(pRes->GetFieldValueByName("StartTime"));
        CSDDateTime stDateTime;
        if(SDTimeFromString(strTime, stDateTime))
        {
            stProp.qwStartTime = stDateTime.GetTimeValue();
        }
        else
        {
            stProp.qwStartTime = SDTimeSecs();
        }

        strTime = pRes->GetFieldLengthByName("EndTime");
        if(SDTimeFromString(strTime, stDateTime))
        {
            stProp.qwEndTime = stDateTime.GetTimeValue();
            if(stProp.qwEndTime < SDTimeSecs()) //已过期，作废
            {
                continue;
            }
        }
        else
        {
            stProp.qwEndTime = SDTimeSecs() + 36000;
        }

        stProp.wExchangeItemKindID = SGDP::SDAtou(pRes->GetFieldValueByName("ExchangeItemKindID"));
        stProp.wExchangeItemNum = SGDP::SDAtou(pRes->GetFieldValueByName("ExchangeItemNum"));
        stProp.awNeedItemKindID[0] = SGDP::SDAtou(pRes->GetFieldValueByName("ExchangeItemNum1"));
        stProp.awNeedItemNum[0] = SGDP::SDAtou(pRes->GetFieldValueByName("NeedItemNum1"));

        stProp.awNeedItemKindID[1] = SGDP::SDAtou(pRes->GetFieldValueByName("ExchangeItemNum2"));
        stProp.awNeedItemNum[1] = SGDP::SDAtou(pRes->GetFieldValueByName("NeedItemNum2"));

        stProp.awNeedItemKindID[2] = SGDP::SDAtou(pRes->GetFieldValueByName("ExchangeItemNum3"));
        stProp.awNeedItemNum[2] = SGDP::SDAtou(pRes->GetFieldValueByName("NeedItemNum3"));

        stProp.awNeedItemKindID[3] = SGDP::SDAtou(pRes->GetFieldValueByName("ExchangeItemNum4"));
        stProp.awNeedItemNum[3] = SGDP::SDAtou(pRes->GetFieldValueByName("NeedItemNum4"));

        stProp.awNeedItemKindID[4] = SGDP::SDAtou(pRes->GetFieldValueByName("ExchangeItemNum5"));
        stProp.awNeedItemNum[4] = SGDP::SDAtou(pRes->GetFieldValueByName("NeedItemNum5"));

        m_mapItemExchangeProp[stProp.wExchangeIdx] = stProp;
    }
    if(NULL != pRes)
    {
        pRes->Release();
        pRes = NULL;
    }
    return TRUE;
}


VOID CItemExchangeMgr::GetItemExchange(CPlayer* poPlayer, UINT8& byExchangeNum, ITEM_EXCHANGE_DATA astExchangeInfo[ITEM_EXCHANGE_ITEM_NUM])
{
    byExchangeNum = 0;
    UINT64 qwCurTime = SDTimeSecs();
    for(CItemExchangePropMapItr itr = m_mapItemExchangeProp.begin();  ((itr != m_mapItemExchangeProp.end()) && (byExchangeNum <= ITEM_EXCHANGE_ITEM_NUM)); itr++)
    {
        SItemExchangeProp& stProp = itr->second;

        if((stProp.qwStartTime > qwCurTime) || (stProp.qwEndTime <= qwCurTime))
        {
            continue;
        }
        ITEM_EXCHANGE_DATA& stITEM_EXCHANGE_DATA = astExchangeInfo[byExchangeNum++];
        SDStrncpy(stITEM_EXCHANGE_DATA.aszTitle, stProp.aszTitle, MAX_COMMON_TXT_NUM - 1);
        SDStrncpy(stITEM_EXCHANGE_DATA.aszDscInfo, stProp.aszDesc, MAX_COMMON_TXT_NUM - 1);
        stITEM_EXCHANGE_DATA.wExchangeIdx = stProp.wExchangeIdx;
        stITEM_EXCHANGE_DATA.wExchangeItemKindID = stProp.wExchangeItemKindID;
        stITEM_EXCHANGE_DATA.wExchangeItemNum = stProp.wExchangeItemNum;
        for(UINT8 byIdx = 0; byIdx < ITEM_EXCHANGE_ITEM_NUM; byIdx++)
        {
            if(0 == stProp.awNeedItemKindID[byIdx])
            {
                continue;
            }
            ITEM_NUM_DATA& stITEM_NUM_DATA = stITEM_EXCHANGE_DATA.astNeedItemInfo[stITEM_EXCHANGE_DATA.byNeedItemNum++];
            stITEM_NUM_DATA.wItemKindID = stProp.awNeedItemKindID[byIdx];
            stITEM_NUM_DATA.wNeedNum = stProp.awNeedItemNum[byIdx];
            stITEM_NUM_DATA.wCurNum = poPlayer->GetBag().GetGoodsPileNum(stITEM_NUM_DATA.wItemKindID);
        }

    }
}


VOID CItemExchangeMgr::OpenItemExchange(CPlayer* poPlayer, PKT_CLIGS_OPEN_ITEM_EXCHANGE_ACK& stAck)
{
    if(NULL == poPlayer)
    {
        SDStrcpy(stAck.aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("INTERNAL_ERROR") .c_str());
        return;
    }

    GetItemExchange(poPlayer, stAck.byExchangeNum, stAck.astExchangeInfo);
}


VOID CItemExchangeMgr::ItemExchange(CPlayer* poPlayer, UINT16 wExchangeIdx, PKT_CLIGS_ITEM_EXCHANGE_ACK& stAck)
{
    if(NULL == poPlayer)
    {
        SDStrcpy(stAck.aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("INTERNAL_ERROR") .c_str());
        return;
    }

    CItemExchangePropMapItr itr = m_mapItemExchangeProp.find(wExchangeIdx);
    if(itr == m_mapItemExchangeProp.end())
    {
        SDStrcpy(stAck.aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("PORPS_NOT_EXIST") .c_str());
        return;
    }

    UINT64 qwCurTime = SDTimeSecs();
    SItemExchangeProp& stProp = itr->second;
    if(stProp.qwStartTime > qwCurTime)
    {
        SDStrcpy(stAck.aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("NOT_CONVERSION_TIME") .c_str());
        return;
    }
    if(stProp.qwEndTime <= qwCurTime)
    {
        SDStrcpy(stAck.aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("NCONVERSION_TIME_OUT") .c_str());
        return;
    }

    CBag& oBag = poPlayer->GetBag();
    if((0 == oBag.GetIdleSize()) && (!oBag.GetGoodsItem(stProp.wExchangeItemKindID)))
    {
        SDStrcpy(stAck.aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("BAG_FULL") .c_str());
        return;
    }
    map<CGoods*, UINT16> mapNeedGoods;	//value为所需物品个数
    for(UINT8 byIdx = 0; byIdx < ITEM_EXCHANGE_ITEM_NUM; byIdx++)
    {
        if(0 == stProp.awNeedItemKindID[byIdx])
        {
            continue;
        }
        CGoods* poGoods = oBag.GetGoodsItem(stProp.awNeedItemKindID[byIdx]);
        if((NULL == poGoods) || (poGoods->GetCurPileNum() < stProp.awNeedItemNum[byIdx]))
        {
            SDStrcpy(stAck.aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("NEED_ITEM_NOT_ENOUGH") .c_str());
            return;
        }
        mapNeedGoods[poGoods] = stProp.awNeedItemNum[byIdx];
    }


    //生成物品
    ECreateItemRet eRet;
    CItem* poItem = CItemMgr::Instance()->CreateItemIntoBag(poPlayer, stProp.wExchangeItemKindID, stProp.wExchangeItemNum, eRet, 0, CRecordMgr::EAIT_ITEMEXCHANGE, wExchangeIdx);
    //背包满
    if((NULL == poItem) && (ECIR_FAILED_BAG_FULL == eRet))
    {
        SDStrcpy(stAck.aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("BAG_FULL") .c_str());
        return;
    }
    else if(NULL == poItem)
    {
        SDStrcpy(stAck.aszErrInfo, CMsgDefMgr::Instance()->GetErrMsg("INTERNAL_ERROR") .c_str());
        return;
    }

    CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(stProp.wExchangeItemKindID, stProp.wExchangeItemNum, stAck.stExchangeItemInfo);

    //清除所需道具
    for(map<CGoods*, UINT16>::iterator itr = mapNeedGoods.begin(); itr != mapNeedGoods.end(); itr++)
    {
        CGoods* poGoods = itr->first;
        if(NULL == poGoods)
        {
            continue;
        }
        //poGoods->GetDT_GOODS_DATA().wPileCount -= itr->second;
        ////日志
        //CRecordMgr::Instance()->RecordInfo( poPlayer->GetID(), ERM_DECITEM, CRecordMgr::EDIT_MAKEEQUIP, poGoods->GetItemKindID(), SDTimeToString(SDNow()), poGoods->GetDT_GOODS_DATA().wPileCount,
        //	poPlayer->GetLevel(), poPlayer->GetDT_PLAYER_BASE_DATA().byVipLevel, itr->second);

        //扣除操作
        poGoods->DecPileNum(itr->second, CRecordMgr::EDIT_MAKEEQUIP);


        if(0 == poGoods->GetCurPileNum())
        {
            poPlayer->OnDeleteItem(poGoods);
        }
    }


    GetItemExchange(poPlayer, stAck.byExchangeNum, stAck.astExchangeInfo);
    poPlayer->GetNewEquipPrompt(stAck.stNewEquipPromptInfo);
}

BOOL CItemExchangeMgr::HaveExchangeActivity(CPlayer* poPlayer)
{
    UINT64 qwCurTime = SDTimeSecs();
    for(CItemExchangePropMapItr itr = m_mapItemExchangeProp.begin();  itr != m_mapItemExchangeProp.end(); itr++)
    {
        SItemExchangeProp& stProp = itr->second;

        if((stProp.qwStartTime > qwCurTime) || (stProp.qwEndTime <= qwCurTime))
        {
            continue;
        }

        return TRUE;
    }

    return FALSE;
}

