
#include "framework/zsapplication.h"
#include "getversionnoticecmd.h"
#include <sdutil.h>
#include <common/client/errdef.h>




CGetVersionNoticeCMD::CGetVersionNoticeCMD()
{
    memset(&m_stAck, 0, sizeof(m_stAck));
}


CGetVersionNoticeCMD::~CGetVersionNoticeCMD()
{
}

VOID CGetVersionNoticeCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;

    PKT_CLIZS_GET_VERSION_NOTICE_REQ* pstReq = (PKT_CLIZS_GET_VERSION_NOTICE_REQ*)m_pUserData;
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

VOID CGetVersionNoticeCMD::OnExecuted()
{
    ResponseMsg(CLIZS_GET_VERSION_NOTICE_ACK, (CHAR*)(&m_stAck));
}


BOOL CGetVersionNoticeCMD::GetVersionNotice(SGDP::ISDDBConnection* poDBConn, UINT32 dwOldVersionID, DT_VERSION_DATA& stVersionInfo, DT_NOTICE_DATA& stNoticeInfo)
{
 //   SVersionNoticeInfo stInfo = ((CZSApplication*)SDGetApp())->GetDBMgr()->GetVersionNotice();
	//
	//memcpy(&stVersionInfo, &stInfo.stVersionInfo, sizeof(DT_VERSION_DATA));
	//memcpy(&stNoticeInfo, &stInfo.stNoticeInfo, sizeof(DT_NOTICE_DATA));
	//stVersionInfo.byForceUpdateFlag = (dwOldVersionID < stInfo.dwMinUnupdateVersionID) ? 1 : 0;

	////若版本一致，不下发更新内容
	//if(dwOldVersionID == stInfo.stVersionInfo.dwVersionID) 
	//{
	//	stVersionInfo.aszUpdateContent[0] = 0;
	//	stVersionInfo.aszUpdateURL[0] = 0;
	//}

    return TRUE;
}