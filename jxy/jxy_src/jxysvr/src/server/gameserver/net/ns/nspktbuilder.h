

#ifndef _NS_SERVER_PACKAGE_BUILDER_H_
#define _NS_SERVER_PACKAGE_BUILDER_H_

#include "sdpkg.h"
#include <common/client/errdef.h>

class CPlayer;

using namespace SGDP;

class CNSPktBuilder : public CSDPacketBuilder
{
public:
    static CNSPktBuilder* Instance()
    {
        static CNSPktBuilder oGTPkgBuilder;
        return &oGTPkgBuilder;
    }
protected:
    CNSPktBuilder();
    ~CNSPktBuilder();

public:

    UINT16 EnterGSAck(UINT16 wErrCode, CPlayer* poPlayer = NULL);

};

#endif //#ifndef _GATE_SERVER_PACKAGE_BUILDER_H_

