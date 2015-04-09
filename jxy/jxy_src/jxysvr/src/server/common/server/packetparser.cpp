
#include "packetparser.h"

#include "sdpkg.h"

CCliPacketParser::CCliPacketParser()
{
}

CCliPacketParser::~CCliPacketParser()
{
}

INT32 CCliPacketParser::ParsePacket(const CHAR *pBuf, UINT32 dwLen)
{
    if (dwLen < sizeof(SDNetMsgHeader))
    {
        return 0;
    }

    SDNetMsgHeader* pNetMsgHeader = (SDNetMsgHeader*)pBuf;

	if(SDNtohs(pNetMsgHeader->wMark) != PKTHEADMAK)
	{
		return CheckNextHeader(pBuf, dwLen);
	}

    INT32 nPkgLen = SDNtohl( pNetMsgHeader->dwDataLen) + sizeof(SDNetMsgHeader);
    if (nPkgLen <= (INT32)dwLen)
    {
        return nPkgLen;
    }
    return 0;
}


INT32 CCliPacketParser::CheckNextHeader(const char *pBuf, UINT32 dwLen)
{
	INT32  i = 1;
	UINT16 wMark = 0;
	
	while (i < (INT32)(dwLen-1))
	{
		wMark = *(UINT16*)(pBuf+i);        
		if (PKTHEADMAK == wMark)
		{
			return i;
		}
		++i;
	}

	return dwLen;
}