

#ifndef _AUTHUSERMGR_H_
#define _AUTHUSERMGR_H_

#include "authuser.h"
#include <sdsingleton.h>
#include <sdobjectpool.h>
#include <sdtype.h>
#include <sdidpool.h>
#include <map>
#include <sdmutex.h>

using namespace std ;
using namespace SGDP;


#define MAX_USER	10000

typedef map<UINT32 , CAuthUser*> CSessionID2AuthUserMap;
typedef CSessionID2AuthUserMap::iterator CSessionID2AuthUserMapItr;


class CAuthUserMgr
{
public:
    DECLARE_SINGLETON(CAuthUserMgr)

public:
	BOOL			Init();

	VOID				ReqAuth(UINT32 dwTransID, PKT_GTLS_LOGIN_REQ* pstReq1, PKT_GTLS_LOGIN_EX_REQ* pstReq2);
	VOID				OnAuthAck(UINT32 dwTransID, UINT16 wErrCode);

	CAuthUser*			CreateUser(UINT32 dwTransID, PKT_GTLS_LOGIN_REQ* pstReq1, PKT_GTLS_LOGIN_EX_REQ* pstReq2);
	CAuthUser*			FindUser(UINT32 dwTransID);
	VOID				ReleaseUser(CAuthUser* poUser);

	VOID				Run();
private:

	VOID				CkUserTimeOut();

protected:
    CAuthUserMgr();
    ~CAuthUserMgr();
protected:

private:
    CSDObjectPool<CAuthUser, CSDMutex>	m_oUserPool;
    CSessionID2AuthUserMap				m_mapID2User;
	CSDMutex							m_oMutex;
	UINT64								m_qwLastCkTime;
};



#endif 


