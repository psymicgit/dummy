

#ifndef _GS_SERVER_PACKAGE_BUILDER_H_
#define _GS_SERVER_PACKAGE_BUILDER_H_

#include "sdframework.h"


class CGSPktBuilder : public CSDPacketBuilder
{
public:
    static CGSPktBuilder* Instance() { static CGSPktBuilder oGSPkgBuilder; return &oGSPkgBuilder;}
protected:
    CGSPktBuilder();
    ~CGSPktBuilder();
public:   
    UINT16 EnterGSReq(TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT16 wZoneID, UINT16 wOriZoneID, UINT32 dwUserID, UINT32 dwPlayerID, UINT16 wCareerID, UINT32 dwLoginIP, UINT32 dwCliVer, UINT8 byAuthType, UINT8 byDaiChong = 0);
	UINT16 RegisterUserNameReq(TCHAR* pszUserName);
	UINT16 KickOutAck(TCHAR* pszErrMsg);
};

#endif //#ifndef _GATE_SERVER_PACKAGE_BUILDER_H_

