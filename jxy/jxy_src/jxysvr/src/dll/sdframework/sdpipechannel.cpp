#include "sdpipechannel.h"
#include "sdutil.h"
#include "sdstring.h"
#include "sdnetutils.h"
#include "sdatomic.h"
#include "sdtype.h"
#include "sdserverid.h"
#include "sdapplication.h"
#include "sdloggerimpl.h"
#include "sdpkg.h"

namespace SGDP{

	UINT32 CSDPipeChannel::m_dwIndex = 0;
	CSDPipeChannel::CSDPipeChannel()
	{
		m_poPipe			= NULL;
		m_byServerType      = 0;
		m_poPacketProcessor = NULL;

		SDAtomicInc32((INT32*)&CSDPipeChannel::m_dwIndex);
	}

	CSDPipeChannel::~CSDPipeChannel()
	{
		SDAtomicDec32((INT32*)&CSDPipeChannel::m_dwIndex);
	}

	void CSDPipeChannel::SetPipe(const ISDPipe* pPipe)
	{
		m_poPipe = (ISDPipe*)pPipe;
		if (m_poPipe)
		{
			CSDServerID serverId (m_poPipe->GetID());
			m_byServerType  = serverId.GetServerType();
		}
	}

	VOID CSDPipeChannel::SetPacketProcessor(CSDPacketProcessor*    poPacketProcessor)
	{
		m_poPacketProcessor = poPacketProcessor;
	}

	VOID CSDPipeChannel::OnEstablish()
	{
		USR_INFO(_SDT("pipe(%s) has connected, ip: %s" ) ,SDServerIDUtoa(GetPipeID()).c_str(), GetRemoteIPStr().c_str());
	}

	VOID CSDPipeChannel::OnTerminate()
	{
		USR_INFO(_SDT("pipe(%s) has terminated, ip: %s" ) ,SDServerIDUtoa(GetPipeID()).c_str(), GetRemoteIPStr().c_str());
	}

	VOID CSDPipeChannel::OnError(const INT32 nErrCode)
	{
		USR_INFO(_SDT("pipe(%s) has error: %d, ip: %s" ) ,SDServerIDUtoa(GetPipeID()).c_str(), nErrCode, GetRemoteIPStr().c_str());
	}

	UINT32 CSDPipeChannel::GetPipeID() const
	{
		return m_poPipe == NULL ? 0 : m_poPipe->GetID();
	}

	UINT8 CSDPipeChannel::GetSvrType() const
	{
		return m_byServerType;
	}

	BOOL CSDPipeChannel::Send(const CHAR* pszData,const UINT32 dwLen)
	{
		if (NULL == m_poPipe)
		{
			return FALSE;
		}
		return m_poPipe->Send(BUSINESSID_ALL,pszData,dwLen);
	}

	BOOL CSDPipeChannel::SendMsg(UINT32 dwTransID, UINT32 dwMsgID ,const CHAR* pData)
	{
		CSDProtocol* pProtocol = m_poPacketProcessor->GetProtocol();
		if ( NULL == pProtocol )
		{
			return FALSE;
		}
		static CHAR  MsgData[PACKET_LENGTH]={0};
		CHAR* pMsgBody = MsgData + sizeof(SDPipeMsgHeader); 
		INT32 nBodyLength = PACKET_LENGTH - sizeof(SDPipeMsgHeader);
		INT32 nEncodeRet = pProtocol->Encode(dwMsgID,(VOID*) pData, pMsgBody, nBodyLength);
		if ( nEncodeRet < 0)
		{
			USR_INFO(_SDT(" Encode failed! MsgID[%d]." ) ,dwMsgID);
			return FALSE;
		}
		SDPipeMsgHeader* pTempHeader = (SDPipeMsgHeader *)(MsgData); 
		if ( 3043 == dwMsgID)//CLIGS_OPEN_PLAYER_PANEL_ACK_OLD//°æ±¾¼æÈÝÎÊÌâ
		{
			dwMsgID = 2823;//CLIGS_OPEN_PLAYER_PANEL_ACK;
		}
		INT32 nTotalLen = nEncodeRet + SDBuildPipeMsgHeader(pTempHeader,dwMsgID,dwTransID);
		return Send(MsgData, nTotalLen);
	}

	TCHAR* CSDPipeChannel::GetLocalIPStr() const 
	{
		return NULL;
	}

	UINT32 CSDPipeChannel::GetLocalIP() const 
	{
		return 0;
	}

	UINT16  CSDPipeChannel::GetLocalPort() const
	{
		return 0;
	}

	tstring CSDPipeChannel::GetRemoteIPStr() const 
	{
		if (m_poPipe)
		{
			return SDInetNtoa(m_poPipe->GetIP());
		}
		return "";
	}

	UINT32 CSDPipeChannel::GetRemoteIP() const 
	{
		if (m_poPipe)
		{
			return m_poPipe->GetIP();
		}
		return 0;
	}

	UINT16  CSDPipeChannel::GetRemotePort() const
	{
		return 0;
	}

	VOID CSDPipeChannel::OnRecv(UINT16 wBusinessID, const CHAR *pData, UINT32 dwLen)
	{
		if (m_poPacketProcessor)
		{
			CSDProtocol* poProtocol = m_poPacketProcessor->GetProtocol();
			if (poProtocol)
			{
				SDPipeMsgHeader* pHeader = (SDPipeMsgHeader*)pData;
				pHeader->dwTransID = SGDP::SDNtohl(pHeader->dwTransID);
				pHeader->wMsgID = SGDP::SDNtohs(pHeader->wMsgID);
				static CHAR  DecodeData[PACKET_LENGTH] = {0};
				INT32 nDecodeLen = poProtocol->Decode( pHeader->wMsgID,
					(CHAR*)(pData + sizeof(SDPipeMsgHeader) ) , 
					dwLen - sizeof(SDPipeMsgHeader),
					DecodeData,
					PACKET_LENGTH);

				if ( nDecodeLen > 0)
				{
					m_poPacketProcessor->ProcessPacket(this,
						pHeader, 
						DecodeData,
						nDecodeLen);
				}
				else
				{
					CSDServerID servierid(GetPipeID());
					USR_INFO(_SDT("decode packet error: pipeid=%s, ip=%s, msgid=%u"), SDServerIDUtoa(GetPipeID()).c_str(), GetRemoteIPStr().c_str(), pHeader->wMsgID);
				}
			}
		}
	}

	VOID CSDPipeChannel::OnReport(UINT16 wBusinessID, INT32 nErrCode)
	{
		if (PIPE_SUCCESS == nErrCode)
		{
			OnEstablish();
		}
		else if (PIPE_DISCONNECT == nErrCode)
		{
			OnTerminate();
		}
		else 
		{
			OnError(nErrCode);
		}
	}


	CSDPipeChannelMgr::CSDPipeChannelMgr()
	{
		m_poPipeModule = NULL;
	}

	CSDPipeChannelMgr::~CSDPipeChannelMgr()
	{
		m_mapPipeChannel.clear();
	}


	VOID CSDPipeChannelMgr::OnReport(INT32 nErrCode, UINT32 dwID)
	{
		if(SGDP::PIPE_SUCCESS == nErrCode)
		{
			_OnConnected(dwID);
			CSDPipeChannel* poPipeChannel = FindPipeChannel(dwID);
			if (poPipeChannel)
			{
				poPipeChannel->OnEstablish();
			}
		}
		else
		{
			USR_INFO(_SDT("PipeID, %s, Pipe report: %d\n"), SGDP::SDServerIDUtoa(dwID).c_str(), nErrCode);
			_OnDisconnected(dwID);
		}

	}

	VOID CSDPipeChannelMgr::_OnConnected(UINT32 dwPipeID)
	{
		CSDApplication* pApp = SDGetApp();
		if (NULL == pApp)
			return;

		CSDPipeChannel * poPipeChannel = SDGetApp()->OnPipeChannelConnected(dwPipeID);
		if (poPipeChannel && m_poPipeModule)
		{
			ISDPipe* poPipe = m_poPipeModule->GetPipe(dwPipeID);
			if (poPipe)
			{
				poPipeChannel->SetPipe(poPipe);
				poPipe->SetSink(BUSINESSID_ALL,poPipeChannel);
			}
			SDGetApp()->UpdateSvrStatus(dwPipeID,poPipe->GetIP(), TRUE);
			AddPipeChannel(poPipeChannel);
		}
	}

	VOID CSDPipeChannelMgr::_OnDisconnected(UINT32 dwPipeID)
	{
		CSDApplication* poApp = SDGetApp();
		if (NULL == SDGetApp())
			return;

		CSDPipeChannel* poPipeChannel = FindPipeChannel(dwPipeID);
		if (poPipeChannel)
		{
			poApp->UpdateSvrStatus(dwPipeID,poPipeChannel->GetRemoteIP(), FALSE);
			poApp->OnPipeChannelDisconnected(poPipeChannel);
			RemovePipeChannel(dwPipeID);
		}
		ISDPipe * poPipe = m_poPipeModule->GetPipe(dwPipeID);
		if(poPipe != NULL)
		{
			poPipe->SetSink(BUSINESSID_ALL, NULL);        
		}
	}

	VOID CSDPipeChannelMgr::Init(ISDPipeModule* pPipeModule)
	{
		m_poPipeModule = pPipeModule;
	}
	VOID CSDPipeChannelMgr::UnInit()
	{
		m_poPipeModule = NULL;
	}
	VOID CSDPipeChannelMgr::AddPipeChannel( CSDPipeChannel* poPipeChannel )
	{
		if (NULL != FindPipeChannel(poPipeChannel->GetPipeID()))
			return;

		m_mapPipeChannel.insert(make_pair(poPipeChannel->GetPipeID(), poPipeChannel));
	}

	VOID CSDPipeChannelMgr::RemovePipeChannel(const UINT32 dwID)
	{
		map<UINT32,CSDPipeChannel*>::iterator itr = m_mapPipeChannel.find(dwID);
		if ( itr != m_mapPipeChannel.end() )
		{
			m_mapPipeChannel.erase(itr);
		}
	}

	CSDPipeChannel* CSDPipeChannelMgr::FindPipeChannel(const UINT32 dwID)
	{
		map<UINT32,CSDPipeChannel*>::iterator itr = m_mapPipeChannel.find (dwID);
		if ( itr != m_mapPipeChannel.end() )
		{
			return (CSDPipeChannel*)itr->second;
		}
		return NULL;
	}

	VOID CSDPipeChannelMgr::BroadcastToSameTypeServers(const UINT32 dwTranID,const UINT16 wMsgID,const CHAR*  pData,const UINT8 byType)
	{
		map<UINT32,CSDPipeChannel*>::iterator itr = m_mapPipeChannel.begin();
		for (; itr != m_mapPipeChannel.end(); itr ++ )
		{
			CSDPipeChannel* poChannel = (CSDPipeChannel*)itr->second;
			if (poChannel&& poChannel->GetSvrType() == byType)
			{
				poChannel->SendMsg(dwTranID,wMsgID,pData);               
			}
		}
	}

	CSDPipeChannelMgr::CPipeChannelMap& CSDPipeChannelMgr::GetPipeChannelMap()
	{
		return m_mapPipeChannel;
	}

} 


