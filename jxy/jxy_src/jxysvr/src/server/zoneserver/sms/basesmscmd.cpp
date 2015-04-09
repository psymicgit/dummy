
#include "framework/zsapplication.h"
#include "basesmscmd.h"
#include <sdutil.h>
#include "smsmgr.h"




CBaseSmsCMD::CBaseSmsCMD():
    m_bSuccess(FALSE)
{
    m_dwTransID = 0;
    m_dwDataLen = 0;
	m_dwUserID = 0;
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

VOID CBaseSmsCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID, UINT32 dwUserID, VOID* pParm)
{
    m_dwTransID = dwTransID;
    m_dwDataLen = dwLen;
	m_dwUserID = dwUserID;
    memcpy(m_pUserData, pUserData, dwLen);
    if (NULL != pParm)
    {
        SDStrcat(m_szParm, (CHAR*)pParm);
    }
}


VOID CBaseSmsCMD::ResponseMsg(UINT16 wMsgID, const CHAR* pBuf)
{
	CZSClient* poZSClient = ((CZSApplication*)SDGetApp())->GetClientMgr()->FindCliSession(m_dwTransID);
	if(poZSClient)
	{
		poZSClient->SendMsg(wMsgID, pBuf);
		//poZSClient->Disconnect();//返回区数据后断开链接
	}
}

VOID CBaseSmsCMD::OnExecuted(void)
{

}

//数据执行
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


