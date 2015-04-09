
#ifndef _BAGPROPMGR_H_
#define _BAGPROPMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>

/**
 * @brief 
 */
typedef struct tagBagProp
{
	UINT16	wMaxOpenNum;		///< 最大打开格子数
	UINT16	wInitOpenNum;		///< 初始化打开格子数
	UINT32	dwBaseOpenCoin;		///< 初始打开格子需要金币，二选1
	UINT32	dwOpenCoinIncrease;		///< 没多打开1格子递增金币
	UINT32	dwBaseOpenGold;		///< 初始打开格子需要元宝，二选1
	UINT32	dwOpenGoldIncrease;		///< 没多打开1格子递增元宝
	UINT16  wTotalBagNum;		//新版本最大背包数
	string strGiftName;			//新版本补偿礼包名
	string strGiftDesc;			//新版本补偿描述

	tagBagProp()
	{
		wMaxOpenNum = 0;
		wInitOpenNum = 0;
		dwBaseOpenCoin = 0;
		dwBaseOpenGold = 0;
		dwOpenCoinIncrease = 0;
		dwOpenGoldIncrease = 0;
		wTotalBagNum = 0;
		strGiftDesc = "";
		strGiftName = "";
	}
}SBagProp;


class CBagPropMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CBagPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_BAG ; }

    BOOL Init();

    VOID UnInit();

    SBagProp& GetBagProp() { return  m_stBagProp; }

	UINT32 GetEnourageGold(UINT8 byOpenNum);
public:
    CBagPropMgr();
    virtual ~CBagPropMgr();

	BOOL LoadFromDB();

private:
	SBagProp m_stBagProp;
};

#endif // #ifndef _BAGPROPMGR_H_
