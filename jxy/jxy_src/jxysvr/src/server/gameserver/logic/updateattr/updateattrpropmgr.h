#ifndef _UPDATE_ATTR_PROP_MGR_H_
#define _UPDATE_ATTR_PROP_MGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>

typedef struct tagUpdateCostData
{
	UINT32 dwCostCoinValue;
	UINT16 wCostItemValue;
}SUpdateCostData;

//升级增加的属性
typedef struct tagUpdateData
{
	SUpdateCostData stCost;
	UINT32 dwPerCountAddValue; //每次增加的属性
}SUpdateData;

class CUpdateAttrPropMgr : public CBasePopMgr
{
public:
	DECLARE_SINGLETON_PROPMGR( CUpdateAttrPropMgr );
	CUpdateAttrPropMgr();
	~CUpdateAttrPropMgr();

public:
	//typedef HashMap< UINT8, tagUpdateCostData > CUpdateCostInfoMgr;
	struct STParam
	{
		//FLOAT fTalentAttackFactor;
		//FLOAT fTalentHPFactor;
		//FLOAT fHPScaleToAttack;
		//FLOAT fH_AttackFactor;
		//FLOAT fH_HPFactor;
		//FLOAT fA_AttackFactor;
		//FLOAT fA_HPFactor;
		UINT16 wStrengthScaleToNormal;
		UINT16 wUpdateLevelAddCount;
	};
public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_UPDATE_ATTR ; }
	BOOL Init();
	VOID UnInit();

	//byType为查询的属性类型,wLevel为玩家级别
	BOOL GetUpdateAttrNeedInfo( BOOL bLeader, UINT8 byAttrType, UINT8 byJobType, UINT16 wAttrLevel, UINT32 dwTalent, SUpdateData &data, UINT16 wLevel );

	//获得总共次数
	UINT16 GetTotalCount( UINT16 wLevel );

	//根据资质和属性等参数，计算出实际级别增加的值,
	//参数bIncludeDefaultAttr是用来确认是否包含初始化中的1J的数据，因为伙伴初始值已经包含1级的数据，计算属性加成时，大部分都不应该重复计算第一级的数据
	DOUBLE GetAttrAddValue( UINT32 dwTalent, UINT16 wLevel, UINT8 byAttrType, UINT8 byJobType, BOOL bIncludeDefaultAttr=TRUE );
private:
	//初始化数据
	BOOL InitData();

	//wLeve为武将等级
	UINT32 GetAttrPerAddValue( UINT8 byAttrType, UINT8 byJobType, UINT16 wLevel, UINT32 dwTalent );
private:
	STParam m_oParam;
};


#endif