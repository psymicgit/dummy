
#include "framework/nsapplication.h"
#include "basedbccmd.h"
#include <sdutil.h>


CBaseDBCMD::CBaseDBCMD():
    m_bSuccess(FALSE)
{
}


CBaseDBCMD::~CBaseDBCMD()
{
}

VOID CBaseDBCMD::Release()
{
    ((CNSApplication*)SDGetApp())->GetDBMgr()->FreeCmd(this);
}

VOID CBaseDBCMD::SetUserData(const VOID* pUserData, const UINT32 dwLen, const UINT32 dwTransID)
{
    m_dwDataLen = dwLen;
    memset(m_pUserData, 0x00, sizeof(m_pUserData));
    memcpy(m_pUserData, pUserData, dwLen);
    m_dwTransID = dwTransID;
}


VOID CBaseDBCMD::ResponseMsg(UINT16 wMsgID, const CHAR* pBuf)
{
    //CZSClient* poZSClient = ((CNSApplication*)SDGetApp())->GetClientMgr()->FindCliSession(m_dwTransID);
    //if(poZSClient)
    //{
    //	poZSClient->SendMsg(wMsgID,pBuf);
    //	poZSClient->Disconnect();//返回区数据后断开链接
    //}
}


