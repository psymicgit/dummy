
#include "gspktbuilder.h"
#include "protocol/server/protogtgs.h"

CGSPktBuilder::CGSPktBuilder()
{

}

CGSPktBuilder::~CGSPktBuilder()
{

}


UINT16 CGSPktBuilder::EnterGSReq(TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT16 wZoneID, UINT16 wOriZoneID, UINT32 dwUserID, UINT32 dwPlayerID, UINT16 wCareerID, UINT32 dwLoginIP, UINT32 dwCliVer, UINT8 byAuthType, UINT8 byDaiChong)
{
    ZeroPacketBuffer(sizeof(PKT_GTGS_ENTERGS_REQ));
    PKT_GTGS_ENTERGS_REQ* pstReq = (PKT_GTGS_ENTERGS_REQ*)(GetPacketBuffer());
    _SDTStrcpy(pstReq->aszUserName, pszUserName);
	_SDTStrcpy(pstReq->aszDeviceID, pszDeviceID);
	_SDTStrcpy(pstReq->aszNotifyID, pszNotifyID);
	pstReq->wZoneID = wZoneID;
    pstReq->wOriZoneID = wOriZoneID;
	pstReq->dwUserID = dwUserID;
	pstReq->dwPlayerID = dwPlayerID;	
	pstReq->wCareerID = wCareerID;
	pstReq->dwLoginIP = dwLoginIP;
	pstReq->dwCliVer = dwCliVer;
	pstReq->byAuthType = byAuthType;
	pstReq->byDaiChong = byDaiChong;
    return GTGS_ENTERGS_REQ;
}

UINT16 CGSPktBuilder::RegisterUserNameReq(TCHAR* pszUserName)
{
	ZeroPacketBuffer(sizeof(PKT_GSGT_REGISTER_USERNAME_REQ));
	PKT_GSGT_REGISTER_USERNAME_REQ* pstReq = (PKT_GSGT_REGISTER_USERNAME_REQ*)(GetPacketBuffer());
	_SDTStrcpy(pstReq->aszUserName, pszUserName);

	return GSGT_REGISTER_USERNAME_REQ;
}


UINT16 CGSPktBuilder::KickOutAck(TCHAR* pszErrMsg)
{
	ZeroPacketBuffer(sizeof(PKT_GSGT_KICK_OUT_ACK));
	PKT_GSGT_KICK_OUT_ACK* pstAck = (PKT_GSGT_KICK_OUT_ACK*)(GetPacketBuffer());
	SDStrncpy(pstAck->aszErrMsg, pszErrMsg, 127);

	return GSGT_KICK_OUT_ACK;
}


