
#ifndef _ONHOOK_H_
#define _ONHOOK_H_

#include <sdhashmap.h>
#include <protocol/server/protocommondata.h>
#include <logic/base/baseobj.h>
#include <logic/instance/instancepropmgr.h>
#include <common/server/utility.h>

#include <list>
using namespace std;
using namespace std ;

typedef HashMap<UINT16, DT_ITEM_DATA*> CKindID2ItemDataMap;//key为道具ID
typedef CKindID2ItemDataMap::iterator CKindID2ItemDataMapItr;

//挂机时武将升级信息
struct SOnhookHeroUpgrage
{
	SOnhookHeroUpgrage() { bHaveUpgrageFlag = FALSE; memset(&stDT_HERO_UPGRADE_DATA2, 0, sizeof(stDT_HERO_UPGRADE_DATA2)); }
	BOOL bHaveUpgrageFlag;//是否升级过
	DT_HERO_UPGRADE_DATA2 stDT_HERO_UPGRADE_DATA2;
};


typedef HashMap<UINT16, SOnhookHeroUpgrage> CKindID2HeroDataMap;//key为武将ID
typedef CKindID2HeroDataMap::iterator CKindID2HeroDataMapItr;


//typedef list<DT_ONHOOK_RESULT> COnHookResultList;//key为武将ID
//typedef COnHookResultList::iterator COnHookResultListItr;


enum em_OnHook
{
	em_COMMON = 0,
	em_TOWER = 1,
};


class CPlayer;
class COnhook:public CBaseObj
{
public:
	COnhook();
	~COnhook();
public:
	//初始化相关对象
	BOOL Init(DT_ONHOOK_RCD_DATA& stOnhookRcdData, CPlayer* poOwner);
	DT_ONHOOK_RCD_DATA& GetDT_ONHOOK_RCD_DATA() { return m_stOnhookRcdData; }
	/////////////////////////////
	VOID SetOnHookParam(UINT8 byRunTimes, DT_INSTANCE_ID* pstDT_INSTANCE_ID);
	//UINT8 GetOhhookOddRunTimes() { return m_stOnhookRcdData.stOnhookResult.byAllRunTimes - m_stOnhookRcdData.stOnhookResult.byCurRunTimes; }
	//VOID GetDT_ONHOOK_RESULT(DT_ONHOOK_RESULT& stDT_ONHOOK_RESULT) {  memcpy(&stDT_ONHOOK_RESULT, &m_stOnhookRcdData.stOnhookResult, sizeof(DT_ONHOOK_RESULT)); }
	VOID GetOnhookResult(DT_ONHOOK_RESULT& stDT_ONHOOK_RESULT, DT_DATA_AFTER_ONHOOK& stAfterOnhookData);//获取当前挂机结果
	BOOL OnHookEncourage(UINT8 &byRunTimes);//副本挂机奖励回调，跑一次副本回调一次.背包满返回TRUE
	VOID GetOnhookInstanceID(DT_INSTANCE_ID& stOnhookInstanceID) { memcpy(&stOnhookInstanceID, &m_stOnhookRcdData.stOnhookInstanceID, sizeof(DT_INSTANCE_ID)); }
	UINT64 GetLastOnhookResultReqTime() { return m_stOnhookRcdData.qwLastOnhookResultReqTime; }
	BOOL OnhookFlag() { return (1 == m_stOnhookRcdData.byOnhookFlag); }
	UINT16& GetLastOnhookResultReqPassSec() { return m_wLastOnhookResultReqPassSec; }
	BOOL GetPause(){ return m_bPause; }
	VOID SetPause(BOOL bPause) { m_bPause = bPause; }

protected:
	BOOL GetHookEncourage(UINT8 &byRunTimes);//背包满返回TRUE
	VOID InitHeroMonster();
private:
	DT_ONHOOK_RCD_DATA			m_stOnhookRcdData;
	vector<SWholeMonsterProp*>*	pvecWholeMonsterProp;//挂机副本怪物信息
	CKindID2ItemDataMap			m_mapOnhookEncourageItem;//挂机奖励道具，用于管理
	CKindID2HeroDataMap			m_mapOnhookUpgrageHero;	 //挂机中升级武将信息
	UINT16						m_wLastOnhookResultReqPassSec;//上次请求挂机结果时扫荡过去的时间
	BOOL							m_bPause;//是否暂停
	//COnHookResultList				m_listOnHookResultList;
};

DECLARE_CLASS_POOL_MGR(COnhook)
#endif //#ifndef 

