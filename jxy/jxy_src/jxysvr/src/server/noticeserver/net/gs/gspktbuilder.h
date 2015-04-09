

#ifndef _GS_PACKAGE_BUILDER_H_
#define _GS_PACKAGE_BUILDER_H_

#include "sdframework.h"
#include <protoclizs.h>

class CGSPktBuilder : public CSDPacketBuilder
{
public:
    static CGSPktBuilder* Instance() { static CGSPktBuilder oCliPkgBuilder; return &oCliPkgBuilder;}
protected:
    CGSPktBuilder();
    ~CGSPktBuilder();
public:
};

#endif //#ifndef _GATE_SERVER_PACKAGE_BUILDER_H_

