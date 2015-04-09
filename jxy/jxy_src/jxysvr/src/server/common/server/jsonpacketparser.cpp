
#include "jsonpacketparser.h"

CJsonPacketParser::CJsonPacketParser()
{

}

CJsonPacketParser::~CJsonPacketParser()
{

}

INT32 CJsonPacketParser::ParsePacket(const CHAR *pBuf, UINT32 dwLen)
{
    UINT32		v_nStartFlag = 0;
    UINT32		v_nEndFlag = 0;
    for (UINT32 i = 0; i < dwLen; i++)
    {
        if ('{' == pBuf[i])
        {
            v_nStartFlag++;
        }
        if ('}' == pBuf[i])
        {
            v_nEndFlag++;
        }
        if (0 != v_nStartFlag && v_nStartFlag == v_nEndFlag)
        {
            return i + 1;
        }
    }
    return 0;
}


