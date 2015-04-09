
#include "framework/zsapplication.h"
#include "logic/dbmgr.h"
#include "getversionnoticecmd2.h"
#include <sdutil.h>
#include <common/client/errdef.h>





CGetVersionNoticeCMD2::CGetVersionNoticeCMD2()
{
    memset(&m_stAck, 0, sizeof(m_stAck));
}


CGetVersionNoticeCMD2::~CGetVersionNoticeCMD2()
{
}

VOID CGetVersionNoticeCMD2::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;

    PKT_CLIZS_GET_VERSION_NOTICE_REQ2* pstReq = (PKT_CLIZS_GET_VERSION_NOTICE_REQ2*)m_pUserData;
    if(NULL == pstReq)
        return;

    if(GetVersionNotice(poDBConn, pstReq->dwCliVersion, m_stAck.stVersionInfo, m_stAck.stNoticeInfo))
    {
        m_bSuccess = TRUE;
    }
    else
    {
        m_bSuccess = FALSE;
    }
}

VOID CGetVersionNoticeCMD2::OnExecuted()
{
	// PKT_CLIZS_GET_VERSION_NOTICE_REQ2* pstReq = (PKT_CLIZS_GET_VERSION_NOTICE_REQ2*)m_pUserData;
	//((CZSApplication *)SDGetApp() )->GetDBMgr()->GetCliVerInfo(pstReq->dwCliVersion, m_stAck.stVersionInfo.dwResVer, m_stAck.stVersionInfo.dwUIVer);

    ResponseMsg(CLIZS_GET_VERSION_NOTICE_ACK2, (CHAR*)(&m_stAck));
}


BOOL CGetVersionNoticeCMD2::GetVersionNotice(SGDP::ISDDBConnection* poDBConn, UINT32 dwOldVersionID, DT_VERSION_DATA2& stVersionInfo, DT_NOTICE_DATA& stNoticeInfo)
{
 //   SVersionNoticeInfo2 stInfo = ((CZSApplication*)SDGetApp())->GetDBMgr()->GetVersionNotice2();
	//
	//memcpy(&stVersionInfo, &stInfo.stVersionInfo, sizeof(DT_VERSION_DATA2));
	//memcpy(&stNoticeInfo,  &stInfo.stNoticeInfo, sizeof(DT_NOTICE_DATA));
	//stVersionInfo.byForceUpdateFlag = (dwOldVersionID < stInfo.dwMinUnupdateVersionID) ? 1 : 0;

	////若版本一致，不下发更新内容
	//if(dwOldVersionID == stInfo.stVersionInfo.dwVersionID) 
	//{
	//	stVersionInfo.aszUpdateContent[0] = 0;
	//	stVersionInfo.aszUpdateURL[0] = 0;
	//}

	//SYS_CRITICAL(_SDT("dwOldVersionID :%u, stInfo.stVersionInfo.dwVersionID:%u, stVersionInfo.aszUpdateContent:%s, stVersionInfo.aszUpdateUR:%s"), 
	//	dwOldVersionID, stInfo.stVersionInfo.dwVersionID, stVersionInfo.aszUpdateContent, stVersionInfo.aszUpdateURL);

    return TRUE;
}