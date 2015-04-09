

#ifndef _LS_SERVER_PACKAGE_BUILDER_H_
#define _LS_SERVER_PACKAGE_BUILDER_H_

#include "sdframework.h"
#include <protocol/server/protolstlc.h>

class CLSPktBuilder : public CSDPacketBuilder
{
public:
    static CLSPktBuilder* Instance() { static CLSPktBuilder oLSPkgBuilder; return &oLSPkgBuilder;}
protected:
    CLSPktBuilder();
    ~CLSPktBuilder();
public:   
	UINT16 LoginAck(UINT16 wErrCode, PKT_LSTLC_AUTH_REQ& stReq);
};

#endif 

