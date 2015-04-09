#include "user.h"
#include <framework/gtclient.h>
#include <net/cli/clipktbuilder.h>

CUser::CUser()
{
	m_poGTClient = NULL;

	memset(&m_aszDeviceID, 0, sizeof(m_aszDeviceID));
	memset(&m_aszNotifyID, 0, sizeof(m_aszNotifyID));
	memset(&m_aszUserName, 0, sizeof(m_aszUserName));

	m_wZoneID = 0;
    m_wOriZoneID = 0;
	m_dwUserID = 0;
	m_dwPlayerID = 0;
	m_wCareerID = 0;
	m_byAuthType = 0;

}

CUser::~CUser()
{
}


VOID CUser::SetUserProp(TCHAR* pszUserName, TCHAR* pszDeviceID, TCHAR* pszNotifyID, UINT16 wZoneID, UINT16 wOriZoneID, UINT32 dwUserID, UINT32 dwPlayerID, UINT16 wCareerID, UINT8 byAuthType, CGTClient* poGTClient, UINT8 byDaiChong)
{	
	ResetGTClient(poGTClient);

	if(pszUserName)
	{
		_SDTStrcpy(m_aszUserName, pszUserName);
	}
	if(pszDeviceID)
	{
		_SDTStrcpy(m_aszDeviceID, pszDeviceID);
	}
	if(pszNotifyID)
	{
		_SDTStrcpy(m_aszNotifyID, pszNotifyID);
	}		
    m_wZoneID = wZoneID;
	m_wOriZoneID = wOriZoneID;
	m_dwUserID = dwUserID;
	m_dwPlayerID = dwPlayerID;	
	m_wCareerID = wCareerID;
	m_byAuthType = byAuthType;
	m_byDaiChong = byDaiChong;
}

VOID CUser::ReleaseGtClient(CGTClient* poGTClient)
{
	//避免被踢新连接赋值或异步过程中新连接赋值
	if(poGTClient == m_poGTClient)
	{
		m_poGTClient = NULL;
	}
}

VOID CUser::ResetGTClient(CGTClient* poGTClient)
{	
	if(poGTClient == m_poGTClient)
	{
		return;
	}

	if(m_poGTClient)
	{
		m_poGTClient->SetState(CLIENT_STATE_ENTERING_GAME_KICK);
		UINT16 wMsgID = CCliPktBuilder::Instance()->KickOutNtf(SAME_ACCOUNT_LOGIN);			
		m_poGTClient->SendMsg(wMsgID, CCliPktBuilder::Instance()->GetPacketBuffer());
		m_poGTClient->Disconnect();//将之前玩家踢下线，重置新连接
	}
	m_poGTClient = poGTClient;
}

UINT32 CUser::GetRemoteIP()  
{ 
	if(m_poGTClient) 
	{
		return m_poGTClient->GetRemoteIP(); 
	}

	return 0; 
}