#include "settongbupayprocessed.h"
#include <framework/gsapi.h>
#include <db/autosqlbuf.h>
#include "logic/record/recordmgr.h"
#include <net/gt/gtpktbuilder.h>
#include "common/server/dblock.h"
#include <sdloggerimpl.h>

#include <net/db/dbprocessor.h>

CSetTongBuPayProcessed::CSetTongBuPayProcessed()
{

}

CSetTongBuPayProcessed::~CSetTongBuPayProcessed()
{

}


VOID SDAPI CSetTongBuPayProcessed::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    SetPayProcessed( poDBConn );
}

/**
* @brief 同步执行SQL命令相关的逻辑部分
* @return VOID
*/
VOID SDAPI CSetTongBuPayProcessed::OnExecuted()
{
    gsapi::GetDBProcessor()->OnProcessedTongBuPayInfo(m_stPayProcessed);

}

//设置数据
VOID CSetTongBuPayProcessed::SetUserData(VOID* pUserData)
{
    m_stPayProcessed = *(STongBuPayProcessed*)pUserData;
}

BOOL CSetTongBuPayProcessed::SetPayProcessed(SGDP::ISDDBConnection* poDBConn)
{
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();
    if((NULL == pszSql))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    m_stPayProcessed.wErrCode = 3;
    CDBLock lock( poDBConn, m_stPayProcessed.strTableName );
    sprintf(pszSql, "call ProcessTBRecharge('%s', '%s', '%s', '%s', '%u', '%u');",
            m_stPayProcessed.strTableName.c_str(),
            m_stPayProcessed.strOrderID.c_str(),
            m_stPayProcessed.strSource.c_str(),
            m_stPayProcessed.strPartner.c_str(),
            m_stPayProcessed.dwGold,
			m_stPayProcessed.dwRMB);

    UINT32	dwErr = 0;
    string	strError;
    ISDDBRecordSet* pRecordSet = NULL;
    INT32	nRet = poDBConn->ExecuteSqlRs(pszSql, &pRecordSet, &dwErr, &strError);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        m_stPayProcessed.dwPlayerID = SDAtou(pRecordSet->GetFieldValueByName("PlayerID"));
        m_stPayProcessed.wErrCode = SDAtou(pRecordSet->GetFieldValueByName("ErrCode"));
    }
    else
    {
        SYS_CRITICAL(_SDT("[%s: %d]: call ProcessTBRecharge failed[%s]. sql:[%s]"), MSG_MARK, strError.c_str(), pszSql);
        m_stPayProcessed.wErrCode = 3;//db更新失败
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
    }


    return TRUE;
}