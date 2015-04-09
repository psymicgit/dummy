
#ifndef _RDCHALLENGE_H_
#define _RDCHALLENGE_H_

#include <logic/base/baseobj.h>
#include <logic/base/basedefine.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <protocligs.h>
#include <common/server/utility.h>
using namespace std ;

typedef map<UINT32, DT_RDC_PLAYER_DATA*> CRDCPlayerMap;
typedef CRDCPlayerMap::iterator CRDCPlayerMapItr;

class CPlayer;
class CRdChallenge: public CBaseObj
{
public:
    CRdChallenge();
    ~CRdChallenge();
public:
    //初始化相关对象
    BOOL Init(DT_RANDOM_CHALLENGE_DATA& stDT_RANDOM_CHALLENGE_DATA, CPlayer* poOwner);
    DT_RANDOM_CHALLENGE_DATA& GetDT_RANDOM_CHALLENGE_DATA() { return m_stDT_RANDOM_CHALLENGE_DATA; }
	UINT16 OpenRdchallengeTab(UINT8 byRefreshFlag, DT_RDC_TAB_DATA& stTabInfo);
	UINT16 Rdchallenge(CPlayer * poEnemyPlayer, DT_BATTLE_DATA5& stBattleInfo, DT_RDC_TAB_DATA& stTabInfo, DT_RDC_ENC_DATA& stEncInfo ,UINT16 &wKindID);
    UINT16 Rdchallenge2(CPlayer * poEnemyPlayer, DT_BATTLE_DATA3& stBattleInfo, DT_RDC_TAB_DATA& stTabInfo, DT_RDC_ENC_DATA& stEncInfo ,UINT16 &wKindID);
	UINT16 Rdchallenge3(CPlayer * poEnemyPlayer, DT_BATTLE_DATA4& stBattleInfo, DT_RDC_TAB_DATA& stTabInfo, DT_RDC_ENC_DATA& stEncInfo ,UINT16 &wKindID);
	UINT16 ClrRDChallengeCD();
	UINT16	GetChallengeTimes(UINT32 &dwLeftTimes, UINT32 &dwTimes);
	BOOL GetPrompt(DT_FUNC_COMMON_PROMPT& stPvpPrompt);
protected:	
	VOID GetRdChallengePointInfo(DT_RDCHALLENGE_POINT_DATA_CLI& stRdChallengePointInfo);
	VOID GetRdPlayerInfo(UINT8& byPlayerNum, DT_RDC_PLAYER_DATA_CLI astPlayerInfo[MAX_GET_RDC_NUM]);
	VOID RdChallengePoinRecover();	
	UINT16 GetChallengeCountDown();
	VOID GetPlayerData(CPlayerBaseDataVec& vecPlayer, UINT8& byPlayerNum, DT_RDC_PLAYER_DATA_CLI astPlayerInfo[MAX_GET_RDC_NUM], C3232Map& mapHave);
	VOID GetRdEncInfo(DT_RDC_PLAYER_DATA_CLI& stPlayerData);
private:
    DT_RANDOM_CHALLENGE_DATA	m_stDT_RANDOM_CHALLENGE_DATA;
	SRecommendPlayerRcd		m_stRecommendPlayerRcd;
	BOOL						m_bFirst;
	CRDCPlayerMap				m_mapChallengePlayerData;//刷出来的玩家
};

DECLARE_CLASS_POOL_MGR(CRdChallenge)
#endif //#ifndef 

