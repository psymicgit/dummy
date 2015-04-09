#include "drinkwine.h"
#include <net/gt/gtpktbuilder.h>
#include <logic/player/playermgr.h>
#include "timereventmgr.h"
#include "protocol/server/protocligs.h"
CDrinkWineEvent::CDrinkWineEvent()
{
	m_dwPlayerID = 0;
}
CDrinkWineEvent::~CDrinkWineEvent()
{
	m_dwPlayerID = 0;
}
/**
	* @brief
	* 超时处理函数
	*/
UINT32 CDrinkWineEvent::OnEvent()
{
	CPlayer* poPlayer = CPlayerMgr::Instance()->FindPlayer(m_dwPlayerID);
	if ( NULL == poPlayer)
	{
		return 0;
	}
	UINT16 wMsgID = CGTPktBuilder::Instance()->HotSpringRewardNtf(poPlayer);
	poPlayer->SendMsg(CGTPktBuilder::Instance()->GetPacketBuffer(), wMsgID);
	PKT_CLIGS_RESOURCE_SYNC_NTF stNtf;
	poPlayer->GetSyncResource(stNtf.stResourceSync);
	poPlayer->SendMsg((TCHAR*)&stNtf, CLIGS_RESOURCE_SYNC_NTF);
	CTimerEventMgr::Instance()->DelDrinkWineEvent(m_dwPlayerID);

	CFactionPlayer* poFactionPlayer = poPlayer->GetFactionPlayer();
	if ( NULL != poFactionPlayer )
	{
		poFactionPlayer->AddDrinkEvent();
	}
	return 0;
}
INT32  CDrinkWineEvent::OnDestroy()
{
	return 0;
}
VOID CDrinkWineEvent::SetParam( UINT32 dwPlayerID )
{
	m_dwPlayerID = dwPlayerID;
}