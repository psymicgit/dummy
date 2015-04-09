
#include "framework/zsapplication.h"
#include "synczonenotice.h"
#include "getversionnoticecmd.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include <common/server/utility.h>



CSyncZoneNoticeCMD::CSyncZoneNoticeCMD()
{
	m_mapVerID2CliVerInfo.clear();
}


CSyncZoneNoticeCMD::~CSyncZoneNoticeCMD()
{
	
}

VOID CSyncZoneNoticeCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
	//CAutoCycle oAutoCycle(__FUNCTION__);

	m_bSuccess = FALSE;

	CkZone(poDBConn);
	
	CkVersionNotice(poDBConn);

	CkVersionNotice2(poDBConn);

	CkResVer(poDBConn);
}

VOID CSyncZoneNoticeCMD::OnExecuted()
{
	//((CZSApplication*)SDGetApp())->GetDBMgr()->SyncZoneInfo(m_vecZoneInfo);
	//
	//((CZSApplication*)SDGetApp())->GetDBMgr()->SyncVersionNotice(m_stVersionNoticeInfo);

	//((CZSApplication*)SDGetApp())->GetDBMgr()->SyncVersionNotice2(m_stVersionNoticeInfo2);

	//((CZSApplication*)SDGetApp())->GetDBMgr()->SyncResVer(m_mapVerID2CliVerInfo);
	
}


BOOL CSyncZoneNoticeCMD::CkZone(SGDP::ISDDBConnection* poDBConn)
{	
	m_vecZoneInfo.clear();

	CHAR szSQL[1024] = {0};
	ISDDBRecordSet* pRecordSet = NULL;

	CHAR szErrMsg[128] = {0};
	sprintf(szSQL, "select * from zone order by ID;");
	SYS_CRITICAL(_SDT("%s"), szSQL);

	UINT32 dwUserID = 0;
	INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
	while((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
	{
		DT_ZONE_DATA_INFO stDT_ZONE_DATA_INFO;
		stDT_ZONE_DATA_INFO.Init();
		stDT_ZONE_DATA_INFO.stDT_ZONE_DATA.wID = SDAtou(pRecordSet->GetFieldValueByName("ID"));
		stDT_ZONE_DATA_INFO.stDT_ZONE_DATA.byDomainLen = pRecordSet->GetFieldLengthByName("DomainName");
		stDT_ZONE_DATA_INFO.stDT_ZONE_DATA.byDomainLen = stDT_ZONE_DATA_INFO.stDT_ZONE_DATA.byDomainLen > MAX_DOMAIN_NAME_LEN ? MAX_DOMAIN_NAME_LEN : stDT_ZONE_DATA_INFO.stDT_ZONE_DATA.byDomainLen;
		memcpy(stDT_ZONE_DATA_INFO.stDT_ZONE_DATA.abyDomainName, pRecordSet->GetFieldValueByName("DomainName"), stDT_ZONE_DATA_INFO.stDT_ZONE_DATA.byDomainLen);

		stDT_ZONE_DATA_INFO.stDT_ZONE_DATA.wZonePort = SDAtou(pRecordSet->GetFieldValueByName("Port"));
		SDStrncpy(stDT_ZONE_DATA_INFO.stDT_ZONE_DATA.aszZoneName, pRecordSet->GetFieldValueByName("Name"), USERNAME_LEN-1);		
		stDT_ZONE_DATA_INFO.stDT_ZONE_DATA.byBusyFlag = SDAtou(pRecordSet->GetFieldValueByName("BusyFlag"));

		stDT_ZONE_DATA_INFO.byVisible = SDAtou(pRecordSet->GetFieldValueByName("IsVisible"));

		stDT_ZONE_DATA_INFO.wMinVersionID = SDAtou(pRecordSet->GetFieldValueByName("MinVersionID"));
		stDT_ZONE_DATA_INFO.wMaxVersionID = SDAtou(pRecordSet->GetFieldValueByName("MaxVersionID"));
		m_vecZoneInfo.push_back(stDT_ZONE_DATA_INFO);
	}
	if(NULL != pRecordSet)
	{
		pRecordSet->Release();
		pRecordSet = NULL;
	}

	return TRUE;
}

BOOL CSyncZoneNoticeCMD::CkVersionNotice(SGDP::ISDDBConnection* poDBConn)
{
	memset(&m_stVersionNoticeInfo, 0, sizeof(m_stVersionNoticeInfo));
	CHAR szSQL[128] = {0};

	DT_VERSION_DATA&		stVersionInfo = m_stVersionNoticeInfo.stVersionInfo;
	DT_NOTICE_DATA&		stNoticeInfo = m_stVersionNoticeInfo.stNoticeInfo;
	FLOAT fOldVersionID = 1.0;

	sprintf(szSQL, "select * from versionnotice;");
	SYS_CRITICAL(_SDT("%s"), szSQL);

	ISDDBRecordSet* pRecordSet = NULL;
	INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
	if(SDDB_HAS_RECORDSET == nRet)
	{
		if(pRecordSet->GetRecord())
		{
			UINT32 dwVersionID = SDAtou(pRecordSet->GetFieldValueByName("VersionID"));
			stVersionInfo.dwVersionID = dwVersionID;
			m_stVersionNoticeInfo.dwMinUnupdateVersionID = SDAtou(pRecordSet->GetFieldValueByName("MinUnupdateVersionID"));
			stVersionInfo.byForceUpdateFlag = (fOldVersionID < m_stVersionNoticeInfo.dwMinUnupdateVersionID) ? 1 : 0;

			//版本更新内容
			INT32 nLen = pRecordSet->GetFieldLengthByName("VersionUpdateContent");
			nLen = nLen >= MAX_CONTENT_SIZE ? (MAX_CONTENT_SIZE - 1) : nLen;
			SDStrncpy(stVersionInfo.aszUpdateContent, pRecordSet->GetFieldValueByName("VersionUpdateContent"), nLen);

			//版本更新URL地址
			nLen = pRecordSet->GetFieldLengthByName("VersionUpdateUrl");
			nLen = nLen >= MAX_CONTENT_SIZE ? (MAX_CONTENT_SIZE - 1) : nLen;
			SDStrncpy(stVersionInfo.aszUpdateURL, pRecordSet->GetFieldValueByName("VersionUpdateUrl"), nLen);


			//显示的是全服维护公告
			stNoticeInfo.dwNoticeID = SDAtou(pRecordSet->GetFieldValueByName("NoticeID"));
			stNoticeInfo.byForcePopFlag = SDAtou(pRecordSet->GetFieldValueByName("ForcePopFlag"));
			stNoticeInfo.byUrlFlag = SDAtou(pRecordSet->GetFieldValueByName("UrlFlag"));
			nLen = pRecordSet->GetFieldLengthByName("NoticeContent2");
			nLen = nLen >= MAX_CONTENT_SIZE ? (MAX_CONTENT_SIZE - 1) : nLen;
			SDStrncpy(stNoticeInfo.aszNoticeContent, pRecordSet->GetFieldValueByName("NoticeContent"), nLen);
		}
	}
	else
	{
		if(NULL != pRecordSet)
		{
			pRecordSet->Release();
			pRecordSet = NULL;
		}
		return FALSE;
	}
	if(NULL != pRecordSet)
	{
		pRecordSet->Release();
		pRecordSet = NULL;
	}

	return TRUE;
}


BOOL CSyncZoneNoticeCMD::CkVersionNotice2(SGDP::ISDDBConnection* poDBConn)
{
	memset(&m_stVersionNoticeInfo2, 0, sizeof(m_stVersionNoticeInfo2));
	CHAR szSQL[128] = {0};

	DT_VERSION_DATA2&		stVersionInfo = m_stVersionNoticeInfo2.stVersionInfo;
	DT_NOTICE_DATA&		stNoticeInfo	 = m_stVersionNoticeInfo2.stNoticeInfo;
	FLOAT fOldVersionID = 1.0;

	sprintf(szSQL, "select * from versionnotice;");
	SYS_CRITICAL(_SDT("%s"), szSQL);

	ISDDBRecordSet* pRecordSet = NULL;
	INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
	if(SDDB_HAS_RECORDSET == nRet)
	{
		if(pRecordSet->GetRecord())
		{
			UINT32 dwVersionID = SDAtou(pRecordSet->GetFieldValueByName("VersionID"));
			stVersionInfo.dwVersionID = dwVersionID;
			m_stVersionNoticeInfo2.dwMinUnupdateVersionID = SDAtou(pRecordSet->GetFieldValueByName("MinUnupdateVersionID"));
			stVersionInfo.byForceUpdateFlag = (fOldVersionID < m_stVersionNoticeInfo.dwMinUnupdateVersionID) ? 1 : 0;

			//版本更新内容
			INT32 nLen = pRecordSet->GetFieldLengthByName("VersionUpdateContent");
			nLen = nLen >= MAX_CONTENT_SIZE ? (MAX_CONTENT_SIZE - 1) : nLen;
			SDStrncpy(stVersionInfo.aszUpdateContent, pRecordSet->GetFieldValueByName("VersionUpdateContent"), nLen);

			//版本更新URL地址
			nLen = pRecordSet->GetFieldLengthByName("VersionUpdateUrl");
			nLen = nLen >= MAX_CONTENT_SIZE ? (MAX_CONTENT_SIZE - 1) : nLen;
			SDStrncpy(stVersionInfo.aszUpdateURL, pRecordSet->GetFieldValueByName("VersionUpdateUrl"), nLen);


			//显示的是全服维护公告
			stNoticeInfo.dwNoticeID = SDAtou(pRecordSet->GetFieldValueByName("NoticeID"));
			stNoticeInfo.byForcePopFlag = SDAtou(pRecordSet->GetFieldValueByName("ForcePopFlag"));
			stNoticeInfo.byUrlFlag = SDAtou(pRecordSet->GetFieldValueByName("UrlFlag"));
			nLen = pRecordSet->GetFieldLengthByName("NoticeContent2");
			nLen = nLen >= MAX_CONTENT_SIZE ? (MAX_CONTENT_SIZE - 1) : nLen;
			SDStrncpy(stNoticeInfo.aszNoticeContent, pRecordSet->GetFieldValueByName("NoticeContent"), nLen);
		}
	}
	else
	{
		if(NULL != pRecordSet)
		{
			pRecordSet->Release();
			pRecordSet = NULL;
		}
		return FALSE;
	}
	if(NULL != pRecordSet)
	{
		pRecordSet->Release();
		pRecordSet = NULL;
	}

	return TRUE;
}


BOOL CSyncZoneNoticeCMD::CkResVer(SGDP::ISDDBConnection* poDBConn)
{	
	SGDP::ISDDBRecordSet* pRes = NULL;
	UINT32 dwErrID = 0;
	string strErr;
	INT32 nRet = poDBConn->ExecuteSqlRs("select * from cliverinfo order by CLiVer;", &pRes, &dwErrID,  &strErr);
	if(SDDB_HAS_RECORDSET != nRet)
	{
		return TRUE;
	}
	while(pRes->GetRecord())// 当有行结果时
	{
		SCliVerInfoProp stProp;
		stProp.dwCliVer = SGDP::SDAtou(pRes->GetFieldValueByName("CLiVer"));
		stProp.dwResVer = SGDP::SDAtou(pRes->GetFieldValueByName("ResVer"));
		stProp.dwUIVer = SGDP::SDAtou(pRes->GetFieldValueByName("UIVer"));

		m_mapVerID2CliVerInfo[stProp.dwCliVer] = stProp;
	}
	if(NULL != pRes)
	{
		pRes->Release();
		pRes = NULL;
	}	

	return TRUE;
}

