#ifndef _FACTION_BATTLELOG_H
#define _FACTION_BATTLELOG_H

#include <protocol/server/protocommondata.h>
#include <protocol/server/protocligs.h>
#include <logic/base/baseobj.h>
#include <deque>
class CPlayer;
class CFaction;
class CFactionBattleLog : public CBaseObj
{
public:
    typedef deque<DT_PLAYER_FACTION_BATTLELOG_DATA> CFactionBattleDeq;
    CFactionBattleLog()
    {
        m_byUnReadLogNum = 0;
        memset(&m_stFactionBattleLog, 0, sizeof(DT_PLAYER_FACTION_BATTLELOG_DATA_LIST));
        m_deqFactionBattleLog.clear();
    }
    ~CFactionBattleLog()
    {
        m_deqFactionBattleLog.clear();
    }
public:
    // 初始化相关对象
    BOOL Init(DT_PLAYER_FACTION_BATTLELOG_DATA_LIST& stFactionBattleLog, CPlayer* poOwner);
    DT_PLAYER_FACTION_BATTLELOG_DATA_LIST& GetDT_PLAYER_FACTION_BATTLELOG_DATA_LIST();

    VOID AddFactionBattleLog(DT_PLAYER_FACTION_BATTLELOG_DATA& stFactionBattleLog);

    UINT8 GetUnReadLogNum()
    {
        return m_byUnReadLogNum;
    }
    VOID GetBattleLogByCli(UINT8& byLogNum, DT_PLAYER_FACTION_BATTLELOG_DATA astPlayerFactionLogList[MAX_FACTION_BATTLE_LOG_NUM]);
	CFaction* GetFaction();
private:
    DT_PLAYER_FACTION_BATTLELOG_DATA_LIST m_stFactionBattleLog;
    CFactionBattleDeq m_deqFactionBattleLog;
    UINT8 m_byUnReadLogNum;
};
#endif