#include "settongbupayinfo.h"
#include <db/autosqlbuf.h>
#include "logic/http/httpmgr.h"
#include "common/server/base64.h"
#include "logic/record/recordmgr.h"
#include "logic/player/playermgr.h"
#include "net/gt/gtpktbuilder.h"
#include <framework/gsapi.h>
#include <net/db/dbprocessor.h>

CSetTongBuPayInfo::CSetTongBuPayInfo()
{
}

CSetTongBuPayInfo::~CSetTongBuPayInfo()
{

}

VOID SDAPI CSetTongBuPayInfo::OnExecuteSql(SGDP::ISDDBConnection* poDBConn)
{
    m_stPayInfo.bSuccess = SaveTongBuPayInfo( poDBConn );
}

VOID SDAPI CSetTongBuPayInfo::OnExecuted()
{
    gsapi::GetDBProcessor()->OnSaveTongBuPayInfo(m_stPayInfo);
}

BOOL CSetTongBuPayInfo::SaveTongBuPayInfo(SGDP::ISDDBConnection* poDBConn)
{
    CAutoSqlBuf oAutoSqlBuf;
    CHAR* pszSql = oAutoSqlBuf.GetSqlBuf();

    if((NULL == pszSql))
    {
        SYS_CRITICAL(_SDT("[%s: %d]: GetSQLMemory is NULL"), MSG_MARK);
        return FALSE;
    }

    sprintf( pszSql, "insert into %s ( OrderID, PlayerID, DateCreate, IsProcess, PlayerLevel, ZoneID, UserID ) \
					 values( '%s', '%u', now(), 0, '%u', '%u', '%u' )",
             m_stPayInfo.strTableName.c_str(), m_stPayInfo.strOrderID.c_str(), m_stPayInfo.dwPlayerID, m_stPayInfo.dwPlayerLevel, m_stPayInfo.dwZoneID, m_stPayInfo.dwUserID);
    UINT32 dwErr = 0;
    string strError;
    INT32 nRet = poDBConn->ExecuteSql(pszSql, NULL,  NULL, &dwErr, &strError);

    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed! player[%d], error:%s"), MSG_MARK, m_stPayInfo.dwPlayerID, strError.c_str());
        return FALSE;
    }

    return TRUE;
}

VOID CSetTongBuPayInfo::SetUserData(VOID* pUserData)
{
    m_stPayInfo = *( (STongBuPayInfo*)pUserData );
}


