
#ifndef _BOSSBATTLEMGR_H_
#define _BOSSBATTLEMGR_H_

#include "bossbattle.h"
#include "bossbattlepropmgr.h"
#include <sdsingleton.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <sdobjectpool.h>
#include <protocligs.h>
#include <set>
using namespace SGDP;
using namespace std;


class CBossBattle;
class CPlayer;
class CBossBattlePropMgr;
class CMonster;
class CBossBBattleLog;
class CGSProcessor;

/**
* @brief 建筑管理类
*/
class CBossBattleMgr
{
	friend class CBossBattlePropMgr;
	friend class CGSProcessor;

public:
    DECLARE_SINGLETON(CBossBattleMgr)

    BOOL Init();

    VOID UnInit();

public:
	BOOL			OnStartEvent(UINT8 byActivityID);
	BOOL			OnEndEvent(UINT8 byActivityID);
	BOOL			Run();

	CBossBBattleLog*	CreateBattleLog();
	VOID				RemoveBattleLog(CBossBBattleLog* poBossBBattleLog);

protected:
	CBossBattleMgr();
	~CBossBattleMgr();

protected:
	BOOL InitModuleDataFromDB();
	BOOL InitModuleLogDataFromDB();
	BOOL InitEvent();
	BOOL LoadCloseArea();
public:
	//玩家进入
	UINT16 OnEnter(CPlayer* poPlayer, UINT16 wPosX, UINT16 wPosY, DT_BOSSB_MY_DATA_CLI& stMyBossBInfo, DT_BOSSB_PLAYER_DATA_CLI_LST& stPlayerInfo,
		DT_BOSSB_DATA_CLI2& stBossBInfo, DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM], DT_HURT_ENC_DATA& stHurtEncInfo);
	//玩家离开
	UINT16 OnLeave(CPlayer* poPlayer, BOOL bDisconnect = FALSE);
	//玩家移动
	UINT16 OnMove(CPlayer* poPlayer, UINT16 wPosX, UINT16 wPosY, UINT16& wOldPosX, UINT16& wOldPosY);
	//玩家伤害
	UINT16 OnHurt(CPlayer* poPlayer, UINT32& dwCurHurtValue, DT_BOSSB_HURT_RATE& stMyHurtInfo, DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM],
		DT_HURT_ENC_DATA& stHurtEncInfo, UINT16& wReliveCountDown, UINT16& wGoldReliveCountDown, UINT64& dwTotalBossHP, UINT64& dwCurBossHP);
	//打开鼓舞面板
	UINT16 OnOpenInspireTab(CPlayer* poPlayer, DT_INSPIRE_TAB_DATA& stInspireTabInfo);
	//鼓舞战力
	UINT16 OnInspirePower(CPlayer* poPlayer, UINT8 byGoldInspireFlag, DT_INSPIRE_TAB_DATA& stInspireTabInfo);
	//复活
	UINT16 OnBossBRelive(CPlayer* poPlayer);
	//获取上次boss战信息
	UINT16 OnGetLastBossBInfo(CPlayer* poPlayer, DT_BOSSB_OVER_INFO2& stBossOverInfo);
	
	BOOL IsCloseArea(UINT16 wZoneID);
public:
	VOID GetBattleLog(UINT64 qwOverTime, UINT16& wBossID, DT_BOSS_BATTLE_ENC_DATA astBossBTopEncInfo[MAX_BOSS_ENC_TOP_NUM], DT_BOSS_BATTLE_ENC_DATA& stBossBKillEncInfo);
	BOOL OnSave();//保存boss战模块信息
	BOOL IsOver() { return m_bActivityOver; }
	UINT8 GetLastActivityID() { return m_byLastActivityID; }

    // 活动开始时把开始通知发到活动聊天频道
    void AddChatOnActivityStart(UINT64 qwActStartTime, UINT64 qwNow);
    //活动结束时把boss战战报发到活动聊天频道
    void AddChatOnActivityOver(DT_BOSS_BATTLE_DATA& stData);
    //活动Boss被击杀时，把boss击杀信息发到活动聊天频道
    void AddChatOnBossKilled(DT_BOSS_BATTLE_DATA& stData);
	VOID ConvertInfo2ToInfo(const DT_BOSSB_OVER_INFO2& stBossInfo2, DT_BOSSB_OVER_INFO& stBossInfo);
protected:
	//添加关注
	VOID Addattention(CPlayer* poNoticePlayer);
	//去除关注
	VOID Delattention(CPlayer* poNoticePlayer);
	//获取显示玩家信息
	VOID GetShowPlayerInfo(CPlayer* poPlayer, UINT32 dwSyncStartIdx, DT_BOSSB_PLAYER_DATA_CLI_LST& stDT_BOSSB_PLAYER_DATA_CLI_LST);
	//获取显示玩家信息
	VOID GetBOSSB_PLAYER_DATA_CLI(CPlayer* poPlayer, DT_BOSSB_PLAYER_DATA_CLI& stDT_BOSSB_PLAYER_DATA_CLI);
	//计算伤害
	UINT32 CountHurtValue(CPlayer* poPlayer, DT_BOSSB_HURT_RATE& stMyHurtInfo, DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM]);
    //活动开始时把开始通知发到活动聊天频道

    //活动结束
	VOID ActivityOver(CPlayer* poKiller = NULL);
	//给奖励
	VOID GiveEnc(CPlayer* poPlayer, DT_BOSSB_OVER_INFO2& stNtf);
	//获取DT_BOSSB_MY_DATA_CLI信息
	VOID GetDT_BOSSB_MY_DATA_CLI(CPlayer* poPlayer, DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA, DT_BOSSB_MY_DATA_CLI& stMyBossBInfo);
	//获取DT_BOSSB_DATA_CLI信息
	VOID GetDT_BOSSB_DATA_CLI(DT_BOSSB_DATA_CLI2& stBossBInfo);
	//获取鼓舞面板信息
	VOID GetDT_INSPIRE_TAB_DATA(DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA, DT_INSPIRE_TAB_DATA& stInspireTabInfo);
	//获取复活倒计时
	UINT16 GetReliveCountDown(DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA, UINT8 byCalculateWalk = 0);
	//获取元宝复活操作倒计时
	UINT16 GetGoldReliveCountDown(DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA);
	//获取伤害奖励
	VOID GetHurtEncInfo(UINT16 wPlayerLevel, DT_BOSS_BATTLE_PLAYER_DATA& stDT_BOSS_BATTLE_PLAYER_DATA, DT_HURT_ENC_DATA& stHurtEncInfo, UINT32 dwHurtValue = 0);
	//获取top伤害信息
	VOID GetTopHurtRate(DT_BOSSB_HURT_RATE astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM]);
	//计算所有的排名
	VOID SortHurtRank();
	//计算TOP伤害的排名
	VOID SortTopHurt(DT_BOSSB_HURT_RATE& stMyHurtInfo);
	//同步信息
	VOID SyncInfo(CPlayer* poPlayer);

///////////////////////////////////////////////////////
	
protected:

private:
	CSDObjectPool<CBossBattle, CSDMutex> m_oBossBattleShmemObjMgr;	
	CSDObjectPool<CBossBBattleLog, CSDMutex> m_oBossBBLShmemObjMgr;	
	C32Vec		m_vecAllEnterPlayer; //	
	C3232Map	m_mapAllEnterPlayer; //	
	C3232Map	m_mapAllPlayer;//包含所有进来的
	CMonster*	m_poBoss;
	SBossBProp* m_pstCurBossBProp;//
	BOOL		m_bActivityOver;
	UINT16		m_wBossLevel;
	UINT16		m_wIncRateByStory;		//阅历提升战力百分比
	UINT16		m_wIncRateByGold;		//元宝提升战力百分比
	DT_BOSSB_HURT_RATE m_astTopHurtInfo[MAX_BOSS_ENC_TOP_NUM];
	map<UINT8, DT_BOSS_BATTLE_DATA> m_mapBassBattleModuleData;
	map<UINT64, DT_BOSS_BATTLE_DATA> m_mapBossBBLData;
	UINT64		m_qwStartTime;
	UINT8		m_byLastActivityID;
	UINT64		m_qwBossTotalHP;
	UINT8		m_bySyncActivityID;
	UINT64		m_qwSyncCurBossHP;
	UINT64		m_qwSyncStartTime;
	BOOL		m_bSyncFlag;
	C3232MapItr	m_itrCur;

	set<UINT16> m_setCloseArea;
};




#endif //#ifndef _ROLEMGR_H_

