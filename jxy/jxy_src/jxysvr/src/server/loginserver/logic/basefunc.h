
#ifndef _BASEFUNC_H_
#define _BASEFUNC_H_

#include <sdtype.h>

class CBaseFunc
{
public:
	//¼ì²é²ÎÊý
	static UINT16 CkAuthParam(TCHAR* pszDeviceID, UINT8  byAuthType, TCHAR* pszNotifyID,TCHAR* pszUserName,TCHAR* pszUserPwd,UINT16 wCareerID, UINT16 wZoneID);
};

#endif 

