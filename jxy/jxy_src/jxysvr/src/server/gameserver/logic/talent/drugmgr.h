
#ifndef _DRUGMGR_H_
#define _DRUGMGR_H_

#include "drug.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdhashmap.h>
#include <sdmutex.h>

using namespace SGDP;


typedef HashMap<UINT64, CDrug*> CDrugMap;
typedef CDrugMap::iterator CDrugMapItr;

struct tagDT_EXPERIENCE_DRUG_DATA;
typedef tagDT_EXPERIENCE_DRUG_DATA DT_EXPERIENCE_DRUG_DATA;

struct tagDT_GOLD_EXPERIENCE_DRUG_DATA;
typedef tagDT_GOLD_EXPERIENCE_DRUG_DATA DT_GOLD_EXPERIENCE_DRUG_DATA;


class CDrugMgr
{
public:
    DECLARE_SINGLETON(CDrugMgr)

    BOOL Init();

    VOID UnInit();

public:

    CDrug*	CreateDrug(UINT32 dwPlayerID, UINT16 wHeroKindID, UINT8 byDrugLevel);

    VOID	RemoveDrug(UINT32 dwPlayerID, UINT16 wHeroKindID, UINT8 byDrugLevel);

    CDrug*	FindDrug(UINT32 dwPlayerID, UINT16 wHeroKindID, UINT8 byDrugLevel);

    UINT16	UseDrug(CPlayer* poPlayer, UINT16 wHeroKindID, UINT16 wDrugLevel);

    UINT16	UseExperienceDrug(CPlayer* poPlayer, UINT16 wHeroKindID, UINT16 wExperienceDrugID, UINT16 wGoldExperienceCount);

	UINT16	GetDT_EXPERIENCE_DRUG_DATA(CPlayer* poPlayer,DT_EXPERIENCE_DRUG_DATA astExperienceDrugDataList[3]);
	
	UINT16	GetDT_EXPERIENCE_DRUG_DATA(CPlayer* poPlayer, UINT16 wExperienceDrugID, DT_EXPERIENCE_DRUG_DATA & stExperienceDrugData);

	UINT16  GetDT_GOLD_EXPERIENCE_DRUG_DATA( CPlayer *poPlayer, DT_GOLD_EXPERIENCE_DRUG_DATA &data );
protected:
    CDrugMgr();
    ~CDrugMgr();

public:
	const static INT32 ms_nGoldExperienceID=99; //元宝经验丹的ID
private:
    CDrugMap				m_mapDrug;
    CSDObjectPool<CDrug, CSDMutex>	m_oDrugShmemObjMgr;
};




#endif //#ifndef _ROLEMGR_H_

