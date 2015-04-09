
#include "framework/nsapplication.h"
#include "savefeedback.h"
#include "apns/defapns.h"
#include <sdutil.h>
#include <common/client/errdef.h>




CSaveFeedBackCMD::CSaveFeedBackCMD()
{

}


CSaveFeedBackCMD::~CSaveFeedBackCMD()
{

}

VOID CSaveFeedBackCMD::OnExecuteSql(SGDP::ISDDBConnection *poDBConn)
{
    m_bSuccess = FALSE;
    pResponce v_pResponce = (pResponce)m_pUserData;
    if(NULL == v_pResponce)
        return;

    CHAR szSQL[1024] = {0};
    sprintf(szSQL, "insert into dropnotice(NotifyID, DropTime) values ('%s', '%s');", v_pResponce->GetToken().c_str(), v_pResponce->GetTime().c_str());

    UINT32 dwErrorCode = 0;
    ISDDBRecordSet*pRecordSet = NULL;
    string strErrorDes;
    auto nRet = poDBConn->ExecuteSqlRs(szSQL, &pRecordSet, &dwErrorCode, &strErrorDes );
    if(NULL != pRecordSet)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed!ErrorCode:%d, ErrorStr:%s"), MSG_MARK, dwErrorCode, strErrorDes.c_str());
        pRecordSet->Release();
        pRecordSet = NULL;
    }

    if(nRet < 0)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: exec sql failed!ErrorCode:%d, ErrorStr:%s"), MSG_MARK, dwErrorCode, strErrorDes.c_str());
        return;
    }
}

VOID CSaveFeedBackCMD::OnExecuted()
{
    if(!m_bSuccess)
    {
        SYS_CRITICAL(_SDT("[%s: %d]: save data failed!"), MSG_MARK);
    }
}