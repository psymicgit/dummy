

#ifndef _PACKETPARSER_H_
#define _PACKETPARSER_H_

#include <sdnet.h>

using namespace SGDP;

class CCliPacketParser: public ISDPacketParser
{
#define SDLOG_MSG_HEAD_LEN            4
#define SDLOG_MSG_TIMESTAMP_SIZE      20
public:
    CCliPacketParser();
    ~CCliPacketParser();
public:
    virtual INT32 SDAPI ParsePacket(const CHAR* pBuf, UINT32 dwLen);
protected:
	INT32 CheckNextHeader(const char *pBuf, UINT32 dwLen);
};

#endif //#ifndef _PACKETPARSER_H_

