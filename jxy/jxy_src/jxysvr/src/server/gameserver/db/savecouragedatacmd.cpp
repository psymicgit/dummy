
#include "savecouragedatacmd.h"
#include "dbcommoperate.h"
#include <db/autosqlbuf.h>
#include <sdutil.h>
#include <logic/player/playersavemgr.h>
#include <sdloggerimpl.h>


CSaveCourageDataCMD::CSaveCourageDataCMD()
{
    memset(&m_stDT_COURAGE_GROUP_DATA_LST, 0, sizeof(m_stDT_COURAGE_GROUP_DATA_LST));
}


CSaveCourageDataCMD::~CSaveCourageDataCMD()
{

}

VOID CSaveCourageDataCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    if(!SaveData(poDBConn))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: CSaveCourageDataCMD save failed: activityid = %u, group num = %u"), MSG_MARK, m_stDT_COURAGE_GROUP_DATA_LST.byActivityID, m_stDT_COURAGE_GROUP_DATA_LST.wGroupNum);
    }
}

BOOL CSaveCourageDataCMD::SaveData(SGDP::ISDDBConnection *poDBConn)
{
    CAutoSqlBuf oAutoSqlBuf;
    CAutoSqlBuf oAutoSaveEnDeBuf;
    CAutoSqlBuf oAutoTmp;
     CAutoSqlBuf oAutoTmp1;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
    CHAR* pszBuff = oAutoSaveEnDeBuf.GetSqlBuf();
    CHAR* pszTmp = oAutoTmp.GetSqlBuf();
     CHAR* pszBossBData = oAutoTmp1.GetSqlBuf();
    if((NULL == pszSql) || (NULL == pszBuff) || (NULL == pszTmp))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    //memset(pszBuff, 0, MAX_SQL_LEN);
    INT32 nRet = m_stDT_COURAGE_GROUP_DATA_LST.EncodeSaveData(pszBuff, MAX_SQL_LEN);
    if(nRet <= 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: stBaseData.EncodeSaveData is failed"), MSG_MARK);
        return FALSE;
    }
    //CHAR szBossBData[MAXENLEN_DT_COURAGE_GROUP_DATA_LST * 2 + 1] = {0};
    UINT32 dwRetCode = poDBConn->EscapeString(pszBuff,
                       nRet,
                       pszBossBData,
                       MAXENLEN_DT_COURAGE_GROUP_DATA_LST * 2 + 1);
    if(dwRetCode <= 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: EscapeString failed!"), MSG_MARK);
        return FALSE;
    }
    sprintf(pszSql, "insert into module (KeyID, KeyID2, Value, UpdateTime ) values ( '%u', '%u', '%s', now() ) on duplicate key update module.Value = values(module.Value), module.UpdateTime = values(module.UpdateTime);",
            EMDT_COURAGE, m_stDT_COURAGE_GROUP_DATA_LST.byActivityID , pszBossBData);
    UINT32 dwErr;
    string strError;
    nRet = poDBConn->ExecuteSql(pszSql, NULL, NULL, &dwErr, &strError);
    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, dwErr, strError.c_str(), pszSql);
        m_bSuccess = FALSE;
        return FALSE;
    }

    if(0 != m_stDT_COURAGE_GROUP_DATA_LST.qwOverTime)
    {
        CSDDateTime oDateTime(m_stDT_COURAGE_GROUP_DATA_LST.qwOverTime);
        tstring strTime = SDTimeToString(oDateTime);
        //插入LOG记录
        sprintf(pszSql, "insert into modulelog (KeyID, KeyID2, OverTime, Value, UpdateTime ) values ( '%u', '%u', '%s', '%s', now() ) on duplicate key update modulelog.Value = values(modulelog.Value), modulelog.UpdateTime = values(modulelog.UpdateTime);",
                EMDT_COURAGE, m_stDT_COURAGE_GROUP_DATA_LST.byActivityID , strTime.c_str(), pszBossBData);
        nRet = poDBConn->ExecuteSql(pszSql, NULL, NULL, &dwErr, &strError);
        if(nRet < 0)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, dwErr, strError.c_str(), pszSql);
        }
        //将module中的删除
        sprintf(pszSql, "delete from module where KeyID = %u;", EMDT_COURAGE);
        INT32 nRet = poDBConn->ExecuteSql(pszSql, NULL, NULL, &dwErr, &strError);
        if (nRet < 0 )
        {
            SYS_CRITICAL(_SDT("[%s: %d]: DeleteCourage[%d] failed! Err(%u), ErrStr(%s), SQL:%s"), MSG_MARK, EMDT_COURAGE, dwErr, strError.c_str(), pszSql);    
        }
    }

    m_bSuccess = TRUE;
    return TRUE;
}

VOID CSaveCourageDataCMD::OnExecuted()
{
    if(!m_bSuccess)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: save bossb data failed!"), MSG_MARK);
    }
}

VOID CSaveCourageDataCMD::SetUserData(VOID* pUserData)
{
    memcpy(&m_stDT_COURAGE_GROUP_DATA_LST, pUserData, sizeof(m_stDT_COURAGE_GROUP_DATA_LST));
}
