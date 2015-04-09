#include "rechargeevent.h"

#include "net/recharge/rechargeclient.h"
#include <framework/gsapi.h>

CRechargeEvent::CRechargeEvent()
{

}

CRechargeEvent::~CRechargeEvent()
{

}

UINT32 CRechargeEvent::OnEvent()
{
	gsapi::GetRechargeClientMgr()->SendHeart();

	return 0;
}

INT32  CRechargeEvent::OnDestroy()
{
	return 0;
}

UINT32	CRechargeEvent::GetSendHeartInterval()
{
	return ms_nSendHeartInterval;
}