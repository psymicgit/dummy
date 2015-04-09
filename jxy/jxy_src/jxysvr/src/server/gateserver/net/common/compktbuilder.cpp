
#include "compktbuilder.h"

CCOMPktBuilder::CCOMPktBuilder()
{
}

CCOMPktBuilder::~CCOMPktBuilder()
{
}

UINT16 CCOMPktBuilder::CliDownRpt()
{
    ZeroPacketBuffer();
    PKT_GTCOM_CLIENT_DOWN_RPT* pstNtf = (PKT_GTCOM_CLIENT_DOWN_RPT*)(GetPacketBuffer());
    return GTCOM_CLIENT_DOWN_RPT;
}

