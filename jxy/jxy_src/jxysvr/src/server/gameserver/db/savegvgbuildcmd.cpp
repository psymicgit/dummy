
#include "savegvgbuildcmd.h"
#include "db/dbcommoperate.h"
#include "logic/gvg/local/gvgbuild.h"
#include "db/autosqlbuf.h"

VOID SDAPI CSaveGvGBuildCMD::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
	SaveData(poDBConn);
}

BOOL CSaveGvGBuildCMD::SaveData(SGDP::ISDDBConnection *poDBConn)
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
		return FALSE;
	}

    CSDDateTime oDate(m_stGvGBuildData.qwStartTime);
    tstring strTime = SDTimeToString(oDate);

	SDSnprintf(pszSql, MAX_SQL_LEN - 1, "insert into gvgbuild (SessionID,ArenaID,StartTime,CurStage,StageFlagList,ApplayFactionData,LastResult,MoneyResult,ChaosWarData,DoubleWarInfo,War16To8List,FinalWarData,FactionsGvGLogData) values (%u, %u, '%s', %u",
        m_stGvGBuildData.wSessionID, m_stGvGBuildData.wArenaID, strTime.c_str(), m_stGvGBuildData.byCurStage);
    INSERT_STRUCT( StageFlagList, m_stGvGBuildData.stStageFlagList);
	INSERT_STRUCT( ApplayFactionData, m_stGvGBuildData.stApplayInfo);
	INSERT_STRUCT( LastResult, m_stGvGBuildData.stLastResult);
	INSERT_STRUCT( MoneyResult, m_stGvGBuildData.stMoneyResult);
	INSERT_STRUCT( ChaosWarData, m_stGvGBuildData.stChaosWar);
    INSERT_STRUCT( DoubleWarInfo, m_stGvGBuildData.stWar16To8To4Info);
    INSERT_STRUCT( War16To8List, m_stGvGBuildData.stWar16To8List);
    INSERT_STRUCT( FinalWarData, m_stGvGBuildData.stFinalWarInfo);
	INSERT_STRUCT( FactionsGvGLogData, m_stGvGBuildData.stFactionsLog);	

	SDStrSafeCat(pszSql, MAX_SQL_LEN - 1, ")  on duplicate key update gvgbuild.SessionID = values(gvgbuild.SessionID),\
gvgbuild.ArenaID = values(gvgbuild.ArenaID),\
gvgbuild.StartTime = values(gvgbuild.StartTime),\
gvgbuild.CurStage = values(gvgbuild.CurStage),\
gvgbuild.ApplayFactionData = values(gvgbuild.ApplayFactionData),\
gvgbuild.LastResult = values(gvgbuild.LastResult), \
gvgbuild.MoneyResult = values(gvgbuild.MoneyResult), \
gvgbuild.ChaosWarData = values(gvgbuild.ChaosWarData),\
gvgbuild.DoubleWarInfo = values(gvgbuild.DoubleWarInfo),\
gvgbuild.War16To8List = values(gvgbuild.War16To8List),\
gvgbuild.FinalWarData = values(gvgbuild.FinalWarData), \
gvgbuild.StageFlagList = values(gvgbuild.StageFlagList), \
gvgbuild.FactionsGvGLogData = values(gvgbuild.FactionsGvGLogData) ; ");

	ISDDBRecordSet *pRecordSet = NULL;
	string strError;
	nRet = poDBConn->ExecuteSqlRs( pszSql, &pRecordSet, NULL, &strError );
	if(pRecordSet)
	{
		pRecordSet->Release();
	}

	if(nRet < 0)
	{
		SYS_CRITICAL( _SDT( "[CSaveGvGBuildCMD::SaveData] exec SQL failed ,error = [%s], len of SQL = [%u]" ), strError.c_str(), SDStrlen(pszSql));
		return FALSE;
	}
	return TRUE;
}

VOID CSaveGvGBuildCMD::SetUserData(VOID* pUserData)
{
	DT_GVG_BUILD_DATA *pstData = (DT_GVG_BUILD_DATA*)pUserData;
	m_stGvGBuildData = *pstData;
}

VOID SDAPI CSaveGvGBuildCMD::OnExecuted()
{
}
