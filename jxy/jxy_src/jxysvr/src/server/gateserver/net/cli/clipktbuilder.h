

#ifndef _CLI_PACKAGE_BUILDER_H_
#define _CLI_PACKAGE_BUILDER_H_

#include "sdframework.h"
#include <protocligt.h>

class CCliPktBuilder : public CSDPacketBuilder
{
public:
    static CCliPktBuilder* Instance()
    {
        static CCliPktBuilder oCliPkgBuilder;
        return &oCliPkgBuilder;
    }
protected:
    CCliPktBuilder();
    ~CCliPktBuilder();
public:
    UINT16 EnteGSAck(UINT16 wErrCode);
    UINT16 EncryptDecryptKeyNtf(UINT8 abyKeyInfo[]);
    UINT16 HeartbeatAck();

    UINT16 RegisterUserNameAck(UINT32 wErrCode, const TCHAR* pszUserName);
    UINT16 ServerNotStartupNtf(UINT8 byServerType);
    UINT16 KickOutNtf(UINT8 byKickOutType, CHAR* pszMsgInfo = NULL);

    UINT16 LoginAck(UINT32 wErrCode, DT_NOTICE_DATA* poDT_NOTICE_DATA);
    UINT16 LoginExAck(UINT32 wErrCode, PKT_CLILS_LOGIN_EX_REQ* pstReq, BOOL bIos7 = FALSE);

	UINT16 LoginExAck(UINT16 wErrCode, DT_NOTICE_DATA *poDT_NOTICE_DATA);

    UINT16 LoginExAck(UINT16 wErrCode, DT_VERSION_DATA  stVersionInfo, DT_NOTICE_DATA stNoticeInfo, UINT8 byOtherDeviceLoginFlag, UINT8 byAuthType,
                      TCHAR  aszDeviceID[DEVICEID_LEN], TCHAR aszNotifyID[NOTIFYID_LEN], TCHAR aszUserName[USERNAME_LEN], UINT16 wZoneID,
                      TCHAR aszZoneName[USERNAME_LEN], UINT32 dwUserID, UINT32 dwPlayerID, UINT16 wCareerID);
	UINT16 LogoutAck(UINT16 wErrCode);
	UINT16 ChangePwdAck(UINT16 wErrCode);
	UINT16 GetUserExpandAck(UINT16 wErrcode, DT_EXPAND_FUNC_PARAM_LST* stExpandLst);
    UINT16 BindMobileAck(UINT16 wErrCode);

    UINT16 BindMobileAck(UINT16 wErrCode, UINT16 wCountDown);

    UINT16 UnBindMobileAck(UINT16 wErrCode);

    UINT16 UnBindMobileAck(UINT16 wErrcode, UINT16 wCountDown);

    UINT16 BindCheckVerCode(UINT16 wErrCode);

    UINT16 BindCheckVerCode(UINT16 wErrCode, TCHAR aszMobile[MAX_MOBILE_LENGTH]);

    UINT16 UnBindCheckVerCode(UINT16 wErrCode);
};

#endif //#ifndef _GATE_SERVER_PACKAGE_BUILDER_H_

