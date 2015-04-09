
#ifndef _MONSTERGROUP_H_
#define _MONSTERGROUP_H_

#include <sdhashmap.h>
#include <protocol/server/protocommondata.h>
#include <logic/battle/battleobj.h>
#include <logic/instance/instancepropmgr.h>
#include "monster.h"

using namespace std ;

typedef vector<UINT16> CVecMonsterID;
typedef CVecMonsterID::iterator CVecMonsterIDItr;

class CPlayer;

typedef HashMap<UINT32, CMonster*> CID2MonsterMap;
typedef CID2MonsterMap::iterator CID2MonsterMapItr;

//typedef HashMap<UINT16, UINT32> CLevel2ExperienceMap;	//key为玩家等级，value为经验值
//typedef CLevel2ExperienceMap::iterator CLevel2ExperienceMapItr;

class CMonsterGroup: public CBattleObj
{
public:
    CMonsterGroup();
    virtual ~CMonsterGroup();

public:
    virtual VOID StartBattle(CBattleObj* poEnemy);//
    virtual EBattleObjType GetBattleObjType()
    {
        return EBOT_MONSTERGROUP;
    }
    virtual VOID EndBattle(); //每次战斗后(清除战斗过程中的产出 buff等)
	virtual VOID GetDT_BATTLE_OBJ_GROUP_DATA(DT_BATTLE_OBJ_GROUP_DATA3& stDT_BATTLE_OBJ_GROUP_DATA);//赋值战斗组战斗者属性
    virtual VOID GetDT_BATTLE_OBJ_GROUP_AFTER_DATA(DT_BATTLE_OBJ_GROUP_HP& stDT_BATTLE_OBJ_GROUP_AFTER_DATA);//赋值战斗组战斗者属性

public:
    //初始化相关对象
    BOOL Init(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT16 wMonsterIdx);

    BOOL Init(vector<SSingleMonsterProp> vecMonster);

    //DT_BATTLE_OBJ_GROUP_DATA& GetBattleGroupData() { return m_stBattleGroupData; }; //用于返回协议
    CMonster* GetMainMonster()
    {
        return m_poMainMonster;
    }

    VOID Encourage(CPlayer* poPlayer, BOOL bFirst = FALSE, UINT8 byRunTimes = 1);//对战胜它的玩家奖励

protected:

private:
    UINT16				m_wSceneIdx;
    UINT8				m_byTownIdx;
    UINT8				m_byInstanceIdx;
    UINT16				m_wMonsterIdx;

    CID2MonsterMap		m_mapID2Monster;
    CMonster*			m_poMainMonster;//主怪
    BOOL					m_bGenBattleAttribute; //怪物属性不会改变，只需用计算一次就可以了
    DT_BATTLE_OBJ_GROUP_DATA3 m_stBattleGroupData;//用于返回协议,因为怪物固定，则做变量保存
    UINT32				m_dwExperience; //用于快速给经验，不需每次运算
    UINT32				m_dwEncourageCoin;//奖励铜币
    UINT32				m_dwEncourageStory;//奖励阅历，科技馆已开放才有
    UINT32				m_dwEncourageJingJie; //奖励境界点，未开放境界不给，而且是首次才爆
public:
    VOID			SetMemID(UINT32 dwMemID)
    {
        m_dwMemID = dwMemID;
    }
    UINT32		GetMemID()
    {
        return m_dwMemID;
    }
private:
    UINT32		m_dwMemID;//内存ID，便于管理器map管理

///////////////////////////////////
public:


};


#endif

