
#include <db/autosqlbuf.h>
#include "activityencouragecmd.h"


#include <db/dbsqlstring.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include <common/server/base64.h>
#include <logic/chat/chatmgr.h>
#include "dbcommoperate.h"
#include <sdloggerimpl.h>
#include <json/json.h>
#include "framework/gsapi.h"

CActivityEncourageCMD::CActivityEncourageCMD()
{
    m_bGetDataSuccess = FALSE;
}


CActivityEncourageCMD::~CActivityEncourageCMD()
{

}

VOID CActivityEncourageCMD::SetUserData(VOID* pUserData)
{

}

BOOL CActivityEncourageCMD::GetActivityEncourageProp(SGDP::ISDDBConnection* poDBConn)
{
    SDTRY_BEGIN
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    UINT32 dwErrID = 0;
    string strErr;
    CHAR szSql[512] = {0};
    sprintf(szSql, "select * from activityencouragepro where (ZoneID = %u or ZoneID = 0) ORDER BY ActivityID", gsapi::GetZoneID());
    INT32 nRet = poDBConn->ExecuteSqlRs(szSql, &pRecordSet, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: has no data!"), MSG_MARK);
        return FALSE;
    }

    //查询的表结果
    while(pRecordSet->GetRecord())// 当有行结果时
    {
        SActivityEncouragePro stProp;
        stProp.qwActivityID	= SGDP::SDAtou64(pRecordSet->GetFieldValueByName("ActivityID"));
        stProp.byNeedCdKey	= SGDP::SDAtou(pRecordSet->GetFieldValueByName("IsNeedCdKey"));
        UINT32	dwLen		= pRecordSet->GetFieldLengthByName("ActivityName");
        SDStrncpy(stProp.szActivityName, pRecordSet->GetFieldValueByName("ActivityName"), dwLen);
        dwLen		= pRecordSet->GetFieldLengthByName("ActivityDesc");
        SDStrncpy(stProp.szActivityDesc, pRecordSet->GetFieldValueByName("ActivityDesc"), dwLen);
        string strJsonValues = pRecordSet->GetFieldValueByName("JsonValues");

        GetDateTime2Stamp(stProp.qwStartTime, "StartTime");
        GetDateTime2Stamp(stProp.qwEndTime, "EndTime");

        if (stProp.qwStartTime > stProp.qwEndTime)
        {
            SYS_CRITICAL(_SDT("[%s: %d]: time limit!!"), MSG_MARK);
            return FALSE;
        }

        //解析JSon数值
        if(!JsonParseResItem(stProp.vecActivityEncourageItemInfo, strJsonValues))
		{
			SYS_CRITICAL(_SDT("[%s: %d]: JsonParseResItem error, JsonValues:%s!"), MSG_MARK, strJsonValues.c_str());
			return FALSE;
		}
        m_mapActivityEncourageProp[stProp.qwActivityID] = stProp;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    szSql[0] = '\0';
    sprintf(szSql, "select p.* from activityencourageplayer p inner join activityencouragepro enc on p.ActivityID = enc.ActivityID and (enc.ZoneID = %u or enc.ZoneID = 0) order by ActivityID, StartPlayerID;", gsapi::GetZoneID());

    nRet = poDBConn->ExecuteSqlRs(szSql, &pRecordSet, &dwErrID,  &strErr);
    if(SDDB_HAS_RECORDSET != nRet)
    {
        // USR_INFO(_SDT("[%s: %d]: activityencourageplayer has no data!"), MSG_MARK);
        return TRUE;
    }

    m_bGetDataSuccess = TRUE;
    //查询的表结果
    while(pRecordSet->GetRecord())// 当有行结果时
    {
        UINT64 qwActivityID		= SGDP::SDAtou64(pRecordSet->GetFieldValueByName("ActivityID"));
        UINT32 dwStartPlayerID		= SGDP::SDAtou(pRecordSet->GetFieldValueByName("StartPlayerID"));
        UINT32 dwEndPlayerID		= SGDP::SDAtou(pRecordSet->GetFieldValueByName("EndPlayerID"));
        if(dwEndPlayerID < dwStartPlayerID)
        {
            USR_INFO(_SDT("[%s: %d]: EndPlayerID[%u] < StartPlayerID[%u]!"), MSG_MARK, dwEndPlayerID, dwStartPlayerID);
            dwEndPlayerID = dwStartPlayerID;
        }

        CActivityEncouragePropMapItr itr;
        itr = m_mapActivityEncourageProp.find(qwActivityID);
        if (itr != m_mapActivityEncourageProp.end())
        {
            SActivityEncouragePro & stProp = itr->second;
            stProp.mapActivityEncourageStartPlayerID[dwStartPlayerID] = dwEndPlayerID;
            stProp.mapActivityEncourageEndPlayerID[dwEndPlayerID] = dwStartPlayerID;
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    m_bGetDataSuccess = TRUE;
    return TRUE;

    SDTRY_END
}


VOID CActivityEncourageCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    if(!GetActivityEncourageProp(poDBConn))
    {
        //SYS_CRITICAL(_SDT("[%s: %d]: GetActivityEncourageProp failed!"), MSG_MARK);
        return;
    }
}

VOID CActivityEncourageCMD::OnExecuted()
{
    SDTRY_BEGIN
    if(m_bGetDataSuccess)
    {
        CActivityEncourageMgr::Instance()->SetActivityEncouragePropMap(m_mapActivityEncourageProp);
    }
    SDTRY_END
}

