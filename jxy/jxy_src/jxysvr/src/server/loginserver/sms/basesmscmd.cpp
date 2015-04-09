
#include "framework/lsapplication.h"
#include "basesmscmd.h"
#include <sdutil.h>
#include "smsmgr.h"




CBaseSmsCMD::CBaseSmsCMD():
    m_bSuccess(FALSE)
{
    m_dwTransID = 0;
    m_dwDataLen = 0;
    memset(m_szParm, 0x00, sizeof(m_szParm));
    memset(m_pUserData, 0x00, sizeof(m_pUserData));
}

CBaseSmsCMD::~CBaseSmsCMD()
{

}

VOID CBaseSmsCMD::Release()
{
    CSmsMgr::Instance()->FreeCmd(this);
}

VOID CBaseSmsCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID, VOID* pParm)
{
    m_dwTransID = dwTransID;
    m_dwDataLen = dwLen;
    memcpy(m_pUserData, pUserData, dwLen);
    if (NULL != pParm)
    {
        SDStrcat(m_szParm, (CHAR*)pParm);
    }
}


VOID CBaseSmsCMD::ResponseMsg(UINT16 wMsgID, const CHAR* pBuf)
{
    CSDPipeChannel* pPipeSink = ((CLSApplication*)SDGetApp())->GetGTServer();
    if (pPipeSink)
    {
        pPipeSink->SendMsg(m_dwTransID, wMsgID, pBuf);
    }
}

VOID CBaseSmsCMD::OnExecuted(void)
{

}

//Êý¾ÝÖ´ÐÐ
VOID CBaseSmsCMD::OnExecute(void)
{

}

size_t CBaseSmsCMD::OnRecv(void *buffer, size_t size, size_t count, void *pData)
{
    if( NULL == pData )
    {
        return 0;
    }
    string *pstrNetData = (string*)pData;
    pstrNetData->append( (char *)buffer, size * count );
    return size * count;
}


