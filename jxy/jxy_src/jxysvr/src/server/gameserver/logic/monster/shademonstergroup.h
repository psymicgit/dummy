
#ifndef _SHADE_MONSTER_GROUP_H_
#define _SHADE_MONSTER_GROUP_H_

#include <sdhashmap.h>
#include <protocol/server/protocommondata.h>
#include <logic/battle/battleobj.h>
#include <logic/instance/instancepropmgr.h>
#include "shademonster.h"

using namespace std ;

typedef vector<UINT16> CVecMonsterID;
typedef CVecMonsterID::iterator CVecMonsterIDItr;

class CPlayer;

typedef HashMap<UINT32, CShadeMonster*> CID2ShadeMonsterMap;
typedef CID2ShadeMonsterMap::iterator CID2ShadeMonsterMapItr;

struct SShadeMonsterFormation
{
	UINT8 byFormation;
	CShadeMonster* poShadeMonster;
};

typedef vector<SShadeMonsterFormation> CVecMonsterFormation;
typedef CVecMonsterFormation::iterator CVecMonsterFormationItr;

class CShadeMonsterGroup: public CBattleObj
{
public:
	CShadeMonsterGroup();
	virtual ~CShadeMonsterGroup();

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
	BOOL Init(UINT16 wLevel, UINT8 byType, const vector<UINT16>& vecMonsterID);

	CShadeMonster* GetMainShadeMonster()
	{
		return m_poMainShadeMonster;
	}

private:
	CShadeMonster*		m_poMainShadeMonster;//主怪
	BOOL					m_bGenBattleAttribute; //怪物属性不会改变，只需用计算一次就可以了
	DT_BATTLE_OBJ_GROUP_DATA3 m_stBattleGroupData;//用于返回协议,因为怪物固定，则做变量保存
	
	CID2ShadeMonsterMap m_mapShadeMonster;

};


#endif

