#ifndef _SCIENCE_UPDATE_PROP_MGR_H_
#define _SCIENCE_UPDATE_PROP_MGR_H_

#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <sdhashmap.h>
#include <logic/base/basepropmgr.h>

using namespace std;
using namespace SGDP;

typedef struct tagScienceUpdateData
{
	UINT16 wLevel; //当前等级
	UINT32 dwCost; //升级到当前
	UINT32 dwAddValue; //增加的属性
	UINT16 wNeedPlayerLevel; //需要玩家的级别
} SScienceUpdateData;

typedef struct tagScienceData
{
	vector<SScienceUpdateData> vecScienceUpdateData;
	UINT16 wVisibleLevel; //可见的级别,未达到，看不见这科技
	UINT16 wUnlockLevel;  //解锁的级别，未达到不能点升级
} SScienceData;

class CScienceUpdatePropMgr : public CBasePopMgr
{
public:
	//获得科技树信息
	enum EGetScienceInfoError
	{ 
		GUIE_SUCCESS=0,
		GUIE_AT_MAXMIUM_LEVEL, //达到配置的最大值,没有升级信息
		GUIE_CANNOT_FIND_SCIENCE, //无法找到指定科技
	};
	DECLARE_SINGLETON_PROPMGR( CScienceUpdatePropMgr );

	virtual EConfigModule	GetConfigModuleType() { return ECM_SCIENCE_UPDATE ; }

public:
	typedef HashMap< UINT8, tagScienceData > CSciencePropMap; //key为科技属性，value为实际消耗的阅历

public:
	CScienceUpdatePropMgr();
	~CScienceUpdatePropMgr();
	BOOL Init();
	VOID UnInit();
	EGetScienceInfoError GetInfo( UINT8 byKind, UINT16 wLevel, UINT32 &wCostVaue, UINT32 &wAddValue ); //获得升级到此级的消耗和增加的值
	EGetScienceInfoError GetInfo( UINT8 byKind, UINT16 wLevel, UINT32 &wCostVaue, UINT32 &wAddValue, UINT16 &wUnlockLevel );
	EGetScienceInfoError GetUnlockInfo( UINT8 byKind, UINT16 wLevel, BOOL &bVisible, BOOL &bUnlock ); //获得此种类型的是否可见或者解锁
	INT32 GetKindCount(); //获得科技的种类数量
	INT32 GetUpdateNeedLevel( UINT8 byKind, UINT16 wSicenceLevel ); //获得升级需要的等级

	vector<UINT8> GetSortScienceKindList(); //获得按解锁顺序的科技列表

	//参数wUpdateScienceLevel为升级的科技信息
	BOOL IsCanUpdateAttr( UINT64 qwCurStory, UINT16 wUpdateScienceLevel, UINT16 wPlayerLevel );
	BOOL IsCanUpdateAttr( UINT64 qwCurStory, UINT16 wUpdateScienceLevel, UINT16 wPlayerLevel, UINT8 byType );

	//获得某一级别解锁的科技列表
	vector<UINT8> GetUnlockKindListAtLevel( UINT16 wPlayerLevel );

	//获得属性的最大级别
	UINT16 GetMaxLevel( UINT8 byKind );
private:
	/**
     * @brief 从数据源获取属性数据
     * @return 执行结果，成功返回TRUE，失败返回FALSE
     */
	BOOL LoadFromDBEx();

	VOID LogAllKind(); //输出所有的科技种类

	static bool SortByUnlockLevel( UINT8 kind1, UINT8 kind2 );
private:
	CSciencePropMap m_mapUpdateProp; //升级所需阅历
	vector<UINT8> m_vecSortScienceList; //科技列表(按解锁的顺序)
};

#endif