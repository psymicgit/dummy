///<------------------------------------------------------------------------------
//< @file:   logic\medal\medalpropmgr.cpp
//< @author: Kang_zhiquan
//< @date:   2014Äê6ÔÂ18ÈÕ 20:3:42
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "logic/medal/medalpropmgr.h"
#include "framework/gsapi.h"
#include "logic/medal/medal.h"
#include "logic/medal/limittime/racemedal.h"
#include "logic/medal/limittime/bosskillmedal.h"
#include "logic/medal/limittime/bosshurtmedal.h"
#include "logic/medal/nonetime/playerpowermedal.h"
#include "logic/medal/nonetime/playerlevelmedal.h"
#include "logic/medal/nonetime/playerviplevelmedal.h"
#include "logic/medal/nonetime/climbtowermedal.h"
#include "logic/medal/nonetime/playerloginmedal.h"
#include "logic/medal/nonetime/awakenmedal.h"
#include "logic/medal/nonetime/blackroomarrestmedal.h"
#include "logic/medal/nonetime/blackroomdrivemedal.h"
#include "logic/medal/nonetime/climbtowermedal.h"
#include "logic/medal/nonetime/gathersciencecoinmedal.h"
#include "logic/medal/nonetime/gathersciencegoldmedal.h"
#include "logic/medal/nonetime/interceptmedal.h"
#include "logic/medal/nonetime/jiebiaomedal.h"
#include "logic/medal/nonetime/phystrengthmedal.h"
#include "logic/medal/nonetime/reseteliteinstancemedal.h"
#include "logic/medal/nonetime/resettowermedal.h"
#include "logic/medal/nonetime/rockmoneymedal.h"
#include "logic/medal/nonetime/worshipplayermedal.h"
#include "logic/medal/nonetime/worshipgodmedal.h"
#include "logic/medal/nonetime/smilemedal.h"
#include "logic/base/logicfunc.h"
#include "logic/player/player.h"
#include "db/dbmgr.h"
#include "logic/medal/limittime/gvgmedal.h"
#include "logic/medal/nonetime/racecontinuekillmedal.h"
#include "logic/medal/nonetime/gvecouragemedal.h"
IMPLEMENT_SINGLETON_PROPMGR(CMedalPropMgr)

CMedalPropMgr::CMedalPropMgr()
{

}
CMedalPropMgr::~CMedalPropMgr()
{

}

BOOL CMedalPropMgr::Init()
{
	if ( !LoadMedalProp())
	{
		return FALSE;
	}

	return TRUE;
}

VOID CMedalPropMgr::UnInit()
{

}

BOOL CMedalPropMgr::LoadMedalProp()
{
	SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetCfgDBSession();

	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = pDBSession->ExecuteSqlRs("select * from medalprop order by MedalID", &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: CMedalPropMgr::LoadMedalProp has no data!"), MSG_MARK);
		return TRUE; //
	}

	while(pRes->GetRecord())
	{
		SMedalProp stProp;
		stProp.wMedalID = SDAtou(pRes->GetFieldValueByName("MedalID"));
		stProp.strMedalName = pRes->GetFieldValueByName("MedalName");
		stProp.strMedalIconName = pRes->GetFieldValueByName("MedalIconName");
		stProp.byOpenBuildID = SDAtou(pRes->GetFieldValueByName("OpenFuncID"));
		stProp.byIsLevelMedal = SDAtou(pRes->GetFieldValueByName("IsLevelMedal"));
		stProp.strFinishDesc = pRes->GetFieldValueByName("FinishDesc");
		stProp.wLevel = SDAtou(pRes->GetFieldValueByName("MedalLevel"));
		stProp.wSmallType = SDAtou(pRes->GetFieldValueByName("MedalType"));
		stProp.qwFirstCondition = SDAtou64(pRes->GetFieldValueByName("FirstCondition"));
		stProp.qwSecondCondition = SDAtou64(pRes->GetFieldValueByName("SecondCondition"));
		stProp.qwThirdCondition = SDAtou64(pRes->GetFieldValueByName("ThirdCondition"));
		stProp.dwOverTime = SDAtou(pRes->GetFieldValueByName("OverTime"));
		stProp.byIsOverly = SDAtou(pRes->GetFieldValueByName("IsOverlay"));
		string strJson = pRes->GetFieldValueByName("AttrJson");
		stProp.byMedalColor = SDAtou(pRes->GetFieldValueByName("MedalColor"));
		if( !JsonParseAttrJson(stProp.vecAttr, strJson) )
		{
			if(NULL != pRes)
			{
				pRes->Release();
				pRes = NULL;
			}
			return FALSE;
		}
		m_mapMedalProp[stProp.wMedalID] = stProp;
		m_mapTypeLevelMedalProp[CLogicFunc::Get1616Key(stProp.wSmallType, stProp.wLevel)] = stProp;
		m_mapBuildNum.insert(make_pair( stProp.byOpenBuildID, stProp.wMedalID ));
	}

	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}

	return TRUE;
}

BOOL CMedalPropMgr::GetMedalProp(UINT16 wMedalID, const SMedalProp*& pstProp)
{
	CMapMedalPropItr itr = m_mapMedalProp.find(wMedalID);
	if ( itr == m_mapMedalProp.end())
	{
		return FALSE;
	}
	pstProp = &itr->second;
	return TRUE;
}

BOOL CMedalPropMgr::GetMedalProp(UINT16 wSmallType, UINT16 wLevel, const SMedalProp*& pstProp)
{
	CMap32MedalPropItr itr = m_mapTypeLevelMedalProp.find(CLogicFunc::Get1616Key(wSmallType, wLevel));
	if ( itr == m_mapTypeLevelMedalProp.end())
	{
		return FALSE;
	}
	pstProp = &itr->second;
	return TRUE;
}

CMedal* CMedalPropMgr::CreateMedal(CPlayer* poPlayer, const DT_MEDAL_DATA& stData)
{
	if ( NULL == poPlayer)
	{
		return NULL;
	}
	CMedal* poMedal = NULL;
	const SMedalProp* poProp = NULL;
	if( !GetMedalProp(stData.wMedalID, poProp) )
	{
		return NULL;
	}
	switch(poProp->wSmallType)
	{
	case EMST_RACE:
		poMedal = m_oRaceMedalPool.Alloc();
		break;
	case EMST_BOSS_KILL:
		poMedal = m_oBossKillMedalPool.Alloc();
		break;
	case EMST_BOSS_HURT:
		poMedal = m_oBossHurtMedalPool.Alloc();
		break;
	case EMST_PLAYER_POWER:
		poMedal = m_oPlayerPowerMedalPool.Alloc();
		break;
	case EMST_PLAYER_LEVEL:
		poMedal = m_oPlayerLevelMedalPool.Alloc();
		break;
	case EMST_PLAYER_VIP_LEVEL:
		poMedal = m_oPlayerVipLevelMedalPool.Alloc();
		break;
	case EMST_CLIMB_TOWER:
		poMedal = m_oClimbTowerMedalPool.Alloc();
		break;
	case EMST_LOGIN_DAY:
		poMedal = m_oPlayerLoginMedalPool.Alloc();
		break;
	case EMST_PHYSTRENGTH:
		poMedal = m_oPhystrengthMedalPool.Alloc();
		break;
	case EMST_ESCORT:
		poMedal = m_oJieBiaoMedalPool.Alloc();
		break;
	case EMST_ROCK_MONEY:
		poMedal = m_oRockMoneyMedalPool.Alloc();
		break;
//	case EMST_GATHER_SCIENCE_COIN:
//		poMedal = m_oGatherCoinMedalPool.Alloc();
//		break;
	case EMST_GATHER_SCIENCE_GOLD:
		poMedal = m_oGatherGoldMedalPool.Alloc();
		break;
// 	case EMST_WORSHIP_GOD:
// 		poMedal = m_oWorshipGoldMedalPool.Alloc();
// 		break;
	case EMST_WORSHIP_PLAYER:
		poMedal = m_oWorshipPlayerMedalPool.Alloc();
		break;
	case EMST_AWAKEN_ZTS:
		poMedal = m_oAwakenMedalPool.Alloc();
		break;
	case EMST_SMILE:
		poMedal = m_oSmileMedalPool.Alloc();
		break;
	case EMST_RESET_TOWER:
		poMedal = m_oResetTowerMedalPool.Alloc();
		break;
	case EMST_RESET_ELITEINSTANCE:
		poMedal = m_oResetEliteInstanceMedalPool.Alloc();
		break;
	case EMST_INTERCEPT:
		poMedal = m_oInterceptMedalPool.Alloc();
		break;
	case EMST_ARREST_BLACK:
		poMedal = m_oBlackRoomArrestMedalPool.Alloc();
		break;
	case EMST_DRIVE_BLACK:
		poMedal = m_oBlackRoomDriveMedalPool.Alloc();
		break;
	case EMST_RACE_CONTINUE_KILL:
		poMedal = m_oRaceContinueKillMedalPool.Alloc();
		break;
	case EMST_GVG_MEDAL:
		poMedal = m_oGVGMedalPool.Alloc();
		break;
	case EMST_GVE_COURAGE:
		poMedal = m_oGVECourageMedalPool.Alloc();
		break;
	};

	if ( NULL == poMedal)
	{
		return NULL;
	}
	if( !poMedal->Init(poPlayer, stData) )
	{
		FreeMedal(poProp->wSmallType, poMedal);
		return NULL;
	}
	return poMedal;
}

CMedal* CMedalPropMgr::CreateMedal(CPlayer* poPlayer, UINT16 wMedalID)
{
	CMapMedalPropItr itr = m_mapMedalProp.find(wMedalID);
	if ( (itr == m_mapMedalProp.end()) || ( NULL == poPlayer))
	{
		return NULL;
	}
	DT_MEDAL_DATA stData = {0};
	stData.byIsActivate = 0;
	stData.qwActivateTime = SGDP::SDTimeSecs();
	stData.wMedalID = wMedalID;

	return CreateMedal(poPlayer, stData);
}

VOID CMedalPropMgr::FreeMedal(UINT16 wSmallType, CMedal* poMedal)
{
	switch(wSmallType)
	{
	case EMST_RACE:
		m_oRaceMedalPool.Free((CRaceMedal*)poMedal);
		break;
	case EMST_PLAYER_VIP_LEVEL:
		m_oPlayerVipLevelMedalPool.Free((CPlayerVipLevelMedal*)poMedal);
		break;
	case EMST_PLAYER_POWER:
		m_oPlayerPowerMedalPool.Free((CPlayerPowerMedal*)poMedal);
		break;
	case EMST_PLAYER_LEVEL:
		m_oPlayerLevelMedalPool.Free((CPlayerLevelMedal*)poMedal);
		break;
	case EMST_BOSS_HURT:
		m_oBossHurtMedalPool.Free((CBossHurtMedal*)poMedal);
		break;
	case EMST_BOSS_KILL:
		m_oBossKillMedalPool.Free((CBossKillMedal*)poMedal);
		break;
	case EMST_LOGIN_DAY:
		m_oPlayerLoginMedalPool.Free((CPlayerLoginMedal*)poMedal);
		break;
	case EMST_PHYSTRENGTH:
		m_oPhystrengthMedalPool.Free((CPhystrengthMedal*)poMedal);
		break;
	case EMST_ESCORT:
		m_oJieBiaoMedalPool.Free((CJieBiaoMedal*)poMedal);
		break;
	case EMST_ROCK_MONEY:
		m_oRockMoneyMedalPool.Free((CRockMoneyMedal*)poMedal);
		break;
// 	case EMST_GATHER_SCIENCE_COIN:
// 		m_oGatherCoinMedalPool.Free((CGatherCoinMedal*)poMedal);
// 		break;
	case EMST_GATHER_SCIENCE_GOLD:
		m_oGatherGoldMedalPool.Free((CGatherGoldMedal*)poMedal);
		break;
// 	case EMST_WORSHIP_GOD:
// 		m_oWorshipGoldMedalPool.Free((CWorshipGodMedal*)poMedal);
// 		break;
	case EMST_WORSHIP_PLAYER:
		m_oWorshipPlayerMedalPool.Free((CWorshipPlayerMedal*)poMedal);
		break;
	case EMST_AWAKEN_ZTS:
		m_oAwakenMedalPool.Free((CAwakenMedal*)poMedal);
		break;
	case EMST_SMILE:
		m_oSmileMedalPool.Free((CSmileMedal*)poMedal);
		break;
	case EMST_RESET_TOWER:
		m_oResetTowerMedalPool.Free((CResetTowerMedal*)poMedal);
		break;
	case EMST_RESET_ELITEINSTANCE:
		m_oResetEliteInstanceMedalPool.Free((CResetEliteInstanceMedal*)poMedal);
		break;
	case EMST_INTERCEPT:
		m_oInterceptMedalPool.Free((CInterceptMedal*)poMedal);
		break;
	case EMST_ARREST_BLACK:
		m_oBlackRoomArrestMedalPool.Free((CBlackRoomArrestMedal*)poMedal);
		break;
	case EMST_DRIVE_BLACK:
		m_oBlackRoomDriveMedalPool.Free((CBlackRoomDriveMedal*)poMedal);
		break;
	case EMST_RACE_CONTINUE_KILL:
		m_oRaceContinueKillMedalPool.Free((CRaceContinueKillMedal*)poMedal);
		break;
	case EMST_GVG_MEDAL:
		m_oGVGMedalPool.Free((CGvGMedal*)poMedal);
		break;
	case EMST_GVE_COURAGE:
		m_oGVECourageMedalPool.Free((CGVECourageMedal*)poMedal);
		break;
	}
}

UINT16 CMedalPropMgr::GetMedalNumByBuildID(UINT8 byBuildID)
{
	return m_mapBuildNum.count(byBuildID);
}

const C1616MultiMap& CMedalPropMgr::GetBuildNumMap()
{
	return m_mapBuildNum;
}