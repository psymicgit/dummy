#ifndef _CTIMEREVENTMGR_H_
#define _CTIMEREVENTMGR_H_

#include <eventreactor.h>
#include <sdsingleton.h>
#include <sdhashmap.h>
#include "saveckofflineevent.h"
#include "saveckonlineevent.h"
#include "releasecaptureevent.h"
#include "rechargeevent.h"
#include "savepvpevent.h"
#include "chatevent.h"
#include "crossdayevent.h"
#include "yabiaoevent.h"
#include "noticeevent.h"
#include "bossbattleevent.h"
#include "gsnoticeevent.h"
#include "activityencourageevent.h"
#include "httpsyncevent.h"
#include "courageevent.h"
#include "givewineevent.h"
#include "drinkwine.h"
#include "huntingevent.h"
#include "couragerefreshevent.h"
#include "logic/gvg/global/gvgevent.h"

#define MAX_SAVE_ONLINE_EVENT_NUM		10000
#define MAX_SAVE_CK_OFFLINE_EVENT_NUM	10000
#define MAX_RELEASE_CAPTURE_EVENT_NUM	10000


typedef map<UINT64, CReleaseCaptureEvent*> CReleaseCaptureEventMap;
typedef CReleaseCaptureEventMap::iterator CReleaseCaptureEventMapItr;

typedef map<UINT64, CSaveCkOfflineEvent*> CSaveCkOfflineEventMap;
typedef CSaveCkOfflineEventMap::iterator CSaveCkOfflineEventMapItr;

typedef map<UINT64, CSaveOnlineEvent*> CSaveOnlineEventMap;
typedef CSaveOnlineEventMap::iterator CSaveOnlineEventMapItr;

typedef map<UINT32, CYabiaoEvent*> CYabiaoEventMap;
typedef CYabiaoEventMap::iterator CYabiaoEventMapItr;

typedef map<UINT16, CBossBattleEvent*> CBossBattleEventMap;
typedef CBossBattleEventMap::iterator CBossBattleEventMapItr;


typedef map<UINT16, CCourageEvent*> CCourageEventMap;
typedef CCourageEventMap::iterator CCourageEventMapItr;

typedef map<UINT32, CGiveWineEvent*> CGiveWineEventMap;
typedef CGiveWineEventMap::iterator  CGiveWineEventMapItr;

typedef map<UINT32, CDrinkWineEvent*> CDrinkWineEventMap;
typedef CDrinkWineEventMap::iterator  CDrinkWineEventMapItr;

typedef map<BOOL, CHuntingEvent*> CHuntingEventMap;
typedef CHuntingEventMap::iterator CHuntingEventMapItr;

//typedef map<UINT32, CCourageFireEvent*>CCourageFireEventMap;
//typedef CCourageFireEventMap::iterator CCourageFireEventMapItr;

/**
* @brief定时器事件管理器
*/
class CTimerEventMgr
{
    DECLARE_SINGLETON(CTimerEventMgr)

public:

	//初始化并且构建一些永驻系统事件
    BOOL            Init();

    BOOL            UnInit();

    /**
    * @brief 反应器的周期反应处理，处理所有添加到反应器中的事件对象
    */
	VOID            Run();

    // 清除所有的定时器
    VOID Clear();

public:	
	//

	CReleaseCaptureEvent* AddReleaseCaptureEvent(UINT32 dwOwnerID, UINT32 dwCaptureID, UINT32 dwInterval);
	BOOL DelReleaseCaptureEvent(UINT32 dwOwnerID, UINT32 dwCaptureID);

	CSaveCkOfflineEvent* AddSaveCkOfflineEvent(UINT32 dwPlayerID, UINT32 dwInterval);
	BOOL DelSaveCkOfflineEvent(UINT32 dwPlayerID);

	CSaveOnlineEvent* AddSaveOnlineEvent(UINT32 dwPlayerID, UINT32 dwInterval);
	BOOL DelSaveOnlineEvent(UINT32 dwPlayerID);

	VOID ResetCrossDayEventInterval();
	CYabiaoEvent *AddYabiaoEvent( UINT32 dwPlayerID, UINT32 dwInterval );
	BOOL DelYabiaoEvent( UINT32 dwPlayerID );

	CBossBattleEvent* AddBossBattleEvent(UINT8 byActivityID, BOOL bStartFlag);
	VOID ResetBossBattleEventInterval(CBossBattleEvent* poBossBattleEvent);

	CCourageEvent* AddCourageEvent(UINT8 byActivityID, BOOL bStartFlag);
	VOID ResetCourageEventInterval(CCourageEvent* poCourageEvent);

	CNoticeEvent* AddNoticeEventEvent(UINT8 byNoticeType, UINT32 dwStartTime, UINT32 dwEndTime, UINT32 dwStartEnterTime = 0);
	VOID ResetNotityEventInterval(CNoticeEvent* poNoticeEvent);

	CGiveWineEvent* AddGiveWineEvent(UINT32 dwFactionID, UINT32 dwInterval);
	VOID DelGiveWineEvent(UINT32 dwFactionID);

	CDrinkWineEvent* AddDrinkWineEvent(UINT32 dwPlayerID, UINT32 dwInterval);
	VOID DelDrinkWineEvent(UINT32 dwPlayerID);

	CHuntingEvent* AddHuntingEvent(BOOL bFlag);
	VOID ResetHuntingEvent(CHuntingEvent* poEvent);

    BOOL AddGvGEvent();
    void StopGvGEvent();

    VOID ResetGvGEventInterval(CGvGEvent* poNoticeEvent, UINT32 dwInterval);
    UINT32 GetGvGEventInterval();
    CGvGEvent* GetGvGEvent(){ return m_poGvGEvent; }

    CGvGReloadPropEvent* AddGvGReloadPropEvent();
    VOID ResetGvGReloadPropEventInterval(CGvGReloadPropEvent* poNoticeEvent, UINT32 dwInterval);

    CGvGRePullEvent* AddGvGRePullEvent();

protected:
    CTimerEventMgr();
    virtual ~CTimerEventMgr();

private:
    CGvGEvent *m_poGvGEvent;

private:
	CEventReactor<CSaveCkOfflineEvent>	m_oSaveCkOfflineEventReactor;		//保存/检测非登录玩家玩家数据事件反应器
	CSaveCkOfflineEventMap				m_mapSaveCkOfflineEvent;
	CEventReactor<CReleaseCaptureEvent> m_oReleaseCaptureEventReactor;		//释放俘虏反应器
	CReleaseCaptureEventMap			m_mapReleaseCaptureEvent;			
	CEventReactor<CSaveOnlineEvent>		m_oSaveOnlineEventReactor;			//保存登录玩家玩家数据事件反应器
	CSaveOnlineEventMap				m_mapSaveOnlineEvent;
	CEventReactor<CRechargeEvent>		m_oRechargeEventReactor;			//充值服务器事件
	CEventReactor<CSavePvpEvent>		m_oSavePvpEventReactor;			//保存pvp信息事件
	CEventReactor<CChatEvent>			m_oChatEventReactor;				//聊天事件
	CEventReactor<CCrossDayEvent>		m_oCrossDayEventReactor;			//跨天事件
	CCrossDayEvent*					m_poCrossDayEvent;				//跨天事件
	CEventReactor<CYabiaoEvent>		m_oYabiaoEventReactor;				//押镖事件
	CYabiaoEventMap					m_mapYabiaoEvent;					//押镖事件
	CEventReactor<CNoticeEvent>		m_oNoticeEventReactor;				//通知事件
	CEventReactor<CBossBattleEvent>		m_oBossBattleEvenReactor;			//BOSS战事件
	CBossBattleEventMap				m_mapBossBattleEvent;				//BOSS战事件,key为活动ID
	CEventReactor<CGetGsNoticeEvent>	m_oGsNoticeEventReactor;		//GS公告同步
	CEventReactor<CCActivityEncourageEvent> m_oActivityEncourageEventReactor;//活动配置同步事件
	CEventReactor<CHttpSyncEvent>			m_oHttpSyncEventReactor;//活动配置同步事件
	
	CEventReactor<CCourageEvent>		m_oCourageEvenReactor;			//勇气试炼事件
	CCourageEventMap					m_mapCourageEvent;			//勇气试炼事件,key为活动ID
	CEventReactor<CGiveWineEvent>	m_oGiveWineEventReactor;	//赐酒事件
	CGiveWineEventMap				m_mapGiveWineEvent;

	CEventReactor<CDrinkWineEvent> m_oDrinkWineEventReactor;	//喝酒
	CDrinkWineEventMap	m_mapDrinkWineEvent;

	CEventReactor<CHuntingEvent> m_oHuntingEventRector;	//捉妖事件
	CHuntingEventMap m_mapHuntingEvent;

    CEventReactor<CCourageRefreshEvent>			m_oCourageReEventReactor;				//勇气刷新事件

    CEventReactor<CGvGEvent>       m_oGvGEvenReactor;			    // 跨服战轮次更替定时器
    CEventReactor<CGvGReloadPropEvent> m_oGvGReloadPropEvenReactor;		    // 跨服战守护定时器
    CEventReactor<CGvGRePullEvent> m_oGvGRePullEvenReactor;			// 跨服战拉人定时器
};



#endif

