

#ifndef _JSONPACKETPARSER_H_
#define _JSONPACKETPARSER_H_

#include <sdutil.h>
#include <sdframework.h>
#include <sdnet.h>

class CJsonPacketParser: public ISDPacketParser
{
public:
    CJsonPacketParser();
    ~CJsonPacketParser();
public:
    virtual INT32 SDAPI ParsePacket(const CHAR* pBuf, UINT32 dwLen);
};

#endif //#ifndef _PACKETPARSER_H_

