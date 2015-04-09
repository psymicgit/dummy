
#include "framework/zsapplication.h"
#include "basedbccmd.h"
#include <sdutil.h>


CBaseDBCMD::CBaseDBCMD():
    m_bSuccess(FALSE)
{
    m_dwTransID = 0;
    m_dwDataLen = 0;
    memset(m_szParm, 0x00, sizeof(m_szParm));
    memset(m_pUserData, 0x00, sizeof(m_pUserData));
}


CBaseDBCMD::~CBaseDBCMD()
{
}

VOID CBaseDBCMD::Release()
{
    //((CZSApplication*)SDGetApp())->GetDBMgr()->FreeCmd(this);
}

VOID CBaseDBCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID, VOID* pParm)
{
    m_dwTransID = dwTransID;
    m_dwDataLen = dwLen;
    memcpy(m_pUserData, pUserData, dwLen);
    if (NULL != pParm)
    {
        SDStrcat(m_szParm, (CHAR*)pParm);
    }
}


VOID CBaseDBCMD::ResponseMsg(UINT16 wMsgID, const CHAR* pBuf)
{

    CZSClient* poZSClient = ((CZSApplication*)SDGetApp())->GetClientMgr()->FindCliSession(m_dwTransID);
    if(poZSClient)
    {
        poZSClient->SendMsg(wMsgID, pBuf);
        //poZSClient->Disconnect();//返回区数据后断开链接
    }
}


