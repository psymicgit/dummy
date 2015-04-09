#ifndef _CALCULATEMGR_H_
#define _CALCULATEMGR_H_

#include <sdtype.h>
#include <LuaPlus.h>
#include <LuaState.h>
#include <sdsingleton.h>
#include <logic/base/basedefine.h>
#include <src/server/common/client/commondef.h>
#include <logic/base/basepropmgr.h>

using namespace SGDP;
using namespace LuaPlus;

//活动错误码
enum EActivityErrCode
{
    ACTIVITY_ERR_PLAYER_NULL 			= 1, 	//玩家对象为空
    ACTIVITY_ERR_INVALID_ID				= 2, 	//无效活动ID
    ACTIVITY_ERR_ACTIVITY_HAVE_FINISH 	= 3, 	//活动已经完成
    ACTIVITY_ERR_ACTIVITY_NOT_START 	= 4, 	//活动尚未开始
    ACTIVITY_ERR_ACTIVITY_OVERDUE 		= 5, 	//活动已经过期
    ACTIVITY_ERR_ACTIVITY_CON_NOT_MATCH	= 6, 	//活动条件未符
};

struct SActivityInfo
{
    UINT32 dwID;
    UINT64 qwDspDate;
    UINT64 qwStartDate;
    UINT64 qwEndDate;
    UINT16 wEnterLevel; //进入等级
    tstring strName;
};
typedef map<UINT32, SActivityInfo> CActivityInfoMap;
typedef CActivityInfoMap::iterator CActivityInfoMapItr;

class CPlayer;
class CLuamgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CLuamgr);

public:
    virtual EConfigModule	GetConfigModuleType()
    {
        return ECM_LUA ;
    }
    BOOL Init();
    VOID UnInit();
public:
    ///////////////////////////公式计算模块///////////////////////////
    //强化铜币
    UINT32 GetStrengthenCoin(UINT16 wStrenthenLevel, UINT16 wDressLevel);
    //附魔所需附魔石ID
    UINT16 GetEnchantStoneID(UINT16 wEnchantLevel);
    //附魔所需附魔石
    UINT32 GetEnchantStoneCost(UINT16 wEnchantLevel, UINT16 wDressLevel);
    //俘虏工作生产铜币
    UINT32 GetCaptureWorkCoin(UINT16 wCaptureLevel);
    //俘虏工作生产器魂
    UINT32 GetCaptureWorkScience(UINT16 wCaptureLevel);
    //排名奖励——铜币
    UINT32 GetRankEncourageCoin(UINT32 dwRank);
    //排名奖励——器魂
    UINT32 GetRankEncourageScience(UINT32 dwRank);
    //排名奖励——境界点
    UINT32 GetRankEncourageJingJie(UINT32 dwRank);
    //挑战奖励——铜币
    UINT32 GetChallengeEncourageCoin(UINT16 wPlayerLevel, BOOL bWinFlag);
    //挑战奖励——器魂
    UINT32 GetChallengeEncourageScience(UINT16 wPlayerLevel, BOOL bWinFlag);
    //挑战奖励——境界
    UINT32 GetChallengeEncourageJingJie(UINT16 wPlayerLevel, BOOL bWinFlag);
    //摇钱树产出——铜币
    UINT32 GetMoneyTreeProduceCoin(UINT16 wPlayerLevel);
    //属性升级——增加的属性值
    DOUBLE GetAttrAddValue(UINT16 wTalent, UINT16 wHeroLevel, UINT8 byAttrType, UINT8 byCareerType);
    //属性升级——消耗铜币
    UINT32 GetUpdateAttrCostCoin(UINT16 wTalent, UINT16 wAttrLevel, BOOL bCoachFlag);
    //悟道召唤次数
    //UINT16 GetAwakenCallNum();
    //境界蓝气转换紫气比例(多少蓝气换1紫气)
    UINT16 GetBlueNumToPerPurple();
    //每次点蓝气值
    UINT32 GetClickBlueValue();
    //获得主城收获铜钱
    UINT32 GetMaintownCrownCoin(UINT16 wPlayerLevel);
    //获得主城收获元宝
    UINT32 GetMaintownCrownGold(UINT16 wPlayerLevel);
    //获得主城收获境界点
    UINT32 GetMaintownCrownJingJie(UINT16 wPlayerLevel);
    //获得主城征收铜钱
    UINT32 GetMaintownLevyCoin(UINT16 wPlayerLevel);
    //获得主城征收境界点
    UINT32 GetMaintownLevyJingJie(UINT16 wPlayerLevel);
    //查找该礼包是否需提示
    BOOL   CkBoxGoodsPrompt(UINT16 wBoxGoodsID);
    ///////////////////////////活动模块///////////////////////////
    //检查是否存在该活动
    BOOL	  CkHaveActivity(UINT32 dwId);
    //检查是否可完成该活动
    UINT16 CkFinishActivity(CPlayer* poPlayer, UINT32 dwId);
    //获得功能的消耗的元宝
    UINT32 GetFuncCost(CPlayer* poPlayer, ECostFunc func, INT32 nCount, INT32 nIdx = 0);

    //获得功能的器魂数
    UINT32 GetFuncObtain(CPlayer* poPlayer, EObtainFunc func, INT32 nCount, INT32 nIdx = 0);

	//获得聚气的次数
	UINT32 GetGenGasNum(UINT8 byVipLevel, UINT8 byLevel, UINT8 byTownIdx, UINT8 byInstanceIdx);

	//获得魂魂倍率
	UINT32 GetGatherScienceIncrease(INT32 nIdx);

	//获得魂魂倍率百分比
	UINT32 GetGatherScienceIncreasePer(INT32 nIdx, INT32 nFunc);

    //获得刚过的添加获取灵气次数时间（当前已过秒数）,0表示没有
    UINT32 GetPassIncGenGasNumTime();
    ///////////////////////////boss战/////////////////////////////
    UINT32 GetBossBTopEncCoin(UINT16 wPlayerLevel, UINT16 wTop); //获取boss战排名奖励
    UINT32 GetBossBTopEncJingJie(UINT16 wPlayerLevel, UINT16 wTop);//获取boss战排名奖励
    UINT32 GetBossBKillEncCoin(UINT16 wBossLevel);//获取boss战击杀奖励
    UINT32 GetBossBKillEncJingJie(UINT16 wBossLevel);//获取boss战击杀奖励
    UINT32 GetBossBHurtEncCoin(UINT16 wPlayerLevel, UINT32 dwHurtValue);//获取boss战伤害奖励
    UINT32 GetBossBHurtEncJingJie(UINT16 wPlayerLevel, UINT32 dwHurtValue);//获取boss战伤害奖励
    UINT32 GetBossBHeroHurtEncCoin();//获取boss战英雄奖励
    UINT32 GetBossBHeroHurtEncJingJie();//获取boss战英雄奖励
    UINT8  GetInspireRate(UINT8 byGoldInspireFlag, UINT16 wIncPowerRate);//获取boss战鼓舞概率
    UINT16 GetIncRateByStory();
    UINT16 GetIncRateByGold();
    ////////////////////////////随机挑战//////////////////////////////
    UINT32 GetRdchallengeEncJingJie(UINT8 byRet, UINT16 wMyLevel, UINT16 wEnemyLevel);
    UINT32 GetRdchallengeEncPurpleGas(UINT8 byRet, UINT16 wMyLevel, UINT16 wEnemyLevel);
    ////////////////
    UINT32 GetMeltGold(UINT32 dwScience);

    UINT32 GetBegBlueMaxTimes(UINT32 dwClimbTowerLayer); // 根据当前爬塔层数来获取当前求仙露的最大次数
    UINT32 GetFreeImproveLuckTimes(); // 获取求仙露每天免费改运次数
    UINT32 GetClimbTowerPrompt(UINT32 dwClimbTowerLayer);  // 获取下一个爬塔提示层
    UINT8  GetBegBlueIsIgnoreXnMultiple(); // 获取求仙露是否忽略倍率

	UINT16 GetTotalFreeCommonUpGasNum();
    
    UINT32 GetBossBHurtValueByPower(UINT32 dwPower);//根据战力获取boss战伤害

	//筋斗云费用
	UINT32 GetFindMonsterGold();
	//获取免费更新妖怪次数
	UINT32 GetFreeRefreshNum();
	//获取酒变质奖励所需Coin
	UINT32 GetCoinByBadWine();
	//门贡换算积分比例值
	UINT32 GetDoorsTributeIntegral();
	//获取捐献一次消耗铜钱
	UINT32 GetCostCoinByDonate(UINT32 FactionLevel);
	//获取捐献一次消耗元宝
	UINT32 GetCostGoldByDonate(UINT32 FactionLevel);
	//捐献一次铜钱增加资金数
	UINT32 GetFundsByDonateCoin(UINT32 FactionLevel);
	//捐献一次元宝增加资金数
	UINT32 GetFundsByDonateGold(UINT32 FactionLevel);
	//捐献一次铜钱增加门贡
	UINT32 GetDoorsTributeByDonateCoin(UINT32 FactionLevel);
	//捐献一次元宝增加门贡
	UINT32 GetDoorsTributeByDonateGold(UINT32 FactionLevel);
    //根据人物等级获取试炼组队的最低等级
    UINT32 GetCourageGroupLevel(UINT32 dwLevel);
    //获取每日免费试炼次数
    UINT32 GetTotalFreeCourageTimes();
    //每日协助次数 即可增加试炼次数
    UINT32 GetTotalHelpCourageTimes();
    //协助后每次增加的试炼次数
    UINT32 GetAddCourageTimes();
    //当前VIP可购买的试炼次数
    UINT32 GetVipBuyCourageTimes(UINT8 byVip);
	//黄金模式的倍数关系
	UINT32 GetGoldModeMulti();
	//加入门派时间间隔
	UINT32 GetJoinFactionInterval();
	//普通找回门贡消耗铜钱
	UINT32 GetNormalFindBackDoorsTribute(UINT32 dwLeftDoorsTribute);
	//完全找回门贡消耗元宝
	UINT32 GetPerfectFindBackDoorsTribute(UINT32 dwLeftDoorsTribute);
	//温泉中可提交经验上限
	UINT32 GetMaxExprience(UINT32 dwVipLevel, UINT32 dwFactionLevel);
	//温泉中经验增加基数
	UINT32 GetExprienceBase(UINT32 dwPlayerLevel, UINT32 dwFactionLevel);
	//生成饰品等级
	UINT32 GenerateJewelryLevel(UINT16 wFirstLevel, UINT16 wSecondLevel, UINT16 wThirdLevel, UINT16 wFourthLevel);
	//生成饰品品质
	UINT8 GenerateJewelryColor(UINT8 byColor1, UINT8 byColor2, UINT8 byColor3, UINT8 byColor4);
	//生成品质消耗
	UINT32 GenerateJewelryCost(UINT16 wDressLevel, UINT8 byColor);

	UINT8 GetProcuctSoulCostType(UINT8 ucProductyType, UINT8 ucSearchType, UINT32 unNum);
	UINT32 GetProcuctSoulCostNum(UINT8 ucProductyType, UINT8 ucSearchType, UINT32 unNum);

	UINT32 GetSoulRefleshCost(UINT8 ucLockNum);
	UINT32 GetSoulDevourCost(UINT8 ucDevourCost);
	//技能作用人数
	UINT32 GetSkillNumByPlayerLevel(UINT8 bySkillType, UINT16 wPlayerLevel);
// 	UINT32 RefinePetGetPetType(UINT8 ucPetType1, UINT8 ucPetType2);
// 	UINT32 RefinePetGetPetQuality(UINT8 ucPetQuality1, UINT8 ucPetQuality2);
	UINT32 GetPetFeedVIPCostGold(UINT8 ucVipLevel, UINT8 ucVipPetFeedCnt);

protected:
    BOOL InitLua();
    BOOL LoadLuaScript();

    BOOL InitLuaActivity();

    //获得功能消耗，根据VIP多增加次数需要的消耗
    UINT32 GetCostByFunc(UINT8 byVipLevel, ECostFunc func, INT32 nCount, INT32 nIdx);

    //获得资源， 根据VIP多增加次数获得资源
    UINT32 GetObtainByFunc(UINT8 byVipLevel, UINT8 byLevel, EObtainFunc func, INT32 nCount, INT32 nIdx);

public:
    CLuamgr();
    ~CLuamgr();
private:
    LuaStateOwner		m_oLuaState;
    CActivityInfoMap	m_mapActivityInfo;
};



#endif

