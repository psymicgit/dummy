
#ifndef _EXPERIENCEDRUGPROPMGR_H_
#define _EXPERIENCEDRUGPROPMGR_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <logic/base/basepropmgr.h>

//名称最大长度
#define NAME_LEN_MAX	32

//丹药配置
typedef struct tagExperienceDrugProp
{
    UINT8	byLevel;						//丹药等级
    TCHAR	szName[NAME_LEN_MAX];			//物品名称
    UINT16   wExperienceDrugItemKindID;		//对应的丹药物品ID
} SExperienceDrugProp;

typedef std::map<UINT16, SExperienceDrugProp> CExperienceDrugPropMap; //key为丹药物品ID
typedef CExperienceDrugPropMap::iterator CExperienceDrugPropMapItr;
class CExperienceDrugPropMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CExperienceDrugPropMgr);
public:
	virtual EConfigModule	GetConfigModuleType() { return ECM_EXP_DRUG ; }
    BOOL Init();
    VOID UnInit();
public:
    CExperienceDrugPropMap & GetExperienceDrugPropMap()
    {
        return m_mapExperienceDrugProp;
    }

    //是否为丹药物品
    BOOL	IsExperienceDrugGoods(UINT16 wItemID)
    {
        return (m_mapExperienceDrugProp.end() != m_mapExperienceDrugProp.find(wItemID));
    };

	SExperienceDrugProp * GetExperienceDrugProp(UINT16 wKindID)
	{
		CExperienceDrugPropMapItr itr = m_mapExperienceDrugProp.find(wKindID);
		if (m_mapExperienceDrugProp.end() != itr)
		{
			return &itr->second;
		}
		return NULL;
	}
public:
    CExperienceDrugPropMgr();
    virtual ~CExperienceDrugPropMgr();
protected:
    BOOL LoadFromDB();
private:
    CExperienceDrugPropMap			m_mapExperienceDrugProp;//丹药物品ID map
};

#endif // 
