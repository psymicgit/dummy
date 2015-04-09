
#include "savefactiondatacmd.h"
#include "dbcommoperate.h"
#include <sdutil.h>
#include <logic/player/playersavemgr.h>

#include <framework/gsapi.h>
#include <db/autosqlbuf.h>
#include <sdloggerimpl.h>
#include "logic/faction/factionmgr.h"

CSaveFactionDataCMD::CSaveFactionDataCMD()
{
    memset(&m_stDT_FACTION_DATA, 0, sizeof(m_stDT_FACTION_DATA));
}


CSaveFactionDataCMD::~CSaveFactionDataCMD()
{

}




VOID CSaveFactionDataCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    poDBConn->BeginTransaction();
    if(SaveData(poDBConn))
    {
        //保存成功,若存在错误文件则删除
        if (gsapi::GetDBMgr()->IsExistErrorInfo(ERROR_FACTION,  m_stDT_FACTION_DATA.stFactionBaseData.dwFactionID ))
        {
            if( !gsapi::GetDBMgr()->DeleteErrorInfo(ERROR_FACTION,  m_stDT_FACTION_DATA.stFactionBaseData.dwFactionID ))
            {
                //删除文件失败
                SYS_CRITICAL(_SDT("[%s: %d]: DeleteErrorInfo failed, PlayerID[%d]!"), MSG_MARK, m_stDT_FACTION_DATA.stFactionBaseData.dwFactionID);
            }
        }
        poDBConn->CommitTransaction();
        return ;//
    }

    poDBConn->RollbackTransaction();
    //记录错误的SQL信息
    vector<string> vecSql;
    if(GetSql(poDBConn, vecSql))
    {
        gsapi::GetDBMgr()->AddErrorInfo(ERROR_FACTION,  m_stDT_FACTION_DATA.stFactionBaseData.dwFactionID, vecSql );
    }
}


BOOL CSaveFactionDataCMD::SaveData(SGDP::ISDDBConnection *poDBConn)
{
    DT_FACTION_BASE_DATA		&stDT_FACTION_BASE_DATA	=	m_stDT_FACTION_DATA.stFactionBaseData;
    DT_FACTION_PLAYER_DATA_LST &stFactionPlayerDataLst	=	m_stDT_FACTION_DATA.stFactionPlayerDataLst;
    DT_FACTION_LOG_DATA_LST	&stFactionLogDataLst		=	m_stDT_FACTION_DATA.stFactionLogDataLst;
    DT_FACTION_COMMON_DATA		&stFactionCommonData		=	m_stDT_FACTION_DATA.stFactionCommonData;
    DT_FACTION_GVG_DATA &stFactionGvGData               =   m_stDT_FACTION_DATA.stGvGData;

    //先比较之前的旧数据，是否相等，相等直接跳过
    if(!gsapi::GetDBMgr()->HaveChanged(stDT_FACTION_BASE_DATA.dwFactionID, m_stDT_FACTION_DATA))
    {
        m_bSuccess = TRUE;
        return TRUE;
    }

    CAutoSqlBuf oAutoSql;
    CAutoSqlBuf oAutoBuf;
    CAutoSqlBuf oAutoTmp;

    CHAR* pszSql		= oAutoSql.GetSqlBuf();
    CHAR* pszBuff		= oAutoBuf.GetSqlBuf();
    CHAR* pszTmp		= oAutoTmp.GetSqlBuf();
	INT32 nRet = 0;

    if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

	if ( 0 == stFactionPlayerDataLst.dwFactionNum)//门派中已经没有人
	{
		 SDSnprintf(pszSql, MAX_SQL_LEN - 1, "delete from faction where FactionID = %u; delete from factionplayer where FactionID = %u ;", stDT_FACTION_BASE_DATA.dwFactionID, stDT_FACTION_BASE_DATA.dwFactionID);
		 UINT32 dwErr;
		 string strError;
		 nRet = poDBConn->ExecuteSql(pszSql, NULL, NULL, &dwErr, &strError);
		 if(nRet < 0)
		 {
			 SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, dwErr, strError.c_str(), pszSql);
			 m_bSuccess = FALSE;
			 return FALSE;
		 }
		 CFaction* poFaction = CFactionMgr::Instance()->GetFaction(stDT_FACTION_BASE_DATA.dwFactionID);
		 if (NULL == poFaction)
		 {
			 m_bSuccess = FALSE;
			 return FALSE;
		 }
		 CFactionMgr::Instance()->DissolveFaction(poFaction);
		 return TRUE;
		 
	}

    
    SDSnprintf(pszSql, MAX_SQL_LEN - 1, "update faction set  ");
    UPDATESIMPLEFIELD1(FactionID, % u, stDT_FACTION_BASE_DATA.dwFactionID);
    UPDATESIMPLEFIELD2(FactionPlayerID, % u, stDT_FACTION_BASE_DATA.dwFactionPlayerID);
	memset(pszTmp, 0, MAX_SQL_LEN);
	CkEmoji(stDT_FACTION_BASE_DATA.aszFactionName);
	poDBConn->EscapeString(stDT_FACTION_BASE_DATA.aszFactionName, SDStrlen(stDT_FACTION_BASE_DATA.aszFactionName), pszTmp, MAX_SQL_LEN);
    UPDATESIMPLEFIELD2(FactionName, % s, pszTmp);
    UPDATESIMPLEFIELD2(FactionRank, % d, stDT_FACTION_BASE_DATA.dwFactionRank);
    UPDATESIMPLEFIELD2(FactionLevel, % d, stDT_FACTION_BASE_DATA.dwFactionLevel);
	memset(pszTmp, 0, MAX_SQL_LEN);
	CkEmoji(stDT_FACTION_BASE_DATA.aszFactionNotice);
	poDBConn->EscapeString(stDT_FACTION_BASE_DATA.aszFactionNotice, SDStrlen(stDT_FACTION_BASE_DATA.aszFactionNotice), pszTmp, MAX_SQL_LEN);
    UPDATESIMPLEFIELD2(FactionNotice, % s, pszTmp);
	memset(pszTmp, 0, MAX_SQL_LEN);
	CkEmoji(stDT_FACTION_BASE_DATA.aszFactionAnnouncement);
	poDBConn->EscapeString(stDT_FACTION_BASE_DATA.aszFactionAnnouncement, SDStrlen(stDT_FACTION_BASE_DATA.aszFactionAnnouncement), pszTmp, MAX_SQL_LEN);
    UPDATESIMPLEFIELD2(FactionAnnouncement, % s, pszTmp);
    UPDATESIMPLEFIELD_STRUCT2(FactionLogDataLst, stFactionLogDataLst);
    UPDATESIMPLEFIELD2(RareItemRoomLevel, % u, stDT_FACTION_BASE_DATA.dwRareItemRoomGrade);
    UPDATESIMPLEFIELD2(GymnasiumLevel, % u, stDT_FACTION_BASE_DATA.dwGymnasiumLevel);
    UPDATESIMPLEFIELD2(FactionFunds, % u, stDT_FACTION_BASE_DATA.dwFactionFunds);
    UPDATESIMPLEFIELD_STRUCT2(FactionTaskSchedule, stFactionCommonData);
    UPDATESIMPLEFIELD2(ShadeLevel, % d, stDT_FACTION_BASE_DATA.dwShadeLevel);
    UPDATESIMPLEFIELD2(HotSpringLevel, % d, stDT_FACTION_BASE_DATA.dwHotSpringLevel);
    UPDATESIMPLEFIELD2(ColdPrisonLevel, % d, stDT_FACTION_BASE_DATA.dwColdPrisonLevel);
    UPDATESIMPLEFIELD2(FactionTowerLevel, % d, stDT_FACTION_BASE_DATA.dwFactionTowerLevel);
	UPDATESIMPLEFIELD2(FactionFundsSum, % u, stDT_FACTION_BASE_DATA.qwFactionFundsSum);
	UPDATESIMPLEFIELD2(UpdateTime, % s, SDNow().ToString().c_str());
	UPDATESIMPLEFIELD2(TodayPassNum, % u, m_stDT_FACTION_DATA.byTodayPassNum);
	CSDDateTime oLastAuditTime(m_stDT_FACTION_DATA.qwLastAuditTime);
	UPDATESIMPLEFIELD2(LastAuditTime, % s, oLastAuditTime.ToString().c_str());
	UPDATESIMPLEFIELD2(Ext, % u, m_stDT_FACTION_DATA.byExt);
    UPDATESIMPLEFIELD_STRUCT2(GvGFormation, stFactionGvGData.stGvGFormation);
    UPDATESIMPLEFIELD_STRUCT2(GvGLog, stFactionGvGData.stGvGLogList);
	UPDATESIMPLEFIELD2(GVGFactionFunds, % llu,stFactionGvGData.qwGVGFactionFunds);
	UPDATESIMPLEFIELD2(SumPower, % llu,stFactionGvGData.qwSumPower);
    SDSnprintf(pszTmp, MAX_SQL_LEN - 1, " where FactionID ='%u';", stDT_FACTION_BASE_DATA.dwFactionID);
    strcat(pszSql, pszTmp);


	UINT32 dwErr;
	string strError;
	nRet = poDBConn->ExecuteSql(pszSql, NULL, NULL, &dwErr, &strError);
	if(nRet < 0)
	{
		SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, dwErr, strError.c_str(), pszSql);
		m_bSuccess = FALSE;
		return FALSE;
	}
    char v_szTemp[256] = {0};
    SDSnprintf(pszSql, MAX_SQL_LEN - 1, "delete from factionplayer where FactionID = %u and JoinPlayerID not in(0 ", stDT_FACTION_BASE_DATA.dwFactionID);
    for (UINT32 i = 0; i < stFactionPlayerDataLst.dwFactionNum; i++)
    {
        SDStrcat(pszSql, ",");
        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u", stFactionPlayerDataLst.astFactionPlayerData[i].dwJoinPlayerID);
        SDStrcat(pszSql, v_szTemp);
    }
    SDStrcat(pszSql, ");");
    nRet = poDBConn->ExecuteSql(pszSql, NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, dwErr, strError.c_str(), pszSql);
        m_bSuccess = FALSE;
        return FALSE;
    }

    SDSnprintf(pszSql, MAX_SQL_LEN - 1, "insert into factionplayer (FactionID, JoinPlayerID, JoinState, JobType, DoorsTribute, DoorsTributeSum, DrinkFlag, NewGoodsFlag, NewSkillFlag, UnLoginBattleLog, ContinueKillKey, JoinTime) values (");

    for (UINT32 i = 0; i < stFactionPlayerDataLst.dwFactionNum; i++)
    {
        if (0 != i)
        {
            SDStrcat(pszSql, "),(");
        }
        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stDT_FACTION_BASE_DATA.dwFactionID);
        SDStrcat(pszSql, v_szTemp);

        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].dwJoinPlayerID);
        SDStrcat(pszSql, v_szTemp);

        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].byJoinState);
        SDStrcat(pszSql, v_szTemp);

        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].byJobType);
        SDStrcat(pszSql, v_szTemp);

        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].dwDoorsTribute);
        SDStrcat(pszSql, v_szTemp);

        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].qwDoorsTributeSum);
        SDStrcat(pszSql, v_szTemp);

        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].byDrinkFlag);
        SDStrcat(pszSql, v_szTemp);

        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].byNewGoodFlag);
        SDStrcat(pszSql, v_szTemp);

        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].byNewSkillFlag);
        SDStrcat(pszSql, v_szTemp);

        memset(pszTmp, 0, MAX_SQL_LEN);
        memset(pszBuff, 0, MAX_SQL_LEN);
        nRet = stFactionPlayerDataLst.astFactionPlayerData[i].stUnLoginBattleLogList.EncodeSaveData(pszBuff, MAX_SQL_LEN);
        if(nRet <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: DT_FACTION_LOG_DATA_LST.EncodeSaveData is failed"), MSG_MARK);
            return FALSE;
        }
        nRet = poDBConn->EscapeString(pszBuff,
                                      nRet,
                                      pszTmp,
                                      MAX_SQL_LEN);
        if(nRet <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
            return FALSE;
        }
        SDStrcat(pszSql, "'");
        SDStrcat(pszSql, pszTmp);
        SDStrcat(pszSql, "',");


		memset(pszTmp, 0, MAX_SQL_LEN);
		memset(pszBuff, 0, MAX_SQL_LEN);
		nRet = stFactionPlayerDataLst.astFactionPlayerData[i].stContinueKillKey.EncodeSaveData(pszBuff, MAX_SQL_LEN);
		if(nRet <= 0)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: stContinueKillKey.EncodeSaveData is failed"), MSG_MARK);
			return FALSE;
		}
		nRet = poDBConn->EscapeString(pszBuff,
			nRet,
			pszTmp,
			MAX_SQL_LEN);
		if(nRet <= 0)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
			return FALSE;
		}
		SDStrcat(pszSql, "'");
		SDStrcat(pszSql, pszTmp);
		SDStrcat(pszSql, "',");


		CSDDateTime oJoinTime(stFactionPlayerDataLst.astFactionPlayerData[i].qwJoinTime);
		SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "'%s'", oJoinTime.ToString().c_str() );
		SDStrcat(pszSql, v_szTemp);

    }
    SDStrcat(pszSql, ")on duplicate key update \
				   factionplayer.FactionID = values(factionplayer.FactionID), \
				   factionplayer.JoinPlayerID = values(factionplayer.JoinPlayerID),	\
				   factionplayer.JoinState = values(factionplayer.JoinState),	\
				   factionplayer.JobType = values(factionplayer.JobType),	\
				   factionplayer.DoorsTribute = values(factionplayer.DoorsTribute), \
				   factionplayer.DoorsTributeSum = values(factionplayer.DoorsTributeSum), \
				   factionplayer.DrinkFlag = values(factionplayer.DrinkFlag), \
				   factionplayer.NewGoodsFlag = values(factionplayer.NewGoodsFlag), \
				   factionplayer.NewSkillFlag = values(factionplayer.NewSkillFlag), \
				   factionplayer.UnLoginBattleLog = values(factionplayer.UnLoginBattleLog), \
				   factionplayer.ContinueKillKey = values(factionplayer.ContinueKillKey), \
				   factionplayer.JoinTime = values(factionplayer.JoinTime);");
    nRet = poDBConn->ExecuteSql(pszSql, NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[ % s:% d]: exec sql failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, dwErr, strError.c_str(), pszSql);
        m_bSuccess = FALSE;
        return FALSE;
    }

    gsapi::GetDBMgr()->Changed(stDT_FACTION_BASE_DATA.dwFactionID, m_stDT_FACTION_DATA);
    m_bSuccess = TRUE;
    return TRUE;
}

VOID CSaveFactionDataCMD::OnExecuted()
{
	CFaction *poFaction = CFactionMgr::Instance()->GetFaction(m_stDT_FACTION_DATA.stFactionBaseData.dwFactionID);
	if (NULL == poFaction)
	{
		return;
	}

	if(m_bSuccess)
	{
		poFaction->SetExceptionFlag(FALSE);
	}
	else
	{
		SYS_CRITICAL(_SDT("[%s: %d]: save Faction Data failed!"), MSG_MARK);
		poFaction->SetExceptionFlag(TRUE);
	}
}


VOID CSaveFactionDataCMD::SetUserData(VOID* pUserData)
{
    memcpy(&m_stDT_FACTION_DATA, pUserData, sizeof(DT_FACTION_DATA));
}
BOOL CSaveFactionDataCMD::GetSql(SGDP::ISDDBConnection* poDBConn, vector<string> &vecSql)
{
    DT_FACTION_BASE_DATA		&stDT_FACTION_BASE_DATA	=	m_stDT_FACTION_DATA.stFactionBaseData;
    DT_FACTION_PLAYER_DATA_LST &stFactionPlayerDataLst	=	m_stDT_FACTION_DATA.stFactionPlayerDataLst;
    DT_FACTION_LOG_DATA_LST	&stFactionLogDataLst		=	m_stDT_FACTION_DATA.stFactionLogDataLst;
    DT_FACTION_COMMON_DATA		&stFactionCommonData		=	m_stDT_FACTION_DATA.stFactionCommonData;
    DT_FACTION_GVG_DATA &stFactionGvGData               =   m_stDT_FACTION_DATA.stGvGData;

    CAutoSqlBuf oAutoSql;
    CAutoSqlBuf oAutoBuf;
    CAutoSqlBuf oAutoTmp;
    CHAR* pszSql		= oAutoSql.GetSqlBuf();
    CHAR* pszBuff		= oAutoBuf.GetSqlBuf();
    CHAR* pszTmp		= oAutoTmp.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }
    INT32 nRet = 0;
    SDSnprintf(pszSql, MAX_SQL_LEN - 1, "update faction set  ");
    UPDATESIMPLEFIELD1(FactionID, % u, stDT_FACTION_BASE_DATA.dwFactionID);
    UPDATESIMPLEFIELD2(FactionPlayerID, % u, stDT_FACTION_BASE_DATA.dwFactionPlayerID);
    UPDATESIMPLEFIELD2(FactionRank, % d, stDT_FACTION_BASE_DATA.dwFactionRank);
    UPDATESIMPLEFIELD2(FactionLevel, % d, stDT_FACTION_BASE_DATA.dwFactionLevel);
	memset(pszTmp, 0, MAX_SQL_LEN);
	CkEmoji(stDT_FACTION_BASE_DATA.aszFactionName);
	poDBConn->EscapeString(stDT_FACTION_BASE_DATA.aszFactionName, SDStrlen(stDT_FACTION_BASE_DATA.aszFactionName), pszTmp, MAX_SQL_LEN);
	UPDATESIMPLEFIELD2(FactionName, % s, pszTmp);
	memset(pszTmp, 0, MAX_SQL_LEN);
	CkEmoji(stDT_FACTION_BASE_DATA.aszFactionNotice);
	poDBConn->EscapeString(stDT_FACTION_BASE_DATA.aszFactionNotice, SDStrlen(stDT_FACTION_BASE_DATA.aszFactionNotice), pszTmp, MAX_SQL_LEN);
	UPDATESIMPLEFIELD2(FactionNotice, % s, pszTmp);
	memset(pszTmp, 0, MAX_SQL_LEN);
	CkEmoji(stDT_FACTION_BASE_DATA.aszFactionAnnouncement);
	poDBConn->EscapeString(stDT_FACTION_BASE_DATA.aszFactionAnnouncement, SDStrlen(stDT_FACTION_BASE_DATA.aszFactionAnnouncement), pszTmp, MAX_SQL_LEN);
	UPDATESIMPLEFIELD2(FactionAnnouncement, % s, pszTmp);
    // UPDATESIMPLEFIELD_STRUCT2(FactionPlayerDataLst, stFactionPlayerDataLst);
    UPDATESIMPLEFIELD_STRUCT2(FactionLogDataLst, stFactionLogDataLst);
    UPDATESIMPLEFIELD2(RareItemRoomLevel, % u, stDT_FACTION_BASE_DATA.dwRareItemRoomGrade);
    UPDATESIMPLEFIELD2(GymnasiumLevel, % u, stDT_FACTION_BASE_DATA.dwGymnasiumLevel);
    UPDATESIMPLEFIELD2(FactionFunds, % u, stDT_FACTION_BASE_DATA.dwFactionFunds);
    UPDATESIMPLEFIELD_STRUCT2(FactionTaskSchedule, stFactionCommonData);
    UPDATESIMPLEFIELD2(ShadeLevel, % d, stDT_FACTION_BASE_DATA.dwShadeLevel);
    UPDATESIMPLEFIELD2(HotSpringLevel, % d, stDT_FACTION_BASE_DATA.dwHotSpringLevel);
    UPDATESIMPLEFIELD2(ColdPrisonLevel, % d, stDT_FACTION_BASE_DATA.dwColdPrisonLevel);
    UPDATESIMPLEFIELD2(FactionTowerLevel, % d, stDT_FACTION_BASE_DATA.dwFactionTowerLevel);
    UPDATESIMPLEFIELD2(FactionFundsSum, % u, stDT_FACTION_BASE_DATA.qwFactionFundsSum);
    UPDATESIMPLEFIELD_STRUCT2(GvGFormation, stFactionGvGData.stGvGFormation);
    UPDATESIMPLEFIELD_STRUCT2(GvGLog, stFactionGvGData.stGvGLogList);
	UPDATESIMPLEFIELD2(GVGFactionFunds, % llu,stFactionGvGData.qwGVGFactionFunds);
	UPDATESIMPLEFIELD2(SumPower, % llu,stFactionGvGData.qwSumPower);
    SDSnprintf(pszTmp, MAX_SQL_LEN - 1, " where FactionID ='%u';", stDT_FACTION_BASE_DATA.dwFactionID);
    strcat(pszSql, pszTmp);
    vecSql.push_back(pszSql);
    char v_szTemp[256] = {0};
    SDSnprintf(pszSql, MAX_SQL_LEN - 1, "delete from factionplayer where FactionID = %u and JoinPlayerID not in(0 ", stDT_FACTION_BASE_DATA.dwFactionID);
    for (UINT32 i = 0; i < stFactionPlayerDataLst.dwFactionNum; i++)
    {
        SDStrcat(pszSql, ",");
        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u", stFactionPlayerDataLst.astFactionPlayerData[i].dwJoinPlayerID);
        SDStrcat(pszSql, v_szTemp);
    }
    SDStrcat(pszSql, ");");
    vecSql.push_back(pszSql);
    SDSnprintf(pszSql, MAX_SQL_LEN - 1, "insert into factionplayer (FactionID, JoinPlayerID, JoinState, JobType, DoorsTribute, DoorsTributeSum, DrinkFlag, NewGoodsFlag, NewSkillFlag, UnLoginBattleLog) values (");
    for (UINT32 i = 0; i < stFactionPlayerDataLst.dwFactionNum; i++)
    {
        if (0 != i)
        {
            SDStrcat(pszSql, "),(");
        }
        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stDT_FACTION_BASE_DATA.dwFactionID);
        SDStrcat(pszSql, v_szTemp);
        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].dwJoinPlayerID);
        SDStrcat(pszSql, v_szTemp);
        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].byJoinState);
        SDStrcat(pszSql, v_szTemp);
        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].byJobType);
        SDStrcat(pszSql, v_szTemp);
        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].dwDoorsTribute);
        SDStrcat(pszSql, v_szTemp);
        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].qwDoorsTributeSum);
        SDStrcat(pszSql, v_szTemp);
        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].byDrinkFlag);
        SDStrcat(pszSql, v_szTemp);
        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].byNewGoodFlag);
        SDStrcat(pszSql, v_szTemp);
        memset(v_szTemp, 0x00, sizeof(v_szTemp));
        SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "%u,", stFactionPlayerDataLst.astFactionPlayerData[i].byNewSkillFlag);
        SDStrcat(pszSql, v_szTemp);
        memset(pszTmp, 0, MAX_SQL_LEN);
        memset(pszBuff, 0, MAX_SQL_LEN);
        nRet = stFactionPlayerDataLst.astFactionPlayerData[i].stUnLoginBattleLogList.EncodeSaveData(pszBuff, MAX_SQL_LEN);
        if(nRet <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: DT_FACTION_LOG_DATA_LST.EncodeSaveData is failed"), MSG_MARK);
            return FALSE;
        }
        nRet = poDBConn->EscapeString(pszBuff,
                                      nRet,
                                      pszTmp,
                                      MAX_SQL_LEN);
        if(nRet <= 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
            return FALSE;
        }
        SDStrcat(pszSql, "'");
        SDStrcat(pszSql, pszTmp);
        SDStrcat(pszSql, "',");

		memset(pszTmp, 0, MAX_SQL_LEN);
		memset(pszBuff, 0, MAX_SQL_LEN);
		nRet = stFactionPlayerDataLst.astFactionPlayerData[i].stContinueKillKey.EncodeSaveData(pszBuff, MAX_SQL_LEN);
		if(nRet <= 0)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: stContinueKillKey.EncodeSaveData is failed"), MSG_MARK);
			return FALSE;
		}
		nRet = poDBConn->EscapeString(pszBuff,
			nRet,
			pszTmp,
			MAX_SQL_LEN);
		if(nRet <= 0)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
			return FALSE;
		}
		SDStrcat(pszSql, "'");
		SDStrcat(pszSql, pszTmp);
		SDStrcat(pszSql, "',");


		CSDDateTime oJoinTime(stFactionPlayerDataLst.astFactionPlayerData[i].qwJoinTime);
		SDSnprintf(v_szTemp, MAX_SQL_LEN - 1, "'%s'", oJoinTime.ToString().c_str() );
		SDStrcat(pszSql, v_szTemp);
    }
    SDStrcat(pszSql, ")on duplicate key update \
					 factionplayer.FactionID = values(factionplayer.FactionID), \
					 factionplayer.JoinPlayerID = values(factionplayer.JoinPlayerID),	\
					 factionplayer.JoinState = values(factionplayer.JoinState),	\
					 factionplayer.JobType = values(factionplayer.JobType),	\
					 factionplayer.DoorsTribute = values(factionplayer.DoorsTribute), \
					 factionplayer.DoorsTributeSum = values(factionplayer.DoorsTributeSum), \
					 factionplayer.DrinkFlag = values(factionplayer.DrinkFlag), \
					 factionplayer.NewGoodsFlag = values(factionplayer.NewGoodsFlag), \
					 factionplayer.NewSkillFlag = values(factionplayer.NewSkillFlag), \
					 factionplayer.UnLoginBattleLog = values(factionplayer.UnLoginBattleLog), \
					 factionplayer.ContinueKillKey = values(factionplayer.ContinueKillKey), \
					 factionplayer.JoinTime = values(factionplayer.JoinTime);");
    vecSql.push_back(pszSql);
    return TRUE;
}
