///<------------------------------------------------------------------------------
//< @file:   logic\player\playersnapshotmgr.h
//< @author: hongkunan
//< @date:   2014年3月19日 20:58:27
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _playersnapshotmgr_h_
#define _playersnapshotmgr_h_

#include "logic/gvg/gvgdef.h"
#include <sdsingleton.h>
#include "logic/player/playermgr.h"
#include "protocommondata.h"

// DECLARE_CLASS_POOL_MGR(DT_PLAYER_NOPVP_DATA);

class CPlayerSnapShotMgr
{
    typedef std::map<UINT32, DT_PLAYER_NOPVP_DATA> CPlayerDataMap;

    DECLARE_SINGLETON(CPlayerSnapShotMgr)

public:
    BOOL Init();

public:
    BOOL LoadFromDB();

    BOOL SaveToDB();

    DT_PLAYER_NOPVP_DATA* FindPlayerData(UINT32 dwPlayerID);

    void AddPlayerData(DT_PLAYER_NOPVP_DATA &stPlayerNoPvpData);

    void AddPlayerSnapShot(UINT32 dwPlayerID);

    void CleanNoBattleData(DT_PLAYER_NOPVP_DATA &stPlayerNoPvpData);

    void Clear();

    CPlayer* FindPlayerCache(UINT32 dwPlayerID);
    CPlayer* CreatePlayerCache(UINT32 dwPlayerID);

    void ClearCache();

private:
    void PostDelPlayerCmd(UINT32 dwPlayerID);

private:
    CPlayerDataMap m_mapPlayerData;

    // 未保存的玩家数据
    CPlayerIDSet m_setNotSavedPlayer;

    // 缓存玩家列表
    CID2PlayerMap m_mapCachePlayer;
};

#endif //_playersnapshotmgr_h_