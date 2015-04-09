///<------------------------------------------------------------------------------
//< @file:   gameserver\db\playersnapshotcmd.cpp
//< @author: hongkunan
//< @date:   2014年5月30日 11:7:46
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "playersnapshotcmd.h"
#include <framework/gsapi.h>
#include "db/autosqlbuf.h"

BOOL CSavePlayerSnapShotCMD::BuildSql(SGDP::ISDDBConnection *poDBConn, CHAR* pszSql)
{
    if(NULL == pszSql)
    {
        return FALSE;
    }

    pszSql[0] = '\0';

    INT32 nRet = m_stPlayerNoPvPData.EncodeSaveData(pszSql, MAX_SQL_LEN);
    if(nRet <= 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: Encode m_stRoundInfo.stVideo failed"), MSG_MARK);
        return FALSE;
    }

    pszSql[nRet] = '\0';

    const UINT32 dwBufSize = sizeof(DT_PLAYER_NOPVP_DATA);
    if(dwBufSize <= nRet)
    {
        return FALSE;
    }

    CAutoSqlBuf oAutoBuf;
    CHAR* pszEscapeBuf = oAutoBuf.GetSqlBuf();

    UINT32 dwRetCode = poDBConn->EscapeString(pszSql, nRet, pszEscapeBuf, MAX_SQL_LEN);
    if(dwRetCode <= 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CSavePlayerSnapShotCMD::BuildSql EscapeString failed, playerID = %u!"), MSG_MARK, sizeof(pszEscapeBuf));
        return FALSE;
    }

    // 64位是%llu
    UINT32 dwLen = sprintf(pszSql, "replace into player_snapshot(PlayerID, PlayerData, SizeOfPlayerData) values ('%u','%s', '%u');", 
        m_stPlayerNoPvPData.stBaseData.dwID,
        pszEscapeBuf,
        sizeof(DT_PLAYER_NOPVP_DATA)
    );

    pszSql[dwLen] = '\0';
    return TRUE;
}

VOID SDAPI CSavePlayerSnapShotCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    CAutoSqlBuf oAutoBuf;
    
    CHAR* pszSql = oAutoBuf.GetSqlBuf();
    if(!BuildSql(poDBConn, pszSql))
    {
        return;
    }

    ISDDBRecordSet *pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs( pszSql, &pRecordSet, NULL, &strError );
    if(pRecordSet)
    {
        pRecordSet->Release();
    }

    if(nRet < 0)
    {
        SYS_CRITICAL( _SDT( "[CSavePlayerSnapShotCMD::OnExecuteSql] exec SQL failed ,error = [%s], len of SQL = [%u]" ), strError.c_str(), SDStrlen(pszSql));
        return;
    }
}

VOID CSavePlayerSnapShotCMD::SetUserData(VOID* pUserData)
{
    DT_PLAYER_NOPVP_DATA *pstData = (DT_PLAYER_NOPVP_DATA*)pUserData;
    m_stPlayerNoPvPData = *pstData;
}

VOID CDelPlayerSnapShotCMD::SetUserData(VOID* pUserData)
{
    UINT32 *pPlayerID = (UINT32*)pUserData;
    m_dwPlayerID = *pPlayerID;
}

VOID SDAPI CDelPlayerSnapShotCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    CHAR szSql[256] = {0};

    if(0 == m_dwPlayerID)
    {
        sprintf(szSql, "truncate player_snapshot;");
    }
    else
    {
        sprintf(szSql, "delete from player_snapshot where PlayerID = %u;", m_dwPlayerID);
    }

    ISDDBRecordSet *pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSql, &pRecordSet, NULL, &strError );
    if(pRecordSet)
    {
        pRecordSet->Release();
    }

    if(nRet < 0)
    {
        SYS_CRITICAL( _SDT( "[%s %d] CDelPlayerSnapShotCMD::OnExecuteSql exec SQL failed ,error = [%s], SQL = [%s]" ), MSG_MARK, strError.c_str(), szSql);
        return;
    }
}