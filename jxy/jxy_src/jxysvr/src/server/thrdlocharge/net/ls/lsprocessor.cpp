#include "lsprocessor.h"
#include "protocol/server/protolstlc.h"
#include "logic/auth/authusermgr.h"
#include "logic/auth/authmgr.h"
#include "framework/tlcapplication.h"
#include "net/ls/lspktbuilder.h"


CLSProcessor::CLSProcessor()
{
    Init();
}

CLSProcessor::~CLSProcessor()
{
}

BOOL CLSProcessor::Init()
{
    RegisterCommand(LSTLC_AUTH_REQ,        CLSProcessor::OnAuthReq);    
    return TRUE;
}



BOOL CLSProcessor::OnAuthReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
	SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32 dwTransID            = pPipeHeader->dwTransID;

	CSDPipeChannel* poSDPipeChannel = ((CTLCApplication*)SDGetApp())->GetLSPipeChannel(pPipeHeader->wMark);

	PKT_LSTLC_AUTH_REQ* pReq = (PKT_LSTLC_AUTH_REQ*)pszBody;
	CAuthUser* poUser = CAuthUserMgr::Instance()->CreateUser(pPipeHeader->wMark, dwTransID, *pReq);
	BOOL bInnerErr = FALSE;
	if(NULL == poUser)
	{
		bInnerErr = TRUE;
	}
	if(!CAuthMgr::Instance()->Auth(poUser))
	{
		bInnerErr = TRUE;
	}

	if (poSDPipeChannel && bInnerErr)
	{
		UINT32 dwMsgID = CLSPktBuilder::Instance()->LoginAck(EAEC_INNERR, *pReq);
		if (FALSE == poSDPipeChannel->SendMsg(dwTransID, dwMsgID, CLSPktBuilder::Instance()->GetPacketBuffer()))
		{
			DBG_INFO(_SDT("[%s:%d]: send packet failed"), MSG_MARK);
			return FALSE;
		}
	}
   
	return TRUE;
}


CSDProtocol* CLSProcessor::GetProtocol()
{
    return CProtoLSTLC::Instance();
}

