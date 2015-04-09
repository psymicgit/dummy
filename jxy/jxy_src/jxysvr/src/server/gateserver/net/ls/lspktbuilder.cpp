
#include "lspktbuilder.h"
#include "protocol/server/protogtls.h"

CLSPktBuilder::CLSPktBuilder()
{

}

CLSPktBuilder::~CLSPktBuilder()
{

}


UINT16 CLSPktBuilder::LoginAck(PKT_GTLS_LOGIN_ACK& stAck, BOOL bServer)
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_LOGIN_ACK));
    PKT_CLILS_LOGIN_ACK* pstAck = (PKT_CLILS_LOGIN_ACK*)(GetPacketBuffer());
    pstAck->wErrCode = stAck.wErrCode;
    memcpy(&(pstAck->stVersionInfo), &stAck.stVersionInfo, sizeof(DT_VERSION_DATA));

    if (bServer)
    {
        memcpy(&(pstAck->stNoticeInfo), &stAck.stNoticeInfo1, sizeof(DT_NOTICE_DATA));
    }
    else
    {
        memcpy(&(pstAck->stNoticeInfo), &stAck.stNoticeInfo2, sizeof(DT_NOTICE_DATA));
    }

    pstAck->byOtherDeviceLoginFlag = stAck.byOtherDeviceLoginFlag;
    pstAck->byAuthType = stAck.byAuthType;
    _SDTStrcpy(pstAck->aszDeviceID, stAck.aszDeviceID);
    _SDTStrcpy(pstAck->aszNotifyID, stAck.aszNotifyID);
    _SDTStrcpy(pstAck->aszUserName, stAck.aszUserName);
    pstAck->wZoneID = stAck.wZoneID;
    _SDTStrcpy(pstAck->aszZoneName, stAck.aszZoneName);
    pstAck->dwPlayerID = stAck.dwPlayerID;
    pstAck->wCareerID = stAck.wCareerID;
    return CLILS_LOGIN_ACK;
}

UINT16 CLSPktBuilder::LoginExAck(PKT_GTLS_LOGIN_EX_ACK& stAck, BOOL bServer)
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_LOGIN_EX_ACK));
    PKT_CLILS_LOGIN_EX_ACK* pstAck = (PKT_CLILS_LOGIN_EX_ACK*)(GetPacketBuffer());
    pstAck->wErrCode = stAck.wErrCode;
    memcpy(&(pstAck->stVersionInfo), &stAck.stVersionInfo, sizeof(DT_VERSION_DATA));

    if (bServer)
    {
        memcpy(&(pstAck->stNoticeInfo), &stAck.stNoticeInfo1, sizeof(DT_NOTICE_DATA));
    }
    else
    {
        memcpy(&(pstAck->stNoticeInfo), &stAck.stNoticeInfo2, sizeof(DT_NOTICE_DATA));
    }

    pstAck->byOtherDeviceLoginFlag = stAck.byOtherDeviceLoginFlag;
    pstAck->byAuthType = stAck.byAuthType;
    _SDTStrcpy(pstAck->aszDeviceID, stAck.aszDeviceID);
    _SDTStrcpy(pstAck->aszNotifyID, stAck.aszNotifyID);
    _SDTStrcpy(pstAck->aszUserName, stAck.aszUserName);
    pstAck->wZoneID = stAck.wZoneID;
    _SDTStrcpy(pstAck->aszZoneName, stAck.aszZoneName);
    pstAck->dwPlayerID = stAck.dwPlayerID;
    pstAck->wCareerID = stAck.wCareerID;
    return CLILS_LOGIN_EX_ACK;
}





