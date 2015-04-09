
#ifndef _GODWEAPONBASEPROPMGR_H_
#define _GODWEAPONBASEPROPMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>


/**
 * @brief 属性管理器
 *
 * 管理相关的配置属性，继承自CPropContainer\n
 * 提供了根据ID和名称查找属性的功能\n
 */
class CGodweaponBasePropMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CGodweaponBasePropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_GODWEAPON_BASE ; }
	

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

	UINT32  GetActiveCoin() { return m_dwActiveCoin; }
	UINT8	GetGodWeaponRecoverRate() { return m_byGodWeaponRecoverRate; }
	UINT16	GetUpgradeQualityItemID() { return m_wUpgradeQualityItemID; }
	UINT8	GetQualityUpdateNum() { return m_byQualityUpdateNum; }

public:
    CGodweaponBasePropMgr();
    virtual ~CGodweaponBasePropMgr();

protected:

    /**
     * @brief 从数据源获取属性数据
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
	BOOL LoadFromDB();

private:
	UINT32	m_dwActiveCoin;	///< 激活神器所需铜币
	UINT8	m_byGodWeaponRecoverRate;//神器器魂回收百分比比例
	UINT16	m_wUpgradeQualityItemID;///< 进阶符对应的具体道具ID
	UINT8	m_byQualityUpdateNum;	///< 每个品质总共可升级次数,从0级开始
};

#endif // #ifndef _PLUNDERBASEPROPMGR_H_
