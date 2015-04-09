///<------------------------------------------------------------------------------
//< @file:   soulmanager.h
//< @author: yewenhui
//< @date:   2014年6月12日 17点10分
//< @brief:  武魂管理
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------
#ifndef _SOUL_MAMAGER_H_
#define _SOUL_MAMAGER_H_

#include "logic/base/singleton.h"
#include "logic/base/basedefine.h"
#include <sdtype.h>
#include "protocol/server/protocligs.h"

class CPlayer;
class CSoul;
class CBag;

class CSoulManager
	: public Singleton<CSoulManager>
{
	enum eSoulCostType {
		eSCT_Coin		= 2,
		eSCT_Gold		,
	};

	enum {
		BROCAST_STAR_LEVEL		= 6,
		FIVE_SATR_CNT			= 100,	// 五星卡探索几次才能获取
		SIX_SATR_CNT			= 200,	// 六星卡探索几次才能获取
	};

public:
	CSoulManager();
	~CSoulManager();

	// 
	UINT8		GetSoulRepInfo(CPlayer* pPlayer, PKT_CLIGS_SOULREP_INFO_ACK& rMsgAck);
	// 产出武魂
	UINT8		ProductSoul(CPlayer* pPlayer, PKT_CLIGS_PRODUCT_SOUL_REQ& rMsgReq, PKT_CLIGS_PRODUCT_SOUL_ACK& msgAck);
	// 装备武魂
	UINT8		EquipSoul(CPlayer* pPlayer, UINT16 unHeroId, UINT16 usSoulID, UINT32 unIndex);
	// 脱下武魂
	UINT8		UnDressSoul(CPlayer* pPlayer, UINT16 unHeroId, UINT16 usSoulID, UINT32 unIndex);
	// 替换战魂
	UINT8		OnReplaceSoul(CPlayer* pPlayer, PKT_CLIGS_HERO_REPLACE_SOUL_REQ& rMsgReq, PKT_CLIGS_HERO_REPLACE_SOUL_ACK& msgAck);

	// 打开吞噬战魂反馈
	UINT8		OpenDevourSoulAck(CPlayer* pPlayer, PKT_CLIGS_OPEN_DEVOUR_SOUL_REQ& rMsgReq, PKT_CLIGS_OPEN_DEVOUR_SOUL_ACK& rMsgAck);
	// 吞噬战魂
	UINT8		DevourSoul(CPlayer* pPlayer, PKT_CLIGS_DEVOUR_SOUL_REQ& rMsgReq, PKT_CLIGS_DEVOUR_SOUL_ACK& rMsgAck);
	// 打开洗练面板
	UINT8		OpenReflash(CPlayer* pPlayer, PKT_CLIGS_OPEN_REFLASH_SOUL_REQ& rMsgReq, PKT_CLIGS_OPEN_REFLASH_SOUL_ACK& rMsgAck);
	// 洗练战魂
	UINT8		ReflashSoul(CPlayer* pPlayer, PKT_CLIGS_REFLASH_SOUL_REQ& rMsgReq, tagPKT_CLIGS_REFLASH_SOUL_ACK& rMsgAck);
	// 保存洗练战魂
	UINT8		SaveReflashSoul(CPlayer* pPlayer, PKT_CLIGS_SAVE_REFLASH_SOUL_REQ& rMsgReq, PKT_CLIGS_SAVE_REFLASH_SOUL_ACK& rMsgAck);

	// 请求战魂兑换信息
	UINT8		SoulExchangeInfo(CPlayer* pPlayer, PKT_CLIGS_SOULEXCHANGE_INFO_ACK& rMsgAck);
	// 材料兑换战魂
	UINT8		ExchangeSoul(CPlayer* pPlayer, PKT_CLIGS_EXCHANGE_SOUL_REQ& rMsgReq, PKT_CLIGS_EXCHANGE_SOUL_ACK& rMsgAck);

	CSoul*		GenerateSoulById(UINT32 unSoulId);
	VOID		FillSoulAttrData(UINT8 ucStarLv, UINT8 ucStrengthLv, UINT8& rAttrNum
							, DT_SOUL_ATTR_DATA* pAttrData, C8Set setLock = C8Set());
	UINT16 OpenSoulPotencyInfo(CPlayer* poPlayer, PKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_REQ& stReq, PKT_CLIGS_OPEN_SOUL_UPDATE_TYPE_ACK& stAck);
	UINT16 SoulPotencyInfo(CPlayer* poPlayer, PKT_CLIGS_SOUL_UPDATE_TYPE_REQ& stReq, PKT_CLIGS_SOUL_UPDATE_TYPE_ACK& stAck);
private:
	CSoul*		GenerateSoul(UINT8 ucStar);
	
	CSoul*		GetSoul(CPlayer* pPlayer, DT_CLIGS_SOUL_MARK_DATA& rSoulMarkData);
	VOID		UpdateSoulData(CPlayer* pPlayer, DT_CLIGS_SOUL_MARK_DATA& rSoulMarkData, CSoul* pSoul);

	BOOL		IsSoulRepActivity(CPlayer* pPlayer);

	VOID		FillSoulRepInfo(CPlayer* pPlayer, DT_SOUL_REP_INFO& rSoulRepData);
	VOID		FillSoulBagData(CPlayer* pPlayer, DT_SOUL_DATA_LST_CLI& rBagSoulData);
	VOID		FillBagMeterialInfo(CBag& rBag, UINT8& rMeterialNum, DT_GOODS_DATA_CLI* pMeterialInfo);

	UINT8		RandomAttrId();

	UINT8		GetSoulBoxType(UINT8 ucStar);

};

#endif // _SOUL_MAMAGER_H_
