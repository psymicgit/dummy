
#ifndef _PLAYERGROUP_H_
#define _PLAYERGROUP_H_

#include <protocol/server/protocommondata.h>
#include <logic/battle/battleobj.h>
#include <logic/hero/hero.h>


using namespace std ;

class CPlayer;

class CPlayerGroup: public CBattleObj
{
public:
    CPlayerGroup();
    virtual ~CPlayerGroup();

public:

    virtual VOID StartBattle(CBattleObj* poEnemy);//

    virtual EBattleObjType GetBattleObjType()
    {
        return EBOT_PLAYERGROUP;
    }
    virtual VOID EndBattle(); //每次战斗后(清除战斗过程中的产出 buff等)

	virtual VOID GetDT_BATTLE_OBJ_GROUP_DATA(DT_BATTLE_OBJ_GROUP_DATA3& stDT_BATTLE_OBJ_GROUP_DATA);//赋值战斗组战斗者属性

    virtual VOID GetDT_BATTLE_OBJ_GROUP_AFTER_DATA(DT_BATTLE_OBJ_GROUP_HP& stDT_BATTLE_OBJ_GROUP_AFTER_DATA);//赋值战斗组战斗者属性

public:
    //初始化相关对象
    BOOL Init(vector<CHero*> vecHero);

protected:

private:
    BOOL						m_bGenBattleAttribute; //怪物属性不会改变，只需用计算一次就可以了
    DT_BATTLE_OBJ_GROUP_DATA3	m_stBattleGroupData;//用于返回协议,因为怪物固定，则做变量保存
///////////////////////////////////
public:


};


#endif

