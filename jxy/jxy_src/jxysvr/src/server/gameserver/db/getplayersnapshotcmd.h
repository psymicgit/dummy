///<------------------------------------------------------------------------------
//< @file:   gameserver\db\getplayersnapshotcmd.h
//< @author: hongkunan
//< @date:   2014年3月19日 21:54:49
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _getplayersnapshotcmd_h_
#define _getplayersnapshotcmd_h_

#include <db/basedbccmd.h>
#include <db/dbmgr.h>
#include <sddb.h>

class CGetPlayerSnapShotCMD : public CBaseDBCMD
{
public:
    CGetPlayerSnapShotCMD()
    {
    }

public:
    // 在数据库线程执行
    VOID SDAPI OnExecuteSql(SGDP::ISDDBConnection*);

    // 在逻辑线程执行
    VOID SDAPI OnExecuted();

    // @impl
    virtual VOID SetUserData(VOID* pUserData);

public:
    virtual EDBCmdType GetDBCmdType() { return EDBCT_GET_PLAYER_SNAP_SHOT; }

private:
    BOOL GetPlayerData(SGDP::ISDDBConnection *poDBConn, UINT32 dwPlayerID, DT_PLAYER_DATA &stPlayerData);

private:
    UINT32 m_dwPlayerID;
    DT_PLAYER_DATA m_stPlayerData;
};

#endif //_getplayersnapshotcmd_h_