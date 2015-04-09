

#ifndef _LS_SERVER_PACKAGE_BUILDER_H_
#define _LS_SERVER_PACKAGE_BUILDER_H_

#include "sdframework.h"
#include <protocol/server/protogtls.h>

class CLSPktBuilder : public CSDPacketBuilder
{
public:
    static CLSPktBuilder* Instance() { static CLSPktBuilder oLSPkgBuilder; return &oLSPkgBuilder;}
protected:
    CLSPktBuilder();
    ~CLSPktBuilder();
public:   
    UINT16 LoginAck(PKT_GTLS_LOGIN_ACK& stAck, BOOL bServer);
	UINT16 LoginExAck(PKT_GTLS_LOGIN_EX_ACK& stAck, BOOL bServer);
};

#endif //#ifndef _GATE_SERVER_PACKAGE_BUILDER_H_

