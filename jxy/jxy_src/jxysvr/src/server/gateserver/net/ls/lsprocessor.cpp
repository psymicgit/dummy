#include "framework/gtapplication.h"

#include "net/ls/lsprocessor.h"
#include "net/cli/clipktbuilder.h"
#include "net/gs/gspktbuilder.h"
#include "net/ls/lspktbuilder.h"
#include "logic/usermgr.h"
#include <common/server/utility.h>
#include <common/client/errdef.h>


CLSProcessor::CLSProcessor()
{
    Init();
}

CLSProcessor::~CLSProcessor()
{
}

BOOL CLSProcessor::Init()
{
    RegisterCommand(GTLS_LOGIN_ACK,        CLSProcessor::OnLoginAck);
    RegisterCommand(GTLS_LOGIN_EX_ACK,		CLSProcessor::OnLoginExAck);
    return TRUE;
}


//处理CLILS_LOGIN_ACK消息
BOOL CLSProcessor::OnLoginAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    //CAutoCycle oAutoCycle("OnLoginAck");

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CGTApplication* poGTApplication = (CGTApplication*)SDGetApp();
    //找到client
    CGTClient* poCliSession = poGTApplication->GetClientMgr()->FindCliSession(pPipeHeader->dwTransID);
    if (NULL == poCliSession)
    {
        DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
        return FALSE;
    }
    PKT_GTLS_LOGIN_ACK* pstAck = (PKT_GTLS_LOGIN_ACK*)pszBody;



    CSDPipeChannel* poSDPipeChannel = ((CGTApplication*)SDGetApp())->GetGSServer();
    if ((NULL == poSDPipeChannel) && (ERR_LOGIN::ID_VERSION_NEED_UPDATE != pstAck->wErrCode)) //优先版本强制更新
    {
        //返回错误消息
        pstAck->wErrCode = ERR_LOGIN::ID_GS_SERVER_NOT_STARTUP;

        //发送到客户端
        UINT16 wMsgID =  CLSPktBuilder::Instance()->LoginAck(*pstAck, FALSE);

        poCliSession->SendMsg(wMsgID, CLSPktBuilder::Instance()->GetPacketBuffer());
		
		SDSleep(0);
		poCliSession->Disconnect();//认证失败要断开连接

        return FALSE;
    }

    //认证失败，直接返回
    if (SUCCESS != pstAck->wErrCode)
    {
        poCliSession->SetState(CLIENT_STATE_UNAUTHED);        

        DBG_INFO(_SDT("[%s:%d]: Auth Failed,  PlayerID[%d], UserID[%d], "), MSG_MARK, pstAck->dwPlayerID, pstAck->dwUserID);

        UINT16 wMsgID = 0;
        if (pstAck->wErrCode == ERR_LOGIN::ID_SYSTEM_MAINTAIN)
        {
            pstAck->wErrCode = ERR_LOGIN::ID_GS_SERVER_NOT_STARTUP;
            wMsgID =  CLSPktBuilder::Instance()->LoginAck(*pstAck, FALSE);
        }
        else
        {
            wMsgID =  CLSPktBuilder::Instance()->LoginAck(*pstAck, TRUE);
        }

        //发送到客户端
        poCliSession->SendMsg(wMsgID, CLSPktBuilder::Instance()->GetPacketBuffer());

		SDSleep(0);
		poCliSession->Disconnect();//认证失败要断开连接

        return FALSE;
    }

	//检查是否被踢下线
	if(CUserMgr::Instance()->BeKickOut(pstAck->dwPlayerID))
	{
		pstAck->wErrCode = ERR_LOGIN::ID_FORBID_LOGIN;
		UINT16 wMsgID =  CLSPktBuilder::Instance()->LoginAck(*pstAck, TRUE);
		//发送到客户端
		poCliSession->SendMsg(wMsgID, CLSPktBuilder::Instance()->GetPacketBuffer());

		SDSleep(0);
		poCliSession->Disconnect();//断开连接

		return FALSE;
	}

    poCliSession->SetState(CLIENT_STATE_AUTHED);

    //记录用户数据
    CUser* poUser = CUserMgr::Instance()->CreateUser(pstAck->aszUserName, pstAck->aszDeviceID, pstAck->aszNotifyID, pstAck->wZoneID, 0, pstAck->dwUserID, pstAck->dwPlayerID, pstAck->wCareerID, pstAck->byAuthType, poCliSession, 0);
    if(NULL == poUser)
    {
        //异常
        DBG_INFO(_SDT("[%s:%d]: CreateUser failed! UserID[%d], UserName[%s], DeviceID[%s]"), MSG_MARK, pstAck->dwPlayerID, pstAck->aszUserName, pstAck->aszDeviceID);
		SDSleep(0);
		poCliSession->Disconnect();//认证失败要断开连接
        return FALSE;
    }
    poCliSession->SetUser(poUser);

    //发送到客户端
    UINT16 wMsgID =  CLSPktBuilder::Instance()->LoginAck(*pstAck, TRUE);
    poCliSession->SendMsg(wMsgID, CLSPktBuilder::Instance()->GetPacketBuffer());

    return FALSE; //已发送，不需转发
}


//处理CLILS_LOGIN_EX_ACK消息
BOOL CLSProcessor::OnLoginExAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    //CAutoCycle oAutoCycle("OnLoginAck");

    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CGTApplication* poGTApplication = (CGTApplication*)SDGetApp();
    //找到client
    CGTClient* poCliSession = poGTApplication->GetClientMgr()->FindCliSession(pPipeHeader->dwTransID);
    if (NULL == poCliSession)
    {
        DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
        return FALSE;
    }
    PKT_GTLS_LOGIN_EX_ACK* pstAck = (PKT_GTLS_LOGIN_EX_ACK*)pszBody;



    CSDPipeChannel* poSDPipeChannel = ((CGTApplication*)SDGetApp())->GetGSServer();
    if ((NULL == poSDPipeChannel) && (ERR_LOGIN::ID_VERSION_NEED_UPDATE != pstAck->wErrCode)) //优先版本强制更新
    {
        //返回错误消息
        pstAck->wErrCode = ERR_LOGIN::ID_GS_SERVER_NOT_STARTUP;

        //发送到客户端
        UINT16 wMsgID =  CLSPktBuilder::Instance()->LoginExAck(*pstAck, FALSE);

        poCliSession->SendMsg(wMsgID, CLSPktBuilder::Instance()->GetPacketBuffer());

		SDSleep(0);
		poCliSession->Disconnect();//认证失败要断开连接

        return FALSE;
    }

    //认证失败，直接返回
    if (SUCCESS != pstAck->wErrCode)
    {
        poCliSession->SetState(CLIENT_STATE_UNAUTHED);
 
        DBG_INFO(_SDT("[%s:%d]: Auth Failed,  PlayerID[%d], UserID[%d], "), MSG_MARK, pstAck->dwPlayerID, pstAck->dwUserID);

        UINT16 wMsgID = 0;
        if (pstAck->wErrCode == ERR_LOGIN::ID_SYSTEM_MAINTAIN)
        {
            pstAck->wErrCode = ERR_LOGIN::ID_GS_SERVER_NOT_STARTUP;
            wMsgID =  CLSPktBuilder::Instance()->LoginExAck(*pstAck, FALSE);
        }
        else
        {
            wMsgID =  CLSPktBuilder::Instance()->LoginExAck(*pstAck, TRUE);
        }

        //发送到客户端
        poCliSession->SendMsg(wMsgID, CLSPktBuilder::Instance()->GetPacketBuffer());

		SDSleep(0);
		poCliSession->Disconnect();//认证失败要断开连接

        return FALSE;
    }

    poCliSession->SetState(CLIENT_STATE_AUTHED);

    //记录用户数据
    CUser* poUser = CUserMgr::Instance()->CreateUser(pstAck->aszUserName, pstAck->aszDeviceID, pstAck->aszNotifyID, pstAck->wZoneID, 0, pstAck->dwUserID, pstAck->dwPlayerID, pstAck->wCareerID, pstAck->byAuthType, poCliSession, 0);
    if(NULL == poUser)
    {
        //异常
        DBG_INFO(_SDT("[%s:%d]: CreateUser failed! UserID[%d], UserName[%s], DeviceID[%s]"), MSG_MARK, pstAck->dwPlayerID, pstAck->aszUserName, pstAck->aszDeviceID);

		SDSleep(0);
		poCliSession->Disconnect();//认证失败要断开连接

        return FALSE;
    }
    poCliSession->SetUser(poUser);

    //发送到客户端
    UINT16 wMsgID =  CLSPktBuilder::Instance()->LoginExAck(*pstAck, TRUE);
    poCliSession->SendMsg(wMsgID, CLSPktBuilder::Instance()->GetPacketBuffer());



    return FALSE; //已发送，不需转发
}

CSDProtocol* CLSProcessor::GetProtocol()
{
    return CProtoGTLS::Instance();
}

