
#include "clipktbuilder.h"
#include "common/client/errdef.h"


CCliPktBuilder::CCliPktBuilder()
{

}

CCliPktBuilder::~CCliPktBuilder()
{

}


UINT16 CCliPktBuilder::EncryptDecryptKeyNtf(UINT8 abyKeyInfo[])
{
    ZeroPacketBuffer(sizeof(PKT_CLIGT_ENDEC_KEY_NTF));
    PKT_CLIGT_ENDEC_KEY_NTF* pstNtf = (PKT_CLIGT_ENDEC_KEY_NTF*)(GetPacketBuffer());
    memcpy(pstNtf->abyKeyInfo, abyKeyInfo, ENDEC_KEY_LEN);

    return CLIGT_ENDEC_KEY_NTF;
}

UINT16 CCliPktBuilder::HeartbeatAck()
{
    ZeroPacketBuffer(sizeof(PKT_CLIGT_HEARTBEAT_ACK));
    PKT_CLIGT_HEARTBEAT_ACK* pstAck = (PKT_CLIGT_HEARTBEAT_ACK*)(GetPacketBuffer());
    pstAck->byExtInfo = 0;

    return CLIGT_HEARTBEAT_ACK;
}


UINT16 CCliPktBuilder::RegisterUserNameAck(UINT32 wErrCode, const TCHAR* pszUserName)
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_REGISTER_ACK));
    PKT_CLILS_REGISTER_ACK* pstAck = (PKT_CLILS_REGISTER_ACK*)(GetPacketBuffer());

    pstAck->wErrCode = wErrCode;
    //只有成功才赋值，避免被外挂利用

    if(ERR_REGISTER_USERNAME::ID_SUCCESS == wErrCode)
    {
        _SDTStrcpy(pstAck->aszUserName, pszUserName);
    }

    return CLILS_REGISTER_ACK;
}


UINT16 CCliPktBuilder::EnteGSAck(UINT16 wErrCode)
{
    ZeroPacketBuffer(sizeof(PKT_CLIGS_ENTERGS_ACK));
    PKT_CLIGS_ENTERGS_ACK* pstAck = (PKT_CLIGS_ENTERGS_ACK*)(GetPacketBuffer());

    pstAck->wErrCode = wErrCode;

    return CLIGS_ENTERGS_ACK;
}

UINT16 CCliPktBuilder::ServerNotStartupNtf(UINT8 byServerType)
{
    ZeroPacketBuffer(sizeof(PKT_CLIGT_SERVER_NOT_STARTUP_NTF));
    PKT_CLIGT_SERVER_NOT_STARTUP_NTF* pstNtf = (PKT_CLIGT_SERVER_NOT_STARTUP_NTF*)(GetPacketBuffer());
    pstNtf->byServerType = byServerType;

    return CLIGT_SERVER_NOT_STARTUP_NTF;
}

UINT16 CCliPktBuilder::KickOutNtf(UINT8 byKickOutType, CHAR* pszMsgInfo)
{
    ZeroPacketBuffer(sizeof(PKT_CLIGT_KICK_OUT_NTF));
    PKT_CLIGT_KICK_OUT_NTF* pstNtf = (PKT_CLIGT_KICK_OUT_NTF*)(GetPacketBuffer());
    pstNtf->byKickOutType = byKickOutType;
    if(NULL != pszMsgInfo)
    {
        SDStrncpy(pstNtf->aszMsgInfo, pszMsgInfo, MSG_LEN - 1);
    }

    return CLIGT_KICK_OUT_NTF;
}


UINT16 CCliPktBuilder::LoginAck(UINT32 wErrCode, DT_NOTICE_DATA* poDT_NOTICE_DATA)
{

    ZeroPacketBuffer(sizeof(PKT_CLILS_LOGIN_ACK));
    PKT_CLILS_LOGIN_ACK* pstAck = (PKT_CLILS_LOGIN_ACK*)(GetPacketBuffer());
    memset(pstAck, 0x00, sizeof(PKT_CLILS_LOGIN_ACK));
    pstAck->wErrCode = wErrCode;


    if (ERR_LOGIN::ID_LS_SERVER_NOT_STARTUP == wErrCode && NULL != poDT_NOTICE_DATA)
    {
        memcpy(&pstAck->stNoticeInfo, poDT_NOTICE_DATA, sizeof(DT_NOTICE_DATA));
    }
    return CLILS_LOGIN_ACK;
    //ZeroPacketBuffer(sizeof(PKT_CLILS_LOGIN_ACK));
    //PKT_CLILS_LOGIN_ACK* pstAck = (PKT_CLILS_LOGIN_ACK*)(GetPacketBuffer());
    //pstAck->wErrCode = wErrCode;

    //_SDTStrcpy(pstAck->aszUserName, pstReq->aszUserName);
    //_SDTStrcpy(pstAck->aszDeviceID, pstReq->aszDeviceID);
    //_SDTStrcpy(pstAck->aszNotifyID, pstReq->aszNotifyID);

    //pstAck->dwPlayerID = pstReq->dwPlayerID;
    //pstAck->wZoneID = pstReq->wZoneID;
    //pstAck->wCareerID = pstReq->wCareerID;

    return CLILS_LOGIN_ACK;
}

UINT16 CCliPktBuilder::LoginExAck(UINT32 wErrCode, PKT_CLILS_LOGIN_EX_REQ* pstReq, BOOL bIos7)
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_LOGIN_EX_ACK));
    PKT_CLILS_LOGIN_EX_ACK* pstAck = (PKT_CLILS_LOGIN_EX_ACK*)(GetPacketBuffer());
    memset(pstAck, 0x00, sizeof(PKT_CLILS_LOGIN_EX_ACK));
    pstAck->wErrCode = wErrCode;

    _SDTStrcpy(pstAck->aszUserName, pstReq->aszUserName);
    _SDTStrcpy(pstAck->aszDeviceID, pstReq->aszDeviceID);
    _SDTStrcpy(pstAck->aszNotifyID, pstReq->aszNotifyID);

    pstAck->dwPlayerID = pstReq->dwPlayerID;
    pstAck->wZoneID = pstReq->wZoneID;
    pstAck->wCareerID = pstReq->wCareerID;

    return CLILS_LOGIN_EX_ACK;
}

UINT16 CCliPktBuilder::LoginExAck(UINT16 wErrCode, DT_NOTICE_DATA *poDT_NOTICE_DATA)
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_LOGIN_EX_ACK));
    PKT_CLILS_LOGIN_EX_ACK* pstAck = (PKT_CLILS_LOGIN_EX_ACK*)(GetPacketBuffer());
    memset(pstAck, 0x00, sizeof(PKT_CLILS_LOGIN_EX_ACK));
    pstAck->wErrCode = wErrCode;

    if ((ERR_LOGIN::ID_SYSTEM_MAINTAIN == wErrCode
            || ERR_LOGIN::ID_LS_SERVER_NOT_STARTUP == wErrCode
            || ERR_LOGIN::ID_GS_SERVER_NOT_STARTUP == wErrCode)
            && NULL != poDT_NOTICE_DATA)
    {
        memcpy(&pstAck->stNoticeInfo, poDT_NOTICE_DATA, sizeof(DT_NOTICE_DATA));
    }
    return CLILS_LOGIN_EX_ACK;
}


UINT16 CCliPktBuilder::LoginExAck(UINT16 wErrCode, DT_VERSION_DATA  stVersionInfo, DT_NOTICE_DATA stNoticeInfo, UINT8 byOtherDeviceLoginFlag, UINT8 byAuthType,
                                  TCHAR  aszDeviceID[DEVICEID_LEN], TCHAR aszNotifyID[NOTIFYID_LEN], TCHAR aszUserName[USERNAME_LEN], UINT16 wZoneID,
                                  TCHAR aszZoneName[USERNAME_LEN], UINT32 dwUserID, UINT32 dwPlayerID, UINT16 wCareerID)
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_LOGIN_EX_ACK));
    PKT_CLILS_LOGIN_EX_ACK* pstAck = (PKT_CLILS_LOGIN_EX_ACK*)(GetPacketBuffer());
    memset(pstAck, 0x00, sizeof(PKT_CLILS_LOGIN_EX_ACK));

    pstAck->wErrCode = wErrCode;
    memcpy(&pstAck->stNoticeInfo, &stVersionInfo, sizeof(DT_VERSION_DATA));
    memcpy(&pstAck->stNoticeInfo, &stNoticeInfo, sizeof(DT_NOTICE_DATA));
    pstAck->byOtherDeviceLoginFlag = byOtherDeviceLoginFlag;
    pstAck->byAuthType = byAuthType;
    pstAck->wZoneID = wZoneID;
    pstAck->dwPlayerID = dwPlayerID;
    pstAck->wCareerID = wCareerID;
    SDStrncpy(pstAck->aszDeviceID, aszDeviceID, DEVICEID_LEN);
    SDStrncpy(pstAck->aszNotifyID, aszNotifyID, NOTIFYID_LEN);
    SDStrncpy(pstAck->aszUserName, aszUserName, USERNAME_LEN);
    SDStrncpy(pstAck->aszZoneName, aszZoneName, USERNAME_LEN);
    return CLILS_LOGIN_EX_ACK;
}


UINT16 CCliPktBuilder::LogoutAck(UINT16 wErrCode)
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_LOGOUT_ACK));
    PKT_CLILS_LOGOUT_ACK* pstAck = (PKT_CLILS_LOGOUT_ACK*)(GetPacketBuffer());
    pstAck->wErrCode = wErrCode;
    return CLILS_LOGOUT_ACK;
}

UINT16 CCliPktBuilder::ChangePwdAck(UINT16 wErrCode)
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_CHANGE_PWD_ACK));
    PKT_CLILS_CHANGE_PWD_ACK* pstAck = (PKT_CLILS_CHANGE_PWD_ACK*)(GetPacketBuffer());
    pstAck->wErrCode = wErrCode;
    return CLILS_CHANGE_PWD_ACK;
}

UINT16 CCliPktBuilder::GetUserExpandAck(UINT16 wErrCode, DT_EXPAND_FUNC_PARAM_LST* stExpandLst)
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_GET_USER_EXPAND_ACK));
    PKT_CLILS_GET_USER_EXPAND_ACK* pstAck = (PKT_CLILS_GET_USER_EXPAND_ACK*)(GetPacketBuffer());
    pstAck->wErrCode = wErrCode;
    if ( stExpandLst != NULL)
    {
        pstAck->stExpandFuncParaLst = *stExpandLst;
    }

    return CLILS_GET_USER_EXPAND_ACK;
}
UINT16 CCliPktBuilder::BindMobileAck( UINT16 wErrCode )
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_BIND_MOBILE_ACK));
    PKT_CLILS_BIND_MOBILE_ACK* pstAck = (PKT_CLILS_BIND_MOBILE_ACK*)(GetPacketBuffer());
    memset(pstAck, 0x00, sizeof(PKT_CLILS_BIND_MOBILE_ACK));
    pstAck->wErrCode = wErrCode;
    return CLILS_BIND_MOBILE_ACK;
}

UINT16 CCliPktBuilder::BindMobileAck( UINT16 wErrCode , UINT16 wCountDown )
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_BIND_MOBILE_ACK));
    PKT_CLILS_BIND_MOBILE_ACK* pstAck = (PKT_CLILS_BIND_MOBILE_ACK*)(GetPacketBuffer());
    memset(pstAck, 0x00, sizeof(PKT_CLILS_BIND_MOBILE_ACK));
    pstAck->wCountdown = wCountDown;
    pstAck->wErrCode = wErrCode;
    return CLILS_BIND_MOBILE_ACK;
}

UINT16 CCliPktBuilder::UnBindMobileAck( UINT16 wErrCode )
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_UNBIND_MOBILE_ACK));
    PKT_CLILS_UNBIND_MOBILE_ACK* pstAck = (PKT_CLILS_UNBIND_MOBILE_ACK*)(GetPacketBuffer());
    memset(pstAck, 0x00, sizeof(PKT_CLILS_UNBIND_MOBILE_ACK));
    pstAck->wErrCode = wErrCode;
    return CLILS_UNBIND_MOBILE_ACK;
}

UINT16 CCliPktBuilder::UnBindMobileAck( UINT16 wErrcode, UINT16 wCountDown )
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_UNBIND_MOBILE_ACK));
    PKT_CLILS_UNBIND_MOBILE_ACK* pstAck = (PKT_CLILS_UNBIND_MOBILE_ACK*)(GetPacketBuffer());
    memset(pstAck, 0x00, sizeof(PKT_CLILS_UNBIND_MOBILE_ACK));
    pstAck->wCountdown = wCountDown;
    pstAck->wErrCode = wErrcode;
    return CLILS_UNBIND_MOBILE_ACK;
}

UINT16 CCliPktBuilder::BindCheckVerCode( UINT16 wErrCode, TCHAR aszMobile[MAX_MOBILE_LENGTH] )
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_BIND_CHECK_VER_CODE_ACK));
    PKT_CLILS_BIND_CHECK_VER_CODE_ACK* pstAck = (PKT_CLILS_BIND_CHECK_VER_CODE_ACK*)(GetPacketBuffer());
    memset(pstAck, 0x00, sizeof(PKT_CLILS_BIND_CHECK_VER_CODE_ACK));
    pstAck->wErrCode = wErrCode;
    SDStrncpy((CHAR*)pstAck->abyMObile, aszMobile, MAX_MOBILE_LENGTH);
    return CLILS_BIND_CHECK_VER_CODE_ACK;
}

UINT16 CCliPktBuilder::BindCheckVerCode( UINT16 wErrCode )
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_BIND_CHECK_VER_CODE_ACK));
    PKT_CLILS_BIND_CHECK_VER_CODE_ACK* pstAck = (PKT_CLILS_BIND_CHECK_VER_CODE_ACK*)(GetPacketBuffer());
    memset(pstAck, 0x00, sizeof(PKT_CLILS_BIND_CHECK_VER_CODE_ACK));
    pstAck->wErrCode = wErrCode;
    return CLILS_BIND_CHECK_VER_CODE_ACK;
}

UINT16 CCliPktBuilder::UnBindCheckVerCode( UINT16 wErrCode )
{
    ZeroPacketBuffer(sizeof(PKT_CLILS_UNBIND_CHECK_VER_CODE_ACK));
    PKT_CLILS_UNBIND_CHECK_VER_CODE_ACK* pstAck = (PKT_CLILS_UNBIND_CHECK_VER_CODE_ACK*)(GetPacketBuffer());
    memset(pstAck, 0x00, sizeof(PKT_CLILS_UNBIND_CHECK_VER_CODE_ACK));
    pstAck->wErrCode = wErrCode;
    return CLILS_UNBIND_CHECK_VER_CODE_ACK;
}
