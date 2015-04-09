
#include "registerusernamecmd.h"
#include <sdutil.h>





CRegisterUserNameCMD::CRegisterUserNameCMD()
{
	
}


CRegisterUserNameCMD::~CRegisterUserNameCMD()
{
}

VOID CRegisterUserNameCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
	SDTRY_BEGIN

	m_bSuccess = FALSE;

	//todo.....

	SDTRY_END

}

VOID CRegisterUserNameCMD::OnExecuted()
{
	SDTRY_BEGIN
		
	//todo...

	SDTRY_END
}


BOOL CRegisterUserNameCMD::Register(SGDP::ISDDBConnection* poDBConn,
	TCHAR* pszDeviceID,
	TCHAR* pszUserName)
{
	SDTRY_BEGIN
	
	//todo...
	
	return TRUE;
	SDTRY_END
		return FALSE;
}


