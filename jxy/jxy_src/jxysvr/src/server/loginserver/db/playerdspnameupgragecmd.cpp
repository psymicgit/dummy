
#include "framework/lsapplication.h"
#include "playerdspnameupgragecmd.h"
#include <sdutil.h>
#include <logic/dbconfig.h>
#include "net/gt/gtpktbuilder.h"
#include "common/client/errdef.h"





CPlayDspNameUpgradeCMD::CPlayDspNameUpgradeCMD()
{

}


CPlayDspNameUpgradeCMD::~CPlayDspNameUpgradeCMD()
{
}

VOID CPlayDspNameUpgradeCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
	

		m_bSuccess = FALSE;

	PKT_GSLS_DSPNAME_UPGRADE_NTF* pstNtf = (PKT_GSLS_DSPNAME_UPGRADE_NTF*)m_pUserData;
	if(NULL == pstNtf)
		return;
	
	if (FALSE == UpdateDspName(poDBConn, pstNtf->dwPlayerID, pstNtf->aszDspName))		
	{
		m_bSuccess = FALSE;
	}
	else
	{
		m_bSuccess = TRUE;
	}

	

}

VOID CPlayDspNameUpgradeCMD::OnExecuted()
{
	

	//nothing

	
}


BOOL CPlayDspNameUpgradeCMD::UpdateDspName(SGDP::ISDDBConnection* poDBConn,
	UINT32	dwPlayerID,
	TCHAR* pszDspName)
{
	if(NULL == pszDspName)
	{
		return FALSE;
	}

	CHAR szSQL[1024] = {0};

	//更新用户信息
	sprintf(szSQL,  "update userzone set PlayerDspName='%s' where PlayerID = '%d';", pszDspName, dwPlayerID);
	UINT32 dwErr;
	string strError;
	INT32 nRet = poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
	if(nRet < 0)
	{
		//异常
		SYS_CRITICAL(_SDT("[%s: %d]: ExecuteSqlRs[%s] failed[%s]!"), MSG_MARK, szSQL, strError.c_str());    
		return FALSE;
	}


	return TRUE;
}


