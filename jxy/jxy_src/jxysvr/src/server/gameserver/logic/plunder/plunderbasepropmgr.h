
#ifndef _PLUNDERBASEPROPMGR_H_
#define _PLUNDERBASEPROPMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>


/**
 * @brief
 */
typedef struct tagPlunderBaseProp
{
    UINT16	wPlunderOpenLevel;		///< 掠夺开放等级
	UINT32	dwPlunderRobotStartID;	///< 掠夺机器人起始ID
    UINT16	wPlunderLowerLevelItemID;	///< 掠夺低自己5级以上的道具
    UINT16	wPlunderFreeTimesPerDay;	///< 每天可免费掠夺次数
    //UINT16	wPlunderMaxTimesPerDay;	///< 每天掠夺最大次数(免费+收费)
    UINT32	dwCaptureWorkSecond;		///< 俘虏工作时间
    UINT8	byBleedPerGoldClrMin;		///< 榨干每元宝所清除分钟
    UINT8	byMaxCaptureNum;			///< 最大俘虏个数
    UINT32	dwCaptureKeepSecond;		///< 俘虏拥有保持时间(单位秒，过了该时间则自动释放)
} SPlunderBaseProp;

//等级每秒钟生产的铜币个数MAP表
typedef std::map<UINT16, UINT32> CWorkCoinMap; 
typedef CWorkCoinMap::iterator CWorkCoinMapItr;

//等级每秒钟生产的器魂个数MAP表
typedef std::map<UINT16, UINT32> CWorkScienceMap; 
typedef CWorkScienceMap::iterator CWorkScienceMapItr;

/**
 * @brief 属性管理器
 *
 * 管理相关的配置属性，继承自CPropContainer\n
 * 提供了根据ID和名称查找属性的功能\n
 */
class CPlunderBasePropMgr : public CBasePopMgr
{
public:
    typedef std::map<UINT16, SPlunderBaseProp> CPlunderBasePropPriMap;
    typedef std::map<UINT16, SPlunderBaseProp>::iterator CPlunderBasePropPriMapItr;

    DECLARE_SINGLETON_PROPMGR(CPlunderBasePropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_PLUNDER_BASE ; }

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

    SPlunderBaseProp& GetPlunderBaseProp()
    {
        return m_stPlunderBaseProp;
    }

    UINT16 GetPlunderLowerLevelItemID()
    {
        return m_stPlunderBaseProp.wPlunderLowerLevelItemID;
    }

	UINT64 GetWorkCoin(UINT16 wLevel)
	{
		CWorkCoinMapItr itr = m_mapWorkCoin.find(wLevel);
		if (itr != m_mapWorkCoin.end())
		{
			return itr->second;
		}
		return 0;
	}

	UINT32 GetWorkScience(UINT16 wLevel)
	{
		CWorkScienceMapItr itr = m_mapWorkScience.find(wLevel);
		if (itr != m_mapWorkScience.end())
		{
			return itr->second;
		}
		return 0;
	}
public:
    CPlunderBasePropMgr();
    virtual ~CPlunderBasePropMgr();
protected:

    /**
     * @brief 从数据源获取属性数据
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL LoadFromDB();

private:
    SPlunderBaseProp	m_stPlunderBaseProp;

	//等级每秒钟生产的铜币个数MAP表
	CWorkCoinMap		m_mapWorkCoin; 
	//等级每秒钟生产的器魂个数MAP表
	CWorkScienceMap		m_mapWorkScience;
};

#endif // #ifndef _PLUNDERBASEPROPMGR_H_
