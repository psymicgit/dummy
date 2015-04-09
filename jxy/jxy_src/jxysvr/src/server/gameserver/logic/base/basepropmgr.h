#ifndef _BASEPROPMGR_H
#define _BASEPROPMGR_H

#include <map>
#include <sdtype.h>

using namespace std;

#define DECLARE_SINGLETON_PROPMGR(cls)\
private:\
	static cls* m_poInstance;\
	static cls* m_poInstance1;\
	static cls* m_poInstance2;\
public:\
	BOOL	ReLoadProp();\
public:\
	static BOOL CreateInstance()\
	{\
		if(NULL == m_poInstance)\
		{\
			m_poInstance1 = SDNew cls;\
			m_poInstance2 = SDNew cls;\
			m_poInstance = m_poInstance1;\
		}\
		if(m_poInstance)\
		{\
			AddToMgr(m_poInstance);\
		}\
		return ((m_poInstance1 != NULL) && (m_poInstance2 != NULL));\
	}\
	static cls* Instance(){ return m_poInstance; }\
	static VOID DestroyInstance()\
{\
	m_poInstance = NULL;\
	if(m_poInstance1 != NULL)\
{\
	SDDelete m_poInstance1;\
	m_poInstance1 = NULL;\
}\
	if(m_poInstance2 != NULL)\
{\
	SDDelete m_poInstance2;\
	m_poInstance2 = NULL;\
}\
}

#define IMPLEMENT_SINGLETON_PROPMGR(cls) \
	cls* cls::m_poInstance = NULL;\
	cls* cls::m_poInstance1 = NULL;\
	cls* cls::m_poInstance2 = NULL;\
	BOOL cls::ReLoadProp()\
{\
	if(m_poInstance == m_poInstance1)\
{\
	m_poInstance2->UnInit();\
	if(m_poInstance2->Init())\
{\
	m_poInstance = m_poInstance2;\
	return TRUE;\
}\
	return FALSE;\
}\
		else\
{\
	m_poInstance1->UnInit();\
	if(m_poInstance1->Init())\
{\
	m_poInstance = m_poInstance1;\
	return TRUE;\
}\
	return FALSE;\
}\
}


#define DB_READ_BEGIN(tablename) \
	SGDP::ISDDBSession* pDBSession = gsapi::GetDBMgr()->GetCfgDBSession(); \
	if (NULL == pDBSession) { \
		PRINT_ERR_INFO; \
		return FALSE; \
	} \
	SGDP::ISDDBRecordSet* pRecordSet = NULL; \
	INT32 nRet = pDBSession->ExecuteSqlRs(GetPropTableSelectSql(tablename).c_str(), &pRecordSet); \
	if(SDDB_HAS_RECORDSET != nRet) { \
		PRINT_ERR_INFO; \
		return FALSE; \
	} \

#define DB_READ_END \
	if (NULL != pRecordSet) { \
		pRecordSet->Release(); \
		pRecordSet = NULL; \
	} \

enum EConfigModule
{
	ECM_ALL				=	0,
	ECM_GODWEAPON		=	1,
	ECM_GODWEAPON_BASE	=	2,
	ECM_SKILL			=	3,
	ECM_HERO			=	4,
	ECM_MONSTER			=	5,
	ECM_MONSTER_UPGRADE	=	6,
	ECM_INSTANCE		=	7,
	ECM_BAG				=	8,
	ECM_BUILD			=	9,
	ECM_TASK			=	10,
	ECM_SHOP			=	11,
	ECM_DRUG			=	12,
	ECM_DRUG_UPGRADE	=	13,
	ECM_EXP_DRUG		=	14,
	ECM_LOGIN_ENC		=	15,
	ECM_ONLINE_ENC		=	16,
	ECM_PLUNDER_BASE	=	17,
	ECM_PLUNDER_GRID	=	18,
	ECM_MONEY_TREE_BASE	=	19,
	ECM_MONEY_TREE		=	20,
	ECM_AWAKEN			=	21,
	ECM_ITEM_COMPOSE	=	22,
	ECM_EQUIP			=	23,
	ECM_EQUIP_STRENGTHEN=	24,
	ECM_GOODS			=	25,
	ECM_CROP			=	26,
	ECM_SCIENCE_UPDATE	=	27,
	ECM_UPDATE_ATTR		=	28,
	ECM_FROMATION		=	29,
	ECM_YABIAO			=	30,
	ECM_VIP				=	31,
	ECM_EATBREAD		=	32,
	ECM_JINGJIE			=	33,
	ECM_CHAT			=	34,
	ECM_LUA				=	35,
	ECM_HPGRID			=	36,
	ECM_PHYSTRENGTH		=	37,
	ECM_SINGLE_PRAM		=	38,
	ECM_XIANQIAN		=	39,
	ECM_RACE			=	40,
	ECM_ACTIVITY_ENC	=	41,
	ECM_BOSSB			=	42,
	ECM_COMMONINFO		=	43,
	ECM_RDCHALLENGE		=	44,
	ECM_WORSHIP			=	45,
	ECM_AWAKEN2			=	46,
	ECM_COLLECTGAS		=	47,
    ECM_BEGBLUE         =   48,
    ECM_OPENLIGHT       =   49,
	ECM_ITEMEXCHANGE	=	50,
	ECM_ERRMSG			=	51,
	ECM_SENDFLOWER      =   52,
	ECM_COURAGE		    =	53,
    ECM_EQUIP_MASTER    =   54,
	ECM_RAREITEMROOM    =   55,
	ECM_GYMNASIUM		=   56,
	ECM_SHADE			=   57,
	ECM_HOTSPRING       =   58,
	ECM_VIPGIFT			=	59,
    ECM_CHALLERGE       =   60,
	ECM_JEWELRY         =   61,
	ECM_GVG			    =	62,
	ECM_SPECIALEFFECT   =   63,
	ECM_SOUL            =   64,
	ECM_MEDAL			=	65,
    ECM_PET				=	66,
	
    ECM_EQUIP_MAKE		=	67, // 装备制作
    ECM_FLY		        =	68, // 飞升
	ECM_EXCHANGE		=   67,
};

class CLogicConfigMgr;
class CBasePopMgr
{
	friend class CLogicConfigMgr;
public:
	virtual EConfigModule	GetConfigModuleType() = 0;
	virtual BOOL			ReLoadProp() = 0;
	static	VOID			AddToMgr(CBasePopMgr* poMgr){ m_mapKind2Mgr[poMgr->GetConfigModuleType()] = poMgr;}
	virtual	BOOL			Init() = 0;
	virtual	VOID			UnInit() = 0;
protected:
	static	map<UINT16, CBasePopMgr*>	m_mapKind2Mgr;
};

string GetPropTableSelectSql(string strTableName, string strPostfixSql="", string strPrefixSql="select * from ");
string GetPropTable(string strTableName);

#endif
