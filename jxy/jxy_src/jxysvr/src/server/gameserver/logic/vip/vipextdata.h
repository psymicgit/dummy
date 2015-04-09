#ifndef VIP_EXT_DATA_H
#define VIP_EXT_DATA_H
#include <logic/base/baseobj.h>
#include <protocol/server/protocommondata.h>
#include <common/server/utility.h>
// #include "logic/vip/vipgiftpropmgr.h"
// #include "logic/base/jsonencourage.h"
class CVipExtData: public CBaseObj
{
public:
	CVipExtData();
	~CVipExtData();
public:
	//初始化相关对象
	BOOL Init(DT_VIP_EXT_DATA &stDT_VIP_EXT_DATA, CPlayer* poOwner);
	DT_VIP_EXT_DATA &GetDT_VIP_EXT_DATA();
	VOID Check4ResetExperience();
	VOID Check4ResetEliteInstance();

	UINT16		GetRemainGoldExperienceNum();
	UINT16		GetRemainResetEliteInstanceNum( UINT8 byIndex );
	UINT16		GetResetEliteInstanceNum( UINT8 byIndex );
	BOOL			ResetEliteInstance( UINT8 byIndex ); //城镇副本从1起始
	UINT16		GetResetEliteInstanceNum();
// public:
// 	//初始化获得VIP礼包的数据
// 	VOID InitGift();
// 
// 	//获得礼包界面打开时的初始化数据
// 	UINT8 GetOpenData(UINT32 &dwMaxVipLevel,UINT32 &dwCurVipLevel,DT_VIP_GIFT_DATA astVipGetNum[MAX_VIP_LEVEL]);
// 
// 	//获得奖励请求，供外面接口调用
// 	UINT8 GetGiftRes(const UINT8 &byVipLevel,TCHAR aszSeriesNo[MAX_SERIES_NO_NUM],DT_RES_ITEM_DATA &stResItemData,DT_RSYNC_RES_ITEM_DATA &stRsyncResItemData,UINT8 &byIndex, UINT16 &wElseCount);
// 
// 	//判断是否礼包可领取
// 	BOOL HaveVipGift();
// protected:
// 	//获得奖励并返回错误码
// 	UINT8 GetGift(TCHAR aszSeriesNo[MAX_SERIES_NO_NUM],SVipGiftData *pVipGiftItem,DT_RES_ITEM_DATA &stResItemData,DT_RSYNC_RES_ITEM_DATA &stRsyncResItemData,const UINT8 &byIndex,UINT16 &wElseCount,const UINT8 &byVipLevel);
// 
// 	//验证领取礼包所需要的资源是否足够
// 	BOOL ChkRequireSource(UINT8 &byErrorCode,TCHAR aszSeriesNo[MAX_SERIES_NO_NUM],const SVipGiftData *pVipGiftItem);
// 
// 	//验证当前的操作是否属于今天
// 	BOOL ChkGiftDataIsToday();
// 
// 	//扣除所需要消耗的资源
// 	VOID DecRequireSource(TCHAR aszSeriesNo[MAX_SERIES_NO_NUM],const SVipGiftData *pVipGiftItem,DT_RSYNC_RES_ITEM_DATA &stRsyncResItemData);
private:
	DT_VIP_EXT_DATA		m_stDT_VIP_EXT_DATA;
};
DECLARE_CLASS_POOL_MGR(CVipExtData);
#endif