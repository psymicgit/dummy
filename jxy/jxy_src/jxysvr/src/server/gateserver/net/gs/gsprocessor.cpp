
#include "gsprocessor.h"
#include "framework/gtapplication.h"
#include <net/cli/clipktbuilder.h>
#include <common/client/errdef.h>
#include <net/gs/gspktbuilder.h>


CGSProcessor::CGSProcessor()
{
    Init();
}

CGSProcessor::~CGSProcessor()
{
}

CSDProtocol* CGSProcessor::GetProtocol()
{
    return CProtoGTGS::Instance();
}


BOOL CGSProcessor::Init()
{
    RegisterCommand(CLIGS_ENTERGS_ACK,			CGSProcessor::OnEnterGSAck);
    RegisterCommand(GSGT_CLR_CACHE_RPT,			CGSProcessor::OnClrPlayerCache);
    RegisterCommand(CLIGS_REGISTER_USERNAME_NTF, CGSProcessor::OnRegisterUserNameNtf);
    RegisterCommand(GSLS_PLAYER_UPGRADE_NTF,		CGSProcessor::OnPlayerUpgrate);
    RegisterCommand(GSLS_DSPNAME_UPGRADE_NTF,    CGSProcessor::OnDspNameUpgrate);
    RegisterCommand(GSLS_ACTIVITY_NOTIFY_NTF,    CGSProcessor::OnActivityNotify);
	RegisterCommand(GSGT_KICK_OUT_REQ,    CGSProcessor::OnKickOutReq);

    return TRUE;
}


BOOL CGSProcessor::OnEnterGSAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CGTApplication* poGTApplication = (CGTApplication*)SDGetApp();
    //与GS通讯的dwTransID是User的，先找User
    CUser* poUser = CUserMgr::Instance()->FindUser(pPipeHeader->dwTransID);
	if(NULL == poUser)
	{
		return FALSE;
	}

    CGTClient* poCliSession = poUser->GetGTClient();
    if (NULL == poCliSession)
    {
        DBG_INFO(_SDT("[%s:%d]: Find client session failed"), MSG_MARK);
        return FALSE;
    }
    //
    PKT_CLIGS_ENTERGS_ACK* pstAck = (PKT_CLIGS_ENTERGS_ACK*)pszBody;
    if (pstAck->wErrCode == SUCCESS)
    {
        poCliSession->SetState(CLIENT_STATE_IN_GS);
    }
    return TRUE;
}


BOOL CGSProcessor::OnClrPlayerCache(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CGTApplication* poGTApplication = (CGTApplication*)SDGetApp();
    //与GS通讯的dwTransID是User的，先找User
    CUser* poUser = CUserMgr::Instance()->FindUser(pPipeHeader->dwTransID);
    if(NULL == poUser)
    {
        return FALSE;
    }
    CGTClient* poCliSession = poUser->GetGTClient();

    PKT_GSGT_CLR_CACHE_RPT* pstRpt = (PKT_GSGT_CLR_CACHE_RPT*)pszBody;
    //此刻该用户没有上线
    if (NULL == poCliSession)
    {
        CUserMgr::Instance()->ReleaseUser(poUser);
    }
    else
    {
        //0为状态判断是否重新登录，1为强制踢下线
        if(1 == pstRpt->byExt)
        {
            poCliSession->Disconnect();
            CUserMgr::Instance()->ReleaseUser(poUser);
        }
        else
        {
            //正在进游戏，则不断连接
            if((CLIENT_STATE_AUTHING == poCliSession->GetState()) ||
                    (CLIENT_STATE_AUTHED == poCliSession->GetState()) ||
                    (CLIENT_STATE_ENTERING_GAME == poCliSession->GetState()) ||
                    (CLIENT_STATE_IN_GS == poCliSession->GetState()))
            {
                return TRUE;
            }
            else
            {
                poCliSession->Disconnect();
                CUserMgr::Instance()->ReleaseUser(poUser);
            }
        }
    }

    return TRUE;
}

BOOL CGSProcessor::OnRegisterUserNameNtf(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CGTApplication* poGTApplication = (CGTApplication*)SDGetApp();
    //与GS通讯的dwTransID是User的，先找User
    CUser* poUser = CUserMgr::Instance()->FindUser(pPipeHeader->dwTransID);
	if(NULL == poUser)
	{
		return FALSE;
	}
    CGTClient* poCliSession = poUser->GetGTClient();

    //此刻该用户已下线
    if (NULL == poCliSession)
    {
        return FALSE;
    }
    //修改连接状态，让客户端消息转发到认证服务器注册用户/密码
    else
    {
        poCliSession->SetState(CLIENT_STATE_RERI_USERNAME);
    }

    return TRUE;
}

BOOL CGSProcessor::OnRegisterUserNameAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    CGTApplication* poGTApplication = (CGTApplication*)SDGetApp();
    //与GS通讯的dwTransID是User的，先找User
    CUser* poUser = CUserMgr::Instance()->FindUser(pPipeHeader->dwTransID);
	if(NULL == poUser)
	{
		return FALSE;
	}
    CGTClient* poCliSession = poUser->GetGTClient();

    //此刻该用户已下线
    if (NULL == poCliSession)
    {
        return FALSE;//不是给客户端的消息，别转发
    }
    else
    {
        PKT_GSGT_REGISTER_USERNAME_ACK* pstAck = (PKT_GSGT_REGISTER_USERNAME_ACK*)pszBody;

        //注册失败，直接返回
        if (SUCCESS != pstAck->wErrCode)
        {
            //给客户端返回ACK
            UINT16 wMsgID = CCliPktBuilder::Instance()->RegisterUserNameAck(pstAck->wErrCode, pstAck->aszUserName);
            poCliSession->SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());
            return FALSE;//不是给客户端的消息，别转发
        }

        //修改状态，让玩家可进gs
        UINT16 wMsgID = CCliPktBuilder::Instance()->RegisterUserNameAck(pstAck->wErrCode, pstAck->aszUserName);
        poCliSession->SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());

        poCliSession->SetState(CLIENT_STATE_IN_GS);
    }

    return FALSE;//不是给客户端的消息，别转发
}


BOOL CGSProcessor::OnPlayerUpgrate(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    //与GS通讯的dwTransID是User的，先找User
    CUser* poUser = CUserMgr::Instance()->FindUser(pPipeHeader->dwTransID);
    if(NULL == poUser)
    {
        return FALSE;
    }

    pPipeHeader->dwTransID = SGDP::SDNtohl(pPipeHeader->dwTransID);
    pPipeHeader->wMsgID = SGDP::SDNtohs(pPipeHeader->wMsgID);

    //转发到LS
    //CSDPipeChannel* poSDPipeChannel = ((CGTApplication*)SDGetApp())->GetLSServer();
    //if (poSDPipeChannel)
    //{
    //    poSDPipeChannel->Send((CHAR*)pHeader, dwLen + sizeof(SDPipeMsgHeader));
    //}

    return FALSE;//不是给客户端的消息，别转发
}



BOOL CGSProcessor::OnDspNameUpgrate(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    //与GS通讯的dwTransID是User的，先找User
    CUser* poUser = CUserMgr::Instance()->FindUser(pPipeHeader->dwTransID);
    if(NULL == poUser)
    {
        return FALSE;
    }
    PKT_GSLS_DSPNAME_UPGRADE_NTF* pstNtf = (PKT_GSLS_DSPNAME_UPGRADE_NTF*)((CHAR*)pHeader + sizeof(SDPipeMsgHeader) );

    pPipeHeader->dwTransID = SGDP::SDNtohl(pPipeHeader->dwTransID);
    pPipeHeader->wMsgID = SGDP::SDNtohs(pPipeHeader->wMsgID);

    //转发到LS
    //CSDPipeChannel* poSDPipeChannel = ((CGTApplication*)SDGetApp())->GetLSServer();
    //if (poSDPipeChannel)
    //{
    //    poSDPipeChannel->Send((CHAR*)pHeader, dwLen + sizeof(SDPipeMsgHeader));
    //}

    return FALSE;//不是给客户端的消息，别转发
}

BOOL CGSProcessor::OnActivityNotify(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
    //与GS通讯的dwTransID是User的，先找User
    CUser* poUser = CUserMgr::Instance()->FindUser(pPipeHeader->dwTransID);
    if(NULL == poUser)
    {
        return FALSE;
    }
    PKT_GSLS_ACTIVITY_NOTIFY_NTF* pstNtf = (PKT_GSLS_ACTIVITY_NOTIFY_NTF*)((CHAR*)pHeader + sizeof(SDPipeMsgHeader) );

    pPipeHeader->dwTransID = SGDP::SDNtohl(pPipeHeader->dwTransID);
    pPipeHeader->wMsgID = SGDP::SDNtohs(pPipeHeader->wMsgID);

    //转发到LS
    //CSDPipeChannel* poSDPipeChannel = ((CGTApplication*)SDGetApp())->GetLSServer();
    //if (poSDPipeChannel)
    //{
    //    poSDPipeChannel->Send((CHAR*)pHeader, dwLen + sizeof(SDPipeMsgHeader));
    //}

    return FALSE;//不是给客户端的消息，别转发
}


BOOL CGSProcessor::OnKickOutReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	PKT_GSGT_KICK_OUT_REQ* pstReq = (PKT_GSGT_KICK_OUT_REQ*)(pszBody);
    
    //通过GM T下线的才加入列表
    if (pstReq->byKickOutType == 0)
    {
        CUserMgr::Instance()->AddKickOutPlayer(pstReq->dwPlayerID);
    }
	UINT16 wMsgID = CGSPktBuilder::Instance()->KickOutAck("success");
	CSDPipeChannel* poGsChannel = ((CGTApplication*)SDGetApp())->GetGSServer();
	if (poGsChannel)
	{
		poGsChannel->SendMsg(pPipeHeader->dwTransID, wMsgID, CGSPktBuilder::Instance()->GetPacketBuffer()); //收到即可返回应答
	}
	
	CUser* poUser = CUserMgr::Instance()->FindUserByPlayerID(pstReq->dwPlayerID);
	if(NULL == poUser)
	{
		return FALSE;
	}
	CGTClient* poGTClient = poUser->GetGTClient();
	if(NULL == poGTClient)
	{
		return FALSE;
	}

	poGTClient->SetState(CLIENT_STATE_UNAUTHED);
	string strMsg = SDGBK2UTF8("您已经被系统踢下线，请反思您的言行！");
	wMsgID = CCliPktBuilder::Instance()->KickOutNtf(SERVER_KICK, (CHAR*)(strMsg.c_str()));			
	poGTClient->SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());
	poGTClient->Disconnect();//将之前玩家踢下线


	return FALSE;//不是给客户端的消息，别转发
}

