
#include <db/autosqlbuf.h>
#include "getgsnoticecmd.h"
#include <db/dbsqlstring.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include <common/server/base64.h>
#include <logic/chat/chatmgr.h>
#include <logic/other/gsnoticemgr.h>
#include <db/dbcommoperate.h>


CGetGsNoticeCMD::CGetGsNoticeCMD()
{
	memset(&m_stGsNoticeInfo, 0, sizeof(m_stGsNoticeInfo));
	m_wZoneID = 0;
	m_bSuccess = FALSE;
}


CGetGsNoticeCMD::~CGetGsNoticeCMD()
{

}

VOID CGetGsNoticeCMD::SetUserData(VOID* pUserData)
{
	m_wZoneID = *(UINT16*)pUserData;
}


VOID CGetGsNoticeCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{	

	CHAR szSQL[128] = {0};
	sprintf(szSQL, "select * from gsnotice where (ZoneID='%u' or ZoneID='0') order by ZoneID desc, NoticeID desc limit 1;", m_wZoneID);
	ISDDBRecordSet* pRecordSet = NULL;
	INT32 nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet);
	if((SDDB_HAS_RECORDSET == nRet) && (pRecordSet->GetRecord()))
	{
		//日常公告
		m_stGsNoticeInfo.dwNoticeID = SDAtou(pRecordSet->GetFieldValueByName("NoticeID"));
		m_stGsNoticeInfo.byForcePopFlag = SDAtou(pRecordSet->GetFieldValueByName("ForcePopFlag"));
		m_stGsNoticeInfo.byUrlFlag = SDAtou(pRecordSet->GetFieldValueByName("UrlFlag"));
		INT32 nLen = pRecordSet->GetFieldLengthByName("NoticeContent");
		nLen = nLen >= MAX_NOTICE_CONTENT_LEN ? (MAX_NOTICE_CONTENT_LEN - 1) : nLen;
		SDStrncpy(m_stGsNoticeInfo.aszNoticeContent, pRecordSet->GetFieldValueByName("NoticeContent"), nLen);		
		GetDateTime2Stamp(m_stGsNoticeInfo.qwExpiresTime,  "ExpiresTime"); 
	}
	if(NULL != pRecordSet)
	{
		pRecordSet->Release();
		pRecordSet = NULL;
	}	

	m_bSuccess = TRUE;
}


VOID CGetGsNoticeCMD::OnExecuted()
{
	if(m_bSuccess)
	{
		CGsNoticeMgr::Instance()->SetGsNoticeData(m_stGsNoticeInfo);
	}
}

