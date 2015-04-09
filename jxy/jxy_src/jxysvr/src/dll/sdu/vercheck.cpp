#include "vercheck.h"
#include <memory.h>

namespace SGDP
{
	UINT32  VerToDWORD(const SSGDPVersion* pstVersion)
	{
		UINT32 dwValue;
		char* p = (char*)&dwValue;
		memcpy(p,&pstVersion->wMinorVersion,sizeof(UINT16));
		memcpy(p + sizeof(UINT16),&pstVersion->wMajorVersion,sizeof(UINT16));
		return dwValue;
	}

	BOOL  VerCheck(const SSGDPVersion * pRequestVer, const SSGDPVersion * pRealVer)
	{
		if( !pRequestVer ) return TRUE;
		if( !pRealVer ) return FALSE;

		UINT16 dwUser,dwDll;

		dwUser = VerToDWORD(pRequestVer);
		dwDll = VerToDWORD(pRealVer);

		if (dwDll == dwUser)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
        return TRUE;
	}

}//end namespace

