#include "stdafx.h"
#include "clidata.h"
#include "PacketRobotDlg.h"
#include <sdnetutils.h>
#include <WinSock2.h>

/// 构建网络头
INT32 CCliSession::BuildNetHeader(STNetMsgHeader* pstHead, UINT16 wMsgID, UINT32 dwDataLen)
{
	pstHead->wMark		= htons(PKTHEADMAK);
    pstHead->wMsgID		= htons(wMsgID);
    pstHead->dwDataLen	= htonl(dwDataLen);
    return dwDataLen + sizeof(STNetMsgHeader);
}

/// 获取连接
SGDP::ISDConnection* CCliSession::GetConnection()
{
    return m_poConnection;
}

/// 发送协议报文
BOOL CCliSession::SendMsg(UINT16 wMsgID, const CHAR* pBuf, INT32 nLen)
{
    CHAR acNetBuf[SD_CLIENT_SENDBUFFER] = {0};
	UINT32 dwOffset = sizeof(STNetMsgHeader);
	//先加密，若已经设置key
	if(m_oMGEncrypt.getKeyLength() > 0)
	{
		if(memcpy_s(acNetBuf + dwOffset + ENCRYPT_HEAD_LEN, SD_CLIENT_SENDBUFFER - dwOffset - ENCRYPT_HEAD_LEN, pBuf, nLen) != 0)
		{
			return FALSE;
		}

		UINT8* pszDecryptBuff =  (UINT8*)(acNetBuf + dwOffset);
		INT32 nDecryptBuffLen = nLen + ENCRYPT_HEAD_LEN + ENCRYPT_TAIL_LEN;//添加加解密头尾
		
		if(nDecryptBuffLen>0)
		{
			m_oMGEncrypt.encryptionDatas(pszDecryptBuff, nDecryptBuffLen);
			nLen = nDecryptBuffLen; //添加加解密头尾
		}	
	}
	else
	{
		if(memcpy_s(acNetBuf + dwOffset, SD_CLIENT_SENDBUFFER - dwOffset, pBuf, nLen) != 0)
		{
			return FALSE;
		}
	}

    STNetMsgHeader* pHeader = (STNetMsgHeader*)acNetBuf;
    INT32 nPackageLen = BuildNetHeader(pHeader, wMsgID, nLen);
	m_poConnection->Send(acNetBuf, nPackageLen);
    return TRUE;
}

//
// Name     : SetConnection
// Function : Set the ISDConnection object to the ISDSession object.
//
VOID SDAPI CCliSession::SetConnection(SGDP::ISDConnection* poConnection)
{
    m_poConnection = poConnection;
}

//
// Name     : OnEstablish
// Function : Callback when the connection is established.
//
VOID SDAPI CCliSession::OnEstablish()
{
    CGlobe::m_poDlg->OnConnect(0);
}

//
// Name     : OnTerminate
// Function : Callback when the connection is closed.
//
VOID SDAPI CCliSession::OnTerminate()
{
    CGlobe::m_poDlg->OnDisConnect(0);
}

//
// Name     : OnError
// Function : Callback when TCP connection has error.
//
bool SDAPI CCliSession::OnError(INT32 nModuleErr, INT32 nSysErr)
{
    // 连接失败在此报错
    CGlobe::m_poDlg->OnDisConnect(0);
    return false;
}

//
// Name     : OnRecv
// Function : Callback when receive a packet, which is parsed by ISDPacketParser.
//            
//
VOID SDAPI CCliSession::OnRecv(const char* pBuf, UINT32 dwLen)
{
    STNetMsgHeader* pNetMsgHeader = (STNetMsgHeader*)pBuf;
    UINT16 wMsgID = SDNtohs(pNetMsgHeader->wMsgID);
    UINT32 dwDataLen = SDNtohl(pNetMsgHeader->dwDataLen);
	CHAR* pData = (CHAR*)pBuf + sizeof(STNetMsgHeader);
    INT32 nPkgLen = dwDataLen + sizeof(STNetMsgHeader);
    if(nPkgLen < 0)
    {
        return;
    }
    if(nPkgLen < (INT32)dwLen)
    {
        return;
    }

	//先解密
	if(m_oMGEncrypt.getKeyLength()>0)
	{
		UINT8* pszEncryptBuff =  (UINT8*)(pBuf + sizeof(STNetMsgHeader));
		INT32 nEncryptBuffLen = dwLen - sizeof(STNetMsgHeader);
		if(nEncryptBuffLen>0)
		{
			if(!m_oMGEncrypt.decryptionDatas(pszEncryptBuff, nEncryptBuffLen))
			{
				return ;
			}
			dwDataLen = dwDataLen - ENCRYPT_HEAD_LEN - ENCRYPT_TAIL_LEN;
			pData = pData + ENCRYPT_HEAD_LEN;
		}	
	}

    CGlobe::m_poDlg->OnRecvMsg(wMsgID, (const CHAR*)(pData), dwDataLen);
}

//
// Name     : Release
// Function : Release ISDSession object. It will be callback when SDNet no
//            more use it.
//
VOID SDAPI CCliSession::Release()
{

}

