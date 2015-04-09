
#include "framework/nsapplication.h"
#include "twodaysnotonlinenoticeinfo.h"
#include "apns/defapns.h"
#include <sdutil.h>
#include <common/client/errdef.h>
#include "protocol/server/protogsns.h"
#include <framework/nsapplication.h>
extern CSDApplication* SDGetApp();


CTwoDaysNotOnlineNoticeInfoCMD::CTwoDaysNotOnlineNoticeInfoCMD()
{
	m_mapPlayerNotifyInfo.clear();
}


CTwoDaysNotOnlineNoticeInfoCMD::~CTwoDaysNotOnlineNoticeInfoCMD()
{
	m_mapPlayerNotifyInfo.clear();
}

VOID CTwoDaysNotOnlineNoticeInfoCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    CHAR szSql[1024] = {0};
    sprintf(szSql, "call TwoDaysNotOnline();");


    UINT32 dwErr = 0;
    string strErr;
    SPlayerNotifyInfo stNotifyInfo;
    SGDP::ISDDBRecordSet* pRecordSet = NULL;
    INT32 nRet = poDBConn->ExecuteSqlRs(szSql, &pRecordSet, &dwErr, &strErr);
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

VOID CTwoDaysNotOnlineNoticeInfoCMD::OnExecuted()
{
    if(m_bSuccess)
    {
		CHAR szPayLoad[256] = {0};
		sprintf(szPayLoad, "玩家已经超过两天未登录");

		CPlayerNotifyInfoMapItr itr = m_mapPlayerNotifyInfo.begin();
		for(itr; itr != m_mapPlayerNotifyInfo.begin(); itr++)
		{
			((CNSApplication*)SDGetApp())->GetNotifications()->SSL_PushNoticeEx(itr->second.aszToken, (CHAR*)SDLocal2UTF8(szPayLoad).c_str(), itr->second);
			SDSleep(0);//引起线程切换
		}
    }
}
