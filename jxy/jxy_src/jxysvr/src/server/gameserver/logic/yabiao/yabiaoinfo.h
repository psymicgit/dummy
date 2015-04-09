#ifndef YABIAO_INFO_H
#define YABIAO_INFO_H

#include <deque>
#include <logic/base/baseobj.h>
#include "protocol/server/protocligs.h"
#include "yabiaobattlelog.h"
#include <common/server/utility.h>
class CPlayer;
class CYabiaoInfo: public CBaseObj
{
public:
    CYabiaoInfo();
    ~CYabiaoInfo();

    typedef deque< CYabiaoBattleLog* > CYabiaoBattleLogDeq;

public:
    //初始化相关对象
    BOOL Init(DT_YABIAO_DATA &stDT_YABIAO_DATA, CPlayer* poOwner);

    VOID UnInit();
    //重置押镖次数
    VOID Check4Reset();
    INT32 GetRemainYabiaoCnt();
    INT32 GetRemainJiebiaoCnt();

    //获得剩余被劫镖的数量
    INT32 GetRemainBeJiebiaoCnt();
    UINT8 GetYabiaoFlag();
    DT_BIAO_INFO GetCurBiaoInfo();
    //返回值为错误码
    UINT16 FlushBiaoInfo();

    //直接达到最高级
    UINT16 DirectMaxLevel();

    UINT16 GetFlushCostGold();
    UINT16 GetFlushCount();

    INT32 BeginYabiao( time_t tmBegin); //返回0表示成功,参数tmBegin为0表示沿用旧的时间,表示重新开启押镖
    BOOL EndYabiao();

    //劫镖，被劫镖
    BOOL Jiebiao( BOOL bSuccess, time_t curTime, UINT32 dwEnemyPlayerID, UINT32 dwCoin,
                  UINT32 dwScience, UINT64 qwBattleIndex, UINT8 byQuality, UINT64 qwEncourageCoin, UINT32 dwJingjie );
    BOOL BeJiebiao( BOOL bSuccess, time_t curTime, UINT32 dwEnemyPlayerID, UINT32 dwCoin,
                    UINT32 dwScience, UINT64 qwBattleIndex, UINT8 byQuality, UINT32 dwJingjie );

    DT_YABIAO_BASE_DATA &GetDT_YABIAO_BASE_DATA();

    //获得押镖已经经过的时间，如果押镖未开始，返回0
    UINT32 GetYabiaoPassedTime();

    //获得劫镖的CD
    INT32 GetRemainJiebiaoCD();

    //添加Log,0为主动劫镖，1为被劫镖, 2为押镖成功,byResult=0为己方胜利，1为对方胜利
    VOID AddYabiaoBattleLog( UINT8 byFlag, UINT64 qwBattleTime, UINT32 dwEnemyPlayerID,
                             UINT8 byResult, UINT32 dwCoinChange, UINT32 dwScienceChange, UINT64 qwBattleIndex, UINT8 byQuality, UINT32 dwJingjie, UINT8 byNewFlag = TRUE );

    CYabiaoBattleLogDeq &GetYabiaoBattleLog();

    VOID OnYabiaoBattleLogRead();
    UINT64 GetLastEncourageCoin();
    UINT64 GetLastEncourageScience();
    UINT64 GetLastEncourageJingjie();

    INT32 OnObtainEncourage();

    UINT8 GetUnreadBattleLogNum();

    INT32 GetClearJiebiaoCDGold();
    VOID ClearJiebiaoCD();
    //VOID OnCrocessDay();
    BOOL IsBiaoQualityMaxLevel();

    UINT16 GetCurBiaoQuality();

    VOID SetJiebiaoCache5( INT32 nIndex, const PKT_CLIGS_BEGIN_JIEBIAO_ACK5 &stBeginJiebiaoAck );
    PKT_CLIGS_BEGIN_JIEBIAO_ACK5 *GetJiebiaoCache5( INT32 nIndex );

    VOID SetJiebiaoCache3( INT32 nIndex, const PKT_CLIGS_BEGIN_JIEBIAO_ACK3 &stBeginJiebiaoAck );
    PKT_CLIGS_BEGIN_JIEBIAO_ACK3 *GetJiebiaoCache3( INT32 nIndex );

	VOID SetJiebiaoCache4( INT32 nIndex, const PKT_CLIGS_BEGIN_JIEBIAO_ACK4 &stBeginJiebiaoAck );
	PKT_CLIGS_BEGIN_JIEBIAO_ACK4 *GetJiebiaoCache4( INT32 nIndex );

    vector<UINT32> GetJiebiaoPlayerID();

    UINT64 GetJiebiaoCoinEncourage( UINT16 wYabiaoLevel, UINT16 wJiebiaoLevel );
    UINT32 GetJiebiaoScienceEncourage( UINT16 wYabiaoLevel, UINT16 wJiebiaoLevel );

public:
	//INT32 GetRemainYabiaoCnt();
	//INT32 GetRemainJiebiaoCnt();
	//CYabiaoInfo* GetYabiaoInfo();
	INT32 BeginYabiao();
	//重新开始押镖检查，在数据库的信息读取到内存后，有可能还需要押镖，那么补上押镖
	VOID Check4ReBeginYabiao(); 
	//VOID EndYabiao();
	BOOL IsYabiao();



public:
    const static INT32 ms_nYabiaoFlagSucceed = 2;
    const static INT32 ms_nYabiaoFlagJiebiao = 0;
    const static INT32 ms_nYabiaoFlagBeJiebiao = 1;
    const static INT32 ms_nYabiaoProcessFlagStand = 0;
    const static INT32 ms_nYabiaoProcessFlagRunning = 1;
    const static INT32 ms_nYabiaoProcessFlagSuccess = 2;
private:
    BOOL IsYabiaoEnd();
    VOID ResetYabiaoInfo();

    //器魂和铜钱都可以用这个公式计算
    UINT64 GetJiebiaoEncourage( UINT16 wYabiaoLevel, UINT16 wJiebiaoLevel, UINT64 dwYabiaoEncourage );
private:
    DT_YABIAO_BASE_DATA m_stDT_YABIAO_BASE_DATA;
    CYabiaoBattleLogDeq m_deqYabiaoBattleLog;
    BOOL m_bYabiaoSucceed;

    //记录在自己被谁劫镖了
    vector<UINT32> m_vecJiebiaoPlayerID;


    INT32 m_nCacheJiebiaoRemainJiebiaoIndex; //剩余劫镖次数的索引
    //劫镖返回
    PKT_CLIGS_BEGIN_JIEBIAO_ACK5 m_stBeginJiebiaoAck5;
    PKT_CLIGS_BEGIN_JIEBIAO_ACK3 m_stBeginJiebiaoAck3;
	PKT_CLIGS_BEGIN_JIEBIAO_ACK4 m_stBeginJiebiaoAck4;
};
DECLARE_CLASS_POOL_MGR(CYabiaoInfo);

#endif