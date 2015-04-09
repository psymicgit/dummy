
#include "lspktbuilder.h"
#include "protocol/server/protolstlc.h"


CLSPktBuilder::CLSPktBuilder()
{

}

CLSPktBuilder::~CLSPktBuilder()
{

}

UINT16 CLSPktBuilder::LoginAck(UINT16 wErrCode, PKT_LSTLC_AUTH_REQ& stReq)
{
	ZeroPacketBuffer(sizeof(PKT_LSTLC_AUTH_ACK));
	PKT_LSTLC_AUTH_ACK* pstAck = (PKT_LSTLC_AUTH_ACK*)(GetPacketBuffer());
	pstAck->wErrCode = wErrCode;
	pstAck->byAuthType = stReq.byAuthType;
	SDStrcpy(pstAck->aszDeviceID, stReq.aszDeviceID);
	SDStrcpy(pstAck->aszUserName, stReq.aszUserName);
	pstAck->byPwdLen = stReq.byPwdLen;
	pstAck->dwParam1 = stReq.dwParam1;
	pstAck->dwParam2 = stReq.dwParam2;
	pstAck->dwParam3 = stReq.dwParam3;
	pstAck->qwParam4 = stReq.qwParam4;
	SDStrcpy(pstAck->aszExtContext, stReq.aszExtContext);

	return LSTLC_AUTH_ACK;
}

