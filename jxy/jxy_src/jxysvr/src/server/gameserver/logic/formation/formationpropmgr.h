
#ifndef _FORMATIONPROPMGR_H_
#define _FORMATIONPROPMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <map>
#include <protocol/server/protocommondata.h>
#include <logic/base/basepropmgr.h>

using namespace std;

#define  ATTACK_FORMATION_KIND	1  //进攻型阵型
#define  DEFENCE_FORMATION_KIND 2  //防御型阵型
#define  QINGLONG_FORMATION_KIND 2 //青龙+生命
#define  BAIHU_FORMATION_KIND 1 //白虎+攻击
#define  ZHUQUE_FORMATION_KIND 3 //朱雀+先攻
#define  XUANWU_FORMATION_KIND 4 //玄武+防御

struct SFormationUnlockProp
{
	UINT8 byFormationID;
	UINT8 byVisableLevel;
	UINT8 byUnlockLevel;
};

typedef map<UINT8, SFormationUnlockProp> CMapFormationUnlockProp;
typedef CMapFormationUnlockProp::iterator CMapFormationUnlockPropItr;

struct SFormationLevelProp
{
	UINT8 byFormationID;
	UINT8 byOpenIdx;
	UINT8 byCoachIdxFlag;
	UINT8 byVisableLevel;
	UINT8 byUnlockLevel;
};

typedef map<UINT8, SFormationLevelProp> CMapFormationLevelProp;
typedef CMapFormationLevelProp::iterator CMapFormationLevelPropItr;
typedef CMapFormationLevelProp::const_iterator CMapFormationLevelPropConstItr;

typedef map<UINT8, CMapFormationLevelProp> CMapFormationLevelMap;
typedef CMapFormationLevelMap::iterator CMapFormationLevelMapItr;
typedef CMapFormationLevelMap::const_iterator CMapFormationLevelMapConstItr;

struct SFormationUpgradeProp
{
	UINT8 byFormationID;
	UINT8 byAttrKindID;
	UINT8 byAttrLevel;
	UINT32 dwAttrValue;
	UINT32 dwUpgradeCostValue;
	UINT8 byNeedPlayerLevel;
	UINT8 byUnlockPosNum;
};

typedef map<UINT8, SFormationUpgradeProp> CMapFormationUpgradeProp;
typedef CMapFormationUpgradeProp::iterator CMapFormationUpgradePropItr;
typedef map<UINT8, CMapFormationUpgradeProp> CMapFormationUpgradeMap;
typedef CMapFormationUpgradeMap::iterator CMapFormationUpgradeMapItr;


typedef map<UINT8, UINT8> CFormationIdxMap;//key/value为阵型序号
typedef CFormationIdxMap::iterator CFormationIdxMapItr;
//阵型配置
typedef struct tagFormationProp
{
    tagFormationProp()
    {
        byFormationKindID = 0;
        mapOpenIdx.clear();
        memset(&stDT_FORMATION_IDX_DATA, 0, sizeof(stDT_FORMATION_IDX_DATA));
        byCoachFormationIdx = 0;
        byCoachFormationLstIdx = 0;
    }
    inline DT_FORMATION_IDX_DATA& GetDT_FORMATION_IDX_DATA()
    {
        return stDT_FORMATION_IDX_DATA;
    }

    UINT8	byFormationKindID;	// 阵型类型
    CFormationIdxMap mapOpenIdx;// 开启的阵型位置
    UINT8   byCoachFormationIdx;			// 主将默认位置
    UINT8   byCoachFormationLstIdx;			// 主将默认位置(DT_FORMATION_HERO_DATA数组下标)

    DT_FORMATION_IDX_DATA stDT_FORMATION_IDX_DATA;
} SFormationProp;

typedef map<UINT8, SFormationProp> CFormationKindMap;
typedef CFormationKindMap::iterator CFormationKindMapItr;

class CFormationPropMgr : public CBasePopMgr
{
public:

    DECLARE_SINGLETON_PROPMGR(CFormationPropMgr);

public:
    virtual EConfigModule	GetConfigModuleType()
    {
        return ECM_FROMATION ;
    }

    BOOL Init();

    VOID UnInit();
public:
    inline CFormationKindMap& GetFormationKindMap()
    {
        return m_mapFormationInfo;
    }
    SFormationProp* GetFormationProp(UINT8 byFormationKind);

	const SFormationLevelProp* GetFormationLevelProp(UINT8 byFormationID, UINT8 byPostionIdx);
	const CMapFormationLevelProp* GetFormationLevelMap(UINT8 byFormationID);

	const CMapFormationLevelMap& GetFormationLevelAllMap()
	{
		return m_mapFormationLevelProp;
	}

	const SFormationUpgradeProp* GetFormationUpgrade(UINT8 byFormationID, UINT8 byFormationLevel);
	UINT8 GetUnlockPosNum(UINT8 byFormationID, UINT8 byFormationLevel);
	BOOL GetFormationUnlock(UINT8 byFormationID, UINT8& byVisableLevel, UINT8& byUnlockLevel);
public:
    CFormationPropMgr();
    virtual ~CFormationPropMgr();
protected:
	
    BOOL LoadFromDB();
    VOID InitData();
	BOOL LoadFromDBProp2();
	BOOL LoadFormationUpgrade();
	BOOL LoadFromationUnlock();
private:
    CFormationKindMap m_mapFormationInfo;
	CMapFormationLevelMap m_mapFormationLevelProp;
	CMapFormationUpgradeMap m_mapFormationUpgradeProp;
	CMapFormationUnlockProp m_mapFormationUnlockProp;
};

#endif // 
