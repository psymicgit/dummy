#include "locale.h"
#include <net/db/dbpktbuilder.h>
#include <db/autosqlbuf.h>
#include <logic/player/player.h>
#include <common/client/errdef.h>

#include <sdloggerimpl.h>
#include <logic/instance/instance.h>

IMPLEMENT_CLASS_POOL_MGR(CLocale)
CLocale::CLocale()
{
	memset(&m_stDT_LOCALE_DATA, 0 ,sizeof(m_stDT_LOCALE_DATA));
}

CLocale::~CLocale()
{

}

BOOL CLocale::Init(DT_LOCALE_DATA &stDT_LOCALE_DATA, CPlayer* poOwner)
{
	if(NULL == poOwner)
	{
		return FALSE;
	}

	if((stDT_LOCALE_DATA.byLocaleType < EPL_HOME) || (stDT_LOCALE_DATA.byLocaleType > EPL_PET))
	{
		SYS_CRITICAL(_SDT("[%s: %d]: byLocaleType is invalid! byLocaleType[%d]"), MSG_MARK, stDT_LOCALE_DATA.byLocaleType);    		
		stDT_LOCALE_DATA.byLocaleType = EPL_HOME;
	}

	memcpy(&m_stDT_LOCALE_DATA, &stDT_LOCALE_DATA, sizeof(m_stDT_LOCALE_DATA));
	m_poOwner = poOwner;

	//若保存的位置信息是副本的，则回调告诉副本
	if(EPL_INSTANCE == m_stDT_LOCALE_DATA.byLocaleType)
	{
		SCurBattleInstance& stSCurBattleInstance =  m_poOwner->GetInstance().GetCurBattleInstance();
		DT_LOCALE_INSTANCE& stInstance = m_stDT_LOCALE_DATA.stLocaleInfo.stInstance;
		stSCurBattleInstance.wSceneIdx = stInstance.wSceneIdx;
		stSCurBattleInstance.byTownIdx = stInstance.byTownIdx;
		stSCurBattleInstance.byInstanceIdx = stInstance.byInstanceIdx;
		stSCurBattleInstance.byBattleIdx = stInstance.byBattleIdx;
		SInstanceProp* poInstanceProp = CInstancePropMgr::Instance()->GetInstanceProp(stSCurBattleInstance.wSceneIdx, stSCurBattleInstance.byTownIdx, stSCurBattleInstance.byInstanceIdx);
		if(NULL == poInstanceProp)
		{
			SYS_CRITICAL(_SDT("[%s: %d]: CInstance::EnterInstance poInstanceProp is NULL!"), MSG_MARK);    
			return TRUE;//跳过有问题数据
		}
		stSCurBattleInstance.byBattleNum = poInstanceProp->vecWholeMonsterProp.size();
		if(stSCurBattleInstance.byBattleIdx >= stSCurBattleInstance.byBattleNum)
		{
			stSCurBattleInstance.bPass = TRUE;
		}
		CInstanceRecord* poInstanceRecord =  m_poOwner->GetInstance().GetSingleInstaceRecord(stSCurBattleInstance.wSceneIdx, stSCurBattleInstance.byTownIdx, stSCurBattleInstance.byInstanceIdx);
		if((poInstanceRecord) && (stSCurBattleInstance.bPass) /*&& (1 == poInstanceRecord->GetDT_INSTANCE_DATA().wPassTimes)*/)
		{
			stSCurBattleInstance.bFirstPass = TRUE;
		}

		//进入副本,初始化数据
		m_poOwner->EnterInstance(stSCurBattleInstance.wSceneIdx, stSCurBattleInstance.byTownIdx, stSCurBattleInstance.byInstanceIdx, 0);
				
	}


	return TRUE;
}



BOOL CLocale::OnEnterInstance(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byBattleIdx)
{
	m_stDT_LOCALE_DATA.byLocaleType = EPL_INSTANCE;
	DT_LOCALE_INSTANCE& stInstance = m_stDT_LOCALE_DATA.stLocaleInfo.stInstance;
	stInstance.wSceneIdx = wSceneIdx;
	stInstance.byTownIdx = byTownIdx;
	stInstance.byInstanceIdx = byInstanceIdx;
	stInstance.byBattleIdx = byBattleIdx;//
	stInstance.bySuccessFlag = 0;

	//SetSaveState(ESS_WANT_SAVE);

	return TRUE;
}

BOOL CLocale::OnPassBattle(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx, UINT8 byBattleIdx)
{
	m_stDT_LOCALE_DATA.byLocaleType = EPL_INSTANCE;
	DT_LOCALE_INSTANCE& stInstance = m_stDT_LOCALE_DATA.stLocaleInfo.stInstance;
	stInstance.wSceneIdx = wSceneIdx;
	stInstance.byTownIdx = byTownIdx;
	stInstance.byInstanceIdx = byInstanceIdx;
	stInstance.byBattleIdx = byBattleIdx;//
	stInstance.bySuccessFlag = 1;

	//SetSaveState(ESS_WANT_SAVE);

	return TRUE;
}


BOOL CLocale::OnEnterHome()
{
	m_stDT_LOCALE_DATA.byLocaleType = EPL_HOME;
	memset(&m_stDT_LOCALE_DATA.stLocaleInfo, 0, sizeof(DT_LOCALE_DATA_UNION));

	//SetSaveState(ESS_WANT_SAVE);

	return TRUE;
}

BOOL CLocale::OnExitInstance()
{
	//退出副本直接回家园
	return OnEnterHome();
}

BOOL CLocale::OnOnhook(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx)
{	
	m_stDT_LOCALE_DATA.byLocaleType = EPL_ONHOOK;
	DT_LOCALE_ONHOOK& stOnhook = m_stDT_LOCALE_DATA.stLocaleInfo.stOnhook;
	stOnhook.wSceneIdx = wSceneIdx;
	stOnhook.byTownIdx = byTownIdx;
	stOnhook.byInstanceIdx = byInstanceIdx;

	//SetSaveState(ESS_WANT_SAVE);

	return TRUE;
}

BOOL CLocale::OnOnhookEnd()
{
	//挂机结束回家园
	m_stDT_LOCALE_DATA.byLocaleType = EPL_HOME;
	memset(&m_stDT_LOCALE_DATA.stLocaleInfo, 0, sizeof(DT_LOCALE_DATA_UNION));

	//SetSaveState(ESS_WANT_SAVE);

	return TRUE;
}


BOOL CLocale::OnEatBread()
{
	m_stDT_LOCALE_DATA.byLocaleType = EPL_EATBREAD;
	memset(&m_stDT_LOCALE_DATA.stLocaleInfo, 0, sizeof(DT_LOCALE_DATA_UNION));

	//SetSaveState(ESS_WANT_SAVE);

	return TRUE;	
}

BOOL CLocale::OnEatBreadEnd()
{
	//退出吃包子直接回家园
	return OnEnterHome();
}

BOOL CLocale::OnEnterBossB()
{
	m_stDT_LOCALE_DATA.byLocaleType = EPL_BOSSB;
	memset(&m_stDT_LOCALE_DATA.stLocaleInfo, 0, sizeof(DT_LOCALE_DATA_UNION));

	return TRUE;
}

BOOL CLocale::OnLeaveBossB()
{
	//退出吃包子直接回家园
	return OnEnterHome();
}

BOOL CLocale::OnOverBossB()
{
	//退出吃包子直接回家园
	return OnEnterHome();
}

BOOL CLocale::OnEnterHotSpring()
{
	m_stDT_LOCALE_DATA.byLocaleType = EPL_HOT_SPRING;
	memset(&m_stDT_LOCALE_DATA.stLocaleInfo, 0, sizeof(DT_LOCALE_DATA_UNION));

	return TRUE;
}
BOOL CLocale::OnLeaveHotSpring()
{
	return OnEnterFaction();
}

BOOL CLocale::OnEnterCourage()
{
    m_stDT_LOCALE_DATA.byLocaleType = EPL_COURAGE;
    memset(&m_stDT_LOCALE_DATA.stLocaleInfo, 0, sizeof(DT_LOCALE_DATA_UNION));

    return TRUE;
}

BOOL CLocale::OnLeaveCourage()
{
    return OnEnterHome();
}

BOOL CLocale::OnEnterFaction()
{
	m_stDT_LOCALE_DATA.byLocaleType = EPL_FACTION;
	memset(&m_stDT_LOCALE_DATA.stLocaleInfo, 0, sizeof(DT_LOCALE_DATA_UNION));

	return TRUE;
}

BOOL CLocale::OnLeaveFaction()
{
	return OnEnterHome();
}

BOOL CLocale::OnEnterCourageBattle(UINT16 wSceneIdx, UINT8 byTownIdx, UINT8 byInstanceIdx)
{
    m_stDT_LOCALE_DATA.byLocaleType = EPL_COURAGE_BATTLE;
    memset(&m_stDT_LOCALE_DATA.stLocaleInfo, 0, sizeof(DT_LOCALE_MARK));
    DT_LOCALE_MARK& stInstance = m_stDT_LOCALE_DATA.stLocaleInfo.stCourageBattle;
    stInstance.byExt = byTownIdx;
    return TRUE;
}

BOOL CLocale::OnLeaveCourageBattle()
{
    return OnEnterHome();
}

BOOL CLocale::OnEnterSoulRep() {
	m_stDT_LOCALE_DATA.byLocaleType = EPL_SOUL;
	memset(&m_stDT_LOCALE_DATA.stLocaleInfo, 0, sizeof(DT_LOCALE_DATA_UNION));
	return TRUE;
}

BOOL CLocale::OnLeaveSoulRep() {
	return OnEnterHome();
}

BOOL CLocale::OnEnterPetRep() {
	m_stDT_LOCALE_DATA.byLocaleType = EPL_PET;
	memset(&m_stDT_LOCALE_DATA.stLocaleInfo, 0, sizeof(DT_LOCALE_DATA_UNION));
	return TRUE;
}

BOOL CLocale::OnLeavePetRep() {
	return OnEnterHome();
}
