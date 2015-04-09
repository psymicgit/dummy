

#ifndef _KS_SERVER_PACKAGE_BUILDER_H_
#define _KS_SERVER_PACKAGE_BUILDER_H_

#include "sdpkg.h"
#include <common/client/errdef.h>

class CPlayer;

using namespace SGDP;

class CKSPktBuilder : public CSDPacketBuilder
{
public:
    static CKSPktBuilder* Instance()
    {
        static CKSPktBuilder oGTPkgBuilder;
        return &oGTPkgBuilder;
    }
protected:
    CKSPktBuilder();
    ~CKSPktBuilder();

public:

    UINT16 EnterGSAck(UINT16 wErrCode, CPlayer* poPlayer = NULL);

};

#endif //#ifndef _GATE_SERVER_PACKAGE_BUILDER_H_

