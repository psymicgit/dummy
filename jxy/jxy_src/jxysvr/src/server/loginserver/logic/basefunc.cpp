#include "basefunc.h"
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <sdstring.h>
#include <protocol/server/protocommondata.h>


UINT16 CBaseFunc::CkAuthParam(TCHAR* pszDeviceID, UINT8  byAuthType, TCHAR* pszNotifyID,TCHAR* pszUserName,TCHAR* pszUserPwd,UINT16 wCareerID, UINT16 wZoneID)
{
	//检查各个参数是否有sql嵌入攻击
	if((HaveSqlToken(pszDeviceID)) || 
		(HaveSqlToken(pszNotifyID)) ||
		(HaveSqlToken(pszUserName))		
		)
	{
		return ERR_LOGIN::ID_INVALID_CHARACTER;		
	}
	if((AUTH_TYPE_TONGBU != byAuthType) && (HaveSqlToken(pszUserPwd)))
	{
		return ERR_LOGIN::ID_INVALID_CHARACTER;		
	}

    if ((AUTH_TYPE_DEVECEID != byAuthType) &&
            (AUTH_TYPE_TONGBU != byAuthType) &&
			(AUTH_TYPE_91 != byAuthType) && 
            (AUTH_TYPE_MAXNET != byAuthType))
    {
        return ERR_LOGIN::ID_INVALID_CHARACTER;
    }


    //判断职业合法性
    if((0 == wCareerID) || (wCareerID > 6))
    {
        return ERR_LOGIN::ID_INVALID_CAREER;
    }

	//判断区ID合法性
	if(0 == wZoneID)
	{
		return ERR_LOGIN::ID_INVALID_ZONEID;	
	}

	//用户名和设备号都为空,认证失败
	if((0 == _SDTStrcasecmp(pszUserName, _SDT(""))) && (0 == _SDTStrcasecmp(pszDeviceID, _SDT(""))))
	{
		return ERR_LOGIN::ID_ALL_EMPTY_ERR;		
	}

	return ERR_LOGIN::ID_SUCCESS;

}
