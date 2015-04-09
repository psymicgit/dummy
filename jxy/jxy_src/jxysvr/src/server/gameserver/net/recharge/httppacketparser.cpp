
#include "httppacketparser.h"

CHttpCliPacketParser::CHttpCliPacketParser()
{
}

CHttpCliPacketParser::~CHttpCliPacketParser()
{
}

INT32 CHttpCliPacketParser::ParsePacket(const CHAR *pBuf, UINT32 dwLen)
{
	string strTmp = pBuf;
	INT32 nPos = strTmp.find("\r\n\r\n");
	if(nPos > 0)
	{
		return nPos + 4;
	}
	else
	{
		return 0;
	}
}

