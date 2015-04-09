
#include "framework/lsapplication.h"
#include "updatelogintimecmd.h"
#include <sdutil.h>
#include <logic/dbconfig.h>
#include "net/gt/gtpktbuilder.h"
#include "common/client/errdef.h"





CUpdateLoginTimeCMD::CUpdateLoginTimeCMD()
{
	m_dwUserID = 0;
	m_wZoneID = 0;
}


CUpdateLoginTimeCMD::~CUpdateLoginTimeCMD()
{
}

VOID CUpdateLoginTimeCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
	

		m_bSuccess = FALSE;
	
	if (FALSE == UpdateLoginTime(poDBConn))	
	{
		m_bSuccess = FALSE;
	}
	else
	{
		m_bSuccess = TRUE;
	}

	

}

VOID CUpdateLoginTimeCMD::OnExecuted()
{
	

	//nothing

	
}

BOOL CUpdateLoginTimeCMD::UpdateLoginTime(SGDP::ISDDBConnection* poDBConn)
{
	

		static CHAR szSQL[1024] = {0};
	memset(&szSQL, 0, sizeof(szSQL));

	sprintf(szSQL, "update userzone set LoginTime='%lld' where UserID = '%d' and ZoneID='%d'", SDTimeSecs(), m_dwUserID, m_wZoneID);

	ISDDBRecordSet* pRecordSet = NULL;
	INT32 nRet= poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
	if(NULL != pRecordSet)
	{
		pRecordSet->Release();
		pRecordSet = NULL;
	}	

	return TRUE;
	
		return FALSE;

	return TRUE;
}
