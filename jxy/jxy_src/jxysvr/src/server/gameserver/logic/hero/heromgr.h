
#ifndef _HEROMGR_H_
#define _HEROMGR_H_

#include "hero.h"
#include <sdsingleton.h>
#include <protogsdb.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>

using namespace SGDP;


typedef HashMap<UINT64, CHero*> CID2HeroMap;
typedef CID2HeroMap::iterator	CID2HeroMapItr;


typedef HashMap<UINT16, CHero*> CKind2HeroMap; //key为武将类型
typedef CKind2HeroMap::iterator CKind2HeroMapItr;

typedef map<UINT8, CHero*> CFormationIdx2HeroMap; //key为阵型位置
typedef CFormationIdx2HeroMap::iterator CFormationIdx2HeroMapItr;

#define UNDEFINE_UPGRADE_EXPERIENCE 0xFFFFFFFF //未配置下一升级等级时下一级的升级经验


/**
* @brief 武将管理类
* 玩家和怪物共用，怪物Hero不做map查找
*/
class CHeroMgr
{
public:
    DECLARE_SINGLETON(CHeroMgr)

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

public:

	CHero*		CreateHero(UINT32 dwPlayerID, UINT16 wKindID);

	VOID		RemoveHero(UINT32 dwPlayerID, UINT16 wKindID);

    CHero*		FindHero(UINT32 dwPlayerID, UINT16 wKindID);
	
    // 新旧英雄互换（用于伙伴进阶）
    BOOL		SwapHero(UINT32 dwPlayerID, UINT16 wHeroID1, UINT16 wHeroID2);

protected:
    CHeroMgr();
    ~CHeroMgr();


private:
    CID2HeroMap						m_mapID2Hero;//只有玩家的Hero信息   
	CSDObjectPool<CHero, CSDMutex>	m_oHeroShmemObjMgr;
};



#endif //#ifndef _ROLEMGR_H_

