#include "framework/lsapplication.h"
#include "syncdata.h"
#include <sdutil.h>
#include <sdtime.h>
#include <logic/dbconfig.h>
#include <net/gt/gtpktbuilder.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>
#include <logic/basefunc.h>
#include <algorithm/sdstrencrypt.h>

#include <sstream>
#include <ostream>
using namespace std;


CSyncDataCMD::CSyncDataCMD()
{
    m_wErrCode = 0;
    m_mapForbidLoginbyDeviceID.clear();
    m_mapForbidLoginbyUserID.clear();
    m_mapForbidLoginbyPlayerID.clear();
    m_mapForbidRegisteredbyDeviceID.clear();
    m_mapForbidRegisteredbyUserID.clear();

}

CSyncDataCMD::~CSyncDataCMD()
{
    m_mapForbidLoginbyDeviceID.clear();
    m_mapForbidLoginbyUserID.clear();
    m_mapForbidLoginbyPlayerID.clear();
    m_mapForbidRegisteredbyDeviceID.clear();
    m_mapForbidRegisteredbyUserID.clear();
}

VOID CSyncDataCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    GetVersionNotice(poDBConn);
    GetForbidLogin(poDBConn);
    GetForbidRegistered(poDBConn);
}

VOID CSyncDataCMD::OnExecuted()
{
    ((CLSApplication*)SDGetApp())->GetDBMgr()->SetVersionNoticeInf(m_stVersionNoticeInfo);

    ((CLSApplication*)SDGetApp())->GetDBMgr()->SetForbidLoginbyDeviceIDMap(m_mapForbidLoginbyDeviceID);
    ((CLSApplication*)SDGetApp())->GetDBMgr()->SetForbidLoginbyUserIDMap(m_mapForbidLoginbyUserID);
    ((CLSApplication*)SDGetApp())->GetDBMgr()->SetForbidLoginbyPlayerIDMap(m_mapForbidLoginbyPlayerID);

    ((CLSApplication*)SDGetApp())->GetDBMgr()->SetForbidRegisteredbyDeviceIDMap(m_mapForbidRegisteredbyDeviceID);
    ((CLSApplication*)SDGetApp())->GetDBMgr()->SetForbidRegisteredbyUserIDMap(m_mapForbidRegisteredbyUserID);
}


BOOL CSyncDataCMD::GetVersionNotice(SGDP::ISDDBConnection* poDBConn)
{
    m_stVersionNoticeInfo.Init();

    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from versionnotice  left join  zonenotice on (zonenotice.ZoneID = %u);", ((CLSApplication*)SDGetApp())->GetZoneID());
    ISDDBRecordSet* pRecordSet = NULL;
	string strErr;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, NULL, &strErr);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        if(pRecordSet->GetRecord())
        {
            m_stVersionNoticeInfo.stVersionInfo.dwVersionID = SDAtou(pRecordSet->GetFieldValueByName("VersionID"));
            m_stVersionNoticeInfo.dwMinUnupdateVersionID = SDAtou(pRecordSet->GetFieldValueByName("MinUnupdateVersionID"));
            m_stVersionNoticeInfo.stVersionInfo.byForceUpdateFlag = 0;

            //版本更新内容
            INT32 nLen = pRecordSet->GetFieldLengthByName("VersionUpdateContent");
            nLen = nLen >= MAX_CONTENT_SIZE ? (MAX_CONTENT_SIZE - 1) : nLen;
            SDStrncpy(m_stVersionNoticeInfo.stVersionInfo.aszUpdateContent, pRecordSet->GetFieldValueByName("VersionUpdateContent"), nLen);

            //版本更新URL地址
            nLen = pRecordSet->GetFieldLengthByName("VersionUpdateUrl");
            nLen = nLen >= MAX_CONTENT_SIZE ? (MAX_CONTENT_SIZE - 1) : nLen;
            SDStrncpy(m_stVersionNoticeInfo.stVersionInfo.aszUpdateURL, pRecordSet->GetFieldValueByName("VersionUpdateUrl"), nLen);

            //日常公告
            m_stVersionNoticeInfo.stNoticeInfo1.dwNoticeID = SDAtou(pRecordSet->GetFieldValueByName("NoticeID2"));
            m_stVersionNoticeInfo.stNoticeInfo1.byForcePopFlag = SDAtou(pRecordSet->GetFieldValueByName("ForcePopFlag2"));
            m_stVersionNoticeInfo.stNoticeInfo1.byUrlFlag = SDAtou(pRecordSet->GetFieldValueByName("UrlFlag2"));
            nLen = pRecordSet->GetFieldLengthByName("NoticeContent2");
            nLen = nLen >= MAX_CONTENT_SIZE ? (MAX_CONTENT_SIZE - 1) : nLen;
            SDStrncpy(m_stVersionNoticeInfo.stNoticeInfo1.aszNoticeContent, pRecordSet->GetFieldValueByName("NoticeContent2"), nLen);

            //维护公告
            m_stVersionNoticeInfo.stNoticeInfo2.dwNoticeID = SDAtou(pRecordSet->GetFieldValueByName("NoticeID3"));
            m_stVersionNoticeInfo.stNoticeInfo2.byForcePopFlag = SDAtou(pRecordSet->GetFieldValueByName("ForcePopFlag3"));
            m_stVersionNoticeInfo.stNoticeInfo2.byUrlFlag = SDAtou(pRecordSet->GetFieldValueByName("UrlFlag3"));
            nLen = pRecordSet->GetFieldLengthByName("NoticeContent3");
            nLen = nLen >= MAX_CONTENT_SIZE ? (MAX_CONTENT_SIZE - 1) : nLen;
            SDStrncpy(m_stVersionNoticeInfo.stNoticeInfo2.aszNoticeContent, pRecordSet->GetFieldValueByName("NoticeContent3"), nLen);

			/////////////////////////////////////////////////////////
			//////////////区公共存在，则取区的公告信息//////////////
			//GS内部公告
			const CHAR* pszTmp = pRecordSet->GetFieldValueByName("GsInnerNoticeID");
			if(pszTmp)
			{
				m_stVersionNoticeInfo.stNoticeInfo1.dwNoticeID = SDAtou(pszTmp);
			}
			pszTmp = pRecordSet->GetFieldValueByName("GsInnerNoticeForcePopFlag");
			if(pszTmp)
			{
				m_stVersionNoticeInfo.stNoticeInfo1.byForcePopFlag = SDAtou(pszTmp);
			}
			pszTmp = pRecordSet->GetFieldValueByName("GsInnerNoticeUrlFlag");
			if(pszTmp)
			{
				m_stVersionNoticeInfo.stNoticeInfo1.byUrlFlag = SDAtou(pszTmp);
			}
			pszTmp = pRecordSet->GetFieldValueByName("GsInnerNoticeNoticeContent");
			if(pszTmp)
			{
				nLen = pRecordSet->GetFieldLengthByName("GsInnerNoticeNoticeContent");
				nLen = nLen >= MAX_CONTENT_SIZE ? (MAX_CONTENT_SIZE - 1) : nLen;
				SDStrncpy(m_stVersionNoticeInfo.stNoticeInfo1.aszNoticeContent, pszTmp, nLen);
			}
			//区维护公共
			pszTmp = pRecordSet->GetFieldValueByName("MaintenanceNoticeID");
			if(pszTmp)
			{
				m_stVersionNoticeInfo.stNoticeInfo2.dwNoticeID = SDAtou(pszTmp);
			}
			pszTmp = pRecordSet->GetFieldValueByName("MaintenanceNoticeForcePopFlag");
			if(pszTmp)
			{
				m_stVersionNoticeInfo.stNoticeInfo2.byForcePopFlag = SDAtou(pszTmp);
			}
			pszTmp = pRecordSet->GetFieldValueByName("MaintenanceNoticeUrlFlag");
			if(pszTmp)
			{
				m_stVersionNoticeInfo.stNoticeInfo2.byUrlFlag = SDAtou(pszTmp);
			}
			pszTmp = pRecordSet->GetFieldValueByName("MaintenanceNoticeContent");
			if(pszTmp)
			{
				nLen = pRecordSet->GetFieldLengthByName("MaintenanceNoticeContent");
				nLen = nLen >= MAX_CONTENT_SIZE ? (MAX_CONTENT_SIZE - 1) : nLen;
				SDStrncpy(m_stVersionNoticeInfo.stNoticeInfo2.aszNoticeContent, pszTmp, nLen);
			}
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}

BOOL CSyncDataCMD::GetForbidLogin(SGDP::ISDDBConnection* poDBConn)
{
    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from forbidlogindeviceid;");
    ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        if(pRecordSet->GetRecord())
        {
            string	strDeviceID = pRecordSet->GetFieldValueByName("DeviceID");
            if (!strDeviceID.empty())
            {
                m_mapForbidLoginbyDeviceID[strDeviceID] = 1;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    sprintf(szSQL, "select * from forbidloginuserid;");
    nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        if(pRecordSet->GetRecord())
        {
            UINT32	dwUserID = SDAtou(pRecordSet->GetFieldValueByName("UserID"));
            if (0 != dwUserID)
            {
                m_mapForbidLoginbyUserID[dwUserID] = 1;
            }
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    sprintf(szSQL, "select * from forbidloginplayerid;");
    nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        if(pRecordSet->GetRecord())
        {
            UINT32	dwPlayerID = SDAtou(pRecordSet->GetFieldValueByName("PlayerID"));
            if (0 != dwPlayerID)
                m_mapForbidLoginbyPlayerID[dwPlayerID] = 1;
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    return TRUE;
}

BOOL CSyncDataCMD::GetForbidRegistered(SGDP::ISDDBConnection* poDBConn)
{
    CHAR szSQL[128] = {0};
    sprintf(szSQL, "select * from forbidregistereddeviceid;");
    ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        if(pRecordSet->GetRecord())
        {
            string	strDeviceID = pRecordSet->GetFieldValueByName("DeviceID");
            if(!strDeviceID.empty())
                m_mapForbidRegisteredbyDeviceID[strDeviceID] = 1;
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    sprintf(szSQL, "select * from forbidregistereduserid;");
    nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        if(pRecordSet->GetRecord())
        {
            UINT32	dwUserID = SDAtou(pRecordSet->GetFieldValueByName("UserID"));
            if (0 != dwUserID)
                m_mapForbidRegisteredbyUserID[dwUserID] = 1;
        }
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    return TRUE;
}