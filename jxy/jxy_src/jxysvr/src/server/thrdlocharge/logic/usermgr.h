

#ifndef _USERMGR_H_
#define _USERMGR_H_

#include "user.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdtype.h>
#include <sdidpool.h>
#include <map>
#include <sdmutex.h>
#include <common/server/auth.h>

using namespace std ;
using namespace SGDP;


#define MAX_USER	10000

typedef map<UINT64 , CUser*> CSessionID2UserMap;		
typedef CSessionID2UserMap::iterator CSessionID2UserMapItr;


class CUserMgr:public CAuthClientMgr
{
public:
    DECLARE_SINGLETON(CUserMgr)

public:
	BOOL			Init();

	CUser*			CreateUser(UINT16 wZoneID, UINT32 dwTransID, PKT_LSTLC_AUTH_REQ& stReq);
	CUser*			FindUser(UINT16 wZoneID, UINT32 dwTransID);
	virtual CAuthClient* SDAPI FindClient(UINT16 wZoneID, UINT32 dwTransID);
	VOID			ReleaseUser(CUser* poUser);


	VOID			Run();
private:

	VOID			CkUserTimeOut();

protected:
    CUserMgr();
    ~CUserMgr();
protected:

private:
    CSDObjectPool<CUser, CSDMutex>		m_oUserPool;
    CSessionID2UserMap					m_mapID2User;
	CSDMutex							m_oMutex	;
	UINT64								m_qwLastCkTime;
};



#endif


