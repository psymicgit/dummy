///<------------------------------------------------------------------------------
//< @file:   logic\pet\petmng.h
//< @author: yewenhui10@qq.com
//< @date:   2014年8月6日 16:45:17
//< @brief:  宠物逻辑管理
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------
#pragma once
#ifndef _PET_MNG_H_ 
#define _PET_MNG_H_

#include "logic/base/singleton.h"
#include "logic/pet/petdef.h"
#include "logic/item/goods/goodspropmgr.h"

class PetBaseCfg;
class CPet;
class CPlayer;
class CPlayerPet;

class CPetMng
	: public Singleton<CPetMng>
{
public:
	CPetMng();
	~CPetMng();

	// 进入宠物副本
	UINT8		OpenPetRep(CPlayer* pPlayer, const PKT_CLIGS_PETREP_INFO_REQ& rMsgReq, PKT_CLIGS_PETREP_INFO_ACK& rMsgAck);
	// 召唤
	UINT8		ProductPet(CPlayer* pPlayer, const PKT_CLIGS_PRODUCT_PET_REQ& rMsgReq, PKT_CLIGS_PRODUCT_PET_ACK& rMsgAck);

	// 使用宠物道具获取宠物
	UINT8		UsePetItem(CPlayer* pPlayer, const SGoodsProp* pGoodsProp);

	// 打开宠物信息界面
	UINT8		OpenPetInformationPanel(CPlayer* pPlayer, const PKT_CLIGS_OPEN_PET_PANEL_REQ& rMsgReq, PKT_CLIGS_OPEN_PET_PANEL_ACK& rMsgAck);

	// 打开升星界面
	UINT8		OpenStarPanel(CPlayer* pPlayer, const PKT_CLIGS_OPEN_STAR_PANEL_REQ& rMsgReq, PKT_CLIGS_OPEN_STAR_PANEL_ACK& rMsgAck);
	// 升星
	UINT8		UpPetStar(CPlayer* pPlayer, const PKT_CLIGS_UP_STAR_LV_REQ& rMsgReq, PKT_CLIGS_UP_STAR_LV_ACK& rMsgAck);

	// 喂养
	UINT8		OpenPetFeedFanel(CPlayer* pPlayer, const PKT_CLIGS_OPEN_PETFEED_REQ& rMsgReq, PKT_CLIGS_OPEN_PETFEED_ACK& rMsgAck);
	UINT8		FirstDraw(CPlayer* pPlayer, const PKT_CLIGS_FIRST_DRAW_REQ& rMsgReq, PKT_CLIGS_FIRST_DRAW_ACK& rMsgAck);
	UINT8		ResetDraw(CPlayer* pPlayer, const PKT_CLIGS_RESET_DRAW_REQ& rMsgReq, PKT_CLIGS_RESET_DRAW_ACK& rMsgAck);
	UINT8		ConfirmFeed(CPlayer* pPlayer, const PKT_CLIGS_CONFIRM_FEED_REQ& rMsgReq, PKT_CLIGS_CONFIRM_FEED_ACK& rMsgAck);

	// 融合
	UINT8		PetMerge(CPlayer* pPlayer, const PKT_CLIGS_MERGE_PET_REQ& rMsgReq, PKT_CLIGS_MERGE_PET_ACK& rMsgAck);

	// 宠物商城
	UINT8		OnOpenPetStore(CPlayer* pPlayer, const PKT_CLIGS_OPEN_PET_STORE_REQ& rMsgReq, PKT_CLIGS_OPEN_PET_STORE_ACK& rMsgAck);
	UINT8		BuyStoreItem(CPlayer* pPlayer, const PKT_CLIGS_BUY_PET_STORE_ITEM_REQ& rMsgReq, PKT_CLIGS_BUY_PET_STORE_ITEM_ACK& rMsgAck);

	// 洗资质
	UINT8		OpenWashQuality(CPlayer* pPlayer, const PKT_CLIGS_OPEN_WASH_QUALITY_REQ& rMsgReq, PKT_CLIGS_OPEN_WASH_QUALITY_ACK& rMsgAck);
	UINT8		WashQuality(CPlayer* pPlayer, const PKT_CLIGS_WASH_QUALITY_REQ& rMsgReq, PKT_CLIGS_WASH_QUALITY_ACK& rMsgAck);

	// 改变宠物状态
	UINT8		ChangePetState(CPlayer* pPlayer, const PKT_CLIGS_CHANGE_PET_STATE_REQ& rMsgReq, PKT_CLIGS_CHANGE_PET_STATE_ACK& rMsgAck);

	// 继承
	UINT8		OpenPetInherit(CPlayer* pPlayer, const PKT_CLIGS_OPEN_PET_INHERIT_REQ& rMsgReq, PKT_CLIGS_OPEN_PET_INHERIT_ACK& rMsgAck);
	UINT8		InheritPet(CPlayer* pPlayer, const PKT_CLIGS_PET_INHERIT_REQ& rMsgReq, PKT_CLIGS_PET_INHERIT_ACK& rMsgAck);
	
	// 打开炼化界面
	UINT8		OpenRefinePetPanel(CPlayer* pPlayer, const PKT_CLIGS_OPEN_REFINEPET_REQ& rMsgReq, PKT_CLIGS_OPEN_REFINEPET_ACK& rMsgAck);
	// 炼化宠物获得宠物
	UINT8		RefinePet(CPlayer* pPlayer, const PKT_CLIGS_REFINEPET_REQ& rMsgReq, PKT_CLIGS_REFINEPET_ACK& rMsgAck);

	// 打开人物宠物信息面板
	UINT8		OpenPlayerPetPanel(CPlayer* pPlayer, const PKT_CLIGS_OPEN_PLAYERPET_PANEL_REQ& rMsgReq, PKT_CLIGS_OPEN_PLAYERPET_PANEL_ACK& rMsgAck);

	// 请求玩家身上所有宠物的信息
	UINT8		RequestAllPetInfo(CPlayer* pPlayer, const PKT_CLIGS_REQUEST_ALL_PET_INFO_REQ& rMsgReq, PKT_CLIGS_REQUEST_ALL_PET_INFO_ACK& rMsgAck);

	const CPet*	CreateAndAddPet(CPlayer* pPlayer, const PetBaseCfg* pPetBaseCfg, UINT8 ucPetLv);

	UINT16		OnOpenPetRule(CPlayer* poPlayer, PKT_CLIGS_OPEN_PET_RULE_ACK& stAck);

	// 宠物类型等级
	UINT8		GetPetTypeLevel(UINT8 ucPetType);

	UINT16		GetBattlePetInfo(CPlayer* poPlayer, PKT_CLIGS_PLAYER_INFO_PET_ACK& stAck);
	UINT16		OnOpenNeiDan(CPlayer* poPlayer, PKT_CLIGS_OPEN_NEIDAN_REQ& stReq, PKT_CLIGS_OPEN_NEIDAN_ACK& stAck);
	UINT16		OnDressNeiDan(CPlayer* poPlayer, PKT_CLIGS_DRESS_NEIDAN_REQ& stReq, PKT_CLIGS_DRESS_NEIDAN_ACK& stAck);
	UINT16		OnUpdataNeiDan(CPlayer* poPlayer, PKT_CLIGS_UPDATA_NEIDAN_REQ& stReq, PKT_CLIGS_UPDATA_NEIDAN_ACK& stAck);
private:
	UINT32		calcFruitSocre(const UINT8* pFruitType);
	UINT8		getRandomFruit();
	BOOL		fillPlayerData(CPlayerPet* pPlayerPet, DT_PET_CLI_DATA_LST& rAllPetCliData);
	UINT16		getInheritPetInfo(CPlayerPet* poPlayer, UINT16 wPetID, UINT16& wPetNum, DT_PET_AFTER_INHERIT_DATA* astPetCoinAfterInheritData, DT_PET_AFTER_INHERIT_DATA* astPetGoldAfterInheritData);
	UINT8		getRandomPetTypeByTypeLevel(UINT8 ucPetTypeLevel);

};

#endif // _PET_MNG_H_
