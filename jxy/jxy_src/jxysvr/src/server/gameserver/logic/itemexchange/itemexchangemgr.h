
#ifndef _ITEMEXCHANGEMGR_H_
#define _ITEMEXCHANGEMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>
#include <protocol/server/protocligs.h>

class CPlayer;

/**
 * @brief
 */
struct SItemExchangeProp
{
	UINT16	wExchangeIdx;		//兑换项
	TCHAR	aszTitle[MAX_COMMON_TXT_NUM];		//标题
	TCHAR	aszDesc[MAX_COMMON_TXT_NUM];		//描述
	UINT64	qwStartTime;		//开放时间
	UINT64	qwEndTime;		//时间时间
	UINT16	wExchangeItemKindID;//可兑换的道具ID
	UINT16	wExchangeItemNum;	//可兑换的道具数量
	UINT16	awNeedItemKindID[ITEM_EXCHANGE_ITEM_NUM];//所需道具ID，0为无效
	UINT16	awNeedItemNum[ITEM_EXCHANGE_ITEM_NUM];//0为无效
};


typedef std::map<UINT16, SItemExchangeProp> CItemExchangePropMap; 
typedef CItemExchangePropMap::iterator CItemExchangePropMapItr;

/**
 * @brief 属性管理器
 *
 * 管理相关的配置属性，继承自CPropContainer\n
 * 提供了根据ID和名称查找属性的功能\n
 */
class CItemExchangeMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CItemExchangeMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_ITEMEXCHANGE ; }
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

public:
	VOID OpenItemExchange(CPlayer* poPlayer, PKT_CLIGS_OPEN_ITEM_EXCHANGE_ACK& stAck);
	VOID ItemExchange(CPlayer* poPlayer, UINT16 wExchangeIdx, PKT_CLIGS_ITEM_EXCHANGE_ACK& stAck);
	BOOL HaveExchangeActivity(CPlayer* poPlayer);//是否有兑换活动

protected:
	VOID GetItemExchange(CPlayer* poPlayer, UINT8& byExchangeNum, ITEM_EXCHANGE_DATA astExchangeInfo[ITEM_EXCHANGE_ITEM_NUM]);

public:
    CItemExchangeMgr();
    virtual ~CItemExchangeMgr();

private:

    /**
     * @brief 从数据源获取属性数据
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
    BOOL LoadFromDB();

private:
    CItemExchangePropMap	m_mapItemExchangeProp;
};

#endif // #ifndef _MONEYTREEPROPMGR_H_
