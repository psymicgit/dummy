#include "cliprocessor.h"
#include "sdutil.h"
#include "framework/zsapplication.h"
#include "protocol/server/protoclizs.h"
#include "logic/getzoneinfocmd.h"
#include "logic/selectzone.h"
#include "logic/getversionnoticecmd.h"
#include <common/client/errdef.h>
#include <logic/dbmgr.h>
#include "clipktbuilder.h"
#include "common/client/commondef.h"
#include "http/httpmgr.h"
#include "http/basehttpcmd.h"


CCliProcessor::CCliProcessor()
{
    Init();
}

CCliProcessor::~CCliProcessor()
{
}

BOOL CCliProcessor::Init()
{
    RegisterCommand(CLIZS_GET_ZONE_INFO_REQ, CCliProcessor::OnGetZoneInfoReq);
    RegisterCommand(CLIGT_SELECT_ZONE_REQ, CCliProcessor::OnSelectZoneInfoReq);
    RegisterCommand(CLIZS_GET_VERSION_NOTICE_REQ, CCliProcessor::OnGetVersionNoticReq);
    RegisterCommand(CLIZS_GET_VERSION_NOTICE_REQ2, CCliProcessor::OnGetVersionNoticReq2);
    RegisterCommand(CLIZS_GET_USERINFO_REQ, CCliProcessor::OnGetUserInfoReq);
    RegisterCommand(CLIZS_REGISTER_USERNAME_REQ, CCliProcessor::OnRegisterUserNameReq);
    RegisterCommand(CLIGT_SELECT_ZONE_EX_REQ, CCliProcessor::OnSelectZoneInfoExReq);
    RegisterCommand(CLIZS_REPORT_USER_DEVICE_INFO_REQ, CCliProcessor::OnReportUserDeviceInfoReq);
    RegisterCommand(CLIGT_SELECT_ZONE_EX2_REQ, CCliProcessor::OnSelectZoneInfoEx2Req);
    RegisterCommand(CLIZS_UN_BIND_DEVICE_REQ, CCliProcessor::OnUnBindDeviceReq);
    RegisterCommand(CLIZS_RESET_PASSWD_MOBILE_REQ, CCliProcessor::OnResetPasswdMobileReq);
    RegisterCommand(CLIZS_RESET_PASSWD_VER_CODE_REQ, CCliProcessor::OnResetPasswdVerCodeReq);
    RegisterCommand(CLIZS_RESET_PASSWD_NEWPASSWD_REQ, CCliProcessor::OnResetPasswdNewPasswdReq);
    return TRUE;
}


CSDProtocol* CCliProcessor::GetProtocol()
{
    return CProtoCliZS::Instance();
}

BOOL CCliProcessor::OnGetZoneInfoReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    return TRUE;
}

BOOL CCliProcessor::OnSelectZoneInfoReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CZSClient* poZSClient = (CZSClient*)poReceiver;

    PKT_CLIGT_SELECT_ZONE_REQ* pstReq = (PKT_CLIGT_SELECT_ZONE_REQ*)pszBody;
    if(AUTH_TYPE_DEVECEID != pstReq->byAuthType)
    {
        UINT8* pszEncryptBuff =  (UINT8*)(pstReq->abyUserPwd);
        INT32 nEncryptBuffLen = pstReq->byPwdLen;
        if(nEncryptBuffLen > (ENCRYPT_HEAD_LEN + ENCRYPT_TAIL_LEN))
        {
            if(!poZSClient->GetMGEncrypt().decryptionDatas(pszEncryptBuff, nEncryptBuffLen))
            {
                //返回错误消息

                PKT_CLIGT_SELECT_ZONE_ACK *pstAck = (PKT_CLIGT_SELECT_ZONE_ACK*)CCliPktBuilder::Instance()->GetPacketBuffer();
                if (NULL == pstAck)
                {
                    return FALSE;
                }
                memset(pstAck, 0x00, sizeof(PKT_CLIGT_SELECT_ZONE_ACK));
                pstAck->wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
                poZSClient->SendMsg(CLIGT_SELECT_ZONE_ACK, (CHAR*)(pstAck));
                return FALSE;
            }
            UINT32	dwLen = 0;
            TCHAR	aszUserPwd[USERNAME_LEN] = {0};
            dwLen	= nEncryptBuffLen - ENCRYPT_HEAD_LEN - ENCRYPT_TAIL_LEN;

            SDStrncpy(aszUserPwd, (CHAR*)(pszEncryptBuff + ENCRYPT_HEAD_LEN),  dwLen);

            pstReq->byPwdLen = dwLen;
            memset(pstReq->abyUserPwd, 0x00, USERNAME_LEN);
            memcpy(pstReq->abyUserPwd, aszUserPwd, dwLen);

            //快速进入不验证密码，只验证用户跟密码是否一至
            if (1 == pstReq->byAuthMethod)
            {
                if ( 0 != SDStrcmp(pstReq->aszUserName, aszUserPwd))
                {
                    //返回错误消息
                    PKT_CLIGT_SELECT_ZONE_ACK *pstAck = (PKT_CLIGT_SELECT_ZONE_ACK*)CCliPktBuilder::Instance()->GetPacketBuffer();
                    if (NULL == pstAck)
                    {
                        return FALSE;
                    }
                    memset(pstAck, 0x00, sizeof(PKT_CLIGT_SELECT_ZONE_ACK));
                    pstAck->wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
                    poZSClient->SendMsg(CLIGT_SELECT_ZONE_ACK, (CHAR*)(pstAck));
                    return FALSE;
                }
            }
            else
            {
                //同步网络上来解码完后，应该用户名跟密码是一至的。
                if (AUTH_TYPE_TONGBU == pstReq->byAuthType)
                {
                    if(0 != SDStrcmp(pstReq->aszUserName, aszUserPwd))
                    {
                        //返回错误消息
                        PKT_CLIGT_SELECT_ZONE_ACK *pstAck = (PKT_CLIGT_SELECT_ZONE_ACK*)CCliPktBuilder::Instance()->GetPacketBuffer();
                        if (NULL == pstAck)
                        {
                            return FALSE;
                        }
                        memset(pstAck, 0x00, sizeof(PKT_CLIGT_SELECT_ZONE_ACK));
                        pstAck->wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
                        poZSClient->SendMsg(CLIGT_SELECT_ZONE_ACK, (CHAR*)(pstAck));
                        return FALSE;
                    }
                }
                else if (AUTH_TYPE_91 == pstReq->byAuthType)
                {
                    if(0 != SDStrcmp(pstReq->aszUserName, aszUserPwd))
                    {
                        //返回错误消息
                        PKT_CLIGT_SELECT_ZONE_ACK *pstAck = (PKT_CLIGT_SELECT_ZONE_ACK*)CCliPktBuilder::Instance()->GetPacketBuffer();
                        if (NULL == pstAck)
                        {
                            return FALSE;
                        }
                        memset(pstAck, 0x00, sizeof(PKT_CLIGT_SELECT_ZONE_ACK));
                        pstAck->wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
                        poZSClient->SendMsg(CLIGT_SELECT_ZONE_ACK, (CHAR*)(pstAck));
                        return FALSE;
                    }
                }
                else if (AUTH_TYPE_PP == pstReq->byAuthType)
                {
                    if(0 != SDStrcmp(pstReq->aszUserName, aszUserPwd))
                    {
                        //返回错误消息
                        PKT_CLIGT_SELECT_ZONE_ACK *pstAck = (PKT_CLIGT_SELECT_ZONE_ACK*)CCliPktBuilder::Instance()->GetPacketBuffer();
                        if (NULL == pstAck)
                        {
                            return FALSE;
                        }
                        memset(pstAck, 0x00, sizeof(PKT_CLIGT_SELECT_ZONE_ACK));
                        pstAck->wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
                        poZSClient->SendMsg(CLIGT_SELECT_ZONE_ACK, (CHAR*)(pstAck));
                        return FALSE;
                    }
                }
            }
        }
        else
        {
            //返回错误消息
            PKT_CLIGT_SELECT_ZONE_ACK *pstAck = (PKT_CLIGT_SELECT_ZONE_ACK*)CCliPktBuilder::Instance()->GetPacketBuffer();
            if (NULL == pstAck)
            {
                return FALSE;
            }
            memset(pstAck, 0x00, sizeof(PKT_CLIGT_SELECT_ZONE_ACK));
            pstAck->wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
            poZSClient->SendMsg(CLIGT_SELECT_ZONE_ACK, (CHAR*)(pstAck));
            return FALSE;
        }
    }


    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_SELECTZONE);
    if(NULL == poHttpCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
        return FALSE;
    }

    poHttpCmd->SetUserData((void*)pszBody, sizeof(PKT_CLIGT_SELECT_ZONE_REQ), poZSClient->GetCliSessionID());
    poHttpCmd->Init();

    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        return FALSE;
    }
    return TRUE;

}

BOOL CCliProcessor::OnGetVersionNoticReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CZSClient* poZSClient = (CZSClient*)poReceiver;

    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_GET_VERSION_NOTICE);
    if(NULL == poHttpCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
        return FALSE;
    }

    poHttpCmd->SetUserData((void*)pszBody, sizeof(PKT_CLIZS_GET_VERSION_NOTICE_REQ), poZSClient->GetCliSessionID());
    poHttpCmd->Init();

    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        return FALSE;
    }
    return TRUE;
}

BOOL CCliProcessor::OnGetVersionNoticReq2(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{

    SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CZSClient*		poZSClient = (CZSClient*)poReceiver;

    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_GET_VERSION_NOTICE);
    if(NULL == poHttpCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
        return FALSE;
    }

    poHttpCmd->SetUserData((void*)pszBody, sizeof(PKT_CLIZS_GET_VERSION_NOTICE_REQ), poZSClient->GetCliSessionID());
    poHttpCmd->Init();

    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        return FALSE;
    }
    return TRUE;
}


BOOL CCliProcessor::OnGetUserInfoReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{

    SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CZSClient*		poZSClient = (CZSClient*)poReceiver;

    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_GET_USER_INFO);
    if(NULL == poHttpCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
        return FALSE;
    }

    poHttpCmd->SetUserData((void*)pszBody, sizeof(PKT_CLIZS_GET_USERINFO_REQ), poZSClient->GetCliSessionID());
    poHttpCmd->Init();

    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        return FALSE;
    }
    return TRUE;
}

BOOL CCliProcessor::OnRegisterUserNameReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader*	pPipeHeader	= (SDPipeMsgHeader*)pHeader;
    CZSClient*		poZSClient	= (CZSClient*)poReceiver;

    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_REGISTER_USERNAME);
    if(NULL == poHttpCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
        return FALSE;
    }

    poHttpCmd->SetUserData((void*)pszBody, sizeof(PKT_CLIZS_REGISTER_USERNAME_REQ), poZSClient->GetCliSessionID());
    poHttpCmd->Init();

    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        return FALSE;
    }
    return TRUE;
}

BOOL CCliProcessor::OnSelectZoneInfoExReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CZSClient*		poZSClient = (CZSClient*)poReceiver;

    PKT_CLIGT_SELECT_ZONE_EX_REQ* pstReq = (PKT_CLIGT_SELECT_ZONE_EX_REQ*)pszBody;
    if(AUTH_TYPE_DEVECEID != pstReq->byAuthType)
    {
        UINT8* pszEncryptBuff =  (UINT8*)(pstReq->abyUserPwd);
        INT32 nEncryptBuffLen = pstReq->byPwdLen;
        if(nEncryptBuffLen > (ENCRYPT_HEAD_LEN + ENCRYPT_TAIL_LEN))
        {
            if(!poZSClient->GetMGEncrypt().decryptionDatas(pszEncryptBuff, nEncryptBuffLen))
            {
                //返回错误消息
                PKT_CLIGT_SELECT_ZONE_EX_ACK stAck;
                stAck.wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
                poZSClient->SendMsg(CLIGT_SELECT_ZONE_EX_ACK, (CHAR*)(&stAck));
                return FALSE;
            }
            UINT32	dwLen = 0;
            TCHAR	aszUserPwd[USERNAME_LEN] = {0};
            dwLen	= nEncryptBuffLen - ENCRYPT_HEAD_LEN - ENCRYPT_TAIL_LEN;

            SDStrncpy(aszUserPwd, (CHAR*)(pszEncryptBuff + ENCRYPT_HEAD_LEN),  dwLen);

            pstReq->byPwdLen = dwLen;
            memset(pstReq->abyUserPwd, 0x00, USERNAME_LEN);
            memcpy(pstReq->abyUserPwd, aszUserPwd, dwLen);

            //快速进入不验证密码，只验证用户跟密码是否一到
            if (1 == pstReq->byAuthMethod)
            {
                if ( 0 != SDStrcmp(pstReq->aszUserName, aszUserPwd))
                {
                    //返回错误消息
                    PKT_CLIGT_SELECT_ZONE_EX_ACK stAck;
                    stAck.wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
                    poZSClient->SendMsg(CLIGT_SELECT_ZONE_EX_ACK, (CHAR*)(&stAck));
                    return FALSE;
                }
            }
            else
            {
                //同步网络上来解码完后，应该用户名跟密码是一至的。
                if (AUTH_TYPE_TONGBU == pstReq->byAuthType || AUTH_TYPE_91 == pstReq->byAuthType)
                {
                    if(0 != SDStrcmp(pstReq->aszUserName, aszUserPwd))
                    {
                        //返回错误消息
                        PKT_CLIGT_SELECT_ZONE_EX_ACK stAck;
                        stAck.wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
                        poZSClient->SendMsg(CLIGT_SELECT_ZONE_EX_ACK, (CHAR*)(&stAck));
                        return FALSE;
                    }
                }
            }
        }
        else
        {
            //返回错误消息
            PKT_CLIGT_SELECT_ZONE_EX_ACK stAck;
            stAck.wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
            poZSClient->SendMsg(CLIGT_SELECT_ZONE_EX_ACK, (CHAR*)(&stAck));
            return FALSE;
        }
    }

    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_SELECTZONE);
    if(NULL == poHttpCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
        return FALSE;
    }

    poHttpCmd->SetUserData((void*)pszBody, sizeof(PKT_CLIGT_SELECT_ZONE_EX_REQ), poZSClient->GetCliSessionID());
    poHttpCmd->Init();

    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        return FALSE;
    }

    return TRUE;
}

BOOL CCliProcessor::OnReportUserDeviceInfoReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CZSClient*		poZSClient = (CZSClient*)poReceiver;

    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_REPORTUSERDEVICEINFO);
    if(NULL == poHttpCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
        return FALSE;
    }

    poHttpCmd->SetUserData((void*)pszBody, sizeof(PKT_CLIZS_REPORT_USER_DEVICE_INFO_REQ), poZSClient->GetCliSessionID());
    poHttpCmd->Init();

    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        return FALSE;
    }
    return TRUE;

}

BOOL CCliProcessor::OnUnBindDeviceReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CZSClient*		poZSClient = (CZSClient*)poReceiver;

    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_UNBINDDEVICE);
    if(NULL == poHttpCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
        return FALSE;
    }

    poHttpCmd->SetUserData((void*)pszBody, sizeof(PKT_CLIZS_GET_USERINFO_REQ), poZSClient->GetCliSessionID());
    poHttpCmd->Init();

    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        return FALSE;
    }
    return TRUE;

}

BOOL CCliProcessor::OnSelectZoneInfoEx2Req(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CZSClient* poZSClient = (CZSClient*)poReceiver;

    PKT_CLIGT_SELECT_ZONE_EX2_REQ* pstReq = (PKT_CLIGT_SELECT_ZONE_EX2_REQ*)pszBody;
    if(AUTH_TYPE_DEVECEID != pstReq->byAuthType)
    {
        UINT8* pszEncryptBuff =  (UINT8*)(pstReq->abyUserPwd);
        INT32 nEncryptBuffLen = pstReq->byPwdLen;
        if(nEncryptBuffLen > (ENCRYPT_HEAD_LEN + ENCRYPT_TAIL_LEN))
        {
            if(!poZSClient->GetMGEncrypt().decryptionDatas(pszEncryptBuff, nEncryptBuffLen))
            {
                //返回错误消息

                PKT_CLIGT_SELECT_ZONE_EX2_ACK *pstAck = (PKT_CLIGT_SELECT_ZONE_EX2_ACK*)CCliPktBuilder::Instance()->GetPacketBuffer();
                if (NULL == pstAck)
                {
                    return FALSE;
                }
                memset(pstAck, 0x00, sizeof(PKT_CLIGT_SELECT_ZONE_EX2_ACK));
                pstAck->wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
                poZSClient->SendMsg(CLIGT_SELECT_ZONE_EX2_ACK, (CHAR*)(pstAck));
                return FALSE;
            }
            UINT32	dwLen = 0;
            TCHAR	aszUserPwd[USERNAME_LEN] = {0};
            dwLen	= nEncryptBuffLen - ENCRYPT_HEAD_LEN - ENCRYPT_TAIL_LEN;

            SDStrncpy(aszUserPwd, (CHAR*)(pszEncryptBuff + ENCRYPT_HEAD_LEN),  dwLen);

            pstReq->byPwdLen = dwLen;
            memset(pstReq->abyUserPwd, 0x00, USERNAME_LEN);
            memcpy(pstReq->abyUserPwd, aszUserPwd, dwLen);

            //快速进入不验证密码，只验证用户跟密码是否一至
            if (1 == pstReq->byAuthMethod)
            {
                if ( 0 != SDStrcmp(pstReq->aszUserName, aszUserPwd))
                {
                    //返回错误消息
                    PKT_CLIGT_SELECT_ZONE_EX2_ACK *pstAck = (PKT_CLIGT_SELECT_ZONE_EX2_ACK*)CCliPktBuilder::Instance()->GetPacketBuffer();
                    if (NULL == pstAck)
                    {
                        return FALSE;
                    }
                    memset(pstAck, 0x00, sizeof(PKT_CLIGT_SELECT_ZONE_EX2_ACK));
                    pstAck->wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
                    poZSClient->SendMsg(CLIGT_SELECT_ZONE_EX2_ACK, (CHAR*)(pstAck));
                    return FALSE;
                }
            }
            else
            {
                //同步网络上来解码完后，应该用户名跟密码是一至的。
                if (AUTH_TYPE_TONGBU == pstReq->byAuthType)
                {
                    if(0 != SDStrcmp(pstReq->aszUserName, aszUserPwd))
                    {
                        //返回错误消息
                        PKT_CLIGT_SELECT_ZONE_EX2_ACK *pstAck = (PKT_CLIGT_SELECT_ZONE_EX2_ACK*)CCliPktBuilder::Instance()->GetPacketBuffer();
                        if (NULL == pstAck)
                        {
                            return FALSE;
                        }
                        memset(pstAck, 0x00, sizeof(PKT_CLIGT_SELECT_ZONE_EX2_ACK));
                        pstAck->wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
                        poZSClient->SendMsg(CLIGT_SELECT_ZONE_EX2_ACK, (CHAR*)(pstAck));
                        return FALSE;
                    }
                }
                else if (AUTH_TYPE_91 == pstReq->byAuthType)
                {
                    if(0 != SDStrcmp(pstReq->aszUserName, aszUserPwd))
                    {
                        //返回错误消息
                        PKT_CLIGT_SELECT_ZONE_EX2_ACK *pstAck = (PKT_CLIGT_SELECT_ZONE_EX2_ACK*)CCliPktBuilder::Instance()->GetPacketBuffer();
                        if (NULL == pstAck)
                        {
                            return FALSE;
                        }
                        memset(pstAck, 0x00, sizeof(PKT_CLIGT_SELECT_ZONE_EX2_ACK));
                        pstAck->wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
                        poZSClient->SendMsg(CLIGT_SELECT_ZONE_EX2_ACK, (CHAR*)(pstAck));
                        return FALSE;
                    }
                }
                else if (AUTH_TYPE_PP == pstReq->byAuthType)
                {
                    //if(0 != SDStrcmp(pstReq->aszUserName, aszUserPwd))
                    //{
                    //    //返回错误消息
                    //    PKT_CLIGT_SELECT_ZONE_EX2_ACK stAck;
                    //    stAck.wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
                    //    poZSClient->SendMsg(CLIGT_SELECT_ZONE_EX2_ACK, (CHAR*)(&stAck));
                    //    return FALSE;
                    //}
                }
            }
        }
        else
        {
            //返回错误消息
            PKT_CLIGT_SELECT_ZONE_EX2_ACK *pstAck = (PKT_CLIGT_SELECT_ZONE_EX2_ACK*)CCliPktBuilder::Instance()->GetPacketBuffer();
            if (NULL == pstAck)
            {
                return FALSE;
            }
            memset(pstAck, 0x00, sizeof(PKT_CLIGT_SELECT_ZONE_EX2_ACK));
            pstAck->wErrCode = ERR_SELECT_ZONE::ID_PWD_ERR;
            poZSClient->SendMsg(CLIGT_SELECT_ZONE_EX2_ACK, (CHAR*)(pstAck));
            return FALSE;
        }
    }

    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_SELECTZONE);
    if(NULL == poHttpCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
        return FALSE;
    }

    poHttpCmd->SetUserData((void*)pszBody, sizeof(PKT_CLIGT_SELECT_ZONE_EX_REQ), poZSClient->GetCliSessionID());
    poHttpCmd->Init();

    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        return FALSE;
    }
    return TRUE;
}

BOOL CCliProcessor::OnResetPasswdMobileReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CZSClient*		poZSClient = (CZSClient*)poReceiver;

    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_RESETPASSWDMOBILE);
    if(NULL == poHttpCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
        return FALSE;
    }

    poHttpCmd->SetUserData((void*)pszBody, sizeof(PKT_CLIZS_RESET_PASSWD_MOBILE_REQ), poZSClient->GetCliSessionID());
    poHttpCmd->Init();

    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        return FALSE;
    }
    return TRUE;
}

BOOL CCliProcessor::OnResetPasswdVerCodeReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CZSClient*		poZSClient = (CZSClient*)poReceiver;

    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_RESETPASSWDVERCODE);
    if(NULL == poHttpCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
        return FALSE;
    }

    poHttpCmd->SetUserData((void*)pszBody, sizeof(PKT_CLIZS_RESET_PASSWD_VER_CODE_REQ), poZSClient->GetCliSessionID());
    poHttpCmd->Init();

    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        return FALSE;
    }
    return TRUE;
}

BOOL CCliProcessor::OnResetPasswdNewPasswdReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CZSClient*		poZSClient = (CZSClient*)poReceiver;

    CBaseHttpCMD *poHttpCmd = CHttpMgr::Instance()->CreateCmd(EHCT_RESETPASSWDNEWPASSWD);
    if(NULL == poHttpCmd)
    {
        DBG_INFO(_SDT("[%s:%d]: CreateCmd failed"), MSG_MARK);
        return FALSE;
    }

    poHttpCmd->SetUserData((void*)pszBody, sizeof(PKT_CLIZS_RESET_PASSWD_NEWPASSWD_REQ), poZSClient->GetCliSessionID());
    poHttpCmd->Init();

    if(FALSE == CHttpMgr::Instance()->AddCommand(poHttpCmd))
    {
        DBG_INFO(_SDT("[%s:%d]: Add Command failed"), MSG_MARK);
        CHttpMgr::Instance()->FreeCmd(poHttpCmd);
        return FALSE;
    }
    return TRUE;
}