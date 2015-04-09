
#include "authuser.h"
#include "framework/lsapplication.h"
#include <sdutil.h>
#include "db/basedbccmd.h"
#include <common/client/errdef.h>

#define MAX_TIME_OUT	3600


CAuthUser::CAuthUser()
{
	m_dwTransID = 0;
	memset(&m_stReq1, 0, sizeof(m_stReq1));
	memset(&m_stReq2, 0, sizeof(m_stReq2));
	m_qwLastRecvMsgTime = SDTimeSecs();
	m_byProtVerID = 0;
}


CAuthUser::~CAuthUser()
{

}
BOOL CAuthUser::SetUserProp(UINT32 dwTransID, PKT_GTLS_LOGIN_REQ* pstReq1, PKT_GTLS_LOGIN_EX_REQ* pstReq2)
{
	m_dwTransID = dwTransID;
	if(pstReq1)
	{
		m_stReq1 = *pstReq1;
		m_byProtVerID = 1;
	}
	else if(pstReq2)
	{
		m_stReq2 = *pstReq2;
		m_byProtVerID = 2;
	}

	m_qwLastRecvMsgTime = SDTimeSecs();

	return TRUE;
}


BOOL CAuthUser::ReqAuth()
{
	CSDPipeChannel* pPipeSink = ((CLSApplication*)SDGetApp())->GetTLCServer();
	if (NULL == pPipeSink)
	{
		return FALSE;		
	}

	PKT_LSTLC_AUTH_REQ stReq;
	if(1 == m_byProtVerID)
	{
		USR_INFO(_SDT("[%s: %d]:ReqAuth DeviceID[%s], UserName[%s], AuthType[%d], AuthMethod[%d]"), MSG_MARK, m_stReq1.aszDeviceID, m_stReq1.aszUserName, m_stReq1.byAuthType, m_stReq1.byAuthMethod);

		stReq.byAuthType = m_stReq1.byAuthType;
		SDStrcpy(stReq.aszDeviceID, m_stReq1.aszDeviceID);
		SDStrcpy(stReq.aszUserName, m_stReq1.aszUserName);
		stReq.byPwdLen = m_stReq1.byPwdLen;
		memcpy(stReq.abyUserPwd, m_stReq1.abyUserPwd, USERNAME_LEN);
		stReq.byAuthMethod = m_stReq1.byAuthMethod;
	}
	else if(2 == m_byProtVerID)
	{
		USR_INFO(_SDT("[%s: %d]:ReqAuth DeviceID[%s], UserName[%s], AuthType[%d], AuthMethod[%d]"), MSG_MARK, m_stReq2.aszDeviceID, m_stReq2.aszUserName, m_stReq2.byAuthType, m_stReq2.byAuthMethod);

		stReq.byAuthType = m_stReq1.byAuthType;
		SDStrcpy(stReq.aszDeviceID, m_stReq1.aszDeviceID);
		SDStrcpy(stReq.aszUserName, m_stReq1.aszUserName);
		stReq.byPwdLen = m_stReq1.byPwdLen;
		memcpy(stReq.abyUserPwd, m_stReq1.abyUserPwd, USERNAME_LEN);
		stReq.byAuthMethod = m_stReq1.byAuthMethod;
	}
	pPipeSink->SendMsg(m_dwTransID, LSTLC_AUTH_REQ, (const CHAR*)&stReq);

	return TRUE;
}

UINT16 CAuthUser::Convert2CliErrCode(UINT16 wErrCode)
{
	/*
		EAEC_SUCCESS = 0,  //成功
			EAEC_INVALID_CHART = 1, //包含无效字符
			EAEC_NOTMATCH = 2, //用户名密码错误
			EAEC_TIMEOUT = 3, //认证超时
			EAEC_INVALID_AUTHTYPE = 4, //无效认证类型
			EAEC_INNERR = 5, //内部错误
			*/	
	switch(wErrCode)
	{
	case EAEC_INVALID_CHART:
		return ERR_LOGIN::ID_INVALID_CHARACTER;
		break;
	case EAEC_NOTMATCH:
		return ERR_LOGIN::ID_PWD_ERR;
		break;
	case EAEC_TIMEOUT:
		return ERR_LOGIN::ID_INVALID_CHARACTER;
		break;
	case EAEC_INVALID_AUTHTYPE:
		return ERR_LOGIN::ID_INVALID_CHARACTER;
		break;
	case EAEC_INNERR:
		return ERR_LOGIN::ID_INVALID_CHARACTER;
		break;
	default:
		return ERR_COMMON::ID_OTHER_ERR;
		break;
	}

	return ERR_COMMON::ID_OTHER_ERR;
}

BOOL CAuthUser::OnAuthAck(UINT16 wErrCode)
{
	CSDPipeChannel* pPipeSink = ((CLSApplication*)SDGetApp())->GetGTServer();
	if(EAEC_SUCCESS != wErrCode)
	{		
		wErrCode = Convert2CliErrCode(wErrCode);
		if(pPipeSink)
		{
			if(1 == m_byProtVerID)
			{
				USR_INFO(_SDT("[%s: %d]:OnAuthAck DeviceID[%s], UserName[%s], AuthType[%d], AuthMethod[%d], ErrCode[%d]"), MSG_MARK, m_stReq1.aszDeviceID, m_stReq1.aszUserName, m_stReq1.byAuthType, m_stReq1.byAuthMethod, wErrCode);
				PKT_GTLS_LOGIN_ACK stAck;
				stAck.wErrCode = wErrCode;
				pPipeSink->SendMsg(m_dwTransID, GTLS_LOGIN_ACK, (const CHAR*)&stAck);
			}
			else if(2 == m_byProtVerID)
			{
				USR_INFO(_SDT("[%s: %d]:OnAuthAck DeviceID[%s], UserName[%s], AuthType[%d], AuthMethod[%d], ErrCode[%d]"), MSG_MARK, m_stReq2.aszDeviceID, m_stReq2.aszUserName, m_stReq2.byAuthType, m_stReq2.byAuthMethod, wErrCode);
				PKT_CLILS_LOGIN_EX_ACK stAck;
				stAck.wErrCode = wErrCode;
				pPipeSink->SendMsg(m_dwTransID, CLILS_LOGIN_EX_REQ, (const CHAR*)&stAck);
			}			
		}
		return TRUE;
	}

	if(1 == m_byProtVerID)
	{
		CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_LOGIN);
		if(NULL == poCMD)
		{
			return FALSE;
		}
		poCMD->SetUserData((VOID*)&m_stReq1, sizeof(m_stReq1), m_dwTransID);
		return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
	}
	else
	{
		CBaseDBCMD* poCMD = ((CLSApplication*)SDGetApp())->GetDBMgr()->CreateCmd(EDBCT_LOGIN_EX);
		if(NULL == poCMD)
		{
			return FALSE;
		}
		poCMD->SetUserData((VOID*)&m_stReq2, sizeof(m_stReq2), m_dwTransID);
		return ((CLSApplication*)SDGetApp())->GetDBMgr()->AddCommand(poCMD);
	}
}

