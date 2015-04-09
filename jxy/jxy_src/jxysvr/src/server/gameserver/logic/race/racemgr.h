
#ifndef _RACEMGR_H_
#define _RACEMGR_H_

#include "race.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <map>

using namespace SGDP;
using namespace std;

class CRaceBattleLog;

typedef HashMap<UINT32, CRace*> CID2RaceMap;
typedef CID2RaceMap::iterator	CID2RaceMapItr;

typedef HashMap<UINT64, CRaceBattleLog*> CID2RaceBattleLogMap;
typedef CID2RaceBattleLogMap::iterator	CID2RaceBattleLogMapItr;

struct SPlayerBaseData;
typedef map<UINT32, SPlayerBaseData*> CRank2PlayerBaseDataMap;//用map(HashMap排序有问题)， key为rank
typedef CRank2PlayerBaseDataMap::iterator CRank2PlayerBaseDataMapItr;

typedef vector<SPlayerBaseData*> CRank2PlayerBaseDataVector;
typedef CRank2PlayerBaseDataVector::iterator CRank2PlayerBaseDataVectorItr;


typedef map<UINT32, SPlayerBaseData*> CRepeatRank2PlayerBaseDataMap;//用map(HashMap排序有问题)， key为玩家ID
typedef CRank2PlayerBaseDataMap::iterator CRepeatRank2PlayerBaseDataMapItr;

// 启动时载入的离线竞技场玩家列表
typedef map<UINT32, CPlayer*> COfflineRacerMap;                // key = 排名，val = 离线的竞技场玩家

// 启动时载入的离线竞技场玩家基础信息列表
typedef map<UINT32, SPlayerBaseData> COfflineRacerBaseDataMap; // key = 排名，val = 离线竞技场玩家的基础信息

class CPlayerBaseDataCacheMgr;
class CGSProcessor;
class CRaceMgr
{
	friend class CPlayerBaseDataCacheMgr;
	friend class CGSProcessor;
public:
    DECLARE_SINGLETON(CRaceMgr)

    BOOL Init();

    VOID UnInit();
public:
    // 载入竞技场的前几名(一般是前100名)玩家以重新计算其战力
    VOID LoadTopRacerPower();

    // 服务器启动时竞技场的玩家载入后的回调
    VOID OnLoadOfflineRacerAtStartUp(CPlayer *poOfflineRacer);

private:
    // 服务器启动时的前几名玩家已全部载入成功
    VOID OnAllOfflineRacerLoaded();

    // 将启动时载入的竞技场玩家清空掉
    VOID ClearOfflineRacerAtStartUp();

    // 将启动时载入的竞技场玩家最新战力回写到数据库
    VOID SaveOfflineRacerPowerAtStartUp();

    // 打印出竞技场玩家新旧战力对比列表，并返回战力前后不匹配的玩家数量
    UINT16 EchoRacerPowerComparison();

public:
	UINT32		GetNewRankID() { return ++m_dwMaxRank; };//获取排名
	VOID		ReturnRankID() { if(m_dwMaxRank > 0) { m_dwMaxRank--; } };//归还排名

	SPlayerBaseData* GetPlayerBaseData(UINT32 dwRank);

	BOOL		CkOutChallengeRang(UINT32 dwMyselfRank, UINT32 dwEnemyRank);//检查是否超出挑战范围,TRUE为超出范围

	VOID		GetChallengePlayerData(UINT32 dwPlayerID, DT_CHALLENGE_PLAYER_DATA& stDT_CHALLENGE_PLAYER_DATA);
	VOID		GetChallengePlayerData(const SPlayerBaseData* pstPlayerBaseData, DT_CHALLENGE_PLAYER_DATA& stDT_CHALLENGE_PLAYER_DATA);

	BOOL		Challenge(CPlayer* poMyself, CPlayer* poEnemy, DT_BATTLE_DATA5& stDT_BATTLE_DATA, DT_RACE_CHALLENGE_ENCOURAGE_DATA& stChallengeEncourage);	
    BOOL		Challenge2(CPlayer* poMyself, CPlayer* poEnemy, DT_BATTLE_DATA3& stDT_BATTLE_DATA, DT_RACE_CHALLENGE_ENCOURAGE_DATA& stChallengeEncourage);	
	BOOL		Challenge3(CPlayer* poMyself, CPlayer* poEnemy, DT_BATTLE_DATA4& stDT_BATTLE_DATA, DT_RACE_CHALLENGE_ENCOURAGE_DATA& stChallengeEncourage);	
	SPlayerBaseData* GetRankPlayerBaseData(UINT32 dwRank);
public:
	VOID		GetChallengePlayerList(UINT32 dwRank, UINT8& byChallengeNum, DT_CHALLENGE_PLAYER_DATA astChallengePlayerList[]);
	UINT16		GetSHowRank(UINT8& byShowNum, DT_RANK_DATA astRankShowInfo[MAX_RANK_SHOW_NUM]);
	
protected:
	BOOL		OnChallenge(CRace* poChallenge, CRace* poBeChallenge ,BOOL bChallengeSuccess);//挑战回调
protected:
    CRaceMgr();
    ~CRaceMgr();
	BOOL InitEvent();


private:
	CRank2PlayerBaseDataMap	m_mapRank2PlayerBaseData; //key为Rank
	UINT32					m_dwMaxRank;//当前最大排行

    COfflineRacerMap         m_mapOfflineRacerAtStartUp;    // 服务器启动时载入的竞技场前几名玩家表
    UINT16                   m_wLastOfflineRacerRank;       // 服务器启动时载入的最后一个离线玩家的排名
    COfflineRacerBaseDataMap m_mapOfflineRacerBaseData;     // 离线竞技场玩家的基础信息
};



#endif

