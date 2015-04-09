
#include "framework/zsapplication.h"
#include "unbinddevice.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>



CUnBindDeviceCMD::CUnBindDeviceCMD()
{
    memset(&m_stAck, 0, sizeof(m_stAck));
}


CUnBindDeviceCMD::~CUnBindDeviceCMD()
{
}

VOID CUnBindDeviceCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{

    m_bSuccess = FALSE;
    PKT_CLIZS_UN_BIND_DEVICE_REQ* pstReq = (PKT_CLIZS_UN_BIND_DEVICE_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;
    UnBind(poDBConn, pstReq);
}

VOID CUnBindDeviceCMD::OnExecuted()
{
    ResponseMsg(CLIZS_UN_BIND_DEVICE_ACK, (CHAR*)(&m_stAck));
}


BOOL CUnBindDeviceCMD::UnBind(SGDP::ISDDBConnection* poDBConn, PKT_CLIZS_UN_BIND_DEVICE_REQ* pstReq)
{
    //用户名密码为空
    if(0 == _SDTStrcasecmp(pstReq->aszUserName, _SDT("")))
    {
        m_stAck.wErrCode = ERR_UNBIND_DEVICE::ID_USERNAME_EMPTY_ERR;
        return FALSE;
    }

    //检查参数
    if(HaveSqlToken(pstReq->aszDeviceID) || 
		HaveSqlToken(pstReq->aszUserName))
    {
        m_stAck.wErrCode =  ERR_UNBIND_DEVICE::ID_INVALID_CHARACTER;
        return FALSE;
    }

	//字符过虑
	TCHAR       aszUserName[USERNAME_LEN] = {0}; //用户名
	MisplacedToken(pstReq->aszUserName, aszUserName);

    CHAR		szSQL[1024] = {0};
    CHAR		szErrMsg[128] = {0};
    sprintf(szSQL, "call UnBindDeviceIDbyUserName(%u, '%s', '%s', @ErrCode, @ErrMsg);", pstReq->byAuthType, pstReq->aszDeviceID, aszUserName);

    SYS_CRITICAL(_SDT("%s"), szSQL);

    UINT32	dwErr	=	0;
    string	strError;
    INT32	nRet		=	poDBConn->ExecuteSql(szSQL, NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        m_stAck.wErrCode = ERR_COMMON::ID_OTHER_ERR;
        SYS_CRITICAL(_SDT("Exec SQL Faild:Errno:%d, Errstr:%s, SQL:%s"), dwErr, strError.c_str(), szSQL);
        return FALSE;
    }
    else
    {
        sprintf(szSQL, "select @ErrCode, @ErrMsg;");
    }

    SYS_CRITICAL(_SDT("%s"), szSQL);
    ISDDBRecordSet* pRecordSet = NULL;
    nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        UINT32 m_wErrCode = SDAtou(pRecordSet->GetFieldValueByName("@ErrCode"));
        SDStrncpy(szErrMsg, pRecordSet->GetFieldValueByName("@ErrMsg"), 128);
        switch(m_wErrCode)
        {
        case 0:
            m_stAck.wErrCode = ERR_UNBIND_DEVICE::ID_SUCCESS;
            break;
        case 1:
            m_stAck.wErrCode = ERR_UNBIND_DEVICE::ID_UNBIND_ACCOUNT;
            break;
        default:
            m_stAck.wErrCode = ERR_COMMON::ID_OTHER_ERR;
            break;
        }
    }
    else
    {
        m_stAck.wErrCode = ERR_COMMON::ID_OTHER_ERR;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}
