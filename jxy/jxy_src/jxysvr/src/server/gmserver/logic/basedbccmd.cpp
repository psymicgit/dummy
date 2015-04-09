
#include "framework/gmapplication.h"
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
//     ((CGMApplication*)SDGetApp())->GetDBMgr()->FreeCmd(this);
}

VOID CBaseDBCMD::SetUserData(VOID* pUserData, UINT32 dwLen, UINT32 dwTransID)
{
    m_dwDataLen = dwLen;
    memcpy(m_pUserData, pUserData, dwLen);
	m_dwTransID = dwTransID;
}


VOID CBaseDBCMD::ResponseMsg(UINT16 wMsgID, const CHAR* pBuf)
{
	//CGMClient* poZSClient = ((CGMApplication*)SDGetApp())->GetClientMgr()->FindCliSession(m_dwTransID);
	//if(poZSClient)
	//{
	//	poZSClient->SendMsg(wMsgID,pBuf);
	//	//poZSClient->Disconnect();//返回区数据后断开链接
	//}
}


