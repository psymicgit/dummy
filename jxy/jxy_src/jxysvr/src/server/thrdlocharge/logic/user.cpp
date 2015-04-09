
#include "user.h"
#include "usermgr.h"
#include <framework/tlcapplication.h>
#include <sdutil.h>
#include <common/server/utility.h>
#include "net/ls/lspktbuilder.h"


#define MAX_TIME_OUT	3600


CUser::CUser()
{
	m_wZoneID = 0;
	m_dwTransID = 0;
	memset(&m_stReq, 0, sizeof(m_stReq));
	m_qwLastRecvMsgTime = SDTimeSecs();
}


CUser::~CUser()
{

}


VOID SDAPI CUser::OnAuthSuccess()
{
	CSDPipeChannel* poSDPipeChannel = ((CTLCApplication*)SDGetApp())->GetLSPipeChannel(m_wZoneID);
	if (poSDPipeChannel)
	{
		UINT32 dwMsgID = CLSPktBuilder::Instance()->LoginAck(EAEC_SUCCESS, m_stReq);
		if (FALSE == poSDPipeChannel->SendMsg(m_dwTransID, dwMsgID, CLSPktBuilder::Instance()->GetPacketBuffer()))
		{
			DBG_INFO(_SDT("[%s:%d]: send packet failed"), MSG_MARK);
			return ;
		}
	}

	CUserMgr::Instance()->ReleaseUser(this);
}


VOID SDAPI CUser::OnAuthFailed(UINT16 wErrCode, TCHAR* pszErrCode)
{
	CSDPipeChannel* poSDPipeChannel = ((CTLCApplication*)SDGetApp())->GetLSPipeChannel(m_wZoneID);
	if (poSDPipeChannel)
	{
		UINT32 dwMsgID = CLSPktBuilder::Instance()->LoginAck(wErrCode, m_stReq);
		if (FALSE == poSDPipeChannel->SendMsg(m_dwTransID, dwMsgID, CLSPktBuilder::Instance()->GetPacketBuffer()))
		{
			DBG_INFO(_SDT("[%s:%d]: send packet failed"), MSG_MARK);
			return ;
		}
	}

	CUserMgr::Instance()->ReleaseUser(this);
}


VOID CUser::SetUserProp(UINT16 wZoneID, UINT32 dwTransID, PKT_LSTLC_AUTH_REQ& stReq)
{
	m_wZoneID = wZoneID;
	m_dwTransID = dwTransID;
	m_stReq = stReq;

	m_qwLastRecvMsgTime = SDTimeSecs();
}

