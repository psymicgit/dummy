#ifndef _SEND_FLOWER_PROPMGR_H_
#define _SEND_FLOWER_PROPMGR_H_

#include <sdtype.h>
#include <sdhashmap.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <logic/base/basepropmgr.h>
#include <dll/sdu/sdobjectpool.h>

#include "friends.h"
#include "logic/base/jsonencourage.h"

using namespace SGDP;

struct tagDT_SEND_FLOWER_PROP_LIST_CLI;
typedef tagDT_SEND_FLOWER_PROP_LIST_CLI DT_SEND_FLOWER_PROP_LIST_CLI;

// 送鲜花配置
struct SSendFlowerProp
{
    UINT32 dwFlowerNum; // 鲜花数量
    UINT16 wSendMaxTimes; // 使用次数
    UINT8  byNeedVipLvl; // 需要的Vip等级
    UINT16 wCostKindID; // 花费的资源类型(1铜钱，2元宝)
    UINT32 dwCostKindIDNum; // 花费的资源数量
    UINT16 wGetKindID; // 获得的资源类型
    UINT32 dwGetKindIDNum; // 获得的资源数量
};


typedef map<UINT32, SSendFlowerProp> CSendFlowerPropMap; // key为鲜花的数量
typedef CSendFlowerPropMap::iterator CSendFlowerPropMapIter;

typedef struct _tagSendFlowerEncourage
{
    _tagSendFlowerEncourage()
    {
        dwFlowerNum = 0;
    }
    UINT32				dwFlowerNum;
    CJsonResItemVectorInfo	vecJsonEncourageItemInfo;
} SSendFlowerEncourage;

typedef vector<SSendFlowerEncourage> CSendFlowerPropVector; // key为鲜花的数量
typedef CSendFlowerPropVector::iterator CSendFlowerPropVectorItr;
typedef CSendFlowerPropVector::reverse_iterator CSendFlowerPropVectorRItr;



typedef struct _tagRecvFlowerEncourage
{
    _tagRecvFlowerEncourage()
    {
        wKindID = 0;
    }
    UINT32				wKindID;
    CJsonResItemVectorInfo	vecJsonEncourageItemInfo;
} SRecvFlowerEncourage;

typedef vector<SRecvFlowerEncourage> CRecvFlowerPropVector; // key为鲜花的数量
typedef CRecvFlowerPropVector::iterator CRecvFlowerPropVectorItr;


class CSendFlowerMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CSendFlowerMgr);

public:
    virtual EConfigModule GetConfigModuleType()
    {
        return ECM_SENDFLOWER;
    }

    BOOL Init();

    VOID UnInit();

public:
    CSendFlowerPropMap& GetSendFlowerPropMap()
    {
        return m_mapSendFlowerPropMap;
    }

	CRecvFlowerPropVector& GetCRecvFlowerPropVector()
	{
		return m_vecRecvFlowerProp;
	}

    // 获取送鲜花配置列表(for 打开送鲜花面板)
    UINT16 GetSendFlowerProp(DT_SEND_FLOWER_PROP_LIST_CLI& stSendFlowerPropList);

	SSendFlowerEncourage *GetSendFlowerEncourage(UINT32	dwFlowerNum);

	SSendFlowerEncourage *GetNextSendFlowerEncourage(UINT32 dwFlowerNum, UINT32 &dwNextFlowerNum);

	SRecvFlowerEncourage *GetRecvFlowerEncourage(UINT16	wKindID);

	SSendFlowerEncourage *GetMaxSendFlowerEncourage();

public:
    CSendFlowerMgr();
    virtual ~CSendFlowerMgr();

protected:
    BOOL LoadSendFlowerPropFromDB();

private:
    CSendFlowerPropMap	m_mapSendFlowerPropMap; // 送鲜花配置

    CSendFlowerPropVector m_vecSendFlowerProp;

    CRecvFlowerPropVector m_vecRecvFlowerProp;
};

#endif // 




