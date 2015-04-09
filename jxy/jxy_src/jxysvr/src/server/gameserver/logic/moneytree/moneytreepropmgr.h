
#ifndef _MONEYTREEPROPMGR_H_
#define _MONEYTREEPROPMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>


/**
 * @brief
 */
typedef struct tagMoneyTreeProp
{
	tagMoneyTreeProp()
	{
		Init();
	}
	void Init()
	{
		memset(this, 0x00, sizeof(tagMoneyTreeProp));
	}

    //UINT16	wRockNumPerDay;		///< 每天可摇次数
    //UINT16	wBaseGold;		///< 需要的基础元宝
    //UINT16	wIncGold;		///< 每多摇一次增加的元宝
    //UINT16	wMaxGold;		///< 最大所需元宝
	
	UINT16	wMoodValue;			///< 心情值
    UINT16	wIncCoinRate;		///< 该心情值增加的铜币百分比
    UINT8	byGetMoodRate;		///< 每次摇钱获得心情值的概率
    UINT16	wMoodKeepTime;		///< 该心情值保持时间(单位秒)
} SMoneyTreeProp;


typedef std::map<UINT16, SMoneyTreeProp> CMoneyTreePropMap; 
typedef CMoneyTreePropMap::iterator CMoneyTreePropMapItr;

/**
 * @brief 属性管理器
 *
 * 管理相关的配置属性，继承自CPropContainer\n
 * 提供了根据ID和名称查找属性的功能\n
 */
class CMoneyTreePropMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CMoneyTreePropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_MONEY_TREE ; }
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


	UINT8 GetMoneyTreeMoodRate(UINT16 wMoodValue); //摇钱获得心情值的概率

	UINT16 GetMoneyTreeIncMoneyRate(UINT16 wMoodValue); //获得该心情值增加的铜币百分比

	UINT16 GetMoodKeepTime()
	{
		CMoneyTreePropMapItr itr = m_MoneyTreePropMap.begin();
		if (itr != m_MoneyTreePropMap.end())
		{
			return itr->second.wMoodKeepTime;
		}
		return 600;
	}

	UINT16 GetMoodKeepTime(UINT16	wMoodValue)
	{
		CMoneyTreePropMapItr itr = m_MoneyTreePropMap.find(wMoodValue);
		if (itr != m_MoneyTreePropMap.end())
		{
			return itr->second.wMoodKeepTime;
		}
		return 0;
	}
	BOOL IsExist(UINT16	wMoodValue)
	{
		CMoneyTreePropMapItr itr = m_MoneyTreePropMap.find(wMoodValue);
		if (itr != m_MoneyTreePropMap.end())
		{
			return TRUE;
		}
		return FALSE;
	}


public:
    CMoneyTreePropMgr();
    virtual ~CMoneyTreePropMgr();

protected:

    /**
     * @brief 从数据源获取属性数据
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL LoadFromDB();

private:
    CMoneyTreePropMap	m_MoneyTreePropMap;
};

#endif // #ifndef _MONEYTREEPROPMGR_H_
