
#include <db/autosqlbuf.h>
#include "clrplayerdatacmd.h"
#include <db/dbsqlstring.h>
#include <sdutil.h>
#include "server/utility.h"
#include "common/client/errdef.h"
#include <sdloggerimpl.h>



CClrPlayerDataCMD::CClrPlayerDataCMD()
{
	m_dwPlayerID = 0;
}


CClrPlayerDataCMD::~CClrPlayerDataCMD()
{
}

VOID CClrPlayerDataCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
	SDTRY_BEGIN

	m_bSuccess = ClrData(poDBConn);

	SDTRY_END

}

VOID CClrPlayerDataCMD::OnExecuted()
{
	SDTRY_BEGIN

	SDTRY_END
}


BOOL CClrPlayerDataCMD::ClrData(SGDP::ISDDBConnection* poDBConn)
{
	SDTRY_BEGIN

	CHAR szSQL[128] = {0};
	sprintf(szSQL, "delete from player where ID = '%u'; ", m_dwPlayerID);
	UINT32 dwErr;
	string strError;
	INT32 nRet = poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
	if (nRet < 0 )
	{
		SYS_CRITICAL(_SDT("[%s: %d]: ClrData[%d] failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, m_dwPlayerID, dwErr, strError.c_str(), szSQL);    
		return FALSE;
	}

	sprintf(szSQL, "delete from playerpvp where ID = '%u'; ", m_dwPlayerID);
	nRet = poDBConn->ExecuteSql(szSQL, NULL, NULL,&dwErr, &strError);

	if(nRet < 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: ClrData[%d] failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, m_dwPlayerID, dwErr, strError.c_str(), szSQL); 
		return FALSE;
	}		

	return TRUE;
	SDTRY_END
	return FALSE;
}

VOID CClrPlayerDataCMD::SetUserData(VOID* pUserData)
{
	m_dwPlayerID  = *((UINT32*)(pUserData));
}
