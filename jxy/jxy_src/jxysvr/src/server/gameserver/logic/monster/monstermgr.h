
#ifndef _MONSTERMGR_H_
#define _MONSTERMGR_H_

#include "monster.h"
#include "shademonster.h"
#include "monstergroup.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <logic/instance/instancepropmgr.h>
#include <protocol/server/protocommondata.h>


using namespace SGDP;


typedef HashMap<UINT64, CMonsterGroup*> CID2MonsterGroupMap; //key为wInstanceIdx(wSceneIdx+byTownIdx+byInstanceIdx)+wMonsterIdx(从0开始)
typedef CID2MonsterGroupMap::iterator	CID2MonsterGroupMapItr;


typedef HashMap<UINT64, DT_MONSTER_DATA_LIST> CInstanceID2MonsterGroupMap; //key为wSceneIdx+byTownIdx+byInstanceIdx
typedef CInstanceID2MonsterGroupMap::iterator CInstanceID2MonsterGroupMapItr;


class CMonsterGroup;
/**
* @brief 怪物管理类
*/
class CMonsterMgr
{
    friend class CMonsterGroup;
public:
    DECLARE_SINGLETON(CMonsterMgr)

    /**
    * @brief
    * 初始化函数,读取配置文件等
    * @return 执行结果，成功返回TRUE，失败返回FALSE
    */
    BOOL Init();

    /**
    * @brief
    * 卸载函数
    */
    VOID UnInit();

    /**
    * @brief
    * 查找怪物，在管理器内查找(每个怪物),若没有则创建
    * @param wInstanceIdx   : 副本序号
    * @param wMonsterIdx	: 怪物序号
    * @return CMonster*     : 查找到的怪物
    */
    CMonsterGroup*		FindMonsterGroup(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT16 wMonsterIdx);
	/**
    * @brief
    * 查找怪物，在管理器内查找(每个怪物),若没有则创建
    * @param vector<SSingleMonsterProp>   : 怪物ID组
    * @return CMonster*     : 查找到的怪物
    */
	CMonsterGroup*		FindMonsterGroup(vector<SSingleMonsterProp> vecMonster);
    /**
    * @brief
    * 获取副本中的怪物信息，没有则返回NULL
    * @return VOID
    */
    DT_MONSTER_DATA_LIST* GetInstanceMonster(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx);

    CMonster*			CreateMonster();
    VOID					RemoveMonster(CMonster* poMonster);
    VOID					RemoveAllMonster();
	
protected:
    CMonsterMgr();
    ~CMonsterMgr();
    BOOL				CreateMonsterGroup(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx);
    CMonsterGroup*	CreateMonsterGroup(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT16 wMonsterIdx, DT_MONSTER_DATA_LIST& stDT_MONSTER_DATA_LIST);
    VOID				RemoveMonsterGroup(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT16 wMonsterIdx);

    UINT64			GetMonsterKey(UINT16 wSceneIdx, UINT16 byTownIdx, UINT16 byInstanceIdx, UINT16 wMonsterIdx);
    UINT64			GetInstanceKey(UINT16 wSceneIdx, UINT16 byTownIdx, UINT16 byInstanceIdx);
	UINT64			GetShadeMonsterKey( UINT8 byType, UINT32 dwPlayerID);
	UINT64			GetMonsterKey(vector<SSingleMonsterProp> vecMonster);

	//按怪物ID创建怪物组
	CMonsterGroup*	CreateMonsterGroup(vector<SSingleMonsterProp> vecMonster);

private:
    CID2MonsterGroupMap					m_mapID2MonsterGroup;
    CInstanceID2MonsterGroupMap			m_mapInstanceID2MonsterGroup;

    CSDObjectPool<CMonster, CSDMutex>		m_oMonsterPool;
    CSDObjectPool<CMonsterGroup, CSDMutex>	m_oMonsterGroupPool;
    static UINT32						m_dwMonsterIDGen;//用于生成内存中的ID
    static UINT32						m_dwMonsterGroupIDGen;//用于生成内存中的ID
};




#endif //#ifndef

