
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
	INT32 nPos = strTmp.find("http");
	if(nPos > 0)
	{
		return nPos;
	}
	//else
	//{
	//	nPos = strTmp.find("HTTP");
	//	if(nPos > 0)
	//	{
	//		return nPos;
	//	}
	//}

	return	dwLen;
}

