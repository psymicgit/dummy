
#include "framework/lsapplication.h"
#include "playerlevelupgragecmd.h"
#include <sdutil.h>
#include <logic/dbconfig.h>
#include "net/gt/gtpktbuilder.h"
#include "common/client/errdef.h"





CPlayUpgrateCMD::CPlayUpgrateCMD()
{

}


CPlayUpgrateCMD::~CPlayUpgrateCMD()
{
}

VOID CPlayUpgrateCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
	

		m_bSuccess = FALSE;

	PKT_GSLS_PLAYER_UPGRADE_NTF* pstNtf = (PKT_GSLS_PLAYER_UPGRADE_NTF*)m_pUserData;
	if(NULL == pstNtf)
		return;
	
	if (FALSE == UpdateLevel(poDBConn, pstNtf->dwPlayerID, pstNtf->wLevel))		
	{
		m_bSuccess = FALSE;
	}
	else
	{
		m_bSuccess = TRUE;
	}

	

}

VOID CPlayUpgrateCMD::OnExecuted()
{
	

	//nothing

	
}


BOOL CPlayUpgrateCMD::UpdateLevel(SGDP::ISDDBConnection* poDBConn,
	UINT32	dwPlayerID,
	UINT16 wPlayerLevel)
{
	

	CHAR szSQL[1024] = {0};

	//更新用户信息
	sprintf(szSQL,  "update userzone set PlayerLevel='%d' where PlayerID = '%d';", wPlayerLevel, dwPlayerID);
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


