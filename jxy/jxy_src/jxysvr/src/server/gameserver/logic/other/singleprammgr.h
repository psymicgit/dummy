
#ifndef _SINGLEPRAMMGR_H_
#define _SINGLEPRAMMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>
#include <protocommondata.h>
#include "logic/pet/petdef.h"

class CSinglePramMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CSinglePramMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_SINGLE_PRAM ; }

    BOOL Init();

    VOID UnInit();
public:
    UINT16 GetMaxPlayerLevel() { return m_wMaxPlayerLevel; }
	UINT8 GetRechargeFlag(){ return m_byRechargeFlag; }
    UINT8 GetOnlineEncourageKeepDays() { return m_byOnlineEncourageKeepDays; }
    UINT8 GetLoginEncourageKeepDays() { return m_byLoginEncourageKeepDays; }
	UINT16 GetRefreshEliteNumPerDay() { return m_wRefreshEliteNumPerDay;}
	UINT16 GetGainCropCDGoldPerMin() { return m_wGainCropCDGoldPerMin;}
	UINT8  GetMaxHPGridNum() { return m_byMaxHPGridNum; }
	UINT16 GetRecruitReturnCoinPercentage(){ return m_wRecruitReturnCoinPercentage;}	//返回的铜钱比例
	UINT16 GetRecruitFavorability(){ return m_wRecruitFavorability;}		//豹子提升的好感度
	UINT16 GetJuniorLevel() { return m_wJuniorLevel; }
	//UINT16 GetResetClimbTower() {return m_wResetClimbTower;}
	UINT8 GetTotalYabiaoCnt(){ return m_byTotalYabiaoCnt; }
	UINT8 GetTotalJiebiaoCnt(){ return m_byTotalJiebiaoCnt; }
	INT32 GetMaxLevelCostGold(){ return m_nMaxLevelCostGold; }
	UINT8 GetTotalBeJiebiaoCnt(){ return m_byTotalBeJiebiaoCnt; }
	UINT8 GetJiebiaoViewPlayerCnt(){ return m_byJiebiaoViewPlayerCnt; }
	UINT16 GetJiebiaoCD(){ return m_wJiebiaoCD; }
	UINT16 GetClearJiebiaoCDCostGoldPerMin(){ return m_wClearJiebiaoCDCostGoldPerMin; }
	UINT16 GetUpdateRecordDataToSqlInterval(){ return m_wUpdateRecordDataToSqlInterval; }
    UINT16 GetSaveOpHistoryToDBInterval(){ return m_wSaveOpHistoryToDBInterval; }
	UINT16 GetPerfectFreeGodweaponGold(){ return m_wPerfectFreeGodponGold;}
	UINT32 GetGoldExperienceAddValue(){ return m_dwGoldExperienceAddValue; }
	UINT16 GetGoldExperienceCostGold(){ return m_wGoldExperienceCostGold; }
	UINT16 GetFreeFlushBiaoCnt(){ return m_wFreeFlushBiaoCnt; }
	CHAR * GetAppAddress(){return m_szAppAddress;}

	UINT16 GetHeroTransFreePercentage(){ return m_wHeroTransFreePercentage; }
	UINT16 GetHeroTransGoldPercentage(){ return m_wHeroTransGoldPercentage; }
	UINT16 GetTransGold(){ return m_wTransGold; }
	UINT16 GetCoachExpRate() { return m_wCoachExpRate; }

	UINT16 GetRdChallengeRefreshCD() { return m_wRdChallengeRefreshCD; }
	CHAR * GetScienceDspContent() { return m_szScienceDspContent; }

	UINT16 GetOpenOprateingActivityLevel() { return m_wOpenOprateingActivityLevel; }

    BOOL   GetReCalcHowManyTopRacer(){ return m_wReCalcHowManyTopRacer; }
    BOOL   GetIsSaveNewRacerPowerToDB(){ return m_bIsSaveNewRacerPowerToDB; }
	UINT32 GetSavePlayerInterval() { return m_dwSavePlayerInterval;}
	UINT16 GetApplyFactionMaxNum() { return m_wApplyFactionMaxNum; }
	UINT8  GetEveryDayAuditMaxNum() { return m_byEveryDayAuditMaxNum; }
	UINT8  GetSureOutOrangeAwaken() {return m_bySureOutOrangeAwaken; }
	UINT16 GetEveryDayRecvFlowers() {return m_wEveryDayRecvFlowers; }
	UINT32 GetExpiresRecvFlower() {return m_dwExpiresRecvFlower; }
	
	UINT8 GetFreeChangeIconNum() { return m_byFreeChangeIconNum; }
	UINT8 GetNormalFindBackDoorsTribute() { return m_byNormalFindBackDoorsTribute; }
	UINT8 GetPerfectFindBackDoorsTribute() { return m_byPerfectFindBackDoorsTribute; }
	UINT32 GetExprienceAddInterval(){ return m_dwHotSpringExprienceInterval; }
	UINT32 GetChangeFactionIcon() { return m_dwChangeFactionIcon; }
	UINT32 GetCloseAreaBoosMulti() { return m_dwCloseAreaBoosMulti; }
	UINT32 GetMaxScienceLevel() { return m_dwMaxScienceLevel;}
	UINT8 GetJewelryStrengIntervalLevel() { return m_byJewelryStrengIntervalLevel; }
	UINT32 GetJewelryResolveReturn(){ return m_dwJewelryResolveReturn;	}
	UINT32 GetJewelryBlueNum() { return m_dwJewelryBlueNum; }
	UINT32 GetJewelryPurpleNum(){ return m_dwJewelryPurpleNum; }
	UINT32 GetJewelryRedNum(){ return m_dwJewelryRedNum; }
	UINT32 GetJewelryOrangeNum(){ return m_dwJewelryOrangeNum; }
	UINT8 GetMaxJewelryStrengNum() { return m_byMaxJewelryStrengNum; }
	
    UINT32 GetCourageHelpTimes() {return m_dwCourageHelpTimes;}	
	UINT8 GetMinDisPlayerOutBoundNum(){ return m_byMinDisPlayerOutBoundNum;}
	UINT8 GetInterceptFlag(){ return m_byInterceptFlag; }
	UINT8 GetHotSpringFlag(){ return m_byHotSpringFlag; }
	UINT8 GetAdvertisementFlag(){ return m_byAdvertisementFlag; }
	UINT16 GetNeiDanInstanceNum(){ return m_wMaxNeiDanInstanceNum; }
public:
    CSinglePramMgr();
    virtual ~CSinglePramMgr();
protected:

    BOOL LoadFromDB();
private:
    UINT16	m_wMaxPlayerLevel;//玩家最大等级
	UINT8 m_byRechargeFlag;//充值按钮标识
	UINT8 m_byInterceptFlag;//拦截标识
	UINT8 m_byHotSpringFlag;//温泉标识
	UINT8 m_byAdvertisementFlag;//广告标识
    UINT8	m_byOnlineEncourageKeepDays;//在线奖励持续天数
    UINT8	m_byLoginEncourageKeepDays;//登录奖励持续天数
	UINT16  m_wRefreshEliteNumPerDay;//每天(使用元宝)刷新精英副本次数

	UINT16  m_wGainCropCDGoldPerMin;//种植强收

	UINT8		m_byMaxHPGridNum; //最大血条格子数
	UINT16		m_wRecruitReturnCoinPercentage;		//返回的铜钱比例
	UINT16		m_wRecruitFavorability;		//豹子提升的好感度

	UINT16		m_wJuniorLevel;//新手等级

	//UINT16		m_wResetClimbTower; //免费重置爬塔次数

	UINT8	m_byTotalYabiaoCnt; //每日的最高押镖次数
	UINT8	m_byTotalJiebiaoCnt; //每日的最高劫镖次数
	INT32	m_nMaxLevelCostGold; //直接达到最好的押镖品质所消耗的元宝
	UINT8	m_byTotalBeJiebiaoCnt; //每次押镖最多被劫镖的次数
	UINT8	m_byJiebiaoViewPlayerCnt; //劫镖可见的玩家个数上限
	UINT16	m_wJiebiaoCD; //劫镖的CD
	UINT16	m_wClearJiebiaoCDCostGoldPerMin; //清空劫镖CD每分钟花费的元宝

	UINT16	m_wUpdateRecordDataToSqlInterval; //更新采集数据到数据库的时间间隔
    UINT16	m_wSaveOpHistoryToDBInterval;     // 资源消耗统计表的采集数据到数据库的间隔, ophistory代表资源耗用历史

	UINT16	m_wPerfectFreeGodponGold;		//完美释放器魂消耗元宝
	UINT32	m_dwGoldExperienceAddValue;		//元宝经验丹增加的经验
	UINT16	m_wGoldExperienceCostGold;		//元宝服用经验丹消耗元宝
	UINT16	m_wFreeFlushBiaoCnt;			//免费刷新镖的次数


	UINT16	m_wHeroTransFreePercentage;	//免费传承百分比
	UINT16	m_wHeroTransGoldPercentage;	//元宝传承百分比
	UINT16	m_wTransGold;					//传承花费元宝
	CHAR		m_szAppAddress[512]; //app评分地址
	UINT16	m_wCoachExpRate;	//主角经验倍数(百分比)

	UINT16	m_wRdChallengeRefreshCD;//随机挑战点回复CD

	TCHAR	m_szScienceDspContent[MAX_CONTENT_SIZE];//打开场景要显示的其他内容

	UINT16	m_wOpenOprateingActivityLevel;//运营活动开放所需玩家等级

    UINT16	m_wReCalcHowManyTopRacer;      // 启动时重新计算竞技场前多少个（一般是前100个）玩家的战力并比对，如果是0则表示不重新计算
    BOOL		m_bIsSaveNewRacerPowerToDB;    // 是否把最新战力回写到数据库：是1/否0
	UINT32	m_dwSavePlayerInterval;			//玩家保存时间
	UINT16	m_wApplyFactionMaxNum;			//同时申请门派最多个数
	UINT8	m_byEveryDayAuditMaxNum;			//每天审核通过上限
	UINT8	m_bySureOutOrangeAwaken;			//几次后必出黄色卦象
	UINT16	m_wEveryDayRecvFlowers;

	UINT32	m_dwExpiresRecvFlower;//收花过期时间（秒）
	UINT8 m_byFreeChangeIconNum;			//免费更换门派头像次数
	UINT8 m_byNormalFindBackDoorsTribute;	//累积门贡
	UINT8 m_byPerfectFindBackDoorsTribute;	//剩余门贡
	UINT32 m_dwHotSpringExprienceInterval;//温泉经验增加间隔
	UINT32 m_dwChangeFactionIcon;//更换门派头像，消耗元宝
	UINT32 m_dwCloseAreaBoosMulti;//合区后BOSS的血量倍数
	UINT32 m_dwMaxScienceLevel;//最大科技等级
	UINT8 m_byJewelryStrengIntervalLevel; //饰品间隔强化等级
	UINT8 m_byMaxJewelryStrengNum;//饰品最多强化次数
	UINT32 m_dwJewelryResolveReturn; //饰品分解返还比例
	UINT32 m_dwJewelryBlueNum;
	UINT32 m_dwJewelryPurpleNum;
	UINT32 m_dwJewelryRedNum;
	UINT32 m_dwJewelryOrangeNum;
	
    UINT32 m_dwCourageHelpTimes;   //GVE协助次数
	UINT8 m_byMinDisPlayerOutBoundNum; //最少显示的外传个数
	UINT16 m_wMaxNeiDanInstanceNum;//每天可挑战斩兽炼魂次数
};

#endif
