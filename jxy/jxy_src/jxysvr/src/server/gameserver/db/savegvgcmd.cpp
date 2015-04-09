///<------------------------------------------------------------------------------
//< @file:   gameserver\db\savegvgcmd.cpp
//< @author: hongkunan
//< @date:   2014年2月17日 22:11:43
//< @brief:  
//< Copyright (c) 2014 heihuo. All rights reserved.
///<------------------------------------------------------------------------------

#include "savegvgcmd.h"
#include "db/dbcommoperate.h"
#include "logic/gvg/global/gvgarena.h"
#include "db/autosqlbuf.h"

VOID SDAPI CSaveGvGActivityCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    CAutoSqlBuf oAutoSql;
    CAutoSqlBuf oAutoBuf;
    CAutoSqlBuf oAutoTmp;

    TCHAR* pszSql = oAutoSql.GetSqlBuf();
    TCHAR* pszBuff = oAutoBuf.GetSqlBuf();
    TCHAR* pszTmp = oAutoTmp.GetSqlBuf();
    INT32 nRet = 0;

    if(NULL == pszSql || NULL == pszBuff || NULL == pszTmp)
    {
        SYS_CRITICAL( _SDT( "[%s %d] CSaveGvGCMD::OnExecuteSql alloc sqlbuf failed, not enough memory" ), MSG_MARK);
        return;
    }

    CSDDateTime oDate(m_stAct.qwStartTime);
    tstring strDate = SDTimeToString(oDate);

    // 这里可以直接用replace into，不用on duplicate key
    SDSnprintf(pszSql, MAX_SQL_LEN - 1, "replace into gvgactivity(SessionID,StartTime,CurStage,ArenaCnt) values (%u, '%s', %u, %u)",
        m_stAct.wSessionID, strDate.c_str(), m_stAct.byCurStage, m_stAct.byArenaCnt);
    
    ISDDBRecordSet *pRecordSet = NULL;
    string strError;
    nRet = poDBConn->ExecuteSqlRs( pszSql, &pRecordSet, NULL, &strError );
    if(pRecordSet)
    {
        pRecordSet->Release();
    }

    if(nRet < 0)
    {
        SYS_CRITICAL( _SDT( "[CSaveGvGActivityCMD::OnExecuteSql] exec SQL failed ,error = [%s], len of SQL = [%u]" ), MSG_MARK, strError.c_str(), SDStrlen(pszSql));
        return;
    }
}

VOID CSaveGvGActivityCMD::SetUserData(VOID* pUserData)
{
    DT_GVG_ACTIVITY_DATA *pstData = (DT_GVG_ACTIVITY_DATA*)pUserData;
    m_stAct = *pstData;
}

VOID SDAPI CSaveGvGActivityCMD::OnExecuted()
{
}

BOOL CSaveGvGArenaCMD::SaveData(SGDP::ISDDBConnection *poDBConn)
{
    CAutoSqlBuf oAutoSql;
    CAutoSqlBuf oAutoBuf;
    CAutoSqlBuf oAutoTmp;

    TCHAR* pszSql = oAutoSql.GetSqlBuf();
    TCHAR* pszBuff = oAutoBuf.GetSqlBuf();
    TCHAR* pszTmp = oAutoTmp.GetSqlBuf();
    INT32 nRet = 0;

    if(NULL == pszSql || NULL == pszBuff || NULL == pszTmp)
    {
        SYS_CRITICAL( _SDT( "[%s %d] CSaveGvGArenaCMD::OnExecuteSql alloc sqlbuf failed, not enough memory" ), MSG_MARK);
        return FALSE;
    }

    pszSql[0] = '\0';
    SDSnprintf(pszSql, MAX_SQL_LEN - 1, "insert into gvgarena(ArenaID,CurStage,IsDone,ZoneCnt,ZoneListData,GroupZoneIDList,War16To8To4Data,FinalWarData,FactionsGvGLogData,StageDoneFlagList) values (%u, %u, %u, %u",
        m_stArena.wArenaID, m_stArena.byCurStage, m_stArena.byIsDone, m_stArena.stZoneIDList.byZoneCnt);

    pszSql[MAX_SQL_LEN - 1] = '\0';

    INSERT_STRUCT( ZoneListData, m_stArena.stZoneIDList);
    INSERT_STRUCT( GroupZoneIDList, m_stArena.stGroupZoneIDList);
    INSERT_STRUCT( War16To8To4Data, m_stArena.stWar16To8List);
    INSERT_STRUCT( FinalWarData, m_stArena.stFinalWarInfo);
    INSERT_STRUCT( FactionsGvGLogData, m_stArena.stFactionsLog);
    INSERT_STRUCT( StageDoneFlagList, m_stArena.stStageDoneFlagList);

    SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ")  on duplicate key update gvgarena.ArenaID = values(gvgarena.ArenaID),\
gvgarena.IsDone = values(gvgarena.IsDone),\
gvgarena.CurStage = values(gvgarena.CurStage),\
gvgarena.ZoneCnt = values(gvgarena.ZoneCnt),\
gvgarena.ZoneListData = values(gvgarena.ZoneListData),\
gvgarena.GroupZoneIDList = values(gvgarena.GroupZoneIDList),\
gvgarena.War16To8To4Data = values(gvgarena.War16To8To4Data),\
gvgarena.FinalWarData = values(gvgarena.FinalWarData),\
gvgarena.FactionsGvGLogData = values(gvgarena.FactionsGvGLogData),\
gvgarena.StageDoneFlagList = values(gvgarena.StageDoneFlagList);");

    ISDDBRecordSet *pRecordSet = NULL;
    string strError;
    nRet = poDBConn->ExecuteSqlRs( pszSql, &pRecordSet, NULL, &strError );
    if(pRecordSet)
    {
        pRecordSet->Release();
    }

    if(nRet < 0)
    {
        SYS_CRITICAL( _SDT( "[%s %d] CSaveGvGArenaCMD::SaveData exec SQL failed ,error = [%s], len of SQL = [%u]" ), MSG_MARK, strError.c_str(), SDStrlen(pszSql));
        return FALSE;
    }

    return TRUE;
}

VOID SDAPI CSaveGvGArenaCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    SaveData(poDBConn);
}

VOID CSaveGvGArenaCMD::SetUserData(VOID* pUserData)
{
    DT_GVG_ARENA_DATA *pstData = (DT_GVG_ARENA_DATA*)pUserData;
    m_stArena = *pstData;
}

VOID SDAPI CSaveGvGArenaCMD::OnExecuted()
{
}

VOID SDAPI CClearGvGActivityCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    string strSql = "truncate gvgarena;";

    ISDDBRecordSet *pRecordSet = NULL;
    string strError;
    INT32 nRet = poDBConn->ExecuteSqlRs(strSql.c_str(), &pRecordSet, NULL, &strError );
    if(pRecordSet)
    {
        pRecordSet->Release();
    }

    if(nRet < 0)
    {
        SYS_CRITICAL( _SDT( "[%s %d] clear video fail: exec SQL error = [%s], SQL = [%s]" ), MSG_MARK, strError.c_str(), strSql.c_str());
        return;
    }
}