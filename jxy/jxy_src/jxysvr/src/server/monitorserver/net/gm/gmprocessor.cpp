#include "gmprocessor.h"
#include "sdutil.h"
#include "framework/msapplication.h"
#include "protocol/server/protomsgm.h"
#include <common/client/errdef.h>


extern CSDApplication* SDGetApp();

CGMProcessor::CGMProcessor()
{
    Init();
}

CGMProcessor::~CGMProcessor()
{

}

BOOL CGMProcessor::Init()
{
    RegisterCommand(GMGS_CONTROL_REQ, CGMProcessor::OnControlReq);
	RegisterCommand(GMGS_GET_SERVER_INFO_REQ, CGMProcessor::OnGetServerInfoReq);
	
    return TRUE;
}

CSDProtocol* CGMProcessor::GetProtocol()
{
    return CProtoMSGM::Instance();
}

BOOL CGMProcessor::OnControlReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
    SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
    UINT32			dwTransID  = pPipeHeader->dwTransID;
    if (sizeof(PKT_GMGS_CONTROL_REQ) != dwLen)
    {
        return FALSE;
    }

	PKT_GMGS_CONTROL_REQ *pstReq = (PKT_GMGS_CONTROL_REQ*)pszBody;
    if (NULL == pstReq)
    {
        return FALSE;
    }

	PKT_GMGS_CONTROL_ACK stAck;
	stAck.dwSerialNumber = pstReq->dwSerialNumber;
	if (CMonitor::Instance()->SetUseFlag(pstReq->dwAreaID, pstReq->aszServer, pstReq->byUseFlag))
	{
		stAck.wErrCode	=	0;
	}
	else
	{
		stAck.wErrCode	=	1;
	}
	((CMSApplication*)SDGetApp())->SendMsg2GMServer(GMGS_CONTROL_ACK, (CHAR*)&stAck);
	 
    return TRUE;
}


BOOL CGMProcessor::OnGetServerInfoReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
	SDPipeMsgHeader*	pPipeHeader = (SDPipeMsgHeader*)pHeader;
	UINT32			dwTransID  = pPipeHeader->dwTransID;
	if (sizeof(PKT_GMGS_GET_SERVER_INFO_REQ) != dwLen)
	{
		return FALSE;
	}

	PKT_GMGS_GET_SERVER_INFO_REQ *pstReq = (PKT_GMGS_GET_SERVER_INFO_REQ*)pszBody;
	if (NULL == pstReq)
	{
		return FALSE;
	}

	PKT_GMGS_GET_SERVER_INFO_ACK stAck;
	stAck.dwAreaID = pstReq->dwAreaID;
	stAck.dwSerialNumber = pstReq->dwSerialNumber;
	CMonitor::Instance()->GET_SERVER_INFO_ACK(stAck);
	((CMSApplication*)SDGetApp())->SendMsg2GMServer(GMGS_GET_SERVER_INFO_ACK, (CHAR*)&stAck);
	return TRUE;
}
