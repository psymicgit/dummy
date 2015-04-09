
#include "framework/lsapplication.h"
#include "basedbccmd.h"
#include <sdutil.h>
#include <logic/dbconfig.h>
#include "net/gt/gtpktbuilder.h"
#include "server/utility.h"


CBaseDBCMD::CBaseDBCMD():
    m_bSuccess(FALSE),
    m_pMiddleData(NULL)
{
}


CBaseDBCMD::~CBaseDBCMD()
{
}

VOID CBaseDBCMD::Release()
{
    ((CLSApplication*)SDGetApp())->GetDBMgr()->FreeCmd(this);
}

VOID CBaseDBCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwDataLen = dwLen;
    memcpy(m_pUserData, pUserData, dwLen);
    m_dwTransID = dwTransID;
}


VOID CBaseDBCMD::ResponseMsg(UINT16 wMsgID, const CHAR* pBuf)
{
    CSDPipeChannel* pPipeSink = ((CLSApplication*)SDGetApp())->GetGTServer();
    if (pPipeSink)
    {
        pPipeSink->SendMsg(m_dwTransID, wMsgID, pBuf);
    }
}


