#include "sdutil.h"
#include "net/gt/gtprocessor.h"
#include "framework/lsapplication.h"
#include "sdframework/sdpipechannel.h"
#include "db/basedbccmd.h"
#include "db/logincmd.h"
#include "db/registerusernamecmd.h"
#include "db/playerlevelupgragecmd.h"
#include "net/tlc/authusermgr.h"

CGTProcessor::CGTProcessor()
{
    Init();
}

CGTProcessor::~CGTProcessor()
{
}



CSDProtocol* CGTProcessor::GetProtocol()
{
    return CProtoGTLS::Instance() ;
}

BOOL CGTProcessor::Init()
{
    RegisterCommand(GTLS_LOGIN_REQ, CGTProcessor::OnLoginReq);
    RegisterCommand(GTLS_LOGIN_EX_REQ, CGTProcessor::OnLoginExReq);

    RegisterCommand(CLILS_REGISTER_REQ, CGTProcessor::OnRegisterUserNameReq);
    RegisterCommand(CLILS_BIND_REQ, CGTProcessor::OnBindReq);
    RegisterCommand(CLILS_UNBIND_REQ, CGTProcessor::OnUnBindReq);

    RegisterCommand(GSLS_PLAYER_UPGRADE_NTF, CGTProcessor::OnPlayerUpgrate);
    RegisterCommand(GSLS_DSPNAME_UPGRADE_NTF, CGTProcessor::OnDspNameUpgrate);

    RegisterCommand(CLILS_LOGOUT_REQ, CGTProcessor::OnLogoutReq);

    RegisterCommand(GSLS_ACTIVITY_NOTIFY_NTF, CGTProcessor::OnActivityNotify);
    RegisterCommand(GTLS_CHANGE_PWD_REQ, CGTProcessor::OnChangePwdReq);

    RegisterCommand(CLILS_BIND_MOBILE_REQ, CGTProcessor::OnBindMobileReq);
    RegisterCommand(CLILS_BIND_CHECK_VER_CODE_REQ, CGTProcessor::OnBindCheckVerCodeReq);

    RegisterCommand(CLILS_UNBIND_MOBILE_REQ, CGTProcessor::OnUnBindMobileReq);
    RegisterCommand(CLILS_UNBIND_CHECK_VER_CODE_REQ, CGTProcessor::OnUnBindCheckVerCodeReq);

	RegisterCommand(CLILS_GET_USER_EXPAND_REQ, CGTProcessor::OnGetUserExpandReq);
    return TRUE;
}


BOOL CGTProcessor::OnLoginReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_LOGIN);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);

    return TRUE;
}

BOOL CGTProcessor::OnLoginExReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_LOGIN_EX);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);

    return TRUE;
}


/*
BOOL CGTProcessor::OnLoginReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	PKT_GTLS_LOGIN_REQ* pstReq = (PKT_GTLS_LOGIN_REQ*)pszBody;
	if((AUTH_TYPE_DEVECEID == pstReq->byAuthType) || (AUTH_TYPE_TONGBU == pstReq->byAuthType)) //设备号不需认真，同步已在GT认证
	{
		CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_LOGIN);
		if(NULL == poCMD)
		{
			return FALSE;
		}
		poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
		return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
	}
	CAuthUserMgr::Instance()->ReqAuth(pPipeHeader->dwTransID, pstReq, NULL);
	
	return TRUE;
}

BOOL CGTProcessor::OnLoginExReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	PKT_GTLS_LOGIN_EX_REQ* pstReq = (PKT_GTLS_LOGIN_EX_REQ*)pszBody;
	if((AUTH_TYPE_DEVECEID == pstReq->byAuthType) || (AUTH_TYPE_TONGBU == pstReq->byAuthType)) //设备号不需认证，同步已在GT认证
	{
		CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_LOGIN_EX);
		if(NULL == poCMD)
		{
			return FALSE;
		}
		poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
		return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
	}
	CAuthUserMgr::Instance()->ReqAuth(pPipeHeader->dwTransID, NULL, pstReq);

	return TRUE;
}
*/

BOOL CGTProcessor::OnRegisterUserNameReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_REGNAME);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

BOOL CGTProcessor::OnPlayerUpgrate(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;

    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_LVLUP);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

BOOL CGTProcessor::OnDspNameUpgrate(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;

    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_DSPNAMEUP);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

BOOL	CGTProcessor::OnBindReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_BIND);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

BOOL	CGTProcessor::OnUnBindReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_UNBIND);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

BOOL CGTProcessor::OnLogoutReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_LOGOUT);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

BOOL	CGTProcessor::OnActivityNotify(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_ACTIVITY_NOTIFY);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

BOOL	CGTProcessor::OnChangePwdReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_CHANGE_PWD);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

BOOL	CGTProcessor::OnBindMobileReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_BINDMOBILE);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

BOOL	CGTProcessor::OnBindCheckVerCodeReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_BINDCHECKVERCODE);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}


BOOL	CGTProcessor::OnUnBindMobileReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_UNBINDMOBILE);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

BOOL	CGTProcessor::OnUnBindCheckVerCodeReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_UNBINDCHECKVERCODE);
    if(NULL == poCMD)
    {
        return FALSE;
    }
    poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
    return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

BOOL	CGTProcessor::OnGetUserExpandReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_GETUSEREXPAND);
	if(NULL == poCMD)
	{
		return FALSE;
	}
	poCMD->SetUserData((VOID*)pszBody, dwLen, pPipeHeader->dwTransID);
	return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
}

