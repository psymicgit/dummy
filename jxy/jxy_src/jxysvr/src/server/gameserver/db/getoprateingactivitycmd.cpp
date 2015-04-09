
#include <db/autosqlbuf.h>
#include "getoprateingactivitycmd.h"
#include <db/dbsqlstring.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include <common/server/base64.h>
#include <logic/chat/chatmgr.h>
#include <logic/activityencourage/oprateingactivitymgr.h>



CGetOprateingActivityCMD::CGetOprateingActivityCMD()
{
    memset(&m_stOprateingActivityProp, 0, sizeof(m_stOprateingActivityProp));
    m_wZoneID = 0;
    m_bSuccess = FALSE;
}


CGetOprateingActivityCMD::~CGetOprateingActivityCMD()
{

}

VOID CGetOprateingActivityCMD::SetUserData(VOID* pUserData)
{
    m_wZoneID = *(UINT16*)pUserData;
}


VOID CGetOprateingActivityCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{

    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from operatingactivityprop where (ZoneID='%u' or ZoneID='0') order by ZoneID Desc, VersionID desc limit 1;", m_wZoneID);
    ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
    {
        DT_OPERATING_ACTIVITY_DATA& stProtData = m_stOprateingActivityProp.stProtData;
        stProtData.byNum = SDAtou(pRecordSet->GetFieldValueByName("Num"));
        stProtData.dwVersionID = SDAtou(pRecordSet->GetFieldValueByName("VersionID"));
        m_stOprateingActivityProp.bOpenSoulAty = SDAtou(pRecordSet->GetFieldValueByName("SoulActivity")) > 0;
		INT32 nLen = pRecordSet->GetFieldLengthByName("Desc");
        nLen = nLen >= MAX_COMMON_TXT_LEN ? (MAX_COMMON_TXT_LEN - 1) : nLen;
        SDStrncpy(stProtData.aszDesc, pRecordSet->GetFieldValueByName("Desc"), nLen);
        nLen = pRecordSet->GetFieldLengthByName("Url");
        nLen = nLen >= MAX_COMMON_TXT_LEN ? (MAX_COMMON_TXT_LEN - 1) : nLen;
        SDStrncpy(stProtData.aszUrl, pRecordSet->GetFieldValueByName("Url"), nLen);
        stProtData.byWebTabType = SDAtou(pRecordSet->GetFieldValueByName("WebTabType"));
        nLen = pRecordSet->GetFieldLengthByName("ExtContext");
        nLen = nLen >= MAX_COMMON_TXT_LEN ? (MAX_COMMON_TXT_LEN - 1) : nLen;
        SDStrncpy(stProtData.aszExtContext, pRecordSet->GetFieldValueByName("ExtContext"), nLen);

        //m_stOprateingActivityProp.byTowerIdx = SDAtou(pRecordSet->GetFieldValueByName("TowerIdx"));
        //m_stOprateingActivityProp.byInstanceIdx = SDAtou(pRecordSet->GetFieldValueByName("InstanceIdx"));
        //m_stOprateingActivityProp.byInstanceIdx2 = SDAtou(pRecordSet->GetFieldValueByName("InstanceIdx2"));
        //m_stOprateingActivityProp.byInstanceIdx3 = SDAtou(pRecordSet->GetFieldValueByName("InstanceIdx3"));

        string strTime(pRecordSet->GetFieldValueByName("StartTime"));
        CSDDateTime stDateTime;
        if(SDTimeFromString(strTime, stDateTime))
        {
            m_stOprateingActivityProp.qwStartTime = stDateTime.GetTimeValue();
        }
        else
        {
            m_stOprateingActivityProp.qwStartTime = SDTimeSecs();
        }

        strTime = pRecordSet->GetFieldValueByName("EndTime");
        if(SDTimeFromString(strTime, stDateTime))
        {
            m_stOprateingActivityProp.qwEndTime = stDateTime.GetTimeValue();
        }
        else
        {
            m_stOprateingActivityProp.qwEndTime = SDTimeSecs() + 36000;
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    m_bSuccess = TRUE;
}


VOID CGetOprateingActivityCMD::OnExecuted()
{
    if(m_bSuccess)
    {
        COprateingActivityMgr::Instance()->SetOprateingActivityData(m_stOprateingActivityProp);
    }
}

