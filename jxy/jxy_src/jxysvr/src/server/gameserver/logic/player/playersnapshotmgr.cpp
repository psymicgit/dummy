///<------------------------------------------------------------------------------
//< @file:   logic\player\playersnapshotmgr.cpp
//< @author: hongkunan
//< @date:   2014年3月19日 20:58:43
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "playersnapshotmgr.h"

#include "logic/player/playersavemgr.h"
#include "framework/gsapi.h"
#include "db/dbmgr.h"
#include "db/basedbccmd.h"
#include "logic/gvg/gvgpropmgr.h"

// IMPLEMENT_CLASS_POOL_MGR(DT_PLAYER_DATA);

IMPLEMENT_SINGLETON(CPlayerSnapShotMgr)

BOOL CPlayerSnapShotMgr::Init()
{
    m_mapPlayerData.clear();
    m_setNotSavedPlayer.clear();
    m_mapCachePlayer.clear();

    LoadFromDB();
    return TRUE;
}

BOOL CPlayerSnapShotMgr::LoadFromDB()
{
    if(!CGvGPropMgr::Instance()->GetGvGProp().bUsePlayerSnapShot)
    {
        return TRUE;
    }

    SGDP::ISDDBSession*	pDBSession = gsapi::GetDBMgr()->GetDBSession();
    if(NULL == pDBSession)
    {
        return FALSE;
    }

    SGDP::ISDDBRecordSet* pRes = NULL;
    string strError;

    INT32 nRet = pDBSession->ExecuteSqlRs("select * from player_snapshot", &pRes, 0, &strError);
    if(SDDB_NO_RECORDSET != nRet && SDDB_HAS_RECORDSET != nRet)
    {
        return FALSE;
    }

    DT_PLAYER_NOPVP_DATA stPlayerData;
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRes->GetRecord()) // 当有行结果时
        {
            UINT32 dwPlayerID     = SDAtou(pRes->GetFieldValueByName("PlayerID"));
            UINT32 dwSizeOfPlayer = SDAtou(pRes->GetFieldValueByName("SizeOfPlayerData"));

            // 如果之前存储的sizeof(DT_PLAYER_NOPVP_DATA)的值与现在不一致，则说明结构体发生过变动，此时直接解码很有可能会导致服务器崩溃，应直接跳过
            if(dwSizeOfPlayer != sizeof(DT_PLAYER_NOPVP_DATA))
            {
                USR_INFO( _SDT("[%s: %d] detecting sizeof(DT_PLAYER_NOPVP_DATA) has changed, before is %u, now is = %u, player id = <%u>!"), 
                    MSG_MARK, dwSizeOfPlayer, sizeof(DT_PLAYER_NOPVP_DATA), dwPlayerID);

                PostDelPlayerCmd(dwPlayerID);
                continue;
            }

            UINT32 dwLen = pRes->GetFieldLengthByName( "PlayerData" );

            // 先尝试解码
            if(-1 == stPlayerData.DecodeSaveData(pRes->GetFieldValueByName("PlayerData"), dwLen))
            {
                SYS_CRITICAL( _SDT("[%s: %d] PlayerData.DecodeSaveData Failed, player id = <%u>, length = %u!"), MSG_MARK, dwPlayerID, dwLen);
                continue;
            }

            AddPlayerData(stPlayerData);
        }
    }

    if(pRes)
    {
        pRes->Release();
    }

    return TRUE;
}

void CPlayerSnapShotMgr::Clear()
{
    m_mapPlayerData.clear();
    m_setNotSavedPlayer.clear();

    PostDelPlayerCmd(0);
    ClearCache();    
}

void CPlayerSnapShotMgr::ClearCache()
{
    for(CID2PlayerMap::iterator itr = m_mapCachePlayer.begin(); itr != m_mapCachePlayer.end(); ++itr)
    {
        CPlayer *poPlayer = itr->second;
        CPlayerPoolMgr::Instance()->Remove(poPlayer);
    }

    m_mapCachePlayer.clear();
}

CPlayer* CPlayerSnapShotMgr::CreatePlayerCache(UINT32 dwPlayerID)
{
    if(!CGvGPropMgr::Instance()->GetGvGProp().bUsePlayerSnapShot)
    {
        return NULL;
    }

    CPlayer* poPlayer = FindPlayerCache(dwPlayerID);
    if(poPlayer)
    {
        return poPlayer;
    }

    DT_PLAYER_NOPVP_DATA *pstPlayerNoPvPData = FindPlayerData(dwPlayerID);
    if (NULL == pstPlayerNoPvPData)
    {
        return NULL;
    }

    poPlayer = CPlayerPoolMgr::Instance()->Create();
    if(NULL == poPlayer)
    {
        return NULL;
    }

    DT_PLAYER_DATA stPlayerData;
    memset(&stPlayerData, 0, sizeof(stPlayerData));

    stPlayerData.stPlayerNoPvpData = *pstPlayerNoPvPData;

    //设置玩家对象
    if(!poPlayer->Init(stPlayerData))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CPlayerSnapShotMgr::CreatePlayer Player Init failed!PlayerID[%d]."), MSG_MARK, dwPlayerID);
        delete poPlayer;

        return NULL;
    }

    m_mapCachePlayer[dwPlayerID] = poPlayer;

    //增加日志
    DBG_INFO(_SDT("[%s: %d]: CPlayerSnapShotMgr::CreatePlayer PlayerID:%u, address:%0x"), MSG_MARK, dwPlayerID, poPlayer);
    return poPlayer;
}

CPlayer* CPlayerSnapShotMgr::FindPlayerCache(UINT32 dwPlayerID)
{
    CID2PlayerMapItr itr = m_mapCachePlayer.find(dwPlayerID);
    if(itr != m_mapCachePlayer.end())
    {
        return itr->second;
    }

    return NULL;
}

void CPlayerSnapShotMgr::PostDelPlayerCmd(UINT32 dwPlayerID)
{
    // 当玩家ID为0时，表示清空玩家快照表中的全部记录
    CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_DEL_PLAYER_SNAP_SHOT);
    if ( NULL == poCMD)
    {
        SYS_CRITICAL("[%s:%d] CPlayerSnapShotMgr::PostDelPlayerCmd failed, no enough memory, PlayerID = %lu", MSG_MARK, dwPlayerID);
        return;
    }

    poCMD->SetUserData((VOID*)&dwPlayerID);
    gsapi::GetDBMgr()->AddCommand(poCMD);
}

DT_PLAYER_NOPVP_DATA* CPlayerSnapShotMgr::FindPlayerData(UINT32 dwPlayerID)
{
    if(!CGvGPropMgr::Instance()->GetGvGProp().bUsePlayerSnapShot)
    {
        return NULL;
    }

    CPlayerDataMap::iterator itr = m_mapPlayerData.find(dwPlayerID);
    if(itr == m_mapPlayerData.end())
    {
        return NULL;
    }

    DT_PLAYER_NOPVP_DATA *pstData = &itr->second;
    return pstData;
}

BOOL CPlayerSnapShotMgr::SaveToDB()
{
    m_setNotSavedPlayer.erase(0);

    for(CPlayerIDSet::iterator itr = m_setNotSavedPlayer.begin(); itr != m_setNotSavedPlayer.end(); ++itr)
    {
        UINT32 dwPlayerID = *itr;
        DT_PLAYER_NOPVP_DATA *pstPlayerData = FindPlayerData(dwPlayerID);
        if(NULL == pstPlayerData)
        {
            SYS_CRITICAL( _SDT("[%s: %d] could not find DT_PLAYER_DATA, playerid = %u!"), MSG_MARK, dwPlayerID);
            continue;
        }

        CBaseDBCMD* poCMD = gsapi::GetDBMgr()->CreateCmd(EDBCT_SAVE_PLAYER_SNAP_SHOT);
        if(NULL == poCMD)
        {
            SYS_CRITICAL( _SDT("[%s: %d]: create new dbcmd EDBCT_SAVE_PLAYER_SNAP_SHOT faild!"), MSG_MARK);
            continue;
        }

        gsapi::GetDBMgr()->AddCommand(poCMD);
        poCMD->SetUserData(pstPlayerData);
    }

    m_setNotSavedPlayer.clear();
    return TRUE;
}

void CPlayerSnapShotMgr::AddPlayerData(DT_PLAYER_NOPVP_DATA &stNewPlayer)
{
    UINT32 dwPlayerID = stNewPlayer.stBaseData.dwID;
    CleanNoBattleData(stNewPlayer);

    DT_PLAYER_NOPVP_DATA *pstOldPlayer = FindPlayerData(dwPlayerID);
    if(NULL == pstOldPlayer)
    {
        m_setNotSavedPlayer.insert(dwPlayerID);
        m_mapPlayerData[dwPlayerID] = stNewPlayer;
    }
    else
    {
        BOOL bDiff = memcmp(pstOldPlayer, &stNewPlayer, sizeof(stNewPlayer));
        if (bDiff)
        {
            *pstOldPlayer = stNewPlayer;
            m_setNotSavedPlayer.insert(dwPlayerID);
        }
    }

    if(m_setNotSavedPlayer.size() >= 10)
    {
        SaveToDB();
    }
}

void CPlayerSnapShotMgr::AddPlayerSnapShot(UINT32 dwPlayerID)
{
    if(0 == dwPlayerID)
    {
        return;
    }

    CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(dwPlayerID);
    if(NULL == poPlayer)
    {
        //内存中查找不到，需要到数据库中去查询
        CGetPlayerDataMgr::Instance()->GetPlayerData(dwPlayerID, GET_PLAYER_SNAPSHOT, dwPlayerID);
        return;
    }

    DT_PLAYER_NOPVP_DATA stNewPlayer;
    memset(&stNewPlayer, 0, sizeof(stNewPlayer));

    if(!CPlayerSaveMgr::Instance()->SavePlayerNoPvPData(poPlayer, stNewPlayer))
    {
        return;
    }

    AddPlayerData(stNewPlayer);
}

void CPlayerSnapShotMgr::CleanNoBattleData(DT_PLAYER_NOPVP_DATA &stNoPvP)
{
    memset(&stNoPvP.stTaskData,     0, sizeof(stNoPvP.stTaskData));
    memset(&stNoPvP.stLocaleData,   0, sizeof(stNoPvP.stLocaleData));
    memset(&stNoPvP.stOnlineEncRecdData, 0, sizeof(stNoPvP.stOnlineEncRecdData));
    memset(&stNoPvP.stLoginEncRecdData, 0, sizeof(stNoPvP.stLoginEncRecdData));
    memset(&stNoPvP.stInstanceData, 0, sizeof(stNoPvP.stInstanceData));
    memset(&stNoPvP.stMoneyTreeData, 0, sizeof(stNoPvP.stMoneyTreeData));
    memset(&stNoPvP.stOnhookRcdData, 0, sizeof(stNoPvP.stOnhookRcdData));
    memset(&stNoPvP.stFewParamData, 0, sizeof(stNoPvP.stFewParamData));
    memset(&stNoPvP.stCropData, 0, sizeof(stNoPvP.stCropData));
    memset(&stNoPvP.stEatBreadInfo, 0, sizeof(stNoPvP.stEatBreadInfo));
    memset(&stNoPvP.stYabiaoInfo, 0, sizeof(stNoPvP.stYabiaoInfo));
    memset(&stNoPvP.stClimbTowerData, 0, sizeof(stNoPvP.stClimbTowerData));
    memset(&stNoPvP.stVipExtData, 0, sizeof(stNoPvP.stVipExtData));
    memset(&stNoPvP.stLuaActivityRecordData, 0, sizeof(stNoPvP.stLuaActivityRecordData));
    memset(&stNoPvP.stGasCollectData, 0, sizeof(stNoPvP.stGasCollectData));

    memset(&stNoPvP.stBossBattlePlayerDataInfo, 0, sizeof(stNoPvP.stBossBattlePlayerDataInfo));
    memset(&stNoPvP.stActivityEncourageData, 0, sizeof(stNoPvP.stActivityEncourageData));
    memset(&stNoPvP.stRdChallengeData, 0, sizeof(stNoPvP.stRdChallengeData));
    memset(&stNoPvP.stWorshipData, 0, sizeof(stNoPvP.stWorshipData));
    memset(&stNoPvP.stScoreData, 0, sizeof(stNoPvP.stScoreData));
    memset(&stNoPvP.stGatherScienceData, 0, sizeof(stNoPvP.stGatherScienceData));
    memset(&stNoPvP.stBegBlueData, 0, sizeof(stNoPvP.stBegBlueData));
    memset(&stNoPvP.stTodayConsume, 0, sizeof(stNoPvP.stTodayConsume));

    memset(&stNoPvP.stFriendsData, 0, sizeof(stNoPvP.stFriendsData));
    memset(&stNoPvP.stExchangeResultLog, 0, sizeof(stNoPvP.stExchangeResultLog));
    memset(&stNoPvP.stGvGData, 0, sizeof(stNoPvP.stGvGData));
}