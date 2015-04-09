
#ifndef _MONSTERUPGRADEPROPMGR_H_
#define _MONSTERUPGRADEPROPMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>

/// 怪物名称名称最大长度
#define NAME_LEN_MAX	32


/**
 * @brief 
 */
typedef struct tagMonsterUpgradeProp
{
	UINT16	wLevel;		///< 等级(只用于显示)
	UINT8	byType;		///< 类型(普通1/boss2/精英3)
	//UINT8   byCareerID; /// 职业ID
	//UINT32	dwHP;		///< 生命	
	//UINT32	dwAttack;		///< 攻击力
	UINT32	dwEncourageExp;//奖励经验
	UINT32	dwEncourageCoin;//奖励铜币	
	UINT32	dwEncourageStory;//奖励阅历，未开放科技不给
	UINT32	dwEncourageJingJie;//奖励境界点，未开放境界不给，而且是首次才爆
}SMonsterUpgradeProp;

/**
 * @brief 属性管理器
 *
 * 管理相关的配置属性，继承自CPropContainer\n
 * 提供了根据ID和名称查找属性的功能\n
 */
class CMonsterUpgradePropMgr : public CBasePopMgr
{
public:
    typedef std::map<UINT32, SMonsterUpgradeProp> CMonsterUpgradePropMap;
    typedef std::map<UINT32, SMonsterUpgradeProp>::iterator CMonsterUpgradePropMapItr;

    DECLARE_SINGLETON_PROPMGR(CMonsterUpgradePropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_MONSTER_UPGRADE ; }

    /**
     * @brief 初始化属性管理器
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL Init();

    /**
     * @brief 结束属性管理器
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    VOID UnInit();


	SMonsterUpgradeProp* GetMonsterUpgradeProp(UINT16 wLevel, UINT8	byType);

public:
    CMonsterUpgradePropMgr();
    virtual ~CMonsterUpgradePropMgr();

protected:

    /**
     * @brief 从数据源获取属性数据
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
	BOOL LoadFromDB();

private:
	CMonsterUpgradePropMap m_mapMonsterUpgradeProp;
};

#endif // #ifndef _MONSTERPROPMGR_H_
