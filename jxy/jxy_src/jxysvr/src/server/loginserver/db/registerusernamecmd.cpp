
#include "framework/lsapplication.h"
#include "registerusernamecmd.h"
#include <sdutil.h>
#include <logic/dbconfig.h>
#include "net/gt/gtpktbuilder.h"
#include "common/client/errdef.h"
#include "common/server/utility.h"





CRegisterUserNameCMD::CRegisterUserNameCMD()
{

}


CRegisterUserNameCMD::~CRegisterUserNameCMD()
{

}

VOID CRegisterUserNameCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    PKT_CLILS_REGISTER_REQ* pstReq = (PKT_CLILS_REGISTER_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;

    if (FALSE == UserRegister(poDBConn, pstReq->dwUserID, pstReq->aszDeviceID, pstReq->aszUserName, pstReq->aszUserPwd))
    {
        m_bSuccess = FALSE;
    }
    else
    {
        m_bSuccess = TRUE;
    }
}


VOID CRegisterUserNameCMD::OnExecuted()
{
    PKT_CLILS_REGISTER_REQ* pstReq = (PKT_CLILS_REGISTER_REQ*)m_pUserData;
    if(NULL == pstReq)
        return;

    UINT16 wMsgID = CGTPktBuilder::Instance()->RegisterAck(m_wErrCode, pstReq->aszUserName);
    ResponseMsg(wMsgID, CGTPktBuilder::Instance()->GetPacketBuffer());
}


BOOL CRegisterUserNameCMD::UserRegister(SGDP::ISDDBConnection* poDBConn,
                                    UINT32 dwUserID,
									TCHAR* pszDeviceID,
                                    TCHAR* pszUserName,
                                    TCHAR* pszUserPwd)
{
    //用户名密码为空
    if(0 == _SDTStrcasecmp(pszUserName, _SDT("")))
    {
        m_wErrCode = ERR_REGISTER_USERNAME::ID_USERNAME_EMPTY_ERR;
        return FALSE;
    }
    if(0 == _SDTStrcasecmp(pszUserPwd, _SDT("")))
    {
        m_wErrCode = ERR_REGISTER_USERNAME::ID_PASSWORD_EMPTY_ERR;
        return FALSE;
    }

    //检查参数
    if(HaveSqlToken(pszUserName) || HaveSqlToken(pszUserPwd))
    {
        m_wErrCode =  ERR_REGISTER_USERNAME::ID_INVALID_CHARACTER;
        return FALSE;
    }


    CHAR szSQL[256] = {0};
    //memset(&szSQL, 0, sizeof(szSQL));
    CHAR szErrMsg[128] = {0};
    sprintf(szSQL, "call UserRegister(%u, '%s', '%s', '%s', @ErrCode, @ErrMsg);", dwUserID, pszDeviceID, pszUserName, pszUserPwd);

    INT32 nRet = poDBConn->ExecuteSql(szSQL);
    if(nRet < 0)
    {
        SET_OTHER_ERR(m_wErrCode);
        return FALSE;
    }
    else
    {
        sprintf(szSQL, "select @ErrCode, @ErrMsg;");
    }

    ISDDBRecordSet* pRecordSet = NULL;
    nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        m_wErrCode = SDAtou(pRecordSet->GetFieldValueByName("@ErrCode"));
        SDStrcpy(szErrMsg, pRecordSet->GetFieldValueByName("@ErrMsg"));
        switch(m_wErrCode)
        {
        case 0:
            m_wErrCode = ERR_REGISTER_USERNAME::ID_SUCCESS;
            break;
        case 1:
            m_wErrCode = ERR_REGISTER_USERNAME::ID_USER_NOT_EXIST_ERR;
            break;
        case 2:
            m_wErrCode = ERR_REGISTER_USERNAME::ID_REGISTERED_ERR;
            break;
        case 3:
            m_wErrCode = ERR_REGISTER_USERNAME::ID_USERNAME_EXIST_ERR;
            break;
        default:
            SET_OTHER_ERR(m_wErrCode);
            break;
        }
    }
    else
    {
        SET_OTHER_ERR(m_wErrCode);
    }

    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return FALSE;
}


