#include "jsonencourage.h"
#include "logic/player/player.h"
#include "logic/hero/hero.h"
#include "logic/task/taskmgr.h"
#include "logic/awaken/awaken.h"
#include "protocol/server/protocligs.h"
#include "logic/other/phystrength.h"


void ParseResItem(CJsonResItemVector &vecJsonResItem, Json::Value &item, ACTE_KIND_TYPE byKindType)
{
    if (item.isNull())
    {
        return ;
    }

    if (!item.isObject())
    {
        return ;
    }

    CJsonResItem stItem;
    if (!item["value"].isInt())
    {
        return ;
    }
    else
    {
        stItem.dwKindValue	= item["value"].asInt();
    }

    if (item["num"].isInt())
    {
        stItem.byNum			= item["num"].asInt();
    }
    else
    {
        stItem.byNum			= 1;
    }

    if (item["percent"].isInt())
    {
        stItem.wPercent		= item["percent"].asInt();
    }
    else
    {
        stItem.wPercent		= 100;
    }

    stItem.byKindType	= byKindType;

    if(0 != stItem.dwKindValue)
    {
        vecJsonResItem.push_back(stItem);
    }
}

void ParseAttrJson(CVecJsonAttrBase& vecAttrBase, Json::Value &jsonValue, EBattleAttributeKind byKindType)
{
	if (jsonValue.isNull())
	{
		return ;
	}

	if (!jsonValue.isObject())
	{
		return ;
	}

	DT_ATTR_BASE_DATA stAttrBase;
	if (!jsonValue["value"].isInt())
	{
		return ;
	}
	else
	{
		stAttrBase.dwAttrValue	= jsonValue["value"].asInt();
	}

	if (jsonValue["type"].isInt())
	{
		stAttrBase.byValueType	= jsonValue["type"].asInt();
	}
	else
	{
		stAttrBase.byValueType = 1;
	}


	stAttrBase.byKindID	= byKindType;

	if(0 != stAttrBase.dwAttrValue)
	{
		vecAttrBase.push_back(stAttrBase);
	}
}


BOOL JsonParseResItem(CJsonResItemVectorInfo &JsonResItemVectorInfo, string strJsonValues)
{
    Json::Value root;
    Json::Reader reader;
    if( !reader.parse( strJsonValues, root ) )
    {
        SYS_CRITICAL( _SDT( "json parse error, result:%s" ), strJsonValues.c_str() );
        return FALSE;
    }

    //检测数据个数，如果为零刚为出错，防止崩溃
    if (!root.isObject())
    {
        SYS_CRITICAL( _SDT( "json size error, result:%s" ), strJsonValues.c_str() );
        return FALSE;
    }
    return JsonParseResItem(JsonResItemVectorInfo, root);
}


BOOL JsonParseResItem(CJsonResItemVectorInfo &JsonResItemVectorInfo, Json::Value &root)
{
    //检测获取资源的类型
    if (root["empercent"].isString() && root["empercent"].asString() == "all")
    {
        JsonResItemVectorInfo.emPercent	 = em_All;
    }
    else
    {
        JsonResItemVectorInfo.emPercent	 = em_SINGLE ;
    }

    //解析物品的个数
    if (root["item"].isArray())
    {
        Json::Value items = root["item"];
        for (UINT32 idx = 0; idx < items.size(); idx++)
        {
            ParseResItem(JsonResItemVectorInfo.vecJsonResItem, items[idx], ACTE_GOOD);
        }
    }


	if (root["soul"].isArray()) {
		Json::Value items = root["soul"];
		for (UINT32 idx = 0; idx < items.size(); idx++) {
			ParseResItem(JsonResItemVectorInfo.vecJsonResItem, items[idx], ACTE_SOUL);
		}
	}

    //解析资源数据的值
    ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["hero"],			ACTE_HERO);
    ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["coin"],			ACTE_COIN);
    ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["gold"],			ACTE_GOLD);
    ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["phystrength"],		ACTE_PHYSTRENGTH);
    ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["science"],			ACTE_SCIENCE);
    ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["story"],			ACTE_STORY);
    ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["bluegas"],			ACTE_BLUEGAS);
    ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["pruplegas"],		ACTE_PRUPLEGAS);
    ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["jingjie"],			ACTE_JINGJIE);
    ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["doorstribute"],	ACTE_DoorsTribute);
    ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["factionfunds"],	ACTE_FactionFunds);
    ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["factiontaskintegral"],	ACTE_FactionTaskIntegral);
    ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["studyclip"],			ACTE_StudyClip);
	ParseResItem(JsonResItemVectorInfo.vecJsonResItem, root["studyexp"], ACTE_STUDYEXP);
    return JsonResItemVectorInfo.vecJsonResItem.size() > 0 ? TRUE : FALSE;

}


UINT16 JsonGetResItem(CJsonResItem &stItem, CPlayer* poPlayer, DT_RES_ITEM_DATA &stGainResItemData, DT_RSYNC_RES_ITEM_DATA &stRsyncResItemData, DT_RES_NUM_ITEM_DATA &stResNumItemData, CRecordMgr::EAddItemType emOpType)
{
    switch (stItem.byKindType)
    {
    case	ACTE_GOOD://物品
    {
        ECreateItemRet ItemRet = ECIR_SUCCESS;
        CItem* poItem = CItemMgr::Instance()->CreateItemIntoBag(poPlayer, stItem.dwKindValue, stItem.byNum, ItemRet, 0, emOpType);
        if(NULL == poItem)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: CreateItem failed[%d], ItemKindID=%d !"), MSG_MARK, ItemRet, stItem.dwKindValue);
        }
        //获取的资源
        CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(poItem->GetItemKindID(), stItem.byNum, stGainResItemData.astItemList[stGainResItemData.byItemNum++]);

        DT_ITEM_DATA &stDT_ITEM_DATA = stResNumItemData.astItemList[stResNumItemData.byItemNum++];
        stDT_ITEM_DATA.wKindID		= stItem.dwKindValue;
        stDT_ITEM_DATA.wNum		= stItem.byNum;
    }
    break;
    case	ACTE_HERO://武将
    {
        BOOL	bAlreadyRecuit = FALSE;
        CKindID2HeroShmemMap& mapKind2Hero = poPlayer->GeHeroMap();
        CHero* poHero = mapKind2Hero.GetData(stItem.dwKindValue);
        if(NULL != poHero)
        {
            if( EHP_RECRUIT != poHero->GetDT_HERO_BASE_DATA().byPos )
            {
                bAlreadyRecuit = TRUE;
            }
        }
        else
        {
            poHero = poPlayer->NewHeroNoBuild(stItem.dwKindValue);
        }

        //如果已经携带则不判断可携带数目
        if( !bAlreadyRecuit )
        {
            CInstance &oInstance = poPlayer->GetInstance();
            //判断可携带数目
            DT_INSTANCE_ID& stInstanceID = oInstance.GetMaxCommonInstanceID();
            SHeroUnlockProp* pstHeroUnlockProp = CInstancePropMgr::Instance()->GetUnlockHero(stInstanceID.wSceneIdx, stInstanceID.byTownIdx, stInstanceID.byInstanceIdx);
            if(NULL == pstHeroUnlockProp)
            {
                SYS_CRITICAL(_SDT("[%s: %d]: ERR_COMMON::ID_OTHER_ERR!"), MSG_MARK);
                break;
            }
            if(poPlayer->CkCarryHeroFull())
            {
                return ERR_RECV_ACTIVITY_ENCOURRAGE::ID_HERO_CARRY_FULL;
            }
        }

        //招募,携带
        if (NULL != poHero)
        {
            poPlayer->GetAllCarryHeroMap().AddData(stItem.dwKindValue, poHero);
            poHero->OnRecuit();//标志为已招募
            CTaskMgr::Instance()->OnRecruitHero(poPlayer, stItem.dwKindValue);
        }
    }
    break;
    case	ACTE_COIN://铜钱
    {
        poPlayer->AddCoin(stItem.dwKindValue, CRecordMgr::EACT_ENCOURAGE);

        DT_RES_DATA &stDT_RES_DATA = stGainResItemData.astResList[stGainResItemData.byResNum++];
        stDT_RES_DATA.byResKind	= stItem.byKindType;
        stDT_RES_DATA.qwResValue	= stItem.dwKindValue;

        //同步资源
        DT_RES_DATA &stRSYNC_DT_RES_DATA = stRsyncResItemData.astResList[stRsyncResItemData.byResNum++];
        stRSYNC_DT_RES_DATA.byResKind	= stItem.byKindType;
        stRSYNC_DT_RES_DATA.qwResValue	= poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;

        DT_RES_DATA &stDT_RES_DATA1 = stResNumItemData.astResList[stResNumItemData.byResNum++];
        stDT_RES_DATA1.byResKind	= stItem.byKindType;
        stDT_RES_DATA1.qwResValue	= stItem.dwKindValue;
    }
    break;
    case	ACTE_GOLD://元宝
    {
        poPlayer->AddGold(stItem.dwKindValue, CRecordMgr::EGGT_ENCOURAGE);

        DT_RES_DATA &stDT_RES_DATA = stGainResItemData.astResList[stGainResItemData.byResNum++];
        stDT_RES_DATA.byResKind	= stItem.byKindType;
        stDT_RES_DATA.qwResValue	= stItem.dwKindValue;

        //同步资源
        DT_RES_DATA &stRSYNC_DT_RES_DATA = stRsyncResItemData.astResList[stRsyncResItemData.byResNum++];
        stRSYNC_DT_RES_DATA.byResKind	= stItem.byKindType;
        stRSYNC_DT_RES_DATA.qwResValue	= poPlayer->GetDT_PLAYER_BASE_DATA().qwGold;

        DT_RES_DATA &stDT_RES_DATA1 = stResNumItemData.astResList[stResNumItemData.byResNum++];
        stDT_RES_DATA1.byResKind	= stItem.byKindType;
        stDT_RES_DATA1.qwResValue	= stItem.dwKindValue;
    }
    break;
    case	ACTE_PHYSTRENGTH://体力
    {
        poPlayer->AddPhyStrength(stItem.dwKindValue, CRecordMgr::EAPST_ENCOURAGE);

        DT_RES_DATA &stDT_RES_DATA = stGainResItemData.astResList[stGainResItemData.byResNum++];
        stDT_RES_DATA.byResKind	= stItem.byKindType;
        stDT_RES_DATA.qwResValue	= stItem.dwKindValue;

        //同步资源
        //DT_RES_DATA &stRSYNC_DT_RES_DATA = stRsyncResItemData.astResList[stRsyncResItemData.byResNum++];
        //stRSYNC_DT_RES_DATA.byResKind	= stItem.byKindType;
        //stRSYNC_DT_RES_DATA.qwResValue	= poPlayer->GetDT_PLAYER_BASE_DATA().qwp;

        DT_RES_DATA &stDT_RES_DATA1 = stResNumItemData.astResList[stResNumItemData.byResNum++];
        stDT_RES_DATA1.byResKind	= stItem.byKindType;
        stDT_RES_DATA1.qwResValue	= stItem.dwKindValue;
    }
    break;
    case	ACTE_SCIENCE://器魂点
    {
        poPlayer->AddScience(stItem.dwKindValue, CRecordMgr::EAST_ENCOURAGE);

        DT_RES_DATA &stDT_RES_DATA = stGainResItemData.astResList[stGainResItemData.byResNum++];
        stDT_RES_DATA.byResKind	= stItem.byKindType;
        stDT_RES_DATA.qwResValue	= stItem.dwKindValue;

        //同步资源
        DT_RES_DATA &stRSYNC_DT_RES_DATA = stRsyncResItemData.astResList[stRsyncResItemData.byResNum++];
        stRSYNC_DT_RES_DATA.byResKind	= stItem.byKindType;
        stRSYNC_DT_RES_DATA.qwResValue	= poPlayer->GetDT_PLAYER_BASE_DATA().qwScience;

        DT_RES_DATA &stDT_RES_DATA1 = stResNumItemData.astResList[stResNumItemData.byResNum++];
        stDT_RES_DATA1.byResKind	= stItem.byKindType;
        stDT_RES_DATA1.qwResValue	= stItem.dwKindValue;
    }
    break;
    case	ACTE_STORY://阅历
    {
        poPlayer->AddStory(stItem.dwKindValue, CRecordMgr::EASTT_ENCOURAGE);

        DT_RES_DATA &stDT_RES_DATA = stGainResItemData.astResList[stGainResItemData.byResNum++];
        stDT_RES_DATA.byResKind	= stItem.byKindType;
        stDT_RES_DATA.qwResValue	= stItem.dwKindValue;

        //同步资源
        DT_RES_DATA &stRSYNC_DT_RES_DATA = stRsyncResItemData.astResList[stRsyncResItemData.byResNum++];
        stRSYNC_DT_RES_DATA.byResKind	= stItem.byKindType;
        stRSYNC_DT_RES_DATA.qwResValue	= poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;

        DT_RES_DATA &stDT_RES_DATA1 = stResNumItemData.astResList[stResNumItemData.byResNum++];
        stDT_RES_DATA1.byResKind	= stItem.byKindType;
        stDT_RES_DATA1.qwResValue	= stItem.dwKindValue;
    }
    break;
    case	ACTE_BLUEGAS://蓝气
    {
        poPlayer->AddBlueGas(stItem.dwKindValue, CRecordMgr::EABGT_ENCOURAGE);

        DT_RES_DATA &stDT_RES_DATA = stGainResItemData.astResList[stGainResItemData.byResNum++];
        stDT_RES_DATA.byResKind	= stItem.byKindType;
        stDT_RES_DATA.qwResValue	= stItem.dwKindValue;

        //同步资源
        DT_RES_DATA &stRSYNC_DT_RES_DATA = stRsyncResItemData.astResList[stRsyncResItemData.byResNum++];
        stRSYNC_DT_RES_DATA.byResKind	= stItem.byKindType;
        stRSYNC_DT_RES_DATA.qwResValue	= poPlayer->GetDT_PLAYER_BASE_DATA().qwBlueGas;

        DT_RES_DATA &stDT_RES_DATA1 = stResNumItemData.astResList[stResNumItemData.byResNum++];
        stDT_RES_DATA1.byResKind	= stItem.byKindType;
        stDT_RES_DATA1.qwResValue	= stItem.dwKindValue;
    }
    break;
    case	ACTE_PRUPLEGAS://紫气
    {
        poPlayer->AddPurpleGas(stItem.dwKindValue, CRecordMgr::EAPGT_ENCOURAGE);

        DT_RES_DATA &stDT_RES_DATA = stGainResItemData.astResList[stGainResItemData.byResNum++];
        stDT_RES_DATA.byResKind	= stItem.byKindType;
        stDT_RES_DATA.qwResValue	= stItem.dwKindValue;

        //同步资源
        DT_RES_DATA &stRSYNC_DT_RES_DATA = stRsyncResItemData.astResList[stRsyncResItemData.byResNum++];
        stRSYNC_DT_RES_DATA.byResKind	= stItem.byKindType;
        stRSYNC_DT_RES_DATA.qwResValue	= poPlayer->GetDT_PLAYER_BASE_DATA().qwPurpleGas;

        DT_RES_DATA &stDT_RES_DATA1 = stResNumItemData.astResList[stResNumItemData.byResNum++];
        stDT_RES_DATA1.byResKind	= stItem.byKindType;
        stDT_RES_DATA1.qwResValue	= stItem.dwKindValue;
    }
    break;
    case	ACTE_JINGJIE://境界
    {
        poPlayer->AddJingJie(stItem.dwKindValue, CRecordMgr::EAJT_ENCOURAGE);

        DT_RES_DATA &stDT_RES_DATA = stGainResItemData.astResList[stGainResItemData.byResNum++];
        stDT_RES_DATA.byResKind	= stItem.byKindType;
        stDT_RES_DATA.qwResValue	= stItem.dwKindValue;

        //同步资源
        DT_RES_DATA &stRSYNC_DT_RES_DATA = stRsyncResItemData.astResList[stRsyncResItemData.byResNum++];
        stRSYNC_DT_RES_DATA.byResKind	= stItem.byKindType;
        stRSYNC_DT_RES_DATA.qwResValue	= poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;

        DT_RES_DATA &stDT_RES_DATA1 = stResNumItemData.astResList[stResNumItemData.byResNum++];
        stDT_RES_DATA1.byResKind	= stItem.byKindType;
        stDT_RES_DATA1.qwResValue	= stItem.dwKindValue;
    }
    break;
    case ACTE_DoorsTribute: //门贡
    {
        CFaction* poFaction = poPlayer->GetFaction();
        if (NULL == poFaction)
        {
            break;
        }
        poFaction->AddDoorsTribute(poPlayer->GetID(), stItem.dwKindValue, CRecordMgr::EAJT_ENCOURAGE);

        DT_RES_DATA &stDT_RES_DATA = stGainResItemData.astResList[stGainResItemData.byResNum++];
        stDT_RES_DATA.byResKind	= stItem.byKindType;
        stDT_RES_DATA.qwResValue	= stItem.dwKindValue;

        DT_RES_DATA &stDT_RES_DATA1 = stResNumItemData.astResList[stResNumItemData.byResNum++];
        stDT_RES_DATA1.byResKind	= stItem.byKindType;
        stDT_RES_DATA1.qwResValue	= stItem.dwKindValue;
    }
    break;
    case	ACTE_FactionFunds:  //门派资金
    {
        CFaction* poFaction = poPlayer->GetFaction();
        if (NULL != poFaction)
        {
            poFaction->AddFactionFunds(stItem.dwKindValue, CRecordMgr::EAJT_ENCOURAGE);
        }

        DT_RES_DATA &stDT_RES_DATA = stGainResItemData.astResList[stGainResItemData.byResNum++];
        stDT_RES_DATA.byResKind	= stItem.byKindType;
        stDT_RES_DATA.qwResValue	= stItem.dwKindValue;

        DT_RES_DATA &stDT_RES_DATA1 = stResNumItemData.astResList[stResNumItemData.byResNum++];
        stDT_RES_DATA1.byResKind	= stItem.byKindType;
        stDT_RES_DATA1.qwResValue	= stItem.dwKindValue;
    }
    break;
    case	ACTE_FactionTaskIntegral: //门派任务积分
    {

    }
    break;
    case ACTE_StudyClip://
    {
        CAwaken *poAwaken = poPlayer->GetAwaken();
        if ( NULL != poAwaken )
        {
            poAwaken->AddStudyClip(stItem.dwKindValue, CRecordMgr::EASCTUDYTCLIP_ENCOURAGE);
        }

        DT_RES_DATA &stDT_RES_DATA = stGainResItemData.astResList[stGainResItemData.byResNum++];
        stDT_RES_DATA.byResKind	= stItem.byKindType;
        stDT_RES_DATA.qwResValue	= stItem.dwKindValue;

        DT_RES_DATA &stDT_RES_DATA1 = stResNumItemData.astResList[stResNumItemData.byResNum++];
        stDT_RES_DATA1.byResKind	= stItem.byKindType;
        stDT_RES_DATA1.qwResValue	= stItem.dwKindValue;
    }
    break;
	case ACTE_SOUL:
	{
		poPlayer->AddReward(ACTE_SOUL, stItem.dwKindValue, stItem.byNum);
	}
	break;
	case ACTE_STUDYEXP://
	{
		CAwaken *poAwaken = poPlayer->GetAwaken();
		if ( NULL != poAwaken )
		{
			poAwaken->AddStudyExpForGm(stItem.dwKindValue);
		}
	}
	break;
    default:
        break;
    }
    return 0;
}


UINT16 JsonGetResItemVec(CJsonResItemVectorInfo &JsonResItemVectorInfo, CPlayer* poPlayer, DT_RES_ITEM_DATA &stGainResItemData, DT_RSYNC_RES_ITEM_DATA &stRsyncResItemData, DT_RES_NUM_ITEM_DATA &stResNumItemData, CRecordMgr::EAddItemType emOpType)
{
    UINT16 wErrCode = 0;
    if(em_All == JsonResItemVectorInfo.emPercent)
    {
        CRandomVec vecRandom;
        UINT32 wIdx = 0;
        for(wIdx = 0; wIdx < JsonResItemVectorInfo.vecJsonResItem.size(); wIdx++)
        {
            vecRandom.push_back(JsonResItemVectorInfo.vecJsonResItem[wIdx].wPercent);
        }

        //随机取一个
        INT32 nRandomHitIdx = GetRandomIdx(vecRandom);
        if(RANDOM_UNHIT == nRandomHitIdx)
        {
            return FALSE;
        }

        if (nRandomHitIdx >= (INT32)JsonResItemVectorInfo.vecJsonResItem.size())
        {
            return FALSE;
        }


        CJsonResItem &stItem = JsonResItemVectorInfo.vecJsonResItem[nRandomHitIdx];
        if (ACTE_GOOD == stItem.byKindType)
        {
            vector<UINT16> vecItemID;
            vecItemID.push_back(stItem.dwKindValue);
            if(CItemMgr::Instance()->CkBagFull(poPlayer, vecItemID))
            {
                return BAG_FULL_ERR;
            }
        }
        wErrCode = JsonGetResItem(stItem, poPlayer, stGainResItemData, stRsyncResItemData, stResNumItemData, emOpType);
        poPlayer->GetPhystrength().GetPhyStrengthInfo(stRsyncResItemData.stCurPhyStrengthInfo);
        return SUCCESS;
    }
    else
    {
        //检测背包物品
        CJsonResItemVectorItr itr;
        vector<UINT16> vecItemID;
        for(itr = JsonResItemVectorInfo.vecJsonResItem.begin(); itr != JsonResItemVectorInfo.vecJsonResItem.end(); itr++)
        {
            if (ACTE_GOOD == itr->byKindType)
            {
                vecItemID.push_back(itr->dwKindValue);
            }
        }

        if(CItemMgr::Instance()->CkBagFull(poPlayer, vecItemID))
        {
            return BAG_FULL_ERR;
        }

        UINT16 wErrCode = 0;
        for (itr = JsonResItemVectorInfo.vecJsonResItem.begin(); itr != JsonResItemVectorInfo.vecJsonResItem.end(); itr++)
        {

            CJsonResItem &stItem = *itr;
            CRandomVec	vecRandom;
            vecRandom.push_back(stItem.wPercent);

            //随机取一个
            INT32 nRandomHitIdx = GetRandomIdx(vecRandom);
            if(RANDOM_UNHIT == nRandomHitIdx)
            {
                continue;;
            }

            wErrCode = JsonGetResItem(stItem, poPlayer, stGainResItemData, stRsyncResItemData, stResNumItemData, emOpType);
            if (SUCCESS != wErrCode)
            {
                return wErrCode;
            }
        }
        poPlayer->GetPhystrength().GetPhyStrengthInfo(stRsyncResItemData.stCurPhyStrengthInfo);
        return SUCCESS;
    }
    return SUCCESS;
}

void	 JsonPackResItem(CJsonResItemVectorInfo& JsonResItemVectorInfo, DT_RES_ITEM_DATA &stDT_RES_ITEM_DATA)
{
    for (CJsonResItemVectorItr itr = JsonResItemVectorInfo.vecJsonResItem.begin(); itr != JsonResItemVectorInfo.vecJsonResItem.end(); itr++)
    {
        CJsonResItem &stItem =  *itr;
        switch (stItem.byKindType)
        {
        case	ACTE_GOOD://物品
        {
            CItemMgr::Instance()->GetDT_ITEM_DATA_CLI2(stItem.dwKindValue, stItem.byNum, stDT_RES_ITEM_DATA.astItemList[stDT_RES_ITEM_DATA.byItemNum++]);
        }
        break;
        case	ACTE_HERO://武将
        {

        }
        break;
        case	ACTE_COIN://铜钱
        case	ACTE_GOLD://元宝
        case	ACTE_PHYSTRENGTH://体力
        case	ACTE_SCIENCE://器魂点
        case	ACTE_STORY://阅历
        case	ACTE_BLUEGAS://蓝气
        case	ACTE_PRUPLEGAS://紫气
        case	ACTE_JINGJIE://境界
        case ACTE_DoorsTribute://门贡
        case ACTE_FactionFunds://门派资金
        case ACTE_FactionTaskIntegral://门派任务积分
        case ACTE_StudyClip:////乾坤碎片
        {
            DT_RES_DATA &stDT_RES_DATA = stDT_RES_ITEM_DATA.astResList[stDT_RES_ITEM_DATA.byResNum++];
            stDT_RES_DATA.byResKind	= stItem.byKindType;
            stDT_RES_DATA.qwResValue	= stItem.dwKindValue;
        }
        break;
        default:
            break;
        }
    }
    return ;
}

BOOL   JsonParseAttrJson(CVecJsonAttrBase &JsonAttrVectorInfo, string strJsonValues)
{
	Json::Value root;
	Json::Reader reader;
	if ( "" == strJsonValues )
	{
		return TRUE;
	}
	if( !reader.parse( strJsonValues, root ) )
	{
		SYS_CRITICAL( _SDT( "json parse error, result:%s" ), strJsonValues.c_str() );
		return FALSE;
	}

	//检测数据个数，如果为零刚为出错，防止崩溃
	if (!root.isObject())
	{
		SYS_CRITICAL( _SDT( "json size error, result:%s" ), strJsonValues.c_str() );
		return FALSE;
	}
	return JsonParseAttrJson(JsonAttrVectorInfo, root);
}

BOOL   JsonParseAttrJson(CVecJsonAttrBase &JsonAttrVectorInfo, Json::Value &root)
{


	//解析资源数据的值
	ParseAttrJson(JsonAttrVectorInfo, root["HP"],			EBAK_HP);
	ParseAttrJson(JsonAttrVectorInfo, root["Attack"],		EBAK_ATTACK);
	ParseAttrJson(JsonAttrVectorInfo, root["Defend"],		EBAK_DEFEND);
	ParseAttrJson(JsonAttrVectorInfo, root["HitRate"],		EBAK_HITRATE);
	ParseAttrJson(JsonAttrVectorInfo, root["DodgeRate"],	EBAK_DODGERATE);
	ParseAttrJson(JsonAttrVectorInfo, root["CritRate"],		EBAK_CRITRATE);
	ParseAttrJson(JsonAttrVectorInfo, root["DecRitRate"],	EBAK_DECRITRATE);
	
	return JsonAttrVectorInfo.size() > 0 ? TRUE : FALSE;
}