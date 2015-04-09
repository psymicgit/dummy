

#ifndef _COM_PACKAGE_BUILDER_H_
#define _COM_PACKAGE_BUILDER_H_

#include "sdframework.h"
#include "protocol/server/protogtcommon.h"

class CCOMPktBuilder : public CSDPacketBuilder
{
public:
    static CCOMPktBuilder* Instance() { static CCOMPktBuilder oCOMPkgBuilder; return &oCOMPkgBuilder;}
    
protected:
    CCOMPktBuilder();
    ~CCOMPktBuilder();
public:
    UINT16 CliDownRpt();
};

#endif //#ifndef _GATE_SERVER_PACKAGE_BUILDER_H_

