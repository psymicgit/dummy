
#ifndef _RACEPROPMGR_H_
#define _RACEPROPMGR_H_

#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basedefine.h>
#include <logic/base/basepropmgr.h>

using namespace SGDP;
using namespace std;

class CPlayer;

struct tagDT_RACE_RANK_ENCOURAGE_DATA;
typedef tagDT_RACE_RANK_ENCOURAGE_DATA DT_RACE_RANK_ENCOURAGE_DATA;

struct tagDT_RACE_CHALLENGE_ENCOURAGE_DATA;
typedef tagDT_RACE_CHALLENGE_ENCOURAGE_DATA DT_RACE_CHALLENGE_ENCOURAGE_DATA;

struct SRaceBaseProp
{
	//UINT16 wMaxChallengeNumPerDay; //一天最多可挑战次数
	UINT16 wFreeChallengeNumPerDay;//一天可免费挑战次数
	UINT32 dwChallengeFailedCD;//挑战输后距离下次挑战CD,单位秒
	UINT32 dwClrCDGoldPerMinute;//清除CD时间每分钟所需元宝
	UINT8  byChallengeNum;//ChallengeNum为每次显示可挑战的玩家(等级高于自己的)个数，若少于ChallengeNum，则去后面排名的玩家补上（后面补的间隔1)
	UINT16 wRankShowNum;//RankShowNum为排行榜显示的排行前多少名并用于挑战
	UINT32 dwLimitRank;//LimitRank为临界排名,用于挑战战斗次数(在该排名以内则战斗2次)	
	UINT32 dwRobotID;//机器人ID
	UINT16 wRecvRankPassDay;//多几天后可以领取排名奖励
};

//挑战区间段配置
struct SChallengeSec
{
	UINT32 dwMin; //起始排名
	UINT32 dwMax; //结束排名，0表示最大
	UINT16 wInterval;//间隔
};

typedef vector<SChallengeSec> CChallengeSecVec; //按dwMix升序排序
typedef CChallengeSecVec::iterator CChallengeSecVecItr;

typedef HashMap<UINT32, UINT32> CChanllengeEncourageMap;//可以为玩家等级
typedef CChanllengeEncourageMap::iterator CChanllengeEncourageMapItr;


/**
* @brief 竞技场配置管理类
*/
class CRacePropMgr : public CBasePopMgr
{
public:
	DECLARE_SINGLETON_PROPMGR(CRacePropMgr)

	virtual EConfigModule	GetConfigModuleType() { return ECM_RACE ; }

	BOOL Init();
	VOID UnInit();	

	UINT32	GetChallengeNum() { return m_stRaceBaseProp.byChallengeNum; }
	UINT32	GetInterval(UINT32 dwRank);//获取挑战间隔	
	VOID	GetRankEncourage(UINT32 dwRank, DT_RACE_RANK_ENCOURAGE_DATA& stRankEncourage);
	VOID	GetChallengeEncourage(UINT32 dwPlayerLevel, BOOL bWinFlag, DT_RACE_CHALLENGE_ENCOURAGE_DATA& stChallEncourage);
	VOID	GetCanChallengeTimes(CPlayer* poPlayer, UINT64 qwLastChallTime, UINT8 byChallTimes, UINT16 wContiWin, UINT8 byFailedFlag, UINT16& wChallengeCountDown, 
		UINT16& wTotalChallengeNum, UINT16& wCanChallengeNum, UINT16& wVipExtNum, UINT16& wFreeChallengeNum, UINT32& dwChallengeNeedGold);
	SRaceBaseProp& GetRaceProp() { return m_stRaceBaseProp; }
	CChallengeSecVec& GetChallengeSecVec() { return m_vecChallengeSec; }	
public:
	CRacePropMgr();
	~CRacePropMgr();
protected:
	BOOL LoadFromDB();
	BOOL LoadRaceBaseProp();
	BOOL LoadChallengeSec();
	//BOOL LoadCRankEncourage();
	//BOOL InitChallengeEncourage();

	static BOOL MyCompare(const SChallengeSec &t1, const SChallengeSec &t2);

private:
	SRaceBaseProp		m_stRaceBaseProp;
	CChallengeSecVec	m_vecChallengeSec;//配置需按升序配置
	//CChanllengeEncourageMap m_mapChanllengeEncCoin;
	//CChanllengeEncourageMap m_mapChanllengeEncScience;
};



#endif
