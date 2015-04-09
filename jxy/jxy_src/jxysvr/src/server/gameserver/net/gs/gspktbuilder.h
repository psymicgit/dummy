

#ifndef _GS_SERVER_PACKAGE_BUILDER_H_
#define _GS_SERVER_PACKAGE_BUILDER_H_

#include "sdpkg.h"
#include <common/client/errdef.h>

using namespace SGDP;
class CPlayer;

class CGSPktBuilder : public CSDPacketBuilder
{
public:
    static CGSPktBuilder* Instance()
    {
        static CGSPktBuilder oGTPkgBuilder;
        return &oGTPkgBuilder;
    }
protected:
    CGSPktBuilder();
    ~CGSPktBuilder();

public:

	UINT16 StartSyncPlayer(UINT64 qwServerStartTime);
    UINT16 SyncPlayer(CPlayer* poPlayer);
	UINT16 SyncPlayerRpt(UINT16 dwPlayerID, UINT16 wErrCode);
	UINT16 SyncDspNameRpt(UINT16 dwLastPlayerID, UINT16 wErrCode);

};

#endif //#ifndef _GATE_SERVER_PACKAGE_BUILDER_H_

