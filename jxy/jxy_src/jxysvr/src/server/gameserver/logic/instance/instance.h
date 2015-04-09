
#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <sdhashmap.h>
#include <protocol/server/protocommondata.h>
#include <logic/base/baseobj.h>
#include <logic/shmem/shmemmap.h>
#include "instancepropmgr.h"
#include "instancerecord.h"
#include "instancebattlercd.h"
#include <logic/score/scorepropmgr.h>
#include <common/server/utility.h>

using namespace std ;

class CPlayer;

#define NONINSTANCE	0 //没在副本


//玩家当前战斗副本，退出时wSceneIdx = NONINSTANCE
struct SCurBattleInstance
{
    UINT16 wSceneIdx;
    UINT8 byTownIdx;
    UINT8 byInstanceIdx;
    UINT8 byBattleIdx;

    BOOL bPass;//是否已经通关
    BOOL bFirstPass;
    UINT8 byBattleNum;//当前副本的战斗数(怪物数)

    SCurBattleInstance()
    {
        Clear();
    }
    VOID Clear()
    {
        wSceneIdx = NONINSTANCE;
        byTownIdx = 0;
        byInstanceIdx = 0;
        byBattleIdx = 0;
        bPass = FALSE;
        bFirstPass = FALSE;
        byBattleNum = 0;
    }
};

// 副本结构体
struct SInstanceID
{
    UINT16 wSceneIdx;
    UINT8 byTownIdx;
    UINT8 byInstanceIdx;
};

/*
//一个城镇拥有的副本信息
typedef HashMap<UINT8, CInstanceRecord*> CIndex2InstanceRecordMap;
typedef CIndex2InstanceRecordMap::iterator CIndex2InstanceRecordMapItr;

//一张地图拥有的城镇信息
typedef HashMap<UINT8, CIndex2InstanceRecordMap> CIndex2TownRecordMap;
typedef CIndex2TownRecordMap::iterator CIndex2TownRecordMapItr;

//所有的地图信息
typedef HashMap<UINT16, CIndex2TownRecordMap> CIndex2SceneRecordMap;
typedef CIndex2SceneRecordMap::iterator CIndex2SceneRecordMapItr;
*/

typedef CShmemMap<UINT32, CInstanceRecord> CInstanceRecordShmemMap; //key为InstanceID

class CMonsterGroup;

//副本通关奖励记录
struct SPassEncourage
{
    SPassEncourage()
    {
        Clear();
    }
    VOID Clear()
    {
        dwCoin = 0;
        dwGold = 0;
        wPhyStrength = 0;
        dwScience = 0;
        dwExperience = 0;
        byHitItemIdx = 0;
    }
    UINT32          dwCoin;
    UINT32          dwGold;
    UINT16          wPhyStrength;
    UINT32          dwScience;
    UINT32			dwExperience;//经验,每个武将都一样
    UINT8			byHitItemIdx;
};


struct SSceneInstanceRecord;
class CInstance: public CBaseObj
{
public:
    CInstance();
    ~CInstance();

public:
    DT_INSTANCE_ID& GetMaxCommonInstanceID()
    {
        return m_stMaxCommonInstanceID;
    };
    DT_INSTANCE_ID& GetMaxEliteInstanceID()
    {
        return m_stMaxEliteInstanceID;
    };
    DT_INSTANCE_ID& GetMaxClimbTownInstanceID()
    {
        return m_stMaxClimbTowerInstanceID;
    };
	DT_INSTANCE_ID& GetMaxNaiDanInstanceID()
	{
		return m_stMaxNanDanInstanceID;
	};

    //初始化相关对象
    BOOL	Init(DT_INSTANCE_DB_DATA &stDT_INSTANCE_DB_DATA, CPlayer* poOwner);
    CInstanceRecordShmemMap& GetCommonInstanceRecord()
    {
        return m_mapCommonInstanceRecord;
    }
    CInstanceRecordShmemMap& GetEliteInstanceRecord()
    {
        return m_mapEliteInstanceRecord;
    }
    CInstanceRecordShmemMap& GetClimbTowerInstanceRecord()
    {
        return m_mapClimbTowerInstanceRecord;
    }
	CInstanceRecordShmemMap& GetOutBoundInstanceRecord()
	{
		return m_mapOutBoundRecord;
	}
	CInstanceRecordShmemMap& GetNeiDanInstanceRecord()
	{
		return m_mapNeiDanInstanceRecord;
	}

	
	UINT8 GetNewCommonInstanceID(UINT8 UnlockNewInstanceFlag, DT_INSTANCE_DATA_CLI& stNewCommonInstanceID);//获取新可打普通副本,1表示有新的可打副本


    VOID	ExitInstanceWhenPass(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byScore); //通关退出副本回调，记录进副本信息(进入次数，若是第一次通关记录评分)，计算下一个副本可以打的副本，保存db
    VOID	DoUnlock(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8& byBuildID, UINT16 awUnlockHeroID[MAX_UNLOCK_HERO_NUM]);//解锁相应功能

    VOID	AddSceneInstanceRecord(UINT16 wScenceIdx, DT_INSTANCE_DATA_LIST& stInstanceData);//添加场景副本记录信息

    BOOL	GetInstanceRecord(UINT16 wSceneIdx, PKT_CLIGS_GET_SCENE_INSTANCE_ACK& stAck);//获取场景副本记录信息,没有返回FALSE
    BOOL	CkHaveRecord(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx);//检查是否存在副本记录
	UINT16  GetOutBoundInfo(PKT_CLIGS_OPEN_OUT_BOUND_ACK& stAck);//外传目录
	UINT16  OnOpenOutBoundMapAck(PKT_CLIGS_OPEN_OUT_BOUND_MAP_REQ& stReq, PKT_CLIGS_OPEN_OUT_BOUND_MAP_ACK& stAck);
	UINT16 OnOutBoundBattleAck(PKT_CLIGS_OUT_BOUND_BATTLE_REQ& stReq, PKT_CLIGS_OUT_BOUND_BATTLE_ACK& stAck);
	UINT16  GetDetailOutBound(UINT8 byTownIdx, DT_OUT_BOUND_DETAIL_LST_DATA& stInstanceLstInfo);
    UINT16	CkCanEnter(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byNewFlag);//检查是否可以进入副本

    SCurBattleInstance& GetCurBattleInstance()
    {
        return m_stCurBattleInstance;
    }

    BOOL	EnterInstance(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx);//进入副本回调
    UINT16	EnterBattle(UINT8 byBattleIndex);//战斗回调
    BOOL	BattlePass(CMonsterGroup* poMonsterGroup, UINT8 byBattleIndex);//战斗通过回调,战斗通过后第一个回调
    BOOL	ExitInstance();//离开副本回调

    CInstanceRecord* GetSingleInstaceRecord(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, BOOL bGetSuccessRecord = TRUE);//获取单个副本记录，

    UINT8	GetCanCarryNum();//获取当前可携带个数，0为错误

    DT_INSTANCE_ID* GetCurMaxUnlockEliteInstanceID();

    //DT_INSTANCE_ID GetCurMaxUnlockClimbTowerInstanceID();

    UINT16  OpenEncourageBox(DT_PASS_ENCOURAGE_BOX_DATA& stDT_PASS_ENCOURAGE_BOX_DATA, DT_DATA_AFTER_PASS_ENCOURAGE_BOX* pstAfterEncourage); //打开副本奖励宝箱，进行奖励
    static BOOL	CkLessEqualID(UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byTargeTownIdx, UINT8 byTargeInstanceIdx);//检查副本ID是否小于等于

    //////////////////
    CInstanceBattleRcd& GetCommonBattleRcd()
    {
        return m_oCommonInstanceBattleRcd;
    }

    CInstanceBattleRcd& GetEliteBattleRcd()
    {
        return m_oEliteInstanceBattleRcd;
    }

    CInstanceBattleRcd& GetClimbTowerBattleRcd()
    {
        return m_oClimbTowerInstanceBattleRcd;
    }

    //////////////////////////////
    VOID	PassEncourage(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byRunTimes = 1, DT_PASS_ENCOURAGE_DATA* pstDT_PASS_ENCOURAGE_DATA = NULL, DT_DATA_AFTER_PASS_ENCOURAGE* pstAfterEncourage = NULL);//通关奖励
    VOID	RatePassBoxEnc(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, DT_PASS_ENCOURAGE_BOX_DATA& stPassEncourageBox, BOOL bOnhook = FALSE);//计算通关宝箱奖励
    UINT16	PassBoxEnc(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, DT_PASS_ENCOURAGE_BOX_DATA& stDT_PASS_ENCOURAGE_BOX_DATA, DT_DATA_AFTER_PASS_ENCOURAGE_BOX* pstAfterPassEncourage = NULL);//通关宝箱奖励
    ///////////////
    VOID	GenEncInfo(UINT8& byHaveBoxFlag, DT_PASS_ENCOURAGE_BOX_DATA& stBoxInfo, DT_HERO_UPGRADE_DATA_LIST& stHeroUpgradeInfo);
    VOID	ClrChallengeTime(); //清除挑战时间
    UINT16 ResetEliteTime(UINT8 byTownIdx);//重置挑战时间

    //获取精英副本的主要数据
    VOID GetEliteInstanceData( PKT_CLIGS_RESET_ELITE_INSTANCE_ACK &ackData, UINT8 byIndex );

    UINT16 ResetChallengeTime(UINT8 byTownIdx);//重置挑战时间
    BOOL	GetClimbTowerInstanceAck(PKT_CLIGS_OPEN_CLIMB_TOWER_ACK& stAck);

    VOID GetClimbTownInstanceNum(UINT32 &dwMaxInstanceNum, UINT32  &dwMaxUnlockInstanceNum);//副本数//副本记录信息

    UINT16 GetClimbTownInstanceInfo(UINT8 &byMaxInstanceNum, UINT8  &byMaxUnlockInstanceNum, DT_INSTANCE_RECORD_DATA_CLI &stInstanceInfo);//副本数//副本记录信息

    void	DelClimbTownInstaceRecord(UINT16 wSceneIdx, UINT8 byTownIdx);//删除单个副本记录，
    BOOL	CkEliteUnlock(UINT8 byTownIdx, UINT8 byInstanceIdx); //检查是否解锁该精英副本

    //获取当前玩家爬塔层数。
    UINT16 GetMaxPassClimbTower();

    //获取指定城镇已经战斗过的精英副本
    UINT8 GetAlreadyBattleMaxIndex( UINT8 byTownIdx );

    //获取精英副本信息
    BOOL	GetEliteInstanceRecordAck(PKT_CLIGS_GET_ELITE_INSTANCE_ACK& stAck);

    //获取精英副本VIP扩展信息
    DT_ELITE_INSTANCE_VIP_EXT_DATA_CLI GetEliteInstanceVipExt();


    VOID		GetEliteInstanceNum(UINT32 &dwMaxInstanceNum, UINT32  &dwMaxUnlockInstanceNum);//副本数//副本记录信息

    UINT8	GetEliteInstanceRecordData( DT_ELITE_INSTANCE_RECORD_DATA_CLI &stData, DT_ELITE_INSTANCE_RECORD_SCORE_DATA_CLI &stScoreData, UINT8 byTownIndex);

    UINT16	GetCurrentClimbTower();

    UINT16	RecvRatingEncourageReq(UINT16 wSceneIdx, UINT8 byType, UINT16 wChapterIdx, DT_ACCUMULATED_SCORE_ENCOURAGE &stAccumulatedEncourage);

    VOID		AddFailedRecord(UINT16 wScenceIdx, UINT8 byTownIdx, UINT8 byInstanceIdx);

    // 获取掉落指定物品的玩家已通关外传副本
    DT_INSTANCE_ID GetMaxPassInstanceOfItem(UINT16 wItemID);
	UINT8 GetInstanceStateByNeiDan(UINT8 byTownIdx);
	UINT16 GetNeiDanCanBattleNum();
protected:
    VOID		GiveFirstInsance(PKT_CLIGS_GET_SCENE_INSTANCE_ACK& stAck);//给第一个普通副本


    BOOL		GetCommonInstanceRecord(PKT_CLIGS_GET_SCENE_INSTANCE_ACK& stAck);

    UINT8	UnlockBuild(SInstanceProp* pstInstanceProp);//建筑解锁，副本通过回调,返回解锁建筑ID
    UINT16	UnlockHero(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT16 awUnlockHeroID[MAX_UNLOCK_HERO_NUM]);//武将解锁，副本通过回调,武将存放于玩家的招募馆中,返回解锁的武将类型(0没有解锁)
    UINT32	UnlockEliteTownIdx(SInstanceProp* pstInstanceProp);//精英副本解锁，副本通过回调,返回解锁的精英副本城镇序号(0没有解锁)

    BOOL		GetRatingEncourage(SScoreProp *poScoreProp);
    BOOL		GetRatingEncourage(CScoreAccumulateProp *poScoreAccumulateProp);

    BOOL		PackRatingEncourage(DT_ENCOURAGE_DATA &stChapterEncourage, SScoreProp *poScoreProp);
    BOOL		PackRatingEncourage(DT_ENCOURAGE_DATA &stAccumulatedEncourage, CScoreAccumulateProp *poScoreAccumulateProp);


private:
    DT_INSTANCE_ID				m_stMaxCommonInstanceID;//最大普通副本记录信息(当前打到的最高副本记录(通关后重新赋值))
    DT_INSTANCE_ID				m_stMaxEliteInstanceID; //最大精英副本记录信息(当前打到的最高副本记录(通关后重新赋值))
    DT_INSTANCE_ID				m_stMaxClimbTowerInstanceID; //最大精英副本记录信息(当前打到的最高副本记录(通关后重新赋值))
	DT_INSTANCE_ID				m_stMaxNanDanInstanceID; //最大斩兽炼魂副本记录信息(当前打到的最高副本记录(通关后重新赋值))

    CInstanceRecordShmemMap		m_mapCommonInstanceRecord; //通关的普通场景副本记录，不包括未通关的
    CInstanceRecordShmemMap		m_mapEliteInstanceRecord; //通关的精英场景副本记录，不包括未通关的
    CInstanceRecordShmemMap		m_mapClimbTowerInstanceRecord; //通关的精英场景副本记录，不包括未通关的
	CInstanceRecordShmemMap		m_mapNeiDanInstanceRecord; //通关的斩兽炼魂场景副本记录，不包括未通关的
	CInstanceRecordShmemMap		m_mapOutBoundRecord;//通关外传，不包括未通关的

    SCurBattleInstance			m_stCurBattleInstance;	//当前进入副本记录

    CInstanceBattleRcd			m_oCommonInstanceBattleRcd;//普通副本战斗记录，不通关，通关则清记录（副本ID除ScenceID全0）
    CInstanceBattleRcd			m_oEliteInstanceBattleRcd;	//精英副本战斗记录，不通关，通关则清记录（副本ID除ScenceID全0）
    CInstanceBattleRcd			m_oClimbTowerInstanceBattleRcd;	//精英副本战斗记录，不通关，通关则清记录（副本ID除ScenceID全0）

    UINT8						m_byCommonPhyStrength;//进入普通副本所需体力
    BOOL							m_bHaveEnterInstance;//是否已进入过副本,用于判断是否扣体力，重启gs或者已清除内存再登录游戏最多能刷1次
	UINT64 m_qwLastResetOutBoundTime;//最后重置外传副本时间
};

DECLARE_CLASS_POOL_MGR(CInstance)

#endif //#ifndef 

