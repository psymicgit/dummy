#ifndef _GYMNASIUM_PROP_MGR_H_
#define _GYMNASIUM_PROP_MGR_H_


#include <sdtype.h>
#include <sdhashmap.h>
#include <logic/base/basedefine.h>
#include <protocol/server/protocligs.h>
#include <common/server/utility.h>
#include <common/client/commondef.h>
#include <logic/base/basepropmgr.h>
#include <map>
#include <sdmutex.h>
#include <sdobjectpool.h>
#include "gymnasium.h"
using namespace std;


#define GS_MSG_LEN 32
struct SSkillProp
{
	UINT8 bySkillKindID;
	UINT32 SkillLevel;
	UINT32 SkillValue;
	UINT32 LearnDoorsTribute;
	UINT32 dwSumDoorsTribute;
	SSkillProp(){ memset(this, 0, sizeof(SSkillProp)); }
};

struct SSkillInfo
{
	UINT8 bySkillKindID;
	TCHAR SKillName[SKILL_NAME_LEN];
	UINT8 byUnlockLevelByGymnasium;
	UINT8 byDisplayerLevel;
	SSkillInfo(){ memset(this, 0, sizeof(SSkillInfo));}
};

struct SGymnasiumUpgradeProp
{
	UINT8 dwGymnasiumLevel;
	UINT32 dwNeedFactionFunds;
	UINT32 dwNeedFactionLevel;
	UINT8  byCurMaxLevel;
};

struct SFactionSkill
{
	UINT8 bySkillID;//技能ID
	TCHAR aszSkillName[SKILL_NAME_LEN];//技能名称
	UINT8 byUnlockFlag;//技能解锁状态
	UINT8 bySkillCurMax;//技能显示上限
};
typedef map<UINT32, SFactionSkill> CMapFactionSkill;
typedef CMapFactionSkill::iterator CMapFactionSkillItr;
typedef CMapFactionSkill::const_iterator CMapFactionSkillconstItr;




typedef map<UINT8, SGymnasiumUpgradeProp> SGymnasiumUpgradePropMap;
typedef map<UINT8, SGymnasiumUpgradeProp>::iterator SGymnasiumUpgradePropItr;

typedef multimap<UINT8, SSkillProp> SkillPropMultiMap;
typedef multimap<UINT8, SSkillProp>::iterator SkillPropMultiItr;
typedef pair<SkillPropMultiItr, SkillPropMultiItr> SkillPropPair;
typedef map<UINT8, SSkillInfo> SkillInfoMap;
typedef map<UINT8, SSkillInfo>::iterator SkillInfoItr;
typedef multimap<UINT8, SSkillInfo> UnlockSkillMap;
typedef UnlockSkillMap::const_iterator UnlockSkillMapIrt;

class CGymnasiumPropMgr : public CBasePopMgr
{
	DECLARE_SINGLETON_PROPMGR( CGymnasiumPropMgr );
public:
	virtual EConfigModule GetConfigModuleType() { return ECM_GYMNASIUM; }

	BOOL Init();

	VOID UnInit();

	BOOL GetSkillInfoProp(SSkillProp& stSkillProp, UINT8 bySkillKindID, UINT32 dwSkillLevel);

	//获取技能名称
	TCHAR* GetSkillName(UINT32 dwSkillKindID);

	//获取技能个数
	UINT32 GetSkillNum();

	SkillInfoMap GetSkillInfoMap(){ return m_mapSkillInfo;}

	BOOL GetGymnasiumUpgradeProp(SGymnasiumUpgradeProp& stGymnasiumUpgradeProp, UINT32 dwGymnasiumLevel);

	const UnlockSkillMap& GetSkillUnLock() {return m_mapSkillInfoByUnLock; }
	
	
	////////////////////////////////////New//////////////////////////////////////
	//门派武堂技能
	CMapFactionSkill GetFactionSkill(UINT32 dwGymnasiumLevel);
	//当前最高技能等级
	UINT8 GetCurMaxSkillLevel(UINT32 dwGymnasiumLevel);
	//总最高技能等级
	UINT32 GetMaxSkillLevel() { return m_dwMaxSkillLevel; }
	//当前技能值与上一级技能值差
	VOID AddDiffAttribute(UINT8 byKindID, UINT32 dwSkillLevel ,UINT32& dwAddAttrValue);
	//当前技能值
	BOOL AddAttribute(UINT8 byKindID, UINT32 dwSkillLevel ,UINT32& dwAddAttrValue);
	//技能解锁等级
	UINT8 GetUnlockSkillLevel(UINT8 bySkillID);
	//学习技能所需武堂等级
	UINT32 GetRequireGymnasiumLevel(UINT8 bySkillLevel);
public:
	CGymnasiumPropMgr(){}
	~CGymnasiumPropMgr(){}

protected:
	BOOL LoadPropFromDB();
	BOOL LoadInfoFromDB();
	BOOL LoadUpgradeFromDB();

private:

	SkillPropMultiMap m_multiMapSkillProp;
	SkillInfoMap m_mapSkillInfo;
	UnlockSkillMap m_mapSkillInfoByUnLock;
	SGymnasiumUpgradePropMap m_mapGymnasiumUpgradeProp;
	UINT32 m_dwMaxSkillLevel;


};
#endif