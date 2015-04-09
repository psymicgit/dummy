///<------------------------------------------------------------------------------
//< @file:   pkpktbuilder.h
//< @author: hongkunan
//< @date:   2014Äê1ÔÂ15ÈÕ 16:36:2
//< @brief:
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _pkpktbuilder_h_
#define _pkpktbuilder_h_

#include "sdpkg.h"
#include "logic/gvg/global/pullgroup.h"

#include "logic/faction/factionmgr.h"

using namespace SGDP;

class CPKPktBuilder : public CSDPacketBuilder
{
public:
    static CPKPktBuilder* Instance()
    {
        static CPKPktBuilder oGTPkgBuilder;
        return &oGTPkgBuilder;
    }

public:
    BOOL BuildRetPlayer(DT_SYNC_PLAYER &stSyncPlayer, CPlayer *poPlayer);

public:
    VOID SyncPlayer(UINT32 dwPKGroupID, CPlayer*);

    UINT16 NotifyNextRound(GVG_STAGE);
    UINT16 RetTopFactions();
    UINT16 RetFactions(UINT32 adwFactionIDList[], BYTE byFactionCnt);

    UINT16 GetPvPVideoReq();
    UINT16 RetPvPVideo(VideoID aVideoIDList[], UINT16 wVideoCnt);

    void GetGvGActivityReq();
};

#endif //_pkpktbuilder_h_