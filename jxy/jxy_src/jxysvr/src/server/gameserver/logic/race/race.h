
#ifndef _RACE_H_
#define _RACE_H_

#include <sdtype.h>
#include <logic/base/baseobj.h>
#include <vector>
#include <deque>
#include "racebattlelog.h"
#include <protocligs.h>
#include <common/server/utility.h>

using namespace std;

//typedef CShmemMap<UINT64, CRaceBattleLog, MAX_RACE_BATTLELOG_NUM> CRaceBattleLogMap;//key为挑战时间
//typedef CRaceBattleLogMap::iterator CRaceBattleLogMapItr;

typedef deque<CRaceBattleLog*> CRaceBattleLogDeq;
typedef CRaceBattleLogDeq::iterator CRaceBattleLogDeqItr;


class CPlayer;
class CRace:public CBaseObj
{
public:
	CRace();
	~CRace();
public:
	BOOL Init(DT_RACE_DATA& stDT_RACE_DATA, CPlayer* poOwner);
	DT_RACE_BASE_DATA& GetDT_RACE_BASE_DATA() { return m_stDT_RACE_BASE_DATA; }
	//CPlayer* GetOwner() { return m_poOwner; }
	UINT16	CkCondition();	
	VOID		ChallengeEncourage(BOOL bSuccess, DT_RACE_CHALLENGE_ENCOURAGE_DATA& stChallengeEncourage);
	UINT16	RankEncourage(DT_RACE_RANK_ENCOURAGE_DATA& stDT_RACE_RANK_ENCOURAGE_DATA);
	UINT16	ClrChallengeCD();

	UINT16	GetChallengeTimes(UINT32 &dwTimes, UINT32 &dwLeftTimes);
	
	/////////////////////////////////////////////////
	BOOL AddBattleLog(DT_RACE_BATTLELOG_DATA* pstDT_RACE_BATTLELOG_DATA, BOOL bActiveFlag = FALSE);
	CRaceBattleLogDeq& GetBattleLogDeq() { return m_deqRaceBattleLog; }	
	VOID GetShowRaceBattleLog(UINT8& byShowNum, DT_RACE_BATTLELOG_DATA_CLI astRankBattleLogInfo[MAX_RACE_BATTLELOG_NUM]);//获取给客户端显示的战报
	///////////////////////////////////////////////////////
	//挑战完成回调(扣除元宝等)
	VOID OnRaceComplete(BOOL bSuccess);

	BOOL GetPrompt(DT_FUNC_COMMON_PROMPT& stRacePrompt); //竞技场提示信息
protected:
	UINT16 CkChallengeCD();	
	UINT16 CkRaceNeedGold();//检查收费挑战元宝是否足够
private:
	DT_RACE_BASE_DATA		m_stDT_RACE_BASE_DATA;
	CRaceBattleLogDeq	m_deqRaceBattleLog;	
	UINT32				m_dwRaceNeedGold;//收费掠夺需要的元宝
};
DECLARE_CLASS_POOL_MGR(CRace)

#endif

