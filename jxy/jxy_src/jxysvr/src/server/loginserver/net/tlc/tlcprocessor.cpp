#include "sdutil.h"
#include "tlcprocessor.h"
#include "net/tlc/authusermgr.h"

CTLCProcessor::CTLCProcessor()
{
    Init();
}

CTLCProcessor::~CTLCProcessor()
{
}



CSDProtocol* CTLCProcessor::GetProtocol()
{
    return CProtoLSTLC::Instance() ;
}

BOOL CTLCProcessor::Init()
{
    RegisterCommand(LSTLC_AUTH_ACK, CTLCProcessor::OnAuthAck);
 
    return TRUE;
}


BOOL CTLCProcessor::OnAuthAck(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader* pPipeHeader = (SDPipeMsgHeader*)pHeader;
	PKT_LSTLC_AUTH_ACK* pstAck = (PKT_LSTLC_AUTH_ACK*)pszBody;
    CAuthUserMgr::Instance()->OnAuthAck(pPipeHeader->dwTransID, pstAck->wErrCode);

    return TRUE;
}
