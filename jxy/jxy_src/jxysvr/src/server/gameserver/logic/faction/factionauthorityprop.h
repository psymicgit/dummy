
#ifndef _FACTION_AUTHORITY_PROP_H_
#define _FACTION_AUTHORITY_PROP_H_


#include <sddb.h>
#include <sdsingleton.h>
#include <sdtype.h>
#include <protocol/server/protocommondata.h>

#include <common/client/commondef.h>
#include <map>


typedef std::map<UINT32, DT_FACTION_AUTHORITY_EX>	CFactionAuthorityMap; //keyΪPlayerID
typedef CFactionAuthorityMap::iterator			CFactionAuthorityMapItr;


class CFactorAuthorityPropMgr
{
public:
    DECLARE_SINGLETON(CFactorAuthorityPropMgr);

public:

    BOOL Init();

    VOID UnInit();
public:
	BOOL CkJobAuthority(em_Faction_Job myJob, em_Faction_Authority myAuth);
	em_Faction_Job UpJob(em_Faction_Job myJob);
	em_Faction_Job DownJob(em_Faction_Job myJob);
	BOOL CkJobNum(em_Faction_Job myJob, UINT16 wJobNum, UINT32 dwFactionLevel);

	DT_FACTION_AUTHORITY GetAuthority(em_Faction_Job myJob);
	DT_FACTION_AUTHORITY_EX* GetAuthorityEx(em_Faction_Job myJob);
protected:
	DT_FACTION_AUTHORITY ToAuthority(DT_FACTION_AUTHORITY_EX* stEx);
    CFactorAuthorityPropMgr();
    virtual ~CFactorAuthorityPropMgr();
protected:
    BOOL LoadFromDB();

private:
	CFactionAuthorityMap m_mapFactionAuthority;
};




#endif //#ifndef _ROLEMGR_H_

