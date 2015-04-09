
#ifndef _MONEYTREEPROPBASEMGR_H_
#define _MONEYTREEPROPBASEMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>



/**
 * @brief
 */
//typedef struct tagMoneyTreeBaseProp
//{
//    //UINT16	wRockNumPerDay;		///< 每天可摇次数
//    //UINT16	wBaseGold;		///< 需要的基础元宝
//    //UINT16	wIncGold;		///< 每多摇一次增加的元宝
//    //UINT16	wMaxGold;		///< 最大所需元宝
//} SMoneyTreeBaseProp;

/**
 * @brief 属性管理器
 *
 * 管理相关的配置属性，继承自CPropContainer\n
 * 提供了根据ID和名称查找属性的功能\n
 */
class CMoneyTreeBasePropMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CMoneyTreeBasePropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_MONEY_TREE_BASE ; }
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


    //SMoneyTreeBaseProp& GetMoneyTreeBaseProp()
    //{
    //    return m_stMoneyTreeBaseProp;
    //}

    UINT32	GetProduceCoin(UINT32 dwPlayerLevel, UINT8 byMoodValue);

    UINT32	GetRockNeedGold(CPlayer* poPlayer, UINT16 wCurRockNum);

public:
    CMoneyTreeBasePropMgr();
    virtual ~CMoneyTreeBasePropMgr();

protected:

    /**
     * @brief 从数据源获取属性数据
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL LoadFromDB();

//private:
//    SMoneyTreeBaseProp	m_stMoneyTreeBaseProp;
};

#endif // #ifndef _MONEYTREEPROPBASEMGR_H_
