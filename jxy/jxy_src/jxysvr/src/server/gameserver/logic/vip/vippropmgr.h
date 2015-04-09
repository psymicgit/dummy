
#ifndef _VIPMGR_H_
#define _VIPMGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sdhashmap.h>
#include <map>
#include <vector>
#include <protocol/server/protocligs.h>
#include <logic/base/basepropmgr.h>

#include <sdobjectpool.h>
#include <sdmutex.h>

#include "vipextdata.h"
#include "logic/base/jsonencourage.h"
using namespace SGDP;
using namespace std;


//vip开放的功能配置
struct SVipOpenFuncProp
{
    UINT8	byOpenFuncKind; //开放的功能类型
    UINT8	byNeedVipLevel;	//开放所需的vip等级
};
typedef HashMap<UINT8, SVipOpenFuncProp> CVipOpenFuncPropMap;
typedef CVipOpenFuncPropMap::iterator CVipOpenFuncPropMapItr;
typedef vector<UINT8> CVipFuncVec;
typedef HashMap<UINT8, CVipFuncVec> CVipLevel2OpenFuncMap;
typedef CVipLevel2OpenFuncMap::iterator CVipLevel2OpenFuncMapItr;

//vip增加的次数配置
struct SVipIncNumProp
{
    UINT8	byFuncKind; //功能类型
    UINT8	byVipLevel;	//vip等级
    UINT16	wIncNum;	//增加的次数
};
typedef HashMap<UINT16, SVipIncNumProp> CVipIncNumPropMap;
typedef CVipIncNumPropMap::iterator CVipIncNumPropMapItr;
typedef vector<SVipIncNumProp> CVipIncNumVec;
typedef HashMap<UINT8, CVipIncNumVec> CVipLevel2IncNumMap;
typedef CVipLevel2IncNumMap::iterator CVipLevel2IncNumMapItr;

//vip等级配置配置
struct SVipLevelProp
{
    UINT8	byVipLevel;	//vip等级
    UINT32	dwNeedRechargeGold; //需要充值元宝数

    UINT8            byExtNumFuncNum; //当前等级提升功能次数的功能数
    DT_VIP_NUM_FUNC_DATA astVipNumFuncInfo[MAX_VIP_FUNC]; //当前等级提升功能次数的功能信息

    SVipLevelProp()
    {
        memset(this, 0x00, sizeof(SVipLevelProp));
    }
};

struct SGoldCostProp
{
    SGoldCostProp()
    {
        nFuncID = 0;
        wIndex = 0;
        wCost = 0;
    }
    UINT16 wIndex;
    UINT16 wCost;
    INT32 nFuncID;
};

struct SBuyGoldProp
{
    SBuyGoldProp()
    {
        dwGold = 0;
        wRMB = 0;
        dwProductID = 0;
        memset(aszProductID, 0x00, MAX_PRODUCTID_LEN);
        memset(aszBuyGoldInfo, 0x00, MAX_COMMON_TXT_NUM);
    }

    UINT32 dwGold;
    UINT16 wRMB;
    UINT32 dwProductID; //产品ID
    TCHAR  aszProductID[MAX_PRODUCTID_LEN]; //VIP购买信息txt
    TCHAR  aszBuyGoldInfo[MAX_COMMON_TXT_NUM]; //VIP购买信息txt
};



typedef HashMap<UINT8, SVipLevelProp> CVipLevelPropMap; //key为byVipLevel
typedef CVipLevelPropMap::iterator CVipLevelPropMapItr;



typedef map<UINT32, SVipLevelProp> CGold2VipLevelPropMap; //key为dwNeedRechargeGold
typedef CGold2VipLevelPropMap::iterator CGold2VipLevelPropMapItr;

//key为index
typedef std::map<UINT16, vector<SGoldCostProp> > CGoldCostPropMap;
typedef CGoldCostPropMap::iterator CGoldCostPropMapItr;

struct SAuthBuyGoldProp
{
    SAuthBuyGoldProp()
    {
        m_vecBuyGoldProp.clear();
        memset(m_aszProContextHead, 0x00, sizeof(m_aszProContextHead));
        memset(m_aszProContext, 0x00, sizeof(m_aszProContext));
        memset(m_aszBuyUrl, 0x00, sizeof(m_aszBuyUrl));
    }
    TCHAR				m_aszProContextHead[MAX_COMMON_TXT_NUM];
    TCHAR				m_aszProContext[MAX_VIP_PRO_CONTEXT_LEN];
    TCHAR				m_aszBuyUrl[MAX_BUY_URL_LEN];
    vector<SBuyGoldProp>	m_vecBuyGoldProp;
};


//key为index
typedef std::map<UINT16, SAuthBuyGoldProp>	CAuthBuyGoldPropMap;
typedef CAuthBuyGoldPropMap::iterator		CAuthBuyGoldPropMapItr;

class CPlayer;
class CVipPropMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CVipPropMgr);

public:
    virtual EConfigModule	GetConfigModuleType()
    {
        return ECM_VIP ;
    }

    BOOL Init();

    VOID UnInit();
public:
    BOOL CkOpenFunc(UINT8 byFuncKind, UINT8 byVipLevel);

    UINT8 GetOpenLevel( UINT8 byFuncKind );

    UINT16 GetIncNum(UINT8 byFuncKind);
    UINT16 GetIncNum(UINT8 byFuncKind, UINT8 byVipLevel);

    BOOL CkCostVIP(UINT8 byFuncKind);

    UINT16 GetLowVipIncLevel( UINT8 byFuncKind );

    UINT8 GetVipLevel(UINT32 wRechargeGold);

    VOID OpenVipTab(UINT8 byAuthType, UINT8& byVipLevelNum, DT_VIP_LEVEL_DATA astVipLevelInfo[MAX_VIP_LEVEL]);
    VOID OpenVipTab(CPlayer *poPlayer, UINT8 byAuthType, UINT8& byVipLevelNum, DT_VIP_LEVEL_DATA astVipLevelInfo[MAX_VIP_LEVEL],
                    DT_BUY_GOLD_TXT astBuyGoldTxtLst[MAX_BUY_GOLD_NUM], TCHAR aszProContextHead[MAX_COMMON_TXT_NUM],
                    TCHAR aszProContext[MAX_VIP_PRO_CONTEXT_LEN], TCHAR aszBuyUrl[MAX_BUY_URL_LEN]);

    //BOOL GetGoldCostByIndex( INT32 nFuncID, UINT16 wIndex, UINT16 &wCostGold );



    UINT32 GetVipTotalAmount(UINT8 byVipLevel);

	SAuthBuyGoldProp* GetBuyGoldInfoByAuthType(UINT8 byAuthType);
    //根据产品ID获得该产品的属性
    BOOL GetBuyPropByProductID(UINT8 byAuthType, INT32 nProductID, SBuyGoldProp &buyGoldProp );

    //根据产品ID获得该产品的属性
    BOOL GetBuyPropByProductID(UINT8 byAuthType, string strProductID, SBuyGoldProp &buyGoldProp );

    //根据充值金额获得该产品的属性
    BOOL GetBuyPropByRMB(UINT8 byAuthType, INT32 nRMB, SBuyGoldProp &buyGoldProp );

    CAuthBuyGoldPropMap &GetAuthBuyGoldProp()
    {
        return m_mapAuthBuyGoldProp;
    }

    CVipLevelPropMap &GetVipLevelProp()
    {
        return m_mapVipLevelProp;
    }

    CGold2VipLevelPropMap &GetGold2VipLevelProp()
    {
        return m_mapGold2VipLevelProp;
    }

    UINT8 & GetMaxVipLevel()
    {
        return m_byMaxVipLevel;
    };

    UINT32 & GetMaxVipLevelRechargeGold()
    {
        return m_dwMaxVipLevelRechargeGold;
    };

public:
    CVipPropMgr();
    virtual ~CVipPropMgr();
protected:
    BOOL LoadFromDB();

    BOOL LoadVipBaseFromDB();

    //BOOL LoadVipLevelFromDB();

    BOOL LoadOpenFuncFromDB();

    BOOL LoadIncNumFromDB();

    BOOL LoadBuyGoldFromDB();

    BOOL LoadFuncCostGoldFromDB();

public:
    VOID InitVipTabInfo();

//

private:
    CVipLevelPropMap		m_mapVipLevelProp;
    CGold2VipLevelPropMap m_mapGold2VipLevelProp;
    UINT32				m_dwMaxVipLevelRechargeGold;
    UINT8				m_byMaxVipLevel;
    CVipOpenFuncPropMap	m_mapFuncKind2OpenFuncProp;  //key为byFuncKind
    CVipLevel2OpenFuncMap m_mapVipLevel2OpenFunc;  //key为VipLevel
    CVipIncNumPropMap		m_mapVipIncNumProp;  //key为byFuncKind+byVipLevel
    CVipLevel2IncNumMap	m_mapVipLevel2IncNum;  //key为byVipLevel
    DT_VIP_LEVEL_DATA		m_astVipLevelInfo[MAX_VIP_LEVEL]; //VIP等级信息
    CGoldCostPropMap		m_mapGoldCostProp; //元宝物品属性表，key为次数
    CAuthBuyGoldPropMap	m_mapAuthBuyGoldProp; //购买元宝信息
};

#endif // 
