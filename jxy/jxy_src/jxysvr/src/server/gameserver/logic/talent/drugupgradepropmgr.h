
#ifndef _DRUGGRADEPROPMGR_H_
#define _DRUGGRADEPROPMGR_H_


#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>
#include <common/server/utility.h>
#include <logic/base/basepropmgr.h>
#include <protocommondata.h>


using namespace SGDP;


//丹药配置
typedef struct tagDrugUpradeProp
{
    UINT32	dwSrcLevel;					//丹药等级
    UINT32	dwSrcNum;					//合成丹药的个数
	UINT32	dwLevel1Rate;	
    UINT32	dwLevel2Rate;					//
    UINT32	dwLevel3Rate;					//
    UINT32	dwLevel4Rate;					//
    UINT32	dwLevel5Rate;					//
    UINT32   dwLevel6Rate;					//
} SDrugUpradeProp;

typedef map<UINT32, SDrugUpradeProp> CDrugUpradePropMap;
typedef CDrugUpradePropMap::iterator CDrugUpradePropMapItr;



class CDrugUpradePropMgr : public CBasePopMgr
{
public:
    DECLARE_SINGLETON_PROPMGR(CDrugUpradePropMgr)
	virtual EConfigModule	GetConfigModuleType() { return ECM_DRUG_UPGRADE ; }

    BOOL Init();
    VOID UnInit();

    VOID  GetDT_MIX_DRUG_NUM(UINT8 byDrugNum[MAX_DRUG_LEVEL]);
	UINT8 GetDT_MIX_DRUG_NUM(UINT8 byDrugLeave);
	VOID  GetDT_MIX_DRUG_RATE(CRandomVec &vecRandom,UINT8 byDrugLeave);

public:
    CDrugUpradePropMgr();
    ~CDrugUpradePropMgr();


protected:
    BOOL LoadFromDB();
private:
    UINT8				m_byDrugNum[MAX_DRUG_LEVEL];
    CDrugUpradePropMap	m_mapUpradePropDrug;
};


#endif