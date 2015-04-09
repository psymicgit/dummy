///<------------------------------------------------------------------------------
//< @file:   logic\medal\medalpropmgr.h
//< @author: Kang_zhiquan
//< @date:   2014年6月18日 14:2:17
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _medalpropmgr_h_
#define _medalpropmgr_h_
#include "logic/base/basepropmgr.h"
#include "logic/medal/medaldef.h"
#include "dll/sdu/sdobjectpool.h"
#include "logic/base/basedefine.h"
class CMedal;
class CRaceMedal;
class CBossHurtMedal;
class CBossKillMedal;
class CPlayerLevelMedal;
class CPlayerPowerMedal;
class CPlayerVipLevelMedal;
class CPlayerLoginMedal;
class CClimbTowerMedal;
class CPhystrengthMedal;
class CInterceptMedal;
class CGatherCoinMedal;
class CGatherGoldMedal;
class CBlackRoomArrestMedal;
class CBlackRoomDriveMedal;
class CResetEliteInstanceMedal;
class CResetTowerMedal;
class CWorshipGodMedal;
class CWorshipPlayerMedal;
class CJieBiaoMedal;
class CRockMoneyMedal;
class CSmileMedal;
class CAwakenMedal;
class CRaceContinueKillMedal;
class CGVECourageMedal;
class CGvGMedal;


class CMedalPropMgr : public CBasePopMgr
{
	DECLARE_SINGLETON_PROPMGR(CMedalPropMgr)
public:
	CMedalPropMgr();
	~CMedalPropMgr();

	EConfigModule	GetConfigModuleType() { return ECM_MEDAL; }
	BOOL Init();
	VOID UnInit();
	CMedal* CreateMedal(CPlayer*, const DT_MEDAL_DATA&);
	CMedal* CreateMedal(CPlayer*, UINT16 wMedalID);
	VOID FreeMedal(UINT16 , CMedal*);
	UINT16 GetMedalNumByBuildID(UINT8 byBuildID);
	const C1616MultiMap& GetBuildNumMap();
public:
	BOOL GetMedalProp(UINT16 wMedalID, const SMedalProp*& pstProp);
	BOOL GetMedalProp(UINT16 wSmallType, UINT16 wLevel, const SMedalProp*& pstProp);
protected:
	BOOL LoadMedalProp();
private:
	CMapMedalProp m_mapMedalProp;//勋章配置
	CMap32MedalProp m_mapTypeLevelMedalProp;
	C1616MultiMap m_mapBuildNum;//各个建筑
	//对象池
	CSDObjectPool<CRaceMedal, CSDMutex> m_oRaceMedalPool;
	CSDObjectPool<CBossKillMedal, CSDMutex> m_oBossKillMedalPool;
	CSDObjectPool<CBossHurtMedal, CSDMutex> m_oBossHurtMedalPool;
	CSDObjectPool<CPlayerLevelMedal, CSDMutex> m_oPlayerLevelMedalPool;
	CSDObjectPool<CPlayerVipLevelMedal, CSDMutex> m_oPlayerVipLevelMedalPool;
	CSDObjectPool<CPlayerLoginMedal, CSDMutex> m_oPlayerLoginMedalPool;
	CSDObjectPool<CPlayerPowerMedal, CSDMutex> m_oPlayerPowerMedalPool;
	CSDObjectPool<CClimbTowerMedal, CSDMutex> m_oClimbTowerMedalPool;
	CSDObjectPool<CPhystrengthMedal, CSDMutex> m_oPhystrengthMedalPool;
	CSDObjectPool<CInterceptMedal, CSDMutex> m_oInterceptMedalPool;
	CSDObjectPool<CJieBiaoMedal, CSDMutex> m_oJieBiaoMedalPool;
	CSDObjectPool<CRockMoneyMedal, CSDMutex> m_oRockMoneyMedalPool;
	CSDObjectPool<CGatherCoinMedal, CSDMutex> m_oGatherCoinMedalPool;
	CSDObjectPool<CGatherGoldMedal, CSDMutex> m_oGatherGoldMedalPool;
	CSDObjectPool<CWorshipGodMedal, CSDMutex> m_oWorshipGoldMedalPool;
	CSDObjectPool<CWorshipPlayerMedal, CSDMutex> m_oWorshipPlayerMedalPool;
	CSDObjectPool<CAwakenMedal, CSDMutex> m_oAwakenMedalPool;
	CSDObjectPool<CSmileMedal, CSDMutex> m_oSmileMedalPool;
	CSDObjectPool<CBlackRoomArrestMedal, CSDMutex> m_oBlackRoomArrestMedalPool;
	CSDObjectPool<CBlackRoomDriveMedal, CSDMutex> m_oBlackRoomDriveMedalPool;
	CSDObjectPool<CResetTowerMedal, CSDMutex> m_oResetTowerMedalPool;
	CSDObjectPool<CResetEliteInstanceMedal, CSDMutex> m_oResetEliteInstanceMedalPool;
	CSDObjectPool<CRaceContinueKillMedal, CSDMutex> m_oRaceContinueKillMedalPool;
	CSDObjectPool<CGvGMedal, CSDMutex> m_oGVGMedalPool;
	CSDObjectPool<CGVECourageMedal, CSDMutex> m_oGVECourageMedalPool;
};

#endif //_medalpropmgr_h_