
#include <db/basedbccmd.h>
#include "dbmgr.h"
#include <framework/gsapi.h>

CBaseDBCMD::CBaseDBCMD(): 
m_bSuccess(FALSE)
{
}


CBaseDBCMD::~CBaseDBCMD()
{
}

VOID CBaseDBCMD::Release()
{
	//CAutoCycle oAutoCycle(__FUNCTION__);
    gsapi::GetDBMgr()->FreeCmd(this);
}

VOID CBaseDBCMD::SetUserData(VOID* pUserData, UINT32 dwTransID)
{	
	m_dwTransID = dwTransID;
	SetUserData(pUserData);
}

