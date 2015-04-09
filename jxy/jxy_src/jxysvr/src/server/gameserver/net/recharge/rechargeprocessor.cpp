#include "rechargeprocessor.h"
#include "rechargeclient.h"
#include <sdutil.h>
#include <db/autosqlbuf.h>
#include <protocol/server/protorechargegs.h>
#include "rechargepktbuilder.h"
#include <logic/player/player.h>
#include <common/client/commondef.h>


CRechargeProcessor::CRechargeProcessor()
{
	Init();
}

CRechargeProcessor::~CRechargeProcessor()
{
}

BOOL CRechargeProcessor::Init()
{
	RegisterCommand(RECGS_RECHARGE_REQ, CRechargeProcessor::OnRechargeReq); 
	RegisterCommand(RECGS_HEART_REQ, CRechargeProcessor::OnHeartReq);
	RegisterCommand(RECGS_GEN_GUID_ACK, CRechargeProcessor::OnRecvOrderIDAckFromExchange);
	RegisterCommand(RECGS_PROCESS_ORDER_ID_REQ, CRechargeProcessor::OnExchangeResultReq);
	//RegisterCommand(RECGS_CHECK_APP_ORDER_ID_ACK, CRechargeProcessor::OnAppOrderIDAckFromExchange);
	
	return TRUE;
}

CSDProtocol* CRechargeProcessor::GetProtocol()
{
	return CProtoRechargeGS::Instance();

	return NULL;
}


BOOL CRechargeProcessor::OnRechargeReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
	CRechargeClient* poRechargeCli = (CRechargeClient*)poReceiver;
	PKT_RECGS_RECHARGE_REQ* pstReq = (PKT_RECGS_RECHARGE_REQ*)pszBody;
	
	//todo

	return TRUE;
}

BOOL CRechargeProcessor::OnHeartReq(VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen)
{
	CRechargeClient* poRechargeCli = (CRechargeClient*)poReceiver;
	PKT_RECGS_HEART_REQ* pstReq = (PKT_RECGS_HEART_REQ*)pszBody;

	//todo

	return TRUE;
}

BOOL CRechargeProcessor::OnRecvOrderIDAckFromExchange( VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen )
{
	CRechargeClient* poRechargeCli = (CRechargeClient*)poReceiver;

	PKT_RECGS_GEN_GUID_ACK* pstAck=(PKT_RECGS_GEN_GUID_ACK*)pszBody;

	if( sizeof(PKT_RECGS_GEN_GUID_ACK) != dwLen )
	{
		return FALSE;
	}
	
	CPlayer *pPlayer=NULL;
	UINT16 wMsg=CRechargePktBuilder::Instance()->GenGUIDAck2GT( pstAck->szInfo, &pPlayer );
	if( wMsg==INVALID_MSGID )
	{
		return FALSE;
	}
	
	return pPlayer->SendMsg( CRechargePktBuilder::Instance()->GetPacketBuffer(), wMsg );
}

BOOL CRechargeProcessor::OnExchangeResultReq( VOID* poReceiver, VOID* pHeader, const CHAR* pszBody, UINT32 dwLen )
{
	CRechargeClient* poRechargeCli = (CRechargeClient*)poReceiver;

	PKT_RECGS_PROCESS_ORDER_ID_REQ* pstAck=(PKT_RECGS_PROCESS_ORDER_ID_REQ*)pszBody;

	if( sizeof(PKT_RECGS_PROCESS_ORDER_ID_REQ) != dwLen )
	{
		return FALSE;
	}

	CRechargePktBuilder::Instance()->ProcessExchangeAck2GT( pstAck->szInfo, poRechargeCli );
	return TRUE;
}
