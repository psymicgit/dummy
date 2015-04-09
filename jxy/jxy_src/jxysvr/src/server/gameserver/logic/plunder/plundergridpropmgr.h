
#ifndef _PLUNDERGRIDPROPMGR_H_
#define _PLUNDERGRIDPROPMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sdhashmap.h>
#include <logic/base/basepropmgr.h>

typedef map<UINT16, UINT16> CGridUnlockPropMap;  //HashMap的lower_bound有问题
typedef CGridUnlockPropMap::iterator CGridUnlockPropMapItr;

/**
 * @brief 属性管理器
 *
 * 管理相关的配置属性，继承自CPropContainer\n
 * 提供了根据ID和名称查找属性的功能\n
 */
class CPlunderGridPropMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CPlunderGridPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_PLUNDER_GRID ; }

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

    UINT16	GetUnlockLevel(UINT8 byGridIdx);
    UINT8	GetUnlockNum(UINT16 wLevel);
    VOID	GetUnlockGridLevel(UINT16 awUnlockGridLevel[MAX_CAPTURE_NUM])
    {
        memcpy(awUnlockGridLevel, m_awUnlockGridLevel, sizeof(m_awUnlockGridLevel));
    }

public:
    CPlunderGridPropMgr();
    virtual ~CPlunderGridPropMgr();

protected:

    /**
     * @brief 从数据源获取属性数据
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL LoadFromDB();

private:
    CGridUnlockPropMap	m_mapGridIdx2LevelProp;
    CGridUnlockPropMap	m_mapLevel2GridIdxProp;
    UINT8				m_byMaxCaptureNum;
    UINT16				m_wMaxPlayerLevel;
	UINT16				m_wMaxGridUnlockLevel; //最后1个格子解锁等级
    UINT16				m_awUnlockGridLevel[MAX_CAPTURE_NUM];
};

#endif // #ifndef _PLUNDERGRIDPROPMGR_H_
