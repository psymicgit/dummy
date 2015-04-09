
#ifndef _SHADE_MONSTER_MGR_H_
#define _SHADE_MONSTER_MGR_H_

#include "shademonster.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <protocol/server/protocommondata.h>
#include <map>
using std::map;

using namespace SGDP;

typedef multimap<UINT32, const SMonsterProp*> CMap32ShadeMonster;	//等级为KEY
typedef CMap32ShadeMonster::iterator CMap32ShadeMonsterItr;

typedef vector<const SMonsterProp*> CVecPShadeMonster;
typedef CVecPShadeMonster::iterator	CVecPShadeMonsterItr;

class CShadeMonsterGroup;
/**
* @brief 怪物管理类
*/
class CShadeMonsterMgr
{
    friend class CShadeMonsterGroup;
public:
    DECLARE_SINGLETON(CShadeMonsterMgr)

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

	VOID				RemoveShadeMonsterGroup(CShadeMonsterGroup* poMonsterGroup);
	CShadeMonsterGroup* CreateShadeMonsterGroup(UINT16 wLevel, UINT8 byType, const vector<UINT16>& vecMonsterID);

	UINT8 GetShadeMonsterVec(UINT16 wLevel, UINT8 byType, CVecPShadeMonster& vecMonster);
	const SMonsterProp* GetShadeMonsterPropByAttrLevelType(UINT16 wLevel, UINT8 byType);
	const SMonsterProp* GetShadeMonsterPropByDefendLevelType(UINT16 wLevel, UINT8 byType);
	const SMonsterProp* GetShadeMonsterPropByLevelType(UINT16 wLevel, UINT8 byType);
	UINT32 GetPowerByProp(const SMonsterProp* poProp);
protected:
	CShadeMonster*		CreateShadeMonster();
	VOID				RemoveAllShadeMonster();
	VOID RemoveShadeMonster(CShadeMonster* poMonster);
	CShadeMonsterGroup* CreateShadeMonsterGroup();
    CShadeMonsterMgr();
    ~CShadeMonsterMgr();


private:
	CSDObjectPool<CShadeMonster, CSDMutex> m_oShadeMonsterPool;
    CSDObjectPool<CShadeMonsterGroup, CSDMutex>	m_oShadeMonsterGroupPool;
	CMap32ShadeMonster m_mapShadeMonster;


	//以等级为KEY 攻击
	CMap32ShadeMonster	m_mapShadeMonsterByBlueAttr;
	CMap32ShadeMonster	m_mapShadeMonsterByPurpleAttr;
	CMap32ShadeMonster	m_mapShadeMonsterByRedAttr;
	CMap32ShadeMonster	m_mapShadeMonsterByOrangeAttr;
	//以等级为KEY 防守
	CMap32ShadeMonster	m_mapShadeMonsterByBlueDefend;
	CMap32ShadeMonster	m_mapShadeMonsterByPurpleDefend;
	CMap32ShadeMonster	m_mapShadeMonsterByRedDefend;
	CMap32ShadeMonster	m_mapShadeMonsterByOrangeDefend;
};




#endif //#ifndef

