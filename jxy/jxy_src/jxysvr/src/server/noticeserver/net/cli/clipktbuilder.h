

#ifndef _CLI_PACKAGE_BUILDER_H_
#define _CLI_PACKAGE_BUILDER_H_

#include "sdframework.h"
#include <protoclizs.h>

class CCliPktBuilder : public CSDPacketBuilder
{
public:
    static CCliPktBuilder* Instance() { static CCliPktBuilder oCliPkgBuilder; return &oCliPkgBuilder;}
protected:
    CCliPktBuilder();
    ~CCliPktBuilder();
public:
	UINT16 EncryptDecryptKeyNtf(UINT8 abyKeyInfo[]);
};

#endif //#ifndef _GATE_SERVER_PACKAGE_BUILDER_H_

