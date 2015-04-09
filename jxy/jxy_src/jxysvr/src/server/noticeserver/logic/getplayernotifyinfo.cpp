
#include "framework/nsapplication.h"
#include "getplayernotifyinfo.h"
#include "apns/defapns.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include "protocol/server/protogsns.h"




CGetPlayerNotifyInfoCMD::CGetPlayerNotifyInfoCMD()
{
	m_mapPlayerNotifyInfo.clear();
}


CGetPlayerNotifyInfoCMD::~CGetPlayerNotifyInfoCMD()
{
	m_mapPlayerNotifyInfo.clear();
}

VOID CGetPlayerNotifyInfoCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
	SPlayerNotifyInfo		stNotifyInfo;
    CHAR szSql[1024] = {0};
    sprintf(szSql, "call GetLastLoginUserInfo();");
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSql, &pRecordSet);
    if(SDDB_HAS_RECORDSET == nRet)
    {
        while(pRecordSet->GetRecord())
        {
			stNotifyInfo.Init();
			stNotifyInfo.dwPlayerID = SDAtou(pRecordSet->GetFieldValueByName("PlayerID"));
			stNotifyInfo.byZoneID = SDAtou( pRecordSet->GetFieldValueByName("ZoneID") );
			stNotifyInfo.byCareerID = SDAtou( pRecordSet->GetFieldValueByName("CareerID"));
			stNotifyInfo.byAuthType  = SDAtou( pRecordSet->GetFieldValueByName("AuthType"));

			UINT32	dwLen = pRecordSet->GetFieldLengthByName("NotifyID");
			SDStrncpy(stNotifyInfo.aszToken, pRecordSet->GetFieldValueByName("NotifyID"), dwLen);

			dwLen = pRecordSet->GetFieldLengthByName("UserName");
			SDStrncpy(stNotifyInfo.aszUserName, pRecordSet->GetFieldValueByName("UserName"), dwLen);

			m_mapPlayerNotifyInfo[stNotifyInfo.dwPlayerID] = stNotifyInfo;
        }
    }
    else if(SDDB_NO_RECORDSET == nRet)
    {
        if(NULL != pRecordSet)
        {
            pRecordSet->Release();
            pRecordSet = NULL;
        }
        return;
    }
    else
    {
        return;
    }
    if(NULL != pRecordSet)
    {
        pRecordSet->Release();
        pRecordSet = NULL;
    }
    m_bSuccess = TRUE;
}

VOID CGetPlayerNotifyInfoCMD::OnExecuted()
{
	if (m_bSuccess)
	{
		((CNSApplication*)SDGetApp())->SetPlayerNotifyInfoMap(m_mapPlayerNotifyInfo);
	}
	
}
