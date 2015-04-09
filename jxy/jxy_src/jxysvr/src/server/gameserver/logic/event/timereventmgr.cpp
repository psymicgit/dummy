#include "timereventmgr.h"
#include <framework/gsconfig.h>
#include <logic/player/player.h>
#include <common/server/utility.h>
#include <logic/base/logicfunc.h>


IMPLEMENT_SINGLETON(CTimerEventMgr)

CTimerEventMgr::CTimerEventMgr():
    m_oSaveOnlineEventReactor(MAX_SAVE_ONLINE_EVENT_NUM, EXECUTE_LOOSE),
    m_oSaveCkOfflineEventReactor(MAX_SAVE_CK_OFFLINE_EVENT_NUM, EXECUTE_LOOSE),
    m_oReleaseCaptureEventReactor(MAX_RELEASE_CAPTURE_EVENT_NUM, EXECUTE_STRICT),
    m_oRechargeEventReactor(1, EXECUTE_STRICT ),
    m_oSavePvpEventReactor(1, EXECUTE_LOOSE),
    m_oChatEventReactor(1, EXECUTE_LOOSE),
    m_oCrossDayEventReactor(1, EXECUTE_LOOSE),
    m_oYabiaoEventReactor( 1, EXECUTE_STRICT ),
    m_oBossBattleEvenReactor( 1, EXECUTE_STRICT ),
    m_oGsNoticeEventReactor(1, EXECUTE_LOOSE),
    m_oActivityEncourageEventReactor( 1, EXECUTE_STRICT ),
    m_oNoticeEventReactor(1, EXECUTE_LOOSE),
    m_oHttpSyncEventReactor(1, EXECUTE_LOOSE),
	m_oGiveWineEventReactor(1, EXECUTE_LOOSE),
    m_oCourageEvenReactor(1, EXECUTE_LOOSE),
	m_oDrinkWineEventReactor(1, EXECUTE_LOOSE),
	m_oHuntingEventRector(1, EXECUTE_LOOSE),
    m_oCourageReEventReactor(1, EXECUTE_LOOSE),
	m_oGvGEvenReactor(1, EXECUTE_STRICT),
    m_oGvGReloadPropEvenReactor(1, EXECUTE_LOOSE),
    m_oGvGRePullEvenReactor(1, EXECUTE_LOOSE)
{
    m_poCrossDayEvent = NULL;
    m_poGvGEvent      = NULL;
}


CTimerEventMgr::~CTimerEventMgr()
{
	m_poCrossDayEvent = NULL;
}

BOOL CTimerEventMgr::Init()
{
    CRechargeEvent *pEvent = m_oRechargeEventReactor.CreateEvent();
    m_oRechargeEventReactor.SetEvent( pEvent, pEvent->GetSendHeartInterval() );

    CSavePvpEvent* poPvpEvent = m_oSavePvpEventReactor.CreateEvent();
    m_oSavePvpEventReactor.SetEvent(poPvpEvent, CGSConfig::Instance()->GetPvpSaveCkInterval());

    CChatEvent* poChatEvent = m_oChatEventReactor.CreateEvent();
    m_oChatEventReactor.SetEvent(poChatEvent, poChatEvent->GetInterval());

    CCActivityEncourageEvent* poActivityPropEvent = m_oActivityEncourageEventReactor.CreateEvent();
    m_oActivityEncourageEventReactor.SetEvent(poActivityPropEvent, poActivityPropEvent->GetInterval());


    CHttpSyncEvent* poHttpSyncEvent = m_oHttpSyncEventReactor.CreateEvent();
    m_oHttpSyncEventReactor.SetEvent(poHttpSyncEvent, poHttpSyncEvent->GetInterval());


    m_poCrossDayEvent = m_oCrossDayEventReactor.CreateEvent();
    m_oCrossDayEventReactor.SetEvent(m_poCrossDayEvent, m_poCrossDayEvent->GetInterval());

    CGetGsNoticeEvent* poGsNoticeEvent = m_oGsNoticeEventReactor.CreateEvent();
    m_oGsNoticeEventReactor.SetEvent(poGsNoticeEvent, poGsNoticeEvent->GetInterval());

    CCourageRefreshEvent* poCourageRefreshEvent = m_oCourageReEventReactor.CreateEvent();
    m_oCourageReEventReactor.SetEvent(poCourageRefreshEvent, poCourageRefreshEvent->GetInterval());

    return TRUE;
}


BOOL CTimerEventMgr::UnInit()
{

    return TRUE;
}

VOID CTimerEventMgr::Run()
{
    //AUTOCYCLE;

    m_oSaveCkOfflineEventReactor.Run();
    m_oSaveOnlineEventReactor.Run();
    m_oReleaseCaptureEventReactor.Run();
    m_oRechargeEventReactor.Run();
    m_oSavePvpEventReactor.Run();
    m_oChatEventReactor.Run();
    m_oCrossDayEventReactor.Run();
    m_oYabiaoEventReactor.Run();
    m_oNoticeEventReactor.Run();
    m_oBossBattleEvenReactor.Run();
    m_oGsNoticeEventReactor.Run();
    m_oActivityEncourageEventReactor.Run();
    m_oHttpSyncEventReactor.Run();
	m_oGiveWineEventReactor.Run();
	m_oCourageEvenReactor.Run();
	m_oDrinkWineEventReactor.Run();
	m_oHuntingEventRector.Run();
    m_oCourageReEventReactor.Run();
    m_oGvGEvenReactor.Run();
    m_oGvGReloadPropEvenReactor.Run();
	m_oGvGRePullEvenReactor.Run();
}

VOID CTimerEventMgr::Clear()
{
    m_oSaveCkOfflineEventReactor.Clear();
    m_oSaveOnlineEventReactor.Clear();
    m_oReleaseCaptureEventReactor.Clear();
    m_oRechargeEventReactor.Clear();
    m_oSavePvpEventReactor.Clear();
    m_oChatEventReactor.Clear();
    m_oCrossDayEventReactor.Clear();
    m_oYabiaoEventReactor.Clear();
    m_oNoticeEventReactor.Clear();
    m_oBossBattleEvenReactor.Clear();
    m_oGsNoticeEventReactor.Clear();
    m_oActivityEncourageEventReactor.Clear();
    m_oHttpSyncEventReactor.Clear();
    m_oGiveWineEventReactor.Clear();
    m_oCourageEvenReactor.Clear();
    m_oDrinkWineEventReactor.Clear();
    m_oHuntingEventRector.Clear();
    m_oGvGEvenReactor.Clear();
    m_oGvGReloadPropEvenReactor.Clear();
    m_oGvGRePullEvenReactor.Clear();
}

CReleaseCaptureEvent* CTimerEventMgr::AddReleaseCaptureEvent(UINT32 dwOwnerID, UINT32 dwCaptureID, UINT32 dwInterval)
{
    CReleaseCaptureEventMapItr itr = m_mapReleaseCaptureEvent.find(CLogicFunc::Get3232Key(dwOwnerID, dwCaptureID));
    //已经存在，直接返回
    if(itr != m_mapReleaseCaptureEvent.end())
    {
        return itr->second;
    }

    CReleaseCaptureEvent* poEvent = m_oReleaseCaptureEventReactor.CreateEvent();
    if(poEvent)
    {
        poEvent->SetParam(dwOwnerID, dwCaptureID);
        m_oReleaseCaptureEventReactor.SetEvent(poEvent, dwInterval);

        m_mapReleaseCaptureEvent[CLogicFunc::Get3232Key(dwOwnerID, dwCaptureID)] = poEvent;
    }

    return poEvent;
}

BOOL CTimerEventMgr::DelReleaseCaptureEvent(UINT32 dwOwnerID, UINT32 dwCaptureID)
{
    CReleaseCaptureEventMapItr itr = m_mapReleaseCaptureEvent.find(CLogicFunc::Get3232Key(dwOwnerID, dwCaptureID));
    if(itr != m_mapReleaseCaptureEvent.end())
    {
        CReleaseCaptureEvent* poEvent = itr->second;
        if(NULL == poEvent)
        {
            return FALSE;
        }
        m_oReleaseCaptureEventReactor.DestroyEvent(poEvent);
        m_mapReleaseCaptureEvent.erase(itr);
        return TRUE;
    }

    return FALSE;
}


CSaveCkOfflineEvent* CTimerEventMgr::AddSaveCkOfflineEvent(UINT32 dwPlayerID, UINT32 dwInterval)
{
    return NULL;

    CSaveCkOfflineEventMapItr itr = m_mapSaveCkOfflineEvent.find(dwPlayerID);
    //已经存在，直接返回
    if(itr != m_mapSaveCkOfflineEvent.end())
    {
        return itr->second;
    }

    CSaveCkOfflineEvent* poEvent = m_oSaveCkOfflineEventReactor.CreateEvent();
    if(poEvent)
    {
        poEvent->SetParam(dwPlayerID);
        m_oSaveCkOfflineEventReactor.SetEvent(poEvent, dwInterval);

        m_mapSaveCkOfflineEvent[dwPlayerID] = poEvent;
    }

    return poEvent;
}

BOOL CTimerEventMgr::DelSaveCkOfflineEvent(UINT32 dwPlayerID)
{
    return FALSE;

    CSaveCkOfflineEventMapItr itr = m_mapSaveCkOfflineEvent.find(dwPlayerID);
    if(itr != m_mapSaveCkOfflineEvent.end())
    {
        CSaveCkOfflineEvent* poEvent = itr->second;
        if(NULL == poEvent)
        {
            return FALSE;
        }
        m_oSaveCkOfflineEventReactor.DestroyEvent(poEvent);
        m_mapSaveCkOfflineEvent.erase(itr);
        return TRUE;
    }

    return FALSE;
}


CSaveOnlineEvent* CTimerEventMgr::AddSaveOnlineEvent(UINT32 dwPlayerID, UINT32 dwInterval)
{
    return NULL;

    CSaveOnlineEventMapItr itr = m_mapSaveOnlineEvent.find(dwPlayerID);
    //已经存在，直接返回
    if(itr != m_mapSaveOnlineEvent.end())
    {
        return itr->second;
    }

    CSaveOnlineEvent* poEvent = m_oSaveOnlineEventReactor.CreateEvent();
    if(poEvent)
    {
        poEvent->SetParam(dwPlayerID);
        m_oSaveOnlineEventReactor.SetEvent(poEvent, dwInterval);

        m_mapSaveOnlineEvent[dwPlayerID] = poEvent;
    }

    return poEvent;
}

BOOL CTimerEventMgr::DelSaveOnlineEvent(UINT32 dwPlayerID)
{
    return FALSE;

    CSaveOnlineEventMapItr itr = m_mapSaveOnlineEvent.find(dwPlayerID);
    if(itr != m_mapSaveOnlineEvent.end())
    {
        CSaveOnlineEvent* poEvent = itr->second;
        if(NULL == poEvent)
        {
            return FALSE;
        }
        m_oSaveOnlineEventReactor.DestroyEvent(poEvent);
        m_mapSaveOnlineEvent.erase(itr);
        return TRUE;
    }

    return FALSE;
}

CYabiaoEvent *CTimerEventMgr::AddYabiaoEvent( UINT32 dwPlayerID, UINT32 dwInterval )
{
    CYabiaoEventMapItr itr = m_mapYabiaoEvent.find( dwPlayerID );
    if( itr != m_mapYabiaoEvent.end() )
    {
        USR_INFO( _SDT( "[%s: %d]Re AddYabiaoEvent，playerid:%u" ), MSG_MARK, dwPlayerID );
        return itr->second;
    }

    CYabiaoEvent* poEvent = m_oYabiaoEventReactor.CreateEvent();

    if( NULL == poEvent )
    {
        SYS_CRITICAL( _SDT( "[%s: %d]poEvent is NULL, player:%d" ), MSG_MARK, dwPlayerID );
        return NULL;
    }

    poEvent->SetParam( dwPlayerID );
    m_oYabiaoEventReactor.SetEvent( poEvent, dwInterval );
    m_mapYabiaoEvent[dwPlayerID] = poEvent;
    //DBG_INFO( _SDT( "[%s: %d]Create yabiao timer,player:%d interval:%d" ), MSG_MARK, dwPlayerID, dwInterval );

    return poEvent;
}

BOOL CTimerEventMgr::DelYabiaoEvent( UINT32 dwPlayerID )
{
    CYabiaoEventMapItr itr = m_mapYabiaoEvent.find( dwPlayerID );
    if( itr == m_mapYabiaoEvent.end() )
    {
        USR_INFO( _SDT( "[%s: %d]can't find player on delete yabiao event player:%d" ), MSG_MARK, dwPlayerID );
        return FALSE;
    }

    CYabiaoEvent *poEvent = itr->second;
    if( NULL == poEvent )
    {
        SYS_CRITICAL( _SDT( "[%s: %d]poEvent is NULL,player:%d" ), MSG_MARK, dwPlayerID );
        return FALSE;
    }
    m_oYabiaoEventReactor.DestroyEvent( poEvent );
    m_mapYabiaoEvent.erase( itr );
    //DBG_INFO( _SDT( "[%s: %d]delete yabiao timer,player:%d" ), MSG_MARK, dwPlayerID );
    return TRUE;
}


VOID CTimerEventMgr::ResetCrossDayEventInterval()
{
    m_oCrossDayEventReactor.ResetInterval(m_poCrossDayEvent, m_poCrossDayEvent->GetInterval());
}

CBossBattleEvent* CTimerEventMgr::AddBossBattleEvent(UINT8 byActivityID, BOOL bStartFlag)
{
    UINT16 wKey = CLogicFunc::Get88Key(bStartFlag, byActivityID);
    CBossBattleEventMapItr itr = m_mapBossBattleEvent.find(wKey);
    //已经存在，直接返回
    if(itr != m_mapBossBattleEvent.end())
    {
        return itr->second;
    }

    CBossBattleEvent* poEvent = m_oBossBattleEvenReactor.CreateEvent();
    if(poEvent)
    {
        poEvent->SetParam(bStartFlag, byActivityID);
        m_oBossBattleEvenReactor.SetEvent(poEvent, poEvent->GetInterval());

        m_mapBossBattleEvent[wKey] = poEvent;
    }

    return poEvent;
}


VOID CTimerEventMgr::ResetBossBattleEventInterval(CBossBattleEvent* poBossBattleEvent)
{
    m_oBossBattleEvenReactor.ResetInterval(poBossBattleEvent, poBossBattleEvent->GetInterval());
}


CNoticeEvent* CTimerEventMgr::AddNoticeEventEvent(UINT8 byNoticeType, UINT32 dwStartTime, UINT32 dwEndTime, UINT32 dwStartEnterTime)
{
    CNoticeEvent* poEvent = m_oNoticeEventReactor.CreateEvent();
    if(poEvent)
    {
        poEvent->SetParam(byNoticeType, dwStartTime, dwEndTime, dwStartEnterTime);
        m_oNoticeEventReactor.SetEvent(poEvent, poEvent->GetInterval());
    }
    return poEvent;
}

VOID CTimerEventMgr::ResetNotityEventInterval(CNoticeEvent* poNoticeEvent)
{
    m_oNoticeEventReactor.ResetInterval(poNoticeEvent, poNoticeEvent->GetInterval());
}

CCourageEvent* CTimerEventMgr::AddCourageEvent(UINT8 byActivityID, BOOL bStartFlag)
{
    UINT16 wKey = CLogicFunc::Get88Key(bStartFlag, byActivityID);
    CCourageEventMapItr itr = m_mapCourageEvent.find(wKey);
    //已经存在，直接返回
    if(itr != m_mapCourageEvent.end())
    {
        return itr->second;
    }

    CCourageEvent* poEvent = m_oCourageEvenReactor.CreateEvent();
    if(poEvent)
    {
        poEvent->SetParam(bStartFlag, byActivityID);
        m_oCourageEvenReactor.SetEvent(poEvent, poEvent->GetInterval());
        m_mapCourageEvent[wKey] = poEvent;
    }
    return poEvent;
}

VOID CTimerEventMgr::ResetCourageEventInterval(CCourageEvent* poCourageEvent)
{
    m_oCourageEvenReactor.ResetInterval(poCourageEvent, poCourageEvent->GetInterval());
}

CGiveWineEvent* CTimerEventMgr::AddGiveWineEvent(UINT32 dwFactionID, UINT32 dwInterval)
{
	if ( dwFactionID == 0)
	{
		return NULL;
	}
	CGiveWineEvent* poEvent = m_oGiveWineEventReactor.CreateEvent();
	if(poEvent)
	{
		poEvent->SetParam(dwFactionID);
		m_oGiveWineEventReactor.SetEvent(poEvent, dwInterval);
		m_mapGiveWineEvent[dwFactionID] = poEvent;
	}
	return poEvent;
}

VOID CTimerEventMgr::DelGiveWineEvent(UINT32 dwFactionID)
{
	CGiveWineEventMapItr itr = m_mapGiveWineEvent.find(dwFactionID);
	if ( itr != m_mapGiveWineEvent.end())
	{
		m_oGiveWineEventReactor.DestroyEvent( itr->second );
		m_mapGiveWineEvent.erase(dwFactionID);
	}
	
}

CDrinkWineEvent* CTimerEventMgr::AddDrinkWineEvent(UINT32 dwPlayerID, UINT32 dwInterval)
{
	if ( dwPlayerID == 0)
	{
		return NULL;
	}
	CDrinkWineEventMapItr itr = m_mapDrinkWineEvent.find(dwPlayerID);
	if ( itr != m_mapDrinkWineEvent.end())
	{
		return itr->second;
	}
	CDrinkWineEvent* poEvent = m_oDrinkWineEventReactor.CreateEvent();
	if(poEvent)
	{
		poEvent->SetParam(dwPlayerID);
		m_oDrinkWineEventReactor.SetEvent(poEvent, dwInterval);
		m_mapDrinkWineEvent[dwPlayerID] = poEvent;
	}
	return poEvent;
}

VOID CTimerEventMgr::DelDrinkWineEvent(UINT32 dwPlayerID)
{
	CDrinkWineEventMapItr itr = m_mapDrinkWineEvent.find(dwPlayerID);
	if ( itr != m_mapDrinkWineEvent.end())
	{
		m_oDrinkWineEventReactor.DestroyEvent( itr->second );
		m_mapDrinkWineEvent.erase(dwPlayerID);
	}
}

CHuntingEvent* CTimerEventMgr::AddHuntingEvent(BOOL bRet)
{
	CHuntingEventMapItr itr = m_mapHuntingEvent.find(bRet);
	if (itr != m_mapHuntingEvent.end())
	{
		return itr->second;
	}

	CHuntingEvent* poEvent = m_oHuntingEventRector.CreateEvent();
	if (poEvent)
	{
		poEvent->SetParam(bRet);
		m_oHuntingEventRector.SetEvent(poEvent, poEvent->GetInterval());
		m_mapHuntingEvent[bRet] = poEvent;
	}
	return poEvent;
}

VOID CTimerEventMgr::ResetHuntingEvent(CHuntingEvent* poEvent)
{
	 m_oHuntingEventRector.ResetInterval(poEvent, poEvent->GetInterval());
}

BOOL CTimerEventMgr::AddGvGEvent()
{
    if(m_poGvGEvent)
    {
        return TRUE;
    }

    m_poGvGEvent = m_oGvGEvenReactor.CreateEvent();
    if(NULL == m_poGvGEvent)
    {
        return FALSE;
    }

    UINT32 dwInterval = m_poGvGEvent->GetInterval();
    m_oGvGEvenReactor.SetEvent(m_poGvGEvent, dwInterval);
    USR_INFO(_SDT("[%s: %d]: CTimerEventMgr::AddGvGEvent interval to next round = %s"), MSG_MARK, gvgutil::TimeToDayHourMinuteSec(dwInterval).c_str());
    return TRUE;
}

void CTimerEventMgr::StopGvGEvent()
{
    if(NULL == m_poGvGEvent)
    {
        return;
    }

    m_oGvGEvenReactor.DestroyEvent(m_poGvGEvent);
    m_poGvGEvent = NULL;
}

CGvGReloadPropEvent* CTimerEventMgr::AddGvGReloadPropEvent()
{
    CGvGReloadPropEvent* poEvent = m_oGvGReloadPropEvenReactor.CreateEvent();
    if(NULL == poEvent)
    {
        return NULL;
    }

    m_oGvGReloadPropEvenReactor.SetEvent(poEvent, poEvent->GetInterval());
    return poEvent;
}

VOID CTimerEventMgr::ResetGvGEventInterval(CGvGEvent* poEvent, UINT32 dwInterval)
{
    m_oGvGEvenReactor.ResetInterval(poEvent, dwInterval);
}

UINT32 CTimerEventMgr::GetGvGEventInterval()
{
    if(NULL == m_poGvGEvent)
    {
        return 0;
    }

    return m_poGvGEvent->GetInterval();;
}

VOID CTimerEventMgr::ResetGvGReloadPropEventInterval(CGvGReloadPropEvent* poEvent, UINT32 dwInterval)
{
    m_oGvGReloadPropEvenReactor.ResetInterval(poEvent, dwInterval);
}

CGvGRePullEvent* CTimerEventMgr::AddGvGRePullEvent()
{
    CGvGRePullEvent* poGvGRePullEvent = m_oGvGRePullEvenReactor.CreateEvent();
    m_oGvGRePullEvenReactor.SetEvent(poGvGRePullEvent, poGvGRePullEvent->GetInterval());

    return poGvGRePullEvent;
}
