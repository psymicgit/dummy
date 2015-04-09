
#ifndef _PLUNDERMGR_H_
#define _PLUNDERMGR_H_

#include "plunder.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <protocol/server/protocligs.h>
#include <common/client/commondef.h>
#include <logic/player/playerbasedatacachemgr.h>
#include <logic/event/releasecaptureevent.h>

using namespace SGDP;
using namespace std;

typedef HashMap<UINT32, CPlunder*> CID2PlunderMap;
typedef CID2PlunderMap::iterator	CID2PlunderMapItr;


class CPlunderMgr
{

public:
    DECLARE_SINGLETON(CPlunderMgr)

    BOOL Init();

    VOID UnInit();

public:

public:
    BOOL		Plunder(CPlayer* poMyself, CPlayer* poEnemy, CPlayer* poCapture, PKT_CLIGS_PLUNDER_REQ& stReqRaram, DT_BATTLE_DATA5& stDT_BATTLE_DATA, DT_PLUNDER_ENCOURAGE_DATA& stPlunderEncourage);
    BOOL		Plunder2(CPlayer* poMyself, CPlayer* poEnemy, CPlayer* poCapture, PKT_CLIGS_PLUNDER_REQ& stReqRaram, DT_BATTLE_DATA3& stDT_BATTLE_DATA, DT_PLUNDER_ENCOURAGE_DATA& stPlunderEncourage);
	BOOL		Plunder3(CPlayer* poMyself, CPlayer* poEnemy, CPlayer* poCapture, PKT_CLIGS_PLUNDER_REQ& stReqRaram, DT_BATTLE_DATA4& stDT_BATTLE_DATA, DT_PLUNDER_ENCOURAGE_DATA& stPlunderEncourage);
    UINT16	OpenPlunderScence(CPlayer* poOwner, UINT16& wCanPlunderNum, UINT16& wFreePlunderNum, UINT32& dwPlunderGold,  UINT16& wPlunderItemNum,
                                  UINT8& byPlunderPlayerNum, DT_PLUNDER_PLAYER_DATA astPlunderPlayerInfo[MAX_GET_PLUNDER_ENEMY_NUM]);//打开掠夺场景(推荐掠夺对象)

    VOID		GetPlunderLevelGapItem(UINT8& byUseItemGapLevel, UINT32& dwPlunderItemPrice, UINT16& wPlunderItemID);

    UINT16	RefreshPlunderRecommend(CPlayer* poOwner, UINT8& byPlunderPlayerNum, DT_PLUNDER_PLAYER_DATA astPlunderPlayerInfo[MAX_GET_PLUNDER_ENEMY_NUM]);//属性掠夺玩家

    BOOL		AddReleaseBattleLog(UINT32 dwOwnerID, UINT32 dwCaptureID, EPlunderBattleLogType eReleaseType, UINT32 dwCoin, UINT32 dwScience);//添加俘虏释放战报
public:
    VOID		OnReleaseCaptureEvent(UINT32 dwOwnerID, UINT32 dwCaptureID); //需释放俘虏事件
    VOID		GetLeftIncome(CPlayer* poOwner, UINT32 dwCaptureID, UINT32& dwCoin, UINT32& dwScience, BOOL bManualFree = FALSE);//获取留下收益
protected:
    BOOL OnPlunder(CPlayer* poMyself, CPlayer* poEney, CPlayer* poCapture, PKT_CLIGS_PLUNDER_REQ& stReqRaram, BOOL bChallengeSuccess, DT_PLUNDER_ENCOURAGE_DATA& stPlunderEncourage);//掠夺回调
    BOOL AddPlunderBattleLog(CPlayer* poMyself, CPlayer* poEney, CPlayer* poCapture, UINT8 byResistFlag, BOOL bChallengeSuccess, UINT64 qwPlunderCoin, UINT64 qwPlunderScience); //添加掠夺战报
    EPlunderBattleLogType GetPlunderBattleLogType(CPlayer* poEney, CPlayer* poCapture, UINT8 byResistFlag);
    VOID CaptureChange(CPlayer* poMyself, CPlayer* poEney, CPlayer* poCapture, UINT8 byResistFlag, UINT8 byCaptureGridIdx, BOOL bChallengeSuccess);
    VOID PlunderEncourage(CPlayer* poMyself, CPlayer* poEney, UINT8 byResistFlag, BOOL bChallengeSuccess, UINT32& dwCoin, UINT32& dwScience);//掠夺俘虏奖励
    BOOL InitRobot();//初始化掠夺机器人
    VOID GetRecommendRobot(UINT16 wLevel, UINT8& byPlunderPlayerNum, DT_PLUNDER_PLAYER_DATA astPlunderPlayerInfo[MAX_GET_PLUNDER_ENEMY_NUM], CPlunder &oOwnPlunder);//获取推荐玩家，20~21 机器人
    VOID GetRecommendLimit(UINT32 dwPlayerID, UINT16 wPlayerLevel, SRecommendPlayerRcd& stRecommendPlayerRcd, UINT8& byPlunderPlayerNum,
                           DT_PLUNDER_PLAYER_DATA astPlunderPlayerInfo[MAX_GET_PLUNDER_ENEMY_NUM], BOOL bNotBecapture, CPlunder &oOwnPlunder);//获取推荐玩家，22~25 没有被俘虏,25以上 等级限制
    VOID GetPlayerData(CPlayerBaseDataVec& vecPlayer, UINT8& byPlunderPlayerNum, DT_PLUNDER_PLAYER_DATA astPlunderPlayerInfo[MAX_GET_PLUNDER_ENEMY_NUM], C3232Map& mapHave);//获取玩家信息
protected:
    CPlunderMgr();
    ~CPlunderMgr();


private:
    DT_PLUNDER_PLAYER_DATA				m_astPlunderRobot[MAX_ROBOT_NUM];//掠夺机器人
    BOOL								m_bRobotInit; //机器人是否初始化
    UINT32								m_dwPlunderItemPrice;//掠夺道具价格
    UINT16								m_wPlunderItemID;//掠夺道具ID
    UINT32								m_dwCaptureWorkSecond;//俘虏工作时间
    UINT16								m_wPlunderOpenLevel;//掠夺开放等级
};



#endif

