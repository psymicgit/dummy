#ifndef __GT_PACKETPROCESSOR_INCLUDE_H_
#define __GT_PACKETPROCESSOR_INCLUDE_H_

#include "sdframework.h"
#include <protocol/server/protogtgs.h>
#include <net/cli/usermgr.h>

using namespace SGDP;


class CGTProcessor : public CSDPacketProcessor
{
public:
    CGTProcessor();
    ~CGTProcessor();
public:
    virtual BOOL  Init() ;
    virtual CSDProtocol* GetProtocol() ;
private:
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////// 未创建CUser时的协议,使用CGTPipeChannel处理 ///////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    static BOOL        OnEnterGSReq(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);


	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////// 非用户操作的协议,使用CGTPipeChannel处理 ///////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	static BOOL        OnKickOutAck(VOID* pPipeChannel, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);


    //////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////// 未创建CRole或游戏数据不完整时的协议,使用CUser处理 ////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////

    //用户断开连接
    static BOOL        OnUserDisconnect(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //获取武将信息请求
    static BOOL        OnGetHeroInfoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //进入家园请求
    static BOOL        OnEnterHomeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //进入副本请求
    static BOOL        OnEnterInstanceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //战斗请求
    static BOOL        OnBattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //战斗请求
    static BOOL        OnPickReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //离开副本请求
    static BOOL        OnExitInstanceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //获取场景副本请求
    static BOOL        OnGetSceneInstanceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //收获请求
    static BOOL        OnCrownReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //建筑升级请求
    static BOOL        OnUpgradeBuildReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开铁匠铺请求
    static BOOL        OnOpenSmithyReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //铁匠铺购买装备请求
    static BOOL        OnSmithyBuyEquipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //强化请求
    static BOOL        OnStrengthenReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //附魔请求
    static BOOL        OnEquipEnchantReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	// 获取开光信息请求
	static BOOL		   OnGetOpenLightInfoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	// 开光请求
	static BOOL		   OnOpenLightReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);


    //打开招募所请求
    static BOOL        OnOpenRecruitReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //招募武将请求
    static BOOL        OnRecruitHeroReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //遣散武将请求
    static BOOL        OnDisbandHeroReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开生产建筑请求
    static BOOL        OnOpenProduceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开背包请求
    static BOOL        OnOpenBagReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //科技属性升级请求
    static BOOL        OnScienceAttrUpgrade(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //科技馆建筑升级请求
    static BOOL        OnScienceBuildUpgrade(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //任务奖励请求
    static BOOL        OnRecvTaskEncourage(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开竞技场请求
    static BOOL        OnOpenRaceBuildReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //挑战请求
    static BOOL        OnChallengeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //领取排名请求
    static BOOL        OnRecvRankEncourageReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开商店请求
    static BOOL        OnOpenShopReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //购买道具请求
    static BOOL        OnBuyItemReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //购买道具请求
    static BOOL        OnSellItemReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //穿戴装备请求
    static BOOL        OnEquipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //一键穿戴装备请求
    static BOOL        OnQuickEquipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //注册用户名请求
    static BOOL        OnRegisterUserNameReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //布阵请求
    static BOOL        OnEmbattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //清除玩家数据请求
    static BOOL        OnClrPlayerDataReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开布阵请求
    static BOOL        OnOpenEmbattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开道具制作卷请求
    static BOOL        OnOpenItemMakerReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //制作道具请求
    static BOOL        OnMakeItemReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //获取武将丹药信息请求
    static BOOL        OnGetHeroDrugReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //使用丹药请求
    static BOOL        OnUseDrugReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开副本奖励宝箱请求
    static BOOL        OnOpenEncourageInstanceBoxReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //背包扩容请求
    static BOOL        OnIncreaseBagSize(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //挂机请求
    static BOOL        OnHookReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //挂机中断请求
    static BOOL        OnInterruptOnhook(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //挂机立即结束请求
    static BOOL        OnNoCDOnhook(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //获取挂机结果请求
    static BOOL        OnOnhookResult(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //清除挑战CD请求
    static BOOL        OnClrChallengeCD(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //领取在线奖励请求
    static BOOL        OnRecvOnlineEncourage(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //同步时间请求
    static BOOL        OnSyncTime(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //注册昵称请求
    static BOOL        OnRegisterDspName(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //物品使用
    static BOOL        OnUseGoodsReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开登录奖励面板
    static BOOL        OnOpenLoginEncourageReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //登录奖励摇奖请求
    static BOOL        OnRateLoginEncourageReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //登录奖励放入背包请求
    static BOOL        OnPutLoginEncourageIntoBagReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //获取竞技场排名请求
    static BOOL        OnGetRankReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //接受任务请求
    static BOOL        OnAcceptTaskReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //放弃任务请求
    static BOOL        OnGiveUpTaskReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //获取竞技场战报请求
    static BOOL        OnGetRaceBattleLogReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开俘虏面板请求
    static BOOL        OnOpenCaptureTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //掠夺请求
    static BOOL        OnPlunderReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //打开仇敌面板请求
    static BOOL        OnOpenEnemyTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //打开掠夺推荐玩家
    static BOOL        OnOpenPlunderRecommendPlayerReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //释放俘虏
    static BOOL        OnFreeCaptureReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //获取掠夺战报
    static BOOL        OnGetPlunderBattleLogReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //刷新掠夺玩家
    static BOOL        OnRefreshPlunderRecommendReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //删除仇敌
    static BOOL        OnDelEnemyReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //收取俘虏收益
    static BOOL        OnCollectIncomeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //榨干俘虏
    static BOOL        OnBleedCaptureReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //设置工作类型
    static BOOL        OnSetWorkTypeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //驱赶俘虏主
    static BOOL        OnDriveAwayReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //领取俘虏留下收益
    static BOOL        OnRecvLeftIncomeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);


    //打开摇钱树
    static BOOL        OnOpenMoneyTreeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //摇钱
    static BOOL        OnRockMoneyTreeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开器魂
    static BOOL        OnOpenGodweaponReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //升级器魂
    static BOOL        OnUpgradeGodweaponReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //释放器魂
    static BOOL        OnFreeGodweaponReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);



    //打开悟道请求
    static BOOL        OnOpenAwakenTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //悟道请求
    static BOOL        OnAwakenReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //打开穿戴心得面板请求
    static BOOL        OnOpenDressStudyTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //心得穿戴请求
    static BOOL        OnDressStudyReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //转换心得经验请求
    static BOOL        OnStudyToExpReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //自动转换红色碎片成心得经验选择请求
    static BOOL        OnStudyUpgradeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//////////////////////////////////////
	//打开悟道请求
	static BOOL        OnOpenAwakenTabReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//悟道请求
	static BOOL        OnAwakenReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开穿戴心得面板请求
	static BOOL        OnOpenDressStudyTabReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//心得穿戴请求
	static BOOL        OnDressStudyReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//转换心得经验请求
	static BOOL        OnStudyToExpReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//自动转换红色碎片成心得经验选择请求
	static BOOL        OnStudyUpgradeReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开分解卦象
    static BOOL        OnOpenResolveTrigramReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //分解卦象
    static BOOL        OnResolveTrigramReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);


    //拜访武将请求
    static BOOL        OnVisitHeroReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //购买体力请求
    static BOOL        OnBuyPhystrengthReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开主城请求
    static BOOL        OnOpenMaintownReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //领取主城收益
    static BOOL        OnRecvIncomeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //征收主城收益
    static BOOL        OnLevyIncomeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);



    //打开丹药服用面包
    static BOOL        OnOpenDrugUseTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //打开武将丹药服用面板请求
    static BOOL        OnOpenHeroDrugUseTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //清除丹药服用CD请求
    static BOOL        OnClrUseDrugCDReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //使用丹药请求
    static BOOL        OnUseDrugReqEx(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //打开丹药升级面板请求
    static BOOL        OnOpenUpgradeDrugReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //丹药升级请求
    static BOOL        OnUpgradeDrugReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //装备合成
    static BOOL        OnComposeEquipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开购买体力面板
    static BOOL        OnOpenPhyStrengthReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //清除强化CD
    static BOOL        OnClrStrengthenCDReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);


    //////////////////////////////////////////////////////////////////////////
    //旧版本种植
    static BOOL        OnOpenCropReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL        OnOpenCropFieldReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL        OnCropReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL        OnGainCropReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL        OnClrCropReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);


    //服用经验丹
    static BOOL        OnOpenExperienceDrugReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL        OnUserExperienceDrugReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL		  OnOpenScienceBuildReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL		  OnUpdateScienceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL		  OnOpenUpdateAttrPannelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL		  OnUpdateAttrReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //装备产器魂
    static BOOL		  OnOpenMeltEquipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL		  OnMeltReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //物品合成
    static BOOL		  OnComposeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //获取物品信息
    static BOOL		  OnGetItemReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    static BOOL		  OnGetOrderIDReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //种子合成
    static BOOL		  OnComposeSeedReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //聊天
    static BOOL		  OnChatReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //聊天
    static BOOL		  OnOpenChatReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);


    //打开活动中心请求
    static BOOL		  OnOpenActivityCenterReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //打开吃包子中心请求
    static BOOL		  OnOpneEatBreadReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //得到体力请求
    static BOOL		  OnObtainPhystrengthenReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //离开吃包子中心请求
    static BOOL		  OnOLeaveEatBreadReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);


    //重置爬塔
    static BOOL		  OnClimbTowerResetReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //爬塔挂机
    static BOOL		  OnClimbTowerOnHookReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);


    //跑商
    static BOOL		  OnOpenPaoshangBuildReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开押镖界面
    static BOOL		  OnOpenYabiaoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //刷新品质
    static BOOL		  OnFlushBiaoQualityReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);


    //开始押镖
    static BOOL		  OnBeginYabiaoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开劫镖界面
    static BOOL		  OnOpenJiebiaoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //开始劫镖
    static BOOL		  OnBeginJiebiaoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //请求战斗信息
    static BOOL		  OnQueryBattleRecordReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //请求押镖战报信息
    static BOOL		  OnQueryYabiaoBattleLogReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //请求押镖奖励信息
    static BOOL		  OnQueryYabiaoEncourageReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //请求清空劫镖CD
    static BOOL		  OnClearJiebiaoCDReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //////////////////////////////////////
    //境界升级
    static BOOL		  OnUpgradeJingjieReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开练气面板
	static BOOL		  OnOpenGenGasTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //生产精气球
    static BOOL		  OnGenGasBallReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //提升精气球等级
    static BOOL		  OnUpGasBallLevelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //收集精气等级
    static BOOL		  OnCollectGasReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //升级技能招式等级
    static BOOL		  OnUpgradeSkillActionLevelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //选择技能招式效果
    static BOOL		  OnSelectSkillActionEffectReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //打开境界面板
    static BOOL		  OnOpenJingjieTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //打开技能面板
    static BOOL		  OnOpenSkillTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //打开VIP面板
    static BOOL		  OnOpenVipTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开爬塔面板
    static BOOL		  OnOpenClimbTowerReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开重置次数请求
    //static BOOL		  OnBuyClimbTowerResetReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //打开镶嵌面板
    static BOOL		  OnOpenXiangqianTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //拖动装备镶嵌宝石
    static BOOL		  OnDragEquipToXiangqianReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //镶嵌宝石
    static BOOL		  OnXiangqianReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    //APP版本的充值结果验证请求
    static BOOL		  OnAppExchangeCheckReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    //移除镶嵌
    static BOOL		  OnRemoveXiangqianReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    //获取精英副本请求
    static BOOL		  OnGetEliteInstanceReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    //重置精英副本请求
    static BOOL		  OnResetEliteInstanceReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    //精英副本挂机请求
    static BOOL		  OnEliteInstanceOnHookReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    //获得活动提示请求
    static BOOL		  OnGetActivityPromptReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    //蓝气转紫气请求
    static BOOL		  OnBlueToPurpleReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    //切磋请求
    static BOOL		  OnPKBattleReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    //资源不足请求
    static BOOL		  OnResourcesPromptReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    //boss战
    static BOOL		  OnEnterBossbReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnLeaveBossbReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnBossbMoveReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnBossbHurtReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnOpenInspireTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnInspirePowerReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnBossBReliveReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnGetLastBossBInfoReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnOpenBossBBLReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    static BOOL		  OnOpenRechargeEncourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnRecvRechargeEncourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnOpenScoreEncourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnRecvScoreEncourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnGetRandomDspNameReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    static BOOL		  OnOpenExperienceTransferReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnExperienceTransferReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    static BOOL		  OnOpenRdchallengeTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnRdchallengeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnClrRDChallengeCDReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    static BOOL		  OnOpenWorshipTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnWorshipReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    static BOOL		  OnActivityNotifyReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    static BOOL		  OnOpenDogFecesReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    static BOOL		  OnGetActivityCenterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnGetActivityEncourageInfoReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnRecvActivityEncourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	static BOOL		  OnGetActivityEncourageInfoReq2( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	static BOOL		  OnRecvActivityEncourageReq2( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    static BOOL		  OnOpenGemMakerTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	static BOOL		  OnOpenGemMakerTabExReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnOpenAwakenExchangeTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnExchangeStudyReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    static BOOL		  OnRecvRatingEncourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    static BOOL		  OnOpenEquipComposeTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnEquipComposeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    static BOOL		  OnRecvComposeEquipReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 打开帮派面板
    static BOOL		  OnOpenFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 申请加入帮派
    static BOOL		  OnApplyFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 加入申请帮派
    static BOOL		  OnCancelApplyFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 创建帮派
    static BOOL		  OnCreateFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 查找帮派
    static BOOL		  OnSearchFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 打开属于自己帮派
    static BOOL		  OnOpenOwnerFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 打开帮派门供
    static BOOL		  OnOpenDoorsTributeFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 门供帮派
    static BOOL		  OnDoorsTributeFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 获取门供帮派奖励
    static BOOL		  OnRecvDoorsTributeFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 帮派踢出
    static BOOL		  OnKickedFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 设置帮派职位
    static BOOL		  OnJobFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 设置帮派基本信息
    static BOOL		  OnSetNoticeFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 帮派解散
    static BOOL		  OnDissolveFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 帮派审核
    static BOOL		  OnAuditFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	static BOOL		  OnAuditFactionReq2( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 打开帮派申请列表
    static BOOL		  OnOpenApplyFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 打开帮派排名列表
    static BOOL		  OnOpenRankFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 打开帮派审核列表
    static BOOL		  OnOpenAuditFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	static BOOL		  OnOpenAuditFactionReq2( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 打开帮派日志列表
    static BOOL		  OnOpenLogFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 打开聚魂面板
    static BOOL		  OnOpenGetherScienceReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 聚魂
    static BOOL		  OnGetherScienceReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	// 打开仙露阁
	static BOOL		  OnOpenBluePavilionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	// 求仙露
	static BOOL		  OnBegBlueReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	// 求仙露改运
	static BOOL		  OnImproveLuckReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	// 求仙露确认祈求结果
	static BOOL       OnComfirmBegBlueReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 领取求仙露奖励
    static BOOL       OnGetBlueEncourage( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//转换职业请求
	static BOOL       OnChangeCareerReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	//自己分职业挑战请求
	static BOOL       OnChallengeMyselfReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//打开道具兑换面板请求
	static BOOL       OnOpenItemExchangeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	//道具兑换请求
	static BOOL       OnItemExchangeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//运营活动战斗请求
	static BOOL       OnOperatingActivityBattleReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    

    // 获取好友系统玩家列表
    static BOOL       OnGetFriendSysPlayerListReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	// 获取好友系统玩家列表
	static BOOL       OnGetFriendSysPlayerListReq2( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 删除好友
    static BOOL       OnDelFriendReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 搜索玩家
    static BOOL       OnSearchPlayerReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	// 搜索玩家
	static BOOL       OnSearchPlayerReq2( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 查看玩家信息
    static BOOL       OnViewPlayerReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 添加好友
    static BOOL       OnAddFriendReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 打开送花面板
    static BOOL       OnOpenSendFlowerTabReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 送花
    static BOOL       OnSendFlowerReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 获取鲜花信息
    static BOOL       OnGetFlowerByPlayerIDReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 获取鲜花榜
    static BOOL       OnGetFlowerRankingReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 获取私聊记录
    static BOOL       OnGetPrivateMsgLogListReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 发送私聊消息
    static BOOL       OnSendPrivateMsgReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 删除私聊消息
    static BOOL       OnDelPrivateMsgReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 拉入黑名单
    static BOOL       OnAddToBlacklistReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

    // 从黑名单中移除
    static BOOL       OnMoveFromBlacklistReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );    

	// 获取发离线消息的玩家列表
	static BOOL       OnGetSendOfflineMsgPlayerListReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );  

	// 读取某个发方玩家的离线消息请求
	static BOOL       OnReadOfflineMsgReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );  
    // 打开勇气试炼
    static BOOL       OnOpenCourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );  
    // 打开创建队伍
    static BOOL       OnCourageOpenCreateGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen ); 
    // 确认创建队伍
    static BOOL       OnCourageCreateGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen ); 
    // 加入协助队伍
    static BOOL       OnCourageJoinGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen ); 
    // 随机加入协助队伍
    static BOOL       OnCourageRandomJoinGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen ); 
    // 添加更改伙伴
    static BOOL       OnCourageOperateHeroReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen ); 
    // 确认添加更改伙伴
    static BOOL       OnCourageConfirmHeroReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen ); 
    // 踢掉队员
    static BOOL       OnCourageFireGroupMemberReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen ); 
    // 打开购买试炼
    static BOOL       OnCouragOpenShopReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen ); 
    // 购买试炼次数
    static BOOL       OnCouragBuyPractiseTimesReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen ); 
    // 玩家准备
    static BOOL       OnCouragPlayerReadyReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen ); 
    // 玩家开始
    static BOOL       OnCouragPlayerStartReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen ); 
    // 玩家离开
    static BOOL       OnCouragLeaveGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen ); 
    // 玩家刷新队伍
    static BOOL       OnCourageRefreshGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 进入队伍界面
    static BOOL       OnCourageEnterGroupReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 离开勇气试炼
    static BOOL		  OnLeaveCourageReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 战斗离开勇气试炼
    static BOOL		  OnCourageLeaveBattleReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 领取勇气奖励
    static BOOL		  OnCourageGetAwardReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 勇气进入战斗场景
    static BOOL		  OnCourageEnterBattleSceneReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    // 勇气请求战斗结果
    static BOOL       OnCouragGetBattleResultReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen ); 
	//打开珍品阁请求
	static BOOL		  OnOpenRareItemReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//兑换珍品请求
	static BOOL		  OnExchangeRareItemReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//打开珍品阁升级面板
	static BOOL       OnOpenUpgradeRareItemReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//升级珍品阁请求
	static BOOL       OnUpgradeRareItemReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//打开武堂请求
	static BOOL       OnOpenGymnasiumReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//学习武堂技能请求
	static BOOL       OnLearnGymnasiumSkillReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	
	//打开武堂升级请求
	static BOOL       OnOpenGymnasiumUpgradeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//确认武堂升级请求
	static BOOL       OnGymnasiumUpgradeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	//打开暗部请求
	static BOOL		  OnOpenShadeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	//打开黄金模式请求
	static BOOL       OnOpenGoldModeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//打开暗部升级请求
	static BOOL       OnOpenShadeUpgradeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//暗部升级请求
	static BOOL       OnShadeUpgradeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//捉妖请求
	static BOOL      OnOpenHuntingReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//筋斗去请求
	static BOOL		OnFindMonsterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	////更新请求		
	//static BOOL     OnChangeMonsterReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	//战斗请求
	static BOOL     OnHuntingBattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	//打开温泉请求
	static BOOL     OnOpenHotSpringReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	//选酒
	static BOOL    OnSelectWineReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	//饮酒
	static BOOL    OnDrinkWineReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//温泉移动
	static BOOL    OnDrinkWineMoveReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//离开温泉
	static BOOL    OnLeaveHotSpringReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	////选酒：上次未喝
	//static BOOL    OnSelectWineNotDrink(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	//退位让贤
	static BOOL    OnChangeOtherReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//赐酒
	static BOOL    OnGiveWineReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//点击选酒
	static BOOL    OnClickSelectWineReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	////寻找妖怪
	//static BOOL    OnFirstFindMonsterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//确认赐酒
	static BOOL    OnSureGiveWineReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//收妖
	static BOOL    OnClosedMonsterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	//人员变动标识请求
	static BOOL    OnFactionPlayerMarkReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//打开退出门派
	static BOOL		OnOpenLeaveFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen  );

	//退出门派
	static BOOL		OnLeaveFactionReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen  );

	//打开门派
	static BOOL     OnOpenFactionUpgradeReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen  );

	static BOOL    OnFactionUpgradeReq(  VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	static BOOL    OnGetFactionBattleLogReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL    OnOpenFindDrinkReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

// 装备精通
    // 打开装备精通面板
    static BOOL       OnOpenEquipMasterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );  
    
    // 激活装备精通
    static BOOL       OnActivateEquipMasterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );  

    // 突破
    static BOOL       OnBreakMasterReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );  

    // 培养精通之书
    static BOOL       OnGetMasterBookReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );  

    // 刮开精通经验书面板
    static BOOL       OnWipeMasterBookReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );  

    // 出千
    static BOOL       OnCheatMasterBookReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
    
    // 确认使用精通经验书
    static BOOL       OnConfirmMasterBookReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	// 勇气战斗请求
	static BOOL       OnCourageBattleReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	//打开门派建筑群
	static BOOL		OnOpenFactionBuildGroupReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	//查看过期奖励
	static BOOL    OnHotSpringOverRewardReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	static BOOL    OnHuntingIntegralReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	static BOOL   OnIgnorePromptReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	static BOOL    OnOpenLevelHotSpring(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	//审核全部拒绝
	static BOOL OnAuditAllRefuseReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

    static BOOL OnOpenRecvFlowerTabReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL OnAcceptFlowerReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL       OnOpenSendFlowerTabExReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	//珍品兑换
	static BOOL   OnBuyRareItemReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	// 送花
	static BOOL	OnSendFlowerExReq( VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );
	//武堂技能学习
	static BOOL   OnLearnGymnasiumSkillReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen );

	//打开门派成员列表
	static BOOL   OnFactionPlayerInfoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//注册门派昵称请求
	static BOOL        OnRegisterFactionName(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开头像更换
	static BOOL   OnOpenChangeIconReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//头像更换
	static BOOL   OnChangeIconReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开找回门贡
	static BOOL	OnOpenFindBackDoorsTributeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//找回门贡
	static BOOL	OnFindBackDoorsTributeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开布阵2
	static BOOL OnOpenEmbattleReq2(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//升级阵法
	static BOOL OnUpgradeFormationReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开阵法（科技馆）
	static BOOL OnOpenFormationScienceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
// 
// 
// 	//打开VIP礼包界面
// 	static BOOL		OnOpenVipGift(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
// 
// 	//获得VIP礼包
// 	static BOOL		OnGetVipGift(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	//升级阵法
	static BOOL OnUpgradeFormationScienceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//领取经验
	static BOOL OnGetExprienceHotSpringReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//学习门派技能
	static BOOL OnOpenFactionSkillReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	//打开人物面板
	static BOOL OnOpenPlayerPanelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开人物信息--算卦
	static BOOL OnOpenPlayerInfoAwakenReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开人物信息--技能
	static BOOL OnOpenPlayerInfoSkillReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开人物信息--神器
	static BOOL OnOpenPlayerInfoGodWeaponReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开人物信息--饰品
	static BOOL OnOpenPlayerInfoJewelryReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开人物信息--装备
	static BOOL OnOpenPlayerInfoEquipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开人物信息--战魂
	static BOOL OnOpenPlayerInfoSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开人物信息--宠物
	static BOOL OnOpenPlayerInfoPetReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//穿戴饰品请求
	static BOOL OnDressJewelryReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开人物选择面板
	static BOOL OnOpenHeroSelectPanelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开饰品合成
	static BOOL OnOpenJewelryGenerateReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//饰品合成
	static BOOL OnJewelryGenerateReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//饰品强化
	static BOOL OnJewelryStrengthReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//饰品合成信息
	static BOOL OnJewelryGenerateInfoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//分解信息
	static BOOL OnJewelryResolveReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开强化首饰面板
	static BOOL OnOpenJewelryStrengthReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//首饰对比
	static BOOL OnCompareJewelryReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//首饰附加属性强化
	static BOOL OnJewelryExtStrengReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开首饰分解
	static BOOL OnOpenJewelryResolveReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//////////////////////////////start GVG//////////////////////////////////////////
	//打开阵型布置
	static BOOL OnOpenGVGFormationReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开GVG活动界面
	static BOOL OnOpenGVGActivityReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开已报名门派名单
	static BOOL OnOpenAlreadApplayReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开上届结果排名
	static BOOL OnOpenGVGLastResultReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开我的押注
	static BOOL OnOpenGVGMyGambleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开门派押注
	static BOOL OnGVGOpenGambleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开规则面板
	static BOOL OnGVGOpenRuleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//押注门派
	static BOOL OnGVGGambleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开财力大排行
	static BOOL OnOpenGVGMoneyRankReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
    //打开指定门派的门派战况
    static BOOL OnOpenGvGLogReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
    //观看门派战斗录像
    static BOOL OnSeeGvGVideoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
    //结束门派战斗录像
    static BOOL OnEndGvGVideoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
    //查看门派战斗中表现突出的玩家列表
    static BOOL OnShowGvGMVPReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//读取TIP请求
	static BOOL OnReadFormationPlayerTipReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//布阵确认
	static BOOL OnGvGFactiomEmbattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//查看礼包
	static BOOL OnOpenGvGiftReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//领取礼包
	static BOOL OnGetGvGiftReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//GVE筛选条件
	static BOOL OnSelectGroupTypeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//门派报名
	static BOOL OnGvGFactionApplayReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//读取未读战报
	static BOOL OnReadBattleLogReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwlen);
	//打开战魂吞噬
	static BOOL OnOpenDevourSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//吞噬战魂
	static BOOL OnDevourSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开战魂洗炼
	static BOOL OnOpenReflashSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//洗炼战魂
	static BOOL OnReflashSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//保留战魂
	static BOOL OnSaveReflashSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	// 请求进入将魂副本
	static BOOL OnSoulRepInfoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	// 在副本中刷将魂
	static BOOL OnProductSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	// 穿戴战魂
	static BOOL OnEquipSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	// 脱下将魂
	static BOOL OnUnDressSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	// 替换将魂
	static BOOL	OnReplaceSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	// 请求战魂兑换信息
	static BOOL OnSoulExchangeInfoReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	// 材料兑换战魂
	static BOOL OnExchangeSoulReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开我的勋章
	static BOOL OnOpenMyMedalReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开地图
	static BOOL OnOpenMapReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//打开详细外传地图
	static BOOL OnOpenOutBoundMapReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    //查看其他门派的成员信息
    static BOOL OnViewFactionMembersReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//外传战斗
	static BOOL OnOutBoundBattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	//测试协议
	static BOOL OnTestReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	// 宠物模块
	static BOOL	OnOpenPetRepReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL	OnProductPetReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL	OnOpenRefinePetReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL	OnOpenPetPanelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL	OnRefinePetReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL	OnOpenStarPanelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL	OnUpPetStarReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnOpenPetFeedPanelReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnPetFristDrawReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnPetResetDrawReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnConfirmFeedReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnPetMerge(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnOpenPetStoreReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnBuyPetStoreItem(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnOpenWashQuality(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnWashQuality(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnChangePetState(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnOpenPetInherit(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnInheritPet(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnOpenPlayerPetPanel(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnRequestAllPetInfo(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	static BOOL OnOpenStoreReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnExchangeGoodsReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    
    // 飞升
    static BOOL OnOpenEquipMakeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL OnEquipMakeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL OnUpgradeHeroReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL OnOpenFlyReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL OnFlyReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);

	static BOOL OnOpenPetRule(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnOpenSoulUpdateTypeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnSoulUpdateTypeReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	//内丹
	static BOOL OnOpenNeiDanReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnUpdataNeiDanReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnDressNeiDanReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnOpenNeiDanInstanceReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnNeiDanBattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnRecvNeiDanAwardReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL OnSkipNeiDanBattleReq(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
private:
    static BOOL		   SendMsg( CUser *poUser, UINT16 wMsgID );

protected:
	static BOOL        OnBattleReq5(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
    static BOOL        OnBattleReq3(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
	static BOOL        OnBattleReq4(VOID* pUser, VOID* pHeader, const CHAR* pBody, UINT32 dwLen);
};

#endif

