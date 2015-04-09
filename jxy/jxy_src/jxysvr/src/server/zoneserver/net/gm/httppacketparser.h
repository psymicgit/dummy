

#ifndef _HTTPCLIPACKETPARSER_H_
#define _HTTPCLIPACKETPARSER_H_

#include <sdutil.h>
#include <sdframework.h>
#include <sdnet.h>

class CHttpCliPacketParser: public ISDPacketParser
{
#define SDLOG_MSG_HEAD_LEN            4
#define SDLOG_MSG_TIMESTAMP_SIZE      20
public:
    CHttpCliPacketParser();
    ~CHttpCliPacketParser();
public:
    virtual INT32 SDAPI ParsePacket(const CHAR* pBuf, UINT32 dwLen);
protected:

};

#endif //#ifndef _PACKETPARSER_H_

