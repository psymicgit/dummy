
#ifndef _DRUGPROPMGR_H_
#define _DRUGPROPMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>

//名称最大长度
#define NAME_LEN_MAX	32

//丹药配置
typedef struct tagDrugProp
{
	UINT8	byLevel;					//丹药等级
	TCHAR	szName[NAME_LEN_MAX];		//物品名称
	UINT8	byCanUseNum;				//最大可使用数目
	UINT16	wUseCD;					//使用间隔时间
	UINT16	wBaseTalent;				//资质基础加成值
	UINT16	wDecTalent;				//每多使用1个资质加成递减值
	UINT16   wDrugItemKindID;			//对应的丹药物品ID
}SDrugProp;


typedef std::map<UINT8, SDrugProp> CDrugPropMap; //key为Level
typedef CDrugPropMap::iterator CDrugPropMapItr;

typedef std::map<UINT16, SDrugProp> CItemID2DrugPropMap; //key为丹药物品ID
typedef CItemID2DrugPropMap::iterator CItemID2DrugPropMapItr;


class CDrugPropMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CDrugPropMgr);

public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_DRUG; }

    BOOL Init();

    VOID UnInit();
public:
	SDrugProp* GetDrugProp(UINT8 byLevel);
	UINT16 GetDrugLevel(UINT16 wDrugItemID); //获取丹药等级

	CDrugPropMap & GetDrugPropMap()	{ return m_mapDrugProp;	}

	UINT8	GetDT_MIX_DRUG_LEAVE_BY_INDEX(const UINT8 byIndex);

	BOOL	IsDrugGoods(UINT16 wItemID) { return (m_mapItemID2DrugProp.end() != m_mapItemID2DrugProp.find(wItemID)); }; //是否为丹药物品

	UINT8	GetMaxDrugNum();

public:
    CDrugPropMgr();
    virtual ~CDrugPropMgr();
protected:

	BOOL LoadFromDB();

private:
	CDrugPropMap m_mapDrugProp;
	CItemID2DrugPropMap m_mapItemID2DrugProp;//丹药物品ID map
};

#endif // 
