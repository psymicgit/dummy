#ifndef _VER_CHECK_
#define _VER_CHECK_

#include "sdbase.h"

namespace SGDP
{

UINT32  VerToDWORD(const SSGDPVersion* pstVersion);
BOOL  VerCheck(const SSGDPVersion* pRequestVer, const SSGDPVersion* pRealVer);

}//end namespace

#endif

