///<------------------------------------------------------------------------------
//< @file:   logic\other\exchange.cpp
//< @author: Kang_zhiquan
//< @date:   2014年8月27日 21:18:26
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "exchange.h"
#include "framework/gsapi.h"
#include "logic/item/itemmgr.h"
#include "sdloggerimpl.h"
#include "logic/player/player.h"
#include "db/dbmgr.h"

IMPLEMENT_SINGLETON_PROPMGR(CExchangePropMgr)

BOOL CExchangePropMgr::Init()
{
	m_mapExchangeProp.clear();
	if ( !LoadCfgFromDB())
	{
		return FALSE;
	}
	return TRUE;
}

VOID CExchangePropMgr::UnInit()
{
	m_mapExchangeProp.clear();
}

BOOL CExchangePropMgr::LoadCfgFromDB()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();
	if(NULL == pDBSession)
	{
		return FALSE;
	}
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql("exchangestoreprop").c_str(), &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);      
		return FALSE;
	}	
	// 打印查询的表结果
	while(pRes->GetRecord())// 当有行结果时
	{
		SExchangeItemProp stProp = {0};
		stProp.wGoodsID = SDAtou(pRes->GetFieldValueByName("ItemID"));

		stProp.byResType = SDAtou(pRes->GetFieldValueByName("ResType"));
		stProp.dwResValue = SDAtou(pRes->GetFieldValueByName("ResValue"));

		m_mapExchangeProp[stProp.wGoodsID] = stProp;
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}
	return TRUE;
}

SExchangeItemProp* CExchangePropMgr::GetExchangeItem(UINT16 wGoodsID)
{
	CMapExchangeItemItr itr = m_mapExchangeProp.find(wGoodsID);
	if ( itr == m_mapExchangeProp.end())
	{
		return NULL;
	}
	return &itr->second;
}

VOID CExchangePropMgr::GetResInfo(CPlayer* poPlayer, DT_PLAYER_RES_LST_DATA& stResInfo)
{
	if ( NULL == poPlayer)
	{
		return ;
	}
	//资源显示
	UINT8 byResNum = 0;
	stResInfo.astPlayerRes[byResNum].byResKind = ACTE_COIN;
	stResInfo.astPlayerRes[byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwCoin;
	byResNum++;
	//阅历
	stResInfo.astPlayerRes[byResNum].byResKind = ACTE_STORY;
	stResInfo.astPlayerRes[byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwStory;
	byResNum++;
	//功力
	stResInfo.astPlayerRes[byResNum].byResKind = ACTE_PRUPLEGAS;
	stResInfo.astPlayerRes[byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwPurpleGas;
	byResNum++;
	stResInfo.astPlayerRes[byResNum].byResKind = ACTE_JINGJIE;
	stResInfo.astPlayerRes[byResNum].qwResValue = poPlayer->GetDT_PLAYER_BASE_DATA().qwJingJie;
	byResNum++;
	stResInfo.byResNum = byResNum;
}

UINT16 CExchangePropMgr::OpenExchagneStroe(CPlayer* poPlayer, DT_PLAYER_RES_LST_DATA& stResInfo, DT_GOODS_EXCHANGE_LST_INFO& stItemInfo)
{
	if ( NULL == poPlayer)
	{
		return ERR_COMMON::ID_OTHER_ERR;
	}
	GetResInfo(poPlayer, stResInfo);

	//物品显示
	UINT8 byItemNum = 0;
	for ( CMapExchangeItemItr itr = m_mapExchangeProp.begin(); itr != m_mapExchangeProp.end(); itr++, byItemNum++)
	{
		CItemMgr::Instance()->GetDT_ITEM_DATA_CLI3(itr->second.wGoodsID, 1,stItemInfo.astGoodsInfo[byItemNum].stGoodsInfo);
		stItemInfo.astGoodsInfo[byItemNum].stResReq.byResKind = itr->second.byResType;
		stItemInfo.astGoodsInfo[byItemNum].stResReq.qwResValue = itr->second.dwResValue;
	}
	stItemInfo.wGoodsNum = byItemNum;
	

	return ERR_OPEN_STORE::ID_SUCCESS;
}

UINT16 CExchangePropMgr::ExchangeItem(CPlayer* poPlayer, PKT_CLIGS_EXCHANGE_GOODS_REQ& stReq, PKT_CLIGS_EXCHANGE_GOODS_ACK& stAck)
{
	if ( NULL == poPlayer)
	{
		return ERR_COMMON::ID_OTHER_ERR;
	}

	if ( 0 == stReq.wGoodsNum)
	{
		return ERR_EXCHANGE_GOODS::ID_NUM_NOT_ENOUGH;
	}
	CMapExchangeItemItr itr = m_mapExchangeProp.find(stReq.wGoodsID);
	if ( itr == m_mapExchangeProp.end())
	{
		return ERR_EXCHANGE_GOODS::ID_NOT_ITEM_ID;
	}

	if( !poPlayer->CheckRes( itr->second.byResType, itr->second.dwResValue * stReq.wGoodsNum))
	{
		return ERR_EXCHANGE_GOODS::ID_RES_NOT_ENOUGH;
	}
	UINT8 byItemType = CItemMgr::Instance()->GetItemKindBYKindID(stReq.wGoodsID);
	ECreateItemRet eRet;
	if ( EIK_GOODS == byItemType)
	{
		CItemMgr::Instance()->CreateItemIntoBag( poPlayer, stReq.wGoodsID, stReq.wGoodsNum, eRet, 1, CRecordMgr::EAIT_EXCHANGE_STORE);
	}
	else
	{
		if( poPlayer->GetBag().GetIdleSize() < stReq.wGoodsNum)
		{
			eRet = ECIR_FAILED_BAG_FULL;
		}
		else
		{
			for ( UINT16 wIdx = 0; wIdx < stReq.wGoodsNum; wIdx++)
			{
				CItemMgr::Instance()->CreateItemIntoBag(poPlayer, stReq.wGoodsID, 1, eRet, 1, CRecordMgr::EAIT_EXCHANGE_STORE);
			}
		}
	}
	if ( ECIR_FAILED_BAG_FULL == eRet )
	{
		return ERR_EXCHANGE_GOODS::ID_BAG_IS_FULL;
	}
	switch (itr->second.byResType)
	{
	case ACTE_COIN:
		poPlayer->DecCoin( itr->second.dwResValue * stReq.wGoodsNum, CRecordMgr::EDCT_EXCHANGE_STORE);
		break;
	case ACTE_JINGJIE:
		poPlayer->DecJingJie(itr->second.dwResValue * stReq.wGoodsNum, CRecordMgr::EDJT_EXCHANGE_STORE);
		break;
	case ACTE_STORY:
		poPlayer->DecStory(itr->second.dwResValue * stReq.wGoodsNum, CRecordMgr::EDST_EXCHANGE_STORE);
		break;
	case ACTE_PRUPLEGAS:
		poPlayer->DecPurpleGas(itr->second.dwResValue * stReq.wGoodsNum, CRecordMgr::EDPGT_EXCHANGE_STORE);
		break;
	}

	//同步数据
	GetResInfo(poPlayer, stAck.stResInfo);
	CBag& oBag = poPlayer->GetBag();
	
	stAck.wItemsID = stReq.wGoodsID;
	stAck.wItemNum = oBag.GetGoodsPileNum(stAck.wItemsID);
	return ERR_EXCHANGE_RESULT_ID::ID_SUCCESS;
}